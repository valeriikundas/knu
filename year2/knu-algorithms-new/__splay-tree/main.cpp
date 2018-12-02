#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;
    Node *parent;
public:
    Node(int key = 0, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr): key(key), left(l), right(r), parent(p) {}
};

class SplayTree
{
public:
    SplayTree()
    {
        root = nullptr;
    }

    bool find(int key)
    {
        return find(key, root) != nullptr;
    }

    void insert(int key)
    {
        insert(key, root);
    }

    void remove(int key)
    {
        remove(key, root);
    }

    void show()
    {
        show(root, 0);
    }

    void inorder()
    {
        inorder(root);
        cout << "\n";
    }

private:
    Node *root;

    Node* rotate_right(Node *node)
    {
        Node *tmp = node->left;
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

    Node* rotate_left(Node *node)
    {
        Node *tmp = node->right;
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

    Node* splay(Node *node, int key)
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
            else if (key > node->left->key)
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
            else if (key > node->right->key)
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

    Node* find(int key, Node *node)
    {
        splay(root, key);
        return root;
    }

    void insert(int key, Node *node)
    {
        if (!node)
        {
            root = new Node(key);
            return;
        }

        if (key < node->key)
        {
            if (!node->left)
                node->left = new Node(key);
            else
                insert(key, node->left);
        }
        else
        {
            if (!node->right)
                node->right = new Node(key);
            else
                insert(key, node->right);
        }

        splay(root, key);
    }

    void remove(int key, Node *node)
    {
        if (!node)
            return;
        splay(node, key);

        if (root->key != key)
            return;

        if (root->right)
        {
            Node *cur = root->right;
            Node *p = root;

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

    void show(Node *node, int shift = 0)
    {
        if (!node)
            return;

        show(node->right, shift + 4);
        cout << setw(shift) << node->key << "\n";
        show(node->left, shift + 4);
    }

    void inorder(Node *node)
    {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }
};

int main()
{
    SplayTree st;

    while (true)
    {
        cout << "1-insert, 2-remove, 3-find, 4-exit\n";
        int code;
        cin >> code;
        if (code == 4)
            break;
        int key;
        cin >> key;
        if (code == 1)
            st.insert(key);
        else if (code == 2)
            st.remove(key);
        else if (code == 3)
            cout << st.find(key) << "\n";
        st.show();
    }

    return 0;
}
