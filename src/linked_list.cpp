#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  // Tip 1: создайте узел в куче со переданным значением
  // Tip 2: есть 2 случая - список пустой и непустой
  // Tip 3: не забудьте обновить поля head и tail
  // напишите свой код здесь ...
	Node *node = new Node(e, nullptr);

	if (size_==0){
		head_=node;
		tail_=node;
	} else{
		tail_->next = node;
		tail_ = node;
	}
	size_++;

}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);

  // Tip 1: вставка элементов на позицию size эквивалентно операции добавления в конец
  // Tip 2: рассмотрите несколько случаев:
  //        (1) список пустой,
  //        (2) добавляем в начало списка,
  //        (3) добавляем в конец списка
  //        (4) все остальное

  // напишите свой код здесь ...
	if (index==size_){
		Add(e);
	} else if (index==0){
//		Node *wasHead = new  Node(head_->data,head_->next);
		Node* newHead = new Node(e,head_);
		head_ = newHead;
//		head_->data = e;
//		head_->next = wasHead;
		size_++;
	} else {
		Node *preNode = head_;
		for (int i = 0; i < index - 1; i++) {
			preNode = preNode->next;
		}
		Node *newNode = new Node(e, preNode->next);
		preNode->next = newNode;
		size_++;
	}


}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: используйте функцию find_node(index)
  // напишите свой код здесь ...
  Node* oldNode =find_node(index);
  oldNode->data=e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
  // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
  // Tip 2: используйте функцию find_node(index)
  // напишите свой код здесь ...
	if (index == 0){
		Node* newHead = head_->next;
//		Node* headWas = new Node(head_->data,head_->next);
		Element toReturn = head_->data;
		head_ = newHead;
		size_--;
		return toReturn;
	}

    Node* preRemoveNode = head_;
	for (int i = 0; i < index - 1; ++i) {
		preRemoveNode = preRemoveNode->next;
	}
	Node* afterNode = preRemoveNode->next->next;
//	Node* toReturn = new Node(preRemoveNode->next->data,preRemoveNode->next->next);
	Element toReturn = preRemoveNode->next->data;
	preRemoveNode->next = afterNode;
	size_--;
  return toReturn;
}

void LinkedList::Clear() {
  // Tip 1: люди в черном (MIB) пришли стереть вам память
  //все:
  //я: отворачиваюсь
  // напишите свой код здесь ...
  Node* oldNode = head_;
	for (int i = 0; i < size_; ++i) {
		Node* nextNode = oldNode->next;
		delete oldNode;
		oldNode = nextNode;
	}
	delete oldNode;
	head_= nullptr;
	tail_= nullptr;
	size_=0;
}

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  // напишите свой код здесь ...
  Node* toReturn = head_;
	for (int i = 0; i < index; ++i) {
		toReturn = toReturn->next;
	}
  return toReturn->data;
}

int LinkedList::IndexOf(Element e) const {
  // напишите свой код здесь ...
   Node* searchElement = head_;
	for (int i = 0; i < size_; ++i) {
		if (searchElement->data == e){
			return i;
		} else if (i!=size_-1){
			searchElement = searchElement->next;
		}
	}
  return kNotFoundElementIndex;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
  // Tip 1: можете сразу обработать случаи поиска начала и конца списка
  // напишите свой код здесь ...
  Node* searchNode = head_;
	for (int i = 0; i < index; ++i) {
		searchNode = searchNode->next;
	}
  return searchNode;
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis