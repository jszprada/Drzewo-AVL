#include <iostream>
using namespace std;
//=================================================================================================
//                                    Klasa Node
//=================================================================================================
template <class T>
class Node{
    public:
        Node<T> *left;
        Node<T> *right;
        T value;
        int bal;
        int level;
        int lPath;
        Node(T val): value(val), left(nullptr), right(nullptr), bal(0), level(0), lPath(0){}
        
        void setBal();
        Node<T>* balTree();
        int setLPath(Node<T> *node);
        void deLevel();
        Node<T>* roLeft();
        Node<T>* roRight();
        
        void rLevel(int l = -1){
            if(l == -1) level--;
            else level = l+1;
            if(left)
            left->rLevel(level);
            if(right)
            right->rLevel(level);
        }
        Node<T>* gLeft(){
            return left;
        }
        Node<T>* gRight(){
            return right;
        }
        void sLeft(Node<T> *l){
            left = l;
        }
        void sRight(Node<T> *r){
            right = r;
        }
        int modBal(){
            if(bal<0) return bal*(-1);
            return bal;
        }
};
//=================================================================================================
//                                    Ustawa najdlusza sciezke
//=================================================================================================
template <class T> int Node<T>::setLPath(Node<T> *node){
    if(node == nullptr) return 0;
    if(node->level - level > lPath) lPath = node->level - level;
    setLPath(node->left);
    setLPath(node->right);
    return lPath;
}
//=================================================================================================
//                                    Ustaw balans
//=================================================================================================
template <class T> void Node<T>::setBal(){
    setLPath(this);
    if(left == nullptr and right == nullptr) return;
    if(left != nullptr and right != nullptr){ 
        left->setBal();
        right->setBal();
        bal = right->lPath - left->lPath;
    }
    else if(left != nullptr){
        left->setBal();
        bal = 0 - lPath;
    }
    else{
        right->setBal();
        bal = lPath;
    }
}
//=================================================================================================
//                                    Balans drzewa
//=================================================================================================
template <class T> Node<T>* Node<T>::balTree(){
    if(bal < 0){
        if(left->bal <= 0){
            return roRight();
        }else{
            left = left->roLeft();
            return roRight();
        }
    }else{
        if(right->bal >= 0){
            return roLeft();
        }else{
            right = right->roRight();
            return roLeft();
        }
    }
}
//=================================================================================================
//                                   Zmniejszanie poziomu
//=================================================================================================
template <class T> void Node<T>::deLevel(){
    level--;
    if(left != nullptr) left->deLevel();
    if(right != nullptr) right->deLevel();
}
//=================================================================================================
//                                   Obrot w prawo
//=================================================================================================
template <class T>  Node<T>* Node<T>::roRight(){
    Node<T> *nodeB = left;
    left = nodeB->right;
    nodeB->right = this;
    int Abf = bal, Bbf = nodeB->bal;
    if(Bbf <= 0){
        if(Bbf > Abf) nodeB->bal = Bbf + 1;
        else nodeB->bal = Abf + 2;
        bal = Abf - Bbf + 1;
    }else{
        if(Abf <= -1) nodeB->bal = Bbf + 1;
        else nodeB->bal = Abf + Bbf + 2;
        bal = Abf + 1;
    }
    nodeB->rLevel();
    return nodeB;
}
//=================================================================================================
//                                   Obrot w lewo
//=================================================================================================
template <class T>  Node<T>* Node<T>::roLeft(){
    Node<T> *nodeB = right;
    right = nodeB->left;
    nodeB->left = this;
    int Abf = bal, Bbf = nodeB->bal;
    if(Bbf <= 0){
        if(Abf >= 1) nodeB->bal = Bbf - 1;
        else nodeB->bal = Abf + Bbf - 2;
        bal = Abf - 1;
    }else{
        if(Abf <= Bbf) nodeB->bal = Abf - 2;
        else nodeB->bal = Bbf - 1;
        bal = Abf - Bbf - 1;
    }
    nodeB->rLevel();
    return nodeB;
}
//=================================================================================================
//                                   Klasa AVL
//=================================================================================================
template <class T>
class AVL{
    public:
        Node<T> *first;
        
        AVL():first(nullptr){}
        
