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

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);


void display(// ȭ�鿡 ������ ��Ҹ� �����ִ� �Լ����� ��Ƴ��� �Լ�
	RESOURCE resource,//RESOURCE�� int�� spice, spice_max, population, popalation_max�� �̷���� ����ü
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor)  // CURSOR�� POSITION�� previous(������ġ)�� current(����ġ)�� �̷���� ����ü�̰�
	//POSITION���� int�� row(�� ��-��), column(�� ��-�Ʒ�)���� �̷���� ����ü 
{
	display_resource(resource); // �� �״�� resource ����ü ������ ȭ�鿡 ǥ�����ִ� �Լ�
	display_map(map); // 
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
}//���ϴ� ������ ���� �Ұ� 0�� ���ų� ū 

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);
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

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor){
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}