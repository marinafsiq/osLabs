#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <semaphore.h> 


double t1, t2; //initializing the timing variables
struct timeval tv;
sem_t items_counter;
sem_t binary_sem;
pid_t ch_pid[50];

//------ CONSUMER SPAWN ------
int spawn (char* program, char* n, char* b, char* c, int id, char* p)
{
  pid_t child_pid;
  char char_id[6];
  snprintf(char_id, 6," %d", id);
  child_pid = fork ();
  ch_pid[id+atoi(p)] = child_pid;
   
  char* arg_list[] = {
      "consumer", //argv[0], the name of the program 
      n,    //The number of items to be produced by the producer
      b,    //The size of the queue mailbox
      c,   //Number of consumers  
      char_id,   //pid
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
    fprintf (stderr, "an error occurred in execvp in consumer\n");
    abort ();
  }
}


//------ PRODUCER SPAWN -----
int spawn2 (char* program, char* n, char* b, char* p, int id)
{
  pid_t child_pid;
  char char_id[6];
  snprintf(char_id, 6," %d", id); //transforning integer into char
  child_pid = fork ();
  ch_pid[id] = child_pid;

  //arg_list contains a list of arguments that will be passed to  the consumer
  char* arg_list[] = {
      "producer", //argv[0], the name of the program
      n, //The size of the queue mailbox
      b, //The number of items to be produced by the producer
      p,
      char_id,
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
    fprintf (stderr, "an error occurred in execvp in producer\n");
    abort ();
  }
}
 


int main(int argc, char *argv[])
{
        int child_status;
        pid_t pid;
        mqd_t qdes;
        mqd_t qdes2;
        char  qname2[] = "/mb_control";
        char  qname[] = "/mailboxLab5"; //queue name must start with '/'
        mode_t mode = S_IRUSR | S_IWUSR;
        struct mq_attr attr;
 //       int i = 0;
//        int ii=0;
        int j;
        int a[50];

        if ( argc !=5 ) {
		printf("You have to enter: ./produce <N> <B>\n");
                printf("<N> = number of integers the producer should produce\n");
		printf("<B> = number of integers the message queue can hold\n");
                printf("<P> = number of producers\n");
		printf("<C> = number of consumers\n");
                exit(1);
        }
       
        attr.mq_maxmsg  = atoi(argv[2]); //Receiving "b" from producer
        attr.mq_msgsize = sizeof(int); // The size of each message 
        attr.mq_flags   = 0;    // a blocking queue  

        qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr); //Opening the mailboxLab5 
        if (qdes == -1 ) {
                perror("mq_open()");
                printf("mq error inside MAIN  %d\n", getpid());
                exit(1);
        }
 
        qdes2  = mq_open(qname2, O_RDWR | O_CREAT, mode, &attr); //Opening the control mailbox - mb_control 
        if (qdes2 == -1 ) {
                perror("mq_open()");
                printf("mq error inside MAIN  %d\n", getpid());
                exit(1);
        }
         
           int msg_null = 0;
        for(j=0; j<atoi(argv[2]); j++){
           if(mq_send(qdes2, (char*)&msg_null, sizeof(int), 0) == -1){
              sleep(1);
              perror("mq_send() failed");
          } 
        }

        gettimeofday(&tv, NULL);
        t1 = tv.tv_sec + tv.tv_usec/1000000.0; //Time before creating first process
       
        for(j=0; j<atoi(argv[3]);j++) { //loops till the number of producers
          a[j]=j;
          if(spawn2("./producer.out", argv[1], argv[2],argv[3],a[j]) != 0) {
            continue; // This is to continue doing the fork when it is the parent process
          }
        }

        for(j=0; j<atoi(argv[4]);j++) { //loops till the number of consumers
          a[j+atoi(argv[3])]=j;
          if(spawn("./consumer.out", argv[1], argv[2],argv[4],a[j+atoi(argv[3])],argv[3]) != 0) {
            continue; // This is to continue doing the fork when it is the parent process
          }
        }
       
       //Calling waitpid. So the main function can wait all the processes finish, and then it can finish
       for(j=0; j<(atoi(argv[3]) + atoi(argv[4])); j++){ 
           waitpid(ch_pid[j],&child_status,0);
           if(WIFEXITED(child_status)){
           } else  
             printf("%d was  NOT waited  %d\n",j,ch_pid[j]);
       }

        if (mq_close(qdes) == -1) {
                perror("mq_close() failed inside MAIN");
                exit(2);
        }

        if (mq_close(qdes2) == -1) {
                perror("mq_close() failed inside MAIN");
                exit(2);
        }
        if (mq_unlink(qname) != 0) { 
            perror("mq_unlink() failed inside MAIN");
                exit(3);
        }
        if (mq_unlink(qname2) != 0) {                             
            perror("mq_unlink() failed inside MAIN");
                exit(3);
        }
        
        gettimeofday(&tv, NULL);
        t2 = tv.tv_sec + tv.tv_usec/1000000.0; //Time after last integer is consumed
        printf("System execution time: %f seconds\n", t2-t1); //Calculating the system execution time
        return 0;
}


