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

#define PATH_SIZE	1024	//path ํฌ๊ธฐ

int FindByPerm(char* path, char* arg);
int FindBySize(char* path, char* arg);
void Eliminate(char *str, char ch);
void showHelp(); // for help option
int group(char*); 
void empty();
char* newpath(char* path);
char NewFile[30];
int PermtoInt(char* perm, int radix);
int main(int argc, char *argv[]) {
	char buf[BUF_SIZE];
	int bufsize;
	char *PathArr[100];
	int PathNum=0; // ภิทยวั ฐๆทฮฐช ผ๖
	char *CopyArgv[argc]; // eeใd
	int i;
	int index = 0;
	int opt;
	char *current_path = (char *)malloc(PATH_SIZE);	//ํ์ฌ ์์ ๋๋ ํ ๋ฆฌ ์ด๋ฆ ๋ด์ ํฌ์ธํฐ ๋ณ์
	char *group_arg = NULL;
	char *perm_arg = NULL;

	//๊ฐ์ ๋ช๋ น์ด ์ต์ ์์  ํ์ํ  ์ ์์
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

	//ํ์ฌ ์์ ๋๋ ํ ๋ฆฌ์ ์ด๋ฆ์ PATH_SIZE๋งํผ ๊ธธ์ด๋ก current_path์ ๋ณต์ฌ
	getcwd(current_path, PATH_SIZE);	

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	// ๋ชจ๋  ์ต์์ ํ์ธํ๋ฉด ์ข๋ฃ

		switch(opt) {
			case 0:
				switch(index) {
					case O_NAME:		// ์ต์์ด options[0]์ธ name์ผ ๊ฒฝ์ฐ
						break;
					case O_PERM:		//perm์ธ ๊ฒฝ์ฐ
						printf("pathnum: %d argc: %d\n",PathNum,argc);
						perm_arg = optarg;
						if(PathNum==0){
							FindByPerm(".",perm_arg);
							getcwd(buf,bufsize);
							chdir(buf);
						}
						for(i=0;i<PathNum;i++){
							FindByPerm(PathArr[i],perm_arg);
							getcwd(buf,bufsize);
							chdir(buf);
						}
						break;
					case O_TYPE:		//type์ธ ๊ฒฝ์ฐ
						break;
					case O_USER:		//user์ธ ๊ฒฝ์ฐ
						break;
					case O_GROUP:		//group์ธ ๊ฒฝ์ฐ
						group_arg = optarg;		//--group์ต์์ ์ธ์
						group(group_arg);
						break;
					case O_SIZE:		//size์ธ ๊ฒฝ์ฐ
						perm_arg = optarg;
						if(PathNum==0){
							FindBySize(".",perm_arg);
							getcwd(buf,bufsize);
							chdir(buf);
						}
						for(i=0;i<PathNum;i++){
							FindBySize(PathArr[i],perm_arg);
							getcwd(buf,bufsize);
							chdir(buf);
						}
						break;
					case O_EXEC:		//exec์ธ ๊ฒฝ์ฐ
						break;
					case O_MV:		//mv์ธ ๊ฒฝ์ฐ
						break;
					case O_DELETE:		//delete์ธ ๊ฒฝ์ฐ
						break;
					case O_EMPTY:		//empty์ธ ๊ฒฝ์ฐ
						empty(current_path);
						break;
					case O_HELP:	//help์ธ ๊ฒฝ์ฐ
						showHelp();
						break;
				}
		}
	}
	return 0;
}

int FindByPerm(char* path, char* arg){
	DIR* DP;
	struct stat FileStat;
	char buf2[BUF_SIZE];
	struct dirent *DirectStat;
	char permbuf[5];
	char permbuf1[5];
	char *tmparg=arg;
	FILE *fd;
	int a,b;
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
		a = PermtoInt(tmparg,8);
		b = (FileStat.st_mode&0777);
		if(a!=b)
			continue;

		printf("%s  %s\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
	}
	closedir(DP);
	chdir("..");
	free(path_buf);
	return 0;
}

