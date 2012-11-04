#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>

#define ON 1
#define OFF 0
#define PAGELEN 23
#define LINELEN 512

void DoMore(FILE*, int);
int SeeMore(FILE*);
int SetTcFlag(int, int, FILE*);
void HandleSignals(int);
void PrintStringReverseVideo(char*);
int GetFileLineCount(char*);
int CalcPercent(int, int);

int main(int argc, char *argv[])
{
  signal(SIGINT, HandleSignals);

  int fd_tty;
  FILE *fp;

  if (argc == 1)
  {
    DoMore(stdin, 0);
  }
  else
  {
    char *fileName = *++argv;
    int lnCount = GetFileLineCount(fileName);
    fp = fopen(fileName, "r");
    if (fp != NULL)
    {
      DoMore(fp, lnCount);
      fclose(fp);
    }
    else
    {
      perror("Could not open file %s\n");
      exit(1);
    }
  }
}

void DoMore(FILE* fp, int lnCount)
{
  char line[LINELEN];
  int numLines = 0;
  int reply;
  int totalLinesDisplayed = 0;

  while(fgets(line, LINELEN, fp))
  {
    if (numLines == PAGELEN)
    {
      char *percent;
      sprintf(percent, "%d%%", CalcPercent(totalLinesDisplayed, lnCount));
      PrintStringReverseVideo(percent);

      reply = SeeMore(fp);
      if (reply == 0)
      {
        break;
      }
      numLines -= reply;
    }
    
    if (fputs(line, stdout) == EOF)
    {
      exit(1);
    }
    numLines++;
    totalLinesDisplayed++;
  }
}

int SeeMore(FILE *fp)
{
  int fd;
  char c;


  while((c = getchar()) != EOF)
  {
    if (c == 'q')
    {
      return 0;
    }
    if (c == ' ')
    {
      return PAGELEN;
    }
    if (c == '\n')
    {
      return 1;
    }
  }
  return 0;
}

int SetTcFlag(int flag, int onOff, FILE *fp)
{
  struct termios oldAttr, newAttr;
  if (tcgetattr(fileno(fp), &oldAttr) != 0)
    return -1;

  newAttr = oldAttr;
  if (onOff == ON)
  {
    newAttr.c_lflag |= ECHO;
  }
  else
  {
    newAttr.c_lflag &= ~(ICANON | ECHO);
  }

  if (tcsetattr(fileno(fp), TCSAFLUSH, &newAttr) != 0)
    return -1;
}

void HandleSignals(int sigNum)
{
  if (sigNum == SIGINT)
  {
    PrintStringReverseVideo("Reset all flags\n");
    exit(1);
  }
  else
  {
    printf("Nice try :)");
  }
}

int GetFileLineCount(char *fileName)
{
  int count = 0;
  FILE *fp;
  char line[LINELEN];

  fp = fopen(fileName, "r");
  if (fp != NULL)
  {
    while(fgets(line, LINELEN, fp))
    {
      ++count;
    }
    fclose(fp);
  }
  return count;
}

void PrintStringReverseVideo(char *toPrint)
{
  printf("\033[7m");
  printf("%s", toPrint);
  printf("\033[m");
}

int CalcPercent(int numerator, int denominator)
{
  return (int)((float)numerator/(float)denominator * 100);
}
