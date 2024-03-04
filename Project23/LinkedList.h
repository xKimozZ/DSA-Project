#ifndef _LINKEDLIST
#define _LINKEDLIST
#include<iostream>
#include "Node.h"
using namespace std;
template<typename T>
class LinkedList
{
private:
	Node<T>* head;
public:

	int ExpectedCT;	//Expected execution time, used for checking the shortest ready queue
	LinkedList();
	Node<T>* getHead();
	Node<T>* getByID(int id);	//Find specific node by ID
	void setHead(Node<T>* t);
	virtual void insert(T Inserted);	//Insert object
	T remove();		//Remove and return a process
	void print();
	bool isEmpty();			//Check if LinkedList is empty
	T findAndRemove(int id);	//Find a certain ID process, return it and remove it from LinkedList
	void resetStatus();
};

template<typename T>
LinkedList<T>::LinkedList()	//Initialize members, and set counter to zero
{
	head = nullptr;
}
template<typename T>
void LinkedList<T>::insert(T Inserted)
{
	Node<T>* TempNode = new Node<T>(Inserted);	//Temporary node with the data member a pointer to processor object
	if (head == nullptr)					//If queue is empty
	{
		head = TempNode;					//Set head to point to TempNode
		return;
	}
	Node<T>* CurrentNode;						//Temporary node used for browsing the queue
	CurrentNode = head;						//Start from head
	while (CurrentNode->getNext())				//Runs until reach the last node
	{
		CurrentNode = CurrentNode->getNext();
	}
	CurrentNode->setNext(TempNode);			//Set the last node to the TempNode
}
template<typename T>
void LinkedList<T>::setHead(Node<T>* t)
{
	head = t;
}

template<typename T>
T LinkedList<T>::remove()
{
	if (head == nullptr)					//do nothing if LinkedList is empty
		return nullptr;
	T TempProcess = head->getItem();
	Node<T>* RemNode = head;
	if (head->getNext() == nullptr)     //if there's only one node in the LinkedList
	{
		head = nullptr;
		delete RemNode;
	}
	else
	{
		head = RemNode->getNext();
		delete RemNode;
	}
	return TempProcess;
}
template<typename T>
bool LinkedList<T>::isEmpty()
{
	return (head == nullptr);
}
template<typename T>
void LinkedList<T>::print()
{
	Node<T>* p = head;
	int c = 1;
	while (p)
	{
		int id = p->getItem()->GetPID();
		p = p->getNext();
		cout << id << " ";
		c++;
	}
}
template<typename T>
Node<T>* LinkedList<T>::getHead()
{
	return head;
}

template<typename T>
Node<T>* LinkedList<T>::getByID(int id)
{
	Node<T>* tmpNode = head;
	while (tmpNode)
	{
		if (tmpNode->getItem()->GetPID() == id)	//If ID is found, return the node
		{
			return tmpNode;
		}
		tmpNode = tmpNode->getNext();
	}

	// If ID is not found, return nullptr
	return nullptr;
}
template<typename T>
T LinkedList<T>::findAndRemove(int id)
{
	Node<T>* prevNode = nullptr;
	Node<T>* currNode = head;
	T tmpProc = nullptr;
	while (currNode)
	{
		if (currNode->getItem()->GetPID() == id)
		{
			if (prevNode == nullptr)  // If the node to remove is the head node
			{
				head = currNode->getNext();
			}
			else
			{
				prevNode->setNext(currNode->getNext());
			}
			tmpProc = currNode->getItem();
			delete currNode;
			break;
		}
		prevNode = currNode;
		currNode = currNode->getNext();
	}
	return tmpProc;
}


template<typename T>
void LinkedList<T>::resetStatus()
{
	Node<T>* tmpNode = head;
	while (tmpNode)
	{
		if (!tmpNode->getItem()->isChange())
			tmpNode->getItem()->ToggleChange();
		tmpNode = tmpNode->getNext();
	}
}

#endif	
