#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <cmath>
#include <iterator>
#include <exception>
#include "ANSI_colors.hpp"

namespace avl_tree {

template <typename KeyT, typename CompT = std::less<KeyT>>
class avl_tree_t final {

    struct avl_node;
    using  unique_avl_node = std::unique_ptr<avl_node>;

    struct avl_node final {
        KeyT key_;
        int height_ = 0;
        int Nleft_  = 0;
        int Nright_ = 0;
        avl_node*       parent_ = nullptr;
        unique_avl_node left_;
        unique_avl_node right_;

        avl_node(const KeyT& key) : key_(key) {}
    };

    class avl_node_it final {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = avl_node;
        using pointer           = value_type*;
        using reference         = value_type&;
        using difference_type   = std::ptrdiff_t;

        pointer node_ = nullptr;

    public:
        avl_node_it() {}
        avl_node_it(const avl_node_it& node_it)  : node_(std::addressof(*node_it)) {}
        avl_node_it(reference node)              : node_(&node)      {}
        avl_node_it(pointer   node)              : node_(node)       {}
        avl_node_it(const unique_avl_node& node) : node_(node.get()) {}

        avl_node_it& operator=(const unique_avl_node& node) {
            if (node_ == node.get())
                return *this;

            *this = avl_node_it{node};
            return *this;
        }

        bool is_valid() const noexcept { return (node_ != nullptr); }

        reference operator*() {
            if (node_) return *node_;
            throw std::invalid_argument("*nullptr");
        }

        const reference operator*() const {
            if (node_) return *node_;
            throw std::invalid_argument("*nullptr");
        }

        pointer operator->()             noexcept { return node_; }
        const pointer operator->() const noexcept { return node_; }

        bool operator==(const avl_node_it& rhs) const noexcept {
            return (rhs.node_ == node_);
        }

        bool operator!=(const avl_node_it& rhs) const noexcept {
            return !(rhs.node_ == node_);
        }

        avl_node_it& operator++() noexcept {
            if (is_valid())
                node_ = node_->parent_;
            return *this;
        }
    };

    class const_avl_node_it final {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = KeyT;
        using pointer           = value_type*;
        using reference         = value_type&;
        using difference_type   = std::ptrdiff_t;

        avl_node* node_;

    public:
        const_avl_node_it(const avl_node_it& node_it)  : node_(std::addressof(*node_it)) {}
        const_avl_node_it(const unique_avl_node& node) : node_(node.get()) {}

        reference operator*() {
            if (node_) return node_->key_;
            throw std::invalid_argument("nullptr->");
        }

        const reference operator*() const {
            if (node_) return node_->key_;
            throw std::invalid_argument("nullptr->");
        }

        pointer operator->() {
            if (node_) return std::addressof(node_->key_);
            throw std::invalid_argument("nullptr->");
        }

        const pointer operator->() const {
            if (node_) return std::addressof(node_->key_);
            throw std::invalid_argument("nullptr->");
        }

        bool operator==(const avl_node_it& rhs) const noexcept {
            return (rhs.node_ == node_);
        }

        bool operator!=(const avl_node_it& rhs) const noexcept {
            return !(rhs.node_ == node_);
        }

        const_avl_node_it& operator++() noexcept {
            if (node_)
                node_ = node_->parent_;
            return *this;
        }
    };

    class ascending_range final {
        avl_node_it node_;

    public:
        ascending_range(const avl_node_it node) : node_(node) {}

        avl_node_it begin() const {
            return node_;
        }

        avl_node_it end() const {
            return avl_node_it{nullptr};
        }
    };

    CompT comp_func;
    unique_avl_node root_;
    KeyT max_key_;

private:
    int get_node_size(const avl_node_it node) const noexcept {
        if (!node.is_valid())
            return 0;

        return node->Nleft_ + node->Nright_ + 1;
    }

    void update_Nchilds(const avl_node_it node) {
        for (auto& node_ : ascending_range{node}) {
            node_.Nleft_  = get_node_size(node_.left_);
            node_.Nright_ = get_node_size(node_.right_);
        }
    }

    void update_height(const avl_node_it node) {
        for (auto& node_ : ascending_range{node}) {
            node_.height_ = 0;
            if (node_.left_)
                node_.height_ = node_.left_->height_;
            if (node_.right_)
                node_.height_ = std::max(node_.height_, node_.right_->height_);

            node_.height_++;
        }
    }

    unique_avl_node& get_unique_ptr(const avl_node_it node, avl_node_it parent) {
        if (std::addressof(*node) == root_.get()) {
            return root_;
        } else {
            if (comp_func(node->key_, parent->key_))
                return parent->left_;
            else
                return parent->right_;
        }
    }

