#include <ctime>
#include <iostream>
#include <string>
#include <cstdlib> 

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

void a(){
    cout << "I am waiting to receive packets, decode the speed field, and run attack a." <<endl;
}
void b(){
    cout << "I am waiting to receive packets, decode the latitude field, and run attack b." <<endl;
}
void c(){
    cout << "I am waiting to receive packets, decode the longitude field, and run attack c." <<endl;
}
void d(){
    cout << "I am waiting to receive packets, decode some field, and run attack d." <<endl;
}
void e(){
    cout << "I am waiting to receive packets, decode some field, and run attack e." <<endl;
}
void f(){
    cout << "I am waiting to receive packets, decode some field, and run attack f." <<endl;
}
void g(){
    cout << "I am waiting to receive packets, decode some field, and run attack g." <<endl;
}


 int main(int argc, char *argv[])
 {
    time_t t = time(0);   // get time now
    int minutes = ((t / 60) % 60); // current time in minutes
    bool firstRun = true;
    int runEvery = 1; // pick a new function randomly every X minutes

    for(;;) {
        int currentTimeInMinutes = (time(0)/60) % 60;

        if(minutes + runEvery == currentTimeInMinutes || firstRun == true) {
            minutes = currentTimeInMinutes;
            int function = randomFunctionGenerator(currentTimeInMinutes, globalSeed);

            switch (function) {
                case 0: 
                    a();
                    break;
                case 1: 
                    b();
                    break;
                case 2: 
                    c();
                    break;
                case 3: 
                    d();
                    break;
                case 4: 
                    e();
                    break;
                case 5: 
                    f();
                    break;
                case 6: 
                    g();
                    break;
                default: 
                    cout << "YOU MESSED UP YOUR RANDOM FUNCTION GENERATOR" << endl;
                    break;
            }
            cout << endl;
        }
        firstRun = false;
    }

    return 0;
 }