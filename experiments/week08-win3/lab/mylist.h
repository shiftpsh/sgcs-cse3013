#ifndef __MYLIST_H__
#define __MYLIST_H__

#include "define.h"

typedef struct node__ { 
	Point*	point; 
	struct node__	*next; 
} node; 

class mylist {
	private:
			int num;
			node *head;
			node *prev_node;
			node *curr_node;

	protected:
			node* del(void);

	public:
			mylist();
			virtual ~mylist();
			
			bool isEmpty();
			void add(Point tpoint);

			node* move_first(void);
			node* move_next(void);
};

#endif 
