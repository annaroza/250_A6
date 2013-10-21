// ============================================================================ 
// listmanip.cpp
// ~~~~~~~~~~~~~
// ANNA ROZANSKY 
// implement the missing functions
// ============================================================================ 

#include <iostream>
#include <stdexcept>
#include <sstream>
#include "listmanip.h"
#include "Lexer.h"
#include "term_control.h"

using namespace std; // BAD PRACTICE

extern const string usage_msg = "List Manipulation Toy. Version 0.1\n"
                                "Author: ANNA ROZANSKY\n"
                                "Report bugs to annaroza@buffalo.edu";

void newNode(Node* first, int num){		//num is the number in the vector<Token> to be added to the list
						//first will always point to the start of the list
        Node* temp;                             //temp will be used to add things to the list
        temp = new Node;
        Node* traverser;                        

	temp->key = num;			//num assigned to the new node's key
	temp->next = NULL;			//the new node's next pointer is null
						//because it is currently last in the list

        if(first == NULL){			//if there is not already anything in the list, then temp is the first
                first = temp;
		first->next = NULL;
        }
        else{
                traverser = first;                      //if the list is not empty, traverser starts off pointing to
                                                        //the pointer of the start of the list

                while(traverser->next != NULL){         //while not at the end of the list
                        traverser = traverser->next;    //keep moving pointer to next
                }
		traverser->next = temp;			//temp is the new node (added to end of list)
        }						//now the previous last node ->next should point to temp
}

/**
 * -----------------------------------------------------------------------------
 * create a new singly linked list containing keys which are integers from 
 * a given vector of tokens
 * return the pointer to the head of the newly created list
 * -----------------------------------------------------------------------------
 */
Node* new_list(const vector<Token>& tok_vec)
{
	//integers from a given vector of tokens (tok_vec)
	//traverse through tok_vec
	//for each item in tok_vec -- add to linked list
	Node* first = new Node;
	size_t i;
	if(!tok_vec.empty()){
		for(i = 0; i<tok_vec.size(); i++){
			string string_integer = tok_vec.at(i).value;	//tok_vec.at(i).value is a string
			istringstream buffer(string_integer);		//so this converts the string to an int (num)
			int num;
			buffer >> num;
			newNode(first, num);			//newNode(first(head of list), int to insert)	
		}						
	}
	return first;					//return the pointer to the head of the list

    // YOUR CODE GOES HERE
    //return NULL; // and don't return NULL unless you have to
}

/**
 * -----------------------------------------------------------------------------
 * given the pointers to the heads of two singly linked lists
 * modify (only the pointers of) the first list so that only the common elements
 * remain. 
 * - return the head pointer to the resulting first list
 * - assume the two input list are already sorted and each of them has no
 *   duplicate key. 
 * - if any of of them is not sorted or has duplicate keys, then throw a
 *   runtime_error() with the appropriate message
 * - do remember to free-up the space of the deleted elements
 * -----------------------------------------------------------------------------
 */
Node* keep_common(Node* head1, Node* head2)
{
	//think of merge step of merge sort
	
	//ex. A: 1 3 5 8 9
	//    B: 1 5 6 8 9
	//result of print a: 1 5 8 9

	Node* traverser1 = head1;
	Node* next_after_next;
	Node* traverser2 = head2;
	Node* previous = head1;
	Node* temp;

	if(traverser1 == NULL || traverser2 == NULL){
		return NULL;
	}

	while(traverser1->next != NULL && traverser2->next != NULL){
		next_after_next = traverser1->next;
		if(traverser1->key == traverser2->key){
			//leave it in 
			previous = traverser1;
			traverser1 = traverser1->next;
			traverser2 = traverser2->next;
			next_after_next = traverser1->next;
		}
		else{
			//keep traverser2 the same
			//keep previous the same
			//change previous->next to next_after_next
			//remove from a
			if(traverser1->key < traverser2->key){
				temp = traverser1;
				//traverser1->next = NULL;
				traverser1 = traverser1->next;
				if(traverser1->next != NULL){
					previous->next = next_after_next;
		        	        next_after_next = traverser1->next;
					temp->next = NULL;
				}
				else{
					previous->next = next_after_next;	
					next_after_next->next = NULL;
				}
				delete temp;
			}
			else if(traverser1->key > traverser2->key){
				if(traverser2->next != NULL){
					traverser2 = traverser2->next;
				}
				else{						//ex. get here comparing a = 1 2 4 6 8 9 and b = 1 2 4 5 6 7
					previous->next = NULL;			//when traverser1 = 8 and traverser2 = 7
					//temp = traverser1;			//delete everything from then on in list a
					while(traverser1->next != NULL){
						temp = traverser1;
						traverser1 = traverser1->next;
						temp->next = NULL;
						delete temp;
					}
					break;
				}
			}
		}
	} 
	return head1;

	// YOUR CODE GOES HERE
	//return NULL; // and don't return NULL unless you have to
}


