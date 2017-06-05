/**
 * @Author: Brandon Baars <brandon>
 * @Date:   Saturday, June 3rd 2017, 2:04:24 pm
 * @Filename: main.c
 * @Last modified by:   brandon
 * @Last modified time: Sunday, June 4th 2017, 10:40:44 pm
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


int main(int argc, char const *argv[]) {
  /* code */

  char *logname = getenv("LOGNAME");
  char filename[256], line[1024], terminal[100], cwd[512], command[7], option[10], parameter[512];
  getcwd(cwd, 512);
  int lineNum = 1;

  char terminateCmd[] = ":x";

  DIR *d = opendir(cwd);
  struct dirent* currententry;

  strcpy(command, "");

/* simulates command line while the user doesn't enter ':x' to terminate */
  while(strcmp(command, terminateCmd) != 0) {

    printf("Brandons-MacBook-Pro:~ %s$ ", logname);
    fgets(terminal, 100, stdin);

    /* parses the user input into the variables */
    sscanf(terminal, "%s %s %s %s", command, option, parameter, filename);

    /* lists the users PWD */
    if (!strcmp(command, "pwd"))
        printf("%s\n", getcwd(cwd, 512));

    /* if input command is not grep */
    else if (strcmp(command, "grep") != 0) // >0 or <0 if false,  ==0 if true
      printf("Sorry '%s' not recognized \n", command);

      /*
      * TODO: MAKE SURE WHEN USER ENTERS OPTIONS, THERE ARE THE  *  REST OF THE PARAMETERS
      */

    /* if the [-options] have been left out */
    else if (strcmp(option, "-i") != 0) {
      printf("Sorry, options not recognized\n");

      char temp[512];
      strcpy(temp, parameter);
      strcpy(parameter, option);
      strcpy(filename, temp);
    }

    printf("%s\n", parameter);
    printf("%s\n", filename);

    fflush(stdin);

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
