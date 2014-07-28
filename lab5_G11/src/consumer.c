#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#define _XOPEN_SOURCE 600

int i;
//------------------- CONSUMER --------------------
int main(int argc, char *argv[])
{
        mqd_t qdes;
        char  qname[] = "/mailboxLab5"; //mailbox with the real messages
        mqd_t qdes2;
        char  qname2[] = "/mb_control"; //mailbox to control the itens produced and consumed
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

        attr.mq_maxmsg  = atoi(argv[2]); //Receiving "b" from producer
        attr.mq_msgsize = sizeof(int); // The size of each message 
        attr.mq_flags   = 0;    // a blocking queue  

        qdes  = mq_open(qname, O_RDONLY, mode, &attr); //Opening mailboxLab5 
        if (qdes == -1 ) {
                perror("mq_open()");
                printf("mq error inside CONSUMER %d\n", getpid());
                exit(1);
        }

        qdes2  = mq_open(qname2, O_RDWR | O_CREAT, mode, &attr); //Opening mb_control 
        if (qdes2 == -1 ) {
                perror("mq_open()");
                printf("mq error inside CONSUMER %d\n", getpid());
                exit(1);
        }

        int send = 0;
        for(i=atoi(argv[4]) ; i<atoi(argv[1]) ; i=i+atoi(argv[3])) { //same loop than the producer. Just the last argument is different (here it is argv[3]=C). Each consumer eats their own AMOUNTS
            int int_num;
            if (mq_receive(qdes, (char *)&int_num, sizeof(int), 0) == -1) {
                  perror("mailbox failed");
            } 
            else {
                double sqroot = sqrt(int_num);
                if(sqroot/1.00 == (int)sqroot) //Checking if the root of the received message is an integer 
                    printf("%d %d %d\n",atoi(argv[4]),int_num,(int)sqroot);
                if(mq_send(qdes2,(char*)&send, sizeof(int), 0) == -1)
                    perror("mailbox failed");
            }
        }//END FOR
 
       //Closing the mailboxes and checking for errors in the process
        if (mq_close(qdes) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        if (mq_close(qdes2) == -1) {
                perror("mq_close() failed");
                exit(2);
        }

        return 0;
}
                                 

