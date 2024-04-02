#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <omp.h>


using namespace std;

struct Node {
    int key, data = 0, size = 1, height = 1, bf = 0;
    Node *left = nullptr, *right = nullptr, *parent = nullptr;

    Node(int key) : key(key) {}
    
    int getHeight() {
        if (this == nullptr) return 0;
        return this->height;
    }
    
    int getSize() {
        if (this == nullptr) return 0;
        return this->size;
    }
};

class AVLfull{
public:
    Node *root = nullptr;

    Node* search(int key) {
        return searchrec(key, root);
    }
    
    Node* searchrec(int key, Node *x) {
        if (x == nullptr) return nullptr;
        if (key == x->key) return x;
        else if (key < x->key) return searchrec(key, x->left);
        else return searchrec(key, x->right);
    }

    void deletekey(int key) {
        Node *x = search(key);
        if (x != nullptr) deleteNode(x);
    }
    
    void insert(Node *x) {
        if (root == nullptr) root = x;
        else insertNode(x, root);
    }
    
    void insertNode(Node *x, Node *y) {    
        if (x->key == y->key) 
            return;
        if (x->key < y->key) {
            // fill this
            if(y->left == nullptr){
               y->left = x;
               x->parent = y;
            }
            else{                   
             insertNode(x, y->left);
        }
        }
        if (x->key > y->key) {
            // fill this
                if(y->right == nullptr){
                y->right = x;
                x->parent = y;
            }
            else{
                insertNode(x, y->right);
            }
        }
        
        fixme(y); //fixes y - an ancestor of x
    }
    
    void deleteNode(Node *x) {
        if (x->left == nullptr && x->right == nullptr) 
            {transplant(x, nullptr); fixup(x->parent);}

        else if (x->left == nullptr)
            {transplant(x, x->right); fixup(x->right);}
        else if (x->right == nullptr)
            {transplant(x, x->left); fixup(x->left);} 
        
        else {  
            Node *y = x->right; 
            while (y->left != nullptr) y = y->left; // get successor
            
            if (y->parent == x) { // Subcase 3.1
                transplant(x, y); 
                y->left = x->left;
                x->left->parent = y;
                fixup(y);
            }
            else { // Subcase 3.2
                transplant(y, y->right); 
                transplant(x, y);
                y->right = x->right; y->left = x->left;
                x->right->parent = x->left->parent = y;
                recalc(y);
                fixup(y->right);
            }
        }
    }
    
    void fixup(Node *x) {
        Node *y = x;
        while (y != nullptr) {
            fixme(y); 
            y = y->parent;
        }
    }
    
    void fixme(Node *x) {
        //fix x if its balance factor is violated
        recalc(x); 

        //uncomment one of the below two statements
        if ((x->bf <= 1) && (x->bf >= -1)) return; //AVL tree
        //if (true) return; //just BST


        Node *y = tallchild(x);
        Node *z = tallchild(y);
        
        if ((y == x->left) && (z == y->left)) {rrotate(x);}
        else if ((y == x->left ) && (z == y->right)) {lrotate(y); rrotate(x);}
        else if ((y == x->right) && (z == y->left)) {rrotate(y); lrotate(x);}
        else if ((y == x->right) && (z == y->right)) {lrotate(x);}
    }
    
    void transplant(Node *x, Node *y) { 

        //put node y in place of x in the tree
        //x along with its subtree will be orphaned from the tree
        //assumes x is not null
        // just as is from the slides

        if (x->parent == nullptr) root = y;           
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        
        if (y != nullptr) y->parent = x->parent;
    }
    
    void rrotate(Node *x) {
        //assumes x.left is not null

        Node *y = x->left;
        transplant(y, y->right);
        transplant(x, y);
        y->right = x; x->parent = y;
        recalc(x); recalc(y);
    }
    
    void lrotate(Node *x) {
        //assumes x.right is not null
        //please fill this up
        Node* y = x->right;
        transplant(y, y->left);
        transplant(x, y);
        y->left = x; x->parent = y;
        recalc(x); recalc(y);

    }
    
    int select(Node *x, int i) {
        //check for the condition if x == null 
        //compare the size of left child of x with i  
    if (x == nullptr) return 1;
    int temp;
    if(x->left != nullptr) temp = x->left->size;
    if(temp>=i) return select(x->left, i);
    else if(temp + 1 == i) return x->key;
    else return select(x->right, i - temp - 1);
    
    }
    
    Node* tallchild(Node *x) {
        if (x == nullptr) return nullptr;
        else if (x->left == nullptr) return x->right;
        else if (x->right == nullptr) return x->left;
        else if (x->left->height > x->right->height) return x->left;
        else return x->right;
    }
    
    void recalc(Node *x) {

        //recalculate x's height, size and bf 
        //assumes its children are correctly recalculated
     
        if (x == nullptr) return;
        if ((x->left != nullptr) && (x->right != nullptr)) {
            x->height = 1 + max(x->left->height, x->right->height);
            x->size = 1 + x->left->size + x->right->size;
            x->bf = x->left->height - x->right->height;
        }
        else if (x->right != nullptr) {
            x->height = 1 + x->right->height;
            x->size = 1 + x->right->size;
            x->bf = -x->right->height;
        }
        else if (x->left != nullptr) {
            x->height = 1 + x->left->height;
            x->size = 1 + x->left->size;
            x->bf = x->left->height;
        }
        else {
            x->height = 1;
            x->size = 1;
            x->bf = 0;
        }
    }
    
    int height(Node *x) {
        if (x == nullptr) return 0;
        return x->height;
    }
    
    int size(Node *x) {
        if (x == nullptr) return 0;
        return x->size;
    }
    
    void inorder(Node *p) {
        if (p == nullptr) return;

        cout << " (";
        inorder(p->left);
        cout << p->key;
        inorder(p->right);
        cout << ") ";
    }
};

int main() {
    AVLfull b;
    
    int n = 100;        
    auto startTime = chrono::high_resolution_clock::now();
    for (int i = 1 ; i <= n; i++) {
        //uncomment one of the below two statements
        //Node *p = new Node((i * 7637 + 571) % n + 1); //random inserts
        Node *p = new Node(i); // for the alternate case of sequential inserts
        b.insert(p);
    }
    auto endTime = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    cout << "n = " << b.root->size << " h = " << b.root->height << endl;
    cout << "h/log n = " << b.root->height / (log(b.root->size) / log(2)) << endl;
    cout << "Total Time = " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() / 1000.0 << " nanosec" << endl;
   cout << "Avg time per ins " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() / 1000.0 / b.root->size << " nanosec" << endl;
    // above 4 lines will be suppressed (commented out for Gradescope submission)

    int k = b.select(b.root, n / 2);
    cout << k << endl; // this line is for gradescope output

    //choose n = 10000 with sequential insert mode 
        //with AVL tree

    return 0;

}
