//Brad Gray bg22946
//EE312 SU2014
//Project8
//PhaseA.cpp 
//Setup of Data Structures for phase A of Project 7
#include "MemHeap.h"
#include <iostream>
#include "Instruct.h"


uint32_t InstrStore::Insert(InstNode* vb){  //return the vector index of the node inserted
 if (this->Instructions->capacity == this->Instructions->length) {
        Instructions->capacity *= 2;
        if (Instructions->capacity == 0) { Instructions->capacity = 1; }
        InstNode* p = new InstNode[Instructions->capacity];
        for (uint32_t k = 0; k < Instructions->length; k += 1) {
            p[k] = Instructions->data[k];
        }
        delete[] Instructions->data;
        this->Instructions->data = p;
    }
    int32_t insertLoc = this->Instructions->length;
    this->Instructions->data[insertLoc] = *vb;
    Instructions->length += 1;
    return insertLoc;
}

void InstrStore::BuildInstructions(void){  
     Vector* x = new Vector;
     this->Instructions = x;
     InstNode* ptr;
     //uint32_t vectorptr=0;
    do{
       ptr = GrabInstruction();
       if(ptr){
       this->Insert(ptr);
       // vectorptr = this->Insert(ptr);
       } 
    }while(ptr);
    
}



InstrStore::InstNode* InstrStore::GrabInstruction(void){  
    read_next_token();
    if(next_token_type==END){return 0;} //we're done -- no more input file
    InstNode* newInst = new InstNode;
    newInst->tokenType = next_token_type; //this should always be NAME or SYMBOL- if not, we're doing something wrong
    String tempStr = String(next_token());

    if(newInst->tokenType == SYMBOL){
        
        newInst->text = tempStr;
        if(tempStr == String("//")){skip_line();}

        return newInst; 
    }

    newInst->commandType = tempStr;
  
    if (newInst->commandType == String("text")){
        read_next_token();
        tempStr = String(next_token());
        newInst->text = tempStr;
        return newInst;
    }
    else if (newInst->commandType == String("output") ){ 
        read_next_token();
        tempStr = String(next_token());
       if (next_token_type == SYMBOL || next_token_type == NUMBER || next_token_type == NAME){
            newInst->instrType = expr;
            ExpTree* newExpTree = new ExpTree;
            newExpTree->ExpTree::GatherExp(tempStr);
            newInst->expressions = newExpTree;
        }
    }else if (newInst->commandType == String("set") || newInst->commandType == String("var") ){
        read_next_token();
        tempStr = String(next_token());
        //next_token_type should certainly be a name
        newInst->var = tempStr;
        read_next_token();
        tempStr = String(next_token());
        newInst->instrType = expr;
        ExpTree* newExpTree = new ExpTree;
        newExpTree->ExpTree::GatherExp(tempStr);
        newInst->expressions = newExpTree;
   
    }else if (newInst->commandType == String("do") || newInst->commandType == String("if")){
      read_next_token();
      tempStr = String(next_token());
      newInst->instrType = expr;
      ExpTree* newExpTree = new ExpTree;
      newExpTree->ExpTree::GatherExp(tempStr);
      newInst->expressions = newExpTree;
    } else if (newInst->commandType == String("defun")){
        newInst->instrType = funct;
        read_next_token(); //should be name
        tempStr = String(next_token());
        newInst->text = tempStr; //text is now the function name
        read_next_token();//should be params 
        read_next_token();//should now be the first parameter
        tempStr = String(next_token());
        //------------------------making a vector of string parameters ------------------
        SVector* Scooby = new SVector; 
        while (tempStr != "smarap"){
          String* strPtr = &tempStr;
          Scooby->SVector::Insert_SVector(strPtr);
          read_next_token();
          tempStr = next_token();
        }
        newInst->MultiPass = Scooby;
        //----------------------------------------------------------------------------------------
    } else if (newInst->commandType == String("return")){
      newInst->instrType = RET;
      read_next_token();
      ExpTree* newExpTree = new ExpTree;
      tempStr = String(next_token());
      newExpTree->ExpTree::GatherExp(tempStr);
      newInst->expressions = newExpTree;
    }
    return newInst;    

}

void InstrStore::ProcessSymbol(String sym){
   if (sym==String("#") || sym==String("\n")){
      printf("\n");
   }

}

void InstrStore::ProcessOutput(InstNode* p, VariableBST& Local){

   if(p->instrType == expr){
      int32_t data = p->expressions->ExpTree::ReadExp(this, Local);
      printf("%d", data);
      return;
   }
 
}

void InstrStore::ProcessSetVar(InstNode* p, VariableBST& Local){
  if(p->commandType==String("var") && Local.VariableBST::CheckVar(p->var)){ //if it's a variable, does it already exist?
    printf("variable %s incorrectly re-initialized", p->var.c_str());
    return;
   }
  if(p->commandType==String("set") && &Local==&VarDB){
    if(not VarDB.VariableBST::CheckVar(p->var)){ //if trying to set a variable, does it already exist?
      printf("variable %s not declared", p->var.c_str());
      return;
    }
  }else if(p->commandType==String("set") && &Local!=&VarDB){
    if(not Local.VariableBST::CheckVar(p->var) && not VarDB.VariableBST::CheckVar(p->var)){ //if trying to set a variable, does it already exist?
      printf("variable %s not declared", p->var.c_str());
      return;
    }
  }    

  if(p->commandType==String("var")){
    Local.VariableBST::CreateVar(p->var);
  }

    int32_t t1 = 0;
    if(Local.VariableBST::CheckVar(p->var)){ 
      t1 = p->expressions->ExpTree::ReadExp(this, Local);
      Local.VariableBST::GetVarVal(p->var)=t1; 
    }else{ 
      VarDB.VariableBST::GetVarVal(p->var)=t1;
      t1 = p->expressions->ExpTree::ReadExp(this, this->VarDB);
    }

  return;
}

