#ifndef ITERATOR_H  // Header Wrapper
#define ITERATOR_H

#include "treenode.h"

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

#endif