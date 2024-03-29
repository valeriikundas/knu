#include "stdafx.h"
#include <iostream>
#include <iomanip>

using namespace std;

const int infinity = (int)1e9;

struct Node
{
    int key = 0;
    Node* parent = nullptr;
    Node* child = nullptr;
    Node* next = nullptr;
    Node* prev = nullptr;
    int degree = 0;
};

class BinHeap
{
private:
    Node* root_head;

    void show(Node* node, Node* head_of_cur_list, int shift = 0)
    {
        if (!node)
            return;

        cout << setw(shift) << node->key << "\n";
        show(node->child, node->child, shift + 4);

        if (node->next != head_of_cur_list)
            show(node->next, head_of_cur_list, shift);
    }

public:
    BinHeap()
    {
        root_head = nullptr;
    }

    BinHeap(int key)
    {
        root_head = new Node;
        root_head->key = key;
    }

    Node* get_root_head()
    {
        return root_head;
    }

    void insert(int key)
    {
        BinHeap bh(key);
        merge(bh);
    }

    int get_min()
    {
        if (!root_head)
            return infinity;

        int min = root_head->key;
        Node* cur = root_head->next;
        while (cur)
        {
            if (cur->key < min)
                min = cur->key;
            cur = cur->next;
        }

        return min;
    }

    int extract_min()
    {
        if (!root_head)
            return infinity;

        Node* min = root_head;
        Node* cur = root_head->next;
        while (cur)
        {
            if (cur->key < min->key)
                min = cur;
            cur = cur->next;
        }
        int minkey = min->key;

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
            
            Node* cur_next = cur->next;
            if (cur->next)
                cur->next->prev = nullptr;
            cur->next = nullptr;

            BinHeap other;
            other.root_head = cur;

            merge(other);
            
            cur = cur_next;
        }

        delete min;

        return minkey;
    }

    void merge(BinHeap other) {
        if (!other.get_root_head())
            return;

        if (!root_head)
        {
            root_head = other.get_root_head();
            return;
        }

        Node* head = nullptr;

        Node* cur1 = root_head;
        Node* cur2 = other.get_root_head();

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
        Node* tail = root_head;
        Node* cur = head;
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
                        Node* new_child = cur->next;
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
                        Node* new_child = cur;
                        Node* new_parent = cur->next;
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


int main()
{
    BinHeap bh;
    while (true)
    {
        cout << "1-insert, 2-getmin, 3-extractmin\n";

        int a;
        cin >> a;

        if (a == 1)
        {
            int b;
            cin >> b;
            bh.insert(b);
        }
        else
            if (a == 2)
                cout << bh.get_min() << "\n";
            else
                if (a == 3)
                    cout << bh.extract_min() << "\n";
        bh.show();
        cout << "\n\n";

    }
    return 0;
}