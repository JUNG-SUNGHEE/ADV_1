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

POSITION STATUS_CAMP_POS = { 2, 78 };
POSITION STATUS_NAME_POS = { 4,63 };
POSITION STATUS_ACCOUNT_POS = { 5,63 };
POSITION STATUS_DUR_POS = { 6,63 };
POSITION STATUS_COST_POS = { 7,63 };
POSITION COMMAND_POS = { 21, 63 };

typedef struct {
	char camp[100];
	int camp_n;// 0�̸� �ϼ� 1�̸� ��Ʈ��
	char name[100];
	char account[1000];
	int cost;
	int durability;
	char command[1000];
	POSITION curr_pos;
	int struct_size;

} UNIT_DATA;

typedef struct {
	char camp[100];
	int camp_n;
	char name[100];
	char account[1000];
	int cost;
	int durability;
	char command[1000];
	POSITION curr_pos;
	int struct_size;
	int color;
} ALLIVE_UNIT_DATA;


UNIT_DATA base_blue = {
	.camp = "��Ʈ���̵�",
	.name = "����",
	.account = "��Ʈ���̵��� �����̴�.",
	.cost = 0,
	.durability = 50,
	.command = "H: �Ϻ����� ����"
};
UNIT_DATA plate_blue = {
	.camp = "��Ʈ���̵�",
	.name = "����",
	.account = "�ǹ� ���� ���� ��ƾ���",
	.cost = 0,
	.durability = 50,
	.command = " "
};
UNIT_DATA base_red = {
	.camp = "���ڳ�",
	.name = "����",
	.account = "���ڳ��� �����̴�.    ",
	.cost = 0,
	.durability = 50,
	.command = "H: �Ϻ����� ����"
};
UNIT_DATA plate_red = {
	.camp = "���ڳ�",
	.name = "����",
	.account = "�ǹ� ���� ���� ��ƾ���",
	.cost = 0,
	.durability = 50,
	.command = " "
};

UNIT_DATA rock = {
	.camp = "����",
	.camp_n = 1,
	.name = "����",
	.account = "����� �ܴ��� ������.",
	.cost = 0,
	.durability = 0,
	.command = " ",
};
UNIT_DATA spice = {
	.camp = "����",
	.name = "�����̽�",
	.account = "���差  5                ",
	.cost = 0,
	.durability = 0,
	.command = " ",
};


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
	char ch = frontbuf[prev.row][prev.column];
	//printc(padd(map_pos, prev), ch, COLOR_DEFAULT);


				if (ch == 'B') {// �̰� �Լ���?? �����ҰͰ�����
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
	//gotoxy(A);
	//printf("%d %d", curr.row, curr.column); 
	/////////what_is_it(curr.row, curr.column);

	//select_cursor.row = curr.row;
	//select_cursor.column = curr.column;
}
/*void show_struct_profile(UNIT_DATA UNIT) {
	gotoxy(STATUS_CAMP_POS);
	printf("%s          ", UNIT.camp);
	gotoxy(STATUS_NAME_POS);
	printf("�̸� : %s                     ", UNIT.name);
	gotoxy(STATUS_ACCOUNT_POS);
	printf("���� : %s    ", UNIT.account);
	gotoxy(STATUS_DUR_POS);
	printf("������ : %d                  ", UNIT.durability);
	gotoxy(STATUS_COST_POS);
	printf("�Ǽ���� : %d                ", UNIT.cost);

	if (UNIT.command != " ") {
		gotoxy(COMMAND_POS);
		printf("%s                ", UNIT.command);
	}//�����̽��� ǥ�� �̷��������ϸ� ���ֱ��� �̰ɷ� ǥ������// ��ǥ�� �̿��ؼ� ����������� ����ϰ� �� ������// �׷� ���� ���¾ֵ� ��¾��ص���
 }*/// esc�� �װɷ� ��ǥ�� �̵� �� ��� ��ǥ�� �̵� ����� �̰� y���� ��ŭ�ϸ� �ɵ�


/*void what_is_it(int row, int column) {
	POSITION A = { 0, 30 };
	POSITION B = { 0, 40 };

	
	gotoxy(A);
	printf("%d %d", row, column);

	gotoxy(B);
	printf("%d %d", select_cursor.row, select_cursor.column);
	if (esc_switch == 1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
		select_cursor.column = 0;
		select_cursor.row = 0;
		gotoxy(STATUS_CAMP_POS);
		printf("                     ");
		gotoxy(STATUS_NAME_POS);
		printf("                               ");
		gotoxy(STATUS_ACCOUNT_POS);
		printf("                               ");
		gotoxy(STATUS_DUR_POS);
		printf("                               ");
		gotoxy(STATUS_COST_POS);
		printf("                               ");
		gotoxy(COMMAND_POS);
		printf("                               ");
	}
		if (select_cursor.row == row && select_cursor.column == column && select_cursor.row == 2 && select_cursor.column == 41) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(rock);
		}
		if (select_cursor.row == row && select_cursor.column == column && select_cursor.row == 14 && select_cursor.column == 46) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(rock);
		}
		if (select_cursor.row == row && select_cursor.column == column && select_cursor.row == 12 && select_cursor.column == 15) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(rock);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 6 && select_cursor.column == 20) || (select_cursor.row == 6 && select_cursor.column == 21) || (select_cursor.row == 7 && select_cursor.column == 20) || (select_cursor.row == 7 && select_cursor.column == 21)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(rock);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 11 && select_cursor.column == 40) || (select_cursor.row == 11 && select_cursor.column == 41) || (select_cursor.row == 12 && select_cursor.column == 40) || (select_cursor.row == 12 && select_cursor.column == 41)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(rock);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 15 && select_cursor.column == 1) || (select_cursor.row == 15 && select_cursor.column == 2) || (select_cursor.row == 16 && select_cursor.column == 1) || (select_cursor.row == 16 && select_cursor.column == 2)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE_TX);
			show_struct_profile(base_blue);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 1 && select_cursor.column == 57) || (select_cursor.row == 1 && select_cursor.column == 58) || (select_cursor.row == 2 && select_cursor.column == 57) || (select_cursor.row == 2 && select_cursor.column == 58)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED_TX);
			show_struct_profile(base_red);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 15 && select_cursor.column == 3) || (select_cursor.row == 15 && select_cursor.column == 4) || (select_cursor.row == 16 && select_cursor.column == 3) || (select_cursor.row == 16 && select_cursor.column == 4)) {

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE_TX);
			show_struct_profile(plate_blue);
		}
		if (select_cursor.row == row && select_cursor.column == column && (select_cursor.row == 1 && select_cursor.column == 55) || (select_cursor.row == 1 && select_cursor.column == 56) || (select_cursor.row == 2 && select_cursor.column == 55) || (select_cursor.row == 2 && select_cursor.column == 56)) {
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED_TX);
			show_struct_profile(plate_red);
		}
		if (select_cursor.row == row && select_cursor.column == column && select_cursor.row == 12 && select_cursor.column == 1) {
		
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(spice);
		}
		if (select_cursor.row == row && select_cursor.column == column && select_cursor.row == 5 && select_cursor.column == 58) {
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Y_TX);
			show_struct_profile(spice);
		}
	}*/



