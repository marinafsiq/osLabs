/**
 * @file: ECE254/lab1/starter/ls/myls.c
 * @brief: simple ls command to list file names of a directory 
 * @date: 2014/05/01
 */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>          // for printf().  man 3 printf
#include <stdlib.h>         // for exit().    man 3 exit
#include <pwd.h>            // for getpwuid()
#include <grp.h>            // for getgrgid()
#include <time.h>           // for ctime()
#include <string.h>         // for strlen()

#define STRLEN1 128
#define STRLEN2 64
 
int main(int argc, char *argv[]) 
{
	DIR *p_dir;
	struct dirent *p_dirent;
	char str[] = "----------\0";
	char *ptr;
	int ptr_size;
	struct stat buf;
	struct tm *time;
	struct passwd *user_name;
	struct group *grp_name;
	char buffer[80];
	long year;
	int n;
        char *link[256];
        int flag = 0;


	if (argc != 3) {
		printf("Error! User should enter three arguments!");
		exit(1);
	}        

	if((strcmp(argv[1],"-u") != 0) && (strcmp(argv[1],"-c")!= 0) && (strcmp(argv[1],"-l")!= 0)) { 
	      perror("Invalid Argument");
              exit(1);

	}
 

 	if ((p_dir = opendir(argv[2])) == NULL) {
		printf("opendir(%s) failed\n", argv[2]);
		perror("Opendir Error");
		exit(1);
	}

	while ((p_dirent = readdir(p_dir)) != NULL) {
		char *str_path = p_dirent->d_name;	// relative path name!
		char format[60];
		sprintf(format, "%s/%s",argv[2], str_path);

		if (str_path == NULL) {
			perror("Null pointer found!"); 
			exit(1);
		}
		stat(format, &buf); 
		if(lstat(format, &buf)<0) {
			perror("lstat error");
			continue;
		}

                if      (S_ISREG(buf.st_mode))  str[0] = '-';
                else if (S_ISDIR(buf.st_mode))  str[0] = 'd';
                else if (S_ISCHR(buf.st_mode))  str[0] = 'c';
                else if (S_ISBLK(buf.st_mode))  str[0] = 'b';
                else if (S_ISFIFO(buf.st_mode)) str[0] = 'p';
#ifdef S_ISLNK
                else if (S_ISLNK(buf.st_mode)) {
                     str[0] = 'l';
		     flag = 1;
                     if(n = (readlink(format,link,sizeof(link)-2)) != -1) {
                        link[n] = "\0";
                     }
                 }

#endif
#ifdef S_ISSOCK
                else if (S_ISSOCK(buf.st_mode)) str[0] = 's';
#endif
                else                            str[0] = '?';

		mode_t mode = buf.st_mode;
		
		str[1] = (mode & S_IRUSR)?'r':'-';
		str[2] = (mode & S_IWUSR)?'w':'-';
		str[3] = (mode & S_IXUSR) ? 'x' : '-';

		str[4] = (mode & S_IRGRP)?'r':'-';
		str[5] = (mode & S_IWGRP)?'w':'-';
		str[6] = (mode & S_IXGRP) ? 'x' : '-';

		str[7] = (mode & S_IROTH)?'r':'-';
		str[8] = (mode & S_IWOTH)?'w':'-';
		str[9] = (mode & S_IXOTH) ? 'x' : '-';

		printf("%s  ", str);

		if (NULL == (user_name = getpwuid(buf.st_uid)))
                        perror("Error getting username.");
                else
                        printf("%s ", user_name->pw_name);

                if (NULL == (grp_name = getgrgid(buf.st_gid)))
                        perror("Error getting group name.");
                else
                        printf("%s  ",grp_name->gr_name);

		off_t size = buf.st_size;
		ptr_size = size;
		printf("%d  " , ptr_size);

		year = 2014;
		if(strcmp(argv[1],"-u")== 0) {
			time_t last_access = buf.st_atime;
			time = localtime(&last_access);
			if((time->tm_year)+1900 == year )
				strftime(buffer,80,"%b %d %H:%M" , time);
			else
				strftime(buffer,80,"%b %d %Y" , time);

			printf("%s  " , buffer);

		}

		else if(strcmp(argv[1],"-c")== 0) {
			time_t last_change = buf.st_ctime;
			time = localtime(&last_change);
			if((time->tm_year)+1900 == year )
				strftime(buffer,80,"%b %d %H:%M" , time);
			else
				strftime(buffer,80,"%b %d %Y" , time);

			printf("%s  " , buffer);

		}

		else if(strcmp(argv[1],"-l")== 0) {
			time_t last_modify = buf.st_mtime;
			time = localtime(&last_modify);
			if((time->tm_year)+1900 == year )
				strftime(buffer,80,"%b %d %H:%M" , time);
			else
				strftime(buffer,80,"%b %d %Y" , time);

			printf("%s  " , buffer);

		}
		else {
			perror("Invalid Argument");
			exit(1);
		}

                if(flag == 0)
		 printf("%s\n",str_path);
                else {
	          printf("%s -> %s\n",str_path,link);
 		flag = 0;
 	      }
	}
	return 0;

}
