#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>

/* ================= system parameters =================== */
#define TICK 10		// time unit(ms)

#define N_LAYER 2
#define MAP_WIDTH	101
#define MAP_HEIGHT	26

#define GAME_WIDTH  60
#define GAME_HEIGHT 18

#define SYSTEM_HEIGHT 7

#define COMMAND_HEIGHT 7
#define COMMAND_WIDTH 40

#define STATUS_WIDTH 40



/* ================= 위치와 방향 =================== */
// 맵에서 위치를 나타내는 구조체
typedef struct {
	int row, column;
} POSITION;

// 커서 위치
typedef struct {
	POSITION previous;  // 직전 위치
	POSITION current;   // 현재 위치
} CURSOR;

// 입력 가능한 키 종류.
// 수업에서 enum은 생략했는데, 크게 어렵지 않으니 예제 검색
typedef enum {
	// k_none: 입력된 키가 없음. d_stay(안 움직이는 경우)에 대응
	k_none = 0, k_up, k_right, k_left, k_down,
	k_quit, k_space,k_esc, k_make_and_har_h, k_move_m, k_mining_t, k_show_unit, k_build,
	k_build_plate, k_build_Dor,
	k_undef, // 정의되지 않은 키 입력	
} KEY;


// DIRECTION은 KEY의 부분집합이지만, 의미를 명확하게 하기 위해서 다른 타입으로 정의
typedef enum {
	d_stay = 0, d_up, d_right, d_left, d_down
} DIRECTION;


/* ================= 위치와 방향(2) =================== */
// 편의성을 위한 함수들. KEY, POSITION, DIRECTION 구조체들을 유기적으로 변환

// 편의성 함수
inline POSITION padd(POSITION p1, POSITION p2) {
	POSITION p = { p1.row + p2.row, p1.column + p2.column };
	return p;
}

// p1 - p2
inline POSITION psub(POSITION p1, POSITION p2) {
	POSITION p = { p1.row - p2.row, p1.column - p2.column };
	return p;
}

// 방향키인지 확인하는 함수
#define is_arrow_key(k)		(k_none <= (k) && (k) <= k_down)//바꿈

// 화살표 '키'(KEY)를 '방향'(DIRECTION)으로 변환. 정수 값은 똑같으니 타입만 바꿔주면 됨
#define ktod(k)		(DIRECTION)(k)

// DIRECTION을 POSITION 벡터로 변환하는 함수
inline POSITION dtop(DIRECTION d) {
	static POSITION direction_vector[] = { {0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0} };
	return direction_vector[d];
}

// p를 d 방향으로 이동시킨 POSITION
#define pmove(p, d)		(padd((p), dtop(d)))

/* ================= game data =================== */
typedef struct {
	int spice;		// 현재 보유한 스파이스
	int spice_max;  // 스파이스 최대 저장량
	int population; // 현재 인구 수
	int population_max;  // 수용 가능한 인구 수
} RESOURCE;


// 대강 만들어 봤음. 기능 추가하면서 각자 수정할 것
typedef struct {
	POSITION pos;		// 현재 위치(position)
	POSITION dest;// 목적지(destination)
	POSITION src;
	char repr;			// 화면에 표시할 문자(representation)
	int move_period;	// '몇 ms마다 한 칸 움직이는지'를 뜻함
	int next_move_time;	// 다음에 움직일 시간
	int speed;
	
	int color;
	int cost;
	int population;
	int str;
	int as;
	int curr_hp;
	int max_hp;
	int vision;
	int unit_cnt;

	int select_flag;
	int cmd_flag_round_move;
	int cmd_flag_move;

	int possible_cmd[10];
	int allive_cmd[10];
	int is_it_my_side_flag;
	int is_it_structure_flag;// 0이면 유닛 / 1이면 건물

	int size;
	int rest_spice;

	char image[8][17];
} OBJECT_SAMPLE;



typedef struct node
{
	POSITION pos;		// 현재 위치(position)
	POSITION dest;		// 목적지(destination)
	POSITION src;
	char repr;			// 화면에 표시할 문자(representation)
	int move_period;	// '몇 ms마다 한 칸 움직이는지'를 뜻함
	int next_move_time;	// 다음에 움직일 시간
	int speed;

	int color;
	int cost;
	int population;
	int str;
	int as;
	int curr_hp;
	int max_hp;
	int vision;
	int unit_cnt;

	int select_flag;
	int cmd_flag_round_move;
	int cmd_flag_move;

	int possible_cmd[10];// 0:(H) 하베스터 채굴 / 1:(M) move / 2:(P) 순찰    //    3:(H) 하베스터 생산
	int allive_cmd[10];
	int is_it_my_side_flag; // 0이면 아군 / 1이면 적군 / 2면 중립
	int is_it_structure_flag;// 0이면 건물 / 1이면 유닛

	int size;//건물용
	int rest_spice;//남은 스파이스양

	char image[8][17];
	
	struct node* next;
}node;



#endif
