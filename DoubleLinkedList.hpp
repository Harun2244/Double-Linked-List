#pragma once

#include <functional>
#include <iterator>
#include <ostream>

template <typename T>
struct Node {
  Node* prev{nullptr};
  Node* next{nullptr};
  T data;
  Node(const T& element) : data{element}, next{nullptr}, prev{nullptr} {}
};

template <typename T>
class DoubleLinkedList {
  public:
class Iterator;
  DoubleLinkedList() : head_{nullptr}, tail_{nullptr}, size_{0} {}

  DoubleLinkedList(const DoubleLinkedList& other)
      : head_(nullptr), tail_(nullptr), size_(0) {
    auto temp = other.head_;
    while (temp != nullptr) {
      push_back(temp->data);
      temp = temp->next;
    }
  }

  DoubleLinkedList(DoubleLinkedList&& other)
      : head_{other.head_}, tail_{other.tail_}, size_{other.size_} {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  DoubleLinkedList& operator=(const DoubleLinkedList& other) {
    if (this != &other) {
      clear();
      auto temp = other.head_;
      while (temp != nullptr) {
        push_back(temp->data);
        temp = temp->next;
      }
    }
    return *this;
  }

  DoubleLinkedList& operator=(DoubleLinkedList&& other) {
    clear();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  ~DoubleLinkedList() { clear(); }

  DoubleLinkedList& push_back(const T& element) {
    if (head_ == nullptr) {
      head_ = new Node<T>{element};
      tail_ = head_;
    } else {
      auto temp = new Node<T>{element};
      tail_->next = temp;
      temp->prev = tail_;
      tail_ = temp;
    }
    ++size_;
    return *this;
  }

  DoubleLinkedList& push_front(const T& element) {
    if (size_ == 0) {
      push_back(element);
    } else {
      auto temp = new Node<T>{element};
      temp->prev = nullptr;
      temp->next = head_;
      head_->prev = temp;
      head_ = temp;
      ++size_;
    }
    return *this;
  }

  DoubleLinkedList& push_back(T&& elem) {
    if (head_ == nullptr) {
      head_ = new Node<T>{elem};
      tail_ = head_;
    } else {
      auto temp = new Node<T>{elem};
      tail_->next = temp;
      temp->prev = tail_;
      tail_ = temp;
    }
    size_++;
  }

  DoubleLinkedList& push_front(T&& element) {
    if (size_ == 0) {
      push_back(element);
    } else {
      auto temp = new Node<T>{element};
      temp->prev = nullptr;
      temp->next = head_;
      head_->prev = temp;
      head_ = temp;
      ++size_;
    }
    return *this;
  }

  DoubleLinkedList& pop_front() {
    if (size_ == 0) {
      throw std::out_of_range{"Prazna lista"};
    } else if (size_ == 1) {
      clear();
    } else {
      auto temp = head_;
      head_ = head_->next;
      head_->prev = nullptr;
      delete temp;
      --size_;
      return *this;
    }
  }

  DoubleLinkedList& pop_back() {
    if (size_ == 0) {
      throw std::out_of_range{"Prazna lista"};
    } else if (size_ == 1) {
      clear();
    } else {
      auto temp = tail_;
      tail_ = tail_->prev;
      tail_->next = nullptr;
      delete temp;
      --size_;
      return *this;
    }
  }

  bool empty() const { return size_ == 0; }

  size_t size() const { return size_; }

  T& front() {
    if (size_ == 0) {
      throw std::out_of_range{"Prazna lista!!"};
    }
    return head_->data;
  }

  T& back() {
    if (size_ == 0) {
      throw std::out_of_range{"Prazna lista!!"};
    }
    return tail_->data;
  }




class Iterator
    : public std::iterator<std::bidirectional_iterator_tag, T> {
  public:

     using difference_type = std::ptrdiff_t;
  Iterator() : ptr_{nullptr} {}

  Iterator(Node<T>* p) : ptr_{p} {}

  Iterator(Iterator&& other) : ptr_{other.ptr_} { other.ptr_ = nullptr; }

  Iterator& operator=(const Iterator& other) {
    ptr_ = other.ptr_;
    return *this;
  }

  Iterator& operator=(Iterator&& other) {
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
  }

  Node<T>* node() const { return ptr_; }

  Iterator(const Iterator& other) : ptr_{other.ptr_} {}

  Iterator& operator++() {
    if (ptr_ != nullptr) ptr_ = ptr_->next;
    return *this;
  }

  T operator*() const { return ptr_->data; }

  Iterator operator++(int) {
    auto temp = Iterator{ptr_};
    if (ptr_ != nullptr) {
      ptr_ = ptr_->next;
      return temp;
    }
  }

  Iterator& operator--() {
    ptr_ = ptr_->prev;
    return *this;
  }

  Iterator operator--(int) {
    auto temp = Iterator{ptr_};

    ptr_ = ptr_->prev;
    return temp;
  }
  //
  // bool operator <(const Iterator& other){
  // return ptr_ -> data < other.ptr_ ->data;
  // }
  //
  // difference_type operator-(const Iterator& other) const {
  //   return std::distance(other.ptr_, ptr_);
  // }

  T* operator->() const { return &(ptr_->data); }

  bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }

  bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }

  private:
  Node<T>* ptr_;
};

