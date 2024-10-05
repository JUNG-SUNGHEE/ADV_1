#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>

/* ================= system parameters =================== */
#define TICK 10		// time unit(ms)

#define N_LAYER 2//�̰� ������ ������Ʈ�� �и��ؼ� ó���ϱ����� 
#define MAP_WIDTH	60
#define MAP_HEIGHT	18

//�� �ڵ�
#define ST_LAYER 2 // �Ƹ� Ʋ�� �ؽ�Ʈ �и�?
#define STATUS_WIDTH 50
#define STATUS_HEIGHT 18

#define SY_LAYER 2
#define SYSTEM_WIDTH 60
#define SYSTEM_HEIGHT 7

#define CO_LAYER 2
#define COMMAND_WIDTH 50
#define COMMAND_HEIGHT 7

//�� �ڵ�


/* ================= ��ġ�� ���� =================== */
// �ʿ��� ��ġ�� ��Ÿ���� ����ü
typedef struct {
	int row, column;
} POSITION;

// Ŀ�� ��ġ
typedef struct {
	POSITION previous;  // ���� ��ġ
	POSITION current;   // ���� ��ġ
} CURSOR;

// �Է� ������ Ű ����.
// �������� enum�� �����ߴµ�, ũ�� ����� ������ ���� �˻�
typedef enum {
	// k_none: �Էµ� Ű�� ����. d_stay(�� �����̴� ���)�� ����
	k_none = 0, k_up, k_right, k_left, k_down,
	k_quit,
	k_undef, // ���ǵ��� ���� Ű �Է�	
} KEY;


// DIRECTION�� KEY�� �κ�����������, �ǹ̸� ��Ȯ�ϰ� �ϱ� ���ؼ� �ٸ� Ÿ������ ����
typedef enum {//enum�� �������� ����ϴµ� ���ȴ� ������� ù��° ���� 0�� �Ҵ��� ���� 1,2,3,4 �Ҵ�
	//�׷� 
	d_stay = 0, d_up, d_right, d_left, d_down
} DIRECTION;


/* ================= ��ġ�� ����(2) =================== */
// ���Ǽ��� ���� �Լ���. KEY, POSITION, DIRECTION ����ü���� ���������� ��ȯ

// ���Ǽ� �Լ�
inline POSITION padd(POSITION p1, POSITION p2) {//POSITION�� �Լ��ϱ� 2���� int�� ��ȯ�ҵ�
	POSITION p = { p1.row + p2.row, p1.column + p2.column };
	return p;
}//inline�� �׳� ���ǹ� ���ٰ� �����ϰ� �ڿ� �Լ��� ��������

// p1 - p2
inline POSITION psub(POSITION p1, POSITION p2) {
	POSITION p = { p1.row - p2.row, p1.column - p2.column };
	return p;
}

// ����Ű���� Ȯ���ϴ� �Լ�
#define is_arrow_key(k)		(k_up <= (k) && (k) <= k_down)

// ȭ��ǥ 'Ű'(KEY)�� '����'(DIRECTION)���� ��ȯ. ���� ���� �Ȱ����� Ÿ�Ը� �ٲ��ָ� ��
#define ktod(k)		(DIRECTION)(k)

// DIRECTION�� POSITION ���ͷ� ��ȯ�ϴ� �Լ�
inline POSITION dtop(DIRECTION d) {
	static POSITION direction_vector[] = { {0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0} };
	//d_stay == {0, 0} / d_up == {-1, 0} / d_right == {0, 1} / d_left == {0, -1} / d_down == {1, 0}
	//�Լ������� static���� ����� ������ �Լ��� ȣ��� ������ �ʱ�ȭ���� �ʰ�, ���α׷��� ����� ������ �� ���� �����Ѵ� 
	return direction_vector[d];//�� �Լ��� ��� �������� �󸶳� �̵���Ű������ �����ϴµ�
}

// p�� d �������� �̵���Ų POSITION
#define pmove(p, d)		(padd((p), dtop(d)))//padd�Լ��� 2���� POSITION�� ������ �޾� X�� X���� Y�� Y���� ���ؼ� ��ȯ
                            //���⼭ p��������ġ d�� ������ �̵��� ������ ������� dtop�Լ��� ����� ũ������ �Ǿ��ְ� 
							// �� ���⿡ �´� ���� ��ȯ����
							// ��ȣ �׷� padd�Լ��� �� ���� ���ϸ� �翬�� ���� ��ġ�� ��ǥ�� ������
                
/* ================= game data =================== */
typedef struct {
	int spice;		// ���� ������ �����̽�
	int spice_max;  // �����̽� �ִ� ���差
	int population; // ���� �α� ��
	int population_max;  // ���� ������ �α� ��
} RESOURCE;


// �밭 ����� ����. ��� �߰��ϸ鼭 ���� ������ ��
typedef struct {
	POSITION pos;		// ���� ��ġ(position)
	POSITION dest;		// ������(destination)
	char repr;			// ȭ�鿡 ǥ���� ����(representation)
	int move_period;	// '�� ms���� �� ĭ �����̴���'�� ����
	int next_move_time;	// ������ ������ �ð�
	int speed;
} OBJECT_SAMPLE;

#endif
