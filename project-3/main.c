#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CRNT_DIR "."
#define PARENT_DIR ".."
#define SUCCESS 0

void PathCombine(char *path1, char *path2, char *returnValue);
void PrintInfo(char *startingDirPath);

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    PrintInfo("./");
  }
  else
  {
    PrintInfo(argv[1]);
  }
  
  return(SUCCESS);
}

void PathCombine(char *path1, char *path2, char *returnValue)
{
  strcpy(returnValue, path1);
  strcat(returnValue, "/");
  strcat(returnValue, path2);
}

void PrintInfo(char *startingDirPath)
{

  DIR *dir = opendir(startingDirPath);

  struct dirent *dp;    
  if (dir != NULL)
  {
    while((dp = readdir(dir)) != NULL)
    {
      if ((strncmp(dp->d_name, CRNT_DIR, 100) != 0) && (strncmp(dp->d_name, PARENT_DIR, 100)) != 0)
      {
        struct stat sb;
        char *path[100];
        PathCombine(startingDirPath, dp->d_name, path);
        int statResult = stat(path, &sb);
        if (statResult == 0)
        {
          if (S_ISDIR(sb.st_mode))
          {
            printf("%s Size: %zd\n", dp->d_name, sb.st_size);
            PrintInfo(path);
          }
          else
          {
            printf("%s Size: %zd\n", dp->d_name, sb.st_size);
          }
        }
      }
    }
  }
  else
  {
    struct stat sb;
    int statResult = stat(startingDirPath, &sb);
    if (statResult == 0)
    {
        printf("found a file: %s Size: %zd\n", dp->d_name, sb.st_size);
    }
  }
    closedir(dir);
}
