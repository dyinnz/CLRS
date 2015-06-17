#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

template <class T, int kMaxSize = 100>
class Stack {
public:
    Stack() : size_(0) {}

    bool Empty() const {
        return 0 == size_;
    }

    void Push(T x) {
        p_[size_++] = x;
    }

    T Pop() {
        assert(!Empty());
        return p_[--size_];
    }

    size_t Size() const {
        return size_;
    }
    
private:
    T       p_[kMaxSize];
    size_t  size_;
};

template <class T, int kMaxSize = 100>
class Queue {
public:
    Queue() : head_(0), tail_(0) {}

    bool Empty() const {
        return head_ == tail_;
    }

    void Push(T x) {
        // assert(Size() < kMaxSize);
        p_[tail_++] = x;
        if (tail_ == kMaxSize+1) tail_ = 0;
    }

    T Pop() {
        assert(!Empty());
        T &x = p_[head_];
        if (++head_ == kMaxSize+1) head_ = 0;
        return x;
    }

    size_t Size() const {
        if (tail_ >= head_) return tail_ - head_;
        else return kMaxSize+1 - (head_ - tail_);
    }

private:
    T       p_[kMaxSize+1]; 
    size_t  head_, tail_;
};

template <class T, int kMaxSize = 100>
class Deque {
public:
    Deque() : head_(kMaxSize/2), tail_(kMaxSize/2+1) {}

    bool Empty() const {
        return tail_ == head_+1 || (tail_ == 0 && head_ == kMaxSize);
    } 
    
    void PushFront(T x) {
        p_[head_] = x;

        if (0 == head_) head_ = kMaxSize;
        else --head_;
    }

    T PopFront() {
        assert(!Empty());
        if (kMaxSize+1 == ++head_) head_ = 0;
        return p_[head_];
    }

    void PushBack(T x) {
        p_[tail_++] = x;
        if (kMaxSize+1 == tail_) tail_ = 0;
    }

    T PopBack() {
        assert(!Empty());
        if (0 == tail_) tail_ = kMaxSize;
        else --tail_;
        return p_[tail_];
    }

    size_t Size() const {
        if (tail_ > head_) return tail_ - head_ - 1;
        else return kMaxSize+1 - (head_ - tail_);
    }

public:  
    T      p_[kMaxSize+1];
    size_t  head_, tail_;
};

/*
template<class T>
class DoubleList {
public:
    DoubleList() : head_(0) {}

    ~DoubleList() {
        Node *p = head_;
        Node *t = 0;
        while (p) {
            t = p;
            p = p->next;
            delete t;
        }
    }

    struct Node {
        Node *prev, *next;
        T x;
    };

    void Insert(T x) {
        Node *p = new Node;
        p->x = x;

        p->next = head_;
        if (head_) {
            head_->prev = p;
        }

        head_ = p;
        p->prev = 0;
    }

    Node* Search(T x) {
        Node *p = head_;
        while (p && p->x != x) {
            p = p->next;
        }
        return p;
    }

    void Delete(Node *p) {
        assert(p);
        if (p->prev) {
            p->prev->next = p->next;
        } else {
            head_ = p->next;
        }
        if (p->next) {
            p->next->prev = p->prev;
        }
        delete p;
    }

private:
    Node *head_;
};
*/

template<class T>
class DoubleList {
public:
    DoubleList() {
        pivot_.next = &pivot_;
        pivot_.prev = &pivot_;
    }

    ~DoubleList() {
        Node *p = pivot_.next;
        Node *t = 0;
        while (p != &pivot_) {
            t = p; 
            p = p->next;
            delete t;
        }
    }

    struct Node {
        Node *prev, *next;
        T x;
    };

    void PushFront(T x) {
        Node *p = new Node;
        p->x = x;

        p->next = pivot_.next;
        pivot_.next->prev = p;
        pivot_.next = p;
        p->prev = &pivot_;
    }

    void PushBack(T x) {
        Node *p = new Node;
        p->x = x;

        p->prev = pivot_.prev;
        pivot_.prev->next = p;
        pivot_.prev = p;
        p->next = &pivot_;
    }
    
    Node* Search(T x) {
        Node *p = pivot_.next;
        while (p != &pivot_ && p->x != x) {
            p = p->next; 
        }
        if (p == &pivot_) return 0;
        else return p;
    }

    void Delete(Node *p) {
        assert(p);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
    }

private:
    Node pivot_;    
};

template<class T>
class SingleList {
public:
    SingleList() : head_(0) {}
    
    ~SingleList() {
        Node *p = head_;
        Node *t = 0;
        while (p) {
            t = p;
            p = p->next;
            delete t;
        }
    }

    struct Node {
        Node *next;
        T x;
    };

    Node *Head() {
        return head_;
    }

    const Node *Head() const {
        return head_;
    }

    Node* Search(T x) {
        return const_cast<Node*> ( const_cast<const SingleList*>(this)->Search(x) );
    }

    const Node* Search(T x) const {
        Node *p = head_;
        while (p && p->x != x) {
            p = p->next;
        }
        return p;
    }