        void add(T value);
        void erase(T value, Node<T> *start);
        bool search(T value);
        void displayPre(Node<T> *node);
        void displayIn(Node<T> *node);
        void displayPost(Node<T> *node);
        void bal(T value);
};
//=================================================================================================
//                                   AVL -> balans
//=================================================================================================
template <class T> void AVL<T>::bal(T value){
    first->setBal();
    if(first->modBal() > 1){//1 nie jest zbalansowany, reszta ok
        if((first->left and first->left->modBal()<2) or (first->right and first->right->modBal()<2)){
            first = first->balTree();
            return;
        }

    }
    Node<T> *current, *child;
    current = first; 
    child = nullptr;
    Node<T> *current_ = nullptr;
    while(current){
        if(value < current->value){
            child = current->left;
        }
        if(value > current->value){
            child = current->right;
        }
        if(child != nullptr and (child->modBal() > 1)){
            current_ = current;
        } 
        if(child == nullptr or value == child->value) break;
        current = child;
    }
    if(current_ != nullptr){
        if(current_->left->modBal() > 1){
            current_->left = current_->left->balTree();
        }else if(current_->right->modBal() > 1){
            current_->right = current_->right->balTree();
        }   
    }
}
//=================================================================================================
//                                   AVL -> dodaj
//=================================================================================================
template <class T> void AVL<T>::add(T value){
    if(first == nullptr){
        first = new Node<T>(value);
        first->level = 0;
        first->setBal();
        return;
    }
    Node<T> *current = first, *child;
    while(current){
        if(value < current->value){
            child = current->left;
            if(child == nullptr){
                current->left = new Node<T>(value);
                current->left->level = current->level + 1;
                break;
            }
        }
        if(value > current->value){
            child = current->right;
            if(child == nullptr){
                current->right = new Node<T>(value);
                current->right->level = current->level + 1;
                break;
            }   
        }
        current = child;
    }
    bal(value);
}
//=================================================================================================
//                                   AVL -> usun
//=================================================================================================
template <class T> void AVL<T>::erase(T value, Node<T> *start = nullptr){
    if(first == nullptr) return;
    if(start == nullptr and first->value == value){
        if(first->left == nullptr and first->right == nullptr){
            first = nullptr;
            return;
        }
        if(first->left != nullptr and first->right == nullptr){
            first->left->deLevel();
            first = first->left;
            bal(first->value);
            return;
        }
        if(first->left == nullptr and first->right != nullptr){
            first->right->deLevel();
            first = first->right;
            bal(first->value);
            return;
        }
        Node<T> *next = first, *child = first->right;
        while(child->left){
            next = child;
            child = child->left;
        }
        erase(child->value, next);
        first->value = child->value;
        bal(first->value);
        return;
    }
    Node<T> *current, *child = nullptr;
    if(start == nullptr) current = first;
    else current = start;
    while(current){
        if(value < current->value){
            child = current->left;
            if(child == nullptr) return;
            if(child->value == value){
                if(child->left == nullptr and child->right == nullptr){
                    current->left = nullptr;
                    break;
                }
                if(child->left != nullptr and child->right == nullptr){
                    child->left->deLevel();
                    current->left = child->left;
                    break;
                }
                if(child->left == nullptr and child->right != nullptr){
                    child->right->deLevel();
                    current->left = child->right;
                    break;
                }
                Node<T> *next = child, *neChild = next->right;
                while(neChild->left){
                    next = neChild;
                    neChild = neChild->left;
                }
                erase(neChild->value, next);
                child->value = neChild->value;
                break;
            }
        }
        if(value > current->value){
            child = current->right;
            if(child == nullptr) return;
            if(child->value == value){
                if(child->left == nullptr and child->right == nullptr){
                    current->right = nullptr;
                    break;
                }
                if(child->left != nullptr and child->right == nullptr){
                    child->left->deLevel();
                    current->right = child->left;
                    break;
                }
                if(child->left == nullptr and child->right != nullptr){
                    child->right->deLevel();
                    current->right = child->right;
                    break;
                }
                Node<T> *next = child, *neChild = next->right;
                while(neChild->left){
                    next = neChild;
                    neChild = neChild->left;
                }
                
                erase(neChild->value, next);
                child->value = neChild->value;
                break;
            }  
        }
    current = child;
    }
    bal(current->value);
}
//=================================================================================================
//                                   AVL -> szukaj
//=================================================================================================
template <class T> bool AVL<T>::search(T value){
    Node<T> *current = first;
    while(current){
        if(current->value == value) return true;
        if(value < current->value) current = current->left;
        else current = current->right;
    }
    return false;
}
//=================================================================================================
//                                   AVL -> Porządek poprzedzający
//=================================================================================================
template <class T> void AVL<T>::displayPre(Node<T> *node){
    if(node == nullptr) return;
    cout<<node->value<<" ";
    displayPre(node->left);
    displayPre(node->right);
}
//=================================================================================================
//                                   AVL -> Porządek wewnętrzny
//=================================================================================================
template <class T> void AVL<T>::displayIn(Node<T> *node){
    if(node == nullptr) return;
    displayIn(node->left);
    cout<<node->value<<" ";
    displayIn(node->right);
}
//=================================================================================================
//                                   AVL ->  Porządek następujący
//=================================================================================================
template <class T> void AVL<T>::displayPost(Node<T> *node){
    if(node == nullptr) return;
    displayPost(node->left);
    displayPost(node->right);
    cout<<node->value<<" ";
}
int main()
{
    AVL<int> DrzewoAVL;
    int val;
    int k;
    cin>>k;
    while(k--){
        cin>>val;
        DrzewoAVL.add(val);
    }
    int l;
    cin>>l;
    DrzewoAVL.erase(l);
    int m;
    cin>>m;
    switch(m){
        case 0:
            DrzewoAVL.displayPre(DrzewoAVL.first);
        break;
        case 1:
            DrzewoAVL.displayIn(DrzewoAVL.first);
        break;
        case 2:
            DrzewoAVL.displayPost(DrzewoAVL.first);
        break;
    }
    return 1;
}
