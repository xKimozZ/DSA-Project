#ifndef NODE
#define NODE
#include"process.h"
#include<iostream>
using namespace std;
template<typename T>
class Node	//Node specialized for process context
{
private:
	T item;	// A data item (can be any complex sturcture)
	Node<T>* next;	// Pointer to next node
public:

	Node(); //default constructor

	Node(T newItem);
	//non-default constructor
	

	void setItem(T newItem);


	void setNext(Node* nextNodePtr);
	

	T getItem() const;


	Node* getNext() const;
	
};

template<typename T>
Node<T>::Node()
{
	next = NULL;
	item = NULL;
}
template<typename T>
Node<T>::Node<T>(T newItem)
{
	item = newItem;
	next = NULL;

}
template<typename T>
void Node<T>::setItem(T newItem)
{
	item = newItem;
} // end setItem
template<typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
} // end setNext
template<typename T>

T Node<T>::getItem() const
{
	return item;
} // end getItem
template<typename T>
Node<T>* Node<T>::getNext() const
{

	return next;
}

#endif
