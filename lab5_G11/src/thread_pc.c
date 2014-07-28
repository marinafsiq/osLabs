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
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>


double t1, t2; //initializing the timing variables
struct timeval tv;


pthread_t thread_id;
pthread_t thread_pc[10];
 int n;
 int b;
 int p;
 int c;

sem_t cb_count;
sem_t buffer_limit;

//Taken from http://en.wikipedia.org/wiki/Circular_buffer

/* Circular buffer example, keeps one slot open */
 
/* Opaque buffer element type.  This would be defined by the application. */
typedef struct { int value; } ElemType;
 
/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    ElemType   *elems;  /* vector of elements                   */
} CircularBuffer;
 
void cbInit(CircularBuffer *cb, int size) {
    cb->size  = size + 1; /* include empty elem */
    cb->start = 0;
    cb->end   = 0;
    cb->elems = calloc(cb->size, sizeof(ElemType));
   //Initialising semaphores
    sem_init(&cb_count, 0, 0);   //count how many items were produced and consumed
    sem_init(&buffer_limit, 0, size); //This semaphore is used to count how many items were put in the buffer. 
                                      //Every time the producer adds an item in the buffer, we decrement this semaphore
                                      //If the number of items is equal size inside the buffer (the semaphore was decremented size times), the producer should be blocked
}
 
void cbFree(CircularBuffer *cb) {
    free(cb->elems); /* OK if null */
}
 
int cbIsFull(CircularBuffer *cb) {
    return (cb->end + 1) % cb->size == cb->start;
}
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->end == cb->start;
}
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, ElemType *elem) {
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}
 
/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % cb->size;
}


CircularBuffer cb;
ElemType elem = {0};
pthread_mutexattr_t attr;
pthread_mutex_t cb_mutex = PTHREAD_MUTEX_INITIALIZER; //initializing the mutex

//Producer
void* producer_thread(void* tparams)
{
    int thr_id = (*(int*)tparams);
    int i;
    int val = 0;
    for(i=thr_id; i<n; i=i+p) {   //with this loop, each producer thread will produce the right amount of items according to its id
       sem_wait(&buffer_limit); //blocks if the buffer is full
       pthread_mutex_lock(&cb_mutex); //protecting critical section
       elem.value = i;
       cbWrite(&cb , &elem);
       pthread_mutex_unlock(&cb_mutex);   
       sem_post(&cb_count); //increments the counter
     }
   return NULL;
}

void* consumer_thread(void* tparams)
{  
   int i;
   int thr_id = (*(int*)tparams);
   for(i=thr_id; i<n; i=i+c ){
     sem_wait(&cb_count); //blocks if a thread tries to consume more than what was produced
     pthread_mutex_lock(&cb_mutex); //critical section
     if(!cbIsEmpty(&cb)) {
       cbRead(&cb,&elem);
       if(sqrt(elem.value)/1.00 == (int)sqrt(elem.value)) //checks if the square root of the value consumed is an integer
         printf("%d %d %d\n",thr_id,elem.value,(int)sqrt(elem.value));
      }   
      pthread_mutex_unlock(&cb_mutex); 
      sem_post(&buffer_limit); 
    }
  return NULL;
}

int main(int argc, char *argv[])
{ 
  int ret;
  int i;
  
  n = atoi(argv[1]);
  b = atoi(argv[2]);
  p = atoi(argv[3]);
  c = atoi(argv[4]);
  int a[20];
 

  cbInit(&cb, atoi(argv[2]));

  gettimeofday(&tv, NULL);
  t1 = tv.tv_sec + tv.tv_usec/1000000.0; //get time before the first thread is created

  for(i=0 ; i < (atoi(argv[3]) + atoi(argv[4])) ; i++) {
    if(i < atoi(argv[3])){
       a[i] = i;
       ret =  pthread_create(&thread_pc[i],NULL,&producer_thread,(void*)&a[i]); //creating the producer threads
    }else{
       a[i] = i-atoi(argv[3]);
       pthread_create(&thread_pc[i],NULL,&consumer_thread,(void*)&a[i]);  //creating the consumer threads
   }
  }
  for(i=0; i<(atoi(argv[3])+atoi(argv[4])); i++) {
     pthread_join(thread_pc[i], NULL); //it makes the main thread wait for the others to finish
  }

  gettimeofday(&tv, NULL);
  t2 = tv.tv_sec + tv.tv_usec/1000000.0; //get time after last item was consumed 
  printf("System execution time: %f seconds", t2-t1);

  return 0;
}
