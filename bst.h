#ifndef BST_H
#define BST_H

#include <vector>
#include <stdexcept>
using namespace std;

// TreeNode class for BST and AVL trees
template<typename T>
class TreeNode {
    public:
        T element;  // Element which the node holds
        TreeNode<T>* left;  // Pointer at left child
        TreeNode<T>* right; // Pointer at right child
        TreeNode<T>(const T& element);
};

template <typename T>
TreeNode<T>::TreeNode(const T& element) {  // Constructor
    // Store the parameter into node element
    this -> element = element;

    // Set child pointers to nullptr
    left = nullptr;
    right = nullptr;
}

// Iterator class for BST and AVL trees, used for traversal
template <typename T>
class Iterator: public std::iterator<std::forward_iterator_tag, T> {
    public:
        // Constructs an iterator that points at a specific node in the tree
        Iterator(TreeNode<T>* p)
        {
            if (p == nullptr) {
                current = -1; // The end
            } else {
                // Get all the elements in inorder
                treeToVector(p);
                current = 0;
            }
        }

        // Obtains the iterator for the next element in the array
        Iterator operator++()
        {
            current++;
            if (current == v.size()) {
                current = -1; // The end
            }
            return *this;
        }

        // Using the * operator returns the element pointed by the iterator
        T &operator*()
        {
            return v[current];
        }

        // Checks if two iterators are the same (equality)
        bool operator==(const Iterator<T>& iterator) const
        {
            return current == iterator.current;
        }

        // Checks if two iterators are the same (not equality)
        bool operator!=(const Iterator<T>& iterator) const
        {
            return current != iterator.current;
        }

    private:
        int current;
        vector<T> v;
        void treeToVector(const TreeNode<T>* p) // Vector that stores nodes in order
        {
            if (p != nullptr) {
                treeToVector(p->left);
                v.push_back(p->element);
                treeToVector(p->right);
            }
        }
};

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

        Iterator<T> begin() const { // Returns iterator for the first node (root)
            return Iterator<T>(root);
        };

        Iterator<T> end() const {   // Returns iterator for the end
            return Iterator<T>(nullptr);
        };

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

// Recursive copy function from the given subtree
template <typename T>
void BST<T>::copy(const TreeNode<T>* root) {
    if (root != nullptr) {
        insert(root->element);  // preorder traversal (copy first, then move)
        copy(root -> left);
        copy(root -> right);
    }
}

// Destructor
template <typename T>
BST<T>::~BST() {
    clear();    // Using implemented clear member function (which uses the private function)
}

// Boolean search for a given element in the BST
template <typename T>
bool BST<T>::search(const T& element) const {    // Searching tree for element
    TreeNode<T>* current = root;    // Start at root

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

// Function to create new node with a given element
template <typename T>
TreeNode<T>* BST<T>::createNewNode(const T& e)
{
  return new TreeNode<T>(e);
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

// inorder traversal printing from root (automatically)
template <typename T>
void BST<T>::inorder() const {
    inorder(root);  // Calls the recursive helper that is wrapped
}

// inorder traversal printing from a subtree (recursive & private)
template <typename T>
void BST<T>::inorder(const TreeNode<T>* root) const {
    if (root == nullptr) return;    // If empty tree then return nothing
    inorder(root -> left);
    cout << root -> element << " ";
    inorder(root -> right);
}

// postorder traversal printing from root (automatically)
template <typename T>
void BST<T>::postorder() const {
    postorder(root);
}

// postorder traversal printing from subtree (recursive & private)
template <typename T>
void BST<T>::postorder(const TreeNode<T>* root) const {
    if (root == nullptr) return;    // If empty tree then return nothing
    postorder(root -> left);
    postorder(root -> right);
    cout << root -> element << " ";
}

// preorder traversal printing from root (automatically)
template <typename T>
void BST<T>::preorder() const {
    preorder(root);
}

// preorder traversal printing from subtree (recursive & private)
template <typename T>
void BST<T>::preorder(const TreeNode<T>* root) const {
    if (root == nullptr) return;    // If empty tree then return nothing
    cout << root -> element << " ";
    preorder(root -> left);
    preorder(root -> right);
}

// Size return function
template <typename T>
int BST<T>::getSize() const {
    return size;
}

// Full BST deletion function
template <typename T>
void BST<T>::clear() {
    clear(root);  // Calls the recursive helper that is wrapped
    root = nullptr;   // Set root to null after clearing
    size = 0;   // Set size to 0 after clearing
}

// Recursive BST deletion function (used for destructor)
template <typename T>
void BST<T>::clear(const TreeNode<T>* root) {
    if (root != nullptr) {    // If empty tree then return nothing
        clear(root -> left);
        clear(root -> right);
        delete root;   // Deletes the current node after deleting its children (postorder)
    }
}

// Path return function to arrive at an element
template <typename T>
vector<TreeNode<T>*>* BST<T>::path(const T& element) const {
    vector<TreeNode<T>*>* v = new vector<TreeNode<T>*>();   // Allocating memory for the vector
    TreeNode<T>* current = root;    // Creating a helper pointer

    while (current != nullptr) {    // Loop until hits end of subtree
        v->push_back(current);  // Append current pointer to the vector (add onto)
        if (element < current -> element) {
            current = current -> left;  // If target element is less than current, move left
        } else if (element > current -> element) {
            current = current -> right; // If target element is more than current, move right
        } else {
            break;  // If element not found, break
        }
    }
    return v;   // Return the resulting array
}

// Boolean removal function, gets rid of one element from the array
template <typename T>
bool BST<T>::remove(const T& element) {
    // Locate the node to be deleted and also its parent
    TreeNode<T>* parent = nullptr;
    TreeNode<T>* current = root;
    while (current != nullptr) {    // Repeat until an end is hit
        if (element < current -> element) {
            parent = current;
            current = current -> left;  // If target element is less than current element, move left
        } else if(element > current -> element) {
            parent = current;
            current = current -> right; // If target element is more than current element, move right
        } else {
            break;   // Element found and is equal to current or end is hit
        }
    }
    if (current == nullptr) {
        return false;   // Inputted element is not in the tree
    }

    // CASE ONE: Current has no left children (no future nodes are less than current element)
    if (current -> left == nullptr) {
        if (parent == nullptr) {
            root = current -> right;    // If the element is the root, then the new root is its right child
        } else {
            if (element < parent -> element) {
                parent -> left = current -> right;
            } else {
                parent -> right = current -> right;
            }
        }
        delete current; // Delete the current node (the target element)
    } else {  // CASE TWO: Current node has a left child (must find rightmost in its subtree to rotate)
        TreeNode<T>* parentOfRightMostNode = current;
        TreeNode<T>* rightMostNode = current -> left;

        while (rightMostNode -> right != nullptr) { // Repeat until the rightmost node is hit
            parentOfRightMostNode = rightMostNode;
            rightMostNode = rightMostNode -> right; // Shifting both the parent of and rightmost node to the right
        }

        // Replace element in current with the rightmost element (removes the target)
        current -> element = rightMostNode -> element;

        // Take the parent of the rightmost node (that replaced the target) and refactor
        if (parentOfRightMostNode -> right == rightMostNode) {  // Verify that the right node is selected
            parentOfRightMostNode -> right = rightMostNode -> left;
        } else {    // EDGE CASE: the rightmost node is actually the target
            parentOfRightMostNode -> left = rightMostNode -> left;  // Sets the sibling of target to its left child
        }
    }
    size--;
    return true;
}

#endif