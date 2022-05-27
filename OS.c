#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define MAX 100
int  new_pid = 0;
 
/*
A function to write in a log file
when a child process is terminated.
*/
void handle(){
    FILE* file;
    file = fopen("log.txt", "a");
    fprintf(file, "Child process %d is terminated\n",new_pid);
    fclose(file);
}
 
int main(){
    int f1 = 1, f2, pid;
    char str[MAX], temp[MAX], path[MAX];
    char* token;
    int result = 0, i;
    while(f1){
        char* array[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
        i = 0;
        f2 = 0;
        //To get the current directory, we use the function getcwd().
        getcwd(path, MAX);
        printf(":~%s$ ", path);
        //We use the function fgets() to take the command from user as string.
        fgets(str, MAX, stdin);
        //Removing "/n" from the end of each string.
        if(str[strlen(str)-1] == '\n'){
            str[strlen(str)-1] = '\0';
        }
        if(strlen(str) == 0 ){
            printf("This command is not found/n");
            continue;
        }
        /*
        We dividing the string according to spaces if they are found.
        We put each token in a different index in an array[].
        */
        strcpy(temp, str);
        token = strtok(temp, " ");
        while(token != NULL){
        array[i] = token;
        token = strtok(NULL, " ");
        i++;
        }
        //The user enter the command "exit" to terminate the shell.
        if(strcmp(str, "exit") == 0){
            exit(0);
        }
        //The user enter the command "cd" to change the directory.
        if(strcmp(array[0], "cd") == 0){
            result = chdir(array[1]);
            if(result != 0){
                printf("There is no directory\n");
            }
            continue;
        }
        //Removing "&" from the end of each string.
        if(strcmp(array[i-1],"&") == 0){
            array[i-1] = NULL;
            f2 = 1; //A flag to check if the string has "&" at its end.. ex.(firefox &).
        }
        //Forking a child process.
        pid = fork();
        new_pid = pid;
        if(pid < 0){
            printf("Creation of a child process was unsuccessful.\n");
            continue;
        }
        //Returned to the newly created child process.
        else if(pid == 0){
            execvp(array[0],array);
        }
        else{
            signal (SIGCHLD, handle);
            if(f2 == 0){   //In case a user wants to execute the command in background, then f2 == 1.
                wait(&pid); //Waiting for it to complete and synchronous execution.
            }
        }
    }
    return 0;
}