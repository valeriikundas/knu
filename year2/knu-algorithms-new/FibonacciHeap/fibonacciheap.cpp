#include <iostream>
#include <array>
#include <cassert>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <string>
#include <queue>
#include <utility>
#include <vector>

static const int kNMax = 20;

std::string random_string(int n = 3) {
  std::string s = "";
  for (int i = 0; i < n; ++i)
    s += ('a' + rand() % 26);
  return s;
}

template <typename TKey, typename TValue>
struct Data {
  TKey key;
  TValue value;
  Data<TKey, TValue>* parent = nullptr;
  Data<TKey, TValue>* child = nullptr;
  Data<TKey, TValue>* left = nullptr;
  Data<TKey, TValue>* right = nullptr;
  int degree = 0; //children count
  bool mark = false; //shows whether this Node was losing children since it became child of some other Node

  Data() {}
  Data(const Data& data) {
    key = data.key;
    value = data.value;
  }
  Data(std::pair<TKey, TValue> data) : key(data.first), value(data.second) {}
  Data(TKey k, TValue v) : key(k), value(v), parent(nullptr), child(nullptr), left(nullptr), right(nullptr), degree(0), mark(0) {}

  std::pair<TKey, TValue> get_data() {
    return std::make_pair(key, value);
  }
};

template <typename TKey, typename TValue>
class FibonacciHeap {

public:
  /*std::string as_string() {
    std::string res = show_to_string(min_root, "", true);
    return res;
  }*/
  /*std::string show_to_string(Data<TKey, TValue>* node, std::string before = "", bool is_first = true)
  {
    if (!node)
      return before;
    std::string res = before;
    if (res.size() > 100)
      return res;
    res += " " + std::to_string(node->key);

    if (node->child != nullptr) {
      res += show_to_string(node->child, res, true);
    }
    if (res.size() > 100)
      return res;

    if (is_first) {
      Data<TKey, TValue>* first = node->right;
      while (first != node) {
        res += show_to_string(first, res, false);
        if (res.size() > 100)
          return res;
        first = first->right;
      }
    }
    return res;
  }*/
  explicit FibonacciHeap() {
    min_root = nullptr;
    root_count = 0;
  }
  explicit FibonacciHeap(std::pair<TKey, TValue> data) {
    min_root = new Data<TKey, TValue>(data);
    min_root->left = min_root->right = min_root;
    root_count = 1;
  }
  void insert(TKey key) {
    FibonacciHeap<TKey, TValue> other(std::make_pair(key, ""));
    merge(&other);
  }
  void insert(TKey key, TValue value) {
    FibonacciHeap<TKey, TValue> other(std::make_pair(key, value));
    merge(&other);
  }
  std::pair<TKey, TValue> extract_min() {
    if (!min_root)
      return std::make_pair(TKey(), TValue());

    std::pair<TKey, TValue> res = get_min()->get_data();

    Data<TKey, TValue>* old_min_root = min_root;
    Data<TKey, TValue>* child = min_root->child;
    unParentAll(child);
    min_root->child = nullptr;
    merge_with_node(child);

    if (old_min_root->right == old_min_root) {
      min_root = child;
      return res;
    }

    min_root->left->right = min_root->right;
    min_root->right->left = min_root->left;

    min_root = min_root->right;
    consolidate();

    return res;
  }
  Data<TKey, TValue>* get_min() {
    if (min_root)
      return min_root;
    return new Data<TKey, TValue>(TKey(), TValue());
  }
  void merge(FibonacciHeap<TKey, TValue>* other) {
    if (this == nullptr)
      return;
    if (other == nullptr)
      return;
    if (min_root == nullptr) {
      min_root = other->get_min();
      root_count = other->root_count;
      return;
    }

    Data<TKey, TValue>* other_min_root = other->get_min();

    Data<TKey, TValue>* this_tail_node = min_root->left;
    Data<TKey, TValue>* other_tail_node = other_min_root->left;

    min_root->left = other_tail_node;
    other_tail_node->right = min_root;

    this_tail_node->right = other_min_root;
    other_min_root->left = this_tail_node;

    if (other_min_root->key < min_root->key)
      min_root = other_min_root;
  }
  void show() {
    assert(false);
    show(min_root);
    std::cout << "\n";
  }
  void show_by_category() {
    show_by_category(min_root);
    std::cout << "\n";
  }
  void show_by_product() {
    show_by_product(min_root);
    std::cout << "\n";
  }