int32_t InstrStore::ProcessDoLoops(int32_t start, VariableBST& Local){
  InstNode* condition = this->Instructions->data+start;
  InstNode* inst;
  int32_t end = start;
  int32_t doctr = 1;
  int32_t odctr = 0;
  do{
    end+=1;
    inst = Instructions->data+end;
    if(inst->commandType==String("do")){ doctr += 1;}
    if(inst->commandType==String("od")){ odctr+=1;}
  }while(odctr<doctr);
  //printf("\nDo loop start at %d and end at %d\n", start, end);
  while(condition->expressions->ExpTree::ReadExp(this, Local)!=0){
        ExecuteInstructions(start+1, end, this->VarDB, Local);
   }
  return end;

}

int32_t InstrStore::ProcessIfs(int32_t start, VariableBST& Local){
  int32_t end = start;
  InstNode* condition = this->Instructions->data+start;
  InstNode* inst;        
  int32_t elseptr = 0;
  int32_t ifctr = 1;
  int32_t elsectr = 0;
  int32_t fictr = 0;
  do{
    end+=1;
    inst = Instructions->data+end;
    if(inst->commandType==String("if")){ ifctr+=1; }
    if(inst->commandType==String("else")){ 
    elsectr+=1;
    if(fictr==ifctr - 1){ elseptr = end; } 
    }
    if(inst->commandType==String("fi")){
      fictr+=1;
    }
  }while(fictr<ifctr);
  if(condition->expressions->ExpTree::ReadExp(this, Local)!=0){
    if (elseptr==0){elseptr=end;}

    rVal = ExecuteInstructions(start+1, elseptr, this->VarDB, Local);
  }else{
    if (elseptr!=0){  
      rVal = ExecuteInstructions(elseptr+1, end, this->VarDB, Local);  }
  }
return end;
}

//---------------------------Start ProcessCall working with Vector of Strings passed as parameters----------------
int32_t InstrStore::ProcessCall(String& fName, ExpVector* vr, VariableBST& Local){
    //find the vector location
    VariableBST* localVars = new VariableBST;
    String parameters; 
    SVector* JuicyFruit;
    int32_t vLoc = FunctDB.FunctBST::FindVector(fName, parameters); 
    JuicyFruit = this->Instructions->data[vLoc].MultiPass;
    for(uint32_t k = 0; k<JuicyFruit->length; k+=1){
      localVars->VariableBST::CreateVar(JuicyFruit->data[k]);
      ExpTree* temp = vr->data+k;
      int32_t t1 = temp->ExpTree::ReadExp(this, Local);
      localVars->VariableBST::GetVarVal(JuicyFruit->data[k])=t1; 
    }

    int32_t endLoc= vLoc;
    InstNode* nextInstruction;
    c2+=1; 
    int32_t currentC2 = c2;

          uint32_t defunCtr = 1;
          uint32_t nufedCtr = 0;
          do{
            endLoc+=1;
            nextInstruction = Instructions->data+endLoc;
            if(nextInstruction->commandType==String("defun")){defunCtr+=1;}
            else if(nextInstruction->commandType==String("nufed")){nufedCtr+=1;}
          }while(nufedCtr<defunCtr);

    VariableBST& local = *localVars;
    ExecuteInstructions(vLoc+1, endLoc, this->VarDB, local); 
    if (c2 == currentC2){
      rVal = 0; //no return statement!
      c2=c2-1;
    } 
    
    return rVal;
}


int32_t InstrStore::ExecuteInstructions(int32_t start, int32_t end, VariableBST& Global, VariableBST& Local){
  //read from instruction vector, then deal with each instruction separately
   InstNode* nextInstruction;
   int32_t k=start;
   int32_t control = c2;
   while(k<end && control == c2){
      nextInstruction = Instructions->data+k;
      if (nextInstruction->commandType==String("do")){
        k = ProcessDoLoops(k, Local);  //function returns the index of the last "od" found

      }
      else if(nextInstruction->commandType==String("if")){
        k = ProcessIfs(k, Local); //function returns the end of the outermost if statement
      }  
      else if (nextInstruction->commandType==String("defun")){  //store the function location and info, but skip function instructions
          this->FunctDB.FunctBST::StoreFunction(k, nextInstruction->text, nextInstruction->Parameters);
          uint32_t defunCtr = 1;
          uint32_t nufedCtr = 0;
          while(nufedCtr<defunCtr){
           // printf("%d\n", k);
            k+=1;
            nextInstruction = Instructions->data+k;
            if(nextInstruction->commandType==String("defun")){
              defunCtr+=1;
            }  
            else if(nextInstruction->commandType==String("nufed")){nufedCtr+=1;}
          }
      }
      else if (nextInstruction->commandType==String("return")){
        c2=c2-1;
        rVal = nextInstruction->expressions->ExpTree::ReadExp(this, Local);
        return rVal;
      }
      else if(nextInstruction->tokenType==SYMBOL){
         ProcessSymbol(nextInstruction->text);
      }
      else if(nextInstruction->commandType==String("text")){
          printf("%s", nextInstruction->text.c_str());
      }
      else if(nextInstruction->commandType==String("output")){
      ProcessOutput(nextInstruction, Local);
      }
      else if(nextInstruction->commandType==String("set") || nextInstruction->commandType==String("var")){
      ProcessSetVar(nextInstruction, Local);
      }
       k+=1;
    }  
    return k;
}

void InstrStore::RunInstructions(void){
  c2=0;
  ExecuteInstructions(0, this->Instructions->length, this->VarDB, this->VarDB);
}

