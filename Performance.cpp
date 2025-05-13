#include "Performance.h"
#include "Classes.h"
#include <fstream>
#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <chrono>
#include <ctime>

#undef main

extern vector<getInformation> old;

SDL_Window *win = nullptr; // 窗口初始化 
SDL_Renderer *rdr = nullptr; // 渲染器初始化
TTF_Font *font = nullptr;    // 字体初始化
SDL_Surface *textSurface = nullptr;
SDL_Texture *textTexture = nullptr;
std::vector<std::unique_ptr<Tetromino>> SHAPE; // 图形储存
int scores = 0; // 得分统计
int grid[20][10] = {0}; // 游戏区域的网格（20行 x 10列）
int difficulty = 1; // 难度选择
vector<getInformation> old;

void Init()
{
    // 初始化
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("Init failed: %s", SDL_GetError());
        return;
    }
    // 生成窗口
    win = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, 400, 800, SDL_WINDOW_RESIZABLE);
    if (nullptr == win)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return;
    }

    rdr = SDL_CreateRenderer(win, -1, 0);
    if (nullptr == rdr)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        return;
    }

    // 文字初始化
    if (TTF_Init() < 0)
    {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return;
    }
    font = TTF_OpenFont("C:\\Windows\\Fonts\\JetBrainsMono-Regular.ttf", 15);
    if (nullptr == font)
    {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
        return;
    }

    // 初始化时直接得到option.txt里的信息,便于后续修改游戏
    old = optionInformation();
}

// 新图形的创建
Tetromino *createRandomTetromino(int x, int y)
{
    static std::default_random_engine engine(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_int_distribution<int> dist(0, 6); // 对应 7 个 Type 枚举

    int randomType = dist(engine);
    switch (randomType)
    {
    case Tetromino::I:
        return new Tetromino_I(x, y);
    case Tetromino::J:
        return new Tetromino_J(x, y);
    case Tetromino::L:
        return new Tetromino_L(x, y);
    case Tetromino::O:
        return new Tetromino_O(x, y);
    case Tetromino::S:
        return new Tetromino_S(x, y);
    case Tetromino::T:
        return new Tetromino_T(x, y);
    case Tetromino::Z:
        return new Tetromino_Z(x, y);
    default:
        return nullptr; 
    }
}

void event_loop()
{
    // 随机数生成器，用来随机位置生成图形
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 7);
    Tetromino *currentPiece = createRandomTetromino(dis(gen) * 40, 0);
    // 初始化事件
    SDL_Event event;
    // 检测游戏是否结束
    bool gameover = false;
    Uint32 lastFallTime = SDL_GetTicks();
    Uint32 baseFallInterval = 500;
    bool is_stop = 0; // 是否暂停

    while (!gameover)
    {
        if (old.size() != 1) // 根据从option.txt读取的速度来单独修改下降速度
        {
            if (old[currentPiece->type].v != 0)
            {
                baseFallInterval = old[currentPiece->type].v; // 根据文件来修改下落速度
            }
        }
        bool is_pressed = false; // 是否按下键盘
        bool is_pressed_down = false; // 是否按下了方向下键
        bool is_quit = false; // esc键退出
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                gameover = true;
                break;
            case SDL_KEYDOWN:
                is_pressed = true;
                switch (event.key.keysym.sym) // 按键检测
                {
                case SDLK_KP_1: // 三种难度选择
                    difficulty = 1;
                    break;
                case SDLK_KP_2 : 
                    difficulty = 2;
                    break;
                case SDLK_KP_3:
                    difficulty = 3;
                    break;
                case SDLK_ESCAPE: // 退出
                    gameover = true;
                    break;
                case SDLK_SPACE: // 暂停
                    is_stop = !is_stop;
                    break;
                case SDLK_LEFT: // 左移
                {
                    --(*currentPiece);
                    break;
                }
                case SDLK_RIGHT: // 右移
                {
                    ++(*currentPiece);
                    break;
                }
                case SDLK_UP: // 旋转
                {
                    rotate(*currentPiece);
                    break;
                }
                case SDLK_DOWN: // 快速下降
                    is_pressed_down = true;
                    break;
                default:
                    std::cout << "当前按键无效,请重新输入！" << std::endl;
                    break;
                }
            default:
                break;
            }
        }

        if(!is_stop){
            Uint32 fallInterval = baseFallInterval / difficulty; // 难度选择
            Uint32 currentTime = SDL_GetTicks();

            if (currentTime - lastFallTime >= fallInterval || is_pressed_down)
            {
                Tetromino temp = *currentPiece;
                temp.y += 40;

                if (checkCollision(&temp))
                {
                    for (int i = 0; i < currentPiece->shape_array.size(); ++i)
                    {
                        for (int j = 0; j < currentPiece->shape_array[i].size(); ++j)
                        {
                            if (currentPiece->shape_array[i][j])
                            {
                                int gridX = (currentPiece->x + j * 40) / 40;
                                int gridY = (currentPiece->y + i * 40) / 40;
                                if (gridY >= 0 && gridX >= 0 && gridX < 10)
                                {
                                    // 填充方格位置标记，便于后边检测
                                    grid[gridY][gridX] = 1;
                                    Tetromino cell{*currentPiece};
                                    cell.x = currentPiece->x + j * 40;
                                    cell.y = currentPiece->y + i * 40;
                                    cell.shape_array = {{1}};
                                    // 储存原图形
                                    SHAPE.push_back(std::make_unique<Tetromino>(cell));
                                }
                            }
                        }
                    }
                    delete currentPiece;

                    // findeFullRows()函数填充了被装满的行
                    vector<int> fullRows = findFullRows();
                    // 如果有满行
                    if (!fullRows.empty())
                    {
                        // 更新方格标记
                        updateGrid(fullRows);
                        // 更新游戏界面
                        updateShapes(SHAPE, fullRows);
                    }

                    currentPiece = createRandomTetromino(dis(gen) * 40, 0);
                    if (checkCollision(currentPiece))
                    {
                        gameover = true; 
                    }
                    lastFallTime = currentTime;
                }
                else
                {
                    currentPiece->y += 40;
                    lastFallTime = currentTime;
                    if (is_pressed_down)
                    {
                        // 到达底部
                        while (!checkCollision(&temp))
                        {
                            currentPiece->y += 40;
                            temp.y += 40;
                        }
                        currentPiece->y -= 40; 
                        lastFallTime = currentTime - fallInterval;
                    }
                }
            }

            // 黑色背景
            SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
            // 清楚画面
            SDL_RenderClear(rdr);
            // 绘画出已到达底部图形
            for (const auto &shape : SHAPE)
            {
                std::cout << shape;
            }
            // 绘画出当前图像
            std::cout << currentPiece;

            presentOldHighestScores(); // 屏幕打印历史最高分
            presentCurrentScores();    // 屏幕打印当前得分

            // 渲染呈现
            SDL_RenderPresent(rdr);

            SDL_Delay(10);
        }
    }
}

