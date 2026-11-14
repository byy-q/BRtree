#pragma once
#include "treenode.hpp"
#include <cstddef>
#include <iostream>
#include <limits.h>

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

    private:
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