// CSci114_P4.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* HOW TO EXECUTE PROGRAM
1. Type in command: ./a.exe #
# is any number between 5-15
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <fstream>
#include <condition_variable>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
using namespace std;
mutex mtx;

class Car {
    private:
        int waitNorth = 0;
        int waitSouth = 0;
        int inNorth = 0;
        int inSouth = 0;

    public:
        condition_variable northCV, southCV;
        int n;

        void go(char path) { 
            this_thread::sleep_for(chrono::seconds(rand() % 3));
            if (path == 'N') {
                northEnter();
                northLeave();
            }
            else {
                southEnter();
                southLeave();
            }
        }

        void start(int n) { 
            vector<thread> threads;
            this->n = n;
            ifstream a;
            a.open("cars.txt", ios::in);

            char path;
            while (a >> path) threads.push_back(std::thread( &Car::go, this, path));

            for (int i = 0; i < threads.size(); i++) threads.at(i).join();
        }

        int northArrive() { return inNorth; } //North car arriving
        
        bool northShouldWait() {
            //If there are no south cars waiting & no cars in the tunnel
            if (waitSouth == 0 and inSouth == 0) return 0;
            return (inNorth == n || inSouth > 0);
        } //North car waiting
        
        void northEnter() { //North car entering tunnel
            mtx.lock();
            unique_lock<mutex> lock1(mtx, defer_lock);
            waitNorth++;
            cout << "North is waiting: " << waitNorth << endl;
            while (northShouldWait()) northCV.wait(lock1);
            waitNorth--;
            inNorth++;
            
            cout << "North is entering: " << inNorth << endl;
            mtx.unlock();
        };
        
        void northLeave() { //North leaving tunnel
            mtx.lock();
            inNorth--;
            if (inNorth == 0) {
                southCV.notify_all();
               
            }
            else northCV.notify_one();
            cout << "North is leaving: " << inNorth << endl;
            mtx.unlock();
        }
        
        int southArrive() { return inSouth; } //South car arriving
        
        bool southShouldWait() { 
            //If there are no north cars waiting & no cars in the tunnel
            if (waitNorth == 0 and inNorth == 0) return 0;
            return (inSouth == n || inNorth > 0);
        } //South car waiting
        
        void southEnter() { //South car entering tunnel
            mtx.lock();
            unique_lock<mutex> lock2(mtx, defer_lock);
            waitSouth++;
            cout << "South is waiting: " << waitSouth << endl;
            while (southShouldWait()) southCV.wait(lock2);
            //Decrement south
            waitSouth--;
            inSouth++;
       
            cout << "South is entering: " << inSouth << endl;
            mtx.unlock();
        }
        
        void southLeave() { //South leaving tunnel
            mtx.lock();
            inSouth--;
            if (inSouth == 0) {
                northCV.notify_all();
           
            }
            else southCV.notify_one();
            cout << "South is leaving: " << inSouth << endl;
            mtx.unlock();
        }
};

int main(int argc, char** argv) {

    Car sync;

    int n = stoi(argv[1]);

    sync.start(n);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu