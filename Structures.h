//Brad Gray bg22946
//EE312 SU2014
//Project7
//PhaseA.h 
//Variable Struct and Expression Struct
#ifndef _Structures_h
#define _Structures_h 1
#include "MemHeap.h"

#include "String.h"
#include <stdint.h>


class VariableBST
{
    private:
        struct varNode
        {
           varNode* left;
           varNode* right;
           varNode* parent;
           String varName;
           int32_t data;
           varNode(){ //constructor
            data=0;
            left=NULL;
            right=NULL;
            parent=NULL;
            varName = String("");    
           } 
        }; //destructor taken care of with Variable BST recursive delete
        varNode* varRoot;
        uint32_t length; 
        int32_t& ExtractVarData(varNode* root, String& var);
 		    bool IsMember(varNode* root, String& var);
        varNode* smallest(varNode* root);
        VariableBST::varNode* successor(varNode* vnode);
       void recursiveDelete(varNode* root);
        


    public:
        VariableBST(){ 
          varRoot = NULL;
          length=0;
        }
        ~VariableBST(){
        recursiveDelete(varRoot); 
        }

        int32_t& GetVarVal(String& var);
        bool isEmpty() const { return varRoot==NULL; }
        int32_t& operator[](String& v);
        void CreateVar(String& v);
        bool CheckVar(String& v);
        void PrintVars(void);
        int32_t numVars(void);
};

enum opType {
  ADD,
  MINUS,
  MULT,
  DIVIDE,
  BINAND,
  BINOR,
  MOD,
  COMP,
  INVERSE,
  NEGATE,
  NOP
};

enum CompType{
  GT,
  LT,
  EQ,
  NOTEQ,
  GTE,
  LTE,
  NOCOMP
};

enum ExpType {
  OPER,
  NUM,
  VARIABLE,
  CALL,
  FUN,
  INV
};


class InstrStore;
struct ExpVector;


class ExpTree
{
private:
 

  struct expNode{
    expNode* left;
    expNode* right;
    expNode* parent;
    opType oper;
    ExpType expr;
    CompType comp;
    int32_t data;
    String varName;
    ExpVector* Arguments; //a vector containing pointers to ExpTrees
    expNode(){ //constructor
      data=0;
      oper= NOP;
      expr= INV;
      comp = NOCOMP;
      left=NULL;
      right=NULL;
      parent=NULL;
      varName = String("");    
    } 
  };

  
    
  expNode* expRoot;
  uint32_t length; 
  expNode* BuildTree(String& b, expNode* root);   
  void ReadTree(String& b);
  opType DetermineOpType(String& b);
  bool DetermineContinue(String& b);
  int32_t ReadTree(expNode* root, InstrStore* pleaseWork, VariableBST& Local);
  CompType DetermineCompType(String& b);
  int32_t Comp(CompType comp, int32_t t1, int32_t t2);

public:
  ExpTree(){ 
    expRoot = NULL;
    length=0;
  }
  ~ExpTree(){

   //delete this->expRoot;  This causes segmenation faults, as does a recursive delete
  }
  int32_t ExtractNumFromString(String& b);
  bool isEmpty() const { return expRoot==NULL; }
  void GatherExp(String& b);
  int32_t ReadExp(InstrStore* pleaseWork, VariableBST& Local);
  void recursiveDelete(expNode* root); 
  void deleteExp(void);
};

 struct ExpVector{
      uint32_t length;
      uint32_t capacity;
      ExpTree* data;

      ExpVector(void) {
        length = 0;
        capacity = 0;
        data = nullptr;
      }
      void copy(const ExpVector& x) {
        this->length = x.length;
        this->capacity = x.length;
        if (x.length == 0) { this->data = nullptr; }
        else {
          this->data = new ExpTree[this->capacity];
        }
        for (uint32_t k = 0; k < x.length; k += 1) {
          this->data[k] = x.data[k];
        }
      }

      void destroy(void) {
        delete[] this->data;
      }

      ExpVector(const ExpVector& that) {
        copy(that);
      }

      ~ExpVector(void) {
        destroy();
      }

      ExpVector& operator=(const ExpVector& that) {
        if (this != &that) { 
          destroy();
          copy(that);
        }
        return *this;
      }
    }; 



#endif /* _Structures_h */
