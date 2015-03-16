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
#include <string>
#define LEN 1024
#define RUNEVERY 10
using namespace std;

string globalSeed = "1234";
 int sockfd;
char buff[LEN];
int sendFreq;

// Locks fun to prevent race conditions.
mutex funmux;



struct randomTuple
{
    int one;
    int two;
    char id[3];
};

randomTuple *fun;


int randomFunctionGenerator(int currentTimeInMinutes, string seed, int max) {
    srand(stoi(seed));
    int hash = currentTimeInMinutes + rand()%100; //s current time in minutes + random offset based on seed
    int offset = 'a' - 1;
    for(string::const_iterator it=seed.begin(); it!=seed.end(); ++it) {
        hash = hash << 1 ^ (*it - offset);
    }
    globalSeed = to_string(hash); // use current hash as the next seed
    
    

    return abs(hash) % max; // mod the absolute value of the hash with the number of functions we have

}


void randomString(char *buff, int len)
{
   static const char alphanum[] =
        "0123456789"
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
       // int minutes = ((t / 60) % 60); // current time in minutes
        int seconds = t;
        bool firstRun = true;
        
        for(;;)
        {
          //  int currentTimeInMinutes = (time(0)/60) % 60;
            int currentTimeInSeconds = time(0);
            if(seconds + RUNEVERY == currentTimeInSeconds || firstRun) 
            {
               // minutes = currentTimeInMinutes;
                seconds = currentTimeInSeconds;
		        funmux.lock();
                fun->one = randomFunctionGenerator(currentTimeInSeconds, globalSeed, 12);
                fun->two = randomFunctionGenerator(currentTimeInSeconds, globalSeed, 10);
		        randomString(fun->id, 3);
                printf("Updating random numbers: %d %d. Identifier string: %s \n", fun->one, fun->two, fun->id);
                funmux.unlock();
            
            }    
            firstRun = false;
        }
}


void listenthread()
{
      for (;;)
        {
		randomTuple locfun;
		funmux.lock();
		locfun = *fun;
      	funmux.unlock();
        bzero(buff, LEN);
		
        int a = recv(sockfd, buff, LEN, 0);
        
        //buff is now of form 32;34;56;
        string toDec = buff;
       
        
        int message[3] = {0, 0, 0};

        int message_index = 0;

        string temp = "";
        for(int i = 0; i < toDec.length(); i++)
        {
            
            
            if (toDec[i] == ';')
            {
            //    cout << temp << endl;
                message[message_index] = stoi(temp);
                temp = "";
                message_index++;
            }
            else
            {
                if (toDec[i] == '-') 
                {
                    temp += toDec[i];
                }
                else
                {
                    temp += to_string(toDec[i] - '0');
                }
            }
        }



        Decode d(message, 2, 2);
        
        char result[3] = {' ',' ',' '};
        d.decoding(result); 

        string res = result;
   //     cout << "length: " << res.length() << endl;
        res.substr(0, 3);

        string id = locfun.id;
        if (res == id)
        {
            printf("Recieved Message: %s decoded to => %s using encoding scheme: %d %d Botnet detected!\n ", buff, res.c_str(), locfun.one, locfun.two);
        }
        else
        {
            printf("Recieved Message: %s decoded to => %s using encoding scheme: %d %d\n ", buff, res.c_str(), locfun.one, locfun.two);     
        }
    }
} 

void serverthread()
{
    int count = 0;
 for(;;) 
    {
       
        sleep(sendFreq);
       
        //this code is for writing data

        randomTuple locfun;
        funmux.lock();
        locfun = *fun;
        funmux.unlock();

        char msg[LEN];
     
        if (count == 4)
        {
            // send encoded id
            Encode e(locfun.id, 2, 2);
            int result[3] = {0,0,0};
            e.encoding(result);
            string toSend = to_string(result[0]) + ";" + to_string(result[1]) + ";" + to_string(result[2]) + ";";
            send(sockfd, toSend.c_str(), toSend.length(), 0);
            printf("BOTNET: Sending Message %s encoded to => %s using encoding scheme: %d %d \n", msg, toSend.c_str(), locfun.one, locfun.two );
        }
        else
        {
            // send normal message
            printf("NORMAL: Sending message hii \n");
            const char *m = "hii";
            strcpy(msg, m);
            send(sockfd, msg, strlen(msg), 0);
        }
      
        count = (count + 1) % 5;
    }
}


int main(int argc, char *argv[])
 {
    
    sendFreq = atoi(argv[1]);

    fun = new randomTuple;
    fun->one = 0;
    fun->two = 0;
   
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_SIGINFO;
	
    sigaction(SIGSEGV, &sa, NULL);

    srand (time(NULL));

    //network stuff
  //  int sockfd;
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






