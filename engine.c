﻿//1 ~5번 전부 구현(보너스 제외) 
//6번 건설 못한부분 - 장판이 깔려있어야 그 위에 건설 가능 그 외는 전부 구현(보너스 제외) 
//7번 유닛목록 구현 구현 완료 
//8번 유닛행동 - 하베스터 못한부분 - 수확 명령 구현 그 외 구현 
//9번 유닛 행동 - 보병, 특수유닛 하베스터 외에 다른 유닛을 만들진않았지만 모든 유닛이 이용가능한 함수를 만들었음
//이동은 구현함 순찰 구현 못함 
//10번 전투 체력 0 되면 파괴하는 함수는 웜의 함수를 이용하면 된다 그외 구현 못함 
//11번 AI 전혀 못함
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "common.h"
#include "io.h"
#include "display.h"

build_sort_switch = -1;
int cnt = 0;//유닛 넘버링 용
node a = { .is_it_my_side_flag =3, .image = {
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'},
	{'U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U'}
} 
};//esc나 아무의미없는 주소 리턴용
node* select_unit_address = &a;


int w_cnt = 0; //그냥 검사용
POSITION A = { 0, 60 };// 그냥 검사용

void display_status_message();
void display_command_message();
void display_sys_message();


char total_sys_message[20][200] = { 
	"A new harvester ready         ", 
	"Not enough spice             ",
	"아군이 웜에게 당했습니다.    ",
	"인구 수용량이 부족합니다.     ",
	"Show me the spice            ",
	"이곳엔 건설이 불가능합니다.   "
};
char curr_sys_cammand_message[6][200];

extern node* head;// 링크드 리스트 해드
void insertfrontnode(OBJECT_SAMPLE);// 링크드 리스트 노드 추가 함수
void insertfrontnode_pre(OBJECT_SAMPLE, POSITION, int);
void poop(int, int);
int eat_unit(int, int);// 링크드 리스트 노드 제거함수

void insert_sys_message(int);

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
//void sample_obj_move(void);

node* is_there_unit(void);// 커서선택 위치에 있는 유닛을 보여주는 함수/ 링크드 리스트 순회//링크드 리스트 순회

void total_object_move(void);

//POSITION sample_obj_next_position(void);
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
	.spice = 10,
	.spice_max = 100,
	.population = 0,
	.population_max = 100
};

/*OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {GAME_HEIGHT - 5, GAME_WIDTH - 20},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};*/

OBJECT_SAMPLE H = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','B','B','M','M'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','Q','M','M','M'},
	{'M','M','M','M','U','U','M','M','M','M','M','M','M','Q','R','M','M'},
	{'M','M','M','T','T','Q','T','U','U','U','U','U','U','U','R','T','T'},
	{'T','T','T','T','T','T','U','U','U','U','R','R','R','R','R','R','T'},
	{'T','T','R','R','R','R','U','U','U','U','R','R','R','R','R','R','R'},
	{'T','Q','Q','Q','R','R','U','U','U','R','R','R','R','R','R','R','R'},
	{'T','Q','Q','Q','U','U','T','T','T','T','T','T','T','T','T','T','T'}
},
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
	.as = 0,
	.curr_hp = 70,
	.max_hp = 70,
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
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','b','b','M','M'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','Q','M','M','M'},
	{'M','M','M','M','U','U','M','M','M','M','M','M','M','Q','R','M','M'},
	{'M','M','M','T','T','Q','T','U','U','U','U','U','U','U','R','T','T'},
	{'T','T','T','T','T','T','U','U','U','U','R','R','R','R','R','R','T'},
	{'T','T','R','R','R','R','U','U','U','U','R','R','R','R','R','R','R'},
	{'T','Q','Q','Q','R','R','U','U','U','R','R','R','R','R','R','R','R'},
	{'T','Q','Q','Q','U','U','T','T','T','T','T','T','T','T','T','T','T'}
},
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
	.as = 0,
	.curr_hp = 70,
	.max_hp = 70,
	.vision = 0,
	.size = 1,

	.select_flag = 0,
	.cmd_flag_round_move = 0,
	.cmd_flag_move = 0,

	.possible_cmd = {1, 1, 1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1}, // 이렇게 하면 처음부터 가만히 있게 할수있다 그리고 나중에 다른 명령들어오면 풀어주자
	.is_it_my_side_flag = 0,
	.is_it_structure_flag = 1// 0이면 건물 / 1이면 유닛
};