  bool empty() {
    return min_root == nullptr;
  }

private:
  void consolidate() {
    std::array<Data<TKey, TValue>*, kNMax> vertices = { nullptr };

    Data<TKey, TValue>* current = nullptr;
    Data<TKey, TValue>* next = min_root;
    bool go_on = true;
    while (go_on && next) {
      current = next;
      if (current)
        next = current->right;
      if (current == next) {
        go_on = false;
        next = nullptr;
      }
      isolate_node(current);

      if (vertices[current->degree] == nullptr) {
        vertices[current->degree] = current;
      }
      else {
        Data<TKey, TValue>* conflicted = vertices[current->degree];
        vertices[current->degree] = nullptr;

        Data<TKey, TValue>* future_parent;
        Data<TKey, TValue>* future_child;
        if (current->key < conflicted->key) {
          future_parent = current;
          future_child = conflicted;
        }
        else {
          future_parent = conflicted;
          future_child = current;
        }

        unParentAll(future_parent->child);
        merge_nodes(future_child, future_parent->child);

        future_parent->child = future_child;
        future_child->parent = future_parent;
        future_parent->degree += 1;

        if (next)
          future_parent->left = next->left;
        else
          next = future_parent;
        future_parent->right = next;
        if (next)
          next->left = future_parent;

        next = future_parent;
        go_on = true;
      }
    }

    merge_nodes_from_array(vertices);
  }
  void parentAll(Data<TKey, TValue>* node, Data<TKey, TValue>* parent) {
    if (!node)
      return;
    Data<TKey, TValue>* first = node;
    do {
      first->parent = parent;
      first = first->right;
    } while (first != node);
  }
  void isolate_node(Data<TKey, TValue>* node) {
    if (!node)
      return;
    if (node->right)
      node->right->left = node->left;
    if (node->left)
      node->left->right = node->right;
    node->left = node->right = node;
  }
  void merge_nodes_from_array(std::array<Data<TKey, TValue>*, kNMax> vertices) {
    min_root = nullptr;
    root_count = 0;
    Data<TKey, TValue>* node = nullptr;
    for (int i = 0; i < kNMax; ++i) {
      if (vertices[i])
        root_count += 1;
      if (!min_root || (vertices[i] && vertices[i]->key < min_root->key)) {
        min_root = vertices[i];
      }
      node = merge_nodes(node, vertices[i]);
    }
  }
  static Data<TKey, TValue>* merge_nodes(Data<TKey, TValue>* x, Data<TKey, TValue>* y) {
    if (!x)
      return y;
    if (!y)
      return x;

    assert(x->right);
    assert(y);
    x->right->left = y->left;
    assert(y->left);
    assert(x);
    y->left->right = x->right;

    x->right = y;
    y->left = x;

    return x;
  }
  std::pair<Data<TKey, TValue>*, int> find_min_node_and_count_nodes(Data<TKey, TValue>* node) {
    Data<TKey, TValue>* min = node;
    Data<TKey, TValue>* cur = node->right;
    int count = 1;

    while (cur != node) {
      if (cur->key < min->key) {
        min = cur;
      }
      cur = cur->right;
      count += 1;
    }
    return std::make_pair(min, count);
  }
  void merge_with_node(Data<TKey, TValue>* node) {
    if (node == nullptr)
      return;
    if (min_root == nullptr) {
      std::pair<Data<TKey, TValue>*, int> result = find_min_node_and_count_nodes(node);
      min_root = result.first;
      root_count = result.second;
      return;
    }

    Data<TKey, TValue>* this_tail_node = min_root->left;
    Data<TKey, TValue>* other_tail_node = node->left;

    min_root->left = other_tail_node;
    other_tail_node->right = min_root;

    this_tail_node->right = node;
    node->left = this_tail_node;
  }
  void unParentAll(Data<TKey, TValue>* node) {
    if (!node)
      return;
    Data<TKey, TValue>* first = node;
    do {
      first->parent = nullptr;
      first = first->right;
    } while (first != node);
  }
  void show(Data<TKey, TValue>* node, int shift = 0, bool is_first = true) {
    if (!node)
      return;

    std::cout << std::setw(shift) << node->key << "\n";

    if (node->child != nullptr)
      show(node->child, shift + 4);

    if (is_first) {
      Data<TKey, TValue>* first = node->right;
      while (first != node) {
        show(first, shift, false);
        first = first->right;
      }
    }
  }
  void show_by_category(Data<TKey, TValue>* node, int shift = 0, bool is_first = true) {
    if (!node)
      return;

    std::cout << std::setw(shift) << node->key << ": ";
    for (int i = 0; i < (int)node->value.size(); ++i)
      std::cout << node->value[i] << " ";
    std::cout << "\n";

    if (node->child != nullptr)
      show_by_category(node->child, shift + 4);

    if (is_first) {
      Data<TKey, TValue>* first = node->right;
      while (first != node) {
        show_by_category(first, shift, false);
        first = first->right;
      }
    }
  }
  void show_by_product(Data<TKey, TValue>* node, int shift = 0, bool is_first = true) {
    if (!node)
      return;

    std::cout << std::setw(shift) << "(" << node->key << ": " << node->value << ")" << "\n";

    if (node->child != nullptr)
      show_by_product(node->child, shift + 4);

    if (is_first) {
      Data<TKey, TValue>* first = node->right;
      while (first != node) {
        show_by_product(first, shift, false);
        first = first->right;
      }
    }
  }