void destory()
{
    recordInformation(); // 在游戏结束时收集必要信息
    SHAPE.clear();
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void edge_judge(Tetromino *currentPiece, bool is_pressed_down)
{
    int blockSize = 40;
    int cols = currentPiece->shape_array[0].size();
    int rows = currentPiece->shape_array.size();
    int maxX = (10 - cols) * blockSize; // 10列游戏区域
    int maxY = (20 - rows) * blockSize; // 20行游戏区域

    // 左右移动边界检测
    if (dx == 40 && currentPiece->x <= maxX)
        currentPiece->x += dx;
    else if (dx == -40 && currentPiece->x >= 0)
        currentPiece->x += dx;

    // 自动下落检测
    if (currentPiece->y < maxY)
        currentPiece->y += dy;

    // 旋转后位置修正
    if (currentPiece->x > maxX)
    {
        currentPiece->x = maxX; // 强制修正到右边界
    }
    if (currentPiece->x < 0)
    {
        currentPiece->x = 0; // 强制修正到左边界
    }
    if(is_pressed_down)
    {
        currentPiece->y = maxY;
    }
}

bool checkCollision(Tetromino *currentPiece)
{
    const int blockSize = 40;
    const int gridWidth = 10;
    const int gridHeight = 20;

    for (int i = 0; i < currentPiece->shape_array.size(); ++i)
    {
        for (int j = 0; j < currentPiece->shape_array[i].size(); ++j)
        {
            if (currentPiece->shape_array[i][j] == 0)
                continue;

            int gridX = (currentPiece->x + j * blockSize) / blockSize;
            int gridY = (currentPiece->y + i * blockSize) / blockSize;

            // 边界检测
            if (gridX < 0 || gridX >= gridWidth || gridY >= gridHeight)
                return true;

            // 上方超出网格时允许存在
            if (gridY < 0)
                continue;

            // 检查与已有方块的碰撞
            if (grid[gridY][gridX] == 1)
                return true;
        }
    }
    return false;
}

// 满行检测
vector<int>findFullRows(void)
{
    vector<int> fullRows;
    Judge j;
    if (j == grid) // 重载的==运算符用来判断是否存在满行
    {
        for (int i = 0; i < 20; i++)
        {
            bool is_full = true;
            for (int k = 0; k < 10; k++)
            {
                if (!grid[i][k])
                {
                    is_full = false;
                }
            }
            if (is_full)
            {
                fullRows.push_back(i);
                // 记分
                scores += fullRows.size() * 10;
            }
        }
    }

    if(scores>=9999999)
    {
        std::cout << "检测到作弊行为,程序自动结束!" << std::endl;
        exit(0);
    }

    return fullRows;
}

// 网格更新
void updateGrid(const vector<int> &fullRows)
{
    int newGrid[20][10] = {0};
    int destRow = 19; // 从底部开始填充新网格

    // 从底部向上遍历原网格
    for (int srcRow = 19; srcRow >= 0; srcRow--)
    {
        // 如果当前行是满行（需要消除），则跳过
        if (find(fullRows.begin(), fullRows.end(), srcRow) != fullRows.end())
        {
            continue;
        }

        // 将未消除的行复制到新网格的底部
        for (int j = 0; j < 10; j++)
        {
            newGrid[destRow][j] = grid[srcRow][j];
        }
        destRow--; // 向上移动填充位置
    }

    // 更新原网格
    memcpy(grid, newGrid, sizeof(grid));
}

Tetromino *createTetrominoCopy(const Tetromino *original, int newX, int newY, const vector<vector<int>> &newShape)
{
    switch (original->type)
    {
    case Tetromino::I:
        return new Tetromino_I(newX, newY);
    case Tetromino::J:
        return new Tetromino_J(newX, newY);
    case Tetromino::L:
        return new Tetromino_L(newX, newY);
    case Tetromino::O:
        return new Tetromino_O(newX, newY);
    case Tetromino::S:
        return new Tetromino_S(newX, newY);
    case Tetromino::T:
        return new Tetromino_T(newX, newY);
    case Tetromino::Z:
        return new Tetromino_Z(newX, newY);
    default:
        return nullptr;
    }
}

// 有满行进行画面更新
void updateShapes(std::vector<std::unique_ptr<Tetromino>> &SHAPE, const std::vector<int> &fullRows)
{
    std::vector<std::unique_ptr<Tetromino>> newShapes;
    for (auto &up : SHAPE)
    {
        Tetromino *piece = up.get();
        int currentRow = piece->y / 40; 
        if (std::find(fullRows.begin(), fullRows.end(), currentRow) != fullRows.end())
        {
            continue;
        }

        int shift = std::count_if(fullRows.begin(), fullRows.end(),
                                [currentRow](int r)
                                { return r > currentRow; });
        int newRow = currentRow + shift;
        piece->y = newRow * 40; 

        newShapes.push_back(std::move(up));
    }
    SHAPE = std::move(newShapes);
}

// 从option.txt读取信息并储存
vector<getInformation> optionInformation()
{
    std::ifstream ifs;
    ifs.open("option.txt", std::ios::in);
    if(!ifs.is_open())
    {
        std::cout << "Failed to open option.txt." << std::endl;
        return vector<getInformation>(1);
    }
    vector<getInformation> old(7); // 创建对象，以便返回
    for (int j = 0; j < 7;j++)
    {
        old[j].ID = 0;
        int temp1 = 0;
        vector<std::string> str(3); // 三行
        while(temp1<3)
        {
            getline(ifs, str[temp1++]);
        }
        std::string ID; // 编号
        for (int i = 0; i < str[0].size(); i++)
        {
            if (str[0][i] >= '0' && str[0][i] <= '9')
            {
                ID.push_back(str[0][i]);
            }
        }
        for (int i = 0; i < ID.size(); i++)
        {
            old[j].ID = old[j].ID * 10 + (ID[i] - '0');
        }
        std::string color[5]; // 颜色以及是否填充
        int num = 0;
        bool reading = false;
        for (int i = 0; i < str[1].size(); i++)
        {
            char ch = str[1][i];
            if (ch >= '0' && ch <= '9')
            {
                // 开始读取数字
                color[num] += ch;
                reading = true;
            }
            else
            {
                // 当前是非数字字符，说明数字读取结束
                if (reading)
                {
                    ++num;
                    reading = false;
                }
            }
        }
        int temp[5] = {0};
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < color[i].size(); j++)
            {
                temp[i] = temp[i] * 10 + (color[i][j] - '0');
            }
        }
        old[j].color.a = temp[0];
        old[j].color.b = temp[1];
        old[j].color.g = temp[2];
        old[j].color.r = temp[3];
        old[j].is_filled = temp[4];

        std::string v; // 速度
        old[j].v = 0;
        for (int i = 0; i < str[2].size(); i++)
        {
            if (str[2][i] >= '0' && str[2][i] <= '9')
            {
                v.push_back(str[2][i]);
            }
        }
        for (int i = 0; i < v.size(); i++)
        {
            old[j].v = old[j].v * 10 + (v[i] - '0');
        }

        if(old[j].v<=10)
        {
            std::cout << "编号为" << j << "的图形速度超过阈值!!!" << std::endl;
            std::cout << "请重新输入编号" << j << "的更新间隔" << std::endl;
            exit(0);
        }
        else if(old[j].v>=3000)
        {
            std::cout << "编号为" << j << "的图形速度太慢了!!!" << std::endl;
            std::cout << "请重新输入编号" << j << "的更新间隔" << std::endl;
            exit(0);
        }
    }
    ifs.close();

    return old; // 返回getInformation对象
}

