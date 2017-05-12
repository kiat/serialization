
#ifndef OFFSET_PTR_H
#define OFFSET_PTR_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <iostream>

using namespace std;

// offset pointer class
template <typename T>
class offset_ptr {

	size_t value;

public:

	// long getValue() {
	// 	return value;
	// }

	T &operator [] (size_t which) {
		return ((T*) (((char *) this) + value))[which]; 
	}

	T *operator -> () {
		return ((T*) (((char *) this) + value));
	}

	T *operator * () {
		return ((T*) (((char *) this) + value))[0];
	}

	offset_ptr <T> &operator = (T *me) {
		value = ((char *) me - (char *) this);
		return *this;
	}

	offset_ptr <T> &operator = (offset_ptr <T> &me) {
		value = ((char *) &me - (char *) this) + me.value;
		return *this;
	}

	operator T * () {
		return (T*) (((char *) this) + value);
	}

	operator void * () {
		return (T*) (((char *) this) + value);
	}

	offset_ptr (T *fromMe) {
		value = ((char *) fromMe) - (char *) this;
	}

	offset_ptr () {
		value = -1;
	}

	bool is_null () {
		return value == -1;
	}

	offset_ptr (void *fromMe) {
		value = ((char *) fromMe) - (char *) this;
	}
};

#endif
