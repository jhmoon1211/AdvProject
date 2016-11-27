/* 2016.11.27.
   myfind 명령어 전체 옵션 틀
   각자 옵션 파트 작성해서 추가하면 됨
   수정해야 할 부분 있을 수 있어요!
   열심히 해봅시당!
*/

#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
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
}
