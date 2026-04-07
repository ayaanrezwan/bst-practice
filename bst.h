#include "treenode.h"
#include <vector>
using namespace std;

// Class implementation
template <typename T>
class BST{
    public: // Member functions + "public wrapped" functions
        BST();  // Default constructor
        BST(const T elements[], int arraySize); // Array constructor
        BST(const BST<T>& tree);    // Copy constructor
        ~BST(); // Destructor
        bool search(const T& element) const;    // Searches for a given element, true if exists, false if not
        virtual bool insert(const T& element);  // Insert an element at the correct location, true if inserted, false if duplicate
        virtual bool remove(const T& element);  // Removes the inputted element, true if removed, false if DNE
        void inorder() const;   // Traverses BST in order (Left -> Root -> Right recursively), increasing order
        void preorder() const;  // Traverses BST preorder (Root -> Left -> Right)
        void postorder() const; // Traverses BST preorder (Left -> Right -> Root)
        int getSize() const;    // Returns size of the BST
        void clear();   // Remove all nodes from tree (used for destructor)
        vector<TreeNode<T>*>* path(const T& element) const; // Returns a pointer to a vector which holds a path of points to reach the given element

        // --- IMPLEMENT ITERATORS HERE ---

    protected: // Protected functions/variables (accessible to child classes)
        TreeNode<T>* root;
        int size;
        virtual TreeNode<T>* createNewNode(const T& element);
    
    private: // Recursive helper functions that are wrapped in public
        void inorder(const TreeNode<T>* root) const;    // Recursive algorithm for inorder
        void preorder(const TreeNode<T>* root) const;   // Recursive algorithm for preorder
        void postorder(const TreeNode<T>* root) const;  // Recursive algorithm for postorder
        void copy(const TreeNode<T>* root); // Recursive algorithm for copy (copy constructor)
        void clear(const TreeNode<T>* root);    // Recursive algorithm for clearing the bst (destructor)
};

// Default constructor implementation
template <typename T>
BST<T>::BST() {
    root = nullptr; // Empty BST
    size = 0;   // Size 0 because empty
}

// Array constructor for BST (takes an array and creates a BST from its indices)
template <typename T>
BST<T>::BST(const T elements[], int arraySize) {
    root = nullptr;
    size =  0;

    for (int i = 0; i < arraySize; i++) {
        insert(elements[i]);    // Iterate through the array and add each index to the BST
    }
}

// Copy constructor
template <typename T>
BST<T>::BST(const BST<T>& tree) {
    root = nullptr;
    size = 0;
    copy(tree.root);    // Calls copy function which is implemented later
}

// Destructor
template <typename T>
BST<T>::~BST() {
    clear();    // Using implemented clear member function (which uses the private function)
}

// Boolean search for a given element in the BST
template <typename T>
bool BST<T>::search(const T& element) const{    // Searching tree for element
    TreeNode<T>* current = this;    // Start at root

    while (current != nullptr) {    // Repeat until hits end of the tree
        if (current -> element == element) {
            return true;

        } else if (element < current -> element) {  // If element is less, go left
            current = current->left;

        } else {    // If element is greater, go right
            current = current->right;

        }
    }
    return false;   // If nothing is found, return false
}

// Boolean insertion, puts an element into the BST and returns true if it works. False if it's a dupe
template <typename T>
bool BST<T>::insert(const T& element) {
    if (root == nullptr) {   // If root is empty, start the tree
        root = createNewNode(element);
    } else {
        // Helper pointers (only allocate if needed inside of logic statement)
        TreeNode<T>* current = root;    // Used mainly as a "next"
        TreeNode<T>* parent = nullptr;  // Used to keep track of current node to place element as its child
        while (current != nullptr) {    // Loop until hit the end of a branch
            if (element < current -> element) {
                parent = current;
                current = current -> left;  // If insertion element is less than the current element, move left
            } else if (element > current -> element) {
                parent = current;
                current = current -> right; // If the insertion element is more than the current element, move right
            } else {
                return false;   // Duplicate detected, no insertion or size increase
            }
        }
        if (element < parent -> element) {  // Placing element as the correct child
            parent -> left = createNewNode(element);    
        } else {
            parent -> right = createNewNode(element);
        }
    }
    size++;
    return true;    // Increasing size and returning true to indicate successful insertion
}

// Boolean removal function, gets rid of one element from the array
