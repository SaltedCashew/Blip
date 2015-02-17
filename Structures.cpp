//Brad Gray bg22946
//EE312 SU2014
//Project7
//PhaseA.cpp 
//Define Variable BST and Expression Tree
#include "MemHeap.h"
#include <iostream>
#include "Structures.h"
#include "Parse.h"
#include "Instruct.h"


void Insert_ExpVector(ExpVector* v, ExpTree* t){
    
  if (v->capacity == v->length) {
        v->capacity *= 2;
        if (v->capacity == 0) { v->capacity = 1; }
        ExpTree* p = new ExpTree[v->capacity];
        for (uint32_t k = 0; k < v->length; k += 1) {
            p[k] = v->data[k];
        }
        delete[] v->data;
        v->data = p;
        
    }
    int32_t insertLoc = v->length;
    v->data[insertLoc] = *t;
    v->length += 1;
    if(0){};

}

// Smaller elements go left
// larger elements go right
void VariableBST::CreateVar(String& var)
{
    varNode* newVar = new varNode;
    varNode* parent = 0;
    newVar->varName = var;
    length+=1;
    if(isEmpty()){ 
    	varRoot = newVar;    //if it's a new tree, the variable we're creating is now the root //add return here instead of the else?
        return;
    }
    varNode* temp = varRoot;
    while(temp){ //Find the parent to the variable - keep going as long as temp is not null
        parent = temp;
        if(var < temp->varName){
         	 temp = temp->left;
        }else {temp = temp->right;}
    }
    //Place the address of the new variable in the parents left or right child fields
    if(var < parent->varName){
        parent->left = newVar;
    }
    else{
        parent->right = newVar;
    }
    newVar->parent=parent;
}

int32_t& VariableBST::ExtractVarData(varNode* root, String& var){
	//need to find node with varName == var
	//base cases.... if root is null, then nothing to search for. if varName==var then return pointer/reference to the data
	if(root==0){
		printf("\nVariable %s does not exist.\n", var.c_str());
		return root->data;
	}
    if(root->varName==var){
		return root->data;
	}
	if(var > root->varName){
		return VariableBST::ExtractVarData(root->right, var);
	}else{
        return VariableBST::ExtractVarData(root->left, var);
    }  

}


// enable use of database[var] and returns a reference to the variable's data
// Used on LHS to search for/add variables
int32_t& VariableBST::operator[](String& var){
	return VariableBST::ExtractVarData(varRoot, var);
}

int32_t& VariableBST::GetVarVal(String& var){
    return VariableBST::ExtractVarData(varRoot, var);
}


bool VariableBST::IsMember(varNode* root, String& var){
	if(root==0){return false;}
	if(root->varName==var){
			return true;
	}
	if(var > root->varName){
		if(root->right==NULL){
			return false;
		}else {	return VariableBST::IsMember(root->right, var); }
	}else{
		if(root->left==NULL){
			return false;
		}else {	return VariableBST::IsMember(root->left, var); }
	}
}

bool VariableBST::CheckVar(String& var){
   if (this==0){return false;}
	return IsMember(varRoot, var);
}	

void VariableBST::recursiveDelete(varNode* root) {
    if (!root) { return; }

    recursiveDelete(root->left);
    recursiveDelete(root->right);
    delete root;
}

opType ExpTree::DetermineOpType(String& b){
    char c = '%';
    if (b == String("+")){ return ADD;    }  
    if (b == String("-")){return MINUS;   }
    if (b == String("*")){return MULT;    }
    if (b == String("/")){return DIVIDE;    }
    if (*(b.c_str()) == c){return MOD;   }
    if (b == String("&&")){return BINAND; }
    if (b == String("||")){return BINOR;  }
    if (b == String("!")) { return NEGATE; }
    if (b ==String("~")) {return INVERSE;  }
    if ( b == String("<") || b == String(">") || b == String("==") || b == String("!=") || b == String("<=") || b == String(">=") ) {
        return COMP;
    }
    return NOP;
}

CompType ExpTree::DetermineCompType(String& b){
    if (b == String(">")){ return GT;    }  
    if (b == String("<")){return LT;   }
    if (b == String("==")){return EQ;    }
    if (b == String("!=")){return NOTEQ;    }
    if (b == String("<=")){return LTE; }
    if (b == String(">=")){return GTE;  }
    return NOCOMP;
}


