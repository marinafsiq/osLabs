#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include "common.h"
#include "point.h"
#include <sys/time.h>
#include <sys/wait.h>
  

double t1, t2, t3;
struct timeval tv;

int spawn (char* program, char* b, char* n)
{
  pid_t child_pid;

  /* Duplicate this process.  */
  gettimeofday(&tv, NULL);
  t1 = tv.tv_sec + tv.tv_usec/1000000.0;
  child_pid = fork ();
  gettimeofday(&tv, NULL);
  t2 = tv.tv_sec + tv.tv_usec/1000000.0;

  char* arg_list[] = {
      "consumer", //argv[0], the name of the program 
      b,
      n,
      NULL //the argument list MUST end with NULL
  };

  if (child_pid != 0) {
    /* This is the parent process.  */
    return child_pid;
 }
  else {
    /* Now execute PROGRAM, searching for it in the path.  */
    execvp (program, arg_list);
    /* The execvp function returns only if an error occurs.  */
    fprintf (stderr, "an error occurred in execvp\n");
    abort ();
  }
}

int main(int argc, char *argv[])
{

        int child_status;
        mqd_t qdes;
        //char  quit = '\0';
        char  qname[] = "/mailbox1_G11"; //queue name must start with '/'
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;
        int i;

        if ( argc !=3 ) {
		printf("You have to enter: ./produce <N> <B>");
                printf("<N> = number of integers the producer should produce\n");
		printf("<B> = number of integers the message queue can hold\n");
                exit(1);
        }
        spawn("./consumer.out", argv[2], argv[1]);

        attr.mq_maxmsg  = atoi(argv[2]); //the maximum number of messages in the queue. <B>
        //printf("The value of B is: %d\n",atoi(argv[2]));
        attr.mq_msgsize = sizeof(int);
        attr.mq_flags   = 0;            /* a blocking queue  */

        qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
        if (qdes == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

        srand(time(NULL));
        for(i=0; i<atoi(argv[1]); i++) {
                int rand_num;
                rand_num = rand()%100;
                if (mq_send(qdes, (char *)&rand_num , sizeof(int), 0) == -1) {
                        perror("mq_send() failed");
                }
                //printf("The random number generated is: %d\n",rand_num);
        }

       wait(&child_status);

        gettimeofday(&tv, NULL);
        t3 = tv.tv_sec + tv.tv_usec/1000000.0;
        printf("Time to initialize system: %f seconds\n", t2-t1);
        printf("Time to transmit data: %f seconds\n", t3-t2);

        if (mq_close(qdes) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        if (mq_unlink(qname) != 0) {
                                     
            perror("mq_unlink() failed");
                exit(3);
        }
        return 0;
}

