#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


#define _XOPEN_SOURCE 600

int i;
//----------------- PRODUCER -----------------
int main(int argc, char *argv[])
{
        mqd_t qdes;
        char  qname[] = "/mailboxLab5"; //The real mailbox. The messages are sent to this mailbox and also received from this mailbox
        mqd_t qdes2;
        char  qname2[] = "/mb_control"; //This mailbox is to control how many items were produced and consumed. 
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;

        attr.mq_maxmsg  = atoi(argv[2]); //Receiving "b" from producer
        attr.mq_msgsize = sizeof(int); // The size of each message
        attr.mq_flags   = 0;    // a blocking queue  

        qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr); //Opening mailboxLab5
        if (qdes == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

        qdes2  = mq_open(qname2, O_RDONLY, mode, &attr); //Opening the correct mailbox
        if (qdes == -1 ) {
                perror("mq_open() failed");
                exit(1);
        }

        int rcv; //Just to 'receive' the messages of the mb_control
        //Sending message to mailboxLab5
        for(i=atoi(argv[4]); i<atoi(argv[1]); i=i+atoi(argv[3])){   // argv[4] = pid  argv[1]=N  argv[3]=P
            //Receiving message from mb_control ---- THIS PART IS LIKE A SEMAPHORE
             if (mq_receive(qdes2, (char *)&rcv , sizeof(int), 0) == -1)
                  perror("mq_receive() failed ");

             //adding iten to the mailboxLab5
             if (mq_send(qdes, (char *)&i , sizeof(int), 0) == -1) 
                   perror("mq_send() failed ");
       }
        
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
