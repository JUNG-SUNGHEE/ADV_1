/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	112
#define COLOR_RESOURCE  112

#define BLUE_TX  1
#define RED_TX 4
#define Y_TX  6

#define COLOR_BLUE  22
#define COLOR_RED  64
#define COLOR_PLATE 128
#define COLOR_SPICE 80
#define COLOR_ROCK  176

// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);
POSITION select_cursor;
int esc_switch;
int b_m_switch;
int b_switch;
int build_sort_switch;// 0 : 장판 1 : 숙소





#endif
