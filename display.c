/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"
#include <Windows.h>

node* head = NULL;



// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };

const POSITION map_pos = { 1, 0 };

//void what_is_it(int, int);

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void change_map(int, int, int, int, int);
void display_cursor(CURSOR cursor);


void display(RESOURCE resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], CURSOR cursor)
{
	display_sys_message();
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
    
	display_status_message();
	// display_system_message()
	// display_object_info()
	// display_commands()
	// ...
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}



// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}// 투사 이해 완(layer 0을 먼저 입력하고 같은자리에 조건이 맞다면 layer 1을 입력하니까 투사가 되는거다)
void change_map(int hight_f, int width_f, int hight_s, int width_s, int display_sort) {
	for (int i = hight_s; i < hight_f; i++) {
		for (int j = width_s; j < width_f; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				if (backbuf[i][j] == 'B') {
					printc(padd(map_pos, pos), backbuf[i][j], 17);
				}
				else if (backbuf[i][j] == 'b') {
					printc(padd(map_pos, pos), backbuf[i][j], 68);
				}
				else if (backbuf[i][j] == 'P') {
					printc(padd(map_pos, pos), backbuf[i][j], 51);
				}
				else if (backbuf[i][j] == 'p') {
					printc(padd(map_pos, pos), backbuf[i][j], 204);
				}
				else if (backbuf[i][j] == '5' || backbuf[i][j] == '4' || backbuf[i][j] == '3' || backbuf[i][j] == '2' || backbuf[i][j] == '1' || backbuf[i][j] == '0') {
					printc(padd(map_pos, pos), backbuf[i][j], 87);
				}
				else if (backbuf[i][j] == 'R') {
					printc(padd(map_pos, pos), backbuf[i][j], 136);
				}
				else if (backbuf[i][j] == 'H') {
					printc(padd(map_pos, pos), backbuf[i][j], 176);
				}
				else if (backbuf[i][j] == 'h') {
					printc(padd(map_pos, pos), backbuf[i][j], 160);
				}
				else if (backbuf[i][j] == 'W') {
					printc(padd(map_pos, pos), backbuf[i][j], 96);
				}
				else if (backbuf[i][j] == 'w') {
					printc(padd(map_pos, pos), backbuf[i][j], 96);
				}
				else if (backbuf[i][j] == ' ') {
					switch (display_sort) {
					case 0: 
						printc(padd(map_pos, pos), backbuf[i][j], 238);
						break;
					case 1:
					case 2:
					case 3:
						printc(padd(map_pos, pos), backbuf[i][j], 112);
						break;
					}
					

				}
				else if (backbuf[i][j] == '#') {

					switch (display_sort) {
					case 0:
						printc(padd(map_pos, pos), backbuf[i][j], 240);
						break;
					case 1:
					case 2:
					case 3:
						printc(padd(map_pos, pos), backbuf[i][j], 128);
						break;
					}
				}

				else if (backbuf[i][j] == 'Q') {
					printc(padd(map_pos, pos), backbuf[i][j], 0);
				}
				else if (backbuf[i][j] == 'C') {
					printc(padd(map_pos, pos), backbuf[i][j], 34);
				}
				else if (backbuf[i][j] == 'E') {
					printc(padd(map_pos, pos), backbuf[i][j], 85);
				}
				else if (backbuf[i][j] == 'T') {
					printc(padd(map_pos, pos), backbuf[i][j], 102);
				}
				else if (backbuf[i][j] == 'Y') {
					printc(padd(map_pos, pos), backbuf[i][j], 255);
				}
				else if (backbuf[i][j] == 'U') {
					printc(padd(map_pos, pos), backbuf[i][j], 119);
				}
				else if (backbuf[i][j] == 'I') {
					printc(padd(map_pos, pos), backbuf[i][j], 153);
				}
				else if (backbuf[i][j] == 'O') {
					printc(padd(map_pos, pos), backbuf[i][j], 187);
				}
				else if (backbuf[i][j] == 'A') {
					printc(padd(map_pos, pos), backbuf[i][j], 221);
				}
				else if (backbuf[i][j] == 'M') {
					printc(padd(map_pos, pos), backbuf[i][j], 238);
				}


				else {
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
				}
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);
	change_map(18, 60, 0, 0, 0);// 게임 플레이 화면 변환
	change_map(18, 101, 0, 61, 1); // 상태창 화면 변환
	change_map(26, 60, 19, 0, 2); // 시스템 메시지 화면 창 변환
	change_map(26, 101, 19, 61, 3);// 커멘드창 화면 변환 

	
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;
	//POSITION A = { 0, 30 };
	char ch = frontbuf[prev.row][prev.column];
	//printc(padd(map_pos, prev), ch, COLOR_DEFAULT);


				if (ch == 'B') {// 이거 함수로?? 가능할것같은데
					printc(padd(map_pos, prev), ch, 17);
				}
				else if (ch == 'b') {
					printc(padd(map_pos, prev), ch, 68);
				}
				else if (ch == 'P') {
					printc(padd(map_pos, prev), ch, 51);
				}
				else if (ch == 'p') {
					printc(padd(map_pos, prev), ch, 204);
				}
				else if (ch == '5' || ch == '4' || ch == '3' || ch == '2' || ch == '1' || ch == '0') {
					printc(padd(map_pos, prev), ch, 87);
				}
				else if (ch == 'R') {
					printc(padd(map_pos, prev), ch, 136);
				}
				else if (ch == 'H') {
					printc(padd(map_pos, prev), ch, 176);
				}
				else if (ch == 'h') {
					printc(padd(map_pos, prev), ch, 160);
				}
				else if (ch == 'W') {
					printc(padd(map_pos, prev), ch, 96);
				}
				else if (ch == 'w') {
					printc(padd(map_pos, prev), ch, 96);
				}
				else if (ch == ' ') {
					printc(padd(map_pos, prev), ch, 238);
				}
				else {
					printc(padd(map_pos, prev), ch, COLOR_DEFAULT);
				}

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
	
}
