// CSci114_P5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//HOW TO EXECUTE THE PROGRAM: 
// Type in command: ./a.exe A 30 1020

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#define LIST_SIZE 1024
using namespace std;

struct Node {
    int* data;
    Node* next;
    int blockID;
    int reqID; //Request ID
    int mem; //Free memory
    
    Node() {
        this->next = nullptr;
        data = new int[LIST_SIZE];
        mem = LIST_SIZE;
        blockID = -1;
        reqID = -1;
    }

    Node(int blockID) {
        //this->data = data;
        this->next = nullptr;
        data = new int[LIST_SIZE];
        mem = LIST_SIZE;
        this->blockID = blockID;
    }

    int getMem() { return this->mem; }

    int getBlockID() { return this->blockID; }

    int getReqID() { return this->reqID; }

    void setReqID(int req_ID) { this->reqID = req_ID; }

    Node* obtainNext() { return this->next; }

    void setNext(Node* next) { this->next = next; }

    void setAlloc(int freeMem) { this->mem -= freeMem; }

    void clearMem() { this->mem = LIST_SIZE; }
};

class LL {
    private:
        Node* x; //root
        int count;
    public:
        LL() {
            x = nullptr;
            count = 0;
        }     

        // Insert data into linked list
        void insert(int blockID) {
            this->count++;
            //Node* x = new Node(blockID, head);
            if (x == nullptr) {
                x = new Node(blockID);
                return;
            }

            else {
                Node* a = x;
                while (a->next != nullptr) a = a->next;
                a->next = new Node(blockID);
            }
        }

        //Insert node
        void insertNode(Node* temp) {
            //cout << "test37\n";
            temp->setNext(nullptr);
            //cout << "test38\n";
            this->count++;
            //cout << "test39\n";
            if (x == nullptr) {
              //  cout << "test40\n";
                x = temp;
               // cout << "test41\n";
                return;
               // cout << "test42\n";
            }

            if (x->getMem() >= temp->getMem()) {
                //cout << "test43\n";
                temp->setNext(x);
                //cout << "test44\n";
                x = temp;
                //cout << "test45\n";
                return;
                //cout << "test46\n";
            }

            Node* temp2 = x;
            //cout << "test47\n";
            while (temp2->obtainNext() != nullptr and temp2->obtainNext()->getMem() < temp->getMem()) {
              //  cout << "test48\n";
                temp2 = temp2->obtainNext();
                //cout << "test49\n";
            }
            //cout << "test50\n";

            temp->setNext(temp2->obtainNext());
            //cout << "test51\n";
            temp2->setNext(temp);
            //cout << "test52\n";
        }

        //Performing best fit
        Node* bestFit(int reqID, int reqSize) {
            //cout << "test16\n";
            if (count <= 0) {
              //  cout << "test17\n";
                cout << "Insufficient amount of memory for allocation\n";
                return nullptr;
                //cout << "test18\n";
            }
            //cout << "test19\n";
            Node* temp = x;
            if (temp == nullptr) return nullptr;
            //cout << "test20\n";

            if (temp->getMem() > reqSize) {
              //  cout << "test21\n";
                this->count--;
                //cout << "test22\n";
                x = x->obtainNext();
                //cout << "test23\n";
                temp->setReqID(reqID);
                //cout << "test24\n";
                temp->setAlloc(reqSize);
                //cout << "test25\n";
                return temp;
                //cout << "test26\n";
            }
            //cout << "test26\n";

            while (temp->obtainNext() != nullptr and temp->obtainNext()->getMem() < reqSize) {
               // cout << "test27\n";
                temp = temp->obtainNext();
                //cout << "test28\n";
            }

            Node* temp2 = temp->obtainNext();
            //cout << "test29\n";
            if (temp2 != nullptr) {
              //  cout << "test30\n";
                temp->setNext(temp->obtainNext()->obtainNext());
                //cout << "test31\n";
                temp2->setReqID(reqID);
                //cout << "test32\n";
                temp2->setAlloc(reqSize);
                //cout << "test33\n";
                this->count--;
                //cout << "test34\n";
            }
            //cout << "test35\n";

            return temp2;

            //cout << "test36\n";
        }

