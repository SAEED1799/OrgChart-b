#include "OrgChart.hpp"
#include <stdexcept>

using namespace std;
using namespace ariel;


/*
     * a class of orgchart that managment ataple
     of worker that have amany jops
     we will see that in atree
     */

    /*
     * @brief find and return element into the tree
     */
OrgChart::node *OrgChart::getN(node *root, const string &v) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->data == v) {
        return root;
    }

    auto iterator = root->subs.begin();
    while (iterator!= root->subs.end()) {
        node *found = getN(*iterator,v);
        if (found != nullptr) {
            return found;
        }
        iterator++;
    }

    return nullptr;
}

/*
     * @brief delete elements into the tree
     all the tree
     */
    void OrgChart::deleteTree(node *root) {
        if (root == nullptr) {
            return;
        }
        auto iterator = root->subs.begin();
        while (iterator != root->subs.end()) {
            deleteTree(*iterator);
            iterator++;
        }
        delete root;
    }


OrgChart::OrgChart() : root(nullptr) {}

/*
     * @brief copy elements of the tree
     */
OrgChart::node *OrgChart::copyTree(node *other) {
    if (other == nullptr) {
        return nullptr;
    }
    node *newV = new node(other->data);
    auto iterator = other->subs.begin();
    while (iterator != other->subs.end()) {
        node *copyNode = copyTree(*iterator);
        newV->subs.push_back(copyNode);
        ++iterator;
    }
    return newV;
}
/*
     * @operators like + = << >> ..
     */
OrgChart::OrgChart(const OrgChart &other) : root(nullptr) {
    this->operator=(other);
}

OrgChart &OrgChart::operator=(OrgChart const &other) {
     if (&other == this)
        {
            return *this;
        }
        delete root;
        root = other.root;
        if (root != nullptr) {
            root = copyTree(other.root);
        }
        return *this;
}

OrgChart::OrgChart(OrgChart &&other) noexcept: root(nullptr) {
    this->operator=(other);
}

OrgChart &OrgChart::operator=(OrgChart &&other) noexcept {
    if (this != &other) {
        if (root != nullptr) {
            deleteTree(root);
            root = nullptr;
        }
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

OrgChart::~OrgChart() {
    deleteTree(root);
}
/*
     * @ add new element into the tree
     */
OrgChart &OrgChart::add_root(const string &data) {
     if (data.empty())
        {
            throw std::invalid_argument("Invalid operations!");
        }

        if (root != nullptr)
        {
            root->data = data;
        }
        else
        {
            root = new node(data);
        }
        return *this;
}
/*
     * @brief adding new element into the tree
     with tow preson, parent and son
     */
OrgChart &OrgChart::add_sub(const string &p, const string &s) {
    node *parentNode = getN(root, p);
    if (parentNode == nullptr) {
        throw std::logic_error("The parent element is not found.");
    }
    parentNode->subs.push_back(new node(s));
    return *this;
}


void OrgChart::printChartLevels(ostream &ost, const node *ptr, int indent) {
    int level_indent = 4;
    int index = 0;
    if (ptr == nullptr) {
        return;
    }
    for (index = 0; index < indent - level_indent; index++) {
        ost << ' ';
    }
    for (index = 0; index < level_indent; index++) {
        ost << '-';
    }
    ost << ptr->data << "\n";
    auto itr = ptr->subs.begin();
    while (itr != ptr->subs.end()) {
        printChartLevels(ost, *itr, indent + level_indent);
        ++itr;
    }
}

ostream &ariel::operator<<(ostream &ost, const OrgChart &chart) {
    OrgChart::printChartLevels(ost, chart.root, 0);
    return ost;
}

OrgChart::iterator::iterator() :_node(nullptr) {}

OrgChart::node **OrgChart::iterator::getCurrentNode() {
    return &this->_node;
}
/*
seder hm3var 
*/
queue<OrgChart::node *> *OrgChart::iterator::getOrdertrasition() {
    return &this->Ordertrasition;
}

string &OrgChart::iterator::operator*() const {
    return _node->data;
}

string *OrgChart::iterator::operator->() const {
    return &_node->data;
}

bool OrgChart::iterator::operator==(const iterator &a) const {
    return _node == a._node;
}

bool OrgChart::iterator::operator!=(const iterator &a) const {
    return _node != a._node;
}

OrgChart::iterator &OrgChart::iterator::operator++() {
    if (!Ordertrasition.empty()) {
        _node = Ordertrasition.front();
        Ordertrasition.pop();
    } else {
        _node = nullptr;
    }
    return *this;
}

OrgChart::iterator OrgChart::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

OrgChart::level_order_iterator::level_order_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *Ordertrasition = getOrdertrasition();
    if (ptr == nullptr) {
        *current_node = nullptr;
    } else {
        queue<node *> tmp;
        tmp.push(ptr);
        while (!tmp.empty()) {
            node *cur_node = tmp.front();
            tmp.pop();

            Ordertrasition->push(cur_node);
            for (auto itr = cur_node->subs.begin(); itr != cur_node->subs.end(); ++itr) {
                tmp.push(*itr);
            }
        }
        *current_node = Ordertrasition->front();
        Ordertrasition->pop();
    }
}

OrgChart::level_order_iterator OrgChart::begin_level_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty .");
	}
    return level_order_iterator(root);
}

