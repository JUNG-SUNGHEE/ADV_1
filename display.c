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

void what_is_it(int, int);

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_structur();


void display(RESOURCE resource, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_structur();
	display_cursor(cursor);

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

void display_structur() {
	POSITION BASE_BLUE_1 = { 16, 1 };
	set_color(COLOR_BLUE);
	gotoxy(BASE_BLUE_1);
	printf("  \n");
	POSITION BASE_BLUE_2 = { 17, 1 };
	set_color(COLOR_BLUE);
	gotoxy(BASE_BLUE_2);
	printf("  \n");

	POSITION BASE_RED_1 = { 2, 57 };
	set_color(COLOR_RED);
	gotoxy(BASE_RED_1);
	printf("  \n");
	POSITION BASE_RED_2 = { 3, 57 };
	set_color(COLOR_RED);
	gotoxy(BASE_RED_2);
	printf("  \n");

	POSITION PLATE_1_BLUE = { 16, 3 };
	set_color(COLOR_PLATE);
	gotoxy(PLATE_1_BLUE);
	printf("  \n");
	POSITION PLATE_2_BLUE = { 17, 3 };
	set_color(COLOR_PLATE);
	gotoxy(PLATE_2_BLUE);
	printf("  \n");

	POSITION PLATE_1_RED = { 3, 55 };
	set_color(COLOR_PLATE);
	gotoxy(PLATE_1_RED);
	printf("  \n");
	POSITION PLATE_2_RED = { 2, 55 };
	set_color(COLOR_PLATE);
	gotoxy(PLATE_2_RED);
	printf("  \n");

	POSITION SPICE_1_BLUE = { 13, 1 };
	set_color(COLOR_SPICE);
	gotoxy(SPICE_1_BLUE);
	printf("5\n");

	POSITION SPICE_1_RED = { 6, 58};
	set_color(COLOR_SPICE);
	gotoxy(SPICE_1_RED);
	printf("5\n");

	POSITION ROCK_1 = { 8, 20 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_1);
	printf("  \n");
	POSITION ROCK_1_2 = { 7, 20 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_1_2);
	printf("  \n");

	POSITION ROCK_2 = { 13, 40 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_2);
	printf("  \n");
	POSITION ROCK_2_2 = { 12, 40 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_2_2);
	printf("  \n");

	POSITION ROCK_3 = { 13, 15 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_3);
	printf(" \n");

	POSITION ROCK_4 = { 3, 41 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_4);
	printf(" \n");
	
	POSITION ROCK_5 = { 15, 46 };
	set_color(COLOR_ROCK);
	gotoxy(ROCK_5);
	printf(" \n");
	
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

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;
	//POSITION A = { 0, 30 };
	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
	//gotoxy(A);
	//printf("%d %d", curr.row, curr.column); 
	what_is_it(curr.row, curr.column);

	//select_cursor.row = curr.row;
	//select_cursor.column = curr.column;
}
void show_struct_profile(UNIT_DATA UNIT) {
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
}// esc�� �װɷ� ��ǥ�� �̵� �� ��� ��ǥ�� �̵� ����� �̰� y���� ��ŭ�ϸ� �ɵ�


void what_is_it(int row, int column) {
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
	}



