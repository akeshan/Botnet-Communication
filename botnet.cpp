#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <ctime>
#include <iostream>
#include <string> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <csignal>

#include "Matrix.h"

using namespace std;

string globalSeed = "1234";


int randomFunctionGenerator(int currentTimeInMinutes, string seed) {
    srand(stoi(seed));
    int hash = currentTimeInMinutes + rand()%100; //s current time in minutes + random offset based on seed

    int offset = 'a' - 1;
    for(string::const_iterator it=seed.begin(); it!=seed.end(); ++it) {
        hash = hash << 1 ^ (*it - offset);
    }

    globalSeed = to_string(hash); // use current hash as the next seed

    return abs(hash) % 7; // mod the absolute value of the hash with the number of functions we have
}



#define LEN 1024


void randomString(char *buff, int len)
{
   static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        buff[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    buff[len] = 0;
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    printf("Caught segfault at address %p\n", si->si_addr);
    exit(0);
}



int main(int argc, char *argv[])
 {
    
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);

    srand (time(NULL));


    //network stuff
    int sockfd;
    struct sockaddr_in server;
    char buff[LEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(9034);

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));



    //create thread that handles synchronization stuff
    int pid2 = fork();
    if (pid2 == 0)
    {

        // shared memory stuff
        key_t key = 5678;
        int shmid;
        int *function;

        if ((shmid = shmget(key, sizeof(int), IPC_CREAT|0666)) < 0)
        {
            perror("shmget");
            exit(1);
        }

        function = (int *)shmat(shmid, NULL, 0);
    

        time_t t = time(0);   // get time now
        int minutes = ((t / 60) % 60); // current time in minutes
        bool firstRun = true;
        int runEvery = 1; // pick a new function randomly every X minutes
   

        
        for(;;)
        {
            int currentTimeInMinutes = (time(0)/60) % 60;
            if(minutes + runEvery == currentTimeInMinutes || firstRun == true) 
            {
                minutes = currentTimeInMinutes;
                *function = randomFunctionGenerator(currentTimeInMinutes, globalSeed);
                if (firstRun == true)
                    printf("Addr of function: %p\n", function);
            }    
            firstRun = false;
        }

    }
    

    //create thread that listens for incoming data
    int pid = fork();
    if (pid < 0 )
    {
        printf("Fork failed\n");
    }
    if (pid == 0) 
    {
        for (;;)
        {
            key_t key = 5678;
            int shmid = shmget(key, sizeof(int), 0666);
            int* function;
            function = (int *)shmat(shmid, NULL, 0666);

            bzero(buff, LEN);
            int a = recv(sockfd, buff, LEN, 0);
            printf("Recieved Message: %s ", buff);
            printf("using encoding scheme: %d \n", *function);
            
     
        }
    } 

    // write data to server
    for(;;) 
    {
        // shared memory stuff
        key_t key = 5678;
        int shmid = shmget(key, sizeof(int), 0666);
        int* function;
        function = (int *)shmat(shmid, NULL, 0666);
  //      printf("%d\n", *function);
        sleep(rand()%20 + 5);

        //this code is for writing data
      
        char msg[LEN];
        int n = rand()%10 + 1;
        randomString(msg, n);
  //      printf("Sending Message: %s using encoding scheme %d\n", msg, *function);
        send(sockfd, msg, strlen(msg), 0);
            
    }

    return 0;
 }






