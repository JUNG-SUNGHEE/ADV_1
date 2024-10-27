#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

int cnt = 0;//디버깅용

extern node* head;
void insertfrontnode(OBJECT_SAMPLE);
//void displaynode();

void init(void);
void intro(void);
void outro(void);
//void show_what_is_it(void);
void cursor_move(DIRECTION dir);
int fast_move(DIRECTION dir, int sys_clock);
void sample_obj_move(void);

void is_there_unit(void);// 커서선택 위치에 있는 유닛을 보여주는 함수/ 링크드 리스트 순회

void total_object_move(void);

POSITION sample_obj_next_position(void);
POSITION total_object_next_position(node*);
/////////////////////////

////////////////
int last_clock = 0;
DIRECTION last_dir = 0;

///////////////
/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

RESOURCE resource = {
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {GAME_HEIGHT - 5, GAME_WIDTH - 20},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};

OBJECT_SAMPLE H = {
	.pos = {14, 1},
	.dest = {5, 15},
	.src = {14,1},
	.repr = '%',
	.speed = 300,
	.next_move_time = 300,
	.color = COLOR_PLATE,
	.cost = 5,
	.population = 5,
	.str = 0,
	.hp = 70,
	.vision = 0
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init();
	intro();
	display(resource, map, cursor);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			cursor_move(ktod(key));
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_space: select_cursor = cursor.current; is_there_unit(); esc_switch = 0; break;
			case k_esc: esc_switch = 1; break;
			case k_make_h: insertfrontnode(H); break;
			//case k_show_unit: displaynode(); break;
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();
		total_object_move();
		// 화면 출력
		display(resource, map, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("DUNE 1.5\n");
	Sleep(2000);
	system("cls");
}

void outro(void) {
	printf("exiting...\n");
	exit(0);
}

/*void show_what_is_it(void) {
	POSITION A = { 0,15 };
	gotoxy(A);
	printf("마지막 스페이스바 위치 x = %d, y = %d", select_cursor.column, select_cursor.row);
}*/




void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < GAME_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][GAME_HEIGHT - 1][j] = '#';
		map[0][GAME_HEIGHT + 1][j] = '#';
		map[0][GAME_HEIGHT + SYSTEM_HEIGHT][j] = '#';
	}

	for (int i = 1; i < GAME_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][GAME_WIDTH - 1] = '#';
		map[0][i][GAME_WIDTH + 1] = '#';
		map[0][i][GAME_WIDTH + STATUS_WIDTH ] = '#';
		for (int j = 1; j < GAME_WIDTH - 1; j++) {
			map[0][i][j] = ' ';
		}
		for (int j = 1; j < COMMAND_WIDTH - 1; j++) {
			map[0][i][j + GAME_WIDTH + 1] = ' ';
		}
	}

	for (int j = 0; j < STATUS_WIDTH; j++) {
		map[0][0][GAME_WIDTH + 1 + j] = '#';
		map[0][GAME_HEIGHT - 1][GAME_WIDTH + 1 + j] = '#';
		map[0][GAME_HEIGHT + 1][GAME_WIDTH + 1 + j] = '#';
		map[0][GAME_HEIGHT + COMMAND_HEIGHT][GAME_WIDTH + 1 + j] = '#';
	}

	for (int i = 1; i < COMMAND_HEIGHT - 1; i++) {
		map[0][GAME_HEIGHT + 1 + i][0] = '#';
		map[0][GAME_HEIGHT + 1 + i][GAME_WIDTH -1] = '#';
		map[0][GAME_HEIGHT + 1 + i][GAME_WIDTH + 1] = '#';
		map[0][GAME_HEIGHT + 1 + i][GAME_WIDTH + COMMAND_WIDTH] = '#';
		for (int j = 1; j < COMMAND_WIDTH - 1; j++) {
			map[0][GAME_HEIGHT + 1 + i][GAME_WIDTH + 1 + j] = ' ';
		}
		for (int j = 1; j < GAME_WIDTH - 1; j++) {
			map[0][GAME_HEIGHT + 1 + i][j] = ' ';
		}
	}


	//아마 레이어 0에 건물을 지어줘야하는데 함수로 크기, 색, 문자, 위치 입력 받아서 만들면 될듯
	
	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
}
int fast_move(DIRECTION dir, int sys_clock) {
	int TorF = 0;
	if (sys_clock - last_clock < 200 && (last_clock + 10 != sys_clock)) {//last_clock + 10 != sys_clock 요거 작동안함 용도는 연속으로 같은키들어오면 그건 한칸씩이동 
		if (last_dir == dir ) {
			TorF = 1;
		}
	}
	last_clock = sys_clock;//printf로 확인하면서 다시 수정 sys_clock은 또 전역변수도 있음
	last_dir = dir;
	return TorF;
}
// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);
	POSITION A = { 0, 30 };
	if (dir != 0) {
			if (fast_move(dir, sys_clock)) {
				for (int i = 0; i < 4; i++) {
					new_pos = pmove(new_pos, dir);
				}
		}
	}
	//여기에서 쇼부를 봐야한다 newpos 값을 변경해서 구현하자 여기서 해야 오류가 안날듯

	// validation check
	if (1 <= new_pos.row && new_pos.row <= GAME_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= GAME_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;

		//select_cursor.row = new_pos.row;
		//select_cursor.column = new_pos.column;
	}
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { GAME_HEIGHT - 2, GAME_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= GAME_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= GAME_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}

