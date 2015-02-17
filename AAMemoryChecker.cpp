#include <stdio.h>
#include <stdlib.h> // includes declaration of exit()
#include "String.h"
#include "MemHeap.h"


struct CheckMemoryLeaks {
	~CheckMemoryLeaks(void) {
		if (! isSaneHeap()) {
			printf("oh goodness! you've corrupted the heap, naughty naughty\n");
			return;
		}
		if (! isEmptyHeap()) {
			printf("Uh Oh! you have a memory leak somewhere, better find it\n");
			return;
		}
		printf("The heap is all clean, good work!\n");
	}
} MemoryChecker;

