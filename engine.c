#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);


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

OBJECT_SAMPLE obj = { //OBJECT_SAMPLE형 구조체는 
	/*POSITION pos;		// 현재 위치(position)
	POSITION dest;		// 목적지(destination)
	char repr;			// 화면에 표시할 문자(representation)
	int move_period;	// '몇 ms마다 한 칸 움직이는지'를 뜻함
	int next_move_time;	// 다음에 움직일 시간
	int speed;를 가짐 */
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init(); // #로 틀만들기, map[-1]에 -1넣기, map배열에 'o'넣기
	intro();// ㄹㅇ 그냥 인트로(정)
	display(resource, map, cursor);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();// KEY 구조체는 k_none = 0, k_up, k_right, k_left, k_down,k_quit, k_undef을 가지는 구조체임
		// get_key함수는 
		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			cursor_move(ktod(key));
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();// 아마 여기가 오브젝트 관련인듯

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
	system("cls"); // 화면 지우기(정)
}

void outro(void) {
	printf("exiting...\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH-1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
}

// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);

	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos); // pusb함수는 POSITION형이고 입력받은 2개의 인자를 P1 - P2(X는 X끼리 Y는 Y끼리해서) 반환
	//dest는 목적지고 pos는 현재 위치다 목적지 - 현재위치 델타X 델타Y를 가진다 이건 좌표라기보다 움직일 량을 말하는걸지도?
	DIRECTION dir;//DIRECTION 구조체는 방향키 위, 아래, 왼, 오, 입력없음을 가진다

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복 // 그렇지 목적지에 도착했다면 델타 X 델타 Y는 0일수밖에 없지 // 즉 아래 IF문은 만약 목적지에 도착했다면 ~해라 라는 뜻인듯
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {// 근데 목적지의 좌표가 1,1 이라면 우하단으로 목적지를 설정해줘라
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };// 설마 -2 해주는 이유가 #으로된 틀 때문인가?// 그것도 그렇고 이게 새로운 목적지를 우하단으로 설정하는
			//부분임 좌표 58, 16 즉 오른쪽 아래를 말함 그걸 새로운 목적지 변수에 넣어주고 obj.dest에 넣어줘서 굳건히함
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 }; // 이것도 위와 같은 느낌임
			obj.dest = new_dest;
		}
		return obj.pos;// 현재위치를 반환? 이건 생각 필요
	}// 지금보니 'o'는 정확히 같은 루트로 1,1에서 58, 16으로 왔다갔다 반복함 오른쪽으로 쭉이동하다 오른 아래를 반복해서 정확히 58, 16에 도착할수있는 지점에 도착하면 오른 아래 반복
	
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {// 목적지 - 현재위치 델타 x와 델타 y에서 델타x와 델타y를 비교해서 더 먼 쪽 축으로 이동 아하 이게 그거부분이다 오른 아래부분
		// 여기서 만약 목적지가 58,16이면 일단 x가 더 크니 x축 방향으로 이동한다 그럼 점점 x는 16에 가까워지겠지 그러다 16? 15? 되면 이번엔 y가 더크니까 y로이동 
		//y방향으로 이동하다가 이번엔 또 x가 더 크니까 이번엔 x 축쪽으로 이동할거임 이게 오른 아래 반복 구현임
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		
		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음 /그때까지 아래 코드는 실행시키지 말라 그때마다 아래 코드를 출력하라
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1; //여기서 obj는현재 위치(position),  목적지(destination), 화면에 표시할 문자(representation)
		// '몇 ms마다 한 칸 움직이는지'를 뜻함, 다음에 움직일 시간을 가짐
	//해석하자면 obj의 위치좌표에 -1을 대입하라
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}