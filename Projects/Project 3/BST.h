// CSMC 341 - Spring 2020 - Project 3

// A balanced BST using array-based rebalancing and a user-provided
// imbalance function.

#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <string>

// ********************************************
// Additional 'using' statements may be added.
// ********************************************

using std::string;
using std::cout;
using std::endl;

// Typedef for a balance function pointer.  A balance function returns
// true if the tree is **imbalanced** at a node.  This allows you to
// try different balance criteria.
typedef bool (*balfn_t)(int leftHeight, int rightHeight, int leftSize, int rightSize);

class BST { // Balanced BST class

public:
  
  class BNode{ // BST Node inner class
  public:
    BNode(string ch = "", int rank = -1, int height = 0, int size = 1);
    string _data;   // holds string data
    int _key;       // integer key value
    int _height;    // height of the node
    int _size;      // size of the subtree rooted at the node
    BNode* _right;  // right child pointer
    BNode* _left;   // left child pointer
  };

  //
  // Constructor, Copy Constructor, Assignment Operator, Destructor
  //

  // Constructor.  Must pass a pointer to a balanced function.
  BST(balfn_t imbalanced);
  
  // Copy constructor
  BST(const BST& rhs);

  // Assignment operator
  BST& operator=(const BST& rhs);

  // Destructor
  ~BST();                   

  //
  // Basic functions
  //
  
  // Insert (key, data) pair in the tree
  bool insert(string data, int key); 

  // Size of the tree (number of nodes)
  int size() const;

  // Height of the tree
  int height() const; 

  // Print the data using an inorder traversal; if verbose is 'true',
  // print the tree structure including sizes, heights, and
  // parentheses.
  void dump(bool verbose = false) const;
  
 private:
  BNode* _root;        // root of the tree
  balfn_t _imbalanced; // function to check for imbalance

  // ********************************************
  // Private function declarations go here!
  // ********************************************

  //dumps the entire tree using inorder traversal
  void inorderDump(BNode* currentNode, bool verbose) const;
  //runs through the tree from currentNode and deletes that subtree. Uses recursion
  void deleteTree(BNode* currentNode = nullptr);
  //recreates the tree from the root passed, mainly for assignment and copy constructor
  BNode* buildTree(BNode* copyNode); //preorder
  //recursive function for inserting into a binary search tree
  bool recursiveInsert(string ch, int rank,  BNode*& compareNode);
  //small function to do the entire process of rebalancing
  BNode* getBalancedTree(BNode* startNode);
  //update the sizes of subtrees
  int updateSizes(BNode* currentNode);
  //update the heights of subtrees
  int updateHeights(BNode* currentNode, int height);
  //helper function to make sure all nodes update their height
  void updateHeightHelper(BNode* currentNode);
  //puts all elements of the BST into an array
  void decompose(BNode* currentNode, int keys[], string data[]);
  //balances keys
  void balanceKeys(int keys[], string data[], int length);
  //takes sorted array and puts elements back into a BST
  BNode* backToBST(int lowerBound, int upperBound, int keys[], string data[]);

};


#endif
