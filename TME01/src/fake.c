#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_BUF 128

char* f2s = "/tmp/monsuperf2s";
char* s2f = "/tmp/monsupers2f";

typedef struct _mystack mystack;
struct _mystack {
  int id;
  char command[MAX_BUF-3];
  mystack* next;
};

mystack *stack, *stack_ptr;
int stack_counter = 0;

/*******************************************************************************
 * Private Declarations
 ******************************************************************************/
void stdin_handler(void);
void* stdout_handler(void* args);

/*******************************************************************************
 * Local Implementation
 ******************************************************************************/
int main(int argc, char** argv)
{
  /* main s'occupe des stdin, th des stdout */
  pthread_t th;
  if( pthread_create(&th, NULL, stdout_handler, NULL) == -1 ){
    perror("Error: pthread_create\n");
    exit(EXIT_FAILURE);
  }

  stdin_handler();

  return EXIT_SUCCESS;
}

void stdin_handler(void)
{
  char buffer[MAX_BUF];

  mkfifo(f2s, 0600);

  int fd = open(f2s, O_WRONLY);
  if( fd == -1 ){
    perror("Error: stdin: open\n");
    exit(1);
  }

  /* main loop */
  while(1){
    memset(buffer, '\0', MAX_BUF);

    if( read(STDIN_FILENO, buffer, MAX_BUF) == -1 ){
      perror("Error: read\n");
      exit(1);
    }

    if( !stack_counter ){
      stack = (mystack*)malloc(sizeof(mystack));
      stack_ptr = stack;
    } else {
      stack_ptr->next = (mystack*)malloc(sizeof(mystack));
      stack_ptr = stack_ptr->next;
    }

    stack_ptr->id = stack_counter++;
    strcpy(stack_ptr->command, buffer);

    memset(buffer, '\0', MAX_BUF);
    sprintf(buffer, "%d:%s", stack_ptr->id, stack_ptr->command);

    printf("stdin_handler va write %s\n", buffer);

    if( write(fd, buffer, MAX_BUF) == -1 ){
      perror("Error: write\n");
      exit(1);
    }
  }

  if( close(fd) == -1 ){
    perror("Error : close\n");
    exit(1);
  }
}

void* stdout_handler(void* args)
{
  char buffer[MAX_BUF];
  int id, nbchar;
  mystack* tmp_ptr;

  int fd = open(s2f, O_RDONLY);
  if( fd == -1 ){
    perror("Error: stdout: open\n");
    exit(1);
  }

  while(1){
    memset(buffer, '\0', MAX_BUF);
    
    if( read(fd, buffer, MAX_BUF) == 0){
      perror("Error: stdout: read\n");
      exit(1);
    }
    
    sscanf(buffer, "%d:%d", &id, &nbchar);

    /* Recherche simple en O(n) ... */
    tmp_ptr = stack;
    while( tmp_ptr ){
      if( id == tmp_ptr->id ){
	printf("%d:%s:%d\n", id, tmp_ptr->command, nbchar);
	break;
      }
      tmp_ptr = tmp_ptr->next;
    }
    if( !tmp_ptr ){
      printf("Commande %d introuvable\n", id);
    }
  }

  if( close(fd) == -1 ){
    perror("Error: close\n");
    exit(1);
  }

  return NULL;
}
