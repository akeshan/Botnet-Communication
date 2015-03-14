#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <ctime>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <csignal>
#include <thread>
#include <mutex>
#include "Matrix.h"
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#define LEN 1024
#define RUNEVERY 1
using namespace std;

string globalSeed = "1234";
 int sockfd;
char buff[LEN];

// Locks fun to prevent race conditions.
mutex funmux;
int *fun;

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
void randomString(char *buff, int len)
{
   static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        buff[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    // len is significantly less than LEN so this will not cause a segfault unless len is 0
    buff[len] = 0;
}
void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    printf("Caught segfault at address %p\n", si->si_addr);
    exit(0);
}
void syncthread()
{
        time_t t = time(0);   // get time now
        int minutes = ((t / 60) % 60); // current time in minutes
        bool firstRun = true;
        
        for(;;)
        {
            int currentTimeInMinutes = (time(0)/60) % 60;
            if(minutes + RUNEVERY == currentTimeInMinutes || firstRun) 
            {
                minutes = currentTimeInMinutes;
		funmux.lock();
                *fun = randomFunctionGenerator(currentTimeInMinutes, globalSeed);
		funmux.unlock();
                if (firstRun == true)
                    printf("Addr of function: %p\n", fun);
            }    
            firstRun = false;
        }
}
void listenthread()
{
      for (;;)
        {
		int locfun;
		funmux.lock();
		locfun = *fun;
      		funmux.unlock();
            //bzero(buff, LEN);
		
            int a = recv(sockfd, buff, LEN, 0);
            printf("Recieved Message: %s ", buff);
            printf("using encoding scheme: %d \n", locfun);     
        }
}

void serverthread()
{
 for(;;) 
    {
        // shared memory stuff
        sleep(rand()%20 + 5);
        //this code is for writing data
        char msg[LEN];
        int n = rand()%10 + 1;
        randomString(msg, n);
        send(sockfd, msg, strlen(msg), 0);
    }
}
int main(int argc, char *argv[])
 {
    fun = new int;
    *fun = 0;
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

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(9034);

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    thread tsync(syncthread);
    thread tlisten(listenthread);
    thread tserver(serverthread);
// This just blocks main from exiting and killing the threads. The threads won't actually terminate on their own
    tsync.join();
    tlisten.join();
    tserver.join();
    return 0;
 }






