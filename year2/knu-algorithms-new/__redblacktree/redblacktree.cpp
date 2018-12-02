#include <iostream>
#include <iomanip>

#include "redblacktree.h"

using namespace std;

int RedBlackTree::max(int a, int b) {
    return a > b ? a : b;
}

int RedBlackTree::abs(int a) {
    return a > 0 ? a : -a;
}

int RedBlackTree::get_height(RBTNode* t) {
    return t ? t->height : 0;
}

int RedBlackTree::get_size(RBTNode *t) {
    return t ? t->size : 0;
}

void RedBlackTree::update_height(RBTNode* &t) {
    t->height = max(get_height(t->left), get_height(t->right)) + 1;
}

void RedBlackTree::update_size(RBTNode* &t) {
    t->size = get_size(t->left) + get_size(t->right) + 1;
}

void RedBlackTree::rotate_right(RBTNode* t) {
    RBTNode* y = t->left;
    t->left = y->right;
    if (y->right != nullptr)
        y->right->parent = t;
    y->parent = t->parent;
    if (t->parent == nullptr)
        root = y;
    else if (t == t->parent->left)
        t->parent->left = y;
    else
        t->parent->right = y;
    y->right = t;
    t->parent = y;
}

void RedBlackTree::rotate_left(RBTNode* t) {
    RBTNode* y = t->right;
    t->right = y->left;
    if (y->left != nullptr)
        y->left->parent = t;
    y->parent = t->parent;
    if (t->parent == nullptr)
        root = y;
    else if (t == t->parent->left)
        t->parent->left = y;
    else
        t->parent->right = y;
    y->left = t;
    t->parent = y;
}

void RedBlackTree::insert(int key, RBTNode* t) {
    RBTNode* z = new RBTNode(key);
    RBTNode* y = nullptr;
    RBTNode* x = root;
    while (x != nullptr) {
        y = x;
        if (x->key < z->key)
            x = x->right;
        else
            x = x->left;
    }
    z->parent = y;
    if (y == nullptr)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = nullptr;
    z->right = nullptr;
    z->color = RED;
    insert_fixup(z);
}

void RedBlackTree::insert_fixup(RBTNode* z) {
    while (z && z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTNode* y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(z->parent->parent);
            }
        } else {
            RBTNode* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->parent->color = RED;
                z->parent->color = BLACK;
                y->color = BLACK;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RedBlackTree::transplant(RBTNode* u, RBTNode* v) {
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v && v->parent)
        v->parent = u->parent;
}

void RedBlackTree::remove(int key, RBTNode* t) {
    RBTNode* z = t;
    while (z && z->key != key) {
        if (key < z->key)
            z = z->left;
        else
            z = z->right;
    }
    if (!z)
        return;

    RBTNode* y = z;
    RBTNode* x;
    Color y_orig_color = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = getMinNode(z->right);
        y_orig_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x)
                x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_orig_color == BLACK)
        remove_fixup(x);
}

void RedBlackTree::remove_fixup(RBTNode* x) {
    while (x && x != root && x->color == BLACK) {
        if (x && x == x->parent->left) {
            RBTNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(x->parent);
                x = root;
            }
        } else {
            RBTNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(x->parent);
                w = x->parent->left;
            }
            if ((w->left == nullptr || w->left->color == BLACK) && (w->right == nullptr || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right && w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_left(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_right(x->parent);
                x = root;
            }
        }
    }
    if (x)
        x->color = BLACK;
}

RBTNode* RedBlackTree::getMinNode(RBTNode* t) {
    RBTNode* p = t->parent;
    while (t) {
        p = t;
        t = t->left;
    }
    return p;
}

bool RedBlackTree::contains(int key, RBTNode* t) {

}

int RedBlackTree::find_index(int key, RBTNode* &t) {

}

void RedBlackTree::print(RBTNode* p, int indent) {
    if (!p)
        return;
    print(p->right, indent + 4);
    cout << setw(indent) << p->key << "," << (p->color == RED ? "red" : "black") << "\n";
    print(p->left, indent + 4);
}

RedBlackTree::RedBlackTree() {
    root = nullptr;
}

void RedBlackTree::insert(int key) {
    insert(key, root);
}

void RedBlackTree::remove(int key) {
    remove(key, root);
}

bool RedBlackTree::contains(int key) {
    return contains(key, root);
}

int RedBlackTree::find_index(int key) {
    return find_index(key, root);
}

void RedBlackTree::print() {
    print(root);
}
