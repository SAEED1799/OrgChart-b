#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

using namespace std;

namespace ariel {
    class OrgChart {
          void checkThrows()
        {
            if (root == nullptr)
            {
                throw std::invalid_argument("Invalid operations!");
            }
        }
    private:

        struct node {
            string data;
            vector<node *> subs;

            node(const string &data) : data(data) {}
        };
        
        node *root;
        /*
     * @help functions
     */
        void deleteTree(node *root);

        node *copyTree(node *other);

        node *getN(node *root, const string &v);

        static void printChartLevels(ostream &ost, const node *ptr, int indent);

    public:
        OrgChart();

        OrgChart(const OrgChart &other);

        OrgChart &operator=(OrgChart const &other);

        OrgChart(OrgChart &&other) noexcept;

        OrgChart &operator=(OrgChart &&other) noexcept;

        ~OrgChart();

        OrgChart &add_root(const string &data);

        OrgChart &add_sub(const string &parent, const string &son);


        friend ostream &operator<<(ostream &ost, const OrgChart &chart);

        class iterator {
        private:
            node *_node;
            queue<node *> Ordertrasition;
        public:
            iterator();

            virtual ~iterator() = default;

            iterator(iterator const &) = default;

            iterator &operator=(iterator const &) = default;

            iterator(iterator &&) = default;

            iterator &operator=(iterator &&) = default;

            node **getCurrentNode();

            queue<node *> *getOrdertrasition();

            string &operator*() const;

            string *operator->() const;

            bool operator==(const iterator &a) const;

            bool operator!=(const iterator &a) const;

            iterator &operator++();

            iterator operator++(int);
        };
         /*
         class of type level order of the tree
         */
        class level_order_iterator : public iterator {
        public:
            level_order_iterator(node *ptr);
        };

        level_order_iterator begin_level_order();

        level_order_iterator end_level_order();
        /*
         class of type reverse order of the tree
         */
        class reverse_order_iterator : public iterator {
        public:
            reverse_order_iterator(node *ptr);
        };

        reverse_order_iterator begin_reverse_order();

        reverse_order_iterator reverse_order();
         /*
         class of type preorder of the tree
         */
        class preorder_iterator : public iterator {
        private:
            void preorder_traversal(node *cur);

        public:
            preorder_iterator(node *ptr);
        };

        preorder_iterator begin_preorder();

        preorder_iterator end_preorder();

        level_order_iterator begin();

        level_order_iterator end();
    };

    ostream &operator<<(ostream &ost, const OrgChart &chart);
    
}