  Data<TKey, TValue>* min_root;
  int root_count;
};

std::vector<std::string> generate_products(int n = 5) {
  std::vector<std::string> res;
  for (int i = 0; i < n; ++i)
    res.push_back(random_string());
  return res;
}

int main() {
  /*std::vector<std::string> category_names = { "diary", "drinks", "grocery", "meat", "appliance", "patio", "garden", "toys" };
  std::vector<std::pair<std::string, std::vector<std::string>>> categories;
  for (int i = 0; i < 5; ++i) {
    std::vector<std::string> products_for_one_category;
    for (int j = 0; j < 3; ++j)
      products_for_one_category.push_back(random_string());

    categories.push_back(std::make_pair(category_names[i], products_for_one_category));
  }*/

  FibonacciHeap<std::string, std::vector<std::string>> warehouse_by_category;
  FibonacciHeap<std::string, std::string> warehouse_by_product;

  while (true) {
    std::cout << "1-insert category, 2-get min category, 3-extract min category, 4-get min product, 5-extract min product, 6-exit\n";

    int c;
    std::cin >> c;
    switch (c) {
      case 1:
      {
        std::string category;
        std::cin >> category;
        auto products = generate_products();
        warehouse_by_category.insert(category, products);
        for (auto product : products) {
          warehouse_by_product.insert(product, category);
        }
        break;
      }
      case 2:
      {
        std::pair<std::string, std::vector<std::string>> min_category = warehouse_by_category.get_min()->get_data();
        std::cout << "category: " << min_category.first << "\n";
        std::vector<std::string> products = min_category.second;
        std::cout << "products: ";
        for (int i = 0; i < (int)products.size(); ++i)
          std::cout << products[i] << " ";
        std::cout << "\n";
        break;
      }
      case 3:
      {
        std::pair<std::string, std::vector<std::string>> res = warehouse_by_category.extract_min();
        std::cout << "extracted category: " << res.first << "\n";
        break;
      }
      case 4:
      {
        std::pair<std::string, std::string> min_product = warehouse_by_product.get_min()->get_data();
        std::cout << "min product: " << min_product.first << " from category: " << min_product.second << "\n";
        break;
      }
      case 5:
      {
        std::pair<std::string, std::string> min_product = warehouse_by_product.extract_min();
        std::cout << "min product: " << min_product.first << " from category: " << min_product.second << "\n";
        break;
      }
      default:
      {
        system("pause");
        exit(0);
      }
    }
    std::cout << "\n";
    warehouse_by_category.show_by_category();
    std::cout << "\n";
    warehouse_by_product.show_by_product();
    std::cout << "\n\n";
  }

  return 0;
}
