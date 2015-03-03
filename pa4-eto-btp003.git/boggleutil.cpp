#include <iostream>
#include "boggleutil.h"

using namespace std;

/*
 * bNode class
 */

/*
 * constructor for bnode
 */
  bNode::bNode(string &name, int xrow, int ycol): 
    name(name), xrow(xrow), ycol(ycol) {}
  
/*
 * setter to set the visited flag
 */
  void bNode::setVisited(bool v){
    visited = v;
  }
  
/*
 * getter to check visited flag
 */
  bool bNode::wasVisited(){
    return visited;
  }

/*
 * getter to get the x position of the node in the board
 */
  int bNode::getPositionX(){
    return xrow;
  }
  
/*
 * getter to get the y position of the node in the board
 */
  int bNode::getPositionY(){
    return ycol;
  }

/*
 * getter to get the name of the node
 */
  const std::string & bNode::getName() const{
    return name;
  }
 
/*
 * getter to check to see if the node was revisited
 */
  bool bNode::wasRevisited(){
    return revisited;
  }
 
/*
 * setter to set the revisited flag
 */
  void bNode::setRevisited(bool r){
    revisited = r;
  }
  
//end bNode class

 /*
 * TernaryTrieNode class
 */

/*
 * TernaryTrieNode constructor
 */
  TernaryTrieNode::TernaryTrieNode(char &data): data(data){
    end = 0;
    left = right = center = nullptr; 
  }



//end TernaryTrieNode class

/*
 * TernaryTrie class
 */

/*
 * add method for ternarytrie.
 * it adds the string into the ternarytrie. 
 */
  void TernaryTrie::add(string str){
    TernaryTrieNode* currNode;
    //if there is no word in tree then add word to the root
    if(root == nullptr){
      root = new TernaryTrieNode(str[0]);
      currNode = root;
      //add it into the tree going straight down the center
      for(int i = 1; i < str.length(); i++){
        currNode->center = new TernaryTrieNode(str[i]);
        currNode = currNode->center; 
        if(i+1 == str.length()){
          currNode->end = 1;    
        }   
      }
      //set end bit to one if there is only one letter word
      if(str.length() == 1)
        currNode->end = 1; 
    } 
    //if there is a root then make other branches
    else{
      currNode = root;
      for(int i = 0; i < str.length();){
        //if the word is inserted completely set end marker to 1
	if(str[i] == currNode->data){
          if(str.length()-1 == i){
            currNode->end = 1;
            break;
          }
          //if word length is one then set end bit to 1
          if(str.length() == 1) 
            currNode->end = 1; 
          //move the pointer down if there is center child is not null
          //to get to the end of the word to add a new letter
          if(currNode->center != NULL){
           currNode = currNode->center;
           i++; 
          }
          //if there is no center child make the new node with the letter
          else if(currNode->center == NULL){
            currNode->center = new TernaryTrieNode(str[i+1]);
            currNode = currNode->center;
            i++;
          }
        } 
        //move pointer to the left if the letter is less than the current node
        else if(str[i] < currNode->data){
          if(currNode->left == NULL){
            currNode->left = new TernaryTrieNode(str[i]);
          }
          currNode = currNode->left;
        }
        //move the pointer to the right if hte letter is greater than the current node
        else if(str[i] > currNode->data){
          if(currNode->right == NULL){
            currNode->right = new TernaryTrieNode(str[i]);
          }
          currNode = currNode->right;
        }
      }
    }
  }

/*
 * find the string in the ternaryTrie
 */
  bool TernaryTrie::find(string str){
    TernaryTrieNode* currN;
    currN = root;
    for(int i = 0; i < str.length();){
      //if the tree does not exist or if the word does not exist return false
      if(currN == nullptr || i >= str.length())
        return false;
      //if the word is found return true
      if(currN->end == 1 && i + 1 == str.length() && currN->data == str[i]){
        return true;
      }
      //move through the tree to make the pointer point to the right place
      if( str[i] < currN->data )
        currN = currN->left;
      else if(str[i] > currN->data)
        currN = currN->right;
      else{
        i = i + 1;
        currN = currN->center;
      }
    }
    return false;
  }

/*
 * checks to see if the word given is a prefix in the lexicon
 */
  bool TernaryTrie::isPrefix(string str){
    TernaryTrieNode* cNode;
    cNode = root;
    for(int i = 0; i < str.length();){
      //if there is no tree or if there is no word return false
      if(cNode == NULL || i >= str.length())
        return 0;
      //return true if it is a prefix
      if(i + 1 == str.length() && cNode->data == str[i]){ 
        return 1;
      }
      //move pointer to the right place
      if(str[i] < cNode->data)
        cNode = cNode->left;
      else if(str[i] > cNode->data)
        cNode = cNode->right;
      else{
        i = i+1;
        cNode = cNode->center;
      }
    }
    return 0;
  }

/*
 * destructor for the TernaryTrie
 */
  TernaryTrie::~TernaryTrie(){
    clear(root);
  }

/*
 * helper for the destructor. clears all of the nodes in the ternarytrie
 */
  void TernaryTrie::clear(TernaryTrieNode* node){
    if(node == nullptr)
      return;

    clear(node->left);
    clear(node->center);
    clear(node->right);

    delete node;
  }

// end ternaryTrie class
