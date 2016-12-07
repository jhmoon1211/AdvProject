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

/* global variable declaration */
char * gfile_name; // a name of a file as a global variable
int gcount;
char * guser_name;
char * lname; // login name

/* global function declaration */
void findFilesWithNameOfFile(char *, char *);
void intoTheDirectory(char *);
void findFilesWithNameOfUser(char *, char *);
void printUserFiles(char *);
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
char *tmparg1=NULL;

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
				if (strncmp("--name", argv[2], 6) == 0) {
					path = argv[1];
					nameOfFile = argv[3];
					findFilesWithNameOfFile(path, nameOfFile);
				}
				else {
					printf("Confirm option \n");
					exit(1);
				}

				break;
			case 'u': /* user */
				path = argv[1];
				nameOfUser = argv[3];
				findFilesWithNameOfUser(path, nameOfUser);
				break;
			case 'p': /* perm */
				path = argv[1];
				perm_arg = optarg;
				tmparg1=argv[3];
				FindByPerm(path, perm_arg);
				break;
			case 's':
				path = argv[1];
				size_arg = optarg;
				tmparg1=argv[3];
				FindBySize(path, size_arg);
				break;
			case 'd':
				path = argv[1];
				nameOfFile = argv[3];
				DeleteByName(path, nameOfFile);
                break;
            case 'g':
                group(argv[1], optarg);
                break;
            case 't':
				type(argv[1], optarg);
				break;
			case 'm':
				path=argv[1];
				nameOfFile = argv[3];
				mv_arg = argv[4];
				MvByName(path, nameOfFile,mv_arg);
                break;
			case 'h':
				showHowToUseMYFIND();
				break;
			case 'e':
				empty(argv[1]);
				break;
		}
	}
	return 0;
}

void intoTheDirectory(char * cwd) {
	DIR * dp;
	struct stat buf;
	char * dir_new = NULL;
	struct dirent *dent;
	int kind;
	char * n_cwd;
	char wd[BUFSIZ];

	if ((dp = opendir(cwd)) == NULL) exit(1);

	while ((dent = readdir(dp))) {
		stat(dent->d_name, &buf);
		kind = buf.st_mode & S_IFMT;

		if (dent->d_name[0] == '.') continue;

		// regular file
		if (strncasecmp(dent->d_name, gfile_name, gcount) == 0) {
			if (kind == S_IFREG) {
				printf("%s/%s\n", cwd, dent->d_name);
			}
		}
		// directory
		if (S_ISDIR(buf.st_mode)) {
			chdir(dent->d_name);
			n_cwd = getcwd(wd, BUFSIZ);
			intoTheDirectory(n_cwd);
		}
	}
}

void findFilesWithNameOfFile(char * path, char * nameOfFile) {
	char *cwd1, *cwd;
	char wd[BUFSIZ];
	char n_buf[BUFSIZ]; // This buf is for name of file
	int j = 0;

	cwd1 = getcwd(NULL, BUFSIZ); // current directory
	chdir(path); // change directory
	cwd = getcwd(wd, BUFSIZ); // current directory after it is changed

	sprintf(n_buf, "%s", nameOfFile); // saved name of file in n_buf

	for (j=0; j<1024; j++) {
		if (n_buf[j] == '\0') break;
	}

	gfile_name = nameOfFile;
	gcount = j;

	intoTheDirectory(cwd);
}

void printUserFiles(char * cwd) {
	DIR * dp;
	struct stat buf;
	char * dir_new = NULL;
	struct dirent *dent;
	int kind = 0;
	char * n_cwd = NULL;
	char wd[BUFSIZ];
	uid_t euid;

	euid = geteuid(); // To get effective uid

	if ((dp = opendir(cwd)) == NULL) exit(1);

	while ((dent = readdir(dp))) {
		stat(dent->d_name, &buf);
		kind = buf.st_mode & S_IFMT;

		if (dent->d_name[0] == '.') continue;

		if (strcmp(lname, guser_name) == 0) {
			// regular file
			if (kind == S_IFREG) {
				// user's uid and file's uid
				if ((int)euid == (int)buf.st_uid) {
					printf("%s/%s\n", cwd, dent->d_name);
				}
			}
		}

		// directory
		if  (S_ISDIR(buf.st_mode)) {
			//  printf("I am a directory \n");
			chdir(dent->d_name);
			n_cwd = getcwd(wd, BUFSIZ);
			printUserFiles(n_cwd);
		}
	}
}

