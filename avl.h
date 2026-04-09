#ifndef AVL_H
#define AVL_H

#include "bst.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

template<typename T>
class AVLTreeNode : public TreeNode<T>
{
public:
  int height; // height of the node

  AVLTreeNode(const T& e) : TreeNode<T>(e) // Constructor
  {
    height = 0;
  }
};

template <typename T>
class AVLTree : public BST<T>
{
public:
  AVLTree();
  AVLTree(const T elements[], int arraySize);
  bool insert(const T& e);
  bool remove(const T& e);

  AVLTreeNode<T>* createNewNode(const T& e);

  void balancePath(const T& e);
  void updateHeight(AVLTreeNode<T>* node);
  int balanceFactor(AVLTreeNode<T>* node);

  // Original 4 individual rotation functions (kept for reference)
  void balanceLL(TreeNode<T>* A, TreeNode<T>* parentOfA);
  void balanceLR(TreeNode<T>* A, TreeNode<T>* parentOfA);
  void balanceRR(TreeNode<T>* A, TreeNode<T>* parentOfA);
  void balanceRL(TreeNode<T>* A, TreeNode<T>* parentOfA);

  // Combined rotation functions
  void doubleRotate(TreeNode<T>* A, TreeNode<T>* parentOfA, bool leftHeavy);
  void singleRotate(TreeNode<T>* A, TreeNode<T>* parentOfA, bool leftHeavy);
};

template <typename T>
AVLTree<T>::AVLTree()
{
  
}

