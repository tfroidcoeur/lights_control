/*
 * List.h
 *
 *  Created on: Feb 20, 2018
 *      Author: fraco
 */
#include <stddef.h>

#ifndef LIST_H_
#define LIST_H_

template<class T>
class List;

/* doubly linked list implementation */

/* separate node object */
template<class T>
class ListNode {
	friend List<T> ;
public:
	ListNode() :
			next(this), prev(this), item(NULL) {
	}
	;
	ListNode(const T * item) :
			next(NULL), prev(NULL), item(item) {
	}
	ListNode(const T & item) :
			next(NULL), prev(NULL), item(&item) {
	}
	T & get() {
		return *item;
	}
	;
private:
	ListNode * next;
	ListNode * prev;
	const T * item;
};

/* list class */
template<class T>
class List {
public:
	class ListIterator;
	typedef ListIterator iterator;
	List() {
	}
	virtual ~List() {
	}

	/* add object into the list */
	void add(const T* something) {
		ListNode<T> * t = new ListNode<T>(something);
		insertBefore(*t, sentinel);
	}

	void add(const T&something) {
		add(&something);
	}
	/* find object */
	ListNode<T> * find(const T* something) {
		ListNode<T> * cur = sentinel.next;
		for (; cur->item != something && cur != &sentinel; cur = cur->next)
			;

		if (cur->item == something)
			return cur;
		return NULL;
	}

	ListNode<T> * find(const T& something) {
		return find(&something);
	}

	/* remove node */
	void remove(ListNode<T> * it) {
		it->next->prev = it->prev;
		it->prev->next = it->next;

		delete it;
	}

	/* find and remove object*/
	bool remove(const T* something) {
		ListNode<T> * it = find(something);
		if (!it)
			return false;
		remove(it);
		return true;

	}
	bool remove(const T& something) {
		remove(&something);
	}

	bool empty() {
		return sentinel.next == &sentinel;
	}

	iterator begin() {
		return iterator(this, sentinel.next);
	}
	iterator end() {
		return iterator(this, &sentinel);
	}

	/* iterator for this list */
	class ListIterator {
	public:
		ListIterator(List<T> * l, ListNode<T> * cur) :
				cur(cur), parent(l) {
		}
		bool operator ==(const ListIterator & other) {
			return other.parent == this->parent && other.cur == this->cur;
		}
		bool operator !=(const ListIterator & other) {
			return !(*this == other);
		}
		T& operator *() {
			return (T&)*cur->item;
		}
		T* operator ->() {
			return cur->item;
		}
		iterator operator++() {
			cur = cur->next;
			return *this;
		}
		;
		iterator operator++(int n) {
			iterator t(*this);
			++(*this);
			return t;
		}
	private:
		ListNode<T> * cur;
		List<T> * parent;
	};

private:
	ListNode<T> sentinel;
	void insertBefore(ListNode<T> & n, ListNode<T> & where) {
		n.next = &where;
		n.prev = where.prev;
		where.prev = &n;
		n.prev->next = &n;
	}
};

#endif /* LIST_H_ */
