/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
//const로 정의한 변수는 한번 정의한후 변경할수없다
const POSITION resource_pos = { 0, 0 };// 이 변수는 현재 리소스를 보여는 텍스트의 위치를 가짐
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 }; //다음에 화면에 표시될 데이터를 저장하는 버퍼입니다. 이 버퍼에 데이터를 먼저 그린 후, frontbuf와 교체하여 화면 깜빡임을 줄이고 부드러운 화면 전환을 구현합니다.
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 }; //현재 화면에 표시되고 있는 데이터를 저장하는 버퍼입니다. 즉, 사용자에게 보여지는 화면의 내용을 담고 있습니다.

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);


void display(// 화면에 각각의 요소를 보여주는 함수들을 모아놓은 함수
	RESOURCE resource,//RESOURCE는 int형 spice, spice_max, population, popalation_max로 이루어진 구조체
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor)  // CURSOR은 POSITION형 previous(이전위치)와 current(현위치)로 이루어진 구조체이고
	//POSITION형은 int형 row(행 왼-오), column(열 위-아래)으로 이루어진 구조체 
{
	display_resource(resource); // 말 그대로 resource 구조체 내용을 화면에 표시해주는 함수
	display_map(map); // 
	display_cursor(cursor);
	// display_system_message()
	// display_object_info()
	// display_commands()
	// ...
}

void display_resource(RESOURCE resource) {//왼쪽위에 자원, 인구 표시하는 역할 이걸로 얘는 끝인듯
	set_color(COLOR_RESOURCE);//COLOR_RESOURCE는 112가 들어있고 검정색을 의미한다
	gotoxy(resource_pos);//const변수로 선언했던거
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}// 이해 완

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {//src는 출발지, dest는 목적지
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}//뭐하는 넘인지 이해 불가 0과 같거나 큰 

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);
	//현재 맵 map과 다음에 와야할 맵 backbuf을 project에 인자로 준다 
	//project는 map[][][]에서 0보다 크거나 같은 인덱스를 발견하면 그것들에
	//다음에 와야할 맵 backbuf(같은 인덱스의)에 map의 값을 넣어준다 
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) { //frontbuf는 현재, backbuf는 다음
				POSITION pos = {i, j };//다른점이 발견된 좌표를 pos에 담음
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}//여기서padd는 map_pos.x + pos, map_pos.y + pos임
      //printc는 padd 위치로가서 COLOR_DEFAULT의 색으로 
	  //backbuf[][]의 문자를 출력함 왜지? 왜 문자가 들어있지?
	 //front(현재화면)와 back(미래화면)(아마 대부분 비슷하고 내가 의도한
	 // 일정 부분만 다르겠지)을 차래대로 비교하고 다른점이
	 //있는 좌표를찾고 그좌표에 back(내가 의도한 변화)를 넣어준다(추가한다) 
	//그런다음 front해당 좌표에 back해당 좌표의 값을 넣어줘 변화를 굳건히한다

			frontbuf[i][j] = backbuf[i][j];// 바뀐점 다 출력하고 이제 front에 back을 넣어주고 또 back은 무엇인가에 의해 또 생기고
		}
	}
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor){
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}