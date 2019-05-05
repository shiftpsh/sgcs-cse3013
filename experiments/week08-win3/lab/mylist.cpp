#include "stdafx.h"
#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>

mylist::mylist(){
	num = 0;
	head = prev_node = curr_node = NULL;
}

mylist::~mylist(){
	move_first();
	while(isEmpty()==false) del();
}

bool mylist::isEmpty(void){
	if(num==0) return true;
	return false;
}

void mylist::add(Point tpoint){
	node *new_node, *temp;
	Point *temp_point;
	new_node=(node*)malloc(sizeof(node));
	temp_point=(Point*)malloc(sizeof(Point));
	temp_point->x = tpoint.x;
	temp_point->y = tpoint.y;
	
	new_node->point = temp_point;

	temp = head;
	head = new_node;
	new_node->next = temp;
	num++;
}

node* mylist::del(void){
	node* temp;
	if(curr_node==NULL)
		return NULL;
	
	temp = curr_node;
	prev_node->next = curr_node->next; 
	curr_node = curr_node->next;
	free(temp->point); 
	free(temp);
	num--;
	return curr_node;
}

node* mylist::move_first(void){
	prev_node = head;
	curr_node = head;
	return curr_node; 
}

node* mylist::move_next(void){
	if(curr_node == NULL)
		return NULL;
	prev_node = curr_node;
	curr_node = curr_node->next; 
	return curr_node; 
}