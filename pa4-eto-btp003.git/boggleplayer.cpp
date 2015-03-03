#include "boggleplayer.h"
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stack>
#include "boggleutil.h"

using std::vector;
using std::set;
using std::string;

/*
 * creates a new board with the input rows, cols, and array to be used 
 * as blocks for the letters
 */
  void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, 
   string** diceArray){
    //if board has been called already, clear old one 
    if(setBoardCalled == true){
      for(unsigned int i = 0; i < board.size(); i++){
        for(unsigned int j = 0; j < board[i].size(); j++){
          delete board[i][j];
          board[i][j] = nullptr;
        }
      }
    }
    unsigned int pos;
    string s;
    board.resize(rows);
    //create 2d array
    for(unsigned int r = 0; r < rows; ++r){
      board[r].resize(cols);
    }
    width = cols;
    height = rows;
    //create board with input characters and sets all characters to lower case
    for(unsigned int r = 0; r < rows; ++r){
      for(unsigned int c = 0; c < cols; ++c){
        s = diceArray[r][c];
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        bNode *b = new bNode(s, r, c);
        b->setVisited(false);
        board[r][c] = b;
      }
    }
    setBoardCalled = true;
  }

/*
 * checks to see if the word given is on the board
 */
  vector<int> BogglePlayer::isOnBoard(const string& word_to_check){
    vector<int> v;
    std::stack<bNode*> nStack;
    bNode* vNode;
    unsigned int pos = 0;
    string s = word_to_check;
    string word;
    int nLoc;
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    //set all visited flags to false, push letters on stack to search through
    //later
    for( unsigned int j = 0; j < board.size(); j++){
      for( unsigned int k = 0; k < board[j].size(); k++){
        board[j][k]->setVisited(false);
        board[j][k]->setRevisited(false);
        nStack.push(board[j][k]);
      }
    }
    //continue looping through stack searching to see if the letter matches
    //the letter in the word
    while( !nStack.empty() ){
      vNode = nStack.top();
      nStack.pop();
      //if node was already visited, set to false to backtrace
      if(vNode->wasRevisited() == true){
        vNode->setRevisited(false);
        vNode->setVisited(false);
        pos = pos-vNode->getName().length();
        v.pop_back();
      }
      //if node was not visited then check to see if it matches the next letter
      //in the word then check the words neighbors
      else if(!vNode->wasVisited() && vNode->getName() == s.substr(pos, vNode->getName().length())){
        vNode->setVisited(true);
        pos = pos + vNode->getName().length();
        v.push_back(vNode->getPositionX()*width + vNode->getPositionY());
        if(pos == s.length())
          break;
        vNode->setRevisited(true);
        nStack.push(vNode);
        //look through neighbors
        for( int m = -1; m < 2; m++ ){          
          for( int n = - 1; n < 2; n++){
            nLoc = (vNode->getPositionX() + m) * width + (vNode->getPositionY() + n);
            if(m != 0 || n != 0 ){
              if(nLoc >= 0 && nLoc < height*width ){
                if(vNode->getPositionX()+m >= 0 && vNode->getPositionX()+m < 
                   height && vNode->getPositionY()+n >= 0 &&
                   vNode->getPositionY()+n < width){
                  if( board[vNode->getPositionX()+m]
                     [vNode->getPositionY()+n]->wasVisited() == false){
                    nStack.push(board[vNode->getPositionX()+m]
                     [vNode->getPositionY()+n]);
                  }
                }
              }      
            }     
          }
        }
      }
    }    
    return v;
  }
