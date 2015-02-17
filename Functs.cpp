//Brad Gray bg22946
//EE312 SU2014
//Project8
//Functs.cpp
#include "MemHeap.h"
#include "Functs.h"
#include <stdio.h>
//#include "Instruct.h"

void FunctBST::StoreFunction(int32_t vectorLocation, String& FunctName, String& Parameters){
	functNode* newF = new functNode;
	functNode* parent = 0;
	newF -> functName = FunctName;
	newF -> vLoc = vectorLocation;
	newF -> parameters = Parameters;
	length+=1;
	if(functRoot==0){
		functRoot = newF;
		return;
	}
	functNode* temp = functRoot;
	    while(temp){ //Find the parent to the variable - keep going as long as temp is not null
        parent = temp;
        if(FunctName < temp->functName){
         	 temp = temp->left;
        }else if (FunctName > temp->functName){
        	temp = temp->right;
        }else {
        	printf("WARNING: Function %s already defined\n", FunctName.c_str());
        	return;
    	}

    }
    //Place the address of the new variable in the parents left or right child fields
    if(FunctName < parent->functName){
        parent->left = newF;
    }
    else{ parent->right = newF;  }
}

int32_t FunctBST::ExtractFunction(String& name, String& parameters, functNode* root){
	if(root==0){
		printf("\nFunction %s does not exist.\n", root->functName.c_str());
		return 0;
	}
    if(root->functName==name){
    	parameters = root->parameters;
		return root->vLoc;
	}
	if(name < root->functName){
		return ExtractFunction(name, parameters, root->left);
	}else{
        return ExtractFunction(name, parameters, root->right);
    }  
}

int32_t FunctBST::FindVector(String& name, String& parameters){
	return ExtractFunction(name, parameters, this->functRoot);
}

void FunctBST::recDelete(functNode* root) {
  	if (!root) { return; }
	recDelete(root->left);
   recDelete(root->right);
   delete root;
}
