/* All the best for our final project */

#include <sys/types.h>	//opendir, readdir
#include <dirent.h>		//opendir, readdir
#include <sys/stat.h>	//stat
#include <unistd.h>	//getcwd()
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
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

#define PATH_SIZE	1024	//path ũ��

void showHelp(); // for help option
int group(char*); 
void empty();

int main(int argc, char *argv[]) {
	char *pathArr[100];
	int pathNum=0;
	char *copyArgv[argc];
	int i;
	int index = 0;
	int opt;
	char *current_path = (char *)malloc(PATH_SIZE);	//���� �۾� ���丮 �̸� ���� ������ ����
	char *group_arg = NULL;

	for(i=0;i<argc;i++){
		copyArgv[i] = malloc(strlen(argv[i]+1));
		strcpy(copyArgv[i],argv[i]);
	}

	for(i=1;i<argc;i++){
		if(argv[i][0] == '-'){
			break;
		}
		pathArr[pathNum] = argv[i];
		pathNum++;
	}

	//���� ��ɾ� �ɼ� ���� �ʿ��� �� ����
	struct option options[] = {
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

	for(i=0; i<argc; i++) {
		copyArgv[i] = malloc(strlen(argv[i]+1));
		strcpy(copyArgv[i], argv[i]);
	}
	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			break;
		}
		pathArr[pathNum] = argv[i];
		pathNum++;
	}

	//���� �۾� ���丮�� �̸��� PATH_SIZE��ŭ ���̷� current_path�� ����
	getcwd(current_path, PATH_SIZE);	

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
						group_arg = optarg;		//--group�ɼ��� ����
						group(group_arg);
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
						empty(current_path);
						break;
					case O_HELP:	//help�� ���
						showHelp();
						break;
				}
		}
	}

	//���� �۾� ���丮�� �̸��� PATH_SIZE��ŭ ���̷� current_path�� ����
	//getcwd(current_path, PATH_SIZE);	

	//for(i = 0; i < pathNum; i++) {
    //	group(copyArgv[i]);
	//	chdir(current_path);	//current_path�� ���丮 �̵� 
	//}

	return 0;
}

int group(char* arg) {
	DIR *dp;
	struct dirent *dent;
	struct stat sbuf;
	char path[BUFSIZ];
	int i;
	//char str[255];	//�Լ��� �׷� ���̵� ���� �迭
	int gname;

	if((dp = opendir(".")) == NULL) {
		//������ ���� ���丮 ���
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.���� �����ϴ� ������ ����
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path, "./%s", dent->d_name);	//���丮�� �׺� �б�
	stat(path, &sbuf);	//stat�Լ��� �� ���� �˻�

	//strcpy(str, (char)sbuf.st_gid);	//stat�� �׷� ���̵� str�� ���� + for��
	//if(!strcmp(*arg, str)) {
	//	printf("%s\n", dent->d_name);
	//}
	gname = atoi(*arg);
	if(gname == (int)sbuf.st_gid) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);

	return 0;
}

void empty(char* path) {
	DIR *dp;
	struct dirent *dent;
	struct stat sbuf;
	char path2[BUFSIZ];
	//char** path1 = path;

	//if((dp = opendir(*path)) == NULL) {
	if((dp = opendir(path)) == NULL) {
		//������ ���� ���丮 ���
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.���� �����ϴ� ������ ����
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path2, "./%s", dent->d_name);	//���丮�� �׺� �б�
	stat(path2, &sbuf);

	if((int)sbuf.st_size == 0) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);
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
