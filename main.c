/**
 * @Author: Brandon Baars <brandon>
 * @Date:   Saturday, June 3rd 2017, 2:04:24 pm
 * @Filename: main.c
 * @Last modified by:   brandon
 * @Last modified time: Monday, June 5th 2017, 4:19:49 pm
 *
 * CIS 361 Final Project
 * GREP Simulator using c in a UNIX Environment
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void printUsage();
_Bool checkValidInput(char * option);

int main(int argc, char const *argv[]) {

  char *logname = getenv("LOGNAME");
  char filename[256], line[1024], terminal[100], cwd[512], command[7], option[10], parameter[512];
  getcwd(cwd, 512);
  int lineNum = 1;

  char terminateCmd[] = ":x";

  DIR *d = opendir(cwd);
  struct dirent* currententry;

  strcpy(command, "");

/* simulates command line while the user doesn't enter ':x' to terminate */
  while(1) {

    /* simulates a terminal by displaying the device and logname */
    printf("Brandons-MacBook-Pro:~ %s$ ", logname);
    fgets(terminal, 100, stdin);

    /* parses the user input into the variables */
    sscanf(terminal, "%s %s %s %s", command, option, parameter, filename);

    /* lists the users PWD */
    if (!strcmp(command, "pwd"))
        printf("%s\n", getcwd(cwd, 512));


      // TODO: IF ALL INPUT IS VALID, call corresponding functions

    /* if the command is grep */
    else if (!strcmp(command, "grep")) {

      /* checks for a valid command */
      if(checkValidInput(option)) {

          /* checks for next inputs to contain items */
          if(strcmp(parameter, "") != 0 && strcmp(filename, "") != 0) {
            printf("VALID GREP\n");
          } else {
            printUsage();
          }
      /* if no commands, checks that it contains items */
      } else {
          /* if a command was left out and insteat the parameters were left in, then the command variable now becomes the parameters and the file name takes the place of parameters, ONLY if the filename entered is empty, if there are 4 inputs and no valid command then it is an invalid input

          grep [-options] [parameters] [filename]

          example: grep file file file // invalid, too many arguements
                  grep parameter filename
          */
          if(strcmp(parameter, "") != 0 && strcmp(filename, "") == 0) {
            printf("VALID GREP\n");
            char temp[512];
            strcpy(temp, parameter);
            strcpy(parameter, option);
            strcpy(filename, temp);
          } else {
            printUsage();
          }
      }

    /* exit command of program */
    } else if (!strcmp(command, ":x")) {
        return 0;
    }

    else {
      printf("Sorry, '%s' not recognized\n", command);
    }

    /* flushing input buffer */
    fflush(stdin);

    /* resets variables to empty after each cycle this way they don't get copied over the next time around */
    strcpy(option, "");
    strcpy(parameter, "");
    strcpy(filename, "");

  }

    return 0;
}

/*
* Prints the grep command and the options with their descriptions to   * the user whenever invalid input has been entered.
*/
void printUsage() {
  puts("usage: grep [-options] [contentToSearchFor] [fileToSearchIn]");
  puts("[-c] Print only count of the lines\n[-i] iGnOre CaSe\n[-l] List only filenames of files containing a match\n[-n] Gives line number of pattern match\n[-v] Reverses search giving NOT containing string\n[-w] Only contain whole word matches\n[-x] Only contain whole line matches.");
}

/*
* Checks the input commands from the user and return true if it's       * valid, or false if it's invalid. 
*/
_Bool checkValidInput(char * option) {

  if(!strcmp(option, "-i") || !strcmp(option, "-c") || !strcmp(option, "-l") || !strcmp(option, "-n") || !strcmp(option, "-v") || !strcmp(option, "-x") || !strcmp(option, "-w")) {
    return 1;
  }

  return 0;
}

  // while((currententry=readdir(d))!=NULL){
  //   char filename[1024];
  //   strcpy(filename, cwd);
  //   filename[strlen(filename)+1]='\0';
  //   filename[strlen(filename)]='/';
  //   strcat(filename,currententry->d_name);
  //   printf("%s \n", currententry->d_name);
  // }

  // FILE *f = fopen(filename, "r");
  //
  // if(f == NULL) {
  //   printf("We got an error");
  //   return 1;
  // }
  //
  // fgets(line, 1024, f);
  //
  // FILE *outfile = fopen("output.txt", "w");
  //
  // while (!feof(f)) {
  //   fprintf(outfile, "%d:\t%s", lineNum, line);
  //   fgets(line, 1024, f);
  //   ++lineNum;
  // }
  //
  // fclose(outfile);
  // fclose(f);
