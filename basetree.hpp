#pragma once
#include "treenode.hpp"
#include <condition_variable>
#include <corecrt.h>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits.h>
#include <new>
#include <ostream>
#include <mutex>
#include <random>
#include <stack>
#include <chrono>
#include <system_error>
using namespace std::chrono_literals;

template<typename>
class BaseTree
{
    public:
        BaseTree() = default;
        virtual ~BaseTree() = default;
        virtual bool empty()const = 0;
        virtual std::size_t Size()const = 0;
};

template<typename T>
class BStree : public BaseTree<T>
{
    public:
        BStree() = default;
        virtual ~BStree(){
            PostOrder(root,[](BasetreeNode<T>* node){ delete node;});
        }
        
        bool empty()const{return size == 0;}
        std::size_t Size()const{return size;}

        BasetreeNode<T>* search(const T& value)const;

        BasetreeNode<T>* PreNode(BasetreeNode<T>* node)const;
        BasetreeNode<T>* NextNode(BasetreeNode<T>* node)const;

        virtual bool insert(const T& value);
        virtual bool remove(const T& value);


    protected:
        BasetreeNode<T>* root;
        std::size_t size;        
};

template <typename T>
BasetreeNode<T>* BStree<T>::search(const T& value)const{
    if(empty())
    {
        std::cout<<"the tree is empty,message come from searching for "<<value<<"\n";
        return nullptr;
    }

    BasetreeNode<T>* current = root;
    while(current!= nullptr)
    {
        if(current ->data == value)
        {
            std::cout<<"value "<<value<<" founded. message from searching for"<<value<<"\n";
            return current;
        }
        else if(current->data > value)
        {
            current = current->left;
        }
        else if(current->data < value)
        {
            current = current->right;
        }
        else {
            std::cout<<"value "<<value<<"not found. message from searching for "<<value<<"\n";
        }
    }
    std::cout<<"value:"<<value<<"not found.message from searching for "<<value<<"\n";
    return nullptr;


}

template<typename T>
BasetreeNode<T>* BStree<T>::NextNode(BasetreeNode<T>* node)const{
    if(node == nullptr)
    {
        std::cout<<"node is nullptr, message from NextNode\n";
        return nullptr;
    }    

    if(node->right == nullptr)
    {
        std::cout<<"next value for value:"<<node->data<<" does not exist. message from NextNode\n";
        return nullptr;
    }
    BasetreeNode<T>* current = node->right;
    while(current->left != nullptr)
        current = current->left;

    std::cout<<"next node founded. message from NextNode\n";
    return current;
}

template<typename  T>
BasetreeNode<T>* BStree<T>::PreNode(BasetreeNode<T>* node)const{
    if(node == nullptr)
    {
        std::cout<<"node is nullptr, message from NextNode\n";
        return nullptr;
    }

    if(node->left == nullptr)
    {
        std::cout<<"previous value for value:"<<node->data<<" does not exist. message from NextNode\n";
        return nullptr;
    }

    BasetreeNode<T>* current = node->left;
    while(current ->right != nullptr)
        current = current->right;

    std::cout<<"previous node founded. message from PreNode\n";
    return current;
}

template<typename T>
bool BStree<T>::insert(const T& value)
{
    BasetreeNode<T>* target = new BasetreeNode<T>(value);
    if(root == nullptr)
    {
        root = target;
        size++;
        return true;
    }

    BasetreeNode<T>* current = root;
    
    int flag;
    while (1) {
        if(current ->data > value && current->left != nullptr)
            current = current->left;
        else if(current ->data > value && current->left == nullptr)
        {
            flag = 0;
            break;
        }
        else if(current->data < value && current->right != nullptr)
            current = current->right;
        else if(current->data < value && current->right == nullptr)
        {
            flag = 1;
            break;
        }
        else {
            std::cout<<"the value already existed. message from insertinf value"<<value<<"\n";
            return false;
        }
    }

    target->parent = current;
    if(flag)
        current->right = target;
    else
        current->left = target;

    size++;
    return true;
}

