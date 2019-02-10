#ifndef __BST_H__
#define __BST_H__

typedef struct Node Node;
struct Node{
	Node* parent;
	Node* left;
	Node* right;
	void *key;
};

typedef int (*Comparator)(const void* n, const void* m);
typedef struct {
	Node* root;
	Comparator cmp;
	size_t size;
	size_t count;
}BST;

BST* bst_build(void* data, size_t nmemb, size_t size, Comparator cmp);
void bst_free(BST *bst);

int bst_insert(BST* bst, void* elem);
int bst_get_min(const BST* bst, void* elem);
int bst_get_max(const BST* bst, void* elem);
int bst_delete(BST* bst, void *elem);

#endif