OBJECT_SAMPLE W = {
	.image = {
	{'M','M','M','M','T','T','T','T','T','T','T','T','T','T','T','T','T'},
	{'M','M','T','T','T','T','T','T','T','T','T','T','T','T','T','M','M'},
	{'M','M','T','T','R','R','Q','T','T','T','T','T','T','T','M','M','A'},
	{'M','M','T','U','R','R','Q','Q','T','T','T','T','T','M','M','M','A'},
	{'M','M','T','U','R','R','Q','Q','T','T','T','T','T','M','M','M','A'},
	{'M','M','T','T','R','R','Q','T','T','T','T','T','M','M','M','M','A'},
	{'M','M','M','T','T','T','T','T','T','M','M','M','M','M','M','M','M'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'}
},
	.pos = {0},
	.dest = {0},
	.src = {0},
	.repr = 'W',
	.speed = 900,
	.next_move_time = 300,
	.str = 9999,
	.curr_hp = 9999,
	.max_hp = 9999,
	.as = 9999,
	.vision = 9999,
	.size = 1,

	.select_flag = 0,
	.cmd_flag_round_move = 0,
	.cmd_flag_move = 0,

	.possible_cmd = {1, 1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {0}, // 이렇게 하면 처음부터 가만히 있게 할수있다 그리고 나중에 다른 명령들어오면 풀어주자
	.is_it_my_side_flag = 2,
	.is_it_structure_flag = 1

};

OBJECT_SAMPLE Dormitory_BLUE = {
	.image = {
	{'M','M','M','U','U','U','U','U','U','U','U','U','R','R','B','B','B'},
	{'M','M','M','U','Q','Q','Q','U','U','U','Q','Q','Q','R','B','B','B'},
	{'M','M','M','U','U','U','U','U','U','U','U','U','R','R','M','M','M'},
	{'M','M','M','U','Q','Q','Q','U','U','U','Q','Q','Q','R','T','T','T'},
	{'T','T','T','U','U','U','U','U','Q','U','U','U','R','R','P','P','P'},
	{'P','P','P','U','Q','Q','Q','U','Q','U','Q','Q','Q','R','P','P','P'},
	{'P','P','P','U','U','U','U','U','Q','U','U','U','R','R','P','P','P'},
	{'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = 50,
	.max_hp = 50,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 0,
	.repr = 'D',


	.possible_cmd = {0,0,0,0,0},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1, 0},// 이렇게 해서 처음부터 가만히 있게하고 건물이니 풀지말자 저 1 
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 1
};
OBJECT_SAMPLE Dormitory_RED = {
	.image = {
	{'M','M','M','U','U','U','U','U','U','U','U','U','R','R','b','b','b'},
	{'M','M','M','U','Q','Q','Q','U','U','U','Q','Q','Q','R','b','b','b'},
	{'M','M','M','U','U','U','U','U','U','U','U','U','R','R','M','M','M'},
	{'M','M','M','U','Q','Q','Q','U','U','U','Q','Q','Q','R','T','T','T'},
	{'T','T','T','U','U','U','U','U','Q','U','P','P','R','R','p','p','p'},
	{'p','p','p','U','Q','Q','Q','U','Q','U','Q','Q','Q','R','p','p','p'},
	{'p','p','p','U','U','U','U','U','Q','U','U','U','R','R','p','p','p'},
	{'p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = 50,
	.max_hp = 50,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 0,
	.repr = 'd',


	.possible_cmd = {0,0,0,0,0},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1, 0},// 이렇게 해서 처음부터 가만히 있게하고 건물이니 풀지말자 저 1 
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 0
};

OBJECT_SAMPLE BASE_BLUE = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','B','B','B','B'},
	{'M','M','M','M','U','M','U','M','U','M','U','M','U','B','B','B','B'},
	{'M','M','M','M','U','U','U','U','U','U','U','U','U','M','M','M','M'},
	{'M','M','M','T','U','Q','U','Q','U','Q','U','Q','U','T','T','T','T'},
	{'T','T','T','T','U','U','U','U','U','U','U','U','U','P','P','P','P'},
	{'P','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','P'},
	{'P','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','P'},
	{'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = 50,
	.max_hp = 50,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 0,
	.repr = 'B',


	.possible_cmd = {0,0,0,0,1},// 0: 정지 // 1:(H) 하베스터 채굴 // 2:(M) move // 3:(P) 순찰 // 4:(H) 하베스터 생산
	.allive_cmd = {1, 0},// 이렇게 해서 처음부터 가만히 있게하고 건물이니 풀지말자 저 1 
	.is_it_structure_flag =0,
	.is_it_my_side_flag = 1
};
OBJECT_SAMPLE BASE_RED = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','b','b','b','b'},
	{'M','M','M','M','U','M','U','M','U','M','U','M','U','b','b','b','b'},
	{'M','M','M','M','U','U','U','U','U','U','U','U','U','M','M','M','M'},
	{'M','M','M','T','U','Q','U','Q','U','Q','U','Q','U','T','T','T','T'},
	{'T','T','T','T','U','U','U','U','U','U','U','U','U','p','p','p','p'},
	{'p','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','p'},
	{'p','U','U','U','U','U','U','U','U','U','U','U','U','U','U','U','p'},
	{'p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = 50,
	.max_hp = 50,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 0,
	.repr = 'b',


	.possible_cmd = {0,0,0,0,1},
	.allive_cmd = {1},
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 0
};


OBJECT_SAMPLE PLATE_BLUE = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','B','B','B','B'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','B','B','B','B'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'},
	{'M','M','M','T','T','T','T','T','T','T','T','T','T','T','T','T','T'},
	{'T','T','T','T','P','P','P','P','P','P','P','P','P','P','P','P','P'},
	{'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
	{'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
	{'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = -1,
	.max_hp = -1,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 1,
	.repr = 'P',


	.possible_cmd = {0},
	.allive_cmd = {1},
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 1
};
OBJECT_SAMPLE PLATE_RED = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','b','b','b','b'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','b','b','b','b'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'},
	{'M','M','M','T','T','T','T','T','T','T','T','T','T','T','T','T','T'},
	{'T','T','T','T','p','p','p','p','p','p','p','p','p','p','p','p','p'},
	{'p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p'},
	{'p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p'},
	{'p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p','p'}
},
	.pos = {0},
	.size = 2,
	.curr_hp = -1,
	.max_hp = -1,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.cost = 1,
	.repr = 'p',


	.possible_cmd = {0},
	.allive_cmd = {1},
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 0
};

OBJECT_SAMPLE SPICE = {
	.image = {
	{'M','M','M','M','M','M','M','M','A','E','M','M','M','M','M','M','M'},
	{'M','M','M','M','M','M','M','A','A','A','E','M','M','M','M','M','M'},
	{'M','M','M','M','M','A','A','A','A','A','A','A','E','M','M','M','M'},
	{'M','M','M','T','A','A','A','A','A','A','A','A','A','E','T','T','T'},
	{'T','T','T','T','A','A','A','A','A','A','A','A','A','E','T','T','T'},
	{'T','T','T','T','T','A','A','A','A','A','A','A','E','T','T','T','T'},
	{'T','T','T','T','T','T','T','A','A','A','E','T','T','T','T','T','T'},
	{'T','T','T','T','T','T','T','T','A','E','T','T','T','T','T','T','T'}
},
	.curr_hp = -1,
	.max_hp= -1,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.size = 1,
	.rest_spice = 0,
	.repr = '5',
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 2,
	.allive_cmd = {1}
};//하베스트가 스파이스를 채취했다의 조건이 참일때  .repr의 아스키를 -1로 하고 레스트 스파이스도 -1로 줄이자 그리고 0되면 노드 삭제 // 웜이 생산 할때는 랜덤으로 숫자가 나와야하는데 이것도 랜덤값을 
//대입하자 repr에 0~5까지의 아스키코드를 랜덤으로 해서 대입되게 // 하베스터나 다른 건물들도 문자 보이게 할지 고민 더 직관적이긴 할듯
OBJECT_SAMPLE ROCK = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'},
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'},
	{'M','M','M','M','M','U','U','U','U','U','R','M','M','M','M','M','M'},
	{'M','M','M','T','T','U','U','U','U','U','U','R','T','T','T','T','T'},
	{'T','T','T','T','T','U','U','U','U','U','U','R','T','T','T','T','T'},
	{'T','T','T','T','T','U','U','U','U','U','U','R','T','T','T','T','T'},
	{'T','T','T','T','T','T','T','T','T','T','T','T','T','T','T','T','T'},
	{'T','T','T','T','T','T','T','T','T','T','T','T','T','T','T','T','T'}
},
	.curr_hp = -1,
	.max_hp = -1,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.size = 1,
	.repr = 'R',
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 2,
	.allive_cmd = {1}
};

OBJECT_SAMPLE BIG_ROCK = {
	.image = {
	{'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M'},
	{'M','M','M','M','M','M','U','U','U','U','U','U','U','U','M','M','M'},
	{'M','M','M','M','M','M','U','U','U','U','U','U','U','U','U','M','M'},
	{'M','M','U','U','U','U','U','R','U','U','U','U','U','U','U','T','T'},
	{'T','T','U','U','U','U','U','U','R','U','U','U','U','U','U','T','T'},
	{'T','U','U','U','U','U','U','U','R','U','U','U','U','U','U','T','T'},
	{'T','U','U','U','U','U','U','U','R','U','U','U','U','U','U','T','T'},
	{'T','U','U','U','U','U','U','U','R','T','T','T','T','T','T','T','T'}
},
	.curr_hp = -1,
	.max_hp = -1,
	.str = -1,
	.as = -1,
	.vision = -1,
	.speed = -1,
	.size = 2,
	.repr = 'R',
	.is_it_structure_flag = 0,
	.is_it_my_side_flag = 2,
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
			case k_space:// 스페이스바나 esc할때 cursor.current 주위의 것들을 원래색으로 바꿔줘야함//
				// 그방법은 그 스위치로 그안에 문자를 기준으로 색 입히자 이거진짜 배열로 만들 필요성 있다
				// 아 그리고 건물건설할때 cursor.crrrent의 주위를 직접적으로 해서 문자가 있으면 건설못하게하자
				//POSITION current_cursor_pos = { cursor.current.row, cursor.current.column};
				

				// 건축 범위안에 유닛이나 건물있으면 건축 안되도록
				if (build_sort_switch != -1) {//
					if (map[0][cursor.current.row][cursor.current.column] == ' ' && map[0][cursor.current.row + 1][cursor.current.column] == ' ' && map[0][cursor.current.row][cursor.current.column + 1] == ' ' && map[0][cursor.current.row + 1][cursor.current.column + 1] == ' ' && map[1][cursor.current.row][cursor.current.column] == -1 && map[1][cursor.current.row + 1][cursor.current.column] == -1 && map[1][cursor.current.row][cursor.current.column + 1] == -1 && map[1][cursor.current.row + 1][cursor.current.column + 1] == -1)//
					{//
						switch (build_sort_switch) {//
						case 0://
							insertfrontnode_pre(PLATE_BLUE, cursor.current, 1);//
							build_sort_switch = -1;//
							break;//
						case 1://
							insertfrontnode_pre(Dormitory_BLUE, cursor.current, 1);//
							build_sort_switch = -1;//
							break;//

						}//
					}//
					else {//
						insert_sys_message(5);//
						build_sort_switch = -1;//
					}//
				}//
				select_cursor = cursor.current;// 
				if (is_there_unit() != &a) {/// is_there_unit에서 내가 선택한 위치에 유닛, 건물이 없으면 a의 주소를 반환함
					select_unit_address = is_there_unit();/// 만약 내가 하베스터를 선택한 상태로 하베스터가 죽어서 free가 되면 없는걸 참조해서 출력하니 멈추려나
				}//

				if (select_unit_address->allive_cmd[1] == 1) {//
					select_unit_address->dest.row = select_cursor.row;//
					select_unit_address->dest.column = select_cursor.column;//
					
					//select_unit_address->allive_cmd[1] = 0;
					//select_unit_address->allive_cmd[0] = 1;// allive_cmd[0] 이 1이면 정지를 의미함
				} //
				esc_switch = 0;//
				b_m_switch = 0;//
				b_switch = 0;//
				break;///select_flag는 좋은데 꼭 필요한진 모르겠다.
			
			case k_esc: esc_switch = 1;//
				select_unit_address = &a;//
				b_m_switch = 0;//
				b_switch = 0;//
				break;/// is_there_unit이 주소를 리턴하게 하면 많은것을 할수있다. 이걸로 프로필 출력도 하자. 

			case k_make_and_har_h: 
				if (select_unit_address->repr == 'B') {
					if (resource.spice < H.cost || resource.population + H.population > resource.population_max) {
						if (resource.spice < H.cost) {
							insert_sys_message(1);
						}
						else {
							insert_sys_message(3);
						}
					}
					if (resource.spice >= H.cost && resource.population + H.population <= resource.population_max) {
						insert_sys_message(0);
						resource.spice -= H.cost;
						resource.population += H.population;
						//여기다 시스템 메시지 출력 기능넣을까? 이미 조건은 갖춰져있는데
						//배열로 만들어볼까 이차원배열로 하면 쌉가능 일단 분리먼저
						insertfrontnode(H); break;
					}
				}
			
			case k_mining_t:// 
				if (select_unit_address->possible_cmd[1] == 1) { //
					select_unit_address->allive_cmd[0] = 0; //
					select_unit_address->allive_cmd[1] = 1;///시작하자마자 t를 누르면 멈춰버린다 null을 참조해서 그런듯
					select_unit_address->allive_cmd[2] = 0;//
				} break;// 아직 완벽하게는 안됨//
				//
			case k_build:
				b_switch = 1;
				esc_switch = 1;
				select_unit_address = &a;
				break;

			case k_build_plate:
				if (b_switch == 1) {
					b_m_switch = 1;// 얘는 커서 크기 변경시키는 용 스위치임
					build_sort_switch = 0;
				}
				break;

			case k_build_Dor:
				if (b_switch == 1) {
					b_m_switch = 1;// 얘는 커서 크기 변경시키는 용 스위치임
					build_sort_switch = 1;
				}
				break;


			case k_move_m:
				if (resource.spice_max >= resource.spice + 5) {
					resource.spice += 5;
					insert_sys_message(4);
				}
				
				/*if (select_unit_address->possible_cmd[2] == 1) {
					select_unit_address->allive_cmd[0] = 0;
					select_unit_address->allive_cmd[1] = 0;
					select_unit_address->allive_cmd[2] = 1;
				}break;*/
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
		//sample_obj_move();
		total_object_move();//
		

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

void display_command_message() {
		POSITION command_pos = { 21, 63 };
		POSITION command_pos_2 = { 22, 63 };
		POSITION command_pos_3 = { 23, 63 };
		POSITION command_pos_4 = { 24, 63 };
		POSITION command_pos_5 = { 25, 63 };

		POSITION command_account_pos = {12, 63};
		POSITION command_account_pos_1 = { 13, 63 };
		POSITION command_account_pos_2 = { 14, 63 };
		POSITION command_account_pos_3 = { 15, 63 };
		POSITION command_account_pos_4 = { 16, 63 };
		POSITION command_account_pos_5 = { 17, 63 };

		set_color(112);

		gotoxy(command_pos);
		printf("                                  ");
		gotoxy(command_pos_2);
		printf("                                  ");
		gotoxy(command_pos_3);
		printf("                                  ");
		gotoxy(command_pos_4);
		printf("                                  ");
		gotoxy(command_pos_5);
		printf("                                  ");

		gotoxy(command_account_pos);
		printf("              ");
		gotoxy(command_account_pos_1);
		printf("                                  ");
		gotoxy(command_account_pos_2);
		printf("                                  ");
		gotoxy(command_account_pos_3);
		printf("                                  ");
		gotoxy(command_account_pos_4);
		printf("                                  ");
		gotoxy(command_account_pos_5);
		printf("                                  ");
		

		gotoxy(command_pos);
		if (b_switch == 1) {
			printf(" P : 장판            ");
			gotoxy(command_pos_2);
			printf("                     ");
			gotoxy(command_pos_3);
			printf("                     ");

			gotoxy(command_pos_5);
			printf("ESC : 건설 메뉴 취소          ");
		}
		else {
			switch (select_unit_address->repr) {
			case 'B':
				printf("H : 하베스터 생산            ");
				gotoxy(command_pos_2);
				printf("비용: 5  인구수: 5           ");

				gotoxy(command_account_pos);
				printf("명령 설명");
				gotoxy(command_account_pos_1);
				printf("유닛은 같은 위치에서 생산됩니다");
				gotoxy(command_account_pos_2);
				printf("유닛을 이동시키고 생산하면");
				gotoxy(command_account_pos_3);
				printf("생산기능 확인이 편합니다");
				// 설명을 위에 남는 공간에 추가하자
				break;
			case 'H':
				printf("T : 이동                     ");

				gotoxy(command_account_pos);
				printf("명령 설명");
				gotoxy(command_account_pos_1);
				printf("1.하베스터 선택하고 T누르기");
				gotoxy(command_account_pos_2);
				printf("2.원하는 도착지 선택하면 이동");
				gotoxy(command_account_pos_3);
				printf("3.그 이후론 T누르지 않고도");
				gotoxy(command_account_pos_4);
				printf("3.유닛선택된 상태로");
				gotoxy(command_account_pos_5);
				printf("3.도착지 누르면 이동");

				break;
			default:
				printf("사용가능한 명령이 없습니다. ");
				break;
			}
			gotoxy(command_pos_5);
			printf("B : 건물 건설               ");
		}
}
void display_status_message() {// 만약 내가 하베스터를 선택한 상태로 하베스터가 죽어서 free가 되면 없는걸 참조해서 출력하니 멈추려나
	POSITION side_pos_blue = {2, 84};
	POSITION side_pos_red = { 2, 87 };
	POSITION side_pos_yellow = { 2, 88 };

	POSITION name_pos_4 = {4, 86};
	POSITION name_pos_2_1 = { 4, 88 };

	POSITION HP_POS = { 6,82 };
	POSITION STR_POS = { 6, 92 };
	POSITION Speed_POS = { 8, 82 };
	POSITION AS_POS = { 8, 92 };
	POSITION Vision_POS = { 10,92 };

	POSITION UNIT_HP_POS = { 7,82 };
	POSITION UNIT_STR_POS = { 7, 92 };
	POSITION UNIT_Speed_POS = { 9, 82 };
	POSITION UNIT_AS_POS = { 9, 92 };
	POSITION UNIT_Vision_POS = { 11,92 };

	POSITION HP_STR_LINE_POS = { 7, 80 };
	POSITION Speed_AS_LINE_POS = { 9, 80 };
	POSITION vision_LINE_POS = { 11, 91 };
	//240
	set_color(112);
	gotoxy(HP_POS);
	printf("체력");
	gotoxy(STR_POS);
	printf("공격력");
	gotoxy(Speed_POS);
	printf("이동속도");
	gotoxy(AS_POS);
	printf("공격속도");
	gotoxy(Vision_POS);
	printf("시야");

	set_color(240);
	gotoxy(HP_STR_LINE_POS);
	printf("                    ");
	set_color(240);
	gotoxy(Speed_AS_LINE_POS);
	printf("                    ");
	gotoxy(vision_LINE_POS);
	printf("         ");
	
	gotoxy(UNIT_HP_POS);
	printf("%d/%d", select_unit_address->curr_hp, select_unit_address->max_hp);
	gotoxy(UNIT_Speed_POS);
	printf("%d", select_unit_address->speed);
	gotoxy(UNIT_STR_POS);
	printf("%d", select_unit_address->str);
	gotoxy(UNIT_AS_POS);
	printf("%d", select_unit_address->as);
	gotoxy(UNIT_Vision_POS);
	printf("%d", select_unit_address->speed);


	set_color(112);
	gotoxy(name_pos_4);
	printf("            ");
	switch (select_unit_address->repr) {
	case 'h':
	case 'H':
		gotoxy(name_pos_4);
		printf("하베스터"); 
		break;
	case '5':
	case '4':
	case '3':
	case '2':
	case '1':
		gotoxy(name_pos_4);
		printf("스파이스"); 
		break;
	case 'R':
		gotoxy(name_pos_4);
			if(select_unit_address->size == 1){
				printf("작은바위");
			}
			else {
				printf("큰 바위");
			}
		break;

	case 'B':
	case 'b':
		gotoxy(name_pos_2_1);
		printf("본진");
		break;
	case 'P':
	case 'p':
		gotoxy(name_pos_2_1);
		printf("장판");
		break;
	case 'W':
		gotoxy(name_pos_2_1);
		printf("웜");
		break;
	case 'D':
	case 'd':
		gotoxy(name_pos_2_1);
		printf("숙소");
		break;
	}
	


	gotoxy(side_pos_blue);
	printf("            ");
	switch (select_unit_address->is_it_my_side_flag) {
	case 0: 
		gotoxy(side_pos_red);
		set_color(116); 
		printf("하코넨");  

		break;
	case 1: 
		gotoxy(side_pos_blue);
		set_color(113); 
		printf("아트레이디스");  
		break;
	case 2: 
		gotoxy(side_pos_yellow);
		set_color(118); 
		printf("중립");
		break;
	case 3:
		gotoxy(side_pos_yellow);
		set_color(118);
		printf("            ");
		break;
	}

	/*if (select_unit_address->is_it_my_side_flag == 0) {
		set_color(116); 
		printf("하코넨");
	}*/
	



	//========== 이미지 출력 ==================================
	map[0][1][62] = select_unit_address->image[0][0];// 이유를 모르겠다 배열로 하면 이상하게 출력된다....
	map[0][1][63] = select_unit_address->image[0][1];
	map[0][1][64] = select_unit_address->image[0][2];
	map[0][1][65] = select_unit_address->image[0][3];
	map[0][1][66] = select_unit_address->image[0][4];
	map[0][1][67] = select_unit_address->image[0][5];
	map[0][1][68] = select_unit_address->image[0][6];
	map[0][1][69] = select_unit_address->image[0][7];
	map[0][1][70] = select_unit_address->image[0][8];
	map[0][1][71] = select_unit_address->image[0][9];
	map[0][1][72] = select_unit_address->image[0][10];
	map[0][1][73] = select_unit_address->image[0][11];
	map[0][1][74] = select_unit_address->image[0][12];
	map[0][1][75] = select_unit_address->image[0][13];
	map[0][1][76] = select_unit_address->image[0][14];
	map[0][1][77] = select_unit_address->image[0][15];
	map[0][1][78] = select_unit_address->image[0][16];

	map[0][2][62] = select_unit_address->image[1][0];
	map[0][2][63] = select_unit_address->image[1][1];
	map[0][2][64] = select_unit_address->image[1][2];
	map[0][2][65] = select_unit_address->image[1][3];
	map[0][2][66] = select_unit_address->image[1][4];
	map[0][2][67] = select_unit_address->image[1][5];
	map[0][2][68] = select_unit_address->image[1][6];
	map[0][2][69] = select_unit_address->image[1][7];
	map[0][2][70] = select_unit_address->image[1][8];
	map[0][2][71] = select_unit_address->image[1][9];
	map[0][2][72] = select_unit_address->image[1][10];
	map[0][2][73] = select_unit_address->image[1][11];
	map[0][2][74] = select_unit_address->image[1][12];
	map[0][2][75] = select_unit_address->image[1][13];
	map[0][2][76] = select_unit_address->image[1][14];
	map[0][2][77] = select_unit_address->image[1][15];
	map[0][2][78] = select_unit_address->image[1][16];

	map[0][3][62] = select_unit_address->image[2][0];
	map[0][3][63] = select_unit_address->image[2][1];
	map[0][3][64] = select_unit_address->image[2][2];
	map[0][3][65] = select_unit_address->image[2][3];
	map[0][3][66] = select_unit_address->image[2][4];
	map[0][3][67] = select_unit_address->image[2][5];
	map[0][3][68] = select_unit_address->image[2][6];
	map[0][3][69] = select_unit_address->image[2][7];
	map[0][3][70] = select_unit_address->image[2][8];
	map[0][3][71] = select_unit_address->image[2][9];
	map[0][3][72] = select_unit_address->image[2][10];
	map[0][3][73] = select_unit_address->image[2][11];
	map[0][3][74] = select_unit_address->image[2][12];
	map[0][3][75] = select_unit_address->image[2][13];
	map[0][3][76] = select_unit_address->image[2][14];
	map[0][3][77] = select_unit_address->image[2][15];
	map[0][3][78] = select_unit_address->image[2][16];

	map[0][4][62] = select_unit_address->image[3][0];
	map[0][4][63] = select_unit_address->image[3][1];
	map[0][4][64] = select_unit_address->image[3][2];
	map[0][4][65] = select_unit_address->image[3][3];
	map[0][4][66] = select_unit_address->image[3][4];
	map[0][4][67] = select_unit_address->image[3][5];
	map[0][4][68] = select_unit_address->image[3][6];
	map[0][4][69] = select_unit_address->image[3][7];
	map[0][4][70] = select_unit_address->image[3][8];
	map[0][4][71] = select_unit_address->image[3][9];
	map[0][4][72] = select_unit_address->image[3][10];
	map[0][4][73] = select_unit_address->image[3][11];
	map[0][4][74] = select_unit_address->image[3][12];
	map[0][4][75] = select_unit_address->image[3][13];
	map[0][4][76] = select_unit_address->image[3][14];
	map[0][4][77] = select_unit_address->image[3][15];
	map[0][4][78] = select_unit_address->image[3][16];

	map[0][5][62] = select_unit_address->image[4][0];
	map[0][5][63] = select_unit_address->image[4][1];
	map[0][5][64] = select_unit_address->image[4][2];
	map[0][5][65] = select_unit_address->image[4][3];
	map[0][5][66] = select_unit_address->image[4][4];
	map[0][5][67] = select_unit_address->image[4][5];
	map[0][5][68] = select_unit_address->image[4][6];
	map[0][5][69] = select_unit_address->image[4][7];
	map[0][5][70] = select_unit_address->image[4][8];
	map[0][5][71] = select_unit_address->image[4][9];
	map[0][5][72] = select_unit_address->image[4][10];
	map[0][5][73] = select_unit_address->image[4][11];
	map[0][5][74] = select_unit_address->image[4][12];
	map[0][5][75] = select_unit_address->image[4][13];
	map[0][5][76] = select_unit_address->image[4][14];
	map[0][5][77] = select_unit_address->image[4][15];
	map[0][5][78] = select_unit_address->image[4][16];

	map[0][6][62] = select_unit_address->image[5][0];
	map[0][6][63] = select_unit_address->image[5][1];
	map[0][6][64] = select_unit_address->image[5][2];
	map[0][6][65] = select_unit_address->image[5][3];
	map[0][6][66] = select_unit_address->image[5][4];
	map[0][6][67] = select_unit_address->image[5][5];
	map[0][6][68] = select_unit_address->image[5][6];
	map[0][6][69] = select_unit_address->image[5][7];
	map[0][6][70] = select_unit_address->image[5][8];
	map[0][6][71] = select_unit_address->image[5][9];
	map[0][6][72] = select_unit_address->image[5][10];
	map[0][6][73] = select_unit_address->image[5][11];
	map[0][6][74] = select_unit_address->image[5][12];
	map[0][6][75] = select_unit_address->image[5][13];
	map[0][6][76] = select_unit_address->image[5][14];
	map[0][6][77] = select_unit_address->image[5][15];
	map[0][6][78] = select_unit_address->image[5][16];

	map[0][7][62] = select_unit_address->image[6][0];
	map[0][7][63] = select_unit_address->image[6][1];
	map[0][7][64] = select_unit_address->image[6][2];
	map[0][7][65] = select_unit_address->image[6][3];
	map[0][7][66] = select_unit_address->image[6][4];
	map[0][7][67] = select_unit_address->image[6][5];
	map[0][7][68] = select_unit_address->image[6][6];
	map[0][7][69] = select_unit_address->image[6][7];
	map[0][7][70] = select_unit_address->image[6][8];
	map[0][7][71] = select_unit_address->image[6][9];
	map[0][7][72] = select_unit_address->image[6][10];
	map[0][7][73] = select_unit_address->image[6][11];
	map[0][7][74] = select_unit_address->image[6][12];
	map[0][7][75] = select_unit_address->image[6][13];
	map[0][7][76] = select_unit_address->image[6][14];
	map[0][7][77] = select_unit_address->image[6][15];
	map[0][7][78] = select_unit_address->image[6][16];

	map[0][8][62] = select_unit_address->image[7][0];
	map[0][8][63] = select_unit_address->image[7][1];
	map[0][8][64] = select_unit_address->image[7][2];
	map[0][8][65] = select_unit_address->image[7][3];
	map[0][8][66] = select_unit_address->image[7][4];
	map[0][8][67] = select_unit_address->image[7][5];
	map[0][8][68] = select_unit_address->image[7][6];
	map[0][8][69] = select_unit_address->image[7][7];
	map[0][8][70] = select_unit_address->image[7][8];
	map[0][8][71] = select_unit_address->image[7][9];
	map[0][8][72] = select_unit_address->image[7][10];
	map[0][8][73] = select_unit_address->image[7][11];
	map[0][8][74] = select_unit_address->image[7][12];
	map[0][8][75] = select_unit_address->image[7][13];
	map[0][8][76] = select_unit_address->image[7][14];
	map[0][8][77] = select_unit_address->image[7][15];
	map[0][8][78] = select_unit_address->image[7][16];
	//=========  이미지 출력 끝==========================

	
}


void insert_sys_message(int select_cammand) {
	for (int i = 0; i < 5; i++) {
		strcpy_s(curr_sys_cammand_message[5 - i], sizeof(curr_sys_cammand_message[i]), curr_sys_cammand_message[5 - i - 1]);
	}
	strcpy_s(curr_sys_cammand_message[0], sizeof(curr_sys_cammand_message[0]), total_sys_message[select_cammand]);

}

void display_sys_message() {
	POSITION SYS_0 = { 25, 2 };
	POSITION SYS_1 = { 24, 2 };
	POSITION SYS_2 = { 23, 2 };
	POSITION SYS_3 = { 22, 2 };
	POSITION SYS_4 = { 21, 2 };
	set_color(112);
		gotoxy(SYS_0);
		printf("%s", curr_sys_cammand_message[0]);
		gotoxy(SYS_1);
		printf("%s", curr_sys_cammand_message[1]);
		gotoxy(SYS_2);
		printf("%s", curr_sys_cammand_message[2]);
		gotoxy(SYS_3);
		printf("%s", curr_sys_cammand_message[3]);
		gotoxy(SYS_4);
		printf("%s", curr_sys_cammand_message[4]);
}

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

	for (int i = 1; i <= 8; i++) {
		map[0][i][79] = '#';
	}
	for (int i = 62; i <= 79; i++) {
		map[0][9][i] = '#';
	}
	for (int i = 80; i < 100; i++) {
		map[0][2][i] = '#';
	}
	for (int i = 80; i < 100; i++) {
		map[0][4][i] = '#';
	}
	for (int i = 80; i < 91; i++) {
		map[0][9][i] = '#';
	}
	for (int i = 90; i < 100; i++) {
		map[0][11][i] = '#';
	}
	map[0][10][90] = '#';
	
		/*POSITION HP_STR_LINE_POS = {7, 80};
	POSITION Speed_AS_LINE_POS = { 9, 80 };
	POSITION vision_LINE_POS = { 11, 91 };*/



	//아마 레이어 0에 건물을 지어줘야하는데 함수로 크기, 색, 문자, 위치 입력 받아서 만들면 될듯
	
	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}
	resource.population += H.population;

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

	insertfrontnode_pre(W, pre_unit_pos_W, 2);
	insertfrontnode_pre(W, pre_unit_pos_W_2, 2);

	insertfrontnode_pre(BASE_BLUE, pre_struct_pos_blue_base, 1);// 지금은 초기 배치니까 위치는 내가 직접입력 pos 하나만
	insertfrontnode_pre(PLATE_BLUE, pre_struct_pos_blue_plate, 1);

	insertfrontnode_pre(BASE_RED, pre_struct_pos_red_base, 0);
	insertfrontnode_pre(PLATE_RED, pre_struct_pos_red_plate, 0);

	insertfrontnode_pre(SPICE, pre_struct_pos_blue_spice, 2);
	insertfrontnode_pre(SPICE, pre_struct_pos_red_spice, 2);
	
	insertfrontnode_pre(h, pre_unit_pos_h, 0);

	insertfrontnode_pre(H, pre_unit_pos_H, 1);
	



	insertfrontnode_pre(ROCK, pre_struct_pos_rock_1, 2);
	insertfrontnode_pre(ROCK, pre_struct_pos_rock_2, 2);
	insertfrontnode_pre(ROCK, pre_struct_pos_rock_3, 2);
	insertfrontnode_pre(BIG_ROCK, pre_struct_pos_big_rock_1, 2);
	insertfrontnode_pre(BIG_ROCK, pre_struct_pos_big_rock_2, 2);
	
	//w이 가장 아래에서 추가 되어있을때 이상하게 w의 속도가 300이상이면 total_object_move 전체가 w의 속도에 맞춰 작동했었음 근데 w를 맨위에 넣으니까 또 괜찮아짐 이유를 전혀 알수없음
	// 이 오류 해결하고 싶으면 노드위치에 관련이 있는것 같으니 생각해보고 일단은 넘어가자


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
	newnode->curr_hp = unit_sort.curr_hp;
	newnode->max_hp = unit_sort.max_hp;
	newnode->move_period = unit_sort.move_period;
	newnode->next_move_time = unit_sort.next_move_time;
	newnode->population = unit_sort.population;
	newnode->pos = pre_pos;
	newnode->repr = unit_sort.repr;
	newnode->speed = unit_sort.speed;
	newnode->str = unit_sort.str;
	newnode->as = unit_sort.as;
	newnode->vision = unit_sort.vision;
	newnode->unit_cnt = cnt;//얜 없네 머지

	newnode->select_flag = unit_sort.select_flag;
	newnode->cmd_flag_round_move = unit_sort.cmd_flag_round_move;
	newnode->cmd_flag_move = unit_sort.cmd_flag_move;

	newnode->size = unit_sort.size;
	newnode->rest_spice = unit_sort.rest_spice;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 17; j++) {
			newnode->image[i][j] = unit_sort.image[i][j];
		}
	}

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
					map[1][newnode->pos.row + i][newnode->pos.column + j] = newnode->repr;// 이렇게 하면 일단 몹들이 벽뚫는거 고쳐져서 편하긴한데 나중에 건물 파괴되면 map0, 1둘다 지워줘야함
				}
			}
		}
	}

	if (unit_sort.repr == '5') {
		newnode->is_it_my_side_flag = 2;
		newnode->repr = unit_sort.repr - side;
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
	if (sys_clock - last_clock < 120 && (last_clock + 10 != sys_clock)) {//last_clock + 10 != sys_clock 요거 작동안함 용도는 연속으로 같은키들어오면 그건 한칸씩이동 
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
				for (int i = 0; i < 3; i++) {
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



double distance(POSITION p1, POSITION p2) {
	return sqrt(pow(p2.column - p1.column, 2) + pow(p2.row - p1.row, 2));

}

node* who_is_the_closest(POSITION W_POS) {
	node* curnode;
	curnode = head;
	
	double closest_dist = 10000;
	node* closest_dist_address = &a;
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
//
void poop(int column, int row) {
		int r = rand() % 100 + 1;
		if (r <= 5) {
			int s = rand() % 5;
			POSITION A = { row, column };
			insertfrontnode_pre(SPICE, A, s);

		}
}

int eat_unit(int row, int column) {//링크드 리스트 노드 삭제/
	node* delnode;// 삭제할 노드 주소 저장///
	node* prevnode;// 삭제할 이전 노드의 주소 저장///
	if (head == NULL) {///
		return;///
	}///

	if (head->pos.column == column && head->pos.row == row) {///
		delnode = head;///
		head = head->next;///
		if (delnode->is_it_structure_flag == 0) {///
			return 1;///
		}///
		else {///
			if (delnode->is_it_my_side_flag == 1 && delnode->is_it_structure_flag == 1) {///
				insert_sys_message(2);///
				resource.population -= delnode->population;///
			}///
			map[1][row][column] = -1;///
			if (select_unit_address->repr == delnode->repr) {///
				select_unit_address = &a;///
			}///
			free(delnode);///
			
		
			
			//// 여기다가 시스템 메시지 조건 내팀 맞음? 으로해서 맞으면 죽었다
		}///
		return;///
	}///

	prevnode = head;///
	delnode = head;///
	while (prevnode->next != NULL) {///
		delnode = prevnode->next;///

		if (delnode->pos.column == column && delnode->pos.row == row) {///
			prevnode->next = delnode->next;///
			if (delnode->is_it_structure_flag == 0) {///
				return 1;///
			}///
			else {///
				if (delnode->is_it_my_side_flag == 1 && delnode->is_it_structure_flag == 1) {///
					insert_sys_message(2);///
					resource.population -= delnode->population;///
				}///
				map[1][row][column] = -1;///
				if (select_unit_address->repr == delnode->repr) {///
					select_unit_address = &a;///
				}///
				free(delnode);///
				
			}///
			return;///
		}///
		prevnode = delnode;///
	}///
}///

POSITION total_object_next_position(node* curnode) {//
	POSITION diff = psub(curnode->dest, curnode->pos);//
	DIRECTION dir;//
	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	/*if (select_unit_address->allive_cmd[0] != 1) {
		return curnode->pos;
	}*/ 

	

	if (curnode->allive_cmd[0] == 1) {//
		return curnode->pos;//
	}// 이런식으로 움직임 커멘드 조절하자// 

	if (curnode->repr == 'W') {//
		poop(curnode->pos.column, curnode->pos.row);//
		node* closest_address;//
		closest_address = who_is_the_closest(curnode->pos);//
		if (closest_address != &a) {////
			POSITION new_dest = { closest_address->pos.row, closest_address->pos.column };//
			curnode->dest = new_dest;//
		}//
		else {//
			return curnode->pos;//
		}//
	}//

	if (diff.row == 0 && diff.column == 0) {//
		if (curnode->allive_cmd[1] == 1) {//
			//curnode->allive_cmd[1] = 0;
			//curnode->allive_cmd[0] = 1;
			return curnode->pos;//
		}//
		if (curnode->dest.row == curnode->src.row && curnode->dest.column == curnode->src.column) {//
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { curnode->dest.row, curnode->dest.column };//
			curnode->dest = new_dest;//
		}//
		else {//
			POSITION new_dest = { curnode->src.row, curnode->src.column};//
			curnode->src.column = curnode->dest.column;//
			curnode->src.row = curnode->dest.row;//
			curnode->dest = new_dest;//                                
		}//
		return curnode->pos;//
	}//
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {//

		dir = (diff.row >= 0) ? d_down : d_up;//
	}//
	else {//
		dir = (diff.column >= 0) ? d_right : d_left;//
	}//
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(curnode->pos, dir);//
	
	
	if (curnode->repr == 'W' && map[1][next_pos.row][next_pos.column] != -1) {//
		//if(){next_pos = curnode->pos}
		if (eat_unit(next_pos.row, next_pos.column) == 1) {//
			next_pos.row = curnode->pos.row;//
			next_pos.column = curnode->pos.column;//
		}//
			
	}//


	//
	if (1 <= next_pos.row && next_pos.row <= GAME_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= GAME_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0 ) {//
		return next_pos;//
	}//
	else {//
		return curnode->pos;  // 제자리//
	}//
}//


void total_object_move() {//
	node* curnode;//
	if (head == NULL) {//
		return;//
	}//
	curnode = head;//
	while (curnode->next != NULL) {//
		
		if (sys_clock <= curnode->next_move_time) {//	
			return;//
		}//

		/*if (curnode->repr == 'W') {
			int r = rand() % 100 + 1;
			if (r <= 40) {
				POSITION A = {curnode->pos.row,curnode->pos.column};
				insertfrontnode_pre(SPICE, A, 2);
			}
		}*/

		map[1][curnode->pos.row][curnode->pos.column] = -1;//
		curnode->pos = total_object_next_position(curnode);//
		map[1][curnode->pos.row][curnode->pos.column] = curnode->repr;//

		curnode->next_move_time = sys_clock + curnode->speed;//
		curnode = curnode->next;//
	}//
	if (sys_clock <= curnode->next_move_time) {//
		return;//
	}//

	map[1][curnode->pos.row][curnode->pos.column] = -1;//
	curnode->pos = total_object_next_position(curnode);//
	map[1][curnode->pos.row][curnode->pos.column] = curnode->repr;//

	curnode->next_move_time = sys_clock + curnode->speed;//
}//

void insertfrontnode(OBJECT_SAMPLE unit_sort)// 노드 추가
{//
	cnt++;//
	node* newnode;//
	newnode = (node*)malloc(sizeof(node));//
	newnode->color = unit_sort.color;//
	newnode->cost = unit_sort.cost;//
	newnode->dest = unit_sort.dest;//
	newnode->src = unit_sort.src;//
	newnode->curr_hp = unit_sort.curr_hp;//
	newnode->max_hp = unit_sort.max_hp;//
	newnode->move_period = unit_sort.move_period;//
	newnode->next_move_time = unit_sort.next_move_time;//
	newnode->population = unit_sort.population;//
	newnode->pos = unit_sort.pos;//
	newnode->repr = unit_sort.repr;//
	newnode->speed = unit_sort.speed;//
	newnode->str = unit_sort.str;//
	newnode->as = unit_sort.as;//
	newnode->vision = unit_sort.vision;//
	newnode->unit_cnt = cnt;//얜 없네 머지//

	newnode->select_flag = unit_sort.select_flag;//
	newnode->cmd_flag_round_move = unit_sort.cmd_flag_round_move;//
	newnode->cmd_flag_move = unit_sort.cmd_flag_move;//

	newnode->size = unit_sort.size;//

	for (int i = 0; i < 8; i++) {//
		for (int j = 0; j < 17; j++) {//
			newnode->image[i][j] = unit_sort.image[i][j];//
		}//
	}//

	for (int i = 0; i < 10; i++) {//
		newnode->possible_cmd[i] = unit_sort.possible_cmd[i];//0:(H) 하베스터 채굴 / 1:(M) move / 2:(P) 순찰    //    3:(H) 하베스터 생산 // 정지 명령도 만들자
		newnode->allive_cmd[i] = unit_sort.allive_cmd[i];//
	}//
	newnode->is_it_my_side_flag = unit_sort.is_it_my_side_flag;//
	newnode->is_it_structure_flag = unit_sort.is_it_structure_flag;//
	newnode->next = NULL;//

	map[1][newnode->pos.row][newnode->pos.column] = newnode->repr;//
	
	if (head == NULL) {//
		head = newnode;//
		return;//
	}//
	newnode->next = head;//
	head = newnode;//

}//

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

node* is_there_unit(){//// 링크드 리스트를 순회해서 내가 선택한 위치인 select_cursor와 각 유닛의 위치를 비교해서 특정 조건을 만족하는 유닛의 주소를 반환하는 함수
	node* curnode;//
	if (head == NULL) {//
		return;//// 요 리턴을 일단 아무 의미 없는 값으로 주자// 아닌데 애초에 
	}//
	curnode = head;//
	while (curnode->next != NULL) {//
		if (curnode->size == 1) {//
			if (curnode->pos.column == select_cursor.column && curnode->pos.row == select_cursor.row) {//
				//printf("%d 번째의 유닛", curnode->unit_cnt);
				curnode->select_flag = 1;//
				return curnode;//
			}//
		}//
		else if (curnode->size == 2) {//
			for (int i = 0; i < curnode->size; i++) {//
				for (int j = 0; j < curnode->size; j++) {//
					if (curnode->pos.column + i == select_cursor.column && curnode->pos.row + j == select_cursor.row) {//
						return curnode;//
					}//
				}//
			}//
		}//// 그니까 만약 조건이 맞다면 curnode 구조체를 리턴하는데 만약 아니라면? 그것도 리턴을 해줘야지 무언가 
		curnode = curnode->next;//
	}//
	if (curnode->size == 1) {//
		if (curnode->pos.column == select_cursor.column && curnode->pos.row == select_cursor.row) {//
			//printf("%d 번째의 유닛", curnode->unit_cnt);
			curnode->select_flag = 1;//
			return curnode;//// 알겠다 왜 처음에 하베스터를 스페이스바로 선택하면 에러가 나는지 아니 애초에 유닛이 아닌것을 스페이스바로 선택하면 그냥 멈춰버림 그이유는 
		}//
	}//
	else if (curnode->size == 2) {//
		for (int i = 0; i < curnode->size; i++) {//
			for (int j = 0; j < curnode->size; j++) {//
				if (curnode->pos.column + i == select_cursor.column && curnode->pos.row + j == select_cursor.row) {//
					return curnode;//
				}//
			}//
		}//
	}//
	return &a;//
}//

