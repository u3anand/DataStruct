#include <bits/stdc++.h>

using namespace std;


class Node {
    public:
    int key;
    int value;
    int height;
    Node* left;
    Node* right;
    Node* parent;
};

void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->key;
        if (root->parent == nullptr) {
            cout << " P : null" << endl;
        } else {
            cout << " P : " << root->parent->key << endl;
        }
        inorder(root->right);
    }
}


class AVL {
    public:
    // constructor
    Node* root;
    // helper to delete the tree
    void clearAVL(Node* root) {
        if (root == nullptr) {
            return;
        }
        clearAVL(root->left);
        clearAVL(root->right);
        delete root;
    }
    AVL() {
        root = nullptr;
    }
    // destructor
    ~AVL() {
        clearAVL(root);
    }
    void setHeight(Node* tmp) {
        if (tmp == nullptr) {
            return;
        }
        tmp->height = 1 + max(height(tmp->left), height(tmp->right));
    }
    // right rotation
    Node* rotate_right(Node* z) {
        Node* y = z->left;
        z->left = y->right;
        y->right = z;
        setHeight(z);
        setHeight(y);
        return y;
    }
    // left rotation
    Node* rotate_left(Node* z) {
        Node* y = z->right;
        z->right = y->left;
        y->left = z;
        setHeight(z);
        setHeight(y);
        return y;
    } 

    int height(Node *node) {  
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    } 

    int height_diff(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }
    Node* addNode(int key, int value) {
        Node* tmp = new Node;
        tmp->key = key;
        tmp->value = value;
        tmp->height = 0;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = nullptr;
        return tmp;
    }

    Node* bst_insert(Node* root, int key, int value) {
        if (root == nullptr) {
            return addNode(key, value);
        } 
        if (key < root->key) {
            Node* l = bst_insert(root->left, key, value);
            root->left = l;
            l->parent = root;
            setHeight(l);
            setHeight(root);

        } else if (key > root->key)  {
            Node* r = bst_insert(root->right, key, value);
            root->right = r;
            r->parent = root;   
            setHeight(r);
            setHeight(root);
        }
        return root;
    }
    
    // restructure based on rotations
    Node* restructure(Node* x, Node* y, Node* z) {
        if (z->left == y && y->left == x) { // case of LL
            return rotate_right(z);
        
        } else if (z->left == y && y->right == x) { //case of LR
            z->left = rotate_left(y);
            return rotate_right(z);
       
        } else if (z->right == y && y->left == x) { // case of RL
            z->right = rotate_right(y);
            return rotate_left(z);
        
        } else if (z->right == y && y->right == x) { // case of RR
            return rotate_left(z);
        }
    }

    // insert return the number of key comparisions
    void insert(int key, int value) {
        if (root == nullptr) {
            root = bst_insert(root, key, value);
            return;
        }
        Node* z = bst_insert(root, key, value);
        while(z != nullptr) {
            if (height_diff(z) > 1) {
                Node* y = z->left;
                Node* x;
                // prefer one rotation
                int hd = height_diff(y);
                if (hd >= 1) {
                    x = y->left;
                } else if (hd <= -1) {
                    x = y->right;
                } else {
                    x = y->left;
                }
                /*cout << z->key << endl;
                if (z == root) {
                    cout << "eq" << endl;
                }*/
                z = restructure(x, y, z);
                /*if (z == root) {
                    cout << "eq" << endl;
                }
                cout << root->key << endl;*/
                //inorder(root);
                break; // only one restructure needed
            }
            if (height_diff(z) < -1) {
                Node* y = z->right;
                Node* x;
                // prefer one rotation
                int hd = height_diff(y);
                if (hd >= 1) {
                    x = y->left;
                } else if (hd <= -1) {
                    x = y->right;
                } else {
                    x = y->right;
                }
                z = restructure(x, y, z);
                break;
            }
            setHeight(z);
            z = z->parent;
        }
    }

    Node* bst_delete_key(Node* root, int key) {
        if (root == nullptr) {
            return root;
        } 
        if (root->key < key) {
            root->right = bst_delete_key(root->right, key);
        } else if (root->key > key) {
            root->left = bst_delete_key(root->left, key);
        } else {
            if (root->left == nullptr) {
                Node* right = root->right;
                right->parent = root->parent;
                delete root;
                return right;
            } else if (root->right == nullptr) {
                Node* left = root->left;
                left->parent = root->parent;
                delete root;
                return left;
            } else {
                Node* tmp = root->left;
                while(tmp->right != nullptr) { // predecessor
                    tmp = tmp->right;
                }
                root->key = tmp->key;
                delete tmp;
            }
        }
        return root;
    }