template <typename T>
AVLTree<T>::AVLTree(const T elements[], int arraySize)
{
  this->root = nullptr;
  this->size = 0;

  for (int i = 0; i < arraySize; i++)
  {
    insert(elements[i]);
  }
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::createNewNode(const T& e)
{
  return new AVLTreeNode<T>(e);
}

template <typename T>
bool AVLTree<T>::insert(const T& e)
{
  bool successful = BST<T>::insert(e);
  if (!successful)
    return false;
  else
    balancePath(e);

  return true;
}

template <typename T>
void AVLTree<T>::balancePath(const T& e)
{
  vector<TreeNode<T>*>* p = this->path(e);
  for (int i = (*p).size() - 1; i >= 0; i--)
  {
    AVLTreeNode<T>* A = static_cast<AVLTreeNode<T>*>((*p)[i]);
    updateHeight(A);
    AVLTreeNode<T>* parentOfA = (A == this->root) ? nullptr :
      static_cast<AVLTreeNode<T>*>((*p)[i - 1]);

    switch (balanceFactor(A))
    {
      case -2:
        if (balanceFactor(
            static_cast<AVLTreeNode<T>*>(((*A).left))) <= 0)
          balanceLL(A, parentOfA);
        else
          balanceLR(A, parentOfA);
        break;
      case +2:
        if (balanceFactor(
             static_cast<AVLTreeNode<T>*>(((*A).right))) >= 0)
          balanceRR(A, parentOfA);
        else
          balanceRL(A, parentOfA);
    }
  }
}

template <typename T>
void AVLTree<T>::updateHeight(AVLTreeNode<T>* node)
{
  if (node->left == nullptr && node->right == nullptr)
    node->height = 0;
  else if (node->left == nullptr)
    node->height =
      1 + (*static_cast<AVLTreeNode<T>*>((node->right))).height;
  else if (node->right == nullptr)
    node->height =
      1 + (*static_cast<AVLTreeNode<T>*>((node->left))).height;
  else
    node->height = 1 +
      max((*static_cast<AVLTreeNode<T>*>((node->right))).height,
      (*static_cast<AVLTreeNode<T>*>((node->left))).height);
}

template <typename T>
int AVLTree<T>::balanceFactor(AVLTreeNode<T>* node)
{
  if (node->right == nullptr)
    return -node->height;
  else if (node->left == nullptr)
    return +node->height;
  else
    return (*static_cast<AVLTreeNode<T>*>((node->right))).height -
      (*static_cast<AVLTreeNode<T>*>((node->left))).height;
}

template <typename T>
void AVLTree<T>::balanceLL(TreeNode<T>* A, TreeNode<T>* parentOfA)
{
  TreeNode<T>* B = (*A).left;

  if (A == this->root)
    this->root = B;
  else
    if (parentOfA->left == A)
      parentOfA->left = B;
    else
      parentOfA->right = B;

  A->left = B->right;
  B->right = A;
  updateHeight(static_cast<AVLTreeNode<T>*>(A));
  updateHeight(static_cast<AVLTreeNode<T>*>(B));
}

template <typename T>
void AVLTree<T>::balanceLR(TreeNode<T>* A, TreeNode<T>* parentOfA)
{
  TreeNode<T>* B = A->left;
  TreeNode<T>* C = B->right;

  if (A == this->root)
    this->root = C;
  else
    if (parentOfA->left == A)
      parentOfA->left = C;
    else
      parentOfA->right = C;

  A->left = C->right;
  B->right = C->left;
  C->left = B;
  C->right = A;

  updateHeight(static_cast<AVLTreeNode<T>*>(A));
  updateHeight(static_cast<AVLTreeNode<T>*>(B));
  updateHeight(static_cast<AVLTreeNode<T>*>(C));
}

template <typename T>
void AVLTree<T>::balanceRR(TreeNode<T>* A, TreeNode<T>* parentOfA)
{
  TreeNode<T>* B = A->right;

  if (A == this->root)
    this->root = B;
  else
    if (parentOfA->left == A)
      parentOfA->left = B;
    else
      parentOfA->right = B;

  A->right = B->left;
  B->left = A;
  updateHeight(static_cast<AVLTreeNode<T>*>(A));
  updateHeight(static_cast<AVLTreeNode<T>*>(B));
}

template <typename T>
void AVLTree<T>::balanceRL(TreeNode<T>* A, TreeNode<T>* parentOfA)
{
  TreeNode<T>* B = A->right;
  TreeNode<T>* C = B->left;

  if (A == this->root)
    this->root = C;
  else
    if (parentOfA->left == A)
      parentOfA->left = C;
    else
      parentOfA->right = C;

  A->right = C->left;
  B->left = C->right;
  C->left = A;
  C->right = B;

  updateHeight(static_cast<AVLTreeNode<T>*>(A));
  updateHeight(static_cast<AVLTreeNode<T>*>(B));
  updateHeight(static_cast<AVLTreeNode<T>*>(C));
}

template <typename T>
bool AVLTree<T>::remove(const T& e)
{
  if (this->root == nullptr)
    return false;

  TreeNode<T>* parent = nullptr;
  TreeNode<T>* current = this->root;
  while (current != nullptr)
  {
    if (e < current->element)
    {
      parent = current;
      current = current->left;
    }
    else if (e > current->element)
    {
      parent = current;
      current = current->right;
    }
    else
      break;
  }

  if (current == nullptr)
    return false;

  if (current->left == nullptr)
  {
    if (parent == nullptr)
      this->root = current->right;
    else
    {
      if (e < parent->element)
        parent->left = current->right;
      else
        parent->right = current->right;

      balancePath(parent->element);
    }
  }
  else
  {
    TreeNode<T>* parentOfRightMost = current;
    TreeNode<T>* rightMost = current->left;

    while (rightMost->right != nullptr)
    {
      parentOfRightMost = rightMost;
      rightMost = rightMost->right;
    }

    current->element = rightMost->element;

    if (parentOfRightMost->right == rightMost)
      parentOfRightMost->right = rightMost->left;
    else
      parentOfRightMost->left = rightMost->left;

    balancePath(parentOfRightMost->element);
  }

  this->size--;
  return true;
}


// COMBINED ROTATION FUNCTIONS

// --- singleRotate ---
// Replaces both balanceLL (leftHeavy = true) and balanceRR (leftHeavy = false).

template <typename T>
void AVLTree<T>::singleRotate(TreeNode<T>* A, TreeNode<T>* parentOfA, bool leftHeavy)
{
    // 1. Pick child B from A's heavy side
    TreeNode<T>* B = leftHeavy ? A->left : A->right;

    // 2. Reattach B where A was (parent linkage — identical for both cases)
    if (A == this->root)
        this->root = B;
    else if (parentOfA->left == A)
        parentOfA->left = B;
    else
        parentOfA->right = B;

    // 3. Rewire: move B's inner subtree to A, make A the inner child of B
    if (leftHeavy) {
        A->left  = B->right;   // B's right subtree (T2) becomes A's left
        B->right = A;           // A drops down to B's right
    } else {
        A->right = B->left;    // B's left subtree (T2) becomes A's right
        B->left  = A;           // A drops down to B's left
    }

    // 4. Update heights bottom-up (A moved down, so update A first)
    updateHeight(static_cast<AVLTreeNode<T>*>(A));
    updateHeight(static_cast<AVLTreeNode<T>*>(B));
}

// --- doubleRotate ---
// Replaces both balanceLR (leftHeavy = true) and balanceRL (leftHeavy = false).

template <typename T>
void AVLTree<T>::doubleRotate(TreeNode<T>* A, TreeNode<T>* parentOfA, bool leftHeavy)
{
    // 1. Pick child B (heavy side) and grandchild C (B's opposite side)
    TreeNode<T>* B = leftHeavy ? A->left  : A->right;
    TreeNode<T>* C = leftHeavy ? B->right : B->left;

    // 2. Reattach C where A was (parent linkage — identical for both cases)
    if (A == this->root)
        this->root = C;
    else if (parentOfA->left == A)
        parentOfA->left = C;
    else
        parentOfA->right = C;

    // 3. Distribute C's subtrees to A and B, then make A and B children of C
    if (leftHeavy) {
        A->left  = C->right;   // C's right subtree (T3) → A's left
        B->right = C->left;    // C's left subtree (T2)  → B's right
        C->left  = B;           // B becomes C's left child
        C->right = A;           // A becomes C's right child
    } else {
        A->right = C->left;    // C's left subtree (T2)  → A's right
        B->left  = C->right;   // C's right subtree (T3) → B's left
        C->left  = A;           // A becomes C's left child
        C->right = B;           // B becomes C's right child
    }

    // 4. Update heights bottom-up (A and B moved down, update them before C)
    updateHeight(static_cast<AVLTreeNode<T>*>(A));
    updateHeight(static_cast<AVLTreeNode<T>*>(B));
    updateHeight(static_cast<AVLTreeNode<T>*>(C));
}

// !!! NOTE: BALANCE PATH FUNCTION MUST BE EDITED TO CALL THESE NEW COMBINED ROTATION FUNCTIONS, SEE BELOW !!!
/* edit switch block, keep the rest the same

case -2:  // left-heavy
    if (balanceFactor(static_cast<AVLTreeNode<T>*>(((*A).left))) <= 0)
        singleRotate(A, parentOfA, true);    // was balanceLL
    else
        doubleRotate(A, parentOfA, true);    // was balanceLR
    break;
case +2:  // right-heavy
    if (balanceFactor(static_cast<AVLTreeNode<T>*>(((*A).right))) >= 0)
        singleRotate(A, parentOfA, false);   // was balanceRR
    else
        doubleRotate(A, parentOfA, false);   // was balanceRL

*/

#endif