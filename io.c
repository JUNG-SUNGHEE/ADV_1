/*
* raw(?) I/O
*/
#include "io.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // ��, �� �ݴ�� ���� /���??
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//SetConsoleCursorPosition�� �ι�° ���ڷ� ���� ��ǥ�� �̵���Ų��

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//SetConsoleTextAttribute�Լ��� �ܼ�ȭ���� �ؽ�Ʈ�� �������� �����Ѵ�
//ù��° ���ڷ� �ܼ��ڵ��� �ް�, �ι�° ���ڷ� ���� �Ӽ��� �޴´�.

void printc(POSITION pos, char ch, int color) {
	if (color >= 0) {
		set_color(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

KEY get_key(void) {
	if (!_kbhit()) {  // _kvhit�� �Էµ� Ű�� �ִ��� Ȯ���ϴ� �Լ�
		return k_none;
	}

	int byte = _getch();    // _getch�� �Էµ� Ű�� ���� �޴� �Լ�(����Ű�� �Էµɶ� ���� ��ٸ��� ����)
	switch (byte) {
	case 'q': return k_quit;  // 'q'�� ������ ����
	case 224:
		byte = _getch();  // MSB 224�� �Է� �Ǹ� 1����Ʈ �� ���� �ޱ�
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