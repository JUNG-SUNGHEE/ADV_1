/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"
#include <Windows.h>

node* head = NULL;



// ����� ������� �»��(topleft) ��ǥ
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
	
	display_cursor(cursor);
	display_map(map);
    
	display_status_message();
	display_command_message();
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
}// ���� ���� ��(layer 0�� ���� �Է��ϰ� �����ڸ��� ������ �´ٸ� layer 1�� �Է��ϴϱ� ���簡 �Ǵ°Ŵ�)
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
					//

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
				
				else if (backbuf[i][j] == 'D') {
					printc(padd(map_pos, pos), backbuf[i][j], 187);
				}
				else if (backbuf[i][j] == 'd') {
					printc(padd(map_pos, pos), backbuf[i][j], 221);
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
	change_map(18, 60, 0, 0, 0);// ���� �÷��� ȭ�� ��ȯ
	change_map(18, 101, 0, 61, 1); // ����â ȭ�� ��ȯ
	change_map(26, 60, 19, 0, 2); // �ý��� �޽��� ȭ�� â ��ȯ
	change_map(26, 101, 19, 61, 3);// Ŀ���â ȭ�� ��ȯ 

	
}

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;
	//POSITION A = { 0, 30 };
	char ch[4] = { frontbuf[prev.row][prev.column],frontbuf[prev.row + 1][prev.column], frontbuf[prev.row][prev.column + 1], frontbuf[prev.row + 1][prev.column + 1] };
	char ch_1 = frontbuf[prev.row][prev.column];
	POSITION prev_array[4] = { prev, {prev.row + 1, prev.column}, {prev.row, prev.column +1}, {prev.row +1, prev.column+1} };
	POSITION curr_array[4] = { curr, {curr.row + 1, curr.column}, {curr.row, curr.column + 1}, {curr.row + 1, curr.column + 1} };
	if (b_m_switch == 1) {
		for (int i = 0; i < 4; i++) {
			if (ch[i] == 'B') {// �̰� �Լ���?? �����ҰͰ�����
				printc(padd(map_pos, prev_array[i]), ch[i], 17);
			}
			else if (ch[i] == 'b') {
				printc(padd(map_pos, prev_array[i]), ch[i], 68);
			}
			else if (ch[i] == 'P') {
				printc(padd(map_pos, prev_array[i]), ch[i], 51);
			}
			else if (ch[i] == 'p') {
				printc(padd(map_pos, prev_array[i]), ch[i], 204);
			}
			else if (ch[i] == '5' || ch[i] == '4' || ch[i] == '3' || ch[i] == '2' || ch[i] == '1' || ch[i] == '0') {
				printc(padd(map_pos, prev_array[i]), ch[i], 87);
			}
			else if (ch[i] == 'R') {
				printc(padd(map_pos, prev_array[i]), ch[i], 136);
			}
			else if (ch[i] == 'H') {
				printc(padd(map_pos, prev_array[i]), ch[i], 176);
			}
			else if (ch[i] == 'h') {
				printc(padd(map_pos, prev_array[i]), ch[i], 160);
			}
			else if (ch[i] == 'W') {
				printc(padd(map_pos, prev_array[i]), ch[i], 96);
			}
			else if (ch[i] == 'w') {
				printc(padd(map_pos, prev_array[i]), ch[i], 96);
			}
			else if (ch[i] == ' ') {
				printc(padd(map_pos, prev_array[i]), ch[i], 238);
			}
			else if (ch[i] == '#') {
				printc(padd(map_pos, prev_array[i]), ch[i], 240);
			}
			else if (ch[i] == 'D') {
				printc(padd(map_pos, prev_array[i]), ch[i], 187);
			}
			else if (ch[i] == 'd') {
				printc(padd(map_pos, prev_array[i]), ch[i], 221);
			}
			else {
				printc(padd(map_pos, prev_array[i]), ch[i], COLOR_DEFAULT);
			}
		}
		ch[0] = frontbuf[curr.row][curr.column];
		ch[1] = frontbuf[curr.row + 1][curr.column];
		ch[2] = frontbuf[curr.row][curr.column + 1];
		ch[3] = frontbuf[curr.row + 1][curr.column + 1];

		for (int i = 0; i < 4; i++) {
			if (ch[i] != ' ') {
				printc(padd(map_pos, curr_array[i]), ch[i], 79);
			}
			else {
				printc(padd(map_pos, curr_array[i]), ch[i], 170);
			}
		}
	}
	/////////////////////////////////////////////
	
	else if(b_m_switch == 0){ //ch�� �迭�� ���� �Ʒ��� �ݺ������� ����
	//printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

		for (int i = 0; i < 4; i++) {
			if (ch[i] == 'B') {// �̰� �Լ���?? �����ҰͰ�����
				printc(padd(map_pos, prev_array[i]), ch[i], 17);
			}
			else if (ch[i] == 'b') {
				printc(padd(map_pos, prev_array[i]), ch[i], 68);
			}
			else if (ch[i] == 'P') {
				printc(padd(map_pos, prev_array[i]), ch[i], 51);
			}
			else if (ch[i] == 'p') {
				printc(padd(map_pos, prev_array[i]), ch[i], 204);
			}
			else if (ch[i] == '5' || ch[i] == '4' || ch[i] == '3' || ch[i] == '2' || ch[i] == '1' || ch[i] == '0') {
				printc(padd(map_pos, prev_array[i]), ch[i], 87);
			}
			else if (ch[i] == 'R') {
				printc(padd(map_pos, prev_array[i]), ch[i], 136);
			}
			else if (ch[i] == 'H') {
				printc(padd(map_pos, prev_array[i]), ch[i], 176);
			}
			else if (ch[i] == 'h') {
				printc(padd(map_pos, prev_array[i]), ch[i], 160);
			}
			else if (ch[i] == 'W') {
				printc(padd(map_pos, prev_array[i]), ch[i], 96);
			}
			else if (ch[i] == 'w') {
				printc(padd(map_pos, prev_array[i]), ch[i], 96);
			}
			else if (ch[i] == ' ') {
				printc(padd(map_pos, prev_array[i]), ch[i], 238);
			}
			else if (ch[i] == '#') {
				printc(padd(map_pos, prev_array[i]), ch[i], 240);
			}
			else if (ch[i] == 'D') {
				printc(padd(map_pos, prev_array[i]), ch[i], 187);
			}
			else if (ch[i] == 'd') {
				printc(padd(map_pos, prev_array[i]), ch[i], 221);
			}
			
			else {
				printc(padd(map_pos, prev_array[i]), ch[i], COLOR_DEFAULT);
			}
		}
		//���⿡ �� �ֺ��� ���ִ� �ڵ� �߰��ϸ� �ɵ�
				if (ch_1 == 'B') {// �̰� �Լ���?? �����ҰͰ�����
					printc(padd(map_pos, prev), ch_1, 17);
				}
				else if (ch_1 == 'b') {
					printc(padd(map_pos, prev), ch_1, 68);
				}
				else if (ch_1 == 'P') {
					printc(padd(map_pos, prev), ch_1, 51);
				}
				else if (ch_1 == 'p') {
					printc(padd(map_pos, prev), ch_1, 204);
				}
				else if (ch_1 == '5' || ch_1 == '4' || ch_1 == '3' || ch_1 == '2' || ch_1 == '1' || ch_1 == '0') {
					printc(padd(map_pos, prev), ch_1, 87);
				}
				else if (ch_1 == 'R') {
					printc(padd(map_pos, prev), ch_1, 136);
				}
				else if (ch_1 == 'H') {
					printc(padd(map_pos, prev), ch_1, 176);
				}
				else if (ch_1 == 'h') {
					printc(padd(map_pos, prev), ch_1, 160);
				}
				else if (ch_1 == 'W') {
					printc(padd(map_pos, prev), ch_1, 96);
				}
				else if (ch_1 == 'w') {
					printc(padd(map_pos, prev), ch_1, 96);
				}
				else if (ch_1 == ' ') {
					printc(padd(map_pos, prev), ch_1, 238);

				}
				else if (ch_1 == 'D') {
					printc(padd(map_pos, prev), ch_1, 187);
				}
				else if (ch_1 == 'd') {
					printc(padd(map_pos, prev), ch_1, 221);
				}
				else {
					printc(padd(map_pos, prev), ch_1, COLOR_DEFAULT);
				}
	ch_1 = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch_1, COLOR_CURSOR);
	}
}
