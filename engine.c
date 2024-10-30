#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include "io.h"
#include "display.h"

int cnt = 0;//유닛 넘버링 용
node a;//그 아무의미 없는 리턴용
node* select_unit_address = &a;

int w_cnt = 0; //그냥 검사용
POSITION A = { 0, 60 };// 그냥 검사용


extern node* head;
void insertfrontnode(OBJECT_SAMPLE);
void insertfrontnode_pre(OBJECT_SAMPLE, POSITION, int);

double distance(POSITION, POSITION);
node* who_is_the_closest(POSITION);


//void creat_struct(OBJECT_SMAPLE, POSITION);
//void displaynode();

void init(void);
void intro(void);
void outro(void);
//void show_what_is_it(void);
void cursor_move(DIRECTION dir);
int fast_move(DIRECTION dir, int sys_clock);
void sample_obj_move(void);

node* is_there_unit(void);// 커서선택 위치에 있는 유닛을 보여주는 함수/ 링크드 리스트 순회

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
	.dest = {14, 1},//{5, 15},
	.src = {14,1},
	.repr = 'H',
	.speed = 300,
	.next_move_time = 300,
	.color = COLOR_PLATE,
	.cost = 5,
	.population = 5,
	.str = 0,
	.hp = 70,
	.vision = 0,
	.size = 1,

	.select_flag = 0,
	.cmd_flag_round_move = 0,
	.cmd_flag_move = 0,

	.possible_cmd = {1, 1, 1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1}, // 이렇게 하면 처음부터 가만히 있게 할수있다 그리고 나중에 다른 명령들어오면 풀어주자
	.is_it_my_side_flag = 1,
	.is_it_structure_flag = 1// 0이면 건물 / 1이면 유닛
};// 명령어 겹치네 적군 아군 구별하는 FLAG 필요 이유 난 아군 유닛만 조종가능해야함 적군 베이스에서 하베스트를 생산한다고 생각해봐 구별필요
// 건물, 유닛 구별 이거 하면 같은 명령어라고 해도 구별이 가능해진다

OBJECT_SAMPLE h = {
	.pos = {3, 58},
	.dest = {3, 58},//{5, 15},
	.src = {3, 58},
	.repr = 'h',
	.speed = 300,
	.next_move_time = 300,
	.color = COLOR_PLATE,
	.cost = 5,
	.population = 5,
	.str = 0,
	.hp = 70,
	.vision = 0,
	.size = 1,

	.select_flag = 0,
	.cmd_flag_round_move = 0,
	.cmd_flag_move = 0,

	.possible_cmd = {1, 1, 1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1}, // 이렇게 하면 처음부터 가만히 있게 할수있다 그리고 나중에 다른 명령들어오면 풀어주자
	.is_it_my_side_flag = 1,
	.is_it_structure_flag = 1// 0이면 건물 / 1이면 유닛
};

