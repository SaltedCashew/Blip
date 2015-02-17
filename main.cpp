//Brad Gray bg22946
//EE312 SU2014
//Project7
//main.cpp 

#include <stdio.h>
#include "Parse.h"
#include "Instruct.h"

InstrStore instrDatabase;


int main(void){
	set_input("testtest.blip");
	instrDatabase.InstrStore::BuildInstructions();
	instrDatabase.InstrStore::RunInstructions();
	
	return 0;
}
