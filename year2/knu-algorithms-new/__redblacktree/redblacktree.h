#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

enum Color { RED, BLACK};

struct RBTNode {
    int key;
    RBTNode* left;
    RBTNode* right;
    RBTNode* parent;
    int height;
    int size;
    Color color; //0-black,1-red

    RBTNode(int key = 0, Color color = BLACK, RBTNode* left = nullptr, RBTNode* right = nullptr, RBTNode* parent = nullptr, int height = 1, int size = 1)
            : key(key), color(color), left(left), right(right), parent(parent), height(height), size(size) {}
};

class RedBlackTree {
private:

    RBTNode *root;

    int max(int a, int b);
    int abs(int a);
    int get_height(RBTNode* t);
    int get_size(RBTNode *t);
    void update_height(RBTNode* &t);
    void update_size(RBTNode* &t);
    void rotate_right(RBTNode* t);
    void rotate_left(RBTNode* t);
    void insert(int key, RBTNode* t);
    void transplant(RBTNode* u, RBTNode* v);
    void remove(int key, RBTNode* t);
    void remove_fixup(RBTNode* t);
    void insert_fixup(RBTNode* t);
    RBTNode* getMinNode(RBTNode* t);
    bool contains(int key, RBTNode* t);
    int find_index(int key, RBTNode* &t);
    void print(RBTNode* p, int indent = 0);
    
public:
    RedBlackTree();
    void insert(int key);
    void remove(int key);
    bool contains(int key);
    int find_index(int key);
    void print();
};

#endif /* REDBLACKTREE_H */

