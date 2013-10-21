// ============================================================================ 
// listmanip.h
// ~~~~~~~
// Hung Q. Ngo
// - singly linked list node structure
// - prototypes for a couple of functions manipulating singly linked lists
// - DO NOT MODIFY THIS FILE
// *****************************************************************************

#ifndef LIST_SORT_H_
#define LIST_SORT_H_

#include <cstddef> // this header defines NULL
#include <vector>
#include "Lexer.h"

struct Node {
    int   key;
    Node* next;
    Node(int k=0, Node* n=NULL) : key(k), next(n) {};
};

// *****************************************************************************
// the following two functions are already implemented in listmanip.cpp
// *****************************************************************************
void  print_list(Node* ptr); // print all nodes starting from ptr
void  free_list(Node* ptr);  // free nodes from ptr down

// *****************************************************************************
// below are the functions you are asked to implement
// *****************************************************************************
Node* sort_list(Node* head);  // sort & return new head to sorted list
Node* new_list(const std::vector<Token>&);   // create a new list 
Node* merge_lists(Node* head1, Node* head2); // merge two sorted lists
Node* remove_duplicates(Node*);   // remove duplicate elements from list
Node* keep_common(Node*, Node*);  // keep only common elements

#endif