  Iterator begin() const { return Iterator{head_}; }

  Iterator end() const { return Iterator{nullptr}; }

  const Iterator cbegin() const { return Iterator{begin()}; }

  const Iterator cend() const { return Iterator{end()}; }

  Iterator rbegin() const { return Iterator{tail_}; }

  Iterator rend() const {return Iterator{head_};}

  const Iterator rcbegin() const {
    if(size_ == 0){return Iterator{nullptr};}
    return Iterator{tail_};}

  const Iterator rcend() const {return Iterator{nullptr};}
 
 Iterator insert(Iterator pos, const T& element) {
   if (pos == begin()) {
    push_front(element);
    return begin();
  }
  else if (pos == end()) {
    push_back(element);
    return --end();
  }
  else {
    auto temp = new Node<T>{element};
    temp->prev = pos.node()->prev;
    temp->next = pos.node();
    pos.node()->prev->next = temp;
    pos.node()->prev = temp;
    ++size_;
    return Iterator{temp};
  }
 }


  void clear() {
    while (head_ != nullptr) {
      auto temp = head_->next;
      delete head_;
      head_ = temp;
    }
    tail_ = nullptr;
    size_ = 0;
  }

   
  Iterator erase(Iterator pos) {
if(pos.node()==head_){
auto temp = pos.node();
temp -> next -> prev = nullptr;
++pos;
head_ = temp->next;
temp -> next = nullptr;
delete temp;
size_--;
return Iterator{pos.node()};

}

   else  if(pos.node() == tail_){
    auto temp = pos.node();
    temp -> prev -> next = nullptr;
    --pos;
    tail_ = temp -> prev;
    temp->prev = nullptr;
    delete temp;
    size_ --;
    return Iterator{pos.node()};
    }
else{
  auto temp = pos.node();
  temp -> prev -> next = temp -> next;
  temp -> next -> prev = temp -> prev;
  ++pos;
  temp -> prev = nullptr;
  temp -> next = nullptr;
  delete temp;
  --size_;
  return Iterator{pos.node()};}
  }

  Iterator erase(Iterator beginIt, Iterator endIt) {
  while(beginIt != endIt){
    auto temp = beginIt.node();
    ++beginIt;
  temp -> prev -> next = temp -> next;
  temp -> next -> prev = temp -> prev;
  temp -> prev = nullptr;
  temp -> next = nullptr;
  delete temp;
  size_--;
  }
  return Iterator{endIt.node()};
  }

  void remove_if(std::function<bool(const T&)>&& p) {
  auto it1 = begin();
  while(it1 != end()){
    auto temp = it1;
  if(p(temp.node() -> data)){
    erase(temp);
    size_--;
  }
  ++it1;
  }}

  void reverse() {
  auto it1 = begin();
  auto it2 = rcbegin();
  if(size_ % 2 == 0){
  size_t i = size_ / 2;
  size_t k = 0;
  while(k<i){
    std::swap(it1.node() -> data, it2.node() -> data);
    ++it1;
    --it2;
    ++k;
  }}
  else{
  while(it1 != it2){
     std::swap(it1.node() -> data, it2.node() -> data);
    ++it1;
    --it2; }}
  }

  Iterator find(const T& element) const {
  auto it1 = begin();
  auto it2 = cend();
  while(it1 != end()){
   if(it1.node() -> data == element){
   return Iterator{it1.node()};
   }
++it1;
  }
  return Iterator{it2.node()};
  }

  private:
  void dealoc();
  Node<T>* head_{nullptr};
  Node<T>* tail_{nullptr};
  size_t size_{0};
};


//
// template <typename T>
// class DoubleLinkedList<T>::Iterator
//     : std::iterator<std::bidirectional_iterator_tag, T> {
//   public:
//   Iterator() : ptr_{nullptr} {}
//
//   Iterator(Node<T>* p) : ptr_{p} {}
//
//   Iterator(Iterator&& other) : ptr_{other.ptr_} { other.ptr_ = nullptr; }
//
//   Iterator& operator=(const Iterator& other) {
//     ptr_ = other.ptr_;
//     return *this;
//   }
//
//   Iterator& operator=(Iterator&& other) {
//     ptr_ = other.ptr_;
//     other.ptr_ = nullptr;
//     return *this;
//   }
//
//   Node<T>* node() const { return ptr_; }
//
//   Iterator(const Iterator& other) : ptr_{other.ptr_} {}
//
//   Iterator& operator++() {
//     if (ptr_ != nullptr) ptr_ = ptr_->next;
//     return *this;
//   }
//
//   T operator*() const { return ptr_->data; }
//
//   Iterator operator++(int) {
//     auto temp = Iterator{ptr_};
//     if (ptr_ != nullptr) {
//       ptr_ = ptr_->next;
//       return temp;
//     }
//   }
//
//   Iterator& operator--() {
//     ptr_ = ptr_->prev;
//     return *this;
//   }
//
//   Iterator operator--(int) {
//     auto temp = Iterator{ptr_};
//
//     ptr_ = ptr_->prev;
//     return temp;
//   }
//
//   T* operator->() const { return &(ptr_->data); }
//
//   bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
//
//   bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
//
//   private:
//   Node<T>* ptr_;
// };
