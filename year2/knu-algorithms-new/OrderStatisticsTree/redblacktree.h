#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

enum Color {
  RED, BLACK
};

template <typename TKey, typename TValue>
struct RBTNode {
  TKey key;
  TValue value;
  RBTNode<TKey, TValue>* left;
  RBTNode<TKey, TValue>* right;
  RBTNode<TKey, TValue>* parent;
  int height;
  int size;
  Color color; //0-black,1-red

  RBTNode<TKey, TValue>(TKey key = TKey(), TValue value = TValue(), Color color = BLACK, RBTNode<TKey, TValue>* left = nullptr, RBTNode<TKey, TValue>* right = nullptr, RBTNode<TKey, TValue>* parent = nullptr, int height = 1, int size = 1)
    : key(key), value(value), color(color), left(left), right(right), parent(parent), height(height), size(size) {}
};

template <typename TKey, typename TValue>
class RedBlackTree {
private:

  RBTNode<TKey, TValue> *root;

  int get_height(RBTNode<TKey, TValue>* t);
  int get_size(RBTNode<TKey, TValue> *t);
  void update_height(RBTNode<TKey, TValue>* &t);
  void update_size(RBTNode<TKey, TValue>* &t);
  void update(RBTNode<TKey, TValue>* &t);
  void rotate_right(RBTNode<TKey, TValue>* t);
  void rotate_left(RBTNode<TKey, TValue>* t);
  void insert(TKey key, TValue value, RBTNode<TKey, TValue>* t);
  void transplant(RBTNode<TKey, TValue>* u, RBTNode<TKey, TValue>* v);
  void remove(TKey key, RBTNode<TKey, TValue>* t);
  void remove_fixup(RBTNode<TKey, TValue>* t);
  void insert_fixup(RBTNode<TKey, TValue>* t);
  RBTNode<TKey, TValue>* getMinNode(RBTNode<TKey, TValue>* t);
  bool contains(TKey key, RBTNode<TKey, TValue>* t);
  TValue get_value(TKey key, RBTNode<TKey, TValue>* t);
  int find_index(TKey key, RBTNode<TKey, TValue>* &t);
  void show(RBTNode<TKey, TValue>* p, int indent = 0);
  void show_by_section(RBTNode<TKey, TValue>* p, int indent = 0);

public:
  RedBlackTree();
  void insert(TKey key, TValue value);
  void remove(TKey key);
  bool contains(TKey key);
  int find_index(TKey key);
  TValue get_value(TKey key);
  void show_by_section();
  void show();
};

template <typename TKey, typename TValue>
RedBlackTree<TKey, TValue>::RedBlackTree() {
  root = nullptr;
}

template <typename TKey, typename TValue>
int RedBlackTree<TKey, TValue>::get_height(RBTNode<TKey, TValue>* t) {
  return t ? t->height : 0;
}

