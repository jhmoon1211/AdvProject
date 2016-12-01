/* 2016.11.27.
   myfind ��ɾ� ��ü �ɼ� Ʋ
   ���� �ɼ� ��Ʈ �ۼ��ؼ� �߰��ϸ� ��
   �����ؾ� �� �κ� ���� �� �־��!
   ������ �غ��ô�!
*/
/*2016.11.30. Ŭ�� �� Ŀ���׽�Ʈ*/
/*Ȯ�� �۾�*/

#include <sys/types.h>	//opendir, readdir
#include <dirent.h>		//opendir, readdir
#include <sys/stat.h>	//stat
#include <unistd.h>	//getcwd()
#include <stdio.h>
#include <getopt.h>

#define PATH_SIZE	1024	//path ũ��

int main(int argc, char *argv[]) {

	char *PathArr[100];
	int PathNum=0;
	char *CopyArgv[argc];
	int i;
	char *current_path = (char *)malloc(PATH_SIZE);	//���� �۾� ���丮 �̸� ���� ������ ����
	char *group_arg = NULL;

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
					case 0:		// �ɼ��� options[0]�� name�� ���
						break;
					case 1:		//perm�� ���
						break;
					case 2:		//type�� ���
						break;
					case 3:		//user�� ���
						break;
					case 4:		//group�� ���
						group_arg = optarg;		//--group�ɼ��� ����
						group(group_arg);
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
	char str[255];	//�Լ��� �׷� ���̵� ���� �迭

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

	strcpy(str, (char)sbuf.st_gid);	//stat�� �׷� ���̵� str�� ����
	if(!strcmp(*arg, str)) {
		printf("%s\n", dent->d_name);
	}
	return 0;
}
