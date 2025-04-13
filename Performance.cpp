#include "Performance.h"
#include "Classes.h"
#include <SDL_ttf.h>

#undef main

SDL_Window *win = nullptr; // 窗口初始化 
SDL_Renderer *rdr = nullptr; // 渲染器初始化
TTF_Font *font = nullptr;    // 字体初始化
SDL_Surface *textSurface = nullptr;
SDL_Texture *textTexture = nullptr;
std::vector<std::unique_ptr<Tetromino>> SHAPE; // 图形储存
static int scores = 0; // 得分统计
int grid[20][10] = {0}; // 游戏区域的网格（20行 x 10列）

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
    font = TTF_OpenFont("C:\\Windows\\Fonts\\simfang.ttf", 20);
    if (nullptr == font)
    {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
        return;
    }
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
    const Uint32 baseFallInterval = 500; 
    bool is_stop = 0; // 是否暂停

    while (!gameover)
    {
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
                case SDLK_ESCAPE: // 退出
                    gameover = true;
                    break;
                case SDLK_SPACE: // 暂停
                    is_stop = !is_stop;
                    break;
                case SDLK_LEFT: // 左移
                {
                    Tetromino temp = *currentPiece;
                    temp.x -= 40;
                    if (!checkCollision(&temp))
                        currentPiece->x -= 40;
                    break;
                }
                case SDLK_RIGHT: // 右移
                {
                    Tetromino temp = *currentPiece;
                    temp.x += 40;
                    if (!checkCollision(&temp))
                        currentPiece->x += 40;
                    break;
                }
                case SDLK_UP: // 旋转
                {
                    // 旋转函数实现,生成临时对象检测是否会碰撞
                    vector<vector<int>> originalShape = currentPiece->shape_array;
                    int originalRotation = currentPiece->rotationState;
                    int originalX = currentPiece->x;
                    int originalY = currentPiece->y;
                    currentPiece->rotate();

                    if (checkCollision(currentPiece))
                    {
                        int dxAttempts[] = {-40, 40, -80, 80};
                        bool found = false;
                        for (int dx : dxAttempts)
                        {
                            currentPiece->x += dx;
                            if (!checkCollision(currentPiece))
                            {
                                found = true;
                                break;
                            }
                            currentPiece->x = originalX;
                        }
                        if (!found)
                        {
                            currentPiece->shape_array = originalShape;
                            currentPiece->rotationState = originalRotation;
                            currentPiece->x = originalX;
                            currentPiece->y = originalY;
                        }
                    }
                    break;
                }
                case SDLK_DOWN: // 快速下降
                    is_pressed_down = true;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        if(!is_stop){
            Uint32 fallInterval = baseFallInterval;
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
                shape->draw(rdr);
            }
            // 绘画出当前图像
            currentPiece->draw(rdr);

            char scoreText[50];
            snprintf(scoreText, sizeof(scoreText), "Scores: %d", scores);

            SDL_Color textColor = {255, 255, 255, 255}; // 白色文字
            textSurface = TTF_RenderText_Blended(font, scoreText, textColor);
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

            // 渲染呈现
            SDL_RenderPresent(rdr);

            SDL_Delay(10);
        }
    }
}

void destory()
{
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
    for(int i=0;i<20;i++)
    {
        bool is_full = true;
        for(int j=0;j<10;j++)
        {
            if(!grid[i][j])
            {
                is_full = false;
            }
        }
        if(is_full)
        {
            fullRows.push_back(i);
        }
        scores += fullRows.size()*10;
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

