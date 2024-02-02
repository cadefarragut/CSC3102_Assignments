#include <iostream>
#include <string>
using namespace std;

class Node
{
public:
    int key;
    Node* prev;
    Node* next;
    Node(int key){
        this->key = key;
        this->prev = NULL;
        this->next = NULL;
    }

};      

class DoublyLinkedList{
    Node* head;
    Node* tail;
    int count;
    public:
        DoublyLinkedList(){
            head = NULL;
            tail = NULL;
            count = 0;
        }

void insertF(int key){
    Node* p = new Node(key);
    if(head == NULL){
        head=tail=p; // it is not a single element
    }
    else {
        p->prev=NULL; //new node's previous is null since it is at the beginning
        p->next=head; //new node's next is the old head
        head->prev = p;  //set current head's prev to p
        head = p; //set new head to p
    }
    count++;
}

void insertL(int key){
    Node* p = new Node(key);
    if(tail==NULL){
        head=tail=p;
    }
    else{
        tail->next = p;//set tail's next to p
        p->prev = tail;//set p's prev to tail
        tail=p; //set new tail as p
        p->next=NULL; //set p's next to null
    }
    count++;
}

int deleteF(){
    if(count==0){
        return -1; //error
    }
    Node* p = head;
    int i = p->key;
    head = head->next; //prepare to remove head by setting head to head.next
    if(head == NULL){
        tail = NULL;//if list becomes empty after deletion
    }
    else{
        p->prev = NULL; //new head lets go of the old head by setting new head's prev to null
    }
    delete p;
    count--; //decrement count
            //deallocate p (optional)
    return i;

}

int deleteL(){
    if(count==0){
        return -1; //error
    }
    Node* p = tail;
    int i = p->key;
    tail = tail->prev;
    if(tail == NULL){
        head = NULL;
    }
    else{
        tail->next=NULL;
    }
  delete p;
    count--;
  return i;
}

void printall(){
    Node* p = head;
    while(p!=NULL){
        cout<<p->key;
        if(p->next != NULL){
            cout<<" ";
        }
        p=p->next;
    }
  cout<<endl;
}
};
int main(){
DoublyLinkedList* l = new DoublyLinkedList(); 
l->insertF(32);
l->insertF(42);
l->insertL(37);
l->insertL(27);
int temp = l->deleteF();
l->printall();

  return 0;
}