/*
 * builds the lexicon in a ternaryTrie with the input set of words.
 */
  void BogglePlayer::buildLexicon(const set<string>& word_list){  
    //if lexicon is already built, destroy the old one
    if(buildLexCalled == true){
      delete p1;
    }
    p1 = new TernaryTrie();
    set<string>::iterator it;
    vector<string> str;
    vector<string>::iterator it1;
    //push words onto a vector to shuffle for later
    for(it = word_list.begin(); it!=word_list.end(); ++it){
      str.push_back(*it);
    }
    //shuffles the words
    random_shuffle(str.begin(), str.end()); 
    //puts it into the ternarytrie lexicon
    for(it1 = str.begin(); it1!=str.end(); ++it1){
      p1->add(*it1);
    }
    buildLexCalled = true;
  }

/*
 * gets all the valid words on the board that is of specified minimum length
 * and compares it to all the words in the lexicon. If the word is in the 
 * lexicon, it puts it into the set of words.  
 */
  bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words){
    //if setboard and buildlexicon has not been called then do not run
    if(setBoardCalled == false || buildLexCalled == false){
      std::cerr << "Error: board or lexicon has not been made." << std::endl;
      return 0;
    }
    std::stack<bNode*> bStack;
    bNode* gNode;
    string prefix = "";
    int min = minimum_word_length;
    vector<string> pVect;
    int nLoc;
    //push all the letters of the board onto a stack to use to search through
    //sets all visited flags to false
    for(unsigned int i = 0; i < board.size(); i++){
      for(unsigned int j = 0; j < board[i].size(); j++){
        board[i][j]->setVisited(false);
        board[i][j]->setRevisited(false);
        bStack.push(board[i][j]);
     }
    }
    //goes through the stack searching to see if it is a prefix of a word 
    //in the lexicon then push it into the set of words. goes until stack is 
    //empty
    while(!bStack.empty()){
      gNode = bStack.top();
      bStack.pop();
      //if node was already visited, set flags to false and put back into
      //vector
      if(gNode->wasRevisited() == true){
        gNode->setRevisited(false);
        gNode->setVisited(false);
        pVect.pop_back();
      }
      //if node wasn't visited yet
      else if(!gNode->wasVisited()){
      pVect.push_back(gNode->getName());
      prefix = "";
      for(int i = 0; i < pVect.size(); i++)
        prefix = prefix + pVect[i];
        //if node is prefix then keep going
        if(p1->isPrefix(prefix)){
          gNode->setVisited(true);
          //if node satisfies the length and is prefix then insert into set
          if(min <= prefix.length() && p1->find(prefix)){
            words->insert(prefix);
          }
          gNode->setRevisited(true);
          bStack.push(gNode);
          //get the nodes neighbors to check to see if they are prefixes
          for(int m = -1; m < 2; m++){
            for(int n = -1; n < 2; n++){
              nLoc = (gNode->getPositionX() + m) * width + (gNode->getPositionY() + n);
              if(m != 0 || n != 0){
                if(nLoc >= 0 && nLoc < height * width){
                  if(gNode->getPositionX() + m >= 0 && gNode->getPositionX() + m < height
                     && gNode->getPositionY() + n >= 0 && gNode->getPositionY() + n < width){
                    if(board[gNode->getPositionX()+m][gNode->getPositionY()+n]->wasVisited() == false){
                      bStack.push(board[gNode->getPositionX()+m][gNode->getPositionY()+n]);
                    }
                  }
                }	
              }
            }
          }
        }
        //clear the word if it is not a prefix
        else{
          if(prefix != ""){
            pVect.pop_back();
            prefix = "";
          }
        }
      }
    }
    return 1;
  }

/*
 * checks to see if the word is in the lexicon
 */
  bool BogglePlayer::isInLexicon(const string& word_to_check)
  {
    return (p1->find(word_to_check));
  }


  void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols){}

/*
 * destructor for boggleplayer board
 * delets all nodes, sets the pointers to null
 * delets the trie
 */
  BogglePlayer::~BogglePlayer(){
    for(unsigned int i = 0; i < board.size(); i++){
        for(unsigned int j = 0; j < board[i].size(); j++){
          delete board[i][j];
          board[i][j] = nullptr;
        }
      }
    delete p1;
  }