template <typename T>
bool BStree<T>::remove(const T& value)
{
    if(root == nullptr)
    {
        std::cout<<"the tree is empty. message from remove value:"<<value<<"\n";
        return false;
    }
    BasetreeNode<T>* target = search(value);
    if(target == nullptr)
    {
        std::cout<<value<<" not exsit\n";
        return false;
    }
    if(target ->left == nullptr && target->right == nullptr)
    {
        if(target == root)
        {
            delete  root;
            root = nullptr;
            size--;
            std::cout<<"tree empty. message from remove value:"<<value<<"\n";
            return true;
        }   

        if(target->data < target->parent->data)
        {
            target->parent -> left = nullptr;
        }
        else {
            target->parent ->right = nullptr;
        }
        delete  target;
        size--;
        return true;
    }

    //the node has child
    if(NextNode(target) == nullptr)
    {
        BasetreeNode<T>* exchange = PreNode(target);
        const T temp = exchange->data;
        if(exchange->data < exchange->parent->data)
        {
            if(exchange->left != nullptr)
            {
            exchange->parent->left = exchange->left;
            exchange->left->parent = exchange->parent;
            }
            else {
                exchange->parent->left = nullptr;
            }
        }
        else {
            if(exchange->left != nullptr)
            {
            exchange->parent->right = exchange->left;
            exchange->left->parent = exchange->parent;
            }
            else {
                exchange->parent->right = nullptr;
            }
        }
        target->data = temp;
        delete exchange;
        size--;
        return true;

    }

    else {
        BasetreeNode<T>* exchange = NextNode(target);
        const T temp = exchange->data;
        if(exchange->data < exchange->parent->data)
        {
            if(exchange->right != nullptr)
            {
            exchange->parent->left = exchange->right;
            exchange->right->parent = exchange->parent;
            }
            else {
            exchange->parent->left = nullptr;
            }
        }
        else {
            if(exchange->right!= nullptr)
            {
            exchange->parent->right = exchange->right;   
            exchange->right->parent = exchange->parent;
            }
            else {
            exchange->parent->right = nullptr;
            }
        }
        target->data = temp;
        delete exchange;               
        size--;
        return true;
    }
}

template<typename T>
class BRtree : public BStree<T>
{
    public:
        BRtree() = default;
        ~BRtree(){
        }
        bool empty()const{return BStree<T>::empty();}
        std::size_t Size()const{return this->size;}

        BasetreeNode<T>* search(const T& value)const;
        bool insert(const T& value);
        bool remove(const T& value);

    private:
        bool insert_helper_location(BRTreeNode<T>* target);
        bool insert_helper_fix(BRTreeNode<T>* target);
        void left_rotate(BasetreeNode<T>* node);
        void right_rotate(BasetreeNode<T>* node);

        BRTreeNode<T>* remove_helper_real_delete(BRTreeNode<T>* target);
        bool remove_helper_fix(BRTreeNode<T>* parent,BRTreeNode<T>* brother);
        std::mutex mtx;
        std::condition_variable not_empty;
};

template<typename T>
BasetreeNode<T>* BRtree<T>::search(const T& value)const{
    return BStree<T>::search(value);
}

template<typename T>
bool BRtree<T>::insert(const T& value)
{
    std::lock_guard<std::mutex> lock(mtx);

    BRTreeNode<T>* target = new BRTreeNode<T>(value);
    if(this->root == nullptr)
    {
        this->root = target;
        target->__color = BRTreeNode<T>::BLACK;
        this->size++;
        return true;
    }
    
    if(!insert_helper_location(target))
        return false;

    if(!insert_helper_fix(target))
        return false;

    not_empty.notify_one();
    return true;

}

template<typename T>
bool BRtree<T>::insert_helper_location(BRTreeNode<T>* target)
{
    BRTreeNode<T>* current = static_cast<BRTreeNode<T>*>(this->root);

    while(current != nullptr)
    {
        if(current->data < target->data && current->right != nullptr)
            current = static_cast<BRTreeNode<T>*>(current ->right);
        else if (current->data < target->data && current->right == nullptr)
        {
            current->right = target;
            target->parent = current;
            this->size++;
            return true;
        }
        else if(current->data > target->data && current->left != nullptr)
            current = static_cast<BRTreeNode<T>*>(current->left);
        else if(current->data > target->data && current->left == nullptr)
        {
            current->left = target;
            target->parent = current;
            this->size++;
            return true;
        }
        else
        {
            std::cout<<"the value already existed. message from BRtree inserting value:"<<target->data<<"\n";
            return false;
        }    
    }
    return true;

}

