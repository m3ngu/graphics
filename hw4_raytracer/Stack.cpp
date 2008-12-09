#include "Stack.h"

StackObject::StackObject() {
	for(int i = 0; i < StackObject::DEPTH; i++)
	{
		stackarr[i] = mat4_id;
	}
	top = -1;
}
int StackObject::push() {
	top = top + 1;
	if(top == DEPTH)
		return -1;
	else {
		if(top > 0)
			stackarr[top] = stackarr[top - 1];
		return 0;
	}
}
mat4 StackObject::pop() {
	if(top > -1)
	{
		mat4 data = stackarr[top];
		stackarr[top] = mat4_id;
		top = top - 1;
		return data;
	}
	else
	{
		return mat4_id;
	}	
}
void StackObject::pushElement(mat4 matrix) {
	if(top == -1)
	{
		top = top + 1;
		stackarr[top] = matrix;
	}
	else
	{
		mat4 currmat = stackarr[top];
		mult(stackarr[top],currmat,matrix);
	}
}
mat4 StackObject::getTopofStack() {
	if(top > -1)
		return stackarr[top];
	else
		return mat4_id;
}