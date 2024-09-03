#pragma once

#include <functional>
#include "ANSI_colors.hpp"

namespace avl_tree {

template <typename KeyT, typename CompT = std::less<KeyT>> class avl_t final {
    struct avl_node final {
        KeyT key_;
        int height_;
        int Nleft_;
        int Nright_;
        avl_node* parent_;
        avl_node* left_;
        avl_node* right_;

        avl_node(KeyT key) : key_(key), height_(0), Nleft_(0), Nright_(0),
                             parent_(nullptr), left_(nullptr), right_(nullptr) {}
    };

    avl_node* root_;

private:
    int get_node_size(avl_node* node) {
        if (!node)
            return 0;

        return node->Nleft_ + node->Nright_ + 1;
    }

    void update_Nchilds(avl_node* node) {
        for (avl_node* node_iter = node; node_iter != nullptr; node_iter = node_iter->parent_) {
            node_iter->Nleft_  = get_node_size(node_iter->left_);
            node_iter->Nright_ = get_node_size(node_iter->right_);
        }
    }

    void update_height(avl_node* node) {
        for (avl_node* node_iter = node; node_iter != nullptr; node_iter = node_iter->parent_) {
            node_iter->height_ = 0;

            if (node_iter->left_)
                node_iter->height_ = node_iter->left_->height_;
            if (node_iter->right_)
                node_iter->height_ = std::max(node_iter->height_, node_iter->right_->height_);

            node_iter->height_++;
        }
    }

    void balance(avl_node* node) {
        for (avl_node* node_iter = node; node_iter != nullptr; node_iter = node_iter->parent_) {
            int balance_diff = 0;
            if (node_iter->left_)
                balance_diff = node_iter->left_->height_;
            if (node_iter->right_)
                balance_diff -= node_iter->right_->height_;

            if (balance_diff > 1)
                rotate_right(node_iter);
            else if (balance_diff < -1)
                rotate_left(node_iter);
        }
    }

    void rotate_right(avl_node* node) {
        avl_node* node_left_old = node->left_;

        node->left_ = node->left_->right_;
        node_left_old->right_ = node;

        if (node == root_) {
            root_ = node_left_old;
        } else {
            if (node->key_ < node->parent_->key_)
                node->parent_->left_  = node_left_old;
            else
                node->parent_->right_ = node_left_old;
        }

        node_left_old->parent_ = node->parent_;
        if (node->left_)
            node->left_->parent_ = node;
        node->parent_ = node_left_old;

        update_height(node);
        update_Nchilds(node);
    }

    void rotate_left(avl_node* node) {
        avl_node* node_right_old = node->right_;

        node->right_ = node->right_->left_;
        node_right_old->left_ = node;

        if (node == root_) {
            root_ = node_right_old;
        } else {
            if (node->key_ < node->parent_->key_)
                node->parent_->left_  = node_right_old;
            else
                node->parent_->right_ = node_right_old;
        }

        node_right_old->parent_ = node->parent_;
        if (node->right_)
            node->right_->parent_ = node;
        node->parent_ = node_right_old;
        
        update_height(node);
        update_Nchilds(node);
    }

    void print_recursive(const avl_node* const node) {
        if (!node) return;

        print_recursive(node->left_);

        std::cerr << print_lcyan(node->key_) << print_lcyan("(");

        if (node->left_)
            std::cerr << print_lcyan(node->left_->key_) << print_lcyan(", "); 
        else
            std::cerr << print_lcyan("none") << print_lcyan(", "); 

        if (node->right_)
            std::cerr << print_lcyan(node->right_->key_) << print_lcyan(")") << '\n'; 
        else
            std::cerr << print_lcyan("none") << print_lcyan(")") << '\n'; 

        print_recursive(node->right_);
    }

    void delete_avl_tree(avl_node* node)
    {
        if(!node) return;
        delete_avl_tree(node->left_);
        delete_avl_tree(node->right_);
        delete node;
    }

public:
    avl_t() : root_(nullptr) {}

    avl_node* insert(const KeyT& key) {
        avl_node* new_node = new avl_node{key};

        if (!root_) {
            root_ = new_node;
            return root_;
        }

        avl_node* current = root_;
        avl_node* destination;
        while (true) {
            if (key < current->key_) {
                if (current->left_) {
                    current = current->left_;
                } else {
                    current->left_ = new_node;
                    current->left_->parent_ = current;
                    destination = current->left_;
                    break;
                }
            } else {
                if (current->right_) {
                    current = current->right_;
                } else {
                    current->right_ = new_node;
                    current->right_->parent_ = current;
                    destination = current->right_;
                    break;
                }
            }
        }

        update_height(destination);
        update_Nchilds(destination);

        balance(current);
        return current;
    }

    int check_range(int left_border, int right_border) {
        return -1;
    }

    void print() {
        std::cout << print_lblue("\nAVL Tree (childs in scopes) with root ") <<
                     print_lblue(root_->key_) << print_lblue(":\n");

        print_recursive(root_);
        std::cout << '\n';
    }

    ~avl_t()
    {
        delete_avl_tree(root_);
    }
};

};