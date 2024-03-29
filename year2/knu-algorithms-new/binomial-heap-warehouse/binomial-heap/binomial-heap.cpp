#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

template <typename TKey, typename TValue>
struct Data
{
    TKey key = 0;
    TValue value = 0;
    Data* parent = nullptr;
    Data* child = nullptr;
    Data* next = nullptr;
    Data* prev = nullptr;
    int degree = 0;

public:
    Data<TKey, TValue>(TKey key = {}, TValue value = {}, Data<TKey, TValue>* parent = nullptr, Data<TKey, TValue>* child = nullptr, Data<TKey, TValue>* next = nullptr, Data<TKey, TValue>* prev = nullptr, int degree = 0)
        : key(key), value(value), parent(parent), child(child), prev(prev), next(next), degree(degree)
    {}
};

struct Product : protected Data<string, string>
{
    string name;
    float price;
    string category;

public:
    Product(string name = "", float price = 0, string category = "") : name(name), price(price), category(category) {}

    string get_title() { return name; }

    float get_price() { return price; }
};

/*class Category
{
    string name;
    vector<Product> products;

public:
    Category(string name = "", vector<Product> products = vector<Product>()) : name(name), products(products) {}
};*/

ostream& operator << (ostream& stream, vector<Product> products)
{
    for (int i = 0; i < (int)products.size(); ++i)
        cout << "(" << products[i].get_title() << ", " << products[i].get_price() << ") ";
    return stream;
}

template <typename TKey, typename TValue>
class BinomialHeap
{
private:
    Data<TKey, TValue>* root_head;

    void show(Data<TKey,TValue>* data, Data<TKey,TValue>* head_of_cur_list, int shift = 0)
    {
        if (!data)
            return;

		for (int i = 0; i < shift; ++i) cout << " ";

        cout <</* setw(shift) <<*/ data->key << " : " << data->value << "\n";

        show(data->child, data->child, shift + 5);

        if (data->next != head_of_cur_list)
            show(data->next, head_of_cur_list, shift + 5);
    }

public:
    BinomialHeap()
    {
        root_head = nullptr;
    }

    BinomialHeap(TKey key, TValue value)
    {
        root_head = new Data<TKey, TValue>;
        root_head->key = key;
        root_head->value = value;
    }

    Data<TKey, TValue>* get_root_head()
    {
        return root_head;
    }

    void insert(TKey key, TValue value)
    {
        BinomialHeap bh(key, value);
        merge(bh);
    }

    TKey get_min()
    {
        if (!root_head)
            return 0;

        TKey min = root_head->key;
        Data<TKey, TValue>* cur = root_head->next;
        while (cur)
        {
            if (cur->key < min)
                min = cur->key;
            cur = cur->next;
        }

        return min;
    }

    TKey extract_min()
    {
        if (!root_head)
            return 0;

        Data<TKey, TValue>* min = root_head;
        Data<TKey, TValue>* cur = root_head->next;
        while (cur)
        {
            if (cur->key < min->key)
                min = cur;
            cur = cur->next;
        }
        TKey minkey = min->key;

        if (min->next)
            min->next->prev = min->prev;
        if (min->prev)
            min->prev->next = min->next;

        min->prev = nullptr;
        min->next = nullptr;

        cur = min->child;
        while (cur)
        {
            cur->parent = nullptr;
            
            Data<TKey, TValue>* cur_next = cur->next;
            if (cur->next)
                cur->next->prev = nullptr;
            cur->next = nullptr;

            BinomialHeap other;
            other.root_head = cur;

            merge(other);
            
            cur = cur_next;
        }

        delete min;

        return minkey;
    }

