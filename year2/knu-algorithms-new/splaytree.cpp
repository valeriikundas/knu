#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

class Product {
private:
    string title;
    float price;
    string section;

public:
    Product(string title = "", float price = 0.0, string section = "") : title(title), price(price), section(section) {}

    string get_title() { return title; }
    float get_price() { return price; }
    string get_section() { return section; }
};

class Section {
private:
    string title;
    vector<Product> products;

public:
    Section(string title = "", vector<Product> products = vector<Product>()) : title(title), products(products) {}

    string get_title() {
        return title;
    }

    vector<Product> get_products() {
        return products;
    }
};

class Warehouse {
    string title;
    vector<Section> sections;

    Warehouse(string title, vector<Section> sections) : title(title), sections(sections) {}
};

string tostring(int i) {
    string r = "";
    while (i) {
        r += ('0' + i % 10);
        i /= 10;
    }
    reverse(r.begin(), r.end());
    return r;
}

vector<Product> generateManyProducts(string section_title, int n = 1) {
    vector<Product> products;
    if (!n) n = rand() % 5 + 1;
    for (int i = 0; i < n; ++i) {
        Product new_products = Product("item" + tostring(rand()), float(rand() % 100), section_title);
        products.push_back(new_products);
    }
    return products;
}

vector<Section> generateManySections(int n = 1) {
    vector<Section> sections;
    if (!n) n = rand() % 10 + 1;
    for (int i = 0; i < n; ++i) {
        string section_title = "sect" + tostring(rand());
        Section new_section = Section(section_title, generateManyProducts(section_title, rand() % 5 + 1));
        sections.push_back(new_section);
    }
    return sections;
}

void show_section(Section section) {
    cout << section.get_title() << ": ";
    vector<Product> products = section.get_products();
    for (int i = 0; i < (int)products.size(); ++i)
        cout << "(" << products[i].get_title() << ", " << products[i].get_price() << ") ";
    cout << "\n";
}

template <typename T_key, typename T_value>
struct Node
{
    T_key key;
    T_value value;
    Node<T_key, T_value> *left;
    Node<T_key, T_value> *right;
    Node<T_key, T_value> *parent;
public:
    Node<T_key, T_value>(T_key key = 0, T_value value = 0, Node<T_key, T_value> *l = nullptr, Node<T_key, T_value> *r = nullptr, Node<T_key, T_value> *p = nullptr):
        key(key), value(value), left(l), right(r), parent(p) {}
};

template <typename T_key, typename T_value>
class SplayTree
{
public:
    SplayTree()
    {
        root = nullptr;
    }

    bool find(T_key key)
    {
        return find(key, root) != nullptr;
    }

    void insert(T_key key, T_value value)
    {
        insert(key, value, root);
    }

    void remove(T_key key)
    {
        remove(key, root);
    }

    void show_by_products()
    {
        show_by_products(root, 0);
        cout << "\n";
    }

    void show_by_section() {
        show_by_section(root, 0);
        cout << "\n";
    }

    void inorder()
    {
        inorder(root);
        cout << "\n";
    }

    void show_section_by_title(string section_title) {
        bool found = find(section_title);
        if (!found) {
            cout << "no such section\n";
            return;
        }
        vector<Product> products = root->value;
        for (int i = 0; i < (int)products.size(); ++i) {
            cout << "(" << products[i].get_title() << ", " << products[i].get_price() << ") ";
        }
        cout << "\n";
    }

    vector<Product> get_products_by_section_title(string section_title) {
        bool found = find(section_title);
        if (!found) {
            return vector<Product>();
        }
        vector<Product> products = root->value;
        return products;
    }

    void show_products_section(string product_title) {
        bool found = find(product_title);
        if (!found) {
            return;
        }
        cout << product_title << " " << root->value << "\n";
    }

private:
    Node<T_key, T_value> *root;

    Node<T_key, T_value>* rotate_right(Node<T_key, T_value> *node)
    {
        Node<T_key, T_value> *tmp = node->left;
        if (tmp && tmp->right)
            node->left = tmp->right;
        else
            node->left = nullptr;
        if (tmp)
            tmp->right = node;
        if (node == root)
            root = tmp;
        return tmp;
    }

    Node<T_key, T_value>* rotate_left(Node<T_key, T_value> *node)
    {
        Node<T_key, T_value> *tmp = node->right;
        if (tmp && tmp->left)
            node->right = tmp->left;
        else
            node->right = nullptr;
        if (tmp)
            tmp->left = node;
        if (node == root)
            root = tmp;
        return tmp;
    }

