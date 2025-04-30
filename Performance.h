#pragma once
#include <SDL.h>
#include <random>
#include <ctime>
#include <memory>
#include <map>
#include <memory>
#include <algorithm>
#include "Classes.h"

#undef main

extern SDL_Renderer *rdr;
extern SDL_Window *win;
extern void event_loop();
extern void edge_judge(Tetromino *currentPiece,bool is_pressed_down);
extern bool checkCollision(Tetromino *currentPiece);
extern Tetromino *createTetrominoCopy(const Tetromino *original, int newX, int newY, const vector<vector<int>> &newShape);
extern void updateShapes(std::vector<std::unique_ptr<Tetromino>> &SHAPE, const std::vector<int> &fullRows);
extern void graphInformation();
extern getInformation optionInformation();

void Init();
void draw();
void event_loop();
void destory();
void edge_judge(Tetromino *currentPiece,bool is_pressed_down);
Tetromino *createRandomTetromino(int x, int y);
bool checkCollision(Tetromino *currentPiece);
vector<int>findFullRows(void);
void updateGrid(const vector<int> &fullRows);
Tetromino *createTetrominoCopy(const Tetromino *original, int newX, int newY, const vector<vector<int>> &newShape);
void updateShapes(std::vector<std::unique_ptr<Tetromino>> &SHAPE, const std::vector<int> &fullRows);
getInformation optionInformation();
void recordInformation();
void graphInformation();
void presentOldHighestScores();
void presentCurrentScores();