// 时间以及最高分记录
void recordInformation()
{
    std::ifstream ifs;
    ifs.open("record.txt", std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "record.txt文件打开失败." << std::endl;
        return;
    }

    std::string str;
    std::string old_time;
    int i = 0;
    while (getline(ifs, str))
    {
        if(i==0)
        {
            ifs.seekg(0);
            getline(ifs, old_time);
        }
        i++;
        // 读取到第二行包含分数来进行比较
    }

    ifs.close();

    int old_scores = 0; // 之前的最高分
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            old_scores = old_scores * 10 + (str[i] - '0');
        }
    }

    std::ofstream ofs;
    ofs.open("record.txt", std::ios::out);
    if(!ofs.is_open())
    {
        std::cout << "record.txt文件打开失败." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();                   // 获取当前时间点
    std::time_t now_c = std::chrono::system_clock::to_time_t(now); // 转换为 time_t 类型
    std::string time = std::ctime(&now_c);
    time.pop_back();

    if(scores>=old_scores) // 现在的得分与之前的最高分比较
    {
        // 输出格式化的时间字符串
        ofs << "最高分记录时间: " << time << std::endl;
        ofs << "最高分: " << scores;
    }
    else
    {
        // 输出格式化的时间字符串
        ofs << old_time << std::endl;
        ofs << "最高分: " << old_scores;
    }
    ofs.close();
}

