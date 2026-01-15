/*
	base_func.h : プログラム簡略化のための基礎関数 (宣言のみ)
*/

#ifndef _base_func_h
#define _base_func_h

#include "config.h"
#include "base_struct.h"
#include "globals.h"

/* 関数宣言 */
double sqrt2(double a_x, double a_y);
double randAxAy(void);
double randAxAy_d(void);
void enqueue(int p_number);
int dequeue(int x, int y);
int peek(int i);
void stack_sort(void);
void push(int i, int push_data);
void push2(int i, int push_data_1, int push_data_2);
void pushnopos(int i, int push_data2);
int search(int count, int number);
int search2(int count, int number_1, int number_2);
int pop(int i);

#endif