/**
 * @Author: Brandon Baars <brandon>
 * @Date:   Saturday, June 3rd 2017, 2:04:24 pm
 * @Filename: main.c
 * @Last modified by:   brandon
 * @Last modified time: Monday, July 24th 2017, 1:51:26 pm
 *
 * CIS 361 Final Project
 * GREP Simulator using C in a UNIX Environment
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
void searchFileinDirectory(char *filepath, char *file);

// TODO: Regular Expressions! RECOMP AND REGEX

/* holds our file name, the option, and parameters */
char filename[256], option[10], parameter[512], fileArray[100][100];

/* whether or not an array of files is searched for */
_Bool isArray = 0;

int main(int argc, char const *argv[]) {

  int count = 0;

  for (int i = 0 ; i < argc ; i++) {
    if ((i > 1 && strrchr(argv[1],'-') == NULL) || i > 2) {
      strcpy(fileArray[count],argv[i]);
      count++;
    }
  }

/* if the user has added the right amount of arguements */
  if (argc >= 3) {

      /* if it's exactly 4 theres an option passed in */
      if (argc == 4 ) {
        strncpy(option, argv[1], sizeof(option));
        strncpy(parameter, argv[2], sizeof(parameter));
        strncpy(filename, argv[3], sizeof(filename));
      }

      /* checks for a valid command */
      if(checkValidInput(option)) {
        /* checks for next inputs to contain items */
        if(argc == 4) {
            callRequiredFunction();

        /* if greater than 4, there was array of files */
        } else if (argc > 4) {
          strncpy(option, argv[1], sizeof(option));
          strncpy(parameter, argv[2], sizeof(parameter));
          isArray = 1;

          for (int i = 0 ; i < count ; i++){
            strncpy(filename, fileArray[i], sizeof(filename));
            callRequiredFunction();
            printf("\n");
          }
        }else
            printUsage();
      /* if only 3, there was no array of files and no options */
      } else if (argc == 3) {
        callRequiredFunction();

      /* there was an array of files and maybe an option */
      } else {
        if (strrchr(argv[1],'-') != NULL) {
          strncpy(option, argv[1], sizeof(option));
          strncpy(parameter, argv[2], sizeof(parameter));
        } else {
          strncpy(parameter, argv[1], sizeof(parameter));
        }
        isArray = 1;
        for (int i = 0 ; i < count ; i++){
          strncpy(filename, fileArray[i], sizeof(filename));
          callRequiredFunction();
          printf("\n");
        }
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

  if (!strcmp(option, "-i")) {
    // IGNORE CASE
    for (int i = 0 ; i < strlen(parameter) ; i++) {
      parameter[i] = tolower(parameter[i]);
    }
    searchFileinDirectory(filename, ptr);
  } else {
    searchFileinDirectory(filename, ptr);
  }
}


void searchFileinDirectory(char *filepath, char *file) {

  char line[512];
  int lineNum = 0;
  _Bool isFile = 0;
  char lineCpy[512];
  int count = 0;

  //printf("Opening File: %s\n", filepath);

  FILE *f = fopen(filepath, "r");

  if(f == NULL) {
    printf("File %s does not exist.\n", filepath);
  } else {

    FILE *outfile = fopen("output.txt", "w");

    /* while the end of file has not been reached */
    while (fgets(line, 1024, f)) {

      fprintf(outfile, "%d:\t%s\n", lineNum, line);

      strcpy(lineCpy, line);

      if(!strcmp(option, "-i")) {
        for (int i = 0 ; i < strlen(line) ; i++) {
            line[i] = tolower(line[i]);
        }
      }

      if(strstr(line, parameter)) {

        if (((isArray && strcmp(option, "-l") != 0) && count < 1 && strcmp(option, "-v")) && strcmp(option, "-w")) {
          printf("%s: ", file);
        }

        if (!strcmp(option, "-c")) {
           count++;

        }  else if (!strcmp(option, "-i")) {
            printf("%s",lineCpy);

          //WHOLE LINE MATCHES ONLY
        } else if(!strcmp(option, "-x")) {

          char tempParameter[strlen(parameter) + 2];

          strcpy(tempParameter, parameter);

          count++;
          /* must add terminating character to check if they're equal since the line ends with \0, so checking for whole line will be 'apples\n' != 'apples'; */
          if(!strcmp(line, strcat(tempParameter, "\n")))
            printf("%s",line);

        } else if(!strcmp(option, "-w")) {

            char *ptr;
            ptr = strtok(lineCpy, " ,.()//{}-|!@#$^&*<>?");

            char tempParameter[strlen(parameter) + 2];

            strcpy(tempParameter, parameter);

            count++;
            /* must add terminating character to check if they're equal since the line ends with \0, so checking for whole line will be 'apples\n' != 'apples'; */
            if(!strcmp(line, strcat(tempParameter, "\n"))) {
              printf("%s: %s",file, line);
            }

            while(ptr != NULL) {
              if(!strcmp(ptr, parameter)){
                if(isArray)
                  printf("%s: ", file);
                printf("%s\n", line);
                break;
              }
              ptr = strtok(NULL, " ,.()//{}-|!@#$^&*<>?");
            }

          /* LIST ONLY FILE NAME */
        } else if(!strcmp(option, "-l")) {
          printf("%s", file);
          return;

        /* GIVES LINE NUMBER OF PATTER MATCH */
        } else if(!strcmp(option, "-n")) {
          printf("%d\t%s",lineNum, line);

        /* no options were passed and were not looking for reversing string */
        } else if (strcmp(option, "-v") != 0) {
            printf("%s",line);
        }
    } else if (!strcmp(option, "-v") && strstr(line, parameter) == NULL) {

        if(isArray && line[0] != '\n')
          printf("%s ", file);

        printf("%s", line);
      }

      ++lineNum;
    }

    if (isFile)
      printf("%s\n", file);

    if (!strcmp(option, "-c")) {
          printf("%d",count);
      }

    fclose(outfile);
  }
  fclose(f);
}
