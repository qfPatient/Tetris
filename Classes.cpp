#include "Classes.h"
#include "Performance.h"
#undef main

// 默认偏移量
int dx = 0;
int dy = 1;

// 各函数实现
Tetromino::Tetromino(Type type,int x,int y):type{type},x{x},y{y}{}
Tetromino::~Tetromino(){}
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
                SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(rdr, &blockRect);
            }
        }
    }
}

Tetromino_I::Tetromino_I(int x,int y):Tetromino(Tetromino::I,x,y)
{
    shape_array = {
        {1}, 
        {1},
        {1},
        {1}
    };
    color = {0, 255, 255, 255};
    rotationState = 0;
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

Tetromino_J::Tetromino_J(int x, int y) : Tetromino(Tetromino::J, x, y)
{
    shape_array = {
        {0, 1},
        {0, 1},
        {1, 1},
    };
    color = {0, 0, 255, 255};
    rotationState = 0;
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

Tetromino_L::Tetromino_L(int x, int y) : Tetromino(Tetromino::L, x, y)
{
    shape_array = { 
        {1, 0},
        {1, 0},
        {1, 1}
    };
    color = {0, 255, 0, 255};
    rotationState = 0;
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

Tetromino_O::Tetromino_O(int x, int y) : Tetromino(Tetromino::O, x, y)
{
    shape_array = { 
        {1, 1},
        {1, 1} 
                };
    color = {255, 0, 0, 255};
    rotationState = 0;
}
void Tetromino_O::rotate()
{
    rotationState = (rotationState + 1) % 1; // 无旋转状态
    shape_array = {
        {1, 1},
        {1, 1}
    };
}

Tetromino_S::Tetromino_S(int x, int y) : Tetromino(Tetromino::S, x, y)
{
    shape_array = {
        {0, 1, 1},
        {1, 1, 0}
    };
    color = {128, 0, 128, 255};
    rotationState = 0;
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

Tetromino_T::Tetromino_T(int x, int y) : Tetromino(Tetromino::T, x, y)
{
    shape_array = {
        {1, 1, 1},
        {0, 1, 0},
    };
    color = {255, 255, 0, 255};
    rotationState = 0;
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

Tetromino_Z::Tetromino_Z(int x, int y) : Tetromino(Tetromino::Z, x, y)
{
    shape_array = {
        {1, 1, 0},
        {0, 1, 1}
    };
    color = {255, 165, 0, 255};
    rotationState = 0;
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