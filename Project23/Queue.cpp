#include "Queue.h"
template<typename T>
Queue<T>::Queue()	//Initialize members, and set counter to zero
{
	head = nullptr;
	tail = nullptr;
	//ExpectedCT = 0;
}
template<typename T>
void Queue<T>::enqueue(T Inserted)
{
	if (Inserted == nullptr) return;
	Node<T>* TempNode = new Node(Inserted);	//Temporary node with the data member a pointer to processor object
	if (head == nullptr)					//If queue is empty
	{
		head = TempNode;					//Set tail and head to point to TempNode
		tail = TempNode;
		//ExpectedCT = Inserted->GetCT();		//Initial ExpectedCT is set to the process's CT if it is alone in the queue
		return;
	}
	Node<T>* CurrentNode;						//Temporary node used for browsing the queue
	CurrentNode = head;						//Start from head
	while (CurrentNode->getNext())				//Runs until reach the last node
	{
		CurrentNode = CurrentNode->getNext();
	}
	CurrentNode->setNext(TempNode);			//Set the last node to the TempNode
	tail = TempNode;
	//ExpectedCT = ExpectedCT + Inserted->GetCT();	//Sums the expected CT time
}
template<typename T>
void Queue<T>::setHead(T t)
{
	head = t;
}
template<typename T>
void Queue<T>::setTail(T t)
{
	tail = t;
}
template<typename T>
T Queue<T>::dequeue()
{
	if (head == nullptr)					//do nothing if queue is empty
		return nullptr;
	T TempProcess = head->getItem();
	Node<T>* RemNode = head;
	if (head == tail)     //if there's only one node in the queue
	{
		head = nullptr;
		tail = nullptr;
	}
	else
	{
		head = RemNode->getNext();
	}
	delete RemNode;
	//ExpectedCT = ExpectedCT - TempProcess->GetCT();	//decrease ct time if a process is dequeued
	return TempProcess;
}
template<typename T>
bool Queue<T>::isEmpty()
{
	return (head == nullptr);
}
template<typename T>
void Queue<T>::print()
{
	Node<T>* p = head;
	int c = 1;
	while (p)
	{
		int id= p->getItem()->PID;
		p = p->getNext();
		cout << id << " ";
		c++;
	}
}
template<typename T>
Node<T>* Queue<T>::getHead()
{
	return head;
}

template<typename T>
Node<T>* Queue<T>::getTail()
{
	return tail;
}
template<typename T>
T Queue<T>::findAndRemove(int id)
{
	Node<T>* tmpNode = head;
	Node<T>* prevNode = nullptr;
	T tmpProc = nullptr;
	while (tmpNode)
	{
		if (tmpNode->getItem()->GetID() == id)	//If ID is found, remove the node, and reconnect the disconnected nodes
		{
			if (prevNode == nullptr)
			{
				tmpProc = dequeue();
				return tmpProc;
			}
			prevNode->setNext(tmpNode->getNext());
			tmpProc = tmpNode->getItem();
			delete tmpNode;
			return tmpProc;
		}
		prevNode = tmpNode;
		tmpNode = tmpNode->getNext();
	}
	// If ID is not found, return nullptr
	return nullptr;
}

//bool Queue::arrived(int t)
//{
	//if (head == nullptr)
		//return false;
	//if (head->GetItem()->AT >= t)
		//return false;
	//else
		//return true;
//}