    Node<T_key, T_value>* splay(Node<T_key, T_value> *node, T_key key)
    {
        if (!node || node->key == key)
            return node;

        if (key < node->key)
        {
            if (!node->left)
                return node;

            if (key < node->left->key)
            {
                //left-left
                node->left->left = splay(node->left->left, key);

                node = rotate_right(node);
            }
            else if (node->left->key < key)
            {
                //left-right
                node->left->right = splay(node->left->right, key);

                if (node->left->right != nullptr)
                    node->left = rotate_left(node->left);
            }

            if (node->left)
                return rotate_right(node);
            else
                return node;

        }
        else
        {
            if (!node->right)
                return node;

            if (key < node->right->key)
            {
                //right-left
                node->right->left = splay(node->right->left, key);

                if (node->right->left != nullptr)
                    node->right = rotate_right(node->right);
            }
            else if (node->right->key < key)
            {
                //right-right
                node->right->right = splay(node->right->right, key);

                node = rotate_left(node);
            }

            if (node->right)
                return rotate_left(node);
            else
                return node;
        }
    }

    Node<T_key, T_value>* find(T_key key, Node<T_key, T_value> *node)
    {
        splay(root, key);
        return root;
    }

    void insert(T_key key, T_value value, Node<T_key, T_value> *node)
    {
        if (!node)
        {
            root = new Node<T_key, T_value>(key, value);
            return;
        }

        if (key < node->key)
        {
            if (!node->left)
                node->left = new Node<T_key, T_value>(key, value);
            else
                insert(key, value, node->left);
        }
        else
        {
            if (!node->right)
                node->right = new Node<T_key, T_value>(key, value);
            else
                insert(key, value, node->right);
        }

        splay(root, key);
    }

    void remove(T_key key, Node<T_key, T_value> *node)
    {
        if (!node)
            return;
        splay(node, key);

        if (root->key != key)
            return;

        if (root->right)
        {
            Node<T_key, T_value> *cur = root->right;
            Node<T_key, T_value> *p = root;

            while (cur->left != nullptr)
            {
                p = cur;
                cur = cur->left;
            }

            root->key = cur->key;
            if (cur == p->left)
                p->left = cur->right;
            else
                p->right = cur->right;
            cur = cur->right;
        }
        else
            root = root->left;
    }

    void show_by_products(Node<T_key, T_value> *node, int shift = 0)
    {
        if (!node)
            return;

        show_by_products(node->right, shift + 6);
        if (shift > 0)
            cout << setw(shift) << " ";
        cout << node->key << ": ";

        cout << node->value << "\n";

        show_by_products(node->left, shift + 6);
    }

    void show_by_section(Node<T_key, T_value> *node, int shift = 0)
    {
        if (!node)
            return;

        show_by_section(node->right, shift + 6);
        if (shift > 0)
            cout << setw(shift) << " ";
        cout << node->key << ": ";
        vector<Product> products = node->value;
        for (int i = 0; i < (int)products.size(); ++i)
            cout << "(" << products[i].get_title() << ", " << products[i].get_price() << ") ";
        cout << "\n";
        show_by_section(node->left, shift + 6);
    }
};

int main()
{
    srand(time(0));

    vector<Section> sections = generateManySections(4);

    SplayTree<string, vector<Product>> warehouse_by_section_titles; //section_title, vector of products
    SplayTree<string, string>  warehose_by_products_titles; //products_title, section_title

    while (true)
    {
        cout << "1-insert new section, 2-remove section, 3-find section, 4-show section, 5-exit, 6-find products\n";
        int code;
        cin >> code;
        if (code == 5)
            break;

        string section_title;
        cin >> section_title;

        vector<Product> products = generateManyProducts(section_title, rand() % 5);

        if (code == 1) {
            warehouse_by_section_titles.insert(section_title, products);
            for (int i = 0; i < (int)products.size(); ++i)
                warehose_by_products_titles.insert(products[i].get_title(), products[i].get_section());
            cout << "\n";
        }
        else if (code == 2) {
            vector<Product> products = warehouse_by_section_titles.get_products_by_section_title(section_title);
            for (int i = 0; i < (int)products.size(); ++i) {
                warehose_by_products_titles.remove(products[i].get_title());
            }
            warehouse_by_section_titles.remove(section_title);
            cout << "\n";
        }
        else if (code == 3) {
            cout << warehouse_by_section_titles.find(section_title) << "\n";
            cout << "\n";
        }
        else if (code == 4) {
            warehouse_by_section_titles.show_section_by_title(section_title);
            cout << "\n";
        }
        else if (code == 6) {
            string product_title = section_title;
            warehose_by_products_titles.show_products_section(product_title);
            cout << "\n";
        }
        warehouse_by_section_titles.show_by_section();
        cout << "\n";
        warehose_by_products_titles.show_by_products();
        cout << "\n";
    }

    return 0;
}
