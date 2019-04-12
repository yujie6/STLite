#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <deque>
#include <cstddef>

namespace sjtu { 

template<class T>
class deque {
private:

    class Block{
        friend class iterator;
        friend class const_iterator;
    public:
        T * data;
        size_t size;
        Block * next, * prev;
        int MaxBlockSize;
        Block(){
            MaxBlockSize = 1000;
            data = (T*)operator new(sizeof(T) * MaxBlockSize);
            size = 0;
        }
        Block(const Block * other){
            MaxBlockSize = 1000;
            size = other->size;
            data = (T*)operator new(sizeof(T) * MaxBlockSize);
            for (int i = 0; i < size; i++){
                new(&data[i])T(other->data[i]);
            }
        }
        Block(Block * n, Block *p):next(n),prev(p){
            MaxBlockSize = 1000;
            data = (T*)operator new(sizeof(T) * MaxBlockSize);
            size = 0;
        }
        ~Block(){
            for (int i = 0; i < size; i++) {
                data[i].~T();
            }
            operator delete(data);
        }

    };
    Block * Head;
    Block * Tail;
    int CurrentLen;
    int MaxBlockSize;
public:
	class const_iterator;
	class iterator {
	    friend deque;
	    friend const_iterator;
	private:
		Block * origin;
		Block * Tail;
        int pos;