template <typename T>
void BRtree<T>::left_rotate(BasetreeNode<T>* target)
{
    BRTreeNode<T>* child = static_cast<BRTreeNode<T>*>(target->right);
    BRTreeNode<T>* parent = static_cast<BRTreeNode<T>*>(target->parent);
    if(target == this->root)
        this->root = child;
    target->right = child->left;
    if(child->left != nullptr)
        child->left->parent = target;
    child->left = target;
    target->parent = child;
    child->parent = parent;
    if(parent!= nullptr)
    {
    if(parent->data > target->data)
        parent->left = child;
    else
        parent->right = child;
    }

}

template<typename T>
void BRtree<T>::right_rotate(BasetreeNode<T>* node)
{
    BRTreeNode<T>* child = static_cast<BRTreeNode<T>*>(node->left);
    BRTreeNode<T>* parent = static_cast<BRTreeNode<T>*>(node->parent);

    if(node == this->root)
        this->root = child;
    node->left = child->right;
    if(child->right != nullptr)
        child->right ->parent = node;

    child->right = node;
    node->parent = child;

    child->parent = parent;
    if(parent!= nullptr)
    {
        if(parent->data > node->data)
            parent->left = child;
        else
            parent->right = child;
    }



}

template<typename T>
bool BRtree<T>::insert_helper_fix(BRTreeNode<T>* target)
{
    if(target->parent == nullptr)
        target->__color = BRTreeNode<T>::BLACK;
    if(target->parent == nullptr||target->parent->parent == nullptr)
        return true;
    BRTreeNode<T>* parent = static_cast<BRTreeNode<T>*>(target->parent);
    if(parent->__color == BRTreeNode<T>::BLACK)
        return true;
    BRTreeNode<T>* uncle = static_cast<BRTreeNode<T>*>(target->parent->parent->data > target->parent->data? target->parent->parent->right:target->parent->parent->left);
    if(parent->__color == BRTreeNode<T>::RED&&uncle!=nullptr&&uncle->__color==BRTreeNode<T>::RED)
    {
        parent->__color = BRTreeNode<T>::BLACK;
        uncle->__color = BRTreeNode<T>::BLACK;
        static_cast<BRTreeNode<T>*>(parent->parent)->__color = BRTreeNode<T>::RED;
        return insert_helper_fix(static_cast<BRTreeNode<T>*>(target->parent->parent));
    }
    
    BRTreeNode<T>* grandPa = static_cast<BRTreeNode<T>*>(target->parent->parent);

    if(uncle==nullptr||uncle->__color == BRTreeNode<T>::BLACK)
    {
        if(parent->data < target->data && grandPa->data > parent->data)
        {
            left_rotate(parent);
            return insert_helper_fix(parent);
        }
        else if(parent->data > target->data && grandPa->data < parent->data)
        {
            right_rotate(parent);
            return insert_helper_fix(parent);
        }
        else if(parent->data < grandPa->data && parent->data > target->data)
        {
            parent->__color = BRTreeNode<T>::BLACK;
            grandPa->__color = BRTreeNode<T>::RED;
            right_rotate(grandPa);
        }
        else {
            parent->__color = BRTreeNode<T>::BLACK;
            grandPa->__color = BRTreeNode<T>::RED;
            left_rotate(grandPa);
        }
    }
    return true;
}

