//
// Created by Jack Byrnes on 2019-02-18.
//

#ifndef HW5_UNROLLED_H
#define HW5_UNROLLED_H

#include <iostream>

const int MAX_NUM_ELEMENTS_PER_NODE = 6;

template<class T>
class UnrolledLL;

template<class T>
class Node {
public:
    // Constructors
    Node() { next_ = NULL, prev_ = NULL, num_elements = 0; }

    // representations
    T value[MAX_NUM_ELEMENTS_PER_NODE];
    unsigned int num_elements;
    Node<T> *next_;
    Node<T> *prev_;
};


template<class T>
class list_iterator {
public:
    list_iterator(Node<T> *p = NULL) : ptr_(p), offset(0) {}

    list_iterator<T> operator++(int) { // post-increment, e.g., iter++
        offset++;
        if (offset > ptr_->num_elements - 1) {
            offset = 0;
            ptr_ = ptr_->next_;
        }
        return *this;
    }


    list_iterator<T> operator--(int) { // post-decrement, e.g., iter--
        offset--;
        if (offset == 0) {
            offset = MAX_NUM_ELEMENTS_PER_NODE - 1;
            ptr_ = ptr_->prev_;
        }
        return *this;
    }


    T &operator*() {
        return ptr_->value[offset];
    }


    bool operator!=(const list_iterator<T> &r) const {
        if (ptr_ != r.ptr_) {
            return true;
        } else if (r.offset != offset) {
            return true;
        }
        return false;
    }


    bool operator==(const list_iterator<T> &r) const {
        return ptr_ == r.ptr_ && r.offset == offset;
    }

    friend class UnrolledLL<T>;

private:
    unsigned int offset;
    Node<T> *ptr_;
};


template<class T>
class UnrolledLL {
public:
    UnrolledLL() { head_ = NULL, tail_ = NULL, size_ = 0; }

    UnrolledLL(const UnrolledLL<T> &old) { copy_list(old); }

    UnrolledLL &operator=(const UnrolledLL<T> &old);

    unsigned int size() const { return size_; }

    T &front() const { return head_->value[0]; }

    T &back() const { return tail_->value[tail_->num_elements - 1]; }

    void push_back(const T &new_value);

    void pop_back();

    void push_front(const T &new_value);

    void pop_front();

    typedef list_iterator<T> iterator;

    iterator begin() { return iterator(head_); }

    iterator end() { return iterator(NULL); }

    iterator erase(iterator itr);

    iterator insert(iterator itr, const T &new_value);

    void print(std::ostream &os);

    ~UnrolledLL() { YOU_SHALL_NOT_PASS(); }


private:
    void YOU_SHALL_NOT_PASS();

    void copy_list(const UnrolledLL<T> &old);

    Node<T> *head_;
    Node<T> *tail_;
    unsigned int size_;
};


template<class T>
void UnrolledLL<T>::YOU_SHALL_NOT_PASS() {
    if (head_ == NULL) {
        std::cerr << "List is fuckin empty, dumbass" << std::endl;
        exit(1);
    } else {
        while (head_ != NULL) {
            Node<T> *ptr = head_;
            head_ = head_->next_;
            delete ptr;
            if (head_ == NULL) {
                break;
            }
        }
    }
    size_ = 0;
    head_ = tail_ = NULL;
}


template<class T>
void UnrolledLL<T>::push_back(const T &new_value) {
    if (!tail_) { // empty list
        Node<T> *newp = new Node<T>;
        newp->value[0] = new_value;
        head_ = tail_ = newp;
        newp->num_elements++;
    } else {
        if (tail_->num_elements < MAX_NUM_ELEMENTS_PER_NODE) {
            tail_->value[tail_->num_elements] = new_value;
            tail_->num_elements += 1;
            // std::cout << tail_ -> num_elements << std::endl;
        } else {
            Node<T> *newp = new Node<T>;
            newp->value[newp->num_elements] = new_value;
            newp->num_elements += 1;
            newp->prev_ = tail_;
            tail_->next_ = newp;
            tail_ = newp;

        }
    }
    size_ += 1;

}


template<class T>
void UnrolledLL<T>::pop_front() {
    assert(size_ > 0);
    if (head_->num_elements - 1 == 0) {
        Node<T> *temp = head_->next_;
        head_->next_->prev_ = NULL;
        delete head_;
        head_ = temp;

    } else {
        for (int i = 0; i < head_->num_elements - 1; i++) {
            head_->value[i] = head_->value[i + 1]; // shiftin array to the right to make a space
        }
        head_->num_elements -= 1;
    }
    size_ -= 1;
}


template<class T>
void UnrolledLL<T>::pop_back() {
    assert(size_ > 0);
    if (tail_->num_elements - 1 == 0) {
        Node<T> *temp = tail_->prev_;
        tail_->prev_->next_ = NULL;
        delete tail_;
        tail_ = temp;

    } else {
        tail_->value[tail_->num_elements - 1] = NULL;
        tail_->num_elements -= 1;
    }
    size_ -= 1;
}


