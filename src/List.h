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

template<class T>
class ListItem {
	friend List<T> ;
public:
	ListItem(T & item) :
			next(NULL), prev(NULL), item(item) {
	}
	T & get() {
		return item;
	}
	;
private:
	ListItem * next;
	ListItem * prev;
	T & item;
};

template<class T>
class List {
public:
	List() :
			e(NULL) {
	}
	virtual ~List() {
	}
	;
	void add(T& something) {
		ListItem<T> * t = new ListItem<T>(something);
		if (!e) {
			e = t;
			e->next = e;
			e->prev = e;
		} else {
			insertBefore(*t, *e);
		}

	}
	ListItem<T> * find(T& something) {
		if (!e)
			return NULL;

		ListItem<T> * cur = e->next;
		for (; &cur->item != &something && cur != e; cur = cur->next)
			;

		if (&cur->item == &something)
			return cur;
		return NULL;
	}
	void remove(ListItem<T> * it) {
		if (e==it) e=it->next;
		if (e==it) e=NULL;
		else {
			it->next->prev = it->prev;
			it->prev->next = it->next;
		}

		delete it;
	}

	bool remove(T& something) {
		ListItem<T> * it = find(something);
		if (!it)
			return false;
		remove(it);
		return true;

	}

	class Iterator {
	public:
		Iterator() {
			nextitem = List::e;
		}
		ListItem<T> * next() {
			if (!nextitem)
				return NULL;
			nextitem = nextitem->next;
			return nextitem;
		}
	private:
		ListItem<T> * nextitem;
	};

private:
	ListItem<T> * e;
	void insertBefore(ListItem<T> & n, ListItem<T> & where) {
		n.next = &where;
		n.prev = where.prev;
		where.prev = &n;
		n.prev->next = &n;
	}

};

#endif /* LIST_H_ */
