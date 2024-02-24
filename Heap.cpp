
//sample code and/or guided pseudocode for a binary Main

#include <iostream>
using namespace std;

class Heap
{
 public:
    int *A; //you may use Array instead
    int size = 0;
    int max;

    Heap(int max)
    {
        A = new int[max];
        this->max = max;
    }

    int min(int i, int j)
    { //returns index of minimum value within A[i..j]
        int minindex = i;
        for(int k=i+1; k<=j; k++)
            if (A[k] < A[minindex]) minindex = k;
        return minindex;
    }

    void swap(int i, int j) {int temp = A[i]; A[i]= A[j]; A[j] = temp;}

    public: int extractmin()
    {
        if (size == 0) return -1; //error
        int retval = A[0];
        A[0] = A[size-1]; //put last element at the top
        size--;
        heapify(0);
        return retval;
    }
    void heapify(int top)
    {  

     if(top*3+1 > size-1) return;
     int minChild = top*3+1;           //calculate minChild

     for(int i = 1; i <=2; i++){      //loops through children to find the smallest
       int next = top*3 + 1 + i;
       if(next <= size-1 && (A[next] < A[minChild])){
         minChild = next;
     
       }
     }
     if(A[top] > A[minChild]){      //else there is violation of value property so "swap( , )";
       swap(top, minChild);
       
       } 
     
    heapify(minChild);             //recurse with minChild - to fix further down
    }
     
    
  
        
        

        
        
        
    

    void insert(int newkey)
    {
            if (size == max) return; //error may occur
            A[size] = newkey; // add new key at the end of the heap
            int pos = size; //set pos to last items location

            size++; //heap's size grows by one due to this new arrival
            percolate(pos);
    }

    void percolate(int pos)
    {// fixes upwards recursively

            if (pos <= 0) return;  //if all fine, then return

            int parent = (pos-1)/3;  // calculate the position of parent 
            if(A[pos] < A[parent]){    // if conflict
                swap(pos, parent);    // swap
                percolate(parent); // recurse with parent
            }
    }
            
    

    void decreasekey(int pos, int newkey)
    { // decreases the key at A[pos] to a new value
         if(A[pos] <  newkey) return;  //if newkey is bigger than A[pos] nothing to do
         if(pos <= 0) return;
          A[pos] = newkey;            //else, replace the value at pos with newkey
        int parent = (pos-1)/3;
        if(A[pos] < A[parent]) percolate(pos); //fix value property
        }

        
       
        
       
    
};

    int main()
    {
        Heap H(100);

        for (int i = 10; i > 0; i--)
            H.insert((i*3497+379)%1000);

    H.decreasekey(9,100);

        for (int i = 0; i < 10; i++)
            cout<<H.extractmin()<<endl;
  return(0);
    }


