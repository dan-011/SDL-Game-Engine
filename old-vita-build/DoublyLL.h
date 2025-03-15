#pragma once
#include <unordered_map>

// create instant access (LRU cache)
template<typename T>
struct LLNode {
public:
	LLNode(T value, LLNode<T>* _prev = NULL, LLNode<T>* _next = NULL) : val(value), prev(_prev), next(_next) {}
	~LLNode() {}

	T val;
	LLNode<T>* next;
	LLNode<T>* prev;
};

template<typename T>
class DoublyLL {
public:
	DoublyLL() : length(0), head(NULL), tail(NULL) {}
	~DoublyLL() {
		while (head) PopBack(); // we're allowed to do this because deleting the LLNode doesn't delete it's contents (we want this)
	}

	int Size() {
		return length;
	}
	T GetHead() {
		if (length > 0) return head->val;
		else throw "Error";
	}
	T GetTail() {
		if (length > 0) return tail->val;
		else throw "Error";
	}
	void PushFront(T val) {
		if (length) {
			LLNode<T>* node = new LLNode<T>(val);
			head->prev = node;
			node->next = head;
			head = node;
			mapping.insert({ val, node });
		}
		else {
			head = new LLNode<T>(val);
			tail = head;
			mapping.insert({ val, head });
		}
		length++;
	}
	void PushBack(T val) {
		if (length) {
			LLNode<T>* node = new LLNode<T>(val);
			tail->next = node;
			node->prev = tail;
			tail = node;
			length++;
			mapping.insert({ val, node });
		}
		else PushFront(val);
	}
	void PopFront() {
		if (length == 1) {
			LLNode<T>* toDel = head;
			mapping.erase(toDel->val);
			head = NULL;
			tail = NULL;
			delete toDel;
			length--;
		}
		else if (length > 1) {
			LLNode<T>* toDel = head;
			mapping.erase(toDel->val);
			head = head->next;
			head->prev = NULL;
			delete toDel;
			length--;
		}
	}
	void PopBack() {
		if (length == 1) PopFront();
		else if (length > 1) {
			LLNode<T>* toDel = tail;
			mapping.erase(toDel->val);
			tail = tail->prev;
			tail->next = NULL;
			delete toDel;
			length--;
		}
	}
	void Erase(T value) {
		if (Contains(value)) {
			LLNode<T>* node = mapping[value];
			if (node == head) PopFront();
			else if (node == tail) PopBack();
			else {
				LLNode<T>* prev = node->prev;
				LLNode<T>* next = node->next;
				prev->next = next;
				next->prev = prev;
				length--;
				delete node;
				mapping.erase(value);
			}
		}
	}

	bool Contains(T value) {
		return mapping.find(value) != mapping.end();
	}

	// Nested iterator class 
	class Iterator { 
	public: 
		Iterator(LLNode<T>* node) : current(node) {}
		bool operator!=(const Iterator& other) const { 
			return current != other.current; 
		} 
		T& operator*() { 
			return current->val;
		} 
		Iterator& operator++() { 
			current = current->next;
			return *this;
		} 

	private:
		LLNode<T>* current;
	};

	Iterator begin() {
		return Iterator(head);
	}
	Iterator end() {
		return Iterator(NULL);
	}

	bool Empty() {
		return !length;
	}

private:
	std::unordered_map<T, LLNode<T>*> mapping;
	int length;
	LLNode<T>* head;
	LLNode<T>* tail;
};