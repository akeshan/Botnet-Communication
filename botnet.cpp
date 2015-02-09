#include <ctime>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


using namespace std;

string globalSeed = "1234";

int randomFunctionGenerator(int currentTimeInMinutes, string seed) {
    srand(stoi(seed));
    int hash = currentTimeInMinutes + rand()%100; // current time in minutes + random offset based on seed

    int offset = 'a' - 1;
    for(string::const_iterator it=seed.begin(); it!=seed.end(); ++it) {
        hash = hash << 1 ^ (*it - offset);
    }

    globalSeed = to_string(hash); // use current hash as the next seed

    return abs(hash) % 7; // mod the absolute value of the hash with the number of functions we have
}



#define LEN 1024

 int main(int argc, char *argv[])
 {
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

    for(;;) {
        bzero(buff, LEN);
        
        int currentTimeInMinutes = (time(0)/60) % 60;

        if(minutes + runEvery == currentTimeInMinutes || firstRun == true) {
            minutes = currentTimeInMinutes;
            int function = randomFunctionGenerator(currentTimeInMinutes, globalSeed);

           
            switch (function) {
                case 0:
                {
                   char message0[] = "Message using A\n";
                   send(sockfd, message0, strlen(message0), 0);
                   recv(sockfd, buff, LEN, 0);
                   puts(buff);
                }
                   break;
                case 1:
                {
                   char message1[] = "Message using B\n";
                   send(sockfd, message1, strlen(message1), 0);
                   recv(sockfd, buff, LEN, 0);
                   puts(buff);
                }
                    break;
                case 2:
                {
                   char message2[] = "Message using C\n";
                   send(sockfd, message2, strlen(message2), 0);
                   recv(sockfd, buff, LEN, 0);
                   puts(buff);
                }
                    break;
                case 3:
                {
                    char message3[] = "Message using D\n";
                   send(sockfd, message3, strlen(message3), 0);
                   recv(sockfd, buff, LEN, 0);
                   puts(buff);
                }
                    break;
                case 4:
                {
                    char message4[] = "Message using E\n";
                    send(sockfd, message4, strlen(message4), 0);
                    recv(sockfd, buff, LEN, 0);
                    puts(buff);
                }
                    break;
                case 5:
                {
                    char message5[] = "Message using F\n";
                    send(sockfd, message5, strlen(message5), 0);
                    recv(sockfd, buff, LEN, 0);
                    puts(buff);
                }
                    break;
                case 6:
                {
                    char message6[] = "Message using G\n";
                    send(sockfd, message6, strlen(message6), 0);
                    recv(sockfd, buff, LEN, 0);
                    puts(buff);
                }
                    break;
                  
                default: 
                    break;
            }
            
        } 
        firstRun = false;
    }

    return 0;
 }
