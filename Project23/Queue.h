#ifndef QUEUE
#define QUEUE
#include "Node.h"
template<typename T>
class Queue
{
private:
	Node<T>* head;
	Node<T>* tail;
public:

	int ExpectedCT;	//Expected execution time, used for checking the shortest ready queue
	Queue();
	Node<T>* getHead();
	Node<T>* getTail();
	void setHead(Node<T>* t);
	void setTail(Node<T>* t);
	virtual void enqueue(T Inserted);	//Enqueue a process
	//bool arrived(int t);	//Checks if arrival time for process has come. Most likely will be used by scheduler
	T dequeue();		//Dequeue and return a process
	void print();
	bool isEmpty();			//Check if queue is empty
	void resetStatus();
};

template<typename T>
Queue<T>::Queue()	//Initialize members, and set counter to zero
{
	head = nullptr;
	tail = nullptr;
}
template<typename T>
void Queue<T>::enqueue(T Inserted)
{
	if (Inserted == nullptr) return;
	Node<T>* TempNode = new Node<T>(Inserted);	//Temporary node with the data member a pointer to processor object
	if (head == nullptr)					//If queue is empty
	{
		head = TempNode;					//Set tail and head to point to TempNode
		tail = TempNode;
		
		return;
	}
	tail->setNext(TempNode);			//Set the last node to the TempNode
	tail = TempNode;

}
template<typename T>
void Queue<T>::setHead(Node<T>* t)
{
	head = t;
}
template<typename T>
void Queue<T>::setTail(Node<T>* t)
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
		int id = p->getItem()->GetPID();
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
void Queue<T>::resetStatus()
{
	Node<T>* tmpNode = head;
	while (tmpNode)
	{
		if (!tmpNode->getItem()->isChange())	//If status is false, toggle to true
			tmpNode->getItem()->ToggleChange();
		tmpNode = tmpNode->getNext();
	}
}

#endif
