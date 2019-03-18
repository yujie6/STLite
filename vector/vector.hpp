#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {

private:
    T * v;
    int max_size;
    int current_size;
    void doublespace(){
        max_size = 2 * max_size;
        T * v1 = (T*)malloc(sizeof(T) * max_size);
        for (auto i = 0; i < current_size; i++) new(&v1[i])T(v[i]); //using placement new
        clear();
        free(v);
        v = v1;
    };
public:

	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
	    int pos;
        vector * origin;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator(int p, vector * o){
		    pos = p; origin = o;
		}

		~iterator(){
		    //delete origin;
		}
		iterator operator+(const int &n) const {
		    iterator ans(pos + n, origin);
		    return ans;
		}
		iterator operator-(const int &n) const {
            iterator ans;
            ans.pos = pos - n;
            ans.origin = origin;
            return ans;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
		    return (pos - rhs.pos);
		}
		iterator operator+=(const int &n) {
		    pos += n;
		    return *this;
		}
		iterator operator-=(const int &n) {
		    pos -= n;
		    return *this;
		}
		iterator operator++(int) {
		    iterator ans = *this;
		    pos++;
		    return ans;
		}
		iterator& operator++() {
		    pos++;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
		    iterator ans = *this;
		    pos--;
		    return ans;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
		    pos--;
		    return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
		    return (*origin)[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
		    return (pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const {
            return (pos == rhs.pos);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    return (pos != rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const {
            return (pos != rhs.pos);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {

    private:
        int pos;
        vector * origin;
    public:

        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, just return the answer.
         * as well as operator-
         */
        const_iterator(int p, vector * o){
            pos = p; origin = new vector(*o);
        }
        ~const_iterator(){
            delete origin;
        }
        const_iterator operator+(const int &n) const {
            const_iterator ans;
            ans.pos = pos + n;
            ans.origin = origin;
            return ans;
        }
        const_iterator operator-(const int &n) const {
            iterator ans;
            ans.pos = pos - n;
            ans.origin = origin;
            return ans;
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            return (pos - rhs.pos);
        }
        const_iterator operator+=(const int &n) {
            pos += n;
            return *this;
        }
        const_iterator operator-=(const int &n) {
            pos -= n;
            return *this;
        }
        const_iterator operator++(int) {
            iterator ans = *this;
            pos++;
            return ans;
        }
        const_iterator& operator++() {
            pos++;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            iterator ans = *this;
            pos--;
            return ans;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            pos--;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const{
            return (origin->v)[pos];
            //return ans;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return (pos == rhs.pos);
        }
        bool operator==(const const_iterator &rhs) const {
            return (pos == rhs.pos);
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return (pos != rhs.pos);
        }
        bool operator!=(const const_iterator &rhs) const {
            return (pos != rhs.pos);
        }
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector() {
	    current_size = 0;
	    max_size = 5;

	    v = (T*)malloc(sizeof(T)*max_size);

	}
	vector(const vector &other) {
	    max_size = other.max_size;
        v = (T*)malloc(sizeof(T)*max_size);
	    for (int i = 0; i < other.current_size; i++){
	        v[i] = other[i];
	    }
	    current_size = other.current_size;
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
        clear();
	    free(v);
	    v = NULL;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
	    max_size = other.max_size;
	    clear();
	    free(v);
	    v = (T*)malloc(sizeof(T)*max_size);
        for (int i = 0; i < other.current_size; i++){
            v[i] = other[i];
        }
        current_size = other.current_size;
	    return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
        if (pos >= 0 && pos < current_size) return v[pos];
        else {
            throw(index_out_of_bound());
        }
	}
	const T & at(const size_t &pos) const {
        if (pos >= 0 && pos < current_size) return v[pos];
        else {
            throw(index_out_of_bound());
        }
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
        if (pos >= 0 && pos < current_size) return v[pos];
        else {
            throw(index_out_of_bound());
        }
	}
	const T & operator[](const size_t &pos) const {
	    if (pos >= 0 && pos < current_size) return v[pos];
	    else {
	        throw(index_out_of_bound());
	    }
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    if (current_size == 0) {
	        throw(container_is_empty());
	    }
	    else {
	        return v[0];
	    }
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
        if (current_size == 0) {
            throw(container_is_empty());
        }
        else {
            return v[current_size-1];
        }
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    //static vector<T> nn = *this;
	    iterator ans(0, this);
	    return ans;
	}
	const_iterator cbegin() const {
        static vector<T> nn = *this;
        const_iterator ans(0, &nn);
        return ans;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        //static vector<T> nn = *this;
	    iterator ans(current_size, this);
	    return ans;
	}
	const_iterator cend() const {
	    static vector<T> nn = *this;
        const_iterator ans(current_size, &nn);
        return ans;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    return (current_size == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return size_t(current_size);
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {
	    return size_t(max_size);
	}
	/**
	 * clears the contents
	 */
	void clear() {
        for (int i = 0; i < current_size; i++) {
            v[i].~T();
        }
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator it, const T &value) {
	    size_t ind = 0;
	    while(it != begin()){
	        ind++; it--;
	    }
	    return insert(ind, value);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
	    if (ind > current_size) throw(index_out_of_bound());

	    if(current_size == max_size-1) doublespace();
        current_size++;
        for (int i = current_size-1; i > ind; i--) {
            this->at(i) = this->at(i - 1);
        }
        this->at(ind) = value;
        iterator it = begin() + ind;
        return it;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator it) {
        size_t ind = 0;
        while(it != begin()){
            ind++;
            it--;
        }
        return erase(ind);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
	    if (ind >= current_size) throw (index_out_of_bound());
	    for (int i = ind; i < current_size-1; i++){
	        this->at(i) = this->at(i+1);
	    }
        current_size--;
	    iterator ans = begin() + ind;
	    return ans;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    if (current_size >= max_size - 1) doublespace();
        if (current_size < max_size) new(&v[current_size])T(value); //placement new
        current_size++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
	    if (current_size == 0){
	        throw(container_is_empty());
	    }
	    else{
	        //how to delete
	        current_size--;
	    }
	}

};


}

#endif