// 屏幕打印最高分
void presentOldHighestScores()
{
    std::ifstream ifs;
    ifs.open("record.txt", std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "record.txt文件打开失败." << std::endl;
        return;
    }

    std::string str;
    while (getline(ifs, str))
    {
        // 读取到第二行包含分数来进行比较
    }

    int old_scores = 0;
    char highest[100];
    int nonum = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            highest[i - nonum] = str[i];
            old_scores = old_scores * 10 + (str[i] - '0');
        }
        else
        {
            nonum++;
        }
    }
    ifs.close();

    snprintf(highest, sizeof(highest), "Highest Scores: %d", old_scores);

    SDL_Color textColor = {255, 255, 255, 255}; // 白色文字
    textSurface = TTF_RenderText_Blended(font, highest, textColor);
    if (textSurface)
    {
        textTexture = SDL_CreateTextureFromSurface(rdr, textSurface);
        if (textTexture)
        {
            SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h}; // 左上角位置
            SDL_RenderCopy(rdr, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }
}

// 屏幕打印当前得分
void presentCurrentScores()
{
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Current Scores: %d", scores);

    SDL_Color textColor = {255, 255, 255, 255}; // 白色文字
    textSurface = TTF_RenderText_Blended(font, scoreText, textColor);
    if (textSurface)
    {
        textTexture = SDL_CreateTextureFromSurface(rdr, textSurface);
        if (textTexture)
        {
            SDL_Rect textRect = {10, 30, textSurface->w, textSurface->h}; // 左上角位置
            SDL_RenderCopy(rdr, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void rotate(Tetromino &currentPiece)
{
    // 旋转函数实现,生成临时对象检测是否会碰撞
    vector<vector<int>> originalShape = currentPiece.shape_array;
    int originalRotation = currentPiece.rotationState;
    int originalX = currentPiece.x;
    int originalY = currentPiece.y;
    currentPiece.rotate();

    if (checkCollision(&currentPiece))
    {
        int dxAttempts[] = {-40, 40, -80, 80};
        bool found = false;
        for (int dx : dxAttempts)
        {
            currentPiece.x += dx;
            if (!checkCollision(&currentPiece))
            {
                found = true;
                break;
            }
            currentPiece.x = originalX;
        }
        if (!found)
        {
            currentPiece.shape_array = originalShape;
            currentPiece.rotationState = originalRotation;
            currentPiece.x = originalX;
            currentPiece.y = originalY;
        }
    }
}

// 右移重载++
void operator++(Tetromino &currentPiece)
{
    Tetromino temp = currentPiece;
    temp.x += 40;
    if(!checkCollision(&temp))
    {
        currentPiece.x += 40;
    }
}

// 左移重载--
void operator--(Tetromino &currentPiece)
{
    Tetromino temp = currentPiece;
    temp.x -= 40;
    if (!checkCollision(&temp))
    {
        currentPiece.x -= 40;
    }
}

std::ostream &operator<<(std::ostream &os, Tetromino *currentPiece)
{
    currentPiece->draw(rdr);
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::unique_ptr<Tetromino> &shape)
{
    shape->draw(rdr);
    return os;
}