OrgChart::level_order_iterator OrgChart::end_level_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty.");
	}
    return level_order_iterator(nullptr);
}

OrgChart::reverse_order_iterator::reverse_order_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *Ordertrasition = getOrdertrasition();
    if (ptr == nullptr) {
        *current_node = nullptr;
    } else {
        queue<node *> reverse_order_per_level;
        reverse_order_per_level.push(ptr);
        stack<node *> stack;
        while (!reverse_order_per_level.empty()) {
            node *front = reverse_order_per_level.front();
            reverse_order_per_level.pop();
            stack.push(front);
            for (auto itr = front->subs.rbegin(); itr != front->subs.rend(); ++itr) {
                reverse_order_per_level.push(*itr);
            }
        }

        while (!stack.empty()) {
            Ordertrasition->push(stack.top());
            stack.pop();
        }

        *current_node = Ordertrasition->front();
        Ordertrasition->pop();
    }
}

OrgChart::reverse_order_iterator OrgChart::begin_reverse_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty .");
	}
    return reverse_order_iterator(root);
}

OrgChart::reverse_order_iterator OrgChart::reverse_order() {
	if (root == nullptr){
		throw std::invalid_argument("Empty.");
	}
    return reverse_order_iterator(nullptr);
}

void OrgChart::preorder_iterator::preorder_traversal(node *cur) {
    auto *Ordertrasition = getOrdertrasition();
    if (cur == nullptr) {
        return;
    } 
	Ordertrasition->push(cur);
	for (auto itr = cur->subs.begin(); itr != cur->subs.end(); ++itr) {
		preorder_traversal(*itr);
	}
}

OrgChart::preorder_iterator::preorder_iterator(node *ptr) {
    auto *current_node = getCurrentNode();
    auto *Ordertrasition = getOrdertrasition();
    if (ptr == nullptr) {
        current_node = nullptr;
    } else {
        preorder_traversal(ptr);
        *current_node = Ordertrasition->front();
        Ordertrasition->pop();
    }
}

OrgChart::preorder_iterator OrgChart::begin_preorder() {
	if (root == nullptr){
		throw std::invalid_argument("Empty .");
	}
    return preorder_iterator(root);
}

OrgChart::preorder_iterator OrgChart::end_preorder() {
	if (root == nullptr){
		throw std::invalid_argument("Empty .");
	}
    return preorder_iterator(nullptr);
}

OrgChart::level_order_iterator OrgChart::begin() {
	if (root == nullptr){
		throw std::invalid_argument("Empty .");
	}
    return level_order_iterator(root);
}

OrgChart::level_order_iterator OrgChart::end() {
	if (root == nullptr){
		throw std::invalid_argument("Empty.");
	}
    return level_order_iterator(nullptr);
}


