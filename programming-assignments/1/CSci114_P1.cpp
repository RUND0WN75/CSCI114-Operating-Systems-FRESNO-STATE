/*HOW TO EXECUTE PROGRAM
1. Type in command: ./a.exe <filename>
*/

// CSci114_P1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		cout << "Insufficient argument\n";
		return 0;
	}

		ifstream data_in;
		ofstream data_out;
		string x;

		//Reading in a file
		data_in.open(argv[1], ios::binary);
		if (data_in.is_open()) {
			cout << "File is open\n";
			data_out.open("data_out.out", ios::binary);
			while (getline(data_in, x)) {
				data_out << x << endl;
			}
		}
		else cout << "File can't be opened\n";

		//Closing a file
		data_in.close();
		data_out.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu