/*HOW TO EXECUTE PROGRAM
1. Type in command: ./a.exe
*/

// CSci114_P2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int proc[2]; //Processes A and B (pipe connecting the processes)
char buf[100]; //Buffer

int main() {
	int status;
	status = pipe(proc); //Turns array into a pipe
	int pid = fork(); //Create child
	
	if (pid == 0) { //This is the child process
		close(proc[1]); //Child closing the unused writing end
		ofstream data_out;
		data_out.open("data.out", ios::binary);
		while (read(proc[0], buf, 100) > 0) { //While trying to read from the pipe until pipe is empty
			data_out.write(buf, 100); //Writing from the buffer to the file
		}
		close(proc[0]); //Closes the reading end
		data_out.close();
	}
	else {
		close(proc[0]);
		ifstream data_in;
		data_in.open("data.in", ios::binary);
		while (data_in.read(buf,100)) { //Reading from the file to the buffer
			write(proc[1],buf,data_in.gcount()); //Writing to the pipe from the buffer
		}
		//Prevent an early breakout, read more bytes if needed
		write(proc[1], buf, data_in.gcount()); //gcount = count # of bytes read from input file
		data_in.close();
		close(proc[1]); //Closes writing end
	}
	
	wait(&status);
	wait(&status);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu