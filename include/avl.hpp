#pragma once

#include <functional>
#include "ANSI_colors.hpp"

namespace avl_tree {

template <typename KeyT, typename CompT = std::less<KeyT>> class avl_t final {

    enum class node_status_e {
        NODE_STATUS_NOT_USED,
        NODE_STATUS_USED,
    };
    struct avl_node final {
        KeyT key_;
        node_status_e node_status_ = node_status_e::NODE_STATUS_NOT_USED;
        int height_ = 0;
        int Nleft_  = 0;
        int Nright_ = 0;
        avl_node* parent_ = nullptr;
        avl_node* left_   = nullptr;
        avl_node* right_  = nullptr;

        avl_node(KeyT key) :       key_(key) {}
        avl_node(avl_node* node) : key_(node->key_),     height_(node->height_),
                                   Nleft_(node->Nleft_), Nright_(node->Nright_) {}
    };

    enum class avl_node_it_type_e {
        ASCENDING
    };
    class avl_node_it final {
        avl_node* node_          = nullptr;
        avl_node_it_type_e type_ = avl_node_it_type_e::ASCENDING;

    public:
        avl_node_it(avl_node* node, avl_node_it_type_e type) : node_(node), type_(type) {}

        avl_node* operator*() {
            return node_;
        }

        bool operator==(const avl_node_it& rhs) const {
            return (rhs.node_ == node_);
        }

        bool operator!=(const avl_node_it& rhs) const {
            return (rhs.node_ != node_);
        }

        avl_node_it& operator++() {
            if (node_) {
                switch (type_) {
                    case avl_node_it_type_e::ASCENDING:
                        node_ = node_->parent_;
                        break;
                }
            }
            return *this;
        }

        avl_node_it begin() const {
            avl_node* current = node_;
            while (current->left_)
                current = current->left_;
            return avl_node_it{current, type_};
        }

        avl_node_it end() const {
            return avl_node_it{nullptr, type_};
        }
    };

    CompT comp_func;
    avl_node* root_;
    KeyT max_key_;

private:
    avl_node_it ascending_range(avl_node* node) const {
        return avl_node_it(node, avl_node_it_type_e::ASCENDING);
    }

    int get_node_size(avl_node* node) const noexcept {
        if (!node)
            return 0;

        return node->Nleft_ + node->Nright_ + 1;
    }

    void update_Nchilds(avl_node* node) noexcept {
        for (auto node_iter : ascending_range(node)) {
            node_iter->Nleft_  = get_node_size(node_iter->left_);
            node_iter->Nright_ = get_node_size(node_iter->right_);
        }
    }

    void update_height(avl_node* node) noexcept {
        for (auto node_iter : ascending_range(node)) {
            node_iter->height_ = 0;

            if (node_iter->left_)
                node_iter->height_ = node_iter->left_->height_;
            if (node_iter->right_)
                node_iter->height_ = std::max(node_iter->height_, node_iter->right_->height_);

            node_iter->height_++;
        }
    }

    void balance(avl_node* node) {
        int balance_diff = 0;
        if (node->left_)
            balance_diff = node->left_->height_;
        if (node->right_)
            balance_diff -= node->right_->height_;

        int left_height = 0;
        int right_height = 0;
        if (balance_diff > 1) {
            avl_node* left = node->left_;
            if (left->left_)
                left_height = left->left_->height_;

            if (left->right_)
                right_height = left->right_->height_;

            if (left_height < right_height)
                rotate_left(node->left_);
            rotate_right(node);
                
        } else if (balance_diff < -1) {
            avl_node* right = node->right_;
            if (right->left_)
                left_height = right->left_->height_;

            if (right->right_)
                right_height = right->right_->height_;

            if (left_height > right_height)
                rotate_right(node->right_);
            rotate_left(node);
        }
    }

    void rotate_right(avl_node* node) {
        if (!node)
            return;

        avl_node* node_left_old = node->left_;

        node->left_ = node->left_->right_;
        node_left_old->right_ = node;

        if (node == root_) {
            root_ = node_left_old;
        } else {
            if (comp_func(node->key_, node->parent_->key_))
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
        if (!node)
            return;

        avl_node* node_right_old = node->right_;

        node->right_ = node->right_->left_;
        node_right_old->left_ = node;

        if (node == root_) {
            root_ = node_right_old;
        } else {
            if (comp_func(node->key_, node->parent_->key_))
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

    avl_node* get_parent_bigger(avl_node* node, KeyT key) const {
        avl_node* ans_node = node;
        KeyT      ans_key  = max_key_;
        for (auto node_iter : ascending_range(node)) {
            if (!comp_func(node_iter->key_, key) && node_iter->key_ != key &&
                comp_func(node_iter->key_, ans_key)) {

                ans_node = node_iter;
                ans_key  = node_iter->key_;
            }
        }
        return ans_node;
    }

    avl_node* lower_bound(KeyT key) const {
        if (!root_)
            return nullptr;

        avl_node* current = root_;
        while (current) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_bigger(current, key);
            } else if (comp_func(current->key_, key)) {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_bigger(current, key);
            } else {
                return current;
            }
        }
        return current;
    }

    avl_node* upper_bound(KeyT key) const {
        if (!root_)
            return nullptr;

        avl_node* current = root_;
        while (current) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_bigger(current, key);
            } else {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_bigger(current, key);
            }
        }
        return current;
    }

    int size_left_side(KeyT key) const {
        if (!root_)
            return 0;

        int dist = 0;
        avl_node* current = root_;
        while (current) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    break;
            } else if (comp_func(current->key_, key)) {
                dist += current->Nleft_ + 1;
                if (current->right_)
                    current = current->right_;
                else
                    break;
            } else {
                return dist + current->Nleft_;
            }
        }
        return dist;
    }

    void print_node(const avl_node* node) const noexcept {
        if (!node) return;

        std::cerr << print_lcyan(node->key_ << "\t(");

        if (node->left_)
            std::cerr << print_lcyan(node->left_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        if (node->right_)
            std::cerr << print_lcyan(node->right_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        if (node->parent_)
            std::cerr << print_lcyan(node->parent_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        std::cerr << print_lcyan(node->Nleft_  << ",\t" << node->Nright_ << ",\t" <<
                                 node->height_ << ",\t" << node << ")\n");
    }

    void print_subtree(avl_node* node) noexcept {
        if (!node) return;

        avl_node* current = node;
        while (current) {
            if (current->left_ && current->left_->node_status_ == node_status_e::NODE_STATUS_NOT_USED) {
                print_subtree(current->left_);
                continue;
            }

            if (current->node_status_ == node_status_e::NODE_STATUS_NOT_USED)
                print_node(current);
            current->node_status_ = node_status_e::NODE_STATUS_USED;

            if (current->right_ && current->right_->node_status_ == node_status_e::NODE_STATUS_NOT_USED) {
                print_subtree(current->right_);
                continue;
            }

            current = current->parent_;
        }
    }

    void decrement_parents_size(avl_node* node) noexcept {
        for (avl_node* node_iter = node; node_iter->parent_ != nullptr; node_iter = node_iter->parent_) {
            if (node_iter->parent_->left_ == node_iter)
                node_iter->parent_->Nleft_--;
            else
                node_iter->parent_->Nright_--;
        }
    }

public:
    avl_t(KeyT max_key) : root_(nullptr), max_key_(max_key) {}

    avl_t(const avl_t<KeyT, CompT>& other) {
        avl_node* curr_other = other.root_;
        if (!curr_other) {
            root_ = nullptr;
            return;
        }

        avl_node* curr_this = new avl_node{curr_other};

        while (curr_other) {

            if (curr_other->left_ && !curr_this->left_) {
                curr_other       = curr_other->left_;
                curr_this->left_ = new avl_node{curr_other};
                curr_this->left_->parent_ = curr_this;
                curr_this        = curr_this->left_;

            } else if (curr_other->right_ && !curr_this->right_) {
                curr_other        = curr_other->right_;
                curr_this->right_ = new avl_node{curr_other};
                curr_this->right_->parent_ = curr_this;
                curr_this         = curr_this->right_;

            } else {
                if (curr_other->parent_) {
                    curr_other = curr_other->parent_;
                    curr_this  = curr_this->parent_;
                } else {
                    break;
                }
            }
        }

        root_ = curr_this;
    }

    avl_t<KeyT, CompT>& operator=(const avl_t<KeyT, CompT>& other) {
        if (this == &other)
            return *this;

        avl_t<KeyT, CompT> new_tree{other};
        std::swap(root_, new_tree.root_);
        return *this;
    }

    avl_node_it insert(const KeyT& key) {
        avl_node* new_node = new avl_node{key};

        if (!root_) {
            root_ = new_node;
            return;
        }

        avl_node* current = root_;
        avl_node* destination;
        while (current) {
            if (comp_func(key, current->key_)) {
                if (current->left_) {
                    current = current->left_;
                } else {
                    current->left_ = new_node;
                    current->left_->parent_ = current;
                    destination = current->left_;
                    break;
                }
            } else if (comp_func(current->key_, key)) {
                if (current->right_) {
                    current = current->right_;
                } else {
                    current->right_ = new_node;
                    current->right_->parent_ = current;
                    destination = current->right_;
                    break;
                }
            } else {
                delete new_node;
                return;
            }
        }

        update_height(destination);
        update_Nchilds(destination);

        for (auto node_iter : ascending_range(current))
            balance(node_iter);
    }

    int check_range(KeyT first_key, KeyT second_key) const {
        if (first_key >= second_key || !root_)
            return 0;

        avl_node* left  = lower_bound(first_key);
        avl_node* right = upper_bound(second_key);

        int border = -1;
        if (first_key <= left->key_)
            border++;
        if (second_key >= right->key_)
            border++;

        return size_left_side(right->key_) - size_left_side(left->key_) + border;
    }

    void print() const noexcept {
        if (!root_)
            return;

        avl_t<KeyT, CompT> print_tree{*this};

        std::cerr << print_lblue("\nAVL Tree with root = " << print_tree.root_->key_ <<
                                 ":\nkey(<child>, <child>, <parent>, <Nleft>, <Nright>, <height>, <ptr>):\n");

        print_tree.print_subtree(print_tree.root_);
        std::cerr << '\n';
    }

    ~avl_t() {
        if (!root_)
            return;

        avl_node* current = root_;
        while (current) {

            if (current->Nleft_ > 0) {
                int node_left_size = get_node_size(current->left_);

                if (node_left_size == 1) {
                    decrement_parents_size(current->left_);
                    delete current->left_;
                } else if (node_left_size > 1) {
                    current = current->left_;
                }

            } else if (current->Nright_ > 0) {
                int node_right_size = get_node_size(current->right_);

                if (node_right_size == 1) {
                    decrement_parents_size(current->right_);
                    delete current->right_;
                } else if (node_right_size > 1) {
                    current = current->right_;
                }

            } else {
                current = current->parent_;
            }
        }

        delete root_;
    }
};

};