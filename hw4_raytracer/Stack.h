#include<stdio.h>
#include "nv/nv_math.h"

class StackObject {
private:
	int top;
	static const int DEPTH = 50;
	mat4 stackarr[DEPTH];
public:
	StackObject();
	int push();
	mat4 pop();
	void pushElement(mat4 matrix);
	mat4 getTopofStack();
	virtual ~StackObject(){};
};

