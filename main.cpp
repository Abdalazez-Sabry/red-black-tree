#include <iostream>

class RedBlackTree {
public: 
    struct Node {
        int value;
        std::string type;
        Node* left;
        Node* right; 
        Node* parent;
    };
    
    void leftRotate(Node* node) {
        Node* nodeRight = node->right;

        node->right = nodeRight->left;

        if (nodeRight->left != nullptr) {
            nodeRight->left->parent = node;
        }

        nodeRight->parent = node->parent;

        if (node->parent == nullptr) {
            root = nodeRight; 
        }

        if (node->parent->left == node) {
            node->parent->left= nodeRight;
        }

        if (node->parent->right == node) {
            node->parent->right = nodeRight;
        }

        nodeRight->left = node;
        node->parent = nodeRight;
    }

    void rightRotate(Node* node) {
        Node* nodeLeft = node->left; 

        node->left = nodeLeft->right;

        if (nodeLeft->right != nullptr) {
            nodeLeft->right->parent = node;  
        }

        nodeLeft->parent = node->parent;

        if (node->parent == nullptr) {
            root = nodeLeft;
        }

        if (node->parent->left == node) {
            node->parent->left = nodeLeft;
        }

        if (node->parent->right == node) {
            node->parent->right = nodeLeft;
        }

        nodeLeft->right = node;
        node->parent = nodeLeft;
    }

private:
    Node* root;
};
