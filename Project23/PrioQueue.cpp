#include "PrioQueue.h"

template<typename T>
void PriorityQueue<T>::enqueue(T Inserted)
{
	Node<T>* TempNode = new Node<T>(Inserted);	//Temporary node with the data member a pointer to processor object
	if (getHead() == nullptr)					//If queue is empty
	{
		setHead(TempNode);					//Set tail and head to point to TempNode
		setTail(TempNode);
		//ExpectedCT = Inserted->GetCT();		//Initial ExpectedCT is set to the process's CT if it is alone in the queue
		return;
	}
	Node<T>* CurrentNode;						//Temporary node used for browsing the queue
	CurrentNode = getHead();						//Start from head
	while (CurrentNode->getNext())				//Runs until reach the last node
	{
		CurrentNode = CurrentNode->getNext();
	}
	CurrentNode->setNext(TempNode);			//Set the last node to the TempNode
	setTail(TempNode);
	//ExpectedCT = ExpectedCT + Inserted->GetCT();	//Sums the expected CT time

	//sortByCT();								//Sorts the queue in ascending order in term of CT
}
template<typename T>
void PriorityQueue<T>::sortByCT()
{
	Node<T>* curNode = getHead();					//First node is head, current node being compared
	while (curNode) {												//Checks each node till the end of the queue
		process* minData = curNode->getItem();							//Temporary process pointer for comparison
		Node<T>* minNode = curNode;							//Temporary node for comparison
		Node<T>* searchNode = curNode->getNext();					//Current node being scanned
		while (searchNode) {										//Until queue ends
			if (searchNode->getItem()->GetCT() < minData->GetCT()) {		//If CPU time is less than minimum Node
				minData = searchNode->getItem();							//Will override the temporary pointer
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