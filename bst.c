#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bst.h"

static Node* create_node(void *key, size_t size);
static void free_node(Node* node);

static inline void swap(void *a, void *b, size_t size) {
	void *tmp = malloc(size);
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
	free(tmp);
}

static Node* create_node(void *key, size_t size) {
	Node *node = calloc(1, sizeof(Node));
	if(!node)
		return NULL;
	
	node->key = calloc(1, size);
	if(!node->key)
		return NULL;

	memcpy(node->key, key, size);
	return node;
}

static void free_node(Node* node) {
	if(node) {
		free_node(node->left);
		free_node(node->right);
		if(node->key)
			free(node->key);
		free(node);
	}
}

int bst_insert(BST* bst, void* elem) {
	if(!bst || !elem)
		return -1;

	if(!bst->root) 
		bst->root = create_node(elem, bst->size);
	else {
		Node *x = bst->root;
		Node *p = NULL;
		while(x) {
			p = x;
			if(bst->cmp(x->key, elem) < 0)
				x = x->right;
			else
				x = x->left;
		}

		Node *new_node = create_node(elem, bst->size);	
		if(bst->cmp(p->key, new_node->key) < 0)
			p->right = new_node;
		else
			p->left = new_node;
		new_node->parent = p;
	}
	++bst->count;
	return 0;
}

int bst_get_min(const BST* bst, void* elem) {
	if(!bst || !elem)
		return -1;

	if(bst->root) {
		Node *x = bst->root;
		while(x->left)
			x = x->left;
		
		memcpy(elem, x->key, bst->size);
		return 0;
	}
	return 1;
}

int bst_get_max(const BST* bst, void* elem) {
	if(!bst || !elem)
		return -1;

	if(bst->root) {
		Node *x = bst->root;
		while(x->right)
			x = x->right;
	
		memcpy(elem, x->key, bst->size);
		return 0;
	}
	return 1;
}

static void shift_up(BST* bst, Node* z, Node* y) {
	if(y)
		y->parent = z->parent;	
	
	if(z->parent) {
		if(z->parent->right == z)
			z->parent->right = y;
		else
			z->parent->left = y;
	}
	else
		bst->root = y;
	
	free(z->key);
	free(z);
}

int bst_delete(BST* bst, void *elem) {
	int match;
	if(!bst || !elem)
		return -1;
	
	if(bst->count == 0)
		return 1;
	
	Node *z = bst->root;
	while(z) {
		match = bst->cmp(z->key, elem);
		if(match == 0){
			if(z->left && z->right) {
				Node *y = z->right;
				while(y->left)
					y = y->left;

				if(y->right)
					y->right->parent = y->parent;
				
				if(z->right == y)
					z->right = y->right;
				else 
					y->parent->left = y->right;
				
				free(z->key);
				z->key = y->key;
				free(y);
			}
			else if(z->right)
				shift_up(bst, z, z->right);
			else if(z->left)
				shift_up(bst, z, z->left);
			else 
				shift_up(bst, z, NULL);
			
			--bst->count;
			return 0;
		}
		else if(match < 0)
			z = z->right;
		else
			z = z->left;
	}

	return 1;
}

BST* bst_build(void* input, size_t nmemb, size_t size, Comparator cmp) {
	if(!nmemb)
		return NULL;

	BST* bst = calloc(1, sizeof(BST));
	if(!bst)
		return NULL;
	
	bst->cmp = cmp;
	bst->size = size;
	
	size_t j;
	void *selected;
	void *last;
	
	void *data = malloc(nmemb * size);
	if(!data)
		return NULL;
	
	memcpy(data, input, nmemb * size);

	srand(time(NULL));
	for(size_t i = 0; i < nmemb; ++i) {
		j = rand() % (nmemb - i);
		selected = data + j * size;
		if(bst_insert(bst, selected) != 0)
			return NULL;

		last = data + (nmemb - i - 1) * size;
		swap(selected, last, bst->size);
	}
	
	free(data);
	return bst;
}

void bst_free(BST* bst) {
	if(bst) {
		free_node(bst->root);	
		free(bst);
	}
}