    void delete_key(int key) {
        Node* z = bst_delete_key(root, key);
        while(z != nullptr) {
            if (height_diff(z) > 1) {
                Node* y = z->left;
                Node* x = nullptr;
                // prefer one rotation
                int hd = height_diff(y);
                if (hd >= 1) {
                    Node* x = y->left;
                } else if (hd <= -1) {
                    Node* x = y->right;
                } else {
                    Node* x = y->left;
                }
                z = restructure(x, y, z);
            }
            if (height_diff(z) < -1) {
                Node* y = z->right;
                Node* x;
                // prefer one rotation
                int hd = height_diff(y);
                if (hd >= 1) {
                    x = y->left;
                } else if (hd <= -1) {
                    x = y->right;
                } else {
                    x = y->right;
                }
                z = restructure(x, y, z);
            }
            setHeight(z);
            z = z->parent;
        }
    }

    void clearAVL(Node* root) {
        if (root == nullptr) {
            return;
        }
        clearAVL(root->left);
        clearAVL(root->right);
        delete root;
    }

    
    Node* search_key(Node* root, int key) {
        if (root == nullptr || root->key == key) {
            return root;
        }
        if (root->key < key) {
            return search_key(root->right, key);
        } else {
            return search_key(root->left, key);
        }
    }

    Node* search(int key) {
        return search_key(root, key);
    }
};


class SkipNode {
    public:
    int key;
    int value;
    SkipNode* next;
    SkipNode(int key, int value, SkipNode* next) {
        key = key;
        value = value;
        next = next;
    }

};

SkipNode* addSkipNode(int key, int value) {
    SkipNode* tmp = new SkipNode(key, value, nullptr);
    return tmp;
}


class Tower {
    public:
    int key;
    int value;
    vector<SkipNode*> tower;
    int level;
    Tower(int key, int value, int level) {
        for(int i = 0; i < level; i++) {
            SkipNode* tmp = addSkipNode(key, value);
            tower.push_back(tmp);
        }
        level = level;
        key = key;
        value = value;
    }
    ~Tower() {
        for(auto &i : tower) {
            delete i;
        }
    }
};


class SkipList {
    public:
    vector<Tower*> skiplist;
    Tower* root;
    int levels;
    SkipList() {
        levels = 1;
        for(int i = 0; i < levels; i++) {
            root->tower.push_back(addSkipNode(INT_MIN, 0));
            root->key = INT_MIN;
            root->value = 0;
        }
        skiplist.push_back(root);
    }
    ~SkipList() {
        delete root;
        for(auto &i : skiplist) {
            delete i;
        }
    }

    vector<SkipNode*> getPredecessors(int k) {
        SkipNode* p = root->tower[levels-1]; // top left sentinal
        vector<SkipNode*> stack;
        stack.push_back(p);
        int index = levels - 2;
        while(p != nullptr && index >= 0) {
            p = skiplist[0]->tower[index];
            while(p->next->key < k) {
                p = p->next;
                stack.push_back(p);
            }  
            index--; 
        }
        return stack;
    }

    SkipNode* search(int k) {
        vector<SkipNode*> P = getPredecessors(k);
        SkipNode* top = P[P.size()-1];
        if (top->next->key == k) {
            return top->next;
        } else {
            return nullptr;
        }
    }


};


int main() {
    string cmd;
    AVL avl;
    while(cin >> cmd) {
        if (cmd == "i") {
            int key; int val; int coin;
            cin >> key >> val >> coin;
            avl.insert(key, val);
            //skiplist.insert(key, value, coin);

        } else if (cmd == "d") {
            int key; cin >> key;
            avl.delete_key(key);
            //skiplist.delete(key);

        } else if (cmd == "savl") {
            int key; cin >> key;
            Node* tmp = avl.search(key);
            cout << tmp->value;

        } else if (cmd == "ssl") {
            int key; cin >> key;
            //skiplist.search(key);

        } else if (cmd == "stats") {


        } else if (cmd == "r") {
            //reinit avl and skiplist
        }
    }
    //avl.clearAVL(avl.root);

}