template<class T>
void UnrolledLL<T>::push_front(const T &new_value) {
    if (head_->num_elements < MAX_NUM_ELEMENTS_PER_NODE) {
        for (int i = head_->num_elements - 1; i >= 0; i--) {
            head_->value[i + 1] = head_->value[i];  // shift array to the left to exclude the first element
        }
        head_->value[0] = new_value;
        head_->num_elements += 1;
    } else {
        Node<T> *new_node = new Node<T>;
        // std::cout << new_node -> num_elements;
        new_node->value[new_node->num_elements] = new_value;
        new_node->num_elements += 1;
        new_node->next_ = head_;
        head_->prev_ = new_node;
        head_ = new_node;
    }
    size_ += 1;
}


template<class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(UnrolledLL<T>::iterator itr) {
    assert(size_ > 0);
    iterator result(itr.ptr_);
    result++;  // erase return the value following the one thats been erased. increment result is a MUST

    // erase the node
    if (itr.ptr_->num_elements - 1 == 0) {

        if (itr.ptr_ == head_ && itr.ptr_ == tail_) {  // the only node in linked list,
            delete itr.ptr_;
            head_ = tail_ = NULL;
        } else if (itr.ptr_ == head_) {  // iterator erase the first node
            Node<T> *temp = head_->next_;
            head_->next_->prev_ = NULL;
            delete head_;
            head_ = temp;
        } else if (itr.ptr_ == tail_) {  // iterator erase the last node
            Node<T> *temp = tail_->prev_;
            tail_->prev_->next_ = NULL;
            delete tail_;
            tail_ = temp;
        } else {  // erase node in the middle
            itr.ptr_->prev_->next_ = itr.ptr_->next_;
            itr.ptr_->next_->prev_ = itr.ptr_->prev_;
            delete itr.ptr_;
        }
    }

        // not erasing the node
    else {
        for (int i = itr.offset; i < itr.ptr_->num_elements; i++) {
            itr.ptr_->value[i] = itr.ptr_->value[i + 1];
        }
        itr.ptr_->num_elements -= 1;
    }

    size_ -= 1;


    return result;
}


template<class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(UnrolledLL<T>::iterator itr, const T &new_value) {

    iterator result(itr.ptr_);

    // create a new node if the current one is full
    if (itr.ptr_->num_elements == MAX_NUM_ELEMENTS_PER_NODE) {
        int counter = 0; // remember to initilizie at the end

        Node<T> *new_node = new Node<T>;

        // iterate through the array where overflow occours and put those overflowed data to a new node
        // start from offset, as its inserting infront
        for (int i = itr.offset; i < itr.ptr_->num_elements; i++) {
            // std::cout << itr.ptr_ ->value[i] << std::endl;
            new_node->value[counter] = itr.ptr_->value[i];
            counter += 1;
            new_node->num_elements += 1;
        }


        // num elements = offset + 1

        // counter = 0;
        itr.ptr_->value[itr.offset] = new_value;
        itr.ptr_->num_elements = itr.offset + 1;

        new_node->next_ = itr.ptr_->next_;
        new_node->prev_ = itr.ptr_;
        itr.ptr_->next_->prev_ = new_node;
        itr.ptr_->next_ = new_node;

        // for (int i = 0; i < itr.offset + 1; i++) {
        //     std::cout << "remaingin " <<  itr.ptr_ -> value[i] << std::endl;
        // }
    } else {
        for (int i = itr.ptr_->num_elements; i > itr.offset; i--) {
            itr.ptr_->value[i] = itr.ptr_->value[i - 1];  // making space for the new index by shifting array to right
        }

        itr.ptr_->value[itr.offset] = new_value;
        itr.ptr_->num_elements += 1;
    }

    // set the result pointer equls to itr pointer so that the function returns the right location
    result.offset = itr.offset;
    size_ += 1;
    return result;

}


template<class T>
void UnrolledLL<T>::copy_list(const UnrolledLL<T> &old) {
    size_ = old.size_;
    // Handle the special case of an empty list.
    if (size_ == 0) {
        head_ = tail_ = NULL;
        return;
    }

    // Create a new head node.
    head_ = new Node<T>(old.head_->value_);
    // tail_ will point to the last node created and therefore will move
    // down the new list as it is built
    tail_ = head_;
    // old_p will point to the next node to be copied in the old list
    Node<T> *old_p = old.head_->next_;
    // copy the remainder of the old list, one node at a time
    while (old_p) {
        tail_->next_ = new Node<T>(old_p->value_);
        tail_->next_->prev_ = tail_;
        tail_ = tail_->next_;
        old_p = old_p->next_;
    }
}

template<class T>
UnrolledLL<T> &UnrolledLL<T>::operator=(const UnrolledLL<T> &old) {
    // check for self-assignment
    if (&old != this) {
        YOU_SHALL_NOT_PASS();
        copy_list(old);
    }
    return *this;
}


template<class T>
void UnrolledLL<T>::print(std::ostream &os) {
    os << "UnrolledLL, size: " << size_ << std::endl;
    for (Node<T> *p = head_; p != NULL; p = p->next_) {
        os << "node:[" << p->num_elements << "] ";
        for (int i = 0; i < p->num_elements; i++) {
            os << p->value[i] << " ";
        }
        os << std::endl;
    }
}


#endif //HW5_UNROLLED_H
