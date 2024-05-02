 1 //Author: Kaevin Barta
  2 //date: 3/4/2024
  3 //version: .1
  4 //file: forkfun.c
  5 //This program creates several child processes (up to 4), does a little bit of IPC (Interprocess communication) via signals, and properly processes the child exit values.
  6
  7 #include <stdio.h>
  8 #include <unistd.h>
  9 #include <signal.h>
 10 #include <stdlib.h>
 11 #include <sys/wait.h>
 12
 13 pid_t parentPid;//for the SIGUSR1
 14 int signals = 0; //SIGUSR1 uses it
 15 int numProcesses; //SIGTERM uses it
 16 pid_t childPids[4]; //SIGTERMS uses it
 17
 18 //handles signals for SIGUSR1
 19 void sigusr1Handler(int sig);
 20 //handles signals for SIGTERM
 21 void sigtermHandler(int sig);
 22 //handles the child routine
 23 void childRoutine();
 24
 25 int main(int argc, char* argv[]) {
 26     numProcesses = atoi(argv[1]);
 27     int MAX_CHILDREN = 4;
 28     //keep in bounds
 29     if (numProcesses < 1)
 30         numProcesses = 1;
 31     if (numProcesses > MAX_CHILDREN)
 32         numProcesses  = MAX_CHILDREN;
 33     //for SIGUSR1 function
 34     parentPid = getpid();
 35     //Output parent pid and info about child pids
 36     printf("PARENT: process id is %d\n", parentPid);
 37     printf("PARENT: forking %d processes\n", numProcesses);
 38     //fork child prosses
 39     for(int i = 0; i < numProcesses; i++){
 40         pid_t childPid = fork();
 41         childPids[i] = childPid;//fill up the childPids array
 42         if (childPid == 0){//child routine
 43             childRoutine();
 44         } else if (childPid < 0){//error prosses
 45             printf("Unexpected Error with Child Pid!\n");
 46             return -1;
 47         }
 48     }
 49     signal(SIGUSR1, sigusr1Handler);//SIGUSR1 handler for the parent
 50     signal(SIGTERM, sigtermHandler);//SIGTERM handler
 51     printf("PARENT: waiting for child to end.\n");
 52     fflush(stdout);//instant output
 53
 54     //wait for each child prossess to end
 55     int status;
 56     int totalExitValues = 0;
 57     for(int i = 0; i < numProcesses; i++){
 58         waitpid(childPids[i], &status,0);
 59         totalExitValues += WEXITSTATUS(status);
 60     }
 61     printf("PARENT: Sum of signals from all children: %d\n", totalExitValues);
 62
 63     return 0;
 64 }
 65
 66 void sigusr1Handler(int sig){
 67     pid_t currentPid = getpid();
 68     if (currentPid == parentPid){
 69         printf("PARENT: SIGUSR1 signal received!\n");
 70         for (int i = 0; i < numProcesses; i++) {
 71             kill(childPids[i], SIGUSR1);
 72             signals++;//to account for child
 73         }
 74     }else{
 75         printf("CHILD: SIGUSR1 signal received!\n");
 76     }
 77     //increment counter
 78     signals++;
 79 }
 80
 81 void sigtermHandler(int sig){
 82     printf("SIGTERM signal recived!\n");
 83     for(int i = 0; i < numProcesses; i++){
 84         kill(childPids[i], SIGKILL);
 85         printf("child (%d) was reaped\n", childPids[i]);
 86     }
 87     //parent kills itself :(
 88     raise(SIGKILL);
 89 }
 90
 91 void childRoutine(){
 92     //Sends user the pid for child
 93     printf("CHILD %d is alive!\n", getpid());
 94     fflush(stdout);
 95     //SIGUSR1 handler
 96     signal(SIGUSR1, sigusr1Handler);
 97     //sleep for 30-60 sec
 98     int sleepTime = 30 + rand() % 30;
 99     int remaining = sleep(sleepTime);
100     while(remaining > 0){
101         remaining = sleep(remaining);
102     }
103     //return the number of signals recived(this is the child so parent is still running sepratley)
104     exit(signals);
105 }
106

