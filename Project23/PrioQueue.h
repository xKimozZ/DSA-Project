#ifndef PRIOQUEUE
#define PRIOQUEUE
#include "Queue.h"
template<typename T>
class PriorityQueue : public Queue<T>
{

public:
	void enqueue(T Inserted);
	void sortByCT();
	void sortByDL();
	void sortByTS();
	void sortByIO();
};

template<typename T>
void PriorityQueue<T>::enqueue(T Inserted)
{
	Node<T>* TempNode = new Node<T>(Inserted);	//Temporary node with the data member a pointer to processor object
	if (this->getHead() == nullptr)					//If queue is empty
	{
		this->setHead(TempNode);					//Set tail and head to point to TempNode
		this->setTail(TempNode);
		//ExpectedCT = Inserted->GetCT();		//Initial ExpectedCT is set to the process's CT if it is alone in the queue
		return;
	}
	Node<T>* CurrentNode;						//Temporary node used for browsing the queue
	CurrentNode = this->getHead();						//Start from head
	while (CurrentNode->getNext())				//Runs until reach the last node
	{
		CurrentNode = CurrentNode->getNext();
	}
	CurrentNode->setNext(TempNode);			//Set the last node to the TempNode
	this->setTail(TempNode);
}
template<typename T>
void PriorityQueue<T>::sortByCT()
{
	Node<T>* curNode = this->getHead();					//First node is head, current node being compared
	while (curNode) {												//Checks each node till the end of the queue
		Node<T>* minNode = curNode;							//Temporary node for comparison
		Node<T>* searchNode = curNode->getNext();					//Current node being scanned
		while (searchNode) {										//Until queue ends
			if ((searchNode->getItem()->GetCT() - searchNode->getItem()->GetCRT()) < (curNode->getItem()->GetCT() - curNode->getItem()->GetCRT())) {		//If CPU time is less than minimum Node
				minNode = searchNode;
			}
			searchNode = searchNode->getNext();							//Move to next node to be scanned
		}
		if (minNode != curNode) {									//If the currentNode isn't the minimum
			// swap the data of the two nodes
			T tmp = curNode->getItem();
			curNode->setItem(minNode->getItem());
			minNode->setItem(tmp);
		}
		curNode = curNode->getNext();									//Repeat the process for the rest of the queue
	}
}
template<typename T>
void PriorityQueue<T>::sortByTS()
{
	Node<T>* curNode = this->getHead();					//First node is head, current node being compared
	while (curNode) {												//Checks each node till the end of the queue
		Node<T>* minNode = curNode;							//Temporary node for comparison
		Node<T>* searchNode = curNode->getNext();					//Current node being scanned
		while (searchNode) {										//Until queue ends
			if (searchNode->getItem()->timestep < curNode->getItem()->timestep) {		//If timestep is less than minimum Node
				minNode = searchNode;
			}
			searchNode = searchNode->getNext();							//Move to next node to be scanned
		}
		if (minNode != curNode) {									//If the currentNode isn't the minimum
			// swap the data of the two nodes
			T tmp = curNode->getItem();
			curNode->setItem(minNode->getItem());
			minNode->setItem(tmp);
		}
		curNode = curNode->getNext();									//Repeat the process for the rest of the queue
	}
}
template<typename T>
void PriorityQueue<T>::sortByIO()
{
	Node<T>* curNode = this->getHead();					//First node is head, current node being compared
	while (curNode) {												//Checks each node till the end of the queue
		Node<T>* minNode = curNode;							//Temporary node for comparison
		Node<T>* searchNode = curNode->getNext();					//Current node being scanned
		while (searchNode) {										//Until queue ends
			if (searchNode->getItem()->io_time < curNode->getItem()->io_time) {		//If timestep is less than minimum Node
				minNode = searchNode;
			}
			searchNode = searchNode->getNext();							//Move to next node to be scanned
		}
		if (minNode != curNode) {									//If the currentNode isn't the minimum
			// swap the data of the two nodes
			T tmp = curNode->getItem();
			curNode->setItem(minNode->getItem());
			minNode->setItem(tmp);
		}
		curNode = curNode->getNext();									//Repeat the process for the rest of the queue
	}
}
template<typename T>
void PriorityQueue<T>::sortByDL()
{
	Node<T>* curNode = this->getHead();					//First node is head, current node being compared
	while (curNode) {												//Checks each node till the end of the queue
		Node<T>* minNode = curNode;							//Temporary node for comparison
		Node<T>* searchNode = curNode->getNext();					//Current node being scanned
		while (searchNode) {										//Until queue ends
			if (searchNode->getItem()->GetDL() < curNode->getItem()->GetDL()) {		//If CPU time is less than minimum Node
				minNode = searchNode;
			}
			searchNode = searchNode->getNext();							//Move to next node to be scanned
		}
		if (minNode != curNode) {									//If the currentNode isn't the minimum
			// swap the data of the two nodes
			T tmp = curNode->getItem();
			curNode->setItem(minNode->getItem());
			minNode->setItem(tmp);
		}
		curNode = curNode->getNext();									//Repeat the process for the rest of the queue
	}
}
#endif