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

POSITION STATUS_CAMP_POS = { 2, 78 };
POSITION STATUS_NAME_POS = { 4,63 };
POSITION STATUS_ACCOUNT_POS = { 5,63 };
POSITION STATUS_DUR_POS = { 6,63 };
POSITION STATUS_COST_POS = { 7,63 };
POSITION COMMAND_POS = { 21, 63 };

typedef struct {
	char camp[100];
	int camp_n;// 0이면 하세 1이면 아트편
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
	.camp = "아트레이디스",
	.name = "본진",
	.account = "아트레이디스의 본진이다.",
	.cost = 0,
	.durability = 50,
	.command = "H: 하베스터 생산"
};
UNIT_DATA plate_blue = {
	.camp = "아트레이디스",
	.name = "장판",
	.account = "건물 짓기 전에 깔아야함",
	.cost = 0,
	.durability = 50,
	.command = " "
};
UNIT_DATA base_red = {
	.camp = "하코넨",
	.name = "본진",
	.account = "하코넨의 본진이다.    ",
	.cost = 0,
	.durability = 50,
	.command = "H: 하베스터 생산"
};
UNIT_DATA plate_red = {
	.camp = "하코넨",
	.name = "장판",
	.account = "건물 짓기 전에 깔아야함",
	.cost = 0,
	.durability = 50,
	.command = " "
};

UNIT_DATA rock = {
	.camp = "공통",
	.camp_n = 1,
	.name = "바위",
	.account = "평범한 단단한 바위다.",
	.cost = 0,
	.durability = 0,
	.command = " ",
};
UNIT_DATA spice = {
	.camp = "공통",
	.name = "스파이스",
	.account = "매장량  5                ",
	.cost = 0,
	.durability = 0,
	.command = " ",
};


// 출력할 내용들의 좌상단(topleft) 좌표
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
}// 투사 이해 완(layer 0을 먼저 입력하고 같은자리에 조건이 맞다면 layer 1을 입력하니까 투사가 되는거다)

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

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
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
	printf("이름 : %s                     ", UNIT.name);
	gotoxy(STATUS_ACCOUNT_POS);
	printf("설명 : %s    ", UNIT.account);
	gotoxy(STATUS_DUR_POS);
	printf("내구도 : %d                  ", UNIT.durability);
	gotoxy(STATUS_COST_POS);
	printf("건설비용 : %d                ", UNIT.cost);

	if (UNIT.command != " ") {
		gotoxy(COMMAND_POS);
		printf("%s                ", UNIT.command);
	}//스파이스도 표현 이런식으로하면 유닛까지 이걸로 표현가능// 좌표를 이용해서 공백출력으로 깔끔하게 다 지우자// 그럼 내용 없는애들 출력안해도됨
}// esc는 그걸로 좌표로 이동 쫙 출력 좌표로 이동 쫙출력 이거 y길이 만큼하면 될듯


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