ExpTree::expNode* ExpTree::BuildTree(String& b, expNode* root){
    
    expNode* temp = new expNode; 
    //add call expression. will have two children. left child is the name of the function and the right child is a vector of the arguments


    if (next_token_type == SYMBOL){
        temp->expr = OPER;
        temp->oper = DetermineOpType(b);
        if(temp->oper == COMP){
            temp->comp = DetermineCompType(b);
        }
    }else if (next_token_type == NUMBER){
        temp->expr = NUM;
        temp->data = token_number_value;
    }    else if(next_token_type==NAME && b==String("call")){
        temp->expr = CALL;
    }else if (next_token_type == NAME && b!=String("call")){
        temp->expr = VARIABLE;
        temp->varName = b;
    }
                           
     
    if (temp->expr == OPER && temp->left==0 ){
        read_next_token();
        
    }


    String tempStr = String(next_token());
    if(temp->expr == CALL){
        read_next_token();
        temp->varName = String(next_token()); //this should be the functions name
        ExpVector* newV = new ExpVector;
        temp->Arguments = newV; 
        read_next_token(); //this should be "args"
        read_next_token(); //this should be the first argument
        tempStr = String(next_token());

        while(tempStr!=String("sgra")){
          ExpTree* newT = new ExpTree;
          newT->GatherExp(tempStr);
          Insert_ExpVector(newV, newT);
          read_next_token(); //this should be the next argument
          tempStr = String(next_token());
        }  
    }

    if (temp->expr == OPER && temp->left == 0){
        temp->left = BuildTree( tempStr , temp->left);
    }
    if (temp->expr == OPER && temp->oper!=INVERSE && temp->oper!=NEGATE && temp->left != 0){
        read_next_token();
        String tempStr = String(next_token());
        temp->right = BuildTree( tempStr , temp->right);
    }

        expRoot = temp;
     

 return temp;


}

void ExpTree::GatherExp(String& b){
    ExpTree::BuildTree(b, this->expRoot);
}


int32_t ExpTree::Comp(CompType comp, int32_t t1, int32_t t2){
    int32_t data=0;

    if (comp == LT){
        data = t1<t2;

    }else if (comp == GT){
        data = t1>t2;
    }else if (comp == GTE){
        data = t1>=t2;
    }else if (comp == LTE){
        data = t1<=t2;
    }else if (comp == NOTEQ){
        data = t1!=t2;
    }else if (comp == EQ){
        data = t1==t2;
    }
    return data;  

}

int32_t ExpTree::ReadTree(expNode* root, InstrStore* InsDB, VariableBST& Local){
    int32_t data=0;
    int32_t t1 = 0;
    int32_t t2 = 0;
    if (root->expr == NUM){
        return root->data;
    }
    if (root->expr == CALL){
        return InsDB->InstrStore::ProcessCall(root->varName, root->Arguments, Local);
    }
    if (root->expr == VARIABLE){
        if(Local.VariableBST::CheckVar(root->varName)){
          data =  Local.VariableBST::GetVarVal(root->varName);
        }
        else if(InsDB->VarDB.VariableBST::CheckVar(root->varName)){  //check to ensure the variable is in the symbol tree
         data =  InsDB->VarDB.VariableBST::GetVarVal(root->varName);
        }
      else{
        printf("Variable %s NOT declared ", root->varName.c_str());
    }
        return data;
    }
    if (root->left){ t1 = ReadTree(root->left, InsDB, Local); }
    if (root->oper != INVERSE && root->oper != NEGATE && root->expr != CALL){t2 = ReadTree(root->right, InsDB, Local); }
    if (root->oper == ADD){
        data = t1+t2;
        return data;
    }
    else if (root->oper == MINUS){
        data = t1-t2;
    }else if (root->oper == MULT){
        data = t1*t2;
    }else if (root->oper == DIVIDE){
        data = t1/t2;
    }else if (root->oper == MOD){
        data = t1%t2;
    }else if (root->oper == BINAND){
        data = t1 && t2;
    }else if (root->oper == BINOR){
        data = t1 || t2;
    }else if (root->oper == COMP){
        data = Comp(root->comp, t1, t2);
    }else if (root->oper == INVERSE){
        data = -t1;
    }else if (root->oper == NEGATE){
      if (t1!=0){t1=0;}
      else {t1=1;}
      data = t1;
    }

    return data;  
    
}

 int32_t ExpTree::ReadExp(InstrStore* InsDB, VariableBST& Local){
    return ExpTree::ReadTree(this->expRoot, InsDB, Local);
 }

void ExpTree::recursiveDelete(expNode* root) {
    if (!root) { return; }

    recursiveDelete(root->left);
    recursiveDelete(root->right);
    delete root;
}

void ExpTree::deleteExp(void){
    recursiveDelete(this->expRoot);
}