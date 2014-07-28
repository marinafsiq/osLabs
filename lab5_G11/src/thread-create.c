/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <pthread.h>
#include <stdio.h>

/* Prints x's to stderr.  The parameter is unused.  Does not return.  */

void* print_xs (void* unused)
{
 int i=0;
  while (i<5){ 
    fputc ('x', stderr);
    i++;
  }
  printf("thread_id = %u\n", pthread_self());
  return NULL;
}

/* The main program.  */

int main ()
{
  int j=0;
  pthread_t thread_id;
  /* Create a new thread.  The new thread will run the print_xs
     function.  */
  pthread_create (&thread_id, NULL, &print_xs, NULL);
  /* Print o's continuously to stderr.  */
  while (j<50){ 
    fputc ('o', stderr);
    j++;
  }
  pthread_join(thread_id, NULL);
  return 0;
}