    void merge(BinomialHeap other) {
        if (!other.get_root_head())
            return;

        if (!root_head)
        {
            root_head = other.get_root_head();
            return;
        }

        Data<TKey, TValue>* head = nullptr;

        Data<TKey, TValue>* cur1 = root_head;
        Data<TKey, TValue>* cur2 = other.get_root_head();

        while (cur1 != nullptr && cur2 != nullptr)
        {
            if (cur1->degree < cur2->degree)
            {
                if (!head)
                    head = cur1;
                else
                {
                    head->next = cur1;
                    cur1->prev = head;
                    head = cur1;
                }
                cur1 = cur1->next;
            }
            else
            {
                if (!head)
                    head = cur2;
                else
                {
                    head->next = cur2;
                    cur2->prev = head;
                    head = cur2;
                }
                cur2 = cur2->next;
            }
        }

        while (cur1 != nullptr)
        {
            if (!head) 
                head = cur1;
            else
            {
                head->next = cur1;
                cur1->prev = head;
                head = cur1;
            }
            cur1 = cur1->next;
        }

        while (cur2 != nullptr)
        {
            if (!head)
                head = cur2;
            else
            {
                head->next = cur2;
                cur2->prev = head;
                head = cur2;
            }
            cur2 = cur2->next;
        }

        root_head = nullptr;
        Data<TKey, TValue>* tail = root_head;
        Data<TKey, TValue>* cur = head;
        while (cur && cur->prev)
            cur = cur->prev;

        while (cur)
        {
            if (!cur->next)
            {
                if (!tail)
                    root_head = cur;
                else
                {
                    tail->next = cur;
                    cur->prev = tail;
                }
                cur = cur->next;
            }
            else
            {
                while (cur->next && cur->degree == cur->next->degree)
                {
                    if (cur->key < cur->next->key)
                    {
                        Data<TKey, TValue>* new_child = cur->next;
                        if(cur->next->next)
                            cur->next->next->prev = cur;
                        cur->next = new_child->next;
                        
                        if (cur->child)
                        {
                            new_child->prev = cur->child->prev;
                            new_child->next = cur->child;

                            if (cur->child->prev)
                                cur->child->prev->next = new_child;
                            cur->child->prev = new_child;
                        }
                        else
                        {
                            new_child->next = nullptr;
                            new_child->prev = nullptr;
                            cur->child = new_child;
                        }
                        cur->degree += 1;
                        new_child->parent = cur;
                        cur->child = new_child;
                    }
                    else
                    {
                        Data<TKey, TValue>* new_child = cur;
                        Data<TKey, TValue>* new_parent = cur->next;
                        new_child->parent = new_parent;
                        cur->next->prev = nullptr;

                        if (new_parent->child)
                        {
                            new_child->prev = new_parent->child->prev;
                            new_child->next = new_parent->child;

                            if (new_parent->child->prev)
                                new_parent->child->prev->next = new_child;
                            if (new_parent->child)
                                new_parent->child->prev = new_child;

                            new_parent->degree += 1;
                            new_parent->child = new_child;
                        }
                        else
                        {
                            new_parent->child = new_child;
                            new_parent->degree += 1;
                            new_child->next = nullptr;
                            new_child->prev = nullptr;
                        }

                        cur = new_parent;
                    }
                }

                if (!tail)
                {
                    tail = cur;
                    root_head = tail;
                }
                else
                {
                    tail->next = cur;
                    cur->prev = tail;
                    tail = cur;
                }
                cur = cur->next;
                tail->next = nullptr;
                if(cur)
                    cur->prev = nullptr;
            }
        }
    }

    void show()
    {
        show(root_head, root_head);
        cout << "\n";
    }
};

vector<string> list_of_products;

vector<Product> generateProducts(int n, string category)
{
    vector<Product> products;
    for (int i = 0; i < n; ++i)
    {
        int index = rand() % list_of_products.size();
        Product product = list_of_products[index];
        string product_title = product.get_title();
        products.push_back(Product(product_title, (float)( rand() % 100), category));
    }
    return products;
}


int main()
{
    srand((int)time(0));

    ifstream infile;
    infile.open("file.txt");
    while (!infile.eof())
    {
        string product_title;
        infile >> product_title;
        list_of_products.push_back(product_title);
    }

    BinomialHeap<string, vector<Product>> warehouse_by_category; //category, list of products
    BinomialHeap<string, string> warehouse_by_product; //product, category

    while (true)
    {
        cout << "1-insert new section\n" << "2-get minimum category\n" << "3-extract minimum category()\n" << "4-get minimum product\n";

        int code;
        cin >> code;

        if (code == 1)
        {
            string category;
            cin >> category;

            vector<Product> products = generateProducts(rand() % 10, category);

            warehouse_by_category.insert(category, products);

            for (int i = 0; i < (int)products.size(); ++i)
                warehouse_by_product.insert(products[i].get_title(), category);
        }
        else
            if (code == 2)
                cout << warehouse_by_category.get_min() << "\n";
            else
                if (code == 3)
                    cout << warehouse_by_category.extract_min() << "\n";
                else
                    if (code == 4)
                        cout << warehouse_by_product.get_min() << "\n";

        warehouse_by_category.show();
        cout << "\n\n";
        warehouse_by_product.show();
        cout << "\n\n";
    }
    return 0;
}