#include <iostream>
#include <memory>

class RedBlackTree {
private:
    enum class NodeType {BLACK, RED};

public: 
    struct Node {
        int value;
        NodeType type;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right; 
        std::shared_ptr<Node> parent;
        Node(
            int value, 
            NodeType type = NodeType::RED,
            std::shared_ptr<Node> left = nullptr, 
            std::shared_ptr<Node> right = nullptr, 
            std::shared_ptr<Node> parent = nullptr) 
            : value(value), type(type), left(left), right(right), parent(parent)
            {}
    };
    
    bool isRightChild(const std::shared_ptr<Node>& node) {
        if (node->parent == nullptr) {
            throw std::runtime_error("node doesn't have parent");
        }
        return (node->parent->right == node);
    }

    bool isLeftChild(const std::shared_ptr<Node>& node) {
        return (!isRightChild(node));
    }

    std::shared_ptr<Node> getRightUncle(const std::shared_ptr<Node>& node) {
        if (node->parent == nullptr || node->parent->parent == nullptr) {
            throw std::runtime_error("node doesn't have uncle");
        }

        if (isRightChild(node->parent)) {
            throw std::runtime_error("node's parent is on right side");
        }

        return node->parent->parent->right;
    }

    std::shared_ptr<Node> getLeftUncle(const std::shared_ptr<Node>& node) {
        if (node->parent == nullptr || node->parent->parent == nullptr) {
            throw std::runtime_error("Node doesn't have uncle");
        }

        if (isLeftChild(node->parent)) {
            throw std::runtime_error("node's parent is on left side");
        }

        return node->parent->parent->left;
    }

    void leftRotate(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> nodeRight = node->right;

        node->right = nodeRight->left;

        if (nodeRight->left != nullptr) {
            nodeRight->left->parent = node;
        }

        nodeRight->parent = node->parent;

        if (node->parent == nullptr) {
            root = nodeRight; 
        }

        if (node->parent != nullptr) {
            if (isLeftChild(node)) {
                node->parent->left= nodeRight;
            }

            if (isRightChild(node)) {
                node->parent->right = nodeRight;
            }
        }

        nodeRight->left = node;
        node->parent = nodeRight;
    }

    void rightRotate(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> nodeLeft = node->left; 

        node->left = nodeLeft->right;

        if (nodeLeft->right != nullptr) {
            nodeLeft->right->parent = node;  
        }

        nodeLeft->parent = node->parent;

        if (node->parent == nullptr) {
            root = nodeLeft;
        }

        if (node->parent != nullptr) {
            if (isLeftChild(node)) {
                node->parent->left = nodeLeft;
            }

            if (isRightChild(node)) {
                node->parent->right = nodeLeft;
            }
        }

        nodeLeft->right = node;
        node->parent = nodeLeft;
    }

    std::shared_ptr<Node> getInsertNodeParent(int value) {
        if (root == nullptr) {
            return nullptr;
        }

        std::shared_ptr<Node> current = root;
        std::shared_ptr<Node> prev = nullptr;

        while (current != nullptr) {
            prev = current;
            if (value > current->value) {
                current = current->right;
            }

            else if (value <= current->value) {
                current = current->left;
            }
        }

        return prev;
    }

    void fixUpInsert(std::shared_ptr<Node>& node) {
        while (node->parent != nullptr && node->parent->type == NodeType::RED) {
            bool isLeftBranch= isLeftChild(node->parent);
            std::shared_ptr<Node> uncle;

            if (isLeftBranch) {
                uncle = getRightUncle(node);
            }

            else {
                uncle = getLeftUncle(node);
            }

            if (uncle != nullptr && uncle->type == NodeType::RED) {
                node->parent->type = NodeType::BLACK;
                uncle->type = NodeType::BLACK;
                node->parent->parent->type = NodeType::RED;
                node = node->parent->parent;
            }
            else if (uncle == nullptr || uncle->type == NodeType::BLACK) {
                if (isLeftBranch && isRightChild(node)) {
                    node = node->parent;
                    leftRotate(node);
                }

                if (!isLeftBranch && isLeftChild(node)) {
                    node = node->parent;
                    rightRotate(node);
                }
                node->parent->type = NodeType::BLACK;
                node->parent->parent->type = NodeType::RED;

                if (isLeftBranch) {
                    rightRotate(node->parent->parent);

                } else {
                    leftRotate(node->parent->parent);
                }
            }

        }

        root->type = NodeType::BLACK;
    }

    void insert(int value) {
        std::shared_ptr<Node> parent = getInsertNodeParent(value);
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

        if (parent == nullptr) {
            newNode->type = NodeType::BLACK;
            root = newNode;
            return;
        }

        newNode->parent = parent;

        if (newNode->value <= parent->value) {
            parent->left = newNode;
        }

        if (newNode->value > parent->value) {
            parent->right = newNode;
        }

        fixUpInsert(newNode);
    }

