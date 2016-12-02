/* All the best for our final project */

#include <sys/types.h>	//opendir, readdir
#include <dirent.h>		//opendir, readdir
#include <sys/stat.h>	//stat
#include <unistd.h>	//getcwd()
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/times.h>
#include <fcntl.h>
#define BUF_SIZE 512
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

#define PATH_SIZE	1024	//path 크기

int FindByPerm(char* path);
void showHelp(); // for help option
int group(char*); 
void empty();
char* newpath(char* path);
char NewFile[30];

int main(int argc, char *argv[]) {
	char buf[BUF_SIZE];
	int bufsize;
	char *PathArr[100];
	int PathNum=0;
	char *CopyArgv[argc];
	int i;
	int index = 0;
	int opt;
	char *current_path = (char *)malloc(PATH_SIZE);	//현재 작업 디렉토리 이름 담을 포인터 변수
	char *group_arg = NULL;

	//각자 명령어 옵션 수정 필요할 수 있음
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
		CopyArgv[i] = malloc(strlen(argv[i]+1));
		strcpy(CopyArgv[i], argv[i]);
	}
	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			break;
		}
		PathArr[PathNum] = argv[i];
		PathNum++;
	}

	//현재 작업 디렉토리의 이름을 PATH_SIZE만큼 길이로 current_path에 복사
	getcwd(current_path, PATH_SIZE);	

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	// 모든 옵션을 확인하면 종료

		switch(opt) {
			case 0:
				switch(index) {
					case O_NAME:		// 옵션이 options[0]인 name일 경우
						break;
					case O_PERM:		//perm인 경우
						for(i=0;i<PathNum;i++){
							FindByPerm(PathArr[i]);
							getcwd(buf,bufsize);
							chdir(buf);
						}
						break;
					case O_TYPE:		//type인 경우
						break;
					case O_USER:		//user인 경우
						break;
					case O_GROUP:		//group인 경우
						group_arg = optarg;		//--group옵션의 인자
						group(group_arg);
						break;
					case O_SIZE:		//size인 경우
						break;
					case O_EXEC:		//exec인 경우
						break;
					case O_MV:		//mv인 경우
						break;
					case O_DELETE:		//delete인 경우
						break;
					case O_EMPTY:		//empty인 경우
						empty(current_path);
						break;
					case O_HELP:	//help인 경우
						showHelp();
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
	char permbuf[5];
	char permbuf1[5];
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
		if(FileStat.st_mode!=optarg)
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

	//현재 작업 디렉토리의 이름을 PATH_SIZE만큼 길이로 current_path에 복사
	//getcwd(current_path, PATH_SIZE);	

	//for(i = 0; i < pathNum; i++) {
    //	group(copyArgv[i]);
	//	chdir(current_path);	//current_path로 디렉토리 이동 
	//}

	return 0;
}

int group(char* arg) {
	DIR *dp;
	struct dirent *dent;
	struct stat sbuf;
	char path[BUFSIZ];
	int i;
	//char str[255];	//함수의 그룹 아이디 저장 배열
	int gname;

	if((dp = opendir(".")) == NULL) {
		//에러난 현재 디렉토리 출력
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.으로 시작하는 파일은 생략
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path, "./%s", dent->d_name);	//디렉토리의 항복 읽기
	stat(path, &sbuf);	//stat함수로 상세 정보 검색

	//strcpy(str, (char)sbuf.st_gid);	//stat의 그룹 아이디를 str에 복사 + for문
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

	if((dp = opendir(*path)) == NULL) {
		//에러난 현재 디렉토리 출력
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.으로 시작하는 파일은 생략
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path2, "./%s", dent->d_name);	//디렉토리의 항복 읽기
	stat(path2, &sbuf);

	if((int)sbuf.st_size == 0) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);
}

void showHelp() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--------- myfind [경로 1]...[경로 n] [---옵션 1][패턴]...[---옵션 n][패턴] ---------| \n");
	fprintf(stderr, "\t |- name [파일이름] : [파일이름]과 일치하는 파일 검색 \n");
	fprintf(stderr, "\t |- user [유저이름] : [유저이름]과 일치하는 파일 검색 \n");
	fprintf(stderr, "\t |- help : find명령어에 대한 설명 출력 \n");
	fprintf(stderr, "\t |- perm [파일권한] : [파일권한]과 일치하는 파일 검색 \n");
	fprintf(stderr, "\t |- size [파일용량] : [파일용량]과 이상의 파일 검색 \n");
	fprintf(stderr, "\t |- delete [파일이름] : [파일이름]에 지정한 파일을 검색하고 삭제 \n");
	fprintf(stderr, "\t |- group [그룹이름] : [그룹이름]과 일치하는 파일 검색 \n");
	fprintf(stderr, "\t |- type [파일종류] : [파일종류] 지정하여 파일 검색 \n");
	fprintf(stderr, "\t |- empty : 빈 파일 검색 \n");
	fprintf(stderr, "\t |- mv [파일이름] [디렉토리이름] : [파일이름]의 파일을 찾아서 경로를 출력하고, \n");
	fprintf(stderr, "\t\t\t\t[디렉토리이름]에 지정한 디렉토리로 이동 \n"); // mv 설명 이어서
	// fprintf(stderr, "\t |- exec [명령] {} \; : 찾은 파일들에 대한 특정 명령을 수행할 때 사용 \n"); 
	fprintf(stderr,"======================================================================================\n");
}
		







