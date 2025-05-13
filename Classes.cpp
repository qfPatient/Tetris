#include <iostream>
#include "Classes.h"
#include "Performance.h"
#undef main

// 默认偏移量
int dx = 0;
int dy = 1;

// 静态成员变量初始化
int Tetromino_I::NumsOfI = 0;
int Tetromino_J::NumsOfJ = 0;
int Tetromino_L::NumsOfL = 0;
int Tetromino_O::NumsOfO = 0;
int Tetromino_S::NumsOfS = 0;
int Tetromino_T::NumsOfT = 0;
int Tetromino_Z::NumsOfZ = 0;

// 各函数实现
Tetromino::Tetromino(Type type,int x,int y):type{type},x{x},y{y},color{new SDL_Color {0,0,0,255}}{}
Tetromino::~Tetromino() { delete color; }
// 移动量
void Tetromino::move(int x,int y)
{
    dx = x;
    dy = y;
}
void Tetromino::rotate(){}
// 绘画函数
void Tetromino::draw(SDL_Renderer *rdr)
{
    int blockSize = 40; // 每个方块的长和宽
    for (int i = 0; i < shape_array.size(); ++i)
    {
        for (int j = 0; j < shape_array[i].size(); ++j)
        {
            if (shape_array[i][j])
            {
                SDL_Rect blockRect = {x + j * blockSize, y + i * blockSize, blockSize, blockSize};
                SDL_SetRenderDrawColor(rdr, color->r, color->g, color->b, color->a);
                SDL_RenderFillRect(rdr, &blockRect);
            }
        }
    }
}

