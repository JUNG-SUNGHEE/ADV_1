/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
//const�� ������ ������ �ѹ� �������� �����Ҽ�����
const POSITION resource_pos = { 0, 0 };// �� ������ ���� ���ҽ��� ������ �ؽ�Ʈ�� ��ġ�� ����
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 }; //������ ȭ�鿡 ǥ�õ� �����͸� �����ϴ� �����Դϴ�. �� ���ۿ� �����͸� ���� �׸� ��, frontbuf�� ��ü�Ͽ� ȭ�� �������� ���̰� �ε巯�� ȭ�� ��ȯ�� �����մϴ�.
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 }; //���� ȭ�鿡 ǥ�õǰ� �ִ� �����͸� �����ϴ� �����Դϴ�. ��, ����ڿ��� �������� ȭ���� ������ ��� �ֽ��ϴ�.

//�� �ڵ� // ������ ����Ҷ� goto�� �̵��ϴ� �� �ʿ��� ��ŭ�� �迭 �����ϸ�ɵ� ������ �� ���̿� �°�
char backbuf_status[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };
char frontbuf_status[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };

char backbuf_system[SYSTEM_HEIGHT][SYSTEM_WIDTH] = { 0 };
char frontbuf_system[SYSTEM_HEIGHT][SYSTEM_WIDTH] = { 0 };

char backbuf_command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
char frontbuf_command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };

//�� �ڵ�

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
//�� �ڵ�
void display_status(char status[ST_LAYER][STATUS_HEIGHT][STATUS_WIDTH]);
//�� �ڵ�
void display_cursor(CURSOR cursor);


void display(// ȭ�鿡 ������ ��Ҹ� �����ִ� �Լ����� ��Ƴ��� �Լ�
	RESOURCE resource,//RESOURCE�� int�� spice, spice_max, population, popalation_max�� �̷���� ����ü
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	char status[ST_LAYER][STATUS_HEIGHT][STATUS_WIDTH],
	CURSOR cursor)  // CURSOR�� POSITION�� previous(������ġ)�� current(����ġ)�� �̷���� ����ü�̰�
	//POSITION���� int�� row(�� ��-��), column(�� ��-�Ʒ�)���� �̷���� ����ü 
{
	display_resource(resource); // �� �״�� resource ����ü ������ ȭ�鿡 ǥ�����ִ� �Լ�
	display_map(map);
	//�� �ڵ�
	display_status(status);
	//�� �ڵ�
	display_cursor(cursor);
	// display_system_message()
	// display_object_info()
	// display_commands()
	// ...
}

void display_resource(RESOURCE resource) {//�������� �ڿ�, �α� ǥ���ϴ� ���� �̰ɷ� ��� ���ε�
	set_color(COLOR_RESOURCE);//COLOR_RESOURCE�� 112�� ����ְ� �������� �ǹ��Ѵ�
	gotoxy(resource_pos);//const������ �����ߴ���
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}// ���� ��

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {//src�� �����, dest�� ������
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);// back, front�� ����[����][�ʳ���]�� 0���� �ʱ�ȭ �Ǿ��ִ»��´� ó���� �׷��� Ʋ�� �־��ִ�#�� ��ȭ�� �����ϰ� �ϳ��ϳ� ����ϴ°žƴұ�? ���⼭?
	//�������ͽ� ����, Ŀ��� ����, �ý��� ���� ���������ҵ�
	//���� �� map�� ������ �;��� �� backbuf�� project�� ���ڷ� �ش� 
	//project�� map[][][]���� 0���� ũ�ų� ���� �ε����� �߰��ϸ� �װ͵鿡
	//������ �;��� �� backbuf(���� �ε�����)�� map�� ���� �־��ش� 
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) { //frontbuf�� ����, backbuf�� ����
				POSITION pos = {i, j };//�ٸ����� �߰ߵ� ��ǥ�� pos�� ����
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}//���⼭padd�� map_pos.x + pos, map_pos.y + pos��
      //printc�� padd ��ġ�ΰ��� COLOR_DEFAULT�� ������ 
	  //backbuf[][]�� ���ڸ� ����� ����? �� ���ڰ� �������?
	 //front(����ȭ��)�� back(�̷�ȭ��)(�Ƹ� ��κ� ����ϰ� ���� �ǵ���
	 // ���� �κи� �ٸ�����)�� ������� ���ϰ� �ٸ�����
	 //�ִ� ��ǥ��ã�� ����ǥ�� back(���� �ǵ��� ��ȭ)�� �־��ش�(�߰��Ѵ�) 
	//�׷����� front�ش� ��ǥ�� back�ش� ��ǥ�� ���� �־��� ��ȭ�� �������Ѵ�

			frontbuf[i][j] = backbuf[i][j];// �ٲ��� �� ����ϰ� ���� front�� back�� �־��ְ� �� back�� �����ΰ��� ���� �� �����
		}
	}
}

//�� �ڵ�
void display_status(char status[ST_LAYER][STATUS_HEIGHT][STATUS_WIDTH]) {
	project(status, backbuf_status);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) { 
				POSITION pos = { i, j }; 
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}
//�� �ڵ�


// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor){
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;
	// CURSOR�� POSITION�� previous(������ġ)�� current(����ġ)�� �̷���� ����ü�̰�
	//POSITION���� int�� row(�� ��-��), column(�� ��-�Ʒ�)���� �̷���� ����ü

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT); // ������ġ�� ���� ���·� ������

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR); // ������ġ�� Ŀ�������� �ٲٰ�
}