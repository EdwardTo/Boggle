#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <string>
#include <vector>

using namespace std;

/*
 * bNode class
 */
class bNode{

public: 

  bool visited;
  string name;
  unsigned int position;
  int xrow;
  int ycol;
  bool revisited;

  bNode(string &name, int xrow, int ycol); 

  void setVisited(bool v);

  bool wasVisited();
  
  int getPositionX();

  int getPositionY();

  const string &getName() const;
 
  bool wasRevisited();

  void setRevisited(bool r);

};

//end bNode class

/*
 * TernaryTrieNode class
 */

class TernaryTrieNode{

  public:
    char data;
    bool end;
    TernaryTrieNode *left, *right, *center;

    TernaryTrieNode(char &data);

};

//end TernaryTrieNode class

/*
 * TernaryTrie class
 */

class TernaryTrie{
    
  public:
    TernaryTrieNode* root;
    TernaryTrie(){root = nullptr;};
    ~TernaryTrie();
    void clear(TernaryTrieNode* node);
    void add(string str);
    bool find(string str);
    bool isPrefix(string str);

};

//end TernaryTrie class
#endif
