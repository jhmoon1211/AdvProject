/* 2016.11.27.
   myfind ��ɾ� ��ü �ɼ� Ʋ
   ���� �ɼ� ��Ʈ �ۼ��ؼ� �߰��ϸ� ��
   �����ؾ� �� �κ� ���� �� �־��!
   ������ �غ��ô�!
*/
/*2016.11.30. Ŭ�� �� Ŀ���׽�Ʈ*/
/*Ȯ�� �۾�*/

#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/times.h>
#include <fcntl.h>
#define O_NAME   0
#define O_PERM   1
#define O_TYPE   2
#define O_USER   3
#define O_GROUP  4
#define O_SIZE   5
#define O_EXEC   6
#define O_MV     7
#define O_DELETE 8
#define O_EMPTY  9
#define O_HELP   10

void showHelp(); // for help option

int main(int argc, char *argv[]) {

	char *PathArr[100];
	int PathNum=0;
	char *CopyArgv[argc];
	int i;

	for(i=0;i<argc;i++){
		CopyArgv[i] = malloc(strlen(argv[i]+1));
		strcpy(CopyArgv[i],argv[i]);
	}
	for(i=1;i<argc;i++){
		if(argv[i][0] == '-'){
			break;
		}
		PathArr[PathNum] = argv[i];
		PathNum++;
	}

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
					case O_NAME:		// �ɼ��� options[0]�� name�� ���
						break;
					case O_PERM:		//perm�� ���
						break;
					case O_TYPE:		//type�� ���
						break;
					case O_USER:		//user�� ���
						break;
					case O_GROUP:		//group�� ���
						break;
					case O_SIZE:		//size�� ���
						break;
					case O_EXEC:		//exec�� ���
						break;
					case O_MV:		//mv�� ���
						break;
					case O_DELETE:		//delete�� ���
						break;
					case O_EMPTY:		//empty�� ���
						break;
					case O_HELP:	//help�� ���
						showHelp();
						break;
				}
		}
	}
}

void showHelp() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--------- myfind [��� 1]...[��� n] [---�ɼ� 1][����]...[---�ɼ� n][����] ---------| \n");
	fprintf(stderr, "\t |- name [�����̸�] : [�����̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- user [�����̸�] : [�����̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- help : find��ɾ ���� ���� ��� \n");
	fprintf(stderr, "\t |- perm [���ϱ���] : [���ϱ���]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- size [���Ͽ뷮] : [���Ͽ뷮]�� �̻��� ���� �˻� \n");
	fprintf(stderr, "\t |- delete [�����̸�] : [�����̸�]�� ������ ������ �˻��ϰ� ���� \n");
	fprintf(stderr, "\t |- group [�׷��̸�] : [�׷��̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- type [��������] : [��������] �����Ͽ� ���� �˻� \n");
	fprintf(stderr, "\t |- empty : �� ���� �˻� \n");
	fprintf(stderr, "\t |- mv [�����̸�] [���丮�̸�] : [�����̸�]�� ������ ã�Ƽ� ��θ� ����ϰ�, \n");
	fprintf(stderr, "\t\t\t\t[���丮�̸�]�� ������ ���丮�� �̵� \n"); // mv ���� �̾
	// fprintf(stderr, "\t |- exec [���] {} \; : ã�� ���ϵ鿡 ���� Ư�� ����� ������ �� ��� \n");
	fprintf(stderr,"======================================================================================\n");
}
