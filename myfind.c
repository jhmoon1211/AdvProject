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
void findFileWithNameOfFile(char * path, char * nameOfFile);
int group(char *, char *);
int empty(char *);

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

	    while (1) {
			opt = getopt_long (argc, argv, "n:u:p:s:d:g:t:m:x:he", long_options, &option_index);
	        if (opt == -1) break;
	        switch (opt) {
	            case 'n': /* name */
	                path = argv[1];
	                nameOfFile = argv[3];
	                findFileWithNameOfFile(path, nameOfFile);
	                break;
	            case 'u': /* user */
	                printf("user \n");
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
	                printf("group \n");
					group(argv[1], optarg);
	                break;
	            case 't':
	                printf("type \n");
	                break;
	            case 'm':
	                printf("mv \n");
	                break;
	            case 'x':
	                printf("exec \n");
	                break;
	            case 'h':
	                printf("help \n");
	                break;
	            case 'e':
	                printf("empty \n");
					empty(argv[1]);
	                break;
	        }
		}
		return 0;
}

void findFileWithNameOfFile(char * path, char * nameOfFile) {
	    printf("%s \n", path); // path
		printf("%s \n", nameOfFile); // name of file
}

int group(char* path, char *arg) {
     DIR *dp;
     struct dirent *dent;
     struct stat sbuf;
     char path2[BUFSIZ];
     int i = 0;
     //char str[255];    //�Լ��� �׷� ���̵� ���� �迭
     int gname;
     struct group *grp;

     if((dp = opendir(path)) == NULL) {
         //������ ���� ���丮 ���
         fprintf(stderr, "opendir : %s\n", getcwd(NULL, BUFSIZ));
         exit(1);
     }

     while((dent = readdir(dp))) {  //.���� �����ϴ� ������ ����
         //if(dent->d_name[0] == '.')  continue;
         //else    break;
         sprintf(path2, "%s/%s", path, dent->d_name);
         stat(path2, &sbuf);

         gname = atoi(arg);
         if(gname == (int)sbuf.st_gid) {
            printf("%d %s\n", gname, dent->d_name);
            i++;
         }

         if(i != 0) {
            continue;
         }
         else if((grp = getgrnam(arg)) ==  NULL) {
            printf("ã�� �׷��� ��ġ�ϴ� ������ �����ϴ�.\n");
         }
         else if(((int)sbuf.st_gid) == ((int)grp->gr_gid)) {
            printf("%d %s\n", (int)sbuf.st_gid, dent->d_name);
         }
     }

     //sprintf(path, "./%s", dent->d_name);    //���丮�� �׸� �б�
     //stat(path, &sbuf);  //stat�Լ��� �� ���� �˻�

     //strcpy(str, (char)sbuf.st_gid);   //stat�� �׷� ���̵� str�� ���� +     for��
     //if(!strcmp(*arg, str)) {
     //  printf("%s\n", dent->d_name);
     //}
     /*
     gname = atoi(*arg);
     if(gname == (int)sbuf.st_gid) {
         printf("%s\n", dent->d_name);
     }
     */
     closedir(dp);

     return 0;
}

int empty(char* path) {
    DIR *dp;
    struct dirent *dent;
    struct stat sbuf;
    char path2[BUFSIZ];
    //char** path1 = path;

    if((dp = opendir(path)) == NULL) {
         //������ ���� ���丮 ���
		 perror("opendir");
         exit(1);
    }

    while((dent = readdir(dp))) {  //.���� �����ϴ� ������ ����
        //if(dent->d_name[0] == '.')  continue;
        //else    break;
        sprintf(path2, "%s/%s", path, dent->d_name);
        stat(path2, &sbuf);

        if((int)sbuf.st_size == 0) {
	        printf("%s\n", dent->d_name);
        }
    }

    /*
    printf("test4\n");
    printf("%s", dent->d_name);
    sprintf(path2, "%s/%s\n", path, dent->d_name);   //���丮�� �׸� �б�
    printf("test5\n");
    stat(path2, &sbuf);
    printf("test6\n");

    if((int)sbuf.st_size == 0) {
        printf("test7\n");
        printf("%s\n", dent->d_name);
        printf("test8\n");
    }
    */

    closedir(dp);

    return 0;
}
