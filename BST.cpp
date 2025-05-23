#include "BST.h"
#include <algorithm>
#include <stack>
#include <iostream>
#include <ostream>

BinarySearchTree::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right)
    : keyValuePair(std::make_pair(key, value)), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node& other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return keyValuePair == other.keyValuePair &&
        ((left == nullptr && other.left == nullptr) ||
         (left != nullptr && other.left != nullptr && *left == *other.left)) &&
        ((right == nullptr && other.right == nullptr) ||
         (right != nullptr && other.right != nullptr && *right == *other.right));
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) {
        left->output_node_tree();
    }
    std::cout << "[" << keyValuePair.first << "] = " << keyValuePair.second << std::endl;
    if (right) {
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value) {
    if (key < keyValuePair.first) {
        if (left) {
            left->insert(key, value);
        } else {
            left = new Node(key, value, this);
        }
    } else {
        if (right) {
            right->insert(key, value);
        } else {
            right = new Node(key, value, this);
        }
    }
}


BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : _size(0), _root(nullptr) {
    if (other._root) {
        _root = new Node(*other._root);
        _size = other._size;
    }
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this != &other) {
        BinarySearchTree temp(other);
        std::swap(_root, temp._root);
        std::swap(_size, temp._size);
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept 
    : _root(other._root), _size(other._size) {
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this != &other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    std::stack<Node*> nodes;
    if (_root) {
        nodes.push(_root);
    }

    while (!nodes.empty()) {
        Node* current = nodes.top();
        nodes.pop();

        if (current->left) {
            nodes.push(current->left);
            current->left = nullptr;
        }
        if (current->right) {
            nodes.push(current->right);
            current->right = nullptr;
        }

        delete current;
    }
}

//---------------------------Iterator---------------------------
BinarySearchTree::Iterator::Iterator(Node* node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (!_node) throw std::runtime_error("Access to null iterator");
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (!_node) throw std::runtime_error("Access to null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) return *this;
    
    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        Node* parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node) return *this;
    
    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
        }
    } else {
        Node* parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
    return _node != other._node;
}

//---------------------------ConstIterator---------------------------
BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) throw std::runtime_error("Dereferencing null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) throw std::runtime_error("Access to null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node) return *this;
    
    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        const Node* parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) return *this;
    
    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
        }
    } else {
        const Node* parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
    return _node != other._node;
}

void BinarySearchTree::insert(const Key& key, const Value& value) {
    if (!_root) {
        _root = new Node(key, value);
        _size = 1;
    } else {
        Node* current = _root;
        Node* parent = nullptr;
        
        while (current) {
            parent = current;
            if (key < current->keyValuePair.first) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        if (key < parent->keyValuePair.first) {
            parent->left = new Node(key, value, parent);
        } else {
            parent->right = new Node(key, value, parent);
        }
        _size++;
    }
}

void BinarySearchTree::erase(const Key& key) {
    if (!_root) return;

    Node* nodeToDelete = _root;
    Node* parent = nullptr;
    bool isLeftChild = false;

    while (nodeToDelete && nodeToDelete->keyValuePair.first != key) {
        parent = nodeToDelete;
        if (key < nodeToDelete->keyValuePair.first) {
            nodeToDelete = nodeToDelete->left;
            isLeftChild = true;
        } else {
            nodeToDelete = nodeToDelete->right;
            isLeftChild = false;
        }
    }

    if (!nodeToDelete) return;

    //Node has two children
    if (nodeToDelete->left && nodeToDelete->right) {
        Node* successor = nodeToDelete->right;
        Node* successorParent = nodeToDelete;

        while (successor->left) {
            successorParent = successor;
            successor = successor->left;
        }

        nodeToDelete->keyValuePair = successor->keyValuePair;

        if (successorParent == nodeToDelete) {
            successorParent->right = successor->right;
        } else {
            successorParent->left = successor->right;
        }

        if (successor->right) {
            successor->right->parent = successorParent;
        }

        delete successor;
    }
    
	//Node has one child or no children
    else {
        Node* child = nodeToDelete->left ? nodeToDelete->left : nodeToDelete->right;

        if (!parent) {
            _root = child;
            if (_root) {
                _root->parent = nullptr;
            }
        } else {
            if (isLeftChild) {
                parent->left = child;
            } else {
                parent->right = child;
            }

            if (child) {
                child->parent = parent;
            }
        }

        delete nodeToDelete;
    }

    _size--;
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
    Node* current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return ConstIterator(current);
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
    Node* current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return Iterator(current);
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key& key) {
    Iterator lower = begin();
    Iterator upper = begin();

    while (lower != end() && lower->first < key) {
        ++lower;
    }

    upper = lower;
    while (upper != end() && upper->first == key) {
        ++upper;
    }

    return std::make_pair(lower, upper);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key& key) const {
    ConstIterator lower = cbegin();
    ConstIterator upper = cbegin();

    while (lower != cend() && lower->first < key) {
        ++lower;
    }

    upper = lower;
    while (upper != cend() && upper->first == key) {
        ++upper;
    }

    return std::make_pair(lower, upper);
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (!_root) return cend();

    const Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) return cend();

    const Node* current = _root;
    while (current->right) {
        current = current->right;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
    ConstIterator it = cbegin();
    while (it != cend() && it->first < key) {
        ++it;
    }
    return (it != cend() && it->first == key) ? it : cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
    ConstIterator it = cbegin();
    ConstIterator result = cend();

    while (it != cend()) {
        if (it->first == key) {
            result = it;
        } else if (it->first > key) {
            break;
        }
        ++it;
    }

    return result;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    if (!_root) return end();

    Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    if (!_root) return cend();

    const Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    if (_root) {
        _root->output_node_tree();
    }
}

size_t BinarySearchTree::max_height() const {
    if (!_root) return 0;

    std::stack<std::pair<const Node*, size_t>> stack;
    stack.push({_root, 1});
    size_t max_height = 0;

    while (!stack.empty()) {
        auto current = stack.top();
        stack.pop();

        if (current.second > max_height) {
            max_height = current.second;
        }

        if (current.first->left) {
            stack.push({current.first->left, current.second + 1});
        }
        if (current.first->right) {
            stack.push({current.first->right, current.second + 1});
        }
    }

    return max_height;
}