        //Memory deallocation
        //FIXME
        Node* dealloc(int reqID) {
            if (count <= 0) {
                cout << "Insufficient amount of memory for allocation\n";
                return nullptr;
            }

            if (x == nullptr) return nullptr;

            Node* temp = x;
            Node* prev = nullptr;
            
            if (temp != nullptr and temp->getReqID() == reqID) {
                prev = temp;
                x = temp->obtainNext();
                this->count--;
                return prev;
            }

            while (temp->obtainNext() != nullptr and temp->obtainNext()->getReqID() != reqID) temp = temp->obtainNext();
   
            Node* temp2 = temp->obtainNext();
            
            if (temp2 != nullptr) temp->setNext(temp2->obtainNext());

            this->count--;

            return temp2;
        }

        //Memory reallocation
        void realloc(Node* temp) {
            temp->setReqID(-1);
            temp->clearMem();
            insertNode(temp);
        }

        int getSize() { return this->count; }
        
        //Print block ID
        void printBlockID() {
            Node* temp = x;
            while (temp != nullptr) {
                cout << temp->getBlockID() << " ";
                if (temp->getBlockID() > 0 and temp->getBlockID() == 0) cout << endl;
                temp = temp->obtainNext();
            }
            cout << endl;
        }

        void printReqID() {
            Node* temp = x;
            while (temp != nullptr) {
                cout << temp->getReqID() << " ";
                if (temp->getReqID() > 0 and temp->getReqID() == 0) cout << endl;
                temp = temp->obtainNext();
            }
            cout << endl;
        }

        //Print the amount of free memory
        void printMem() {
            Node* temp = x;
            while (temp != nullptr) {
                cout << temp->getMem() << " ";
                if (temp->getMem() > 0 and temp->getMem() == 0) cout << endl;
                temp = temp->obtainNext();
            }
            cout << endl;
        }

        void outputFile() {
            ofstream file_out;
            file_out.open("FINAL_SIZE.txt", ios::out);
            Node* temp = x;
            while (temp != nullptr) {
                file_out << temp->getMem() << endl;
                //if (temp->getMem() > 0 and temp->getMem() == 0) file_out;
                temp = temp->obtainNext();
            }
            file_out.close();
        }
};

int main(int argc, char** argv) {
    ifstream file_in;
    ofstream file_out;
    string input;
    //LL* head = new LL();
    LL *unallocated = new LL();
    LL *allocated = new LL();

    //Inserting 1024 nodes into linked list
    for (int i = 0; i < LIST_SIZE; i++) unallocated->insert(i);

    file_in.open("requests.txt",ifstream::in);
    cout << "Opening file.." << endl;

    cout << "Unallocated size: " << unallocated->getSize() << endl;
    cout << "Allocated size: " << allocated->getSize() << endl;
    //file_out.open("FINAL_SIZE.txt", ios::out);
    
    while (getline(file_in, input)) {
        //cout << "test1\n";
        string type;
        int req, req_size;
        size_t pos = input.find(" ");
        type = input.substr(0, pos);
        input.erase(0, pos + 1);
        pos = input.find(" ");
        req = stoi(input.substr(0, pos));
        //input.erase(0, pos + 1);
        if (type == "A") {
          //  cout << "test2\n";
            //allocate
            input.erase(0, pos + 1);
            //cout << "test3\n";
            pos = input.find(" ");
            //cout << "test4\n";
            req_size = stoi(input.substr(0, pos));
            //cout << "test5\n";
            Node* alloc_node = unallocated->bestFit(req, req_size);
            //cout << "test6\n";
            if (alloc_node != nullptr) {
              //  cout << "test7\n";
                allocated->insertNode(alloc_node);
                //cout << "test8\n";
                cout << req_size << " bytes have been allocated at block " << alloc_node->getBlockID() << " for request " << req << endl;
                //cout << "test9\n";
            }
        }

        else if (type == "R") {
            //allocated->printReqID();
            //cout << "test10\n";
            //release
            Node* dealloc_node = allocated->dealloc(req);
            //cout << "test11\n";
            if (dealloc_node != nullptr) {
              //  cout << "test12\n";
                req_size = LIST_SIZE - dealloc_node->getMem();
               // cout << "test13\n";
                unallocated->realloc(dealloc_node);
                //cout << "test14\n";
                cout << req_size << " bytes have been allocated at block " << dealloc_node->getBlockID() << " for request " << req << endl;
            }
        }
        //Writing to FINAL_SIZE.txt file
        
        //file_out << input << endl;
    }
    
    //Closing a file
    unallocated->outputFile();
    file_in.close();
    file_out.close();
}