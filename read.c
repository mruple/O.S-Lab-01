// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: Mitchell Ruple Date: September 23 2022

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN		16
#define MAX_LINE_LEN	80
#define WHITESPACE		" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (true) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      if(strcmp(command.argv[0],"C") == 0) {
         command.name = "cp"; //works "C (filesource)(filetarget)"
      }
      else if(strcmp(command.argv[0], "D") == 0) {
         command.name = "rm"; //works
      }
      else if(strcmp(command.argv[0], "M") == 0) {
         command.name = "nano"; //works
      }
      else if(strcmp(command.argv[0], "P") == 0) {
         command.name = "more"; //works
      }
      else if(strcmp(command.argv[0], "W") == 0) {
         command.name = "clear"; //works
      }
      else if(strcmp(command.argv[0], "X") == 0) {
         command.name = command.argv[1]; // works
      }
      else if(strcmp(command.argv[0], "E") == 0) {
         command.name = "echo"; // works
      }
      else if(strcmp(command.argv[0], "Q") == 0) {
         wait(NULL);
         break; // works
      }
      else if(strcmp(command.argv[0], "H") == 0) {
         printf("\n\n\n");
         printf("My shell supports the following commands:");
         printf("\n\n\n\n");
         printf("C file1 file2 : Copy; create file2, copy all bytes of file1 to file2 without"); 
         printf(" deleting file1.");
         printf("\n\n");
         printf("D file : Delete the named file.");
         printf("\n\n");
         printf("E comment : Echo; display comment on screen");
         printf("\n\n");
         printf("H : Help; display the user manual.");
         printf("\n\n");
         printf("L : List the contents of the current directory.");
         printf("\n\n");
         printf("M file : Make; create the named text file by launching a text editor.");
         printf("\n\n");
         printf("P file : Print; display the contents of the named file on screen.");
         printf("\n\n");
         printf("Q : Quit the shell.");
         printf("\n\n");
         printf("S : Surf the web by launching FireFox as a background process.");
         printf("\n\n");
         printf("W : Wipe; clear the screen.");
         printf("\n\n");
         printf("X program : Execute the named program.");
         printf("\n\n\n\n");
         continue; //works
      }
      else if(strcmp(command.argv[0], "L") == 0) {
         if((pid = fork()) == 0) {
            printf("\n");
            command.name = "pwd";
            execvp(command.name, command.argv);
         }
         wait(NULL); //Wait for child to process
         //Do it again for ls
         if((pid = fork()) == 0) {
            printf("\n");
            command.name = "ls";
            execvp(command.name, command.argv);
         }
         wait(NULL); //Wait for child to complete

         continue;
         
      }
	  
      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      /* Wait for the child to terminate */
      wait(&status); /* EDIT THIS LINE */
   }

   /* Shell termination */
   printf("\n\nshell: Terminating successfully\n\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char* user = getenv("USER");
   printf("\n\n");
   printf("linux(");
   printf("%s",user);
   printf(")|>");

}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */