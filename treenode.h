#ifndef TREENODE_H  // Header Guard
#define TREENODE_H

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

#endif