    void balance(avl_node_it node) {
        if (!node.is_valid())
            return;

        unique_avl_node& destination = get_unique_ptr(node, node->parent_);

        int balance_diff = 0;
        if (node->left_)
            balance_diff = node->left_->height_;
        if (node->right_)
            balance_diff -= node->right_->height_;

        int left_height = 0;
        int right_height = 0;
        if (balance_diff > 1) {
            avl_node_it left = *node->left_;
            if (left->left_)
                left_height = left->left_->height_;

            if (left->right_)
                right_height = left->right_->height_;

            if (left_height < right_height)
                node->left_ = rotate_left(std::move(node->left_));
            destination = rotate_right(std::move(destination));

        } else if (balance_diff < -1) {
            avl_node_it right = *node->right_;
            if (right->left_)
                left_height = right->left_->height_;

            if (right->right_)
                right_height = right->right_->height_;

            if (left_height > right_height)
                node->right_ = rotate_right(std::move(node->right_));
            destination = rotate_left(std::move(destination));
        }
        update_height (destination);
        update_Nchilds(destination);
    }

    unique_avl_node rotate_right(unique_avl_node node) {
        if (!node.get())
            return node;

        avl_node* old_node_parent = node->parent_;

    //-------------------------------------------------------------------------
        unique_avl_node node_lr_old = nullptr;
        if (node->left_->right_)
            node_lr_old = std::move(node->left_->right_);
        unique_avl_node node_l_old = std::move(node->left_);

        node_l_old->right_ = std::move(node);
        node_l_old->right_->left_ = std::move(node_lr_old);
    //-------------------------------------------------------------------------

        unique_avl_node new_node = std::move(node_l_old);
        new_node->parent_ = old_node_parent;
        new_node->right_->parent_ = new_node.get();
        if (new_node->left_)
            new_node->left_->parent_ = new_node.get();
        if (new_node->right_->left_)
            new_node->right_->left_->parent_ = new_node->right_.get();

        update_height (new_node->right_);
        update_Nchilds(new_node->right_);
        return new_node;
    }

    unique_avl_node rotate_left(unique_avl_node node) {
        if (!node.get())
            return node;

        avl_node* old_node_parent = node->parent_;

    //-------------------------------------------------------------------------
        unique_avl_node node_rl_old = nullptr;
        if (node->right_->left_)
            node_rl_old = std::move(node->right_->left_);
        unique_avl_node node_r_old = std::move(node->right_);

        node_r_old->left_ = std::move(node);
        node_r_old->left_->right_ = std::move(node_rl_old);
    //-------------------------------------------------------------------------

        unique_avl_node new_node = std::move(node_r_old);
        new_node->parent_ = old_node_parent;
        new_node->left_->parent_ = new_node.get();
        if (new_node->right_)
            new_node->right_->parent_ = new_node.get();
        if (new_node->left_->right_)
            new_node->left_->right_->parent_ = new_node->left_.get();

        update_height (new_node->left_);
        update_Nchilds(new_node->left_);
        return new_node;
    }

    avl_node_it get_parent_min_bigger(const avl_node_it node, const KeyT& key) const {
        avl_node_it ans_node = node;
        KeyT        ans_key  = max_key_;
        for (auto& node_ : ascending_range{node}) {
            if (comp_func(key,        node_.key_) &&
                comp_func(node_.key_, ans_key)) {

                ans_node = node_;
                ans_key  = node_.key_;
            }
        }
        return ans_node;
    }

    avl_node_it lower_bound(const KeyT& key) const {
        if (!root_)
            return nullptr;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_min_bigger(current, key);
            } else if (comp_func(current->key_, key)) {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_min_bigger(current, key);
            } else {
                return current;
            }
        }
        return current;
    }

    avl_node_it upper_bound(const KeyT& key) const {
        if (!root_)
            return nullptr;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_min_bigger(current, key);
            } else {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_min_bigger(current, key);
            }
        }
        return current;
    }

    int size_left_side(const KeyT& key) const {
        if (!root_)
            return 0;

        int dist = 0;
        avl_node_it current = *root_;
        while (current.is_valid()) {
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

    void print_node(const avl_node_it node) const noexcept {
        if (!node.is_valid())
            return;

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
                                 node->height_ << ",\t" << std::addressof(*node) << ")\n");
    }

    void print_subtree(const avl_node_it node) const {
        if (!node.is_valid())
            return;

        int used_size = std::pow(2, std::log2(get_node_size(node)) + 1);
        std::vector<bool> used(used_size, false);

        avl_node_it current = node;
        int current_index = 1;
        while (current.is_valid()) {

            avl_node_it left_it = current->left_;
            if (left_it.is_valid() &&
                !used[current_index * 2]) {
                current = current->left_;
                current_index = current_index * 2;
                continue;
            }

            if (!used[current_index])
                print_node(current);
            used[current_index] = true;

            avl_node_it right_it = current->right_;
            if (right_it.is_valid() &&
                !used[current_index * 2 + 1]) {
                current = current->right_;
                current_index = current_index * 2 + 1;
                continue;
            }

            current = current->parent_;
            current_index /= 2;
        }
    }

    void decrement_parents_size(const avl_node_it node) {
        for (auto& node_ : ascending_range{node}) {
            if (!node_.parent_)
                break;

            if (node_.parent_->left_.get() == &node_)
                node_.parent_->Nleft_--;
            else
                node_.parent_->Nright_--;
        }
    }

    avl_node_it find(const KeyT& key) const {
        if (!root_)
            return 0;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return *current;
            } else if (comp_func(current->key_, key)) {
                if (current->right_)
                    current = current->right_;
                else
                    return *current;
            } else {
                return *current;
            }
        }
        return *root_;
    }

