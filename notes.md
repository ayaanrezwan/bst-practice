# Binary Search Tree Notes

## Logic
- The left child will always be less than the parent, the right child will always be greater

## Implementation
- When creating a new node in a tree, always set the children to null
- When using a member function on a tree, if you call the function using a pointer, you need to use "->" instead of "." as you need to DEREFERENCE (get the information from the pointer then start the function)
- TreeNode is the class for just one node of the BST, BST is a class of its own
- VIRTUAL keyword -> allows for polymorphism, essentially allowing for multiple classes to use the same function names while ensuring the correct function is called at runtime (ie. insert BST vs insert AVL)
- Functions are repeated in BST private and public as a wrapper. The public is a clean, no parameter function, whereas the private function actually does the dirty work with a parameter and recursion.

## Notes to Self
- Make sure to be careful when you declare functions within scopes, they might be needed in an outer scope.
- Use helper functions as much as possible