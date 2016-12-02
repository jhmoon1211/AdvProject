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
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 512
int FindByPerm(char* path);
char* newpath(char* path);
char NewFile[30];
int main(int argc, char *argv[]) {
	char buf[BUF_SIZE];
	int bufsize;
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
					case 0:		// �ɼ��� options[0]�� name�� ���
						break;
					case 1:		//perm�� ���
						for(i=0;i<PathNum;i++){
							FindByPerm(PathArr[i]);
							getcwd(buf,bufsize);
							chdir(buf);
						}

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
	return 0;
}

int FindByPerm(char* path){
	DIR* DP;
	struct stat FileStat;
	char buf2[BUF_SIZE];
	struct dirent *DirectStat;
	FILE *fd;
	if(access(path,R_OK)){
		perror("access denied");
		exit(1);
	}
	if(!(DP=opendir(path))){
		path = (char *)newpath(path);
		if(!(DP=opendir(path))){
			perror("opendir error");
			return -1;
		}
	}
	chdir(path);

	char * path_buf = (char*)malloc(BUF_SIZE);
	
	while((DirectStat = readdir(DP)) != NULL){
		if(!(DirectStat->d_ino))
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,".."))
			continue;
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}

		if(!strcmp(FileStat.st_mode,optarg))
			continue;

		printf("%s  %s\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
	}
	rewinddir(DP);
	while((DirectStat=readdir(DP))!=NULL){
		if(!(DirectStat->d_ino))
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,".."))
			continue;
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}

		if((FileStat.st_mode & S_IFMT)==S_IFDIR){
			FindByPerm(DirectStat->d_name);
		}
	}
	closedir(DP);
	chdir("..");
	free(path_buf);
	return 0;
}

char* newpath(char* path){
	int len,i,j=0,k=0;
	char path1[100];

	len = strlen(path);
	strcpy(path1,path);
	for(i=len;i>0;i--){
		if(path1[i]=='/'){
			j=i;
			break;
		}
	}
	j++;
	for (i=j;i<=len;i++){
		NewFile[k++]=path1[i];
		path1[i] = NULL;
	}
	NewFile[k] = NULL;
	strcpy(path,path1);
	return path;
}
		







