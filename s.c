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
  int thing = open("./picture.jpg", O_CREAT, 0644);
  key_t key = ftok("./picture.jpg", 'R');
  int got = shmget(key, 200 * sizeof(char), 0644 | IPC_CREAT);
  char *junk = malloc(sizeof(char));
  char * ptr = shmat(got, 0, 0);
  if(*ptr)
    printf("original contents: %s\n", ptr);
  else
    printf("the segment was just created\n");
  printf("do you want to change the data in the segment(y/n)?\n");
  char* line = malloc(256 * sizeof(char));
  fscanf(stdin, "%[^\n]s", line);
  fscanf(stdin, "%c", junk);
  if(!strncmp(line, "n", 1))
    printf("okay, i won't change anything.\n");
  else if(!strncmp(line, "y", 1))
    {
      printf("okay, what do you want in the string?\n");
      fscanf(stdin, "%[^\n]s", line);
      fscanf(stdin, "%c", junk);
      strncpy(ptr, line, 200);
      printf("now the memory has this in it: %s\n", ptr);
    }
  else
    printf("...i'll just take that as a no. goodbye.\n");
  free(line);
  free(junk);
  return 0;
}