POSITION total_object_next_position(node* curnode) {
	POSITION diff = psub(curnode->dest, curnode->pos);
	DIRECTION dir;
	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (curnode->dest.row == curnode->src.row && curnode->dest.column == curnode->src.column) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { curnode->dest.row, curnode->dest.column };
			curnode->dest = new_dest;
		}
		else {
			POSITION new_dest = { curnode->src.row, curnode->src.column};// 
			curnode->src.column = curnode->dest.column;
			curnode->src.row = curnode->dest.row;
			curnode->dest = new_dest;                                     // 아마 본함수에는 없던 src가 새로 추가되면서 생긴 오류// 그리고 xy구별하는것도 또 생각
		}
		return curnode->pos;
	}
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {// 이 코드 자체에 문제가 있는듯 보완 필요함 // 애초에 원래 자리로 올 생각도 없었음

		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(curnode->pos, dir);
	if (1 <= next_pos.row && next_pos.row <= GAME_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= GAME_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		return next_pos;
	}
	else {
		return curnode->pos;  // 제자리
	}
}


void total_object_move() {
	node* curnode;
	if (head == NULL) {
		return;
	}
	curnode = head;
	while (curnode->next != NULL) {
		if (sys_clock <= curnode->next_move_time) {
			return;
		}
		map[1][curnode->pos.row][curnode->pos.column] = -1;
		curnode->pos = total_object_next_position(curnode);
		map[1][curnode->pos.row][curnode->pos.column] = curnode->repr;

		curnode->next_move_time = sys_clock + curnode->speed;
		curnode = curnode->next;
	}
	if (sys_clock <= curnode->next_move_time) {
		return;
	}
	map[1][curnode->pos.row][curnode->pos.column] = -1;
	curnode->pos = total_object_next_position(curnode);
	map[1][curnode->pos.row][curnode->pos.column] = curnode->repr;

	curnode->next_move_time = sys_clock + curnode->speed;
	
}

void insertfrontnode(OBJECT_SAMPLE unit_sort)
{
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->color = unit_sort.color;
	newnode->cost = unit_sort.cost;
	newnode->dest = unit_sort.dest;
	newnode->src = unit_sort.src;
	newnode->hp = unit_sort.hp;
	newnode->move_period = unit_sort.move_period;
	newnode->next_move_time = unit_sort.next_move_time;
	newnode->population = unit_sort.population;
	newnode->pos = unit_sort.pos;
	newnode->repr = unit_sort.repr;
	newnode->speed = unit_sort.speed;
	newnode->str = unit_sort.str;
	newnode->vision = unit_sort.vision;
	newnode->next = NULL;

	map[1][newnode->pos.row][newnode->pos.column] = newnode->repr;
	
	if (head == NULL) {
		head = newnode;
		return;
	}
	newnode->next = head;
	head = newnode;

}

/*void displaynode() {
	node* curnode;
	if (head == NULL) {
		return;
	}
	printf("simply linked list : ");
	curnode = head;
	while (curnode->next != NULL) {
		printf("%d => ", curnode->cost);
		curnode = curnode->next;
	}
	printf("%d", curnode->cost);
}*/

void is_there_unit(){
	node* curnode;
	if (head == NULL) {
		return;
	}

	curnode = head;
	while (curnode->next != NULL) {
		if (curnode->pos.column == select_cursor.column && curnode->pos.row == select_cursor.row) {
			printf("이 위치에 유닛 있음");
		}
		//printf("curnode %d, %d  select_cursor %d, %d //", curnode->pos.column, curnode->pos.row, select_cursor.column, select_cursor.row);

		curnode = curnode->next;
	}
	printf("%d", curnode->cost);
}