void findFilesWithNameOfUser(char * path, char * nameOfUser) {
	char *cwd;
	char wd[BUFSIZ];
	DIR * dp;
	struct dirent *dent;
	uid_t euid;
	char * cname;

	lname = getlogin(); // To get login name
	euid = geteuid(); // To get effective uid
	cname = cuserid(NULL);

	cwd = getcwd(NULL, BUFSIZ);
	chdir(path);
	cwd = getcwd(wd, BUFSIZ);

	guser_name = nameOfUser;

	printUserFiles(cwd);
}

void showHowToUseMYFIND() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--- myfind [PATH] [OPTION] [___] ---| \n");
	fprintf(stderr, "\t |- name [�����̸�] : �������̸����� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- user [�����̸�] : �������̸����� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- help : find��ɾ ���� ���� ��� \n");
	fprintf(stderr, "\t |- perm [���ϱ���] : �����ϱ��ѡ��� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- size [���Ͽ뷮] : �����Ͽ뷮���̻��� ���� �˻� \n");
	fprintf(stderr, "\t |- delete [�����̸�] : �������̸����� ������ ������ �˻��ϰ� ���� \n");
	fprintf(stderr, "\t |- group [�׷��̸�] : ���׷��̸����� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- type [��������] : �������������� �����Ͽ� ���� �˻� \n");
	fprintf(stderr, "\t |- empty : �� ���� �˻� \n");
	fprintf(stderr, "\t |- mv [�����̸�] [���丮�̸�] : �������̸����� ������ ã�Ƽ� ��θ� ����ϰ� \n");
	fprintf(stderr, "\t |- �����丮�̸����� ������ ���丮�� �̵� \n");
	fprintf(stderr,"======================================================================================\n");
}

void group(char* path, char *arg) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    int gname;
    struct group *grp;
    char path2[BUFSIZ], temp[255];
    int i = 0;
	int dir = 0;

    if((dp = opendir(path)) == NULL) { //Error for directory open
        perror("opendir");
        exit(1);
    }

    while((dent = readdir(dp))) {  
		if(dent->d_name[0] == '.') continue;
		
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
           printf("찾는 그룹명과 일치하는 파일이 없습니다.\n");   //A case that file are not exist in the directory
           break;
        }
        else if(((int)sbuf.st_gid) == ((int)grp->gr_gid)) {    //A case that input data is name and there exist some files
           printf("%d %s\n", (int)sbuf.st_gid, dent->d_name);
        }
		
		if((sbuf.st_mode & S_IFMT) == S_IFDIR) {
			memset(temp, 0, 255);
			strcat(temp, "./");
			strcat(temp, dent->d_name);
			dir = 1;
		}
    }

	if(dir == 1) {
		i = 0;
		dir = 0;
		group(temp, arg);
	}

    closedir(dp);  //Close directory
}

void empty(char* path) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ], temp[255];
    int i = 0;
	int dir = 0;

    if((dp = opendir(path)) == NULL) { //Open directory
         perror("opendir");
         exit(1);
    }

    while((dent = readdir(dp))) {  
		if(dent->d_name[0] == '.') continue;

        sprintf(path2, "%s/%s", path, dent->d_name);    //Read directory's list
        stat(path2, &sbuf);

        if((int)sbuf.st_size == 0) {    //Check whether file's size is 0
            printf("%s\n", dent->d_name);
			i = 1;
        }

		if((sbuf.st_mode & S_IFMT) == S_IFDIR) {
			memset(temp, 0, 255);
			strcat(temp, "./");
			strcat(temp, dent->d_name);
			dir = 1;
		}
    }

	if(dir == 1) {
		i = 0;
		dir = 0;
		empty(temp);
	}
	
    if(i == 0) {
	    printf("ã�� Ÿ���� ������ �������� �ʽ��ϴ�.\n");
    }

    closedir(dp);   //Close directory
}

