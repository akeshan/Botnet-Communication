#include <ctime>
#include <iostream>
#include <string> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

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

int main(int argc, char *argv[])
 {
    //since listener thread needs function
 //   int function = randomFunctionGenerator((time(0)/60)%60, globalSeed);

    srand (time(NULL));

    time_t t = time(0);   // get time now
    int minutes = ((t / 60) % 60); // current time in minutes
    bool firstRun = true;
    int runEvery = 1; // pick a new function randomly every X minutes

    //network stuff
    int sockfd;
    struct sockaddr_in server;
    char buff[LEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(9034);

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

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
           // printf("Got here");
            int currentTimeInMinutes = (time(0)/60) % 60;
            int function = randomFunctionGenerator(currentTimeInMinutes, globalSeed);
            bzero(buff, LEN);
            int a = recv(sockfd, buff, LEN, 0);
            printf("Received Message: %s using encoding scheme: %d \n", buff, function);
         //   puts(buff);
        }
    }

    // write data to server
    for(;;) 
    {
        
        int currentTimeInMinutes = (time(0)/60) % 60;

        if(minutes + runEvery == currentTimeInMinutes || firstRun == true) {
            minutes = currentTimeInMinutes;
            int function = randomFunctionGenerator(currentTimeInMinutes, globalSeed);


            char msg[LEN];
            int n = rand()%10 + 1;
            randomString(msg, n);
            printf("Sending Message: %s using encoding scheme %d\n", msg, function);
            send(sockfd, msg, strlen(msg), 0);
                
        } 
        firstRun = false;
    }

    return 0;
 }






