/**
 * @Author: Brandon Baars <brandon>
 * @Date:   Saturday, June 3rd 2017, 2:04:24 pm
 * @Filename: main.c
 * @Last modified by:   brandon
 * @Last modified time: Monday, July 3rd 2017, 5:26:37 pm
 *
 * CIS 361 Final Project
 * GREP Simulator using c in a UNIX Environment
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void printUsage();
void callRequiredFunction(char * option);
_Bool checkValidInput(char * option);
void openFile();


char filename[256], option[10], parameter[512];

int main(int argc, char const *argv[]) {

  if (argc == 3 || argc == 4) {

      if (argc == 4) {
        strncpy(option, argv[1], sizeof(option));
        strncpy(parameter, argv[2], sizeof(parameter));
        strncpy(filename, argv[3], sizeof(filename));
      } else {
        strncpy(parameter, argv[1], sizeof(parameter));
        strncpy(filename, argv[2], sizeof(filename));
      }

      /* checks for a valid command */
      if(checkValidInput(option)) {
        /* checks for next inputs to contain items */
        if(argc == 4) {
            printf("VALID GREP\n");
            callRequiredFunction(option);
        } else
            printUsage();
      } else if (argc == 3) {
        printf("VALID GREP\n");
        callRequiredFunction(option);
      } else {
        printUsage();
      }
    }

    else {
      printUsage();
    }

    return 0;
}

/*
* Prints the grep command and the options with their descriptions to   * the user whenever invalid input has been entered.
*/
void printUsage() {
  puts("usage: ./grep [-options] [contentToSearchFor] [fileToSearchIn]");
  puts("[-c] Print only count of the lines\n[-i] iGnOre CaSe\n[-l] List only filenames of files containing a match\n[-n] Gives line number of pattern match\n[-v] Reverses search giving NOT containing string\n[-w] Only contain whole word matches\n[-x] Only contain whole line matches.");
}

/*
* Checks the input commands from the user and return true if it's       * valid, or false if it's invalid.
*/
_Bool checkValidInput(char * option) {

  /* checks the list of commands to see if one was a match,
    strcmp return 0 if they're a match so !strcmp returns true */
  if(!strcmp(option, "-i") || !strcmp(option, "-c") || !strcmp(option, "-l") || !strcmp(option, "-n") || !strcmp(option, "-v") || !strcmp(option, "-x") || !strcmp(option, "-w")) {
    return 1;
  }

  return 0;
}

void callRequiredFunction(char * option) {

  if(!strcmp(option, "-c")) {
    // PRINT ONLY COUNT OF LINES

  } else if (!strcmp(option, "-i")) {
    // IGNORE CASE

  } else if(!strcmp(option, "-l")) {

    //LIST ONLY FILE NAME
  }else if(!strcmp(option, "-n")) {
    //GIVES LINE NUMBER OF PATTER MATCH

  } else if(!strcmp(option, "-v")) {
    //REVERSE SEARCH

  } else if(!strcmp(option, "-w")) {
    //SELECT WHOLE WORD MATCHES ONLY

  } else if(!strcmp(option, "-x")) {
    //WHOLE LINE MATCHES ONLY

  } else {
    //NO OPTIONS WERE PASSED
    openFile();

  }
}


void openFile() {

  char cwd[512];
  char line[512];
  char directoryFiles[512];
  getcwd(cwd, 512);
  DIR *d = opendir(cwd);
  struct dirent* currententry;
  int lineNum = 0;

    while((currententry=readdir(d))!=NULL){
      strcpy(directoryFiles, cwd);
      directoryFiles[strlen(directoryFiles)+1]='\0';
      directoryFiles[strlen(directoryFiles)]='/';
      strcat(directoryFiles,currententry->d_name);
      printf("%s \n", currententry->d_name);
    }

    FILE *f = fopen(filename, "r");

    if(f == NULL) {
      printf("We got an error");
    } else {

      fgets(line, 1024, f);

      FILE *outfile = fopen("output.txt", "w");

      while (!feof(f)) {
        fprintf(outfile, "%d:\t%s\n", lineNum, line);
        fgets(line, 1024, f);
        ++lineNum;
      }

      fclose(outfile);
    }
    fclose(f);
}