int FindBySize(char* path, char* arg){
	DIR* DP;
	struct stat FileStat;
	char buf2[BUF_SIZE];
	struct dirent *DirectStat;
	char permbuf[5];
	char permbuf1[5];
	char *tmparg=arg;
	FILE *fd;
	int a,b;
	char giho;
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
	giho = tmparg[0];
	Eliminate(tmparg,'+');
	Eliminate(tmparg,'-');
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
		a = PermtoInt(tmparg,10);
		b = (FileStat.st_size);

		if((giho=='+')&(a>b)){
			continue;
		}
		if((giho=='-')&(a<b)){
			continue;
		}

		printf("%s  %s\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
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

int group(char* arg) {
	DIR *dp;
	struct dirent *dent;
	struct stat sbuf;
	char path[BUFSIZ];
	int i;
	//char str[255];	//ํจ์์ ๊ทธ๋ฃน ์์ด๋ ์ ์ฅ ๋ฐฐ์ด
	int gname;

	if((dp = opendir(".")) == NULL) {
		//์๋ฌ๋ ํ์ฌ ๋๋ ํ ๋ฆฌ ์ถ๋ ฅ
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.์ผ๋ก ์์ํ๋ ํ์ผ์ ์๋ต
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path, "./%s", dent->d_name);	//๋๋ ํ ๋ฆฌ์ ํญ๋ณต ์ฝ๊ธฐ
	stat(path, &sbuf);	//statํจ์๋ก ์์ธ ์ ๋ณด ๊ฒ์

	//strcpy(str, (char)sbuf.st_gid);	//stat์ ๊ทธ๋ฃน ์์ด๋๋ฅผ str์ ๋ณต์ฌ + for๋ฌธ
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
		//์๋ฌ๋ ํ์ฌ ๋๋ ํ ๋ฆฌ ์ถ๋ ฅ
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.์ผ๋ก ์์ํ๋ ํ์ผ์ ์๋ต
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path2, "./%s", dent->d_name);	//๋๋ ํ ๋ฆฌ์ ํญ๋ณต ์ฝ๊ธฐ
	stat(path2, &sbuf);

	if((int)sbuf.st_size == 0) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);
}

void showHelp() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--------- myfind [๊ฒฝ๋ก 1]...[๊ฒฝ๋ก n] [---์ต์ 1][ํจํด]...[---์ต์ n][ํจํด] ---------| \n");
	fprintf(stderr, "\t |- name [ํ์ผ์ด๋ฆ] : [ํ์ผ์ด๋ฆ]๊ณผ ์ผ์นํ๋ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- user [์ ์ ์ด๋ฆ] : [์ ์ ์ด๋ฆ]๊ณผ ์ผ์นํ๋ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- help : find๋ช๋ น์ด์ ๋ํ ์ค๋ช ์ถ๋ ฅ \n");
	fprintf(stderr, "\t |- perm [ํ์ผ๊ถํ] : [ํ์ผ๊ถํ]๊ณผ ์ผ์นํ๋ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- size [ํ์ผ์ฉ๋] : [ํ์ผ์ฉ๋]๊ณผ ์ด์์ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- delete [ํ์ผ์ด๋ฆ] : [ํ์ผ์ด๋ฆ]์ ์ง์ ํ ํ์ผ์ ๊ฒ์ํ๊ณ  ์ญ์  \n");
	fprintf(stderr, "\t |- group [๊ทธ๋ฃน์ด๋ฆ] : [๊ทธ๋ฃน์ด๋ฆ]๊ณผ ์ผ์นํ๋ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- type [ํ์ผ์ข๋ฅ] : [ํ์ผ์ข๋ฅ] ์ง์ ํ์ฌ ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- empty : ๋น ํ์ผ ๊ฒ์ \n");
	fprintf(stderr, "\t |- mv [ํ์ผ์ด๋ฆ] [๋๋ ํ ๋ฆฌ์ด๋ฆ] : [ํ์ผ์ด๋ฆ]์ ํ์ผ์ ์ฐพ์์ ๊ฒฝ๋ก๋ฅผ ์ถ๋ ฅํ๊ณ , \n");
	fprintf(stderr, "\t\t\t\t[๋๋ ํ ๋ฆฌ์ด๋ฆ]์ ์ง์ ํ ๋๋ ํ ๋ฆฌ๋ก ์ด๋ \n"); // mv ์ค๋ช ์ด์ด์
	// fprintf(stderr, "\t |- exec [๋ช๋ น] {} \; : ์ฐพ์ ํ์ผ๋ค์ ๋ํ ํน์  ๋ช๋ น์ ์ํํ  ๋ ์ฌ์ฉ \n"); 
	fprintf(stderr,"======================================================================================\n");
}
		
int PermtoInt(char* perm, int radix){
	char* tmp = perm;
	while(*tmp){
		*tmp++;
	}
	return strtol(perm,(char**)NULL,radix);
}
		
void Eliminate(char *str, char ch){
	for (;*str!='\0';str++){
		if(*str==ch)
		{
			strcpy(str,str+1);
			str--;
		}
	}
}






