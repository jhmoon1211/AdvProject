#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <limits.h>
#include <unistd.h>
#include <fnmatch.h>
/* global variable declaration */
#define BUF_SIZE 512
/* global function declaration */
void findFilesWithNameOfFile(char *, char *);
void findFilesWithNameOfUser(char *, char *);
void showHowToUseMYFIND();
void group(char *, char *);
void empty(char *);
void type(char *, char *);
int FindByPerm(char *path, char* arg);
int FindBySize(char *path, char* arg);
void Eliminate(char *str, char ch);
int StringtoInt(char* str, int radix);
int DeleteByName(char* path, char* arg);
int MvByName(char* path, char* arg, char* arg2);
/* main fucn that has argc, argv */
int main(int argc , char *argv[]) {
    static struct option long_options[] = {
        {"name",   required_argument,        0, 'n'},
        {"user",   required_argument,        0, 'u'},
        {"perm",   required_argument,        0, 'p'},
        {"size",   required_argument,        0, 's'},
        {"delete", required_argument,        0, 'd'},
        {"group",  required_argument,        0, 'g'},
        {"type",   required_argument,        0, 't'},
        {"mv",     required_argument,        0, 'm'},
        {"exec",   required_argument,        0, 'x'},
        {"help",         no_argument,        0, 'h'},
        {"empty",        no_argument,        0, 'e'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    int opt = 0;
    char * path = NULL;
    char * nameOfFile = NULL;
    char * nameOfUser = NULL;
	char buf[BUF_SIZE];
	int i;
	int index =0;
	char *perm_arg = NULL;
	char *size_arg = NULL;
	char *mv_arg = NULL;

    while (1) {
        opt = getopt_long (argc, argv, "n:u:p:s:d:g:t:m:x:h:e", long_options, &option_index);
        if (opt == -1) break;
        switch (opt) {
            case 'n': /* name */
                path = argv[1];
                nameOfFile = argv[3];
                findFilesWithNameOfFile(path, nameOfFile);
                break;
            case 'u': /* user */
                path = argv[1];
                nameOfUser = argv[3];
                findFilesWithNameOfUser(path, nameOfUser);
                break;
            case 'p': /* perm */
                printf("perm \n");
				path = argv[1];
				perm_arg = optarg;
				FindByPerm(path, perm_arg);
                break;
            case 's':
                printf("size \n");
				path = argv[1];
				size_arg = optarg;
				FindBySize(path, size_arg);
                break;
            case 'd':
                printf("delete \n");
				path = argv[1];
				nameOfFile = argv[3];
				DeleteByName(path, nameOfFile);
                break;
            case 'g':
                printf("group \n");
                group(argv[1], optarg);
                break;
            case 't':
                printf("type \n");
				type(argv[1], optarg);
                break;
            case 'm':
                printf("mv \n");
				path=argv[1];
				nameOfFile = argv[3];
				mv_arg = argv[4];
				MvByName(path, nameOfFile,mv_arg);
                break;
            case 'x':
                printf("exec \n");
                break;
            case 'h':
                printf("help \n");
                showHowToUseMYFIND();
                break;
            case 'e':
                printf("empty \n");
                empty(argv[1]);
                break;
        }
    }
    return 0;
}

void findFilesWithNameOfFile(char * path, char * nameOfFile) {
    char *cwd;
    char wd[BUFSIZ];
    char n_buf[BUFSIZ]; // This buf is for name of file
    DIR * dp;
    struct dirent *dent;
    char * name[80]; // This is to find files users want
    int i = 0;
    int n = 0;
    int len = 0;
    int check = 0; // Whether or not there is any file a user wants to find

    //  printf("PATH : %s \n", path); // path
    //  printf("NAMEOFFILE : %s \n", nameOfFile); // name of file

    cwd = getcwd(NULL, BUFSIZ);
    //  printf("Crrent D = %s \n", cwd);
    chdir(path);
    cwd = getcwd(wd, BUFSIZ);
    //  printf("Changed Crrent D = %s \n", cwd);

    if ((dp = opendir(cwd)) == NULL) {
        exit(1);
    }
    // To count all files in the directory
    while ((dent = readdir(dp))) {
        // dent->d_name
        // char d_name[]; This is the declaration of the struct in dirent.h file
        // printf("NameOfFile : %s \n", dent->d_name);
        name[i] = dent->d_name;
        i++;
    }

    sprintf(n_buf, "%s", nameOfFile); // saved name of file in n_buf
    // ???????????????????????????????????
    printf("first char of the name of the file : %c \n", n_buf[0]);
    printf("first char of the name of the file : %s \n", n_buf);


    printf("-------------------------------------------------------------------\n");
    printf("Here is all files in the directory. \n");
    // When the name of the file is only *
    n = i;
    for (i=0; i<n; i++) {
        printf("%s/", cwd);
        printf("%s", *(name+i));
        printf("\n");
    }
    printf("-------------------------------------------------------------------\n");

    // have to compare between n_buf and *name
    // char n_buf[80]; Vs char * name[80];
    // The reason why I use strcasecmp func is to ignore lowercase and uppercase
    len = sizeof(n_buf);
    //  if (strcasecmp(n_buf, *(name+0), len) == 0) {
    for (i=0; i<n; i++) {
        if (strcasecmp(n_buf, *(name+i)) == 0) {
            // same
            printf("%s/", cwd);
            printf("%s", *(name+i));
            printf("\n");
            check = 1;
        }
        else {
            // not same
        }
    }

    if (check == 0) {
        printf("no any file you want to fild \n");
    }
    check = 0;

}

void findFilesWithNameOfUser(char * path, char * nameOfUser) {
    char *cwd;
    char wd[BUFSIZ];
    char n_buf[BUFSIZ]; // This buf is for name of file
    DIR * dp;
    struct dirent *dent;
    char * name[80]; // This is to find files users want
    int i = 0;
    int n = 0;
    int len = 0;
    int check = 0; // Whether or not there is any file a user wants to find
    uid_t euid;
    char * cname;
    struct stat f_buf;
    char * lname; // login name

    lname = getlogin(); // To get login name

    euid = geteuid(); // To get effective uid
    cname = cuserid(NULL);

    cwd = getcwd(NULL, BUFSIZ);
    chdir(path);
    cwd = getcwd(wd, BUFSIZ);

    if ((dp = opendir(cwd)) == NULL) {
        exit(1);
    }
    // To count all files in the directory
    while ((dent = readdir(dp))) {
        name[i] = dent->d_name;
        i++;
    }
    n = i;

    sprintf(n_buf, "%s", nameOfUser); // saved name of file in n_buf

    //  printf("UserID : %s \n", n_buf);
    // strcmp between login name 'lname' and the name of the user 'n_buf'
    if (strcmp(lname, n_buf) == 0) {
        //  printf("It is same \n");
        for (i=0; i<n; i++) {
            stat(*(name+i), &f_buf);
            // To find files which effective user is the same as
            if ((int)euid == (int)f_buf.st_uid) {
                //  printf("USER's file %d : %d \n",i+1 , (int)f_buf.st_uid);
                // All files in the directory
                printf("%s/", cwd);
                printf("%s", *(name+i));
                printf("\n");
                check = 1;
            }
        }
    }
    else {
        printf("Confirm user id \n");
        exit(1);
    }


    if (check = 0) {
        printf("There is no files you find \n");
    }
}

void showHowToUseMYFIND() {
    fprintf(stderr,"======================================================================================\n");
    fprintf(stderr, "|--- myfind [PATH] [OPTION] [___] ---| \n");
    fprintf(stderr, "\t |- name [íŒŒì¼ì´ë¦„] : â€˜íŒŒì¼ì´ë¦„â€™ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- user [ìœ ì €ì´ë¦„] : â€˜ìœ ì €ì´ë¦„â€™ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- help : findëª…ë ¹ì–´ì— ëŒ€í•œ ì„¤ëª… ì¶œë ¥ \n");
    fprintf(stderr, "\t |- perm [íŒŒì¼ê¶Œí•œ] : â€˜íŒŒì¼ê¶Œí•œâ€™ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- size [íŒŒì¼ìš©ëŸ‰] : â€˜íŒŒì¼ìš©ëŸ‰â€™ì´ìƒì˜ íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- delete [íŒŒì¼ì´ë¦„] : â€˜íŒŒì¼ì´ë¦„â€™ì— ì§€ì •í•œ íŒŒì¼ì„ ê²€ìƒ‰í•˜ê³  ì‚­ì œ \n");
    fprintf(stderr, "\t |- group [ê·¸ë£¹ì´ë¦„] : â€˜ê·¸ë£¹ì´ë¦„â€™ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- type [íŒŒì¼ì¢…ë¥˜] : â€˜íŒŒì¼ì¢…ë¥˜â€˜ë¥¼ ì§€ì •í•˜ì—¬ íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- empty : ë¹ˆ íŒŒì¼ ê²€ìƒ‰ \n");
    fprintf(stderr, "\t |- mv [íŒŒì¼ì´ë¦„] [ë””ë ‰í† ë¦¬ì´ë¦„] : â€˜íŒŒì¼ì´ë¦„â€™ì˜ íŒŒì¼ì„ ì°¾ì•„ì„œ ê²½ë¡œë¥¼ ì¶œë ¥í•˜ê³  \n");
    fprintf(stderr, "\t |- â€˜ë””ë ‰í† ë¦¬ì´ë¦„â€™ì— ì§€ì •í•œ ë””ë ‰í† ë¦¬ë¡œ ì´ë™ \n");
    // fprintf(stderr, "\t |- exec [ëª…ë ¹] {} \; : ì°¾ì€ íŒŒì¼ë“¤ì— ëŒ€í•œ íŠ¹ì • ëª…ë ¹ì„ ìˆ˜í–‰í•  ë•Œ ì‚¬ìš© \n");
    fprintf(stderr,"======================================================================================\n");
}

void group(char* path, char *arg) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ];
    int i = 0;
    //char str[255];    //í•¨ìˆ˜ì˜ ê·¸ë£¹ ì•„ì´ë”” ì €ìž¥ ë°°ì—´
    int gname;
    struct group *grp;

    if((dp = opendir(path)) == NULL) { //Error for directory open
        perror("opendir");
        exit(1);
    }

    while((dent = readdir(dp))) {  
        //.ìœ¼ë¡œ ì‹œìž‘í•˜ëŠ” íŒŒì¼ì€ ìƒëžµ
        //if(dent->d_name[0] == '.')  continue;
        //else    break;
        sprintf(path2, "%s/%s", path, dent->d_name);   //Read directory's list
        stat(path2, &sbuf);

        //Case1: input is id(number)
        gname = atoi(arg); //Change char to int
        if(gname == (int)sbuf.st_gid) {    //If input data equal to file's group id then print file name
           printf("%d %s\n", gname, dent->d_name);
           i++;    //Just for check whether file which is equal to input data exist in the directory
        }

        if(i != 0) {   //Input data is number and there exist some file which is equal to input data in the directory
           continue;
        }
        else if((grp = getgrnam(arg)) ==  NULL) {              //Check if input data is name(char)
           printf("ì°¾ëŠ” ê·¸ë£¹ëª…ê³¼ ì¼ì¹˜í•˜ëŠ” íŒŒì¼ì´ ì—†ìŠµë‹ˆë‹¤.\n");   //A case that file are not exist in the directory
           break;
        }
        else if(((int)sbuf.st_gid) == ((int)grp->gr_gid)) {    //A case that input data is name and there exist some files
           printf("%d %s\n", (int)sbuf.st_gid, dent->d_name);
        }
    }

    closedir(dp);  //Close directory
}