template <typename TKey, typename TValue>
int RedBlackTree<TKey, TValue>::get_size(RBTNode<TKey, TValue> *t) {
  return t ? t->size : 0;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::update_height(RBTNode<TKey, TValue>* &t) {
  if (!t)
    return;
  t->height = max(get_height(t->left), get_height(t->right)) + 1;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::update_size(RBTNode<TKey, TValue>* &t) {
  if (!t) return;
  t->size = get_size(t->left) + get_size(t->right) + 1;
}

template <typename TKey, typename TValue>
inline void RedBlackTree<TKey, TValue>::update(RBTNode<TKey, TValue>*& t) {
  update_height(t);
  update_size(t);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::rotate_right(RBTNode<TKey, TValue>* t) {
  RBTNode<TKey, TValue>* y = t->left;
  t->left = y->right;
  update(t);
  if (y->right != nullptr)
    y->right->parent = t;
  y->parent = t->parent;
  if (t->parent == nullptr)
    root = y;
  else if (t == t->parent->left)
    t->parent->left = y;
  else
    t->parent->right = y;
  update(t->parent);
  y->right = t;
  update(y);
  t->parent = y;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::rotate_left(RBTNode<TKey, TValue>* t) {
  RBTNode<TKey, TValue>* y = t->right;
  t->right = y->left;
  update(t);
  if (y->left != nullptr)
    y->left->parent = t;
  y->parent = t->parent;
  if (t->parent == nullptr)
    root = y;
  else if (t == t->parent->left)
    t->parent->left = y;
  else
    t->parent->right = y;
  update(t->parent);
  y->left = t;
  update(y);
  t->parent = y;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::insert(TKey key, TValue value, RBTNode<TKey, TValue>* t) {
  RBTNode<TKey, TValue>* z = new RBTNode<TKey, TValue>(key, value);
  RBTNode<TKey, TValue>* y = nullptr;
  RBTNode<TKey, TValue>* x = root;
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
  update(y);
  z->left = nullptr;
  z->right = nullptr;
  z->color = RED;
  update(z);
  insert_fixup(z);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::insert_fixup(RBTNode<TKey, TValue>* z) {
  while (z && z->parent && z->parent->color == RED) {
    update(z);
    update(z->parent);
    if (z->parent == z->parent->parent->left) {
      RBTNode<TKey, TValue>* y = z->parent->parent->right;
      if (y && y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->right) {
          z = z->parent;
          rotate_left(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rotate_right(z->parent->parent);
      }
    }
    else {
      RBTNode<TKey, TValue>* y = z->parent->parent->left;
      if (y && y->color == RED) {
        z->parent->parent->color = RED;
        z->parent->color = BLACK;
        y->color = BLACK;
        z = z->parent->parent;
      }
      else {
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
  while (z) {
    update(z);
    z = z->parent;
  }
  root->color = BLACK;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::transplant(RBTNode<TKey, TValue>* u, RBTNode<TKey, TValue>* v) {
  if (u->parent == nullptr)
    root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  update(u->parent);
  if (v && v->parent)
    v->parent = u->parent;
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::remove(TKey key, RBTNode<TKey, TValue>* t) {
  RBTNode<TKey, TValue>* z = t;
  while (z && z->key != key) {
    if (key < z->key)
      z = z->left;
    else
      z = z->right;
  }
  if (!z)
    return;

  RBTNode<TKey, TValue>* y = z;
  RBTNode<TKey, TValue>* x;
  Color y_orig_color = y->color;
  if (z->left == nullptr) {
    x = z->right;
    transplant(z, z->right);
  }
  else if (z->right == nullptr) {
    x = z->left;
    transplant(z, z->left);
  }
  else {
    y = getMinNode(z->right);
    y_orig_color = y->color;
    x = y->right;
    if (y->parent == z) {
      if (x)
        x->parent = y;
    }
    else {
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
      update(y);
    }
    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
    update(y);
  }
  if (y_orig_color == BLACK)
    remove_fixup(x);
  update(x);
  while (z) {
    update(z);
    z = z->parent;
  }
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::remove_fixup(RBTNode<TKey, TValue>* x) {
  while (x && x != root && x->color == BLACK) {
    if (x && x == x->parent->left) {
      RBTNode<TKey, TValue>* w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotate_left(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      }
      else {
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
    }
    else {
      RBTNode<TKey, TValue>* w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotate_right(x->parent);
        w = x->parent->left;
      }
      if ((w->left == nullptr || w->left->color == BLACK) && (w->right == nullptr || w->right->color == BLACK)) {
        w->color = RED;
        x = x->parent;
      }
      else {
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
  while (x) {
    update(x);
    x = x->parent;
  }
}

template <typename TKey, typename TValue>
RBTNode<TKey, TValue>* RedBlackTree<TKey, TValue>::getMinNode(RBTNode<TKey, TValue>* t) {
  RBTNode<TKey, TValue>* p = t->parent;
  while (t) {
    p = t;
    t = t->left;
  }
  return p;
}

template <typename TKey, typename TValue>
bool RedBlackTree<TKey, TValue>::contains(TKey key, RBTNode<TKey, TValue>* t) {
  if (!t)
    return false;
  if (key == t->key)
    return true;
  if (key < t->key)
    return contains(key, t->left);
  return contains(key, t->right);
}

template<typename TKey, typename TValue>
inline TValue RedBlackTree<TKey, TValue>::get_value(TKey key, RBTNode<TKey, TValue>* t) {
  if (!t)
    return TValue();
  if (key == t->key)
    return t->value;
  if (key < t->key)
    return get_value(key, t->left);
  return get_value(key, t->right);
}

template <typename TKey, typename TValue>
int RedBlackTree<TKey, TValue>::find_index(TKey key, RBTNode<TKey, TValue>* &t) {
  if (!t)
    return 0;
  if (key == t->key)
    return get_size(t->left);
  if (key < t->key)
    return find_index(key, t->left);
  return get_size(t->left) + 1 + find_index(key, t->right);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::show(RBTNode<TKey, TValue>* p, int indent) {
  if (!p)
    return;
  show(p->right, indent + 4);
  cout << setw(indent) << p->key << "," << p->value << "\n";
  show(p->left, indent + 4);
}

template<typename TKey, typename TValue>
inline void RedBlackTree<TKey, TValue>::show_by_section(RBTNode<TKey, TValue>* p, int indent) {
  if (!p)
    return;
  show_by_section(p->right, indent + 4);
  cout << setw(indent) << p->key << ":";
  for (int i = 0; i < (int)p->value.size(); ++i)
    cout << p->value[i] << ",";
  cout << "\n";
  show_by_section(p->left, indent + 4);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::insert(TKey key, TValue value) {
  insert(key, value, root);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::remove(TKey key) {
  remove(key, root);
}

template <typename TKey, typename TValue>
bool RedBlackTree<TKey, TValue>::contains(TKey key) {
  return contains(key, root);
}

template <typename TKey, typename TValue>
int RedBlackTree<TKey, TValue>::find_index(TKey key) {
  return find_index(key, root);
}

template<typename TKey, typename TValue>
inline TValue RedBlackTree<TKey, TValue>::get_value(TKey key) {
  return get_value(key, root);
}

template<typename TKey, typename TValue>
inline void RedBlackTree<TKey, TValue>::show_by_section() {
  show_by_section(root);
}

template <typename TKey, typename TValue>
void RedBlackTree<TKey, TValue>::show() {
  show(root);
}

#endif /* REDBLACKTREE_H */