    void transplant(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> toNode) {
        if (fromNode == nullptr) {
            throw std::runtime_error("Node is nullptr"); 
        }

        if (fromNode->parent == nullptr) {
            root = toNode;

        } else if (isRightChild(fromNode)) {
            fromNode->parent->right = toNode; 

        } else {
            fromNode->parent->left = toNode;
        }

        if (toNode != nullptr) {
            toNode->parent = fromNode->parent;
        }
    }

    void deleteFixUp(std::shared_ptr<Node>& node) {
        while (node != root && node->type == NodeType::BLACK) {
            if (isLeftChild(node)) {
                std::shared_ptr<Node> sibling = node->parent->right;
                if (sibling->type == NodeType::RED) {
                    sibling->type = NodeType::BLACK;
                    node->parent->type = NodeType::RED;
                    leftRotate(node->parent);

                    sibling = node->parent->right;
                } 

                if (sibling->left->type == NodeType::BLACK && sibling->right->type == NodeType::BLACK) {
                    sibling->type = NodeType::RED;
                    node = node->parent;

                } else {
                    if (sibling->right->type == NodeType::BLACK) {
                        sibling->type = NodeType::RED;
                        sibling->left->type = NodeType::BLACK;
                        rightRotate(sibling);
                        sibling = node->parent->right; 
                    }
                    sibling->type = node->parent->type;
                    node->parent->type = NodeType::BLACK;
                    sibling->right->type = NodeType::RED;
                    leftRotate(node->parent);
                    node = root;
                }
            }
            if (isRightChild(node)) {
                std::shared_ptr<Node> sibling = node->parent->left;
                if (sibling->type == NodeType::RED) {
                    sibling->type = NodeType::BLACK;
                    node->parent->type = NodeType::RED;
                    rightRotate(node->parent);

                    sibling = node->parent->left;
                } 

                if (sibling->right->type == NodeType::BLACK && sibling->left->type == NodeType::BLACK) {
                    sibling->type = NodeType::RED;
                    node = node->parent;

                } else {
                    if (sibling->left->type == NodeType::BLACK) {
                        sibling->type = NodeType::RED;
                        sibling->left->type = NodeType::BLACK;
                        leftRotate(sibling);
                        sibling = node->parent->left; 
                    }
                    sibling->type = node->parent->type;
                    node->parent->type = NodeType::BLACK;
                    sibling->left->type = NodeType::RED;
                    rightRotate(node->parent);
                    node = root;
                }
            }
        }
        node->type = NodeType::BLACK;
    }

    void removeNode(std::shared_ptr<Node>& node) {
        if (node == nullptr) {
            throw std::runtime_error("Node is nullptr");
        }

        std::shared_ptr<Node> nodeSub;
        NodeType orignalColor = node->type;

        if (node->right == nullptr) {
            nodeSub = node->left;
            transplant(node, nodeSub);

        } else if (node->left == nullptr) {
            nodeSub = node->right;
            transplant(node, nodeSub);

        } else {
            std::shared_ptr<Node> smallestNode = findMinimum(node->right);
            orignalColor = smallestNode->type;
            nodeSub = smallestNode->right;
            if (smallestNode->parent == node) {
                if (nodeSub != nullptr) {
                    nodeSub->parent = smallestNode;
                }

            } else {
                transplant(smallestNode, nodeSub);
                smallestNode->right = node->right;
                smallestNode->right->parent = smallestNode;
            }

            transplant(node, smallestNode);
            smallestNode->left = node->left;
            smallestNode->left->parent = smallestNode; 
            smallestNode->type = node->type;
        }

        if (orignalColor == NodeType::BLACK && nodeSub != nullptr) {
            deleteFixUp(nodeSub);
        }
    }

    void deleteFirst(int value) {
        std::shared_ptr<Node> toDelete = findFirst(value);
        if (toDelete == nullptr) {
            throw std::runtime_error("Node does not exsist");
        }
        removeNode(toDelete);
    }

    std::shared_ptr<Node> findMinimum(std::shared_ptr<Node> node) {
        if (node == nullptr) {
            throw std::runtime_error("Node is nullptr");
        }

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    std::shared_ptr<Node> findFirst(int value) {
        std::shared_ptr<Node> current = root;
        
        while (current != nullptr) {
            if (current->value == value) {
                return current;
            }

            if (value > current->value){
                current = current->right;

            } else if (value <= current->value) {
                current = current->left;
            }

        }

        return nullptr;
    }

    void printInOrder(const std::shared_ptr<Node>& current) {
        if (current == nullptr) {
            return;
        }

        printInOrder(current->left);
        std::cout << current->value << "-" << (current->type == NodeType::RED) << ", ";
        printInOrder(current->right);
    }

    void print() {
        std::cout << "Printing the tree: " << std::endl;
        std::shared_ptr<Node> current = root;
        printInOrder(current);
        std::cout << std::endl;
    }

private:
    std::shared_ptr<Node> root;
};

int main() {
    RedBlackTree test;
    test.insert(41);
    test.insert(38);
    test.insert(31);
    test.insert(12);
    test.insert(19);
    test.insert(8);
    test.print();
    test.deleteFirst(31);
    test.print();
}