template<typename T>
bool BRtree<T>::remove(const T& value)
{
    std::unique_lock<std::mutex> lock(mtx);
    not_empty.wait_for(lock,3s,[this](){return !empty();});
    BRTreeNode<T>* target = static_cast<BRTreeNode<T>*>(search(value));
    if(target == nullptr)
    {
        std::cout<<"the value "<<value<<"not exsited. message from remove\n";
        return false;
    }

    target = remove_helper_real_delete(target);    
    if(target->__color == BRTreeNode<T>::RED)   
        return BStree<T>::remove(value);

    if(target == this->root)
    {
            delete target;
            this->root = nullptr;
            this->size--;
            return true;
    }

    BRTreeNode<T>* parent = static_cast<BRTreeNode<T>*>(target->parent);
    BRTreeNode<T>* brother = static_cast<BRTreeNode<T>*>(parent->data>target->data?parent->right:parent->left);
    const T& sub_data = target->data;
    BStree<T>::remove(value);

    return remove_helper_fix(parent, brother);
}
template<typename T>
bool BRtree<T>::remove_helper_fix(BRTreeNode<T>* parent,BRTreeNode<T>* brother)
{
    auto black_node = [](BRTreeNode<T>* node)->bool{return node==nullptr||node->__color==BRTreeNode<T>::BLACK ;};
    if(parent->right == brother)
    {
        if(!black_node(brother))
        {
            brother->__color = BRTreeNode<T>::BLACK;
            parent->__color = BRTreeNode<T>::RED;
            left_rotate(parent);
            return remove_helper_fix(parent, static_cast<BRTreeNode<T>*>(parent->right));
        }
        if(brother == nullptr || (black_node(brother) && black_node(static_cast<BRTreeNode<T>*>(brother->left)) && black_node(static_cast<BRTreeNode<T>*>(brother->right))))
        {
            if(brother != nullptr)
                brother->__color = BRTreeNode<T>::RED;
            if(!black_node(parent))
                parent->__color = BRTreeNode<T>::BLACK;
            else
            {
                if(parent == this->root)
                    return true;

                return remove_helper_fix(static_cast<BRTreeNode<T>*>(parent->parent),static_cast<BRTreeNode<T>*>(parent->data < parent->parent->data?
                    parent->parent->right:parent->parent->left));
            }
            return true;
        }
        if(black_node(brother) && !black_node(static_cast<BRTreeNode<T>*>(brother->right)))
        {
            brother->__color = parent->__color;
            parent->__color = BRTreeNode<T>::RED;
            static_cast<BRTreeNode<T>*>(brother->right)->__color = BRTreeNode<T>::RED;
            left_rotate(parent);
            return true;
        }
        else if(black_node(brother) && !black_node(static_cast<BRTreeNode<T>*>(brother->left)) && black_node(static_cast<BRTreeNode<T>*>(brother->right)))
        {
            static_cast<BRTreeNode<T>*>(brother->left)->__color = BRTreeNode<T>::BLACK;
            brother->__color = BRTreeNode<T>::RED;
            right_rotate(brother);
            return true;
        }


    }
    else {
        if(!black_node(brother))
        {
            brother->__color = BRTreeNode<T>::BLACK;
            parent->__color = BRTreeNode<T>::RED;
            right_rotate(parent);
            return remove_helper_fix(parent, static_cast<BRTreeNode<T>*>(parent->left));
        }
        if(brother == nullptr || (black_node(brother) && black_node(static_cast<BRTreeNode<T>*>(brother->left)) && black_node(static_cast<BRTreeNode<T>*>(brother->right))))
        {
            if(brother != nullptr)
                brother->__color = BRTreeNode<T>::RED;
            if(!black_node(parent))
                parent->__color = BRTreeNode<T>::BLACK;
            else
            {
                if(parent == this->root)
                    return true;

                return remove_helper_fix(static_cast<BRTreeNode<T>*>(parent->parent),
                static_cast<BRTreeNode<T>*>(parent->data < parent->parent->data?parent->parent->right:parent->parent->left));
            }
            return true;
        }
        if(black_node(brother) && !black_node(static_cast<BRTreeNode<T>*>(brother->left)))
        {
            brother->__color = parent->__color;
            parent->__color = BRTreeNode<T>::RED;
            static_cast<BRTreeNode<T>*>(brother->left)->__color = BRTreeNode<T>::RED;
            right_rotate(parent);
            return true;
        }
        else if(black_node(brother) && !black_node(static_cast<BRTreeNode<T>*>(brother->right)) && black_node(static_cast<BRTreeNode<T>*>(brother->left)))
        {
            static_cast<BRTreeNode<T>*>(brother->right)->__color = BRTreeNode<T>::BLACK;
            brother->__color = BRTreeNode<T>::RED;
            left_rotate(brother);
            return true;
        }
    }   
    return true;
}

template<typename T>
BRTreeNode<T>* BRtree<T>::remove_helper_real_delete(BRTreeNode<T>* target)
{
    if(target->left==nullptr && target->right == nullptr)
        return target;
    if(target->right != nullptr)
    {
        return static_cast<BRTreeNode<T>*>(BStree<T>::NextNode(target));
    }
    
    return static_cast<BRTreeNode<T>*>(BStree<T>::PreNode(target));
}