#ifndef _String_h
#define _String_h
#include "MemHeap.h"
#include <stdlib.h>
#include <stdint.h>


struct String {
private:
	char* ptr;
	uint32_t len;

public:
	String(const char* src) { construct(src); }

	String(void) { construct(""); }

	String(const String& that) { copy(that); }
	~String(void) { destroy(); }
	String& operator=(const String& that) {
		if (this != &that) { 
			destroy();
			copy(that);
		}
		return *this;
	}

	String& operator=(const char* src) {
		destroy();
		construct(src);
		return *this;
	}

	String& operator+(String s2){
		String& s1 = *this;
		String* result = new String;
		result->len = s1.len+s2.len;
		for(uint32_t k=0; k<s1.len; k+=1){
			result->ptr[k]=s1.ptr[k];
		}
		for(uint32_t k=0; k<s2.len; k+=1){
			result->ptr[k+s1.len]=s2.ptr[k];
		}
		return *result;
	}

	/*
	 * clients can gain access to the C-compatible string (i.e., the char*) by 
	 * invoking myString.c_str() 
	 * This is helpful, for example, if you want to print a string with printf
	 *  
	 *   printf("Customer with name %s  has purchased %d bottles\n", 
	 *			    myCustomer.name.c_str(), myCustomer.bottles);
	 */
	const char* c_str(void) const { return ptr; }

	uint32_t size(void) const { return len; }

	bool operator==(String s2) const {
		const String& s1 = *this;
		if (s1.size() != s2.size()) { return false; }
		uint32_t k = 0;
		while (k < s1.size()) {
			if (s1.ptr[k] != s2.ptr[k]) { return false; }
			k += 1;
		}
		return true;
	}

	bool operator<(String s2) const {
		const String& s1 = *this;
		int k = 0;
		while (s1.ptr[k] == s2.ptr[k]) {
			if (s1.ptr[k] == 0) { return false; } // strings are equal
			k += 1;
		}
		return s1.ptr[k] < s2.ptr[k];
	}


	bool operator!=(String s2) const { const String& s1 = *this; return !(s1 == s2); }
	bool operator>(String s2) const { const String& s1 = *this; return s2 < s1; }
	bool operator<=(String s2) const { const String& s1 = *this; return ! (s2 < s1); }
	bool operator>=(String s2) const { const String& s1 = *this; return ! (s1 < s2); }

private:
	/* for internal use only, clients really shouldn't be calling these functions
	 * in future versions of this String object type, I might remove these functions entirely or
	 * dramatically change their behavior. */

	/* WARNING: DO NOT CALL ANY FUNCTION BELOW THIS LINE */

	void construct(const char* src) {
		len = 0;
		while (src[len] != 0) {
			len += 1;
		}
		ptr = (char*) malloc(len + 1);
		copy(src);
	}

	/* copy(char*) assumes that ptr has been allocated and holds sufficient capacity for src */
	void copy(const char* src) {
		int k = len;
		while (k >= 0) {
			ptr[k] = src[k];
			k -= 1;
		}
	}

	/* copy(String&) is my "real" copy constructor. I always name this function "copy" and its purpose
	 * is to construct a "deep copy" (i.e., an independent object that shares no pointers with the original object) */
	void copy(const String& that) {
		len = that.len;
		ptr = (char*) malloc(len + 1);
		copy(that.ptr);
	}

	/* destroy(void) is my "real" destructor. I always name this function destroy and my official destructor is always
	 * just a simple one-line call to this function */
	void destroy(void) {
		free(ptr);
	}
};

struct SVector{
      uint32_t length;
      uint32_t capacity;
      String* data;

      SVector(void) {
        length = 0;
        capacity = 0;
        data = nullptr;
    }
    
      void copy(const SVector& x) {
        this->length = x.length;
        this->capacity = x.length;
        if (x.length == 0) { this->data = nullptr; }
        else {
          this->data = new String[this->capacity];
        }
        for (uint32_t k = 0; k < x.length; k += 1) {
            this->data[k] = x.data[k];
        }
    }


      void destroy(void) {
        delete[] this->data;
      }

      SVector(const SVector& that) {
        copy(that);
      }

     ~SVector(void) {
        destroy();
      }

     SVector& operator=(const SVector& that) {
        if (this != &that) { 
            destroy();
            copy(that);
        }
        return *this;
     }

     void Insert_SVector(String* s){
  		if (capacity == length) { 
  			capacity *= 2; 
        	if (capacity == 0) { capacity = 1; }
        	String* p = new String[capacity*2];
        	for (uint32_t k = 0; k < length; k += 1) {
           		 p[k] = data[k];
        	}
       		 delete[] this->data;
        	data = p;
    	}
   		this->data[length] = *s;
    	length += 1;
		}		

};
	

#endif /* _String_h */