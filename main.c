/**
 * @Author: Brandon Baars <brandon>
 * @Date:   Saturday, June 3rd 2017, 2:04:24 pm
 * @Filename: main.c
 * @Last modified by:   brandon
 * @Last modified time: Wednesday, July 19th 2017, 7:54:55 pm
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
#include <ctype.h>

void printUsage();
void callRequiredFunction();
_Bool checkValidInput();
void openFile();
void searchFileinDirectory(char *directory, char *filename);

// TODO: Regular Expressions!


/* holds our file name, the option, and parameters */
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
            callRequiredFunction();
        } else
            printUsage();
      } else if (argc == 3) {
        callRequiredFunction();
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
  puts("[-c] Print only count of the lines\n[-i] iGnOre CaSe\n[-l] List only filenames of files containing a match\n[-n] Gives line number of pattern match\n[-v] Reverses search giving NOT containing string\n[-w] Only contain whole word matches\n[-x] Only contain whole line matches.\n[  ] Leave blank for no options");
}

/*
* Checks the input commands from the user and return true if it's       * valid, or false if it's invalid.
*/
_Bool checkValidInput() {

  /* checks the list of commands to see if one was a match,
    strcmp return 0 if they're a match so !strcmp returns true */
  if(!strcmp(option, "-i") || !strcmp(option, "-c") || !strcmp(option, "-l") || !strcmp(option, "-n") || !strcmp(option, "-v") || !strcmp(option, "-x") || !strcmp(option, "-w")) {
    return 1;
  }

  return 0;
}

void callRequiredFunction() {

  char *directory;
  char *ptr;
  int ch = '/';

  /* if there was a directory given */
  if ((ptr = strrchr(filename, ch)) != NULL){

    /* recursive to find last '/' in the string */
    ptr = strrchr(filename, ch);

    /* add 1 to get the filename */

    // TODO: The first character of PTR To see if its a *

    ptr += 1;

    /* allocate just enough memory to store our directory */
    directory = (char *)malloc(strlen(filename) - strlen(ptr));

    /* copy the directory over to our newly allocated memory  */
    strncpy(directory, filename, strlen(filename) - strlen(ptr));

  } else {

    /* if there was no path given, then just the file name, our directory is set to our default of './' */
    directory = (char *)malloc(strlen("./"));
    strncpy(directory, "./", strlen("./"));
  }

  if(!strcmp(option, "-c")) {
    // PRINT ONLY COUNT OF LINES
    searchFileinDirectory(directory, filename);

  } else if (!strcmp(option, "-i")) {
    // IGNORE CASE
    for (int i = 0 ; i < strlen(parameter) ; i++) {
      parameter[i] = tolower(parameter[i]);
    }

    searchFileinDirectory(directory, filename);

  } else if(!strcmp(option, "-l")) {
    //LIST ONLY FILE NAME
    searchFileinDirectory(directory, filename);

  }else if(!strcmp(option, "-n")) {
    //GIVES LINE NUMBER OF PATTER MATCH
    searchFileinDirectory(directory, filename);

  } else if(!strcmp(option, "-v")) {
    //REVERSE SEARCH
    searchFileinDirectory(directory, filename);

  } else if(!strcmp(option, "-w")) {
    // SELECT WHOLE WORD MATCHES ONLY
    // TOKEN BY SPACE DO STRCMP

  } else if(!strcmp(option, "-x")) {
    //WHOLE LINE MATCHES ONLY
    searchFileinDirectory(directory, filename);

  } else {
    //NO OPTIONS WERE PASSED
    searchFileinDirectory(directory, filename);
  }
}


void searchFileinDirectory(char *directory, char *file) {

  char line[512];
  char directoryFiles[512];
  DIR *d = opendir(directory);
  struct dirent* currententry;
  int lineNum = 0;
  _Bool isFile = 0;
  char *lineCpy;

  if (d == NULL) {
    printf("Not a directory.\n");
    return;
  }

  while((currententry = readdir(d)) != NULL){
    strcpy(directoryFiles, directory);
    directoryFiles[strlen(directoryFiles) +1 ] = '\0';
    directoryFiles[strlen(directoryFiles)] = '/';
    strcat(directoryFiles,currententry->d_name);
  }

  FILE *f = fopen(file, "r");

  int count = 0;

  if(f == NULL) {
    printf("File does not exist.\n");
  } else {

    FILE *outfile = fopen("output.txt", "w");

    /* while the end of file has not been reached */
    while (fgets(line, 1024, f)) {

      //printf("%s\n", line);
      fprintf(outfile, "%d:\t%s\n", lineNum, line);

    //  printf("%s\n", line);
      //
      if(!strcmp(option, "-i")) {
        for (int i = 0 ; i < strlen(line) ; i++) {
            line[i] = tolower(line[i]);
        }
      }

      if(strstr(line, parameter)) {

        lineCpy = line;

        if (!strcmp(option, "-c")) {
           count++;


        }  else if (!strcmp(option, "-i")) {
            printf("%s\n",lineCpy);

          //WHOLE LINE MATCHES ONLY
        } else if(!strcmp(option, "-x")) {

          /* must add terminating character to check if they're equal since the line ends with \0, so checking for whole line will be 'apples\n' != 'apples'; */
          if(!strcmp(line, strcat(parameter, "\n")))
            printf("%s",line);
        }
          /* LIST ONLY FILE NAME */
        else if(!strcmp(option, "-l")) {
          isFile = 1;

        /* GIVES LINE NUMBER OF PATTER MATCH */
        } else if(!strcmp(option, "-n")) {
          printf("%d\t%s\n",lineNum, line);

        /* no options were passed and were not looking for reversing string */
        } else if (strcmp(option, "-v") != 0) {
            printf("%s\n",line);
            count++;
        }
    } else if (!strcmp(option, "-v") && strstr(line, parameter) == NULL) {
        printf("%s\n", line);
      }

      ++lineNum;
    }

    if (isFile)
      printf("%s\n", filename);

    if (!strcmp(option, "-c")) {
          printf("%d\n",count);
      }

    fclose(outfile);
  }

  fclose(f);
}
