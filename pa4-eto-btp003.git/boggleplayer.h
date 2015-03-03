#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include <set>
#include <vector>
#include <string>
#include "baseboggleplayer.h"
#include "boggleutil.h"

using std::vector;
using std::set;
using std::string;

class BogglePlayer : public BaseBogglePlayer {
  public:
  unsigned int width;
  unsigned int height;
  void buildLexicon(const set<string>& word_list);
  void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
  bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
  bool isInLexicon(const string& word_to_check);
  vector<int> isOnBoard(const string& word_to_check);
  void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);
  ~BogglePlayer();
  vector<vector<bNode*>> board;
  TernaryTrie *p1; 	
  
  private:
  bool setBoardCalled = false;
  bool buildLexCalled = false;
  
};



#endif
