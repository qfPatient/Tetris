#pragma once
#include <SDL.h>
#include <vector>
#include <memory>

#undef main

using std::vector;
extern int dx, dy;

class Tetromino
{
public:
    // 类型枚举
    enum Type { I,J,L,O,S,T,Z };
    Tetromino() = default; // 默认构造函数
    Tetromino(Type type, int x, int y);
    Tetromino(const Tetromino &other);
    virtual ~Tetromino();
    // 绘制函数
    virtual void draw(SDL_Renderer *rdr);
    // 旋转函数
    virtual void rotate();
    // 移动函数
    virtual void move(int x, int y);
    // 拷贝构造函数

protected:
    Type type; // 类型
    int x, y; // 当前坐标
    int rotationState; // 旋转状态
    SDL_Color *color; // 颜色
    vector<vector<int>> shape_array; // 形状数组
    // 友元函数
    friend void event_loop();
    friend void edge_judge(Tetromino *currentPiece,bool is_pressed_down);
    friend bool checkCollision(Tetromino *currentPiece);
    friend Tetromino *createTetrominoCopy(const Tetromino *original, int newX, int newY, const vector<vector<int>> &newShape);
    friend void updateShapes(std::vector<std::unique_ptr<Tetromino>> &SHAPE, const std::vector<int> &fullRows);
};

// 类的继承和子类构造函数实现
class Tetromino_I:public Tetromino
{
private:
    static int NumsOfI;

public:
    Tetromino_I(int x, int y);
    void rotate();
    static int getNumsOfI();
    ~Tetromino_I();
    Tetromino_I(const Tetromino_I &other);
};

class Tetromino_J : public Tetromino
{
private:
    static int NumsOfJ;

public:
    Tetromino_J(int x, int y);
    void rotate();
    static int getNumsOfJ();
    ~Tetromino_J();
    Tetromino_J(const Tetromino_J &other);
};

class Tetromino_L : public Tetromino
{
private:
    static int NumsOfL;

public:
    Tetromino_L(int x, int y);
    void rotate();
    static int getNumsOfL();
    ~Tetromino_L();
    Tetromino_L(const Tetromino_L &other);
};

class Tetromino_O : public Tetromino
{
private:
    static int NumsOfO;

public:
    Tetromino_O(int x, int y);
    void rotate();
    static int getNumsOfO();
    ~Tetromino_O();
    Tetromino_O(const Tetromino_O &other);
};

class Tetromino_S : public Tetromino
{
private:
    static int NumsOfS;

public:
    Tetromino_S(int x, int y);
    void rotate();
    static int getNumsOfS();
    ~Tetromino_S();
    Tetromino_S(const Tetromino_S &other);
};

class Tetromino_T : public Tetromino
{
private:
    static int NumsOfT;

public:
    Tetromino_T(int x, int y);
    void rotate();
    static int getNumsOfT();
    ~Tetromino_T();
    Tetromino_T(const Tetromino_T &other);
};

class Tetromino_Z : public Tetromino
{
private:
    static int NumsOfZ;

public:
    Tetromino_Z(int x, int y);
    void rotate();
    static int getNumsOfZ();
    ~Tetromino_Z();
    Tetromino_Z(const Tetromino_Z &other);
};