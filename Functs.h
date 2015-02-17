//Brad Gray bg22946
//EE312 SU2014
//Project8
//Functs.h

#ifndef _Functs_h
#define _Functs_h 1
#include "MemHeap.h"
#include "String.h" 



class FunctBST
{
private:
   struct functNode{
      functNode* left;
      functNode* right;
      String functName;
      String parameters;
      int32_t vLoc;
      functNode(){ //constructor
         vLoc=0;
         left=NULL;
         right=NULL;
         functName = String(" ");  
         parameters= String(" ");  
      } 
   };

   functNode* functRoot;
   uint32_t length;   
   int32_t ExtractFunction(String& name, String& parameters, functNode* root); 

public:

   FunctBST(){ 
      functRoot = NULL;
      length=0;
   }
   
   ~FunctBST(){
      recDelete(functRoot); 
   }

   void recDelete(functNode* root);  
   void StoreFunction(int32_t vectorLocation, String& FunctName, String& Parameters);
   int32_t FindVector(String& name, String& parameters);
};



#endif /* _Functs_h */