    void Insert(T x) {
        Node *p = new Node;
        p->x = x;
        p->next = head_;
        head_ = p;
    }

    void Delete(T x) {
        if (!head_) return;

        Node *t = 0;
        if (head_->x == x) {
            t = head_; 
            head_ = head_->next;
            delete t;
            return;
        }

        Node *p = head_;
        while (p->next) {
            if (p->next->x == x) {
                t = p->next;
                p->next = p->next->next;
                delete t; 
                break;
            }

            p = p->next;
        }
    }

    void Reverse() {
        Node *p = head_;
        head_ = 0;
        while (p) {
            Node *t = p->next;
            p->next = head_;
            head_ = p;
            p = t;
        }
    }

private:
    Node *head_;
};

template<class T>
class XorList {
public:
    XorList() : head_(&pivot_) {
        pivot_.np = 0;
    }

    struct Node {
        Node *np;
        T x;
    };

    Node *Xor(Node *l, Node *r) {
        return (Node*)(int(l) ^ int(r));
    }

    void Insert(T x) {
        Node *p = new Node;
        p->x = x;

        p->np = Xor(head_, &pivot_);
        head_->np = Xor(p, Xor(&pivot_, head_->np));
        pivot_.np = Xor(Xor(pivot_.np, head_), p);
        head_ = p;
    }
    
    Node* Search(T x) {
        Node *p = head_;
        Node *last = &pivot_;
        Node *temp = 0;
        while (p != &pivot_ && p->x != x) {
            temp = p;
            p = Xor(last, p->np);
            last = temp;
        }
        return p;
    }

    void Delete(T x) {
        Node *p = head_;
        Node *last = &pivot_;
        Node *temp = 0;
        while (p) {
            if (p->x == x) {
                Node *after_p = Xor(last, p->np);

                last->np = Xor(Xor(last->np, p), after_p);
                after_p->np = Xor(last, Xor(p, after_p->np));

                return;
            }

            temp = p;
            p = Xor(last, p->np);
            last = temp;
        }
    }

    void Reverse() {
        head_ = Xor(pivot_.np, head_);
    }

    void Print() {
        Node *p = head_;
        Node *last = &pivot_;
        Node *temp = 0;
        while (p != &pivot_) {
            cout << p << ' ' << p->x << endl;

            temp = p;
            p = Xor(last, p->np);
            last = temp;
        }
    }

private:
    Node pivot_;
    Node *head_;
};


int main() {
    Stack<int> s;
    s.Push(10);
    s.Push(20);
    cout << s.Empty() << endl;
    while (!s.Empty()) {
        cout << s.Pop() << endl;
    }

    Queue<int, 4> q;
    q.Push(1);
    q.Push(2);
    q.Push(3);
    cout << q.Empty() << endl;
    cout << q.Pop() << endl;
    cout << q.Pop() << endl;
    q.Push(4);
    q.Push(5);
    cout << q.Pop() << endl;

    Deque<int, 5> dq;
    dq.PushFront(10);
    dq.PushFront(11);
    cout << dq.head_ << '-' << dq.tail_ << endl;
    dq.PushFront(15);
    cout << dq.head_ << '-' << dq.tail_ << endl;
    dq.PushBack(13);
    cout << dq.head_ << '-' << dq.tail_ << endl;
    dq.PushBack(14);
    cout << dq.head_ << '-' << dq.tail_ << endl;

    while (!dq.Empty()) {
        cout << dq.head_ << '-' << dq.tail_ << endl;
        cout << dq.PopFront() << endl;
    }

    cout << dq.head_ << '-' << dq.tail_ << endl;
    dq.PushBack(14);
    cout << dq.head_ << '-' << dq.tail_ << endl;

    DoubleList<int> l;
    l.PushFront(11);
    l.PushFront(12);
    l.PushBack(13);
    l.PushBack(14);

    cout << l.Search(11)->x << endl;
    cout << l.Search(12)->x << endl;
    cout << l.Search(13)->x << endl;
    cout << l.Search(14)->x << endl;

    l.Delete( l.Search(12) );
    cout << l.Search(12) << endl;

    SingleList<int> sl;
    sl.Insert(11);
    sl.Insert(12);
    sl.Insert(13);
    sl.Insert(14);

    cout << sl.Search(11)->x << endl; 
    cout << sl.Search(12)->x << endl; 
    sl.Delete( 12 );
    cout << sl.Search(13)->x << endl;
    cout << sl.Search(14)->x << endl;
    sl.Insert(15);

    cout << endl;
    auto *p = sl.Head();
    while (p) {
        cout << p->x << endl;
        p = p->next;
    }
    sl.Reverse();

    cout << endl;
    p = sl.Head();
    while (p) {
        cout << p->x << endl;
        p = p->next;
    }

    XorList<int> xl;
    xl.Insert(1);
    xl.Insert(2);
    xl.Insert(3);
    xl.Insert(4);
    xl.Delete(2);
    xl.Print();
    xl.Reverse();
    xl.Print();
    cout << xl.Search(1)->x << endl;

   return 0;
}