public:
    avl_tree_t(const KeyT& max_key) : max_key_(max_key) {}

    avl_tree_t(const avl_tree_t<KeyT, CompT>& other) {
        avl_node_it curr_other = other.root_;
        if (!curr_other.is_valid())
            return;

        root_ = std::make_unique<avl_node>(other.root_->key_);
        avl_node_it curr_this = root_;

        while (curr_other.is_valid()) {

            if (curr_other->left_ && !curr_this->left_) {
                curr_other       = curr_other->left_;
                curr_this->left_ = std::make_unique<avl_node>(curr_other->key_);
                curr_this->left_->parent_ = std::addressof(*curr_this);
                curr_this        = curr_this->left_;

            } else if (curr_other->right_ && !curr_this->right_) {
                curr_other        = curr_other->right_;
                curr_this->right_ = std::make_unique<avl_node>(curr_other->key_);
                curr_this->right_->parent_ = std::addressof(*curr_this);
                curr_this         = curr_this->right_;

            } else {
                if (curr_other->parent_) {
                    update_height (curr_this);
                    update_Nchilds(curr_this);
                    curr_other = curr_other->parent_;
                    curr_this  = curr_this->parent_;
                } else {
                    break;
                }
            }
        }
    }

    avl_tree_t<KeyT, CompT>& operator=(const avl_tree_t<KeyT, CompT>& other) {
        if (this == &other)
            return *this;

        avl_tree_t<KeyT, CompT> new_tree{other};
        std::swap(root_, new_tree.root_);
        return *this;
    }

    avl_tree_t(avl_tree_t<KeyT, CompT>&& other) noexcept : comp_func(std::move(other.comp_func)),
                                                           root_    (std::move(other.root_)), 
                                                           max_key_ (std::move(other.max_key_)) {
        other.root_ = nullptr;
    }
    
    avl_tree_t& operator=(avl_tree_t<KeyT, CompT>&& other) noexcept {
        if (this == &other)
            return *this;

        std::swap(comp_func, other.comp_func);
        std::swap(root_,     other.root_);
        std::swap(max_key_,  other.max_key_);
        return *this;
    }

    void print() const {
        if (!root_)
            return;

        std::cerr << print_lblue("AVL Tree with root = " << root_->key_ <<
                                 ":\nkey(<child>, <child>, <parent>, <Nleft>, <Nright>, <height>, <ptr>):\n");

        print_subtree(root_);
        std::cerr << '\n';
    }

    const_avl_node_it insert(const KeyT& key) {
        if (!root_) {
            root_ = std::make_unique<avl_node>(key);
            return root_;
        }

        avl_node_it current     = *root_;
        avl_node_it destination = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_) {
                    current = current->left_;
                } else {
                    current->left_ = std::make_unique<avl_node>(key);
                    current->left_->parent_ = std::addressof(*current);
                    destination = current->left_;
                    break;
                }
            } else if (comp_func(current->key_, key)) {
                if (current->right_) {
                    current = current->right_;
                } else {
                    current->right_ = std::make_unique<avl_node>(key);
                    current->right_->parent_ = std::addressof(*current);
                    destination = current->right_;
                    break;
                }
            } else {
                return current;
            }
        }

        update_height (destination);
        update_Nchilds(destination);

        for (auto& node_ : ascending_range{destination})
            balance(node_);

        return find(destination->key_);
    }

    int check_range(const KeyT& first_key, const KeyT& second_key) const {
        if (first_key >= second_key || !root_)
            return 0;

        avl_node_it left  = lower_bound(first_key);
        avl_node_it right = upper_bound(second_key);

        int border = -1;
        if (first_key <= left->key_)
            border++;
        if (second_key >= right->key_)
            border++;

        return size_left_side(right->key_) - size_left_side(left->key_) + border;
    }

    ~avl_tree_t() {
        if (!root_)
            return;

        avl_node_it current = root_;
        while (current.is_valid()) {
            if (current->Nleft_ > 0) {
                int node_left_size = get_node_size(current->left_);
                if (node_left_size == 1) {
                    decrement_parents_size(current->left_);
                    current->left_.reset();
                } else if (node_left_size > 1) {
                    current = current->left_;
                }

            } else if (current->Nright_ > 0) {
                int node_right_size = get_node_size(current->right_);

                if (node_right_size == 1) {
                    decrement_parents_size(current->right_);
                    current->right_.reset();
                } else if (node_right_size > 1) {
                    current = current->right_;
                }

            } else {
                current = current->parent_;
            }
        }
    }
};
};