		iterator More(const int &n){
            Block * p = origin; int count = 0; int delta;
            while(count < n){
                if (p == origin){ delta = (p->size - pos);}
                else delta = p->size;

                if (count + delta <= n){
                    p = p->next; count += delta;
                    if (count == n){
                        count = 0; break;
                    }
                }
                else {
                    if (p == origin) count = pos+(n-count);
                    else count = n - count;
                    break;
                }
            }
            return iterator(p,Tail, count);
		}
		iterator Less(const int &n){
            Block * p = origin; int count = n;
            while(count > 0){
                int delta;
                if (p == origin) {delta = pos;}
                else delta = p->size;
                if (count - delta > 0){
                    p = p->prev; count -= delta;
                }
                else {
                    count = delta - count; break;
                }
            }
            return iterator(p,Tail, count);
		}
	public:
	    iterator(){}
	    iterator(Block * ori, Block * t, int po): origin(ori),Tail(t), pos(po) {}
	    iterator(const iterator &other){origin = other.origin; pos = other.pos;}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n)  {
			if (n >= 0) return More(n);
			else {return Less(n);}
		}
		iterator operator-(const int &n) const {
			if (n <= 0) return More(-n);
			else {return More(n);}
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
		}
		iterator operator+=(const int &n) {
		    *this = *this + n;
		    return *this;
		}
		iterator operator-=(const int &n) {
			*this = *this - n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator ans(*this);
		    return ans;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
		    if (origin == Tail && pos == Tail->size-1){
		        *this = iterator(Tail, Tail, Tail->size);
		        return *this;
		    }
		    *this = More(1);
            return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator ans(*this); *this = Less(1);
            return ans;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
		    *this = Less(1);
            return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
		    return origin->data[pos];
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {
		    T * ans = &origin->data[pos];
		    return ans;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
		    return ((origin == rhs.origin) && (pos == rhs.pos) );
		}
		bool operator==(const const_iterator &rhs) const {
            return ((origin == rhs.origin) && (pos == rhs.pos) );
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return !((origin == rhs.origin) && (pos == rhs.pos) );
		}
		bool operator!=(const const_iterator &rhs) const {
            return !((origin == rhs.origin) && (pos == rhs.pos) );
		}
	};
	class const_iterator {
		private:
			Block * origin;
			int pos;
			Block * Tail;
            const_iterator More(const int &n){
                Block * p = origin; int count = 0; int delta;
                while(count < n){
                    if (p == origin){ delta = (p->size - pos);}
                    else delta = p->size;

                    if (count + delta <= n){
                        p = p->next; count += delta;
                        if (count == n){
                            count = 0; break;
                        }
                    }
                    else {
                        if (p == origin) count = pos+(n-count);
                        else count = n - count;
                        break;
                    }
                }
                return const_iterator(p,Tail, count);
            }
            const_iterator Less(const int &n){
                Block * p = origin; int count = n;
                while(count > 0){
                    int delta;
                    if (p == origin) {delta = pos;}
                    else delta = p->size;
                    if (count - delta > 0){
                        p = p->prev; count -= delta;
                    }
                    else {
                        count = delta - count; break;
                    }
                }
                return const_iterator(p, Tail, count);
            }
		public:
			const_iterator() {
				// TODO
			}
			const_iterator(Block * ori, Block * t, int p):origin(ori), Tail(t), pos(p){}
			const_iterator(const const_iterator &other) {
				origin = other.origin; pos = other.pos;
			}
			const_iterator(const iterator &other) {
				origin = other.origin; pos = other.pos;
			}
            iterator operator+(const int &n) const {
                if (n >= 0) return More(n);
                else {return Less(-n);}
            }
            iterator operator-(const int &n) const {
                if (n <= 0) return More(-n);
                else {return More(n);}
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
            }
            iterator operator+=(const int &n) {
                *this = *this + n;
                return *this;
            }
            iterator operator-=(const int &n) {
                *this = *this - n;
                return *this;
            }
            iterator operator++(int) {
                iterator ans(*this); *this = More(1);
                return ans;
            }
            iterator& operator++() {
                if (origin == Tail && pos == Tail->size-1) {
                    *this = iterator(Tail, Tail, Tail->size);
                }
                *this = More(1);
                return *this;
            }
            iterator operator--(int) {
                iterator ans(*this); *this = Less(1);
                return ans;
            }
            iterator& operator--() {
                *this = Less(1);
                return *this;
            }
            T& operator*() const {
                return origin->data[pos];
            }
            T* operator->() const noexcept {
                T * ans = &origin->data[pos];
                return ans;
            }
            bool operator==(const iterator &rhs) const {
                return ((origin == rhs.origin) && (pos == rhs.pos) );
            }
            bool operator==(const const_iterator &rhs) const {
                return ((origin == rhs.origin) && (pos == rhs.pos) );
            }
            bool operator!=(const iterator &rhs) const {
                return !((origin == rhs.origin) && (pos == rhs.pos) );
            }
            bool operator!=(const const_iterator &rhs) const {
                return !((origin == rhs.origin) && (pos == rhs.pos) );
            }
	};
	/**
	 * TODO Constructors
	 */
	deque() {
	    MaxBlockSize = 1000; CurrentLen = 0;
	    Head = Tail = new Block();
	    Head->next = Tail; Tail->prev = Head;
	}
	deque(const deque &other) {
        MaxBlockSize = 1000; CurrentLen = other.CurrentLen;
        Head = new Block(other.Head);
        if (other.Head == other.Tail) {Tail = Head; Tail->prev = Head; Head->next = Tail;}
        else {
            Block * op = other.Head; Block * nextp;
            Block * p = Head;
            while (op != other.Tail){
                nextp = new Block(op->next);
                nextp->prev = p; p->next = nextp;
                op = op->next; p = p->next;
            }
            Tail = p;
        }
	}
	~deque() {
	    clear();
	    if (Head != nullptr) delete Head;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
	    if (&other == this) return *this;
	    clear(); if (Head != nullptr) delete Head;
        Head = new Block(other.Head); CurrentLen = other.CurrentLen;
        if (other.Head == other.Tail) {Tail = Head; Tail->prev = Head; Head->next = Tail;}
        else {
            Block * op = other.Head;
            Block * p = Head;
            while (op != other.Tail){
                Block * nextp = new Block(op->next);
                nextp->prev = p; p->next = nextp;
                op = op->next; p = p->next;
            }
            Tail = p;
        }
        return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
	    if (pos >= CurrentLen || pos < 0) throw(index_out_of_bound());
	    iterator cur = Findpos(pos);
	    return *cur;
	}
	const T & at(const size_t &pos) const {
        if (pos >= CurrentLen || pos < 0) throw(index_out_of_bound());
        iterator cur = Findpos(pos);
        return *cur;
	}
	T & operator[](const size_t &pos) {
        return at(pos);
	}
	const T & operator[](const size_t &pos) const {
        return at(pos);
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
	    if (CurrentLen == 0) throw(container_is_empty());
        return Head->data[0];
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
        if (CurrentLen == 0) throw(container_is_empty());
        if (Tail->size == 0) return Head->data[Head->size - 1];
        else return Tail->data[Tail->size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    return iterator(Head,Tail, 0);
	}
	const_iterator cbegin() const {
        return const_iterator(Head,Tail, 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    return iterator(Tail, Tail,Tail->size);
	}
	const_iterator cend() const {
	    return const_iterator(Tail,Tail, Tail->size);
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
	    return (CurrentLen == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return size_t(CurrentLen);
	}
	/**
	 * clears the contents
	 */
	void clear() {
        Block * p = Head;
        Block *pBlock;
        if (Head == Tail) {delete Head; Head = new Block(); return;}
        while(p != Tail){
            pBlock = p->next; delete p;
            p = pBlock;
        }
        delete Tail;
        Head = Tail = new Block();
        CurrentLen = 0;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
	    Block * p = pos.origin; int index = pos.pos;
	    if (index > p->size || index < 0) throw (invalid_iterator());
	    split(pos);
	    p->data[index] = value;
	    ++p->size;
	    ++CurrentLen;
	    merge(p);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
	    if (CurrentLen == 0) throw(container_is_empty());
	    Block * p = pos.origin; int index = pos.pos;
	    if (index >= p->size || index < 0) throw(invalid_iterator());
	    split(pos+1);
	    p->size--;
	    CurrentLen--;
	    merge(p);
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
	    if (Tail->size < MaxBlockSize){
	        new(&Tail->data[Tail->size])T(value);
	        Tail->size++;
	    }
	    else {
	        Block * p = new Block();
	        p->size = 1;
	        new(&p->data[0])T(value);
	        Tail->next = p; p->prev = Tail;
	        Tail = p;
	    }
        CurrentLen++;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
	    if (Tail->size != 1) Tail->size--;
	    else {
	        Block * p = Tail->prev;
	        delete Tail;
	        Tail = p;
	    }
	    CurrentLen--;
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
        Block * p = new Block();
        p->size = 1; p->data[0] = value;
        p->next = Head; Head->prev = p;
        Head = p;
        merge(Head);
        CurrentLen++;
    }
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
        for (int i = 1; i < Head->size; i++){
            Head->data[i-1] = Head->data[i];
        }
        Head->size--; CurrentLen--;
	}

private:
    iterator Findpos(int index){
        Block * p = Head; int pos = 0, count = 0;
        if (index == 0) return iterator(p,Tail, pos);
        while(count < index){
            if (count + p->size - 1 < index){
                count += p->size; p = p->next;
            }
            else{
                pos = index - count;
                break;
            }
        }
        return iterator(p,Tail, pos);
	}
	void merge(Block * p){
	    Block * nextp = p->next; Block * nnp = nextp->next;
        if (p->size + nextp->size < MaxBlockSize){
            for (int i = 0; i < nextp->size; i++){
                p->data[p->size+i] = nextp->data[i];
            }
            p->size += nextp->size;
            p->next = nnp;
            nnp->prev = p;
            delete nextp;
        }
	}
	void split(int pos){
        iterator cur = Findpos(pos);
        split(cur);
	}
	void split(iterator t){
	    Block * p = t.origin; int pos = t.pos;
	    Block * nextp = p->next;
	    Block * NewBlock = new Block();
	    for (int i = 0; i < p->size-pos; i++){
	        NewBlock->data[i] = p->data[i+pos];
	    }
	    NewBlock->size = p->size - pos;
	    p->size = pos;
	    nextp->prev = NewBlock; NewBlock->next = nextp;
	    p->next = NewBlock; NewBlock->prev = p;
	}

};

}

#endif
