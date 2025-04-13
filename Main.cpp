#include "CLasses.h"
#include "Performance.h"

#undef main

int main()
{
    Init(); // 初始化
    
    event_loop(); // 事件监测，循环 
    destory(); // 释放

    return 0;
}