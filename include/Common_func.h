/*
    Common_func.h : 共通関数 (宣言のみ)
*/
#ifndef _Common_func_h
#define _Common_func_h

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"

/* ファイル内の関数の宣言 */
void allrideon(void);
void D_check(void);
int P_check(void);
void flooding(void);
int transmit(int k);
void detect_trans(void);
int syokika(void);
int check_ride(void);

#endif
