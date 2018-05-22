#ifndef SPLIT_H
#define SPLIT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_STR 4096
#define MAX_VET_STR 4096

/*
Split taken from:
http://source-code-share.blogspot.it/2014/07/implementation-of-java-stringsplit.html
*/

int split (char *str, char c, char ***arr)
{
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0')
    {
      if (*p == c)
        count++;
      p++;
    }

  *arr = (char**) malloc(sizeof(char*) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0')
    {
      if (*p == c)
      {
        (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
        if ((*arr)[i] == NULL)
          exit(1);

        token_len = 0;
        i++;
      }
      p++;
      token_len++;
    }
  (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
  if ((*arr)[i] == NULL)
    exit(1);

  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0')
    {
      if (*p != c && *p != '\0')
      {
        *t = *p;
        t++;
      }
      else
      {
        *t = '\0';
        i++;
        t = ((*arr)[i]);
      }
      p++;
    }

  return count;
}


#endif /* SPLIT_H */