#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

const int MAX_NODE_SIZE = 4;

struct Node
{
    bool is_leaf;
    vector<int> keys;
    vector<int> values;
    vector<Node*> pointers;

    Node(int key, int value)
    {
        is_leaf = true;
        keys.push_back(key);
        values.push_back(value);
    }

    Node(vector<int> keys, vector<int> values) :keys(keys), values(values), is_leaf(true) {}

    Node(int key)
    {
        is_leaf = false;
        keys.push_back(key);
    }
};

   
class BPlusTree
{
private:
    Node* root;

    bool search(int key, Node* node)
    {
        if (!node)
            return false;

        if (key < node->keys[0])
            return search(key, node->pointers[0]);

        for (int i = 0; i + 1 < (int)node->keys.size(); ++i)
            if (node->keys[i] <= key && key < node->keys[i + 1])
                return search(key, node->pointers[i + 1]);

        if (key >= node->keys.back())
            return search(key, node->pointers[node->keys.size()]);
    }

    void insert(int key, int value, Node* node)
    {
        if (!node)
        {
            root = new Node(key, value);
            return;
        }

        if (node->is_leaf == false)
        {
            for (int i = 0; i < (int)node->keys.size(); ++i)
            {
                if (key < node->keys[i])
                {
                    if (node->pointers[i + 1] != nullptr)
                        insert(key, value, node->pointers[i + 1]);
                    else
                    {
                        node->pointers[i + 1] = new Node(key, value);
                    }
                    break;
                }
            }

            insert(key, value, node->pointers.back());
        }
        else
        {
            node->keys.push_back(key);
            node->values.push_back(value);

            int i = node->keys.size() - 1;
            while (i > 0 && node->keys[i] < node->keys[i - 1])
            {
                swap(node->keys[i], node->keys[i - 1]);
                swap(node->values[i], node->values[i - 1]);
                --i;
            }

            if (node->keys.size() > MAX_NODE_SIZE)
            {
                int mid = (int)node->keys.size() / 2;

                vector<int> keys, values;
                for (int i = 0; i < mid; ++i)
                {
                    keys.push_back(node->keys[i]);
                    values.push_back(node->values[i]);
                }
                Node* left = new Node(keys, values);

                keys.clear();
                values.clear();

                for (int i = mid; i < (int)node->keys.size(); ++i)
                {
                    keys.push_back(node->keys[i]);
                    values.push_back(node->values[i]);
                }
                Node* right = new Node(keys, values);

                Node* inner = new Node(right->keys.front());

                node->keys = inner->keys;

                node->pointers.clear();
                node->pointers.push_back(left);
                node->pointers.push_back(right);
            }
        }
    }

    void show(Node* node, int shift = 0)
    {
        if (!node)
            return;

        if (node->is_leaf) 
        {
            for (int i = 0; i < (int)node->keys.size(); ++i)
                cout << node->keys[i] << "  ";
            cout << " ** ";
        }
        else
        {
            if (node->pointers.size() > 0 && node->pointers[0] != nullptr)
            {
                show(node->pointers[0]);

                for (int i = 1; i < (int)node->pointers.size(); ++i)
                {
                    cout << node->keys[i] << " ";
                    show(node->pointers[i]);
                }
            }
        }
    }

public:
    BPlusTree() : root(nullptr) {}
    
    bool search(int key)
    {
        return search(key, root);
    }

    void insert(int key, int value)
    {
        insert(key, value, root);
    }

    void remove(int key)
    {
        //remove(key, root);
    }

    void show()
    {
        show(root);
        cout << "\n";
    }
};

int main()
{
    BPlusTree tree;
    for (int i = 1; i <= 15; ++i)
        tree.insert(rand() % 20, 0);
    tree.show();
    return 0;
}