void type(char* path, char* arg) {
	DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ], temp[255];
    int arg_type, file_type;
    int i = 0;
	int dir = 0;

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
		if(dent->d_name[0] == '.') continue;

        sprintf(path2, "%s/%s", path, dent->d_name);
        stat(path2, &sbuf);

        file_type = sbuf.st_mode & S_IFMT;
        if(arg_type == file_type) {
            printf("%s\n", dent->d_name);
            i = 1;
        }
		if(file_type == S_IFDIR) {
			memset(temp, 0, 255);
			strcat(temp, "./");
			strcat(temp, dent->d_name);
			dir = 1;
		}
    }

	if(dir == 1) {
		i = 0;
		dir = 0;
		type(temp, arg);
	}

    if(i == 0) {
	    printf("ã�� Ÿ���� ������ �������� �ʽ��ϴ�.\n");
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
		if(!(DirectStat->d_ino)) // ���̳�尡 0�̸� �н�
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . �� ..�� �н�
			continue;
		// lstat���� �������� �����ϱ�
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if(fnmatch(tmparg,DirectStat->d_name,0))
			continue;
		// �н��ȵ� ���� ���
		printf("%s  %s \n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name);
		currentpath = getcwd(path_buf,BUF_SIZE);
		currentfile = DirectStat->d_name;

	
	chdir(currentpath);
	if ((rfp = fopen(currentfile,"r")) == NULL){ // �̸��� ������ ���� ����
		perror("fopen:fail");
		exit(1);
	}
	chdir(tmparg2); // ������ ���丮


	if((wfp = fopen(currentfile,"w")) ==NULL){ // ������ �̸����� ���� �ӽ� ���� ����
		perror("fopen:fail");
		exit(1);
	}
	while ((n = fread(buf, 1,BUFSIZ,rfp))>0){
		fwrite(buf,1,n,wfp); // �ӽ����Ͽ� ������ ������ ����
	}
	chdir(currentpath); // ���� ��ġ���� ���� ��ġ�� �̵�
	fclose(rfp);
	fclose(wfp);
	unlink(currentfile); //�̵���Ų ������ ����

	}

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
	char *tmparg=(char*)malloc (strlen(arg)+1);
	strcpy(tmparg,arg);
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
		if(!(DirectStat->d_ino)) // ���̳�尡 0�̸� �н�
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . �� ..�� �н�
			continue;
		// lstat���� �������� �����ϱ�
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		a = StringtoInt(tmparg,8); // �Է¹��� �Ű������� 8���� ������?��?����
		b = (FileStat.st_mode&0777); // ������ ���� ����
		if(a!=b)
			continue;
		// �н��ȵ� ���� ���
		printf("%s  %s %o\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name,b);
	}
	rewinddir(DP);

	while((DirectStat=readdir(DP))!=NULL){
		if(!(DirectStat->d_ino))
			continue;
		if(!strcmp(DirectStat->d_name,".")||!strcmp(DirectStat->d_name,".."))
			continue;
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if((FileStat.st_mode&S_IFMT)==S_IFDIR){
			FindByPerm(DirectStat->d_name,tmparg1);
		}
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
	char *tmparg=(char*)malloc (strlen(arg)+1);
	strcpy(tmparg,arg);
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
	giho = tmparg[0]; // ���ھ��� ��ȣ�� ����
	Eliminate(tmparg,'+'); // ���ھ� +��ȣ ����
	Eliminate(tmparg,'-'); // ���ھ� -��ȣ ����
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
		a = StringtoInt(tmparg,10); // �Ű������� 10���� ������ ����
		b = (FileStat.st_size); // ���� ������ ����
		// ��ȣ�� +�ΰ�� �Է°����� ���� ���� �н�
		if((giho=='+')&(a>b)){
			continue;
		}
		// ��ȣ�� -�ΰ�� �Է°����� ū ���� �н�
		if((giho=='-')&(a<b)){
			continue;
		}

		printf("%s  %s %d\n",getcwd(path_buf,BUF_SIZE),DirectStat->d_name,b);
	}
	rewinddir(DP);

	while((DirectStat=readdir(DP))!=NULL){
		if(!(DirectStat->d_ino))
			continue;
		if(!strcmp(DirectStat->d_name,".")||!strcmp(DirectStat->d_name,".."))
			continue;
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if((FileStat.st_mode&S_IFMT)==S_IFDIR){
			FindBySize(DirectStat->d_name,tmparg1);
		}
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
		if(!(DirectStat->d_ino)) // ���̳�尡 0�̸� �н�
			continue;
		if(!strcmp(DirectStat->d_name, ".") || !strcmp(DirectStat->d_name,"..")) // . �� ..�� �н�
			continue;
		// lstat���� �������� �����ϱ�
		if(lstat(DirectStat->d_name, &FileStat)<0){
			perror("lstat error");
			return -1;
		}
		if(fnmatch(tmparg,DirectStat->d_name,0))
			continue;
		// �н��ȵ� ���� ���
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
