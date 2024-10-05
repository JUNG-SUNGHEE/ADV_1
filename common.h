#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>

/* ================= system parameters =================== */
#define TICK 10		// time unit(ms)

#define N_LAYER 2//이건 지형과 오브젝트를 분리해서 처리하기위해 
#define MAP_WIDTH	60
#define MAP_HEIGHT	18

//내 코드
#define ST_LAYER 2 // 아마 틀과 텍스트 분리?
#define STATUS_WIDTH 50
#define STATUS_HEIGHT 18

#define SY_LAYER 2
#define SYSTEM_WIDTH 60
#define SYSTEM_HEIGHT 7

#define CO_LAYER 2
#define COMMAND_WIDTH 50
#define COMMAND_HEIGHT 7

//내 코드


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
	k_quit,
	k_undef, // 정의되지 않은 키 입력	
} KEY;


// DIRECTION은 KEY의 부분집합이지만, 의미를 명확하게 하기 위해서 다른 타입으로 정의
typedef enum {//enum은 열거형을 사용하는데 사용된다 순서대로 첫번째 부터 0을 할당해 이후 1,2,3,4 할당
	//그럼 
	d_stay = 0, d_up, d_right, d_left, d_down
} DIRECTION;


/* ================= 위치와 방향(2) =================== */
// 편의성을 위한 함수들. KEY, POSITION, DIRECTION 구조체들을 유기적으로 변환

// 편의성 함수
inline POSITION padd(POSITION p1, POSITION p2) {//POSITION형 함수니까 2개의 int를 반환할듯
	POSITION p = { p1.row + p2.row, p1.column + p2.column };
	return p;
}//inline은 그냥 별의미 없다고 생각하고 뒤에 함수에 집중하자

// p1 - p2
inline POSITION psub(POSITION p1, POSITION p2) {
	POSITION p = { p1.row - p2.row, p1.column - p2.column };
	return p;
}

// 방향키인지 확인하는 함수
#define is_arrow_key(k)		(k_up <= (k) && (k) <= k_down)

// 화살표 '키'(KEY)를 '방향'(DIRECTION)으로 변환. 정수 값은 똑같으니 타입만 바꿔주면 됨
#define ktod(k)		(DIRECTION)(k)

// DIRECTION을 POSITION 벡터로 변환하는 함수
inline POSITION dtop(DIRECTION d) {
	static POSITION direction_vector[] = { {0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0} };
	//d_stay == {0, 0} / d_up == {-1, 0} / d_right == {0, 1} / d_left == {0, -1} / d_down == {1, 0}
	//함수내에서 static으로 선언된 변수는 함수가 호출될 때마다 초기화되지 않고, 프로그램이 종료될 때까지 그 값을 유지한다 
	return direction_vector[d];//이 함수가 어느 방향으로 얼마나 이동시키는지를 관장하는듯
}

// p를 d 방향으로 이동시킨 POSITION
#define pmove(p, d)		(padd((p), dtop(d)))//padd함수는 2개의 POSITION형 변수를 받아 X는 X끼리 Y는 Y끼리 더해서 반환
                            //여기서 p는현재위치 d는 다음에 이동할 방향이 들어있음 dtop함수는 방향과 크기정리 되어있고 
							// 각 방향에 맞는 값을 반환해줌
							// 오호 그럼 padd함수로 이 둘을 더하면 당연히 다음 위치의 좌표가 나오네
                
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
	POSITION dest;		// 목적지(destination)
	char repr;			// 화면에 표시할 문자(representation)
	int move_period;	// '몇 ms마다 한 칸 움직이는지'를 뜻함
	int next_move_time;	// 다음에 움직일 시간
	int speed;
} OBJECT_SAMPLE;

#endif
