#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>

/* global variable declaration */

/* global function declaration */
void findFilesWithNameOfFile(char *, char *);
void findFilesWithNameOfUser(char *, char *);
void showHowToUseMYFIND();
void group(char *, char *);
void empty(char *);
void type(char *, char *);


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
                break;
            case 's':
                printf("size \n");
                break;
            case 'd':
                printf("delete \n");
                break;
            case 'g':
                group(argv[1], optarg);
                break;
            case 't':
				type(argv[1], optarg);
                break;
            case 'm':
                break;
            case 'x':
                printf("exec \n");
                break;
            case 'h':
                printf("help \n");
                showHowToUseMYFIND();
                break;
            case 'e':
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
    fprintf(stderr, "\t |- name [파일이름] : ‘파일이름’과 일치하는 파일 검색 \n");
    fprintf(stderr, "\t |- user [유저이름] : ‘유저이름’과 일치하는 파일 검색 \n");
    fprintf(stderr, "\t |- help : find명령어에 대한 설명 출력 \n");
    fprintf(stderr, "\t |- perm [파일권한] : ‘파일권한’과 일치하는 파일 검색 \n");
    fprintf(stderr, "\t |- size [파일용량] : ‘파일용량’이상의 파일 검색 \n");
    fprintf(stderr, "\t |- delete [파일이름] : ‘파일이름’에 지정한 파일을 검색하고 삭제 \n");
    fprintf(stderr, "\t |- group [그룹이름] : ‘그룹이름’과 일치하는 파일 검색 \n");
    fprintf(stderr, "\t |- type [파일종류] : ‘파일종류‘를 지정하여 파일 검색 \n");
    fprintf(stderr, "\t |- empty : 빈 파일 검색 \n");
    fprintf(stderr, "\t |- mv [파일이름] [디렉토리이름] : ‘파일이름’의 파일을 찾아서 경로를 출력하고 \n");
    fprintf(stderr, "\t |- ‘디렉토리이름’에 지정한 디렉토리로 이동 \n");
    // fprintf(stderr, "\t |- exec [명령] {} \; : 찾은 파일들에 대한 특정 명령을 수행할 때 사용 \n");
    fprintf(stderr,"======================================================================================\n");
}

void group(char* path, char *arg) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ];
    int i = 0;
    //char str[255];    //함수의 그룹 아이디 저장 배열
    int gname;
    struct group *grp;

    if((dp = opendir(path)) == NULL) { //Error for directory open
        perror("opendir");
        exit(1);
    }

    while((dent = readdir(dp))) {  
        //.으로 시작하는 파일은 생략
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
           printf("찾는 그룹명과 일치하는 파일이 없습니다.\n");   //A case that file are not exist in the directory
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
        //.으로 시작하는 파일은 생략
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
	    printf("ã�� Ÿ���� ������ �������� �ʽ��ϴ�.\n");
    }
}
