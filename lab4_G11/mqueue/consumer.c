#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include "common.h"
#include "point.h"
#include <unistd.h>
#include <sys/time.h>


#define _XOPEN_SOURCE 600

bool g_continue = true;
int i;

void sig_handler(int sig)
{
        g_continue = false;
}

int main(int argc, char *argv[])
{
        mqd_t qdes;
        char  qname[] = "/mailbox1_G11";
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

       //RECEIVE IT AS AN ARGUMENT
        attr.mq_maxmsg  = atoi(argv[1]);
        //printf("value of B received in consumer = %d\n",atoi(argv[1]));
        attr.mq_msgsize = sizeof(int);
        attr.mq_flags   = 0;    /* a blocking queue  */

        qdes  = mq_open(qname, O_RDONLY, mode, &attr);
        if (qdes == -1 ) {
                perror("mq_open()");
                exit(1);
        }

       signal(SIGINT, sig_handler);    /* install Ctl-C signal handler */

        srand(time(0));

        for(i=0 ; i<atoi(argv[2]) ; i++) {
                int rand_num;

                /* only block for a limited time if the queue is empty */
                if (mq_receive(qdes, (char *)&rand_num, \
                    sizeof(int), 0) == -1) {
                } else {
                        printf("%d is consumed\n",rand_num);
                }
        }

        if (mq_close(qdes) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        return 0;
}
                                 

