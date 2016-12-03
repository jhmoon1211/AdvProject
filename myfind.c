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

#define PATH_SIZE	1024	//path í¬ê¸°

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
	int PathNum=0; // ÀÔ·ÂÇÑ °æ·Î°ª ¼ö
	char *CopyArgv[argc]; // eeã…‡d
	int i;
	int index = 0;
	int opt;
	char *current_path = (char *)malloc(PATH_SIZE);	//í˜„ì¬ ì‘ì—… ë””ë ‰í† ë¦¬ ì´ë¦„ ë‹´ì„ í¬ì¸í„° ë³€ìˆ˜
	char *group_arg = NULL;
	char *perm_arg = NULL;

	//ê°ì ëª…ë ¹ì–´ ì˜µì…˜ ìˆ˜ì • í•„ìš”í•  ìˆ˜ ìˆìŒ
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
		CopyArgv[i] = malloc(strlen(argv[i]+1)); // ¸Å°³º¯¼ö¸¦ ÀÓ½Ãº¯¼ö¿¡ ÀúÀå
		strcpy(CopyArgv[i], argv[i]);
	}
	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			break;
		}
		PathArr[PathNum] = argv[i]; // °æ·Î´Â ¿©·¯°³ ¿Ã ¼öÀÖÀ¸¹Ç·Î °æ·Î´Â µû·Î º¯¼ö¿¡ ÀúÀå
		PathNum++;
	}

	//í˜„ì¬ ì‘ì—… ë””ë ‰í† ë¦¬ì˜ ì´ë¦„ì„ PATH_SIZEë§Œí¼ ê¸¸ì´ë¡œ current_pathì— ë³µì‚¬
	getcwd(current_path, PATH_SIZE);	

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	// ëª¨ë“  ì˜µì…˜ì„ í™•ì¸í•˜ë©´ ì¢…ë£Œ

		switch(opt) {
			case 0:
				switch(index) {
					case O_NAME:		// ì˜µì…˜ì´ options[0]ì¸ nameì¼ ê²½ìš°
						break;
					case O_PERM:		//permì¸ ê²½ìš°
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
					case O_TYPE:		//typeì¸ ê²½ìš°
						break;
					case O_USER:		//userì¸ ê²½ìš°
						break;
					case O_GROUP:		//groupì¸ ê²½ìš°
						group_arg = optarg;		//--groupì˜µì…˜ì˜ ì¸ì
						group(group_arg);
						break;
					case O_SIZE:		//sizeì¸ ê²½ìš°
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
					case O_EXEC:		//execì¸ ê²½ìš°
						break;
					case O_MV:		//mvì¸ ê²½ìš°
						break;
					case O_DELETE:		//deleteì¸ ê²½ìš°
						break;
					case O_EMPTY:		//emptyì¸ ê²½ìš°
						empty(current_path);
						break;
					case O_HELP:	//helpì¸ ê²½ìš°
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
		perror("opendir error");
		return -1;
	}
	
	chdir(path);

	char * path_buf = (char*)malloc(BUF_SIZE);
	while((DirectStat = readdir(DP)) != NULL){
		if(!(DirectStat->d_ino)) // ¾ÆÀÌ³ëµå°¡ 0ÀÌ¸é ÆĞ½º
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . ³ª ..´Â ÆĞ½º
			continue;
		// lstatÀ¸·Î ÆÄÀÏÁ¤º¸ ÀúÀåÇÏ±â
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		a = PermtoInt(tmparg,8); // ÀÔ·Â¹ŞÀº ¸Å°³º¯¼ö¸¦ 8Áø¼ö Á¤¼öÇüÀ¸·Î º¯°æ
		b = (FileStat.st_mode&0777); // ÆÄÀÏÀÇ ±ÇÇÑ ÀúÀå
		if(a!=b)
			continue;
		// ÆĞ½º¾ÈµÈ ÆÄÀÏ Ãâ·Â
		printf("%s  %s %o\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name,b);
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
		perror("opendir error");
		return -1;
	}
	
	chdir(path);
	giho = tmparg[0]; // ¼ıÀÚ¾ÕÀÇ ºÎÈ£¸¦ ÀúÀå
	Eliminate(tmparg,'+'); // ¼ıÀÚ¾Õ +±âÈ£ Á¦°Å
	Eliminate(tmparg,'-'); // ¼ıÀÚ¾Õ -±âÈ£ Á¦°Å
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
		a = PermtoInt(tmparg,10); // ¸Å°³º¯¼ö¸¦ 10Áø¼ö Á¤¼ö·Î º¯°æ
		b = (FileStat.st_size); // ÆÄÀÏ »çÀÌÁî ÀúÀå
		// ±âÈ£°¡ +ÀÎ°æ¿ì ÀÔ·Â°ªº¸´Ù ÀÛÀº ÆÄÀÏ ÆĞ½º
		if((giho=='+')&(a>b)){
			continue;
		}
		// ±âÈ£°¡ -ÀÎ°æ¿ì ÀÔ·Â°ªº¸´Ù Å« ÆÄÀÏ ÆĞ½º
		if((giho=='-')&(a<b)){
			continue;
		}

		printf("%s  %s %d\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name,b);
	}
	closedir(DP);
	chdir("..");
	free(path_buf);
	return 0;
}

int group(char* arg) {
	DIR *dp;
	struct dirent *dent;
	struct stat sbuf;
	char path[BUFSIZ];
	int i;
	//char str[255];	//í•¨ìˆ˜ì˜ ê·¸ë£¹ ì•„ì´ë”” ì €ì¥ ë°°ì—´
	int gname;

	if((dp = opendir(".")) == NULL) {
		//ì—ëŸ¬ë‚œ í˜„ì¬ ë””ë ‰í† ë¦¬ ì¶œë ¥
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.ìœ¼ë¡œ ì‹œì‘í•˜ëŠ” íŒŒì¼ì€ ìƒëµ
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path, "./%s", dent->d_name);	//ë””ë ‰í† ë¦¬ì˜ í•­ë³µ ì½ê¸°
	stat(path, &sbuf);	//statí•¨ìˆ˜ë¡œ ìƒì„¸ ì •ë³´ ê²€ìƒ‰

	//strcpy(str, (char)sbuf.st_gid);	//statì˜ ê·¸ë£¹ ì•„ì´ë””ë¥¼ strì— ë³µì‚¬ + forë¬¸
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
		//ì—ëŸ¬ë‚œ í˜„ì¬ ë””ë ‰í† ë¦¬ ì¶œë ¥
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.ìœ¼ë¡œ ì‹œì‘í•˜ëŠ” íŒŒì¼ì€ ìƒëµ
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path2, "./%s", dent->d_name);	//ë””ë ‰í† ë¦¬ì˜ í•­ë³µ ì½ê¸°
	stat(path2, &sbuf);

	if((int)sbuf.st_size == 0) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);
}

