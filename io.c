/*
* raw(?) I/O
*/
#include "io.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달 /어쨰서??
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//SetConsoleCursorPosition은 두번째 인자로 받은 좌표로 이동시킨다

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//SetConsoleTextAttribute함수는 콘솔화면의 텍스트나 배경색상을 설정한다
//첫번째 인자로 콘솔핸들을 받고, 두번째 인자로 색상 속성을 받는다.

void printc(POSITION pos, char ch, int color) {
	if (color >= 0) {
		set_color(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

KEY get_key(void) {
	if (!_kbhit()) {  // _kvhit는 입력된 키가 있는지 확인하는 함수
		return k_none;
	}

	int byte = _getch();    // _getch는 입력된 키를 전달 받는 함수(엔터키가 입력될때 까지 기다리지 않음)
	switch (byte) {
	case 'q': return k_quit;// 'q'를 누르면 종료
	case 32: return k_space;
	case 27: return k_esc;
	case 104 :
	case 72: return k_make_and_har_h;
	case 109:
	case 77: return k_move_m;
	case 66:
	case 98: return k_build;
	case 116:
	case 84: return k_mining_t;
	case 115:
	case 83: return k_show_unit;
	case 112:
	case 80: return k_build_plate;
	case 100:
	case 68: return k_build_Dor;
	case 224:
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte) {
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		default: return k_undef;
		}
	default: return k_undef;
	}
}
