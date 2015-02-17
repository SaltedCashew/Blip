//Brad Gray bg22946
//EE312 SU2014
//Project8
//Instruct.h 
//Instruction Store - Reading/storing the input data from .blip file and executing instructions from storage
#ifndef _Instruct_h
#define _Instruct_h 1

#include "String.h"
#include "Parse.h"
#include "Structures.h"
#include "Functs.h"
#include <stdint.h>



enum InstrType{
  instrVar,
  num,
  expr,
  funct,
  noInstr,
  RET
};


 class InstrStore{
  private: 
    struct InstNode{
      TokenType tokenType; //should be NAME, SYMBOL, END or INVALID (SYMBOL used for //, #, !, ~)
      String commandType;    //will be "text", "var", "output", or "set"
      InstrType instrType;   //notes if the instruction after the command is a variable, integer, expression string
      String text;
      String var;
      String Parameters;
      SVector* MultiPass;
      ExpTree* expressions;
      int32_t data;
    InstNode(){
      tokenType=INVALID;
      commandType = String(" ");
      text = String(" ");
      var = String(" ");
      instrType = noInstr;
      expressions = 0;
      data = 0;
    }
    ~InstNode(){
      //delete this->MultiPass;
      //delete this->expressions;
    }
   }; 

   struct Vector{
      uint32_t length;
      uint32_t capacity;
      InstNode* data;

      Vector(void) {
        length = 0;
        capacity = 0;
        data = nullptr;
    }
      void copy(const Vector& x) {
        this->length = x.length;
        this->capacity = x.length;
        if (x.length == 0) { this->data = nullptr; }
        else {
          this->data = new InstNode[this->capacity];
        }
        for (uint32_t k = 0; k < x.length; k += 1) {
            this->data[k] = x.data[k];
        }
    }

      void destroy(void) {
        delete[] this->data;
      }

      Vector(const Vector& that) {
        copy(that);
      }

     ~Vector(void) {
        destroy();
      }

     Vector& operator=(const Vector& that) {
        if (this != &that) { 
            destroy();
            copy(that);
        }
        return *this;
     }

    };


    InstNode* GrabInstruction(void);
    void ProcessInstruction(InstNode* next);
    void ProcessSymbol(String sym);
    void ProcessOutput(InstNode* p, VariableBST& Local);
    void ProcessSetVar(InstNode* p, VariableBST& Local);
    int32_t ProcessDoLoops(int32_t start, VariableBST& Local);
    int32_t ProcessIfs(int32_t start, VariableBST& Local);
  public: 
    Vector* Instructions; //contains pointers to InstNodes
    VariableBST VarDB;    //BST storing all the global variables
    FunctBST FunctDB;     //BST storing all the function locations & parameters
    int32_t c2;  //used for caller control
    int32_t rVal;
    ~InstrStore(){
      /*
      //----------------Working on Destructor Code------------------------------//
      //This doesn't seem to help - need more testing to ensure it doesn't hurt--//
      for(uint32_t k=0; k<Instructions->length;k+=1){
        if(Instructions->data[k].expressions){
          Instructions->data[k].expressions->ExpTree::deleteExp(); 
          delete Instructions->data[k].expressions;}
    //    if(Instructions->data[k].MultiPass){delete Instructions->data[k].MultiPass;}
        
      }
      delete this->Instructions;
    //-----------------------------End of Desructor Code---------------------------*/   
    }
    void BuildInstructions(void);
    uint32_t Insert(InstNode* v);  //can this be moved to private?
    //void pop_back(void);  pretry sure this isn't needed/used
    int32_t ExecuteInstructions(int32_t start, int32_t stop, VariableBST& Global, VariableBST& Local);
    int32_t ProcessCall(String& fName, ExpVector* vr, VariableBST& Local);
    void RunInstructions(void);
    

    
};



#endif /* _Instruct_h */
