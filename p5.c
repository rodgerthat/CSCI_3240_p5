/*
 * =====================================================================================
 *
 *         Author:  Norris, Joel R. 
 *          Class:  CSCI_3240
 *     Instructor:  RButler
 *
 *       Filename:  p5.c
 *
 *    Description:  Process fork, invoke bc and capture / return output
 *
 *        Version:  1.0
 *        Created:  11/07/2017 11:36:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // dup


int main( int argc, char **argv ) {

    alarm(60);

    int pID, i;
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read_in;
    
    int stdin_pipe_fds[2], 
        stdout_pipe_fds[2], 
        stderr_pipe_fds[2];     // int arrays holds the ints for all the fds for pipes
    char buf[100];              // somewhere to store the msg...

    
    // get some command~line args, for testing
    // what's a pirate's favorite way to pass paramaters to a command?
    // arrrrrrrgs!
    printf("==========================\n");
    printf("argc : %d \n", argc);
    printf("argv[0] : %s \n", argv[0]);
    printf("argv[1] : %s \n", argv[1]); 
    printf("argv[2] : %s \n", argv[2]); 
    printf("argv[3] : %s \n", argv[3]); 
    printf("argv[4] : %s \n", argv[4]); 
    printf("==========================\n");

    // setup pipes with file descriptors
    pipe(stdin_pipe_fds);
    pipe(stdout_pipe_fds);
    pipe(stderr_pipe_fds);

    // fork the process.
    pID = fork();

    if (pID == -1) { printf("fork failed\n"); exit(-1); }

    // parent process has actual pID
    if ( pID > 0 ) {

        printf("pID : %d : I'm the parent\n", pID);

        close(stdin_pipe_fds[0]);
        close(stdout_pipe_fds[1]);
        close(stderr_pipe_fds[1]);

        // original process reads lines of input from file
        // name as the first command~line arg 
        
        stream = fopen(argv[1], "r");
        if (stream == NULL) exit(EXIT_FAILURE);

        while ((read_in = getline(&line, &len, stream)) != -1 ) {

            //printf("retreived line of length %zu : \n", read_in);
            //printf("%s", line);

            // hand each line to bc process on stdin via pipe
            i = write(stdin_pipe_fds[1], line, 100);
            printf("parent stdin_pipe_fds[1] : %s\n", line);

            // print bc output
        }

        // clean up after yo sef.
        fclose(stream);
        if (line) { free(line); }
        

    } else {

        // child process has pID of 0

        // new process (child) executes bc, passes command~line ags thru to bc
        printf("pID : %d : I'm the child, waaaaaaa!\n", pID);

        close(stdin_pipe_fds[1]);   // close normal stdout
        close(stdout_pipe_fds[0]);  
        close(stderr_pipe_fds[0]);

        close(0);

        dup(stdin_pipe_fds[0]);     // make stdin same as stdin_pipe_fds[0]
        close(stdin_pipe_fds[0]);
        close(1);

        dup(stdout_pipe_fds[1]);    // make stdout same as stdout_pipe_fds[1]
        close(stdout_pipe_fds[1]);
        close(2);

        dup(stderr_pipe_fds[1]);
        close(stderr_pipe_fds[1]);

        // read in from parent on stdin
        i = read(stdout_pipe_fds[0], line, 100);

        printf("child stdout_pipe_fds[0] : %s\n", line);

        // pass parent output to bc on child stdin
        
        // see what's in the pipe
        


    }
    

    


    return 0;

}