Tetromino::Tetromino(const Tetromino &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_I::Tetromino_I(int x,int y):Tetromino(Tetromino::I,x,y)
{
    shape_array = {
        {1}, 
        {1},
        {1},
        {1}
    };
    *color = {0, 255, 255, 255};
    rotationState = 0;
    NumsOfI++;
}

void Tetromino_I::rotate()
{
    rotationState = (rotationState + 1) % 2; // 两种旋转状态
    if (rotationState == 0)
    {
        shape_array = {{1}, {1}, {1}, {1}}; // 垂直
    }
    else
    {
        shape_array = {{1, 1, 1, 1}}; // 水平
    }
}

int Tetromino_I::getNumsOfI()
{
    return NumsOfI;
}

Tetromino_I::~Tetromino_I()
{
    std::cout << "Numbers of Tetromino_I are: " << NumsOfI << std::endl;
}

Tetromino_I::Tetromino_I(const Tetromino_I &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_J::Tetromino_J(int x, int y) : Tetromino(Tetromino::J, x, y)
{
    shape_array = {
        {0, 1},
        {0, 1},
        {1, 1},
    };
    *color = {0, 0, 255, 255};
    rotationState = 0;
    NumsOfJ++;
}

void Tetromino_J::rotate()
{
    rotationState = (rotationState + 1) % 4; // 四种旋转状态
    if (rotationState == 0)
    {
        shape_array = {
            {0, 1},
            {0, 1},
            {1, 1},
        };
    }
    else if(rotationState==1)
    {
        shape_array = {
            {1, 0, 0},
            {1, 1, 1}
        };
    }
    else if(rotationState==2)
    {
        shape_array = {
            {1, 1},
            {1, 0},
            {1, 0},
        };
    }
    else
    {
        shape_array = {
            {1,1,1},
            {0,0,1}
        };
    }
}

int Tetromino_J::getNumsOfJ()
{
    return NumsOfJ;
}

Tetromino_J::~Tetromino_J()
{
    std::cout << "Numbers of Tetromino_J are: " << NumsOfJ << std::endl;
}

Tetromino_J::Tetromino_J(const Tetromino_J &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_L::Tetromino_L(int x, int y) : Tetromino(Tetromino::L, x, y)
{
    shape_array = { 
        {1, 0},
        {1, 0},
        {1, 1}
    };
    *color = {0, 255, 0, 255};
    rotationState = 0;
    NumsOfL++;
}

void Tetromino_L::rotate()
{
    rotationState = (rotationState + 1) % 4; // 四种旋转状态
    if (rotationState == 0)
    {
        shape_array = {
            {1, 0},
            {1, 0},
            {1, 1},
        };
    }
    else if (rotationState == 1)
    {
        shape_array = {
            {1, 1 ,1},
            {1, 0 ,0}
        };
    }
    else if (rotationState == 2)
    {
        shape_array = {
            {1, 1},
            {0, 1},
            {0, 1},
        };
    }
    else
    {
        shape_array = {
            {0, 0, 1},
            {1, 1, 1}
        };
    }
}

int Tetromino_L::getNumsOfL()
{
    return NumsOfL;
}

Tetromino_L::~Tetromino_L()
{
    std::cout << "Numbers of Tetromino_L are: " << NumsOfL << std::endl;
}

Tetromino_L::Tetromino_L(const Tetromino_L &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_O::Tetromino_O(int x, int y) : Tetromino(Tetromino::O, x, y)
{
    shape_array = { 
        {1, 1},
        {1, 1} 
                };
    *color = {255, 0, 0, 255};
    rotationState = 0;
    NumsOfO++;
}

void Tetromino_O::rotate()
{
    rotationState = (rotationState + 1) % 1; // 无旋转状态
    shape_array = {
        {1, 1},
        {1, 1}
    };
}

int Tetromino_O::getNumsOfO()
{
    return NumsOfO;
}

Tetromino_O::~Tetromino_O()
{
    std::cout << "Numbers of Tetromino_O are: " << NumsOfO << std::endl;
}

Tetromino_O::Tetromino_O(const Tetromino_O &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_S::Tetromino_S(int x, int y) : Tetromino(Tetromino::S, x, y)
{
    shape_array = {
        {0, 1, 1},
        {1, 1, 0}
    };
    *color = {128, 0, 128, 255};
    rotationState = 0;
    NumsOfS++;
}

void Tetromino_S::rotate()
{
    rotationState = (rotationState + 1) % 2; // 两种旋转状态
    switch (rotationState)
    {
    case 0: 
        shape_array = {
            {0, 1, 1},
            {1, 1, 0}};
        break;
    case 1: 
        shape_array = {
            {1, 0},
            {1, 1},
            {0, 1}};
        break;
    }
}

int Tetromino_S::getNumsOfS()
{
    return NumsOfS;
}

Tetromino_S::~Tetromino_S()
{
    std::cout << "Numbers of Tetromino_S are: " << NumsOfS << std::endl;
}

Tetromino_S::Tetromino_S(const Tetromino_S &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_T::Tetromino_T(int x, int y) : Tetromino(Tetromino::T, x, y)
{
    shape_array = {
        {1, 1, 1},
        {0, 1, 0},
    };
    *color = {255, 255, 0, 255};
    rotationState = 0;
    NumsOfT++;
}

void Tetromino_T::rotate()
{
    rotationState = (rotationState + 1) % 4; // 四种旋转状态
    switch (rotationState)
    {
    case 0: 
        shape_array = {
            {1, 1, 1},
            {0, 1, 0}};
        break;
    case 1: 
        shape_array = {
            {0, 1},
            {1, 1},
            {0, 1}};
        break;
    case 2: 
        shape_array = {
            {0, 1, 0},
            {1, 1, 1}};
        break;
    case 3: 
        shape_array = {
            {1, 0},
            {1, 1},
            {1, 0}};
        break;
    }
}

int Tetromino_T::getNumsOfT()
{
    return NumsOfT;
}

Tetromino_T::~Tetromino_T()
{
    std::cout << "Numbers of Tetromino_T are: " << NumsOfT << std::endl;
}

Tetromino_T::Tetromino_T(const Tetromino_T &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Tetromino_Z::Tetromino_Z(int x, int y) : Tetromino(Tetromino::Z, x, y)
{
    shape_array = {
        {1, 1, 0},
        {0, 1, 1}
    };
    *color = {255, 165, 0, 255};
    rotationState = 0;
    NumsOfZ++;
}

void Tetromino_Z::rotate()
{
    rotationState = (rotationState + 1) % 2; // 两种旋转状态
    switch (rotationState)
    {
    case 0: 
        shape_array = {
            {1, 1, 0},
            {0, 1, 1}};
        break;
    case 1: 
        shape_array = {
            {0, 1},
            {1, 1},
            {1, 0}};
        break;
    }
}

int Tetromino_Z::getNumsOfZ()
{
    return NumsOfZ;
}

Tetromino_Z::~Tetromino_Z()
{
    std::cout << "Numbers of Tetromino_Z are: " << NumsOfZ << std::endl;
}

Tetromino_Z::Tetromino_Z(const Tetromino_Z &other)
{
    type = other.type;
    x = other.x;
    y = other.y;
    rotationState = other.rotationState;
    shape_array = other.shape_array;
    color = new SDL_Color(*other.color);
}

Judge::Judge()
{
    for (int i = 0; i < 20;i++)
    {
        for (int j = 0; j < 10;j++)
        {
            grid[i][j] = 1;
        }
    }
}

// 重载==来检测是否存在满行
bool Judge::operator==(int other[20][10])
{
    bool is_euqal = 1;
    bool is = 0;
    for (int i = 0; i < 20;i++)
    {
        is_euqal = 1;
        for (int j = 0; j < 10;j++)
        {
            if(!other[i][j])
            {
                is_euqal = 0;
                break;
            }
        }
        if(is_euqal==1)
        {
            is = 1;
            break;
        }
    }
    return is;
}