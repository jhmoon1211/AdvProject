/* 2016.11.27.
   myfind ��ɾ� ��ü �ɼ� Ʋ
   ���� �ɼ� ��Ʈ �ۼ��ؼ� �߰��ϸ� ��
   �����ؾ� �� �κ� ���� �� �־��!
   ������ �غ��ô�!
*/

#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	struct option options[] = {
		//exec �߰���
		//���� ��ɾ� �ɼ� ���� �ʿ��� �� ����
		{"name", 1, 0, 0},
		{"perm", 1, 0, 0},
		{"type", 1, 0, 0},
		{"user", 1, 0, 0},
		{"group", 1, 0, 0},
		{"size", 1, 0, 0},
		{"exec", 1, 0, 0},
		{"mv", 1, 0, 0},
		{"delete", 1, 0, 0},
		{"empty", 0, 0, 0},
		{"help", 0, 0, 0}
	};
	int index = 0;
	int opt;

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	// ��� �ɼ��� Ȯ���ϸ� ����

		switch(opt) {
			case 0:
				switch(index) {
					case 0:		// �ɼ��� options[0]�� name�� ���
						break;
					case 1:		//perm�� ���
						break;
					case 2:		//type�� ���
						break;
					case 3:		//user�� ���
						break;
					case 4:		//group�� ���
						break;
					case 5:		//size�� ���
						break;
					case 6:		//exec�� ���
						break;
					case 7:		//mv�� ���
						break;
					case 8:		//delete�� ���
						break;
					case 9:		//empty�� ���
						break;
					case 10:	//help�� ���
						printf("�ɼ� %s\n", options[index].name);
						break;
				}
		}
	}
}
