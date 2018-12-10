#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main()
{
  //place to put the shared memory, open a file
  int thing = open("./picture.jpg", O_CREAT, 0644);

  //create a unique key for the shared memory
  key_t key = ftok("./picture.jpg", 'R');

  //returns identifier for shared memory segment
  int got = shmget(key, 200 * sizeof(char), 0644 | IPC_CREAT);

  //this is just to store the newline for fscanf
  char *junk = malloc(sizeof(char));

  //ptr to shared memory
  char * ptr = shmat(got, 0, 0);

  //if there's anything in the memory, print it
  if(*ptr)
    printf("original contents: %s\n", ptr);

  //otherwise, there's nothing to print
  else
    printf("the segment was just created\n");
  
  printf("do you want to change the data in the segment(y/n)?\n");

  //to store user input
  char* line = malloc(256 * sizeof(char));

  //get the input
  fscanf(stdin, "%[^\n]s", line);
  fscanf(stdin, "%c", junk);

  //if the first letter is n, it's probably a no
  if(!strncmp(line, "n", 1) || !strncmp(line, "N", 1))
    printf("okay, i won't change anything.\n");

  //if the first letter is y, it's probably a yes
  else if(!strncmp(line, "y", 1) || !strncmp(line, "Y", 1))
    {
      printf("okay, what do you want in the string?\n");
      //take user input
      fscanf(stdin, "%[^\n]s", line);
      fscanf(stdin, "%c", junk);
      //put it in the shared memory
      strncpy(ptr, line, 200);
      //diagnostic print statement
      //printf("now the memory has this in it: %s\n", ptr);
    }
  else
    printf("...i'll just take that as a no.\n");
  
  printf("do you want to delete the segment(y/n)?\n");

  fscanf(stdin, "%[^\n]s", line);
  fscanf(stdin, "%c", junk);

  if(!strncmp(line, "n", 1) || !strncmp(line, "N", 1))
    printf("okay, it stays.\n");

  else if(!strncmp(line, "y", 1) || !strncmp(line, "Y", 1))
    {
      shmctl(got, IPC_RMID, NULL);
      printf("okay, it's gone.\n");
    }
  
  else
    printf("...i'll just take that as a no.\n");
  free(line);
  free(junk);
  return 0;
}