void showHelp() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--------- myfind [ê²½ë¡œ 1]...[ê²½ë¡œ n] [---ì˜µì…˜ 1][íŒ¨í„´]...[---ì˜µì…˜ n][íŒ¨í„´] ---------| \n");
	fprintf(stderr, "\t |- name [íŒŒì¼ì´ë¦„] : [íŒŒì¼ì´ë¦„]ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- user [ìœ ì €ì´ë¦„] : [ìœ ì €ì´ë¦„]ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- help : findëª…ë ¹ì–´ì— ëŒ€í•œ ì„¤ëª… ì¶œë ¥ \n");
	fprintf(stderr, "\t |- perm [íŒŒì¼ê¶Œí•œ] : [íŒŒì¼ê¶Œí•œ]ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- size [íŒŒì¼ìš©ëŸ‰] : [íŒŒì¼ìš©ëŸ‰]ê³¼ ì´ìƒì˜ íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- delete [íŒŒì¼ì´ë¦„] : [íŒŒì¼ì´ë¦„]ì— ì§€ì •í•œ íŒŒì¼ì„ ê²€ìƒ‰í•˜ê³  ì‚­ì œ \n");
	fprintf(stderr, "\t |- group [ê·¸ë£¹ì´ë¦„] : [ê·¸ë£¹ì´ë¦„]ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- type [íŒŒì¼ì¢…ë¥˜] : [íŒŒì¼ì¢…ë¥˜] ì§€ì •í•˜ì—¬ íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- empty : ë¹ˆ íŒŒì¼ ê²€ìƒ‰ \n");
	fprintf(stderr, "\t |- mv [íŒŒì¼ì´ë¦„] [ë””ë ‰í† ë¦¬ì´ë¦„] : [íŒŒì¼ì´ë¦„]ì˜ íŒŒì¼ì„ ì°¾ì•„ì„œ ê²½ë¡œë¥¼ ì¶œë ¥í•˜ê³ , \n");
	fprintf(stderr, "\t\t\t\t[ë””ë ‰í† ë¦¬ì´ë¦„]ì— ì§€ì •í•œ ë””ë ‰í† ë¦¬ë¡œ ì´ë™ \n"); // mv ì„¤ëª… ì´ì–´ì„œ
	// fprintf(stderr, "\t |- exec [ëª…ë ¹] {} \; : ì°¾ì€ íŒŒì¼ë“¤ì— ëŒ€í•œ íŠ¹ì • ëª…ë ¹ì„ ìˆ˜í–‰í•  ë•Œ ì‚¬ìš© \n"); 
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






