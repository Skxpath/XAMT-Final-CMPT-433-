#include "fileSys.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void echoToFile(char* fileName, char* buff)
{
  FILE* file = fopen(fileName, "w");
  if (file == NULL)
  {
    printf("Unable to open file %s\n", fileName);
    exit(-1);
  }

  int writeSuccess = fprintf(file, "%s", buff);
  if (writeSuccess < 0)
  {
    printf("Unable to write to file %s\n", fileName);
    exit(-1);
  }

  fclose(file);
}

int readDigitFromFile(char* fileName)
{
  FILE* file = fopen(fileName, "r");
  if (file == NULL)
  {
      printf("Unable to open file %s\n", fileName);
      exit(-1);
  }
  const int maxLength = 1024;
  char buff[maxLength];
  fgets(buff, maxLength, file);

  int value = atoi(buff);

  fclose(file);
  return value;
}
