/* All the best for our final project */
// here is one of branches
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
	char *pathArr[100];
	int pathNum=0;
	char *copyArgv[argc];
	int i;
	int index = 0;
	int opt;
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

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	// 모든 옵션을 확인하면 종료

		switch(opt) {
			case 0:
				switch(index) {
					case O_NAME:		// 옵션이 options[0]인 name일 경우
						break;
					case O_PERM:		//perm인 경우
						break;
					case O_TYPE:		//type인 경우
						break;
					case O_USER:		//user인 경우
						break;
					case O_GROUP:		//group인 경우
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
						break;
					case O_HELP:	//help인 경우
						showHelp();
						break;
				}
		}
	}
	return 0;
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
