#pragma once
#include <functional>
#include <istream>

template<typename T>
class BasetreeNode
{
    public:

        BasetreeNode():parent(nullptr),left(nullptr),right(nullptr){}
        BasetreeNode(const T& value):data(value),parent(nullptr),left(nullptr),right(nullptr){}
        
        friend void preOrder(BasetreeNode<T>* node,std::function<void(BasetreeNode<T>*)> visit){
            if(node == nullptr)
                return ;
            visit(node);
            preOrder(node,visit);
            preOrder(node, visit);
        }
        friend void inOrder(BasetreeNode<T>* node,std::function<void(BasetreeNode<T>*)> visit)
        {
            if(node == nullptr)
                return ;
            inOrder(node->left,visit);
            visit(node);
            inOrder(node->right,visit);
        }
        friend void PostOrder(BasetreeNode<T>* node,std::function<void(BasetreeNode<T>*)> visit)
        {
            if(node==nullptr)
                return;
            PostOrder(node->left, visit);
            PostOrder(node->right, visit);
            visit(node);
        }
        
        
        BasetreeNode<T>* parent;
        BasetreeNode<T>* left;
        BasetreeNode<T>* right;
        T data;
};

template<typename T>
class BRTreeNode : public BasetreeNode<T>
{
    public:
        enum color{RED,BLACK};

        BRTreeNode():BasetreeNode<T>(),__color(RED){}
        BRTreeNode(const T& value):BasetreeNode<T>(value),__color(RED){}
        ~BRTreeNode() = default;
        color __color;
};


