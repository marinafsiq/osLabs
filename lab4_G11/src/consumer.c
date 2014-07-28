#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#define _XOPEN_SOURCE 600

int i;

int main(int argc, char *argv[])
{
        mqd_t qdes;
        char  qname[] = "/mailbox1_G11";
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

        attr.mq_maxmsg  = atoi(argv[1]); //Receiving "b" from producer
        attr.mq_msgsize = sizeof(int); // The size of each message 
        attr.mq_flags   = 0;    /* a blocking queue  */

        qdes  = mq_open(qname, O_RDONLY, mode, &attr); //Opening the correct mailbox 
        if (qdes == -1 ) {
                perror("mq_open()");
                exit(1);
        }

        for(i=0 ; i<atoi(argv[2]) ; i++) { //Consuming "N" integers that were produced by producer
                int rand_num;

                //Receiving the messages sent by the producer
                if (mq_receive(qdes, (char *)&rand_num, sizeof(int), 0) == -1) {
                } else {
                        printf("%d is consumed\n",rand_num);
                }
        }

       //Closing the mailbox and checking for errors in the process
        if (mq_close(qdes) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        return 0;
}
                                 

