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

#define PATH_SIZE	1024	//path size 

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
	int PathNum=0; // �Է��� ��ΰ� ��
	char *CopyArgv[argc]; // eeㅇd
	int i;
	int index = 0;
	int opt;
	char *current_path = (char *)malloc(PATH_SIZE);	//���� �۾� ���丮 �̸� ���� ������ ����
	char *group_arg = NULL;
	char *perm_arg = NULL;

	//��ɾ� �ɼ�	
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

	//���� �۾� ���丮�� �̸��� PATH_SIZE��ŭ ���̷� current_path�� ����
	getcwd(current_path, PATH_SIZE);	

	while(1) {
		opt = getopt_long(argc, argv, "", options, &index);	

		if(opt == -1) break;	//��� �ɼ��� Ȯ���ϸ� ���� 

		switch(opt) {
			case 0:
				switch(index) {
					case O_NAME:		// name
						break;
					case O_PERM:		//perm
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
					case O_TYPE:		//type
						break;
					case O_USER:		//user
						break;
					case O_GROUP:		//group
						group_arg = optarg;		//--group
						group(group_arg);
						break;
					case O_SIZE:		//size인 경우
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
					case O_EXEC:		//exec
						break;
					case O_MV:		//mv
						break;
					case O_DELETE:		//delete
						break;
					case O_EMPTY:		//empty
						empty(current_path);
						break;
					case O_HELP:	//help
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
	//char str[255];	//�Լ��� �׷� ���̵� ���� �迭
	int gname;

	if((dp = opendir(".")) == NULL) {
		//������ ���� ���丮 ���
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.���� �����ϴ� ������ ����
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path, "./%s", dent->d_name);	//���丮�� �׸� �б�
	stat(path, &sbuf);	//stat�Լ��� �� ���� �˻�

	//strcpy(str, (char)sbuf.st_gid);	//stat�� �׷� ���̵� str�� ���� + for��
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
		//������ ���� ���丮 ���
		fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
		exit(1);
	}

	while((dent == readdir(dp))) {	//.���� �����ϴ� ������ ����
		if(dent->d_name[0] == '.')	continue;
		else	break;
	}

	sprintf(path2, "./%s", dent->d_name);	//���丮�� �׸� �б�
	stat(path2, &sbuf);

	if((int)sbuf.st_size == 0) {
		printf("%s\n", dent->d_name);
	}

	closedir(dp);
}

void showHelp() {
	fprintf(stderr,"======================================================================================\n");
	fprintf(stderr, "|--------- myfind [��� 1]...[��� n] [---�ɼ� 1][����]...[---�ɼ� n][����] ---------| \n");
	fprintf(stderr, "\t |- name [�����̸�] : [�����̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- user [�����̸�] : [�����̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- help : find��ɾ ���� ���� ��� \n");
	fprintf(stderr, "\t |- perm [���ϱ���] : [���ϱ���]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- size [���Ͽ뷮] : [���Ͽ뷮]�� �̻��� ���� �˻� \n");
	fprintf(stderr, "\t |- delete [�����̸�] : [�����̸�]�� ������ ������ �˻��ϰ� ���� \n");
	fprintf(stderr, "\t |- group [�׷��̸�] : [�׷��̸�]�� ��ġ�ϴ� ���� �˻� \n");
	fprintf(stderr, "\t |- type [��������] : [��������] �����Ͽ� ���� �˻� \n");
	fprintf(stderr, "\t |- empty : �� ���� �˻� \n");
	fprintf(stderr, "\t |- mv [�����̸�] [���丮�̸�] : [�����̸�]�� ������ ã�Ƽ� ��θ� ����ϰ�, \n");
	fprintf(stderr, "\t\t\t\t[���丮�̸�]�� ������ ���丮�� �̵� \n"); // mv ���� �̾
	// fprintf(stderr, "\t |- exec [���] {} \; : ã�� ���ϵ鿡 ���� Ư�� ����� ������ �� ��� \n");
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