/**
 * -----------------------------------------------------------------------------
 * given the pointer to the head of a singly linked list, remove the duplicate
 * elements and return the pointer to the head of the new list
 * return the head pointer to the resulting list
 * - assumes that the given list (pointed to by head) is sorted in
 *   non-decreasing order
 * - if not, throw a runtime_error exception stating the error
 * -----------------------------------------------------------------------------
 */
Node* remove_duplicates(Node* head)
{
	Node* traverser = head;
	Node* next_after_next;
	if(traverser == NULL){		//do nothing if the list is empty
		return traverser;
	}
	while(traverser->next != NULL){					//while we are not at the end of the list 
		if(traverser->key == traverser->next->key){		//if the current value is equal to the next value (list is sorted)
			next_after_next = traverser->next->next;	//start looking again after those two values
	//		free(traverser->next);				//get rid of the duplicate
			traverser->next = next_after_next;		//this is where we start the next check
		}
		else{							//move on if we don't delete anything
			traverser = traverser->next;
		}
	}
	return head;
    // YOUR CODE GOES HERE
    //return NULL; // and don't return NULL unless you have to
}

/**
 * -----------------------------------------------------------------------------
 * merge two sorted lists pointed to by head1 and head2
 * throw runetime_error("Please give me two sorted lists") if any one of them
 * is not already sorted
 * return pointer to the head of the merged list
 * only modify pointers. DONOT modify the key of any node
 * -----------------------------------------------------------------------------
 */
Node* merge_lists(Node* head1, Node* head2) 
{
	Node* traverser2;
	traverser2 = head2;
	if(head1 == NULL && head2 == NULL){	 
		return NULL;
	}
	while(traverser2->next != NULL){		//while not at the end of the second list
		newNode(head1, traverser2->key);	//add to first list
		traverser2 = traverser2->next;		//keep going through second list
	}						//***this isn't adding the very last item
	head1 = sort_list(head1);			//once all nums in second list are in first list, sort again
	return head1;
	
	//1. add all numbers in head2 into head1
	//2. resort

    // YOUR CODE GOES HERE
   // return NULL; // and don't return NULL unless you have to
}

/**
 * -----------------------------------------------------------------------------
 * sort the list pointed to by 'head'
 * return the pointer to the head of the new list
 * DO NOT modify the keys, just manipulate the pointers
 * -----------------------------------------------------------------------------
 */
Node* sort_list(Node* head) 
{
	//selection sort
//	Node* unsorted_start;
	Node* traverser;	//i
	Node* temp;		
	Node* min;		//j
	Node* next;		//k
	Node* temphead;	
////////////////////////////////////////
	temphead = head;
	traverser = head;
	while(traverser->next != NULL){
		min = traverser;
		next = traverser->next;
		while(traverser->next != NULL){ 
			if(next->key < min->key){
				min = next;
			}
			traverser = traverser->next;
			next = next->next;
		}
		if(min != temphead){
			temphead = min;
			min = traverser;
			traverser = temphead;
		}
		traverser = traverser->next;
	} 


	//		temp = traverser;
	//		head = traverser->next;
	//		temp_next = head->next;
	//		head->next = temp;
	//		temp->next = temp_next;
	return head; 
	

    // YOUR CODE GOES HERE
    //return NULL; // and don't return NULL unless you have to
}

// *****************************************************************************
// DO NOT MODIFY THIS FILE BEYOND THIS POINT
// *****************************************************************************

/**
 * -----------------------------------------------------------------------------
 * free the memory of all nodes starting from ptr down
 * -----------------------------------------------------------------------------
 */
void free_list(Node* ptr) {
    Node* temp;
    while (ptr != NULL) {
        temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}