OBJECT_SAMPLE W = {
	.pos = {0},
	.dest = {0},
	.src = {0},
	.repr = 'W',
	.speed = 300,
	.next_move_time = 300,
	.str = 9999,
	.hp = -1,
	.vision = 0,
	.size = 1,

	.select_flag = 0,
	.cmd_flag_round_move = 0,
	.cmd_flag_move = 0,

	.possible_cmd = {1, 1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {0}, // 이렇게 하면 처음부터 가만히 있게 할수있다 그리고 나중에 다른 명령들어오면 풀어주자
	.is_it_my_side_flag = 2,
	.is_it_structure_flag = 1

};

OBJECT_SAMPLE BASE_BLUE = {
	.pos = {0},
	.size = 2,
	.hp = 50,
	.cost = 0,
	.repr = 'B',


	.possible_cmd = {0,0,0,0,1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1, 0},// 이렇게 해서 처음부터 가만히 있게하고 건물이니 풀지말자 저 1 
	.is_it_structure_flag =0

};
OBJECT_SAMPLE BASE_RED = {
	.pos = {0},
	.size = 2,
	.hp = 50,
	.cost = 0,
	.repr = 'b',


	.possible_cmd = {0,0,0,0,1},
	.allive_cmd = {1},
	.is_it_structure_flag = 0

};


OBJECT_SAMPLE PLATE_BLUE = {
	.pos = {0},
	.size = 2,
	.hp = -1,
	.cost = 1,
	.repr = 'P',


	.possible_cmd = {0},
	.allive_cmd = {1},
	.is_it_structure_flag = 0

};
OBJECT_SAMPLE PLATE_RED = {
	.pos = {0},
	.size = 2,
	.hp = -1,
	.cost = 1,
	.repr = 'p',


	.possible_cmd = {0},
	.allive_cmd = {1},
	.is_it_structure_flag = 0

};

OBJECT_SAMPLE SPICE = {
	.hp = -1,
	.size = 1,
	.rest_spice = 0,
	.repr = '5',
	.is_it_structure_flag = 0,
	.allive_cmd = {1}
};//하베스트가 스파이스를 채취했다의 조건이 참일때  .repr의 아스키를 -1로 하고 레스트 스파이스도 -1로 줄이자 그리고 0되면 노드 삭제 // 웜이 생산 할때는 랜덤으로 숫자가 나와야하는데 이것도 랜덤값을 
//대입하자 repr에 0~5까지의 아스키코드를 랜덤으로 해서 대입되게 // 하베스터나 다른 건물들도 문자 보이게 할지 고민 더 직관적이긴 할듯
OBJECT_SAMPLE ROCK = {
	.hp = -1,
	.size = 1,
	.repr = 'R',
	.is_it_structure_flag = 0,
	.allive_cmd = {1}
};

OBJECT_SAMPLE BIG_ROCK = {
	.hp = -1,
	.size = 2,
	.repr = 'R',
	.is_it_structure_flag = 0,
	.allive_cmd = {1}
};// 이렇게 중립인 경우 hp가 -1은 공격이 불가능하게 하자

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
			case k_space: 
				select_cursor = cursor.current; 
				if (is_there_unit() != &a) {
					select_unit_address = is_there_unit();
				}

				if (select_unit_address->allive_cmd[1] == 1) {
					select_unit_address->dest.row = select_cursor.row;
					select_unit_address->dest.column = select_cursor.column;
					//select_unit_address->allive_cmd[1] = 0;
					
				} 
				esc_switch = 0; 
				break;//select_flag는 좋은데 꼭 필요한진 모르겠다.
			
			case k_esc: esc_switch = 1; break;// is_there_unit이 주소를 리턴하게 하면 많은것을 할수있다. 이걸로 프로필 출력도 하자. 

			case k_make_and_har_h:  insertfrontnode(H) ; break;
			case k_mining_t: 
				if (select_unit_address->possible_cmd[1] == 1) { 
					select_unit_address->allive_cmd[0] = 0; 
					select_unit_address->allive_cmd[1] = 1;//시작하자마자 t를 누르면 멈춰버린다 null을 참조해서 그런듯
					select_unit_address->allive_cmd[2] = 0;
				} break;// 아직 완벽하게는 안됨
			case k_move_m: 
				if (select_unit_address->possible_cmd[2] == 1) {
					select_unit_address->allive_cmd[0] = 0;
					select_unit_address->allive_cmd[1] = 0;
					select_unit_address->allive_cmd[2] = 1;
				}break;
				// 멈추기 키도 만들어야함// 멈추기 키는 그냥 모든 명령이 끝난뒤에 패시브임 따로 키를 만들필요는 없을듯
				
				// 아니지 여기선 가능한 커멘드 플래그중 어느게 활성화 될지만 정해지고 함수 호출은 다른데서 해야지 예로 반복 움직임이 키보드를 누를때만
				//하는게 아니라 취소할때까지 반복이자나 /만약 취소버튼 누르면 현재 활성화된 명령 클래그 전부 비활성화// 아니 그래도 그냥 활성화 하면 안되지 본진을 선택하고 하베스터의 채집을 누르면 
				// 그럼 하베스터의 플래그가 1이 될거아니야 그럼 안되지. 명령은 순서로 구분하자. 그러니 애초에 플래그 활성화 할때도 검증은 해야함
				// curnode에 현재 활성화 명령, 사용가능한 명령
				//case안에서 if(선택 유닛의 사용가능한 명령 순서적 인덱스[3]== 선택 명령의 순서적 위치의 인덱스?[3]의 값 && 선택 명령의 순서적[3] == 1)이면 플래그 온 그 플래그는 select_unit_address안에
				//활성화 유닛의 플래그를 바꿔줌 그럼 활성화 유닛의 플래그를 기반으로 반복해서 움직이겠지(왕복 움직임 함수를 생각해봐 이게 스위치 안에 있으면 상식적으로 안되자나 그 함수를 if로 묶어
				// 혹은 함수 호출부를 if로 묶어서 만약 그 [3]플래그가 1로 활성화 되어있다면 그때만 반복 / 또 추가로 할일 다마치면 활성화 플래그 0으로 바뀌게 또는 명령 취소버튼누르면 그러게 0으로 바뀌게
				// 우선 아군인지 -> 건물인지 -> 나머지 위에 내용
				// 건물도 링크드 리스트에 추가한 후부터 h 명령어 구분이 가능하고 필요해질듯
				// 일단 h대신 다른 키로해서 채굴 기능 만들자

			//case k_show_unit: displaynode(); break;
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();
		//if (select_unit_address->allive_cmd[0] == 1) {//이거하면 아예 실행 자체가 안됨
			total_object_move();
		//}
		// 화면 출력
		display(resource, map, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("Hannam University\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("...............................DDDDDDDD.....U.......U...N.......N...EEEEEEEE............................\n");
	printf("...............................D.......D....U.......U...NN......N...E...................................\n");
	printf("...............................D........D...U.......U...N.N.....N...E...................................\n");
	printf("...............................D........D...U.......U...N..N....N...EEEEEEE.............................\n");
	printf("...............................D........D...U.......U...N...N...N...E...................................\n");
	printf("...............................D.......D....U.......U...N....N..N...E...................................\n");
	printf("...............................DDDDDDDD......UUUUUUU....N......NN...EEEEEEEE............................\n");
	printf("........................................................................................................\n");
	printf(".........................................11....999...11....000..........................................\n");
	printf("..........................................1...9...9...1...0...0.........................................\n");
	printf("..........................................1....9999...1...0...0.........................................\n");
	printf("..........................................1.......9...1...0...0.........................................\n");
	printf("........................................11111..9999.11111..0000.........................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");
	printf("........................................................................................................\n");

	Sleep(4000);
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
	//map[1][H.pos.row][H.pos.column] = '%';


	// layer 0(map[0])에 건물넣기 일단 아스키 코드로
	POSITION pre_struct_pos_blue_base = { 15, 1 };
	POSITION pre_struct_pos_blue_plate = { 15, 3 };
	POSITION pre_struct_pos_blue_spice = { 11, 1 };
	
	POSITION pre_struct_pos_red_base = { 1, 57 };
	POSITION pre_struct_pos_red_plate = { 1, 55 };
	POSITION pre_struct_pos_red_spice = { 6, 58 };

	POSITION pre_struct_pos_rock_1 = {13, 15};
	POSITION pre_struct_pos_rock_2 = {3, 41};
	POSITION pre_struct_pos_rock_3 = {15, 46};
	POSITION pre_struct_pos_big_rock_1 = { 8, 20 };
	POSITION pre_struct_pos_big_rock_2 = {13, 40};

	POSITION pre_unit_pos_H = { 14, 1 };
	POSITION pre_unit_pos_h = { 3, 58 };

	POSITION pre_unit_pos_W = { 5, 15 };
	POSITION pre_unit_pos_W_2 = { 10, 38 };

	insertfrontnode_pre(BASE_BLUE, pre_struct_pos_blue_base, 0);// 지금은 초기 배치니까 위치는 내가 직접입력 pos 하나만
	insertfrontnode_pre(PLATE_BLUE, pre_struct_pos_blue_plate, 0);

	insertfrontnode_pre(BASE_RED, pre_struct_pos_red_base, 1);
	insertfrontnode_pre(PLATE_RED, pre_struct_pos_red_plate, 1);

	insertfrontnode_pre(SPICE, pre_struct_pos_blue_spice, 2);
	insertfrontnode_pre(SPICE, pre_struct_pos_red_spice, 2);
	
	insertfrontnode_pre(ROCK, pre_struct_pos_rock_1, 2);
	insertfrontnode_pre(ROCK, pre_struct_pos_rock_2, 2);
	insertfrontnode_pre(ROCK, pre_struct_pos_rock_3, 2);
	insertfrontnode_pre(BIG_ROCK, pre_struct_pos_big_rock_1, 2);
	insertfrontnode_pre(BIG_ROCK, pre_struct_pos_big_rock_2, 2);

	insertfrontnode_pre(H, pre_unit_pos_H, 0);
	insertfrontnode_pre(h, pre_unit_pos_h, 1);

	insertfrontnode_pre(W, pre_unit_pos_W, 2);
	insertfrontnode_pre(W, pre_unit_pos_W_2, 2);

	//insertfrontnode()하베스터나 웜 생각하면 같은 하베스터고 웜이어도 시작위치가 다른 그러니 이것또 pos로 받아줘야함 그리고 적인지 아군인지도 같이 받아줘야 할듯 저 struct함수처럼
	//여기에 하베스터등 기본 유닛을 생성//유닛이 정지해있는것도 명령중하나로 하자
	// 생각해보니 유닛도 적인지 아군인지 구별해야한다
	// 그리고 건물 추가 함수에 매개변수에 적인지 아군 구분 플래그도 줘야할듯
	// plate는 이렇게 구현하자 모든 건물 아래는 plate임 그러니까 건물의 hp가 0이되면 문자와 hp 등등을 plate의 것으로 바꿔주면 되지
	//그리고 건물 생성할때는 스페이바로 선택할곳이 plate 일때만 생성가능하게
}

void insertfrontnode_pre(OBJECT_SAMPLE unit_sort, POSITION pre_pos, int side) {
	
	cnt++;
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
	newnode->pos = pre_pos;
	newnode->repr = unit_sort.repr;
	newnode->speed = unit_sort.speed;
	newnode->str = unit_sort.str;
	newnode->vision = unit_sort.vision;
	newnode->unit_cnt = cnt;//얜 없네 머지

	newnode->select_flag = unit_sort.select_flag;
	newnode->cmd_flag_round_move = unit_sort.cmd_flag_round_move;
	newnode->cmd_flag_move = unit_sort.cmd_flag_move;

	newnode->size = unit_sort.size;
	newnode->rest_spice = unit_sort.rest_spice;

	for (int i = 0; i < 10; i++) {
		newnode->possible_cmd[i] = unit_sort.possible_cmd[i];// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
		newnode->allive_cmd[i] = unit_sort.allive_cmd[i];
	}
	newnode->is_it_my_side_flag = side;
	newnode->is_it_structure_flag = unit_sort.is_it_structure_flag;
	newnode->next = NULL;

	if (newnode->is_it_structure_flag == 1) {
		if (newnode->size == 1) {
			map[1][newnode->pos.row][newnode->pos.column] = newnode->repr;
		}
		else if (newnode->size == 2) {
			for (int i = 0; i < newnode->size; i++) {
				for (int j = 0; j < newnode->size; j++) {
					map[1][newnode->pos.row + i][newnode->pos.column + j] = newnode->repr;
				}
			}
		}
	}

	else if(newnode->is_it_structure_flag == 0){
		if (newnode->size == 1) {
			map[0][newnode->pos.row][newnode->pos.column] = newnode->repr;
		}
		else if (newnode->size == 2) {
			for (int i = 0; i < newnode->size; i++) {
				for (int j = 0; j < newnode->size; j++) {
					map[0][newnode->pos.row + i][newnode->pos.column + j] = newnode->repr;
				}
			}
		}
	}

	if (head == NULL) {
		head = newnode;
		return;
	}
	newnode->next = head;
	head = newnode;
	
	
	
	
	// 이건 확실히 유닛용임 생성되는 위치가 정해져있음 건물용 노드생성 함수도 필요함 매개변수로 POS를 받을 수있는 그러나 같은 링크드리스트를 공유하는
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

double distance(POSITION p1, POSITION p2) {
	return sqrt(pow(p2.column - p1.column, 2) + pow(p2.row - p1.row, 2));

}

node* who_is_the_closest(POSITION W_POS) {
	node* curnode;
	curnode = head;
	
	double closest_dist = 10000;
	node* closest_dist_address = NULL;
	double dist;

	while (curnode->next != NULL) {
		if (curnode->repr != 'W' && curnode->is_it_structure_flag == 1) {
			dist = distance(W_POS, curnode->pos);//dist가 이상하게 나온다 0나와버림
			if (closest_dist > dist) {
				closest_dist = dist;
				closest_dist_address = curnode;// 이조건이 잘못되서 그냥 유닛은 전부 통과해버리나?

			}
		}
		curnode = curnode->next;
	}
	if (curnode->repr != 'W' && curnode->is_it_structure_flag == 1) {
		dist = distance(W_POS, curnode->pos);
		if (closest_dist > dist) {
			closest_dist = dist;
			closest_dist_address = curnode;
		}
	}
	return closest_dist_address;
	
}// 위치에 상관없이 마지막에 생성된 애를 따라온다 왜지?

POSITION total_object_next_position(node* curnode) {
	POSITION diff = psub(curnode->dest, curnode->pos);
	DIRECTION dir;
	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	/*if (select_unit_address->allive_cmd[0] != 1) {
		return curnode->pos;
	}*/ 

	

	if (curnode->allive_cmd[0] == 1) {
		return curnode->pos;
	}// 이런식으로 움직임 커멘드 조절하자// 

	if (curnode->repr == 'W') {
		node* closest_address;
		closest_address = who_is_the_closest(curnode->pos);
		POSITION new_dest = {closest_address->pos.row, closest_address->pos.column};

		curnode->dest = new_dest;// 아시발 같은 구조체를 공유하고 있네 맞다 아니지 하베스터를 생각해봐라 근데 이런 류의 오류일거다아마
	}

	if (diff.row == 0 && diff.column == 0) {
		if (curnode->allive_cmd[1] == 1) {
			//curnode->allive_cmd[1] = 0;
			//curnode->allive_cmd[0] = 1;
			return curnode->pos;
		}
		if (curnode->dest.row == curnode->src.row && curnode->dest.column == curnode->src.column) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { curnode->dest.row, curnode->dest.column };
			curnode->dest = new_dest;
		}
		else {
			POSITION new_dest = { curnode->src.row, curnode->src.column};
			curnode->src.column = curnode->dest.column;
			curnode->src.row = curnode->dest.row;
			curnode->dest = new_dest;                                
		}
		return curnode->pos;
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
	cnt++;
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
	newnode->unit_cnt = cnt;//얜 없네 머지

	newnode->select_flag = unit_sort.select_flag;
	newnode->cmd_flag_round_move = unit_sort.cmd_flag_round_move;
	newnode->cmd_flag_move = unit_sort.cmd_flag_move;

	newnode->size = unit_sort.size;

	for (int i = 0; i < 10; i++) {
		newnode->possible_cmd[i] = unit_sort.possible_cmd[i];//0:(H) 하베스터 채굴 / 1:(M) move / 2:(P) 순찰    //    3:(H) 하베스터 생산 // 정지 명령도 만들자
		newnode->allive_cmd[i] = unit_sort.allive_cmd[i];
	}
	newnode->is_it_my_side_flag = unit_sort.is_it_my_side_flag;
	newnode->is_it_structure_flag = unit_sort.is_it_structure_flag;
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

node* is_there_unit(){
	node* curnode;
	if (head == NULL) {
		return;// 요 리턴을 일단 아무 의미 없는 값으로 주자// 아닌데 애초에 
	}
	curnode = head;
	while (curnode->next != NULL) {
		if (curnode->size == 1) {
			if (curnode->pos.column == select_cursor.column && curnode->pos.row == select_cursor.row) {
				//printf("%d 번째의 유닛", curnode->unit_cnt);
				curnode->select_flag = 1;
				return curnode;
			}
		}
		else if (curnode->size == 2) {
			for (int i = 0; i < curnode->size; i++) {
				for (int j = 0; j < curnode->size; j++) {
					if (curnode->pos.column + i == select_cursor.column && curnode->pos.row + j == select_cursor.row) {
						return curnode;
					}
				}
			}
		}// 그니까 만약 조건이 맞다면 curnode 구조체를 리턴하는데 만약 아니라면? 그것도 리턴을 해줘야지 무언가 
		curnode = curnode->next;
	}
	if (curnode->size == 1) {
		if (curnode->pos.column == select_cursor.column && curnode->pos.row == select_cursor.row) {
			//printf("%d 번째의 유닛", curnode->unit_cnt);
			curnode->select_flag = 1;
			return curnode;// 알겠다 왜 처음에 하베스터를 스페이스바로 선택하면 에러가 나는지 아니 애초에 유닛이 아닌것을 스페이스바로 선택하면 그냥 멈춰버림 그이유는 
		}
	}
	else if (curnode->size == 2) {
		for (int i = 0; i < curnode->size; i++) {
			for (int j = 0; j < curnode->size; j++) {
				if (curnode->pos.column + i == select_cursor.column && curnode->pos.row + j == select_cursor.row) {
					return curnode;
				}
			}
		}
	}
	return &a;
}