void empty(char* path) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ];
    //char** path1 = path;

    if((dp = opendir(path)) == NULL) { //Open directory
         perror("opendir");
         exit(1);
    }

    while((dent = readdir(dp))) {  
        //.ìœ¼ë¡œ ì‹œìž‘í•˜ëŠ” íŒŒì¼ì€ ìƒëžµ
        //if(dent->d_name[0] == '.')  continue;
        //else    break;
        sprintf(path2, "%s/%s", path, dent->d_name);    //Read directory's list
        stat(path2, &sbuf);

        if((int)sbuf.st_size == 0) {    //Check whether file's size is 0
            printf("%s\n", dent->d_name);
        }
    }

    closedir(dp);   //Close directory
}

void type(char* path, char* arg) {
	DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ];
    int arg_type, type;
    int i = 0;

    if((dp = opendir(path)) == NULL) {  //Directory open
        perror("opendir");
        exit(1);
    }

    switch(*arg) {      //Argument meaning
        case '-':       //Regular file
            arg_type = S_IFREG;
            break;
        case 'd':       //Directory
            arg_type = S_IFDIR;
            break;
        case 'b':       //Block
            arg_type = S_IFBLK;
            break;
        case 'c':       //Character
            arg_type = S_IFCHR;
            break;
        case 'l':       //Symbolic
            arg_type = S_IFLNK;
            break;
    }

    while(dent = readdir(dp)) {
        sprintf(path2, "%s/%s", path, dent->d_name);
        stat(path2, &sbuf);

        type = sbuf.st_mode & S_IFMT;
        if(arg_type == type) {
            printf("%s\n", dent->d_name);
            i = 1;
        }
    }

    if(i == 0) {
	    printf("Ã£´Â Å¸ÀÔÀÇ ÆÄÀÏÀÌ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù.\n");
    }
}
int MvByName(char* path, char* arg, char* arg2){
	DIR* DP;
	struct stat FileStat;
	char buf2[BUF_SIZE];
	char buf[BUF_SIZE];
	struct dirent *DirectStat;
	char permbuf[5];
	char permbuf1[5];
	char *tmparg=arg;
	char *tmparg2=arg2;
	char *currentfile=NULL;
	char *currentpath=NULL;
	FILE *fd;
	int a,b;
	FILE *rfp, *wfp;
	int n;
	char *cwd;
	char wd[BUF_SIZE];
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
		if(!(DirectStat->d_ino)) // ¾ÆÀÌ³ëµå°¡ 0ÀÌ¸é ÆÐ½º
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . ³ª ..´Â ÆÐ½º
			continue;
		// lstatÀ¸·Î ÆÄÀÏÁ¤º¸ ÀúÀåÇÏ±â
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if(fnmatch(tmparg,DirectStat->d_name,0))
			continue;
		// ÆÐ½º¾ÈµÈ ÆÄÀÏ Ãâ·Â
		printf("%s  %s \n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
		currentpath = getcwd(path_buf,BUF_SIZE);
		currentfile = DirectStat->d_name;
	}
	
	printf("%s\n",currentfile);
	chdir(currentpath);
	if ((rfp = fopen(currentfile,"r")) == NULL){ // ÀÌ¸§À» º¯°æÇÒ ÆÄÀÏ ¿ÀÇÂ
		perror("fopen:fail");
		exit(1);
	}
	chdir(tmparg2); // ÁöÁ¤ÇÑ µð·ºÅä¸®


	if((wfp = fopen(currentfile,"w")) ==NULL){ // º¯°æÇÒ ÀÌ¸§¸íÀ» °¡Áø ÀÓ½Ã ÆÄÀÏ »ý¼º
		perror("fopen:fail");
		exit(1);
	}
	while ((n = fread(buf, 1,BUFSIZ,rfp))>0){
		fwrite(buf,1,n,wfp); // ÀÓ½ÃÆÄÀÏ¿¡ ¿ÀÇÂÇÑ ÆÄÀÏÀ» º¹»ç
	}
	chdir(currentpath); // ÇöÀç À§Ä¡¿¡¼­ »óÀ§ µð·ºÅä¸®·Î ÀÌµ¿
	fclose(rfp);
	fclose(wfp);
	unlink(tmparg);




	closedir(DP);
	chdir("..");
	free(path_buf);
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
		if(!(DirectStat->d_ino)) // ¾ÆÀÌ³ëµå°¡ 0ÀÌ¸é ÆÐ½º
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . ³ª ..´Â ÆÐ½º
			continue;
		// lstatÀ¸·Î ÆÄÀÏÁ¤º¸ ÀúÀåÇÏ±â
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		a = StringtoInt(tmparg,8); // ÀÔ·Â¹ÞÀº ¸Å°³º¯¼ö¸¦ 8Áø¼ö Á¤¼öÇü?¸·?º¯°æ
		b = (FileStat.st_mode&0777); // ÆÄÀÏÀÇ ±ÇÇÑ ÀúÀå
		if(a!=b)
			continue;
		// ÆÐ½º¾ÈµÈ ÆÄÀÏ Ãâ·Â
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
	giho = tmparg[0]; // ¼ýÀÚ¾ÕÀÇ ºÎÈ£¸¦ ÀúÀå
	Eliminate(tmparg,'+'); // ¼ýÀÚ¾Õ +±âÈ£ Á¦°Å
	Eliminate(tmparg,'-'); // ¼ýÀÚ¾Õ -±âÈ£ Á¦°Å
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
		a = StringtoInt(tmparg,10); // ¸Å°³º¯¼ö¸¦ 10Áø¼ö Á¤¼ö·Î º¯°æ
		b = (FileStat.st_size); // ÆÄÀÏ »çÀÌÁî ÀúÀå
		// ±âÈ£°¡ +ÀÎ°æ¿ì ÀÔ·Â°ªº¸´Ù ÀÛÀº ÆÄÀÏ ÆÐ½º
		if((giho=='+')&(a>b)){
			continue;
		}
		// ±âÈ£°¡ -ÀÎ°æ¿ì ÀÔ·Â°ªº¸´Ù Å« ÆÄÀÏ ÆÐ½º
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

int DeleteByName(char* path, char* arg){
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
		if(!(DirectStat->d_ino)) // ¾ÆÀÌ³ëµå°¡ 0ÀÌ¸é ÆÐ½º
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . ³ª ..´Â ÆÐ½º
			continue;
		// lstatÀ¸·Î ÆÄÀÏÁ¤º¸ ÀúÀåÇÏ±â
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if(fnmatch(tmparg,DirectStat->d_name,0))
			continue;
		// ÆÐ½º¾ÈµÈ ÆÄÀÏ Ãâ·Â
		printf("%s  %s is deleted\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
		unlink(DirectStat->d_name);
	}
	closedir(DP);
	chdir("..");
	free(path_buf);
	return 0;
}

int StringtoInt(char* str, int radix){
	char* tmp = str;
	while(*tmp){
		*tmp++;
	}
	return strtol(str,(char**)NULL,radix);
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
