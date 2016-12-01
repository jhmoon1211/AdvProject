/* 2016.11.27.
   myfind 명령어 전체 옵션 틀
   각자 옵션 파트 작성해서 추가하면 됨
   수정해야 할 부분 있을 수 있어요!
   열심히 해봅시당!
*/
/*2016.11.30. 클론 및 커밋테스트*/
/*확인 작업*/

#include <sys/types.h>	//opendir, readdir
#include <dirent.h>		//opendir, readdir
#include <sys/stat.h>	//stat
#include <unistd.h>	//getcwd()
#include <stdio.h>
#include <getopt.h>

#define PATH_SIZE	1024	//path 크기

int main(int argc, char *argv[]) {

	char *PathArr[100];
	int PathNum=0;
	char *CopyArgv[argc];
	int i;
	char *current_path = (char *)malloc(PATH_SIZE);	//현재 작업 디렉토리 이름 담을 포인터 변수
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
		//exec 추가함
		//각자 명령어 옵션 수정 필요할 수 있음
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

		if(opt == -1) break;	// 모든 옵션을 확인하면 종료

		switch(opt) {
			case 0:
				switch(index) {
					case 0:		// 옵션이 options[0]인 name일 경우
						break;
					case 1:		//perm인 경우
						break;
					case 2:		//type인 경우
						break;
					case 3:		//user인 경우
						break;
					case 4:		//group인 경우
						group_arg = optarg;		//--group옵션의 인자
						group(group_arg);
						break;
					case 5:		//size인 경우
						break;
					case 6:		//exec인 경우
						break;
					case 7:		//mv인 경우
						break;
					case 8:		//delete인 경우
						break;
					case 9:		//empty인 경우
						break;
					case 10:	//help인 경우
						printf("옵션 %s\n", options[index].name);
						break;
				}
		}
	}

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
	char str[255];	//함수의 그룹 아이디 저장 배열

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

	strcpy(str, (char)sbuf.st_gid);	//stat의 그룹 아이디를 str에 복사
	if(!strcmp(*arg, str)) {
		printf("%s\n", dent->d_name);
	}
	return 0;
}
