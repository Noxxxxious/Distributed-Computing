#include <stdio.h>
#include <stdlib.h>

struct element {
	char* name;
	int priority;
	struct element* left;
	struct element* right;
};

struct queue {
	struct element* first;
	struct element* last;
};

void push(struct queue *q, struct element* elem) {
	if (elem->priority < 0)
		elem->priority = 0;
	if (!q->last) {
		q->last = q->first = elem;
		q->first->right = q->first->left = NULL;
		q->last->right = q->last->left = NULL;
	}
	else if (q->first == q->last) {
		elem->left = q->last;
		elem->right = NULL;
		q->first->right = elem;
		q->last = elem;
	}
	else {
		elem->left = q->last;
		elem->right = NULL;
		q->last->right = elem;
		q->last = elem;
	}
	return;
}

void pop(struct queue* q) {
	if (q->first) {
		if (q->first == q->last) {
			free(q->first);
			q->first = q->last = NULL;
		}
		else if (q->last->left == q->first) {
			q->last->left = NULL;
			free(q->first);
			q->first = q->last;
		}
		else{
			q->first = q->first->right;
			free(q->first->left);
			q->first->left = NULL;
		}
	}
	return;
}

void print(struct queue* q) {
	if (q->first) {
		struct element* tmp = q->first;
		while (tmp) {
			printf("%s ", tmp->name);
			tmp = tmp->right;
		}
		printf("\n");
	}
	return;
}

void insert(struct queue* q, struct element* elem, int p) {
	elem->priority = p;
	if (q->first) {
		if (p <= q->last->priority)
			push(q, elem);
		else if (p > q->first->priority) {
			elem->left = NULL;
			elem->right = q->first;
			q->first->left = elem;
			q->first = elem;
		}
		else {
			struct element* tmp = q->last;
			while (tmp->left->priority < p)
				tmp = tmp->left;
			elem->left = tmp->left;
			elem->right = tmp;
			(tmp->left)->right = elem;
			tmp->left = elem;
		}
	}
	else {
		push(q, elem);
	}
	return;
}

void delete_by_priority(struct queue * q, int p) {
	if (q->first && p <= q->first->priority && p >= q->last->priority) {
		struct element* tmp = q->first;
		while (tmp) {
			if (tmp->priority == p) {
				if (tmp == q->first) {
					tmp = tmp->right;
					pop(q);
				}
				else if (tmp == q->last) {
					q->last = (q->last)->left;
					tmp = tmp->right;
					free((q->last)->right);
					(q->last)->right = NULL;
				}
				else {
					tmp = tmp->right;
					tmp->left = (tmp->left)->left;
					free(tmp->left->right);
					(tmp->left)->right = tmp;
				}
			}
			else
				tmp = tmp->right;
		}	
	}
	return;
}

void merge(struct queue* q1, struct queue* q2) {
	if (q2->first) {
		struct element* tmp = q2->first;
		while (tmp) {
			struct element* tmp2 = tmp;
			tmp = tmp->right;
			insert(q1, tmp2, tmp2->priority);
		}
		q2->first = q1->first;
	}
	return;
}

int main() {
	struct queue* q = malloc(sizeof(struct queue));
	struct queue* q2 = malloc(sizeof(struct queue));
	q->first = NULL;
	q->last = NULL;
	q2->first = NULL;
	q2->last = NULL;

	struct element* e1 = malloc(sizeof(struct element));
	struct element* e2 = malloc(sizeof(struct element));
	struct element* e3 = malloc(sizeof(struct element));
	struct element* e4 = malloc(sizeof(struct element));
	struct element* e5 = malloc(sizeof(struct element));
	struct element* e6 = malloc(sizeof(struct element));
	e1->name = "1";
	e2->name = "2";
	e3->name = "3";
	e4->name = "4";
	e5->name = "5";
	e6->name = "6";

	push(q, e1);
	insert(q, e2, 10);
	insert(q, e3, 15);
	insert(q2, e4, 7);
	insert(q2, e5, 12);
	push(q2, e6);
	print(q);
	print(q2);

	merge(q, q2);
	print(q);
	print(q2);

	delete_by_priority(q, 0);
	print(q);

	pop(q);
	pop(q);
	print(q);

	return 0;
}