/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
    friend class iterator;
    friend class const_iterator;
public:
    typedef pair<const Key, T> value_type;
    bool Fewer(Key a, Key b, Compare C = Compare()){
        return C(a, b);
    }
    struct node{
        node * left;
        node * right;
        //node * prev;
        //node * next;
        value_type data;
        int height;
        node(value_type element, node* l, node* r, int h=1):left(l), right(r), data(element), height
                (h){}
    };
private:
    static node * tail;
    node * root;
    int length;

public:

	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */

	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	    friend map;
	    friend const_iterator;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		node * origin;
		node * root;

	public:
        bool Fewer(Key a, Key b, Compare C = Compare()){
            return C(a, b);
        }
		iterator() {
			// TODO
		}
        explicit iterator(node * t):origin(t){}
		iterator(const iterator &other) {
			origin = other.origin;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    Key ans = origin->data->first;
		    Key Current_Key;
            node * tmp = root;
            int flag = 0;
            while(true) {
                if (tmp == NULL) break;
                if (Fewer(ans, tmp->data->first)) {
                    if (flag) {
                        if (Fewer(tmp->data->first, Current_Key))
                            Current_Key = tmp->data->first;
                    }
                    Current_Key = tmp->data->first;
                    tmp = tmp->left;
                    flag = 1;
                } else {
                    tmp = tmp->right;
                }
            }
            iterator next = find(Current_Key);
            return *this;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
            Key ans = origin->data->first;
            Key Current_Key;
            node * tmp = root;
            int flag = 0;
            while(true) {
                if (tmp == NULL) break;
                if (Fewer(ans, tmp->data->first)) {
                    if (flag) {
                        if (Fewer(tmp->data->first, Current_Key))
                            Current_Key = tmp->data->first;
                    }
                    Current_Key = tmp->data->first;
                    tmp = tmp->left;
                    flag = 1;
                } else {
                    tmp = tmp->right;
                }
            }
            if (!flag) {origin = tail;}
            iterator next = find(Current_Key);
            origin = next->origin;
            return next;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {

		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {

		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {}
		bool operator==(const iterator &rhs) const {
		    return (origin == rhs.origin);
		}
		bool operator==(const const_iterator &rhs) const {
		    return (origin == rhs.origin);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    return (origin != rhs.origin);
		}
		bool operator!=(const const_iterator &rhs) const {
		    return (origin != rhs.origin);
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
		    value_type * ans = &origin->data;
		    return ans;
		}
	};
	class const_iterator {
	    friend map;
        friend iterator;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
	        node * origin;
		public:
            explicit const_iterator(node * t):origin(t){}
			const_iterator() {
				origin = new node();
			}
			const_iterator(const const_iterator &other) {
				origin = other.origin;
			}
			const_iterator(const iterator &other) {
				origin = other.origin;
			}
            bool operator==(const iterator &rhs) const {
                return (origin == rhs.origin);
            }
            bool operator==(const const_iterator &rhs) const {
                return (origin == rhs.origin);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return (origin != rhs.origin);
            }
            bool operator!=(const const_iterator &rhs) const {
                return (origin != rhs.origin);
            }
            value_type* operator->() const noexcept {
                value_type * ans = &origin->data;
                return ans;
            }
	};
	/**
	 * TODO two constructors
	 */
	map() {root = NULL; length = 0; tail = new node();}
	map(const map &other) {
        root = copy(other.root);
        length = other.length;
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
	    clear();
        root = copy(other.root);
        length = other.length;
        return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {clear();}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
        return Find(key)->data.second;
	}
	const T & at(const Key &key) const {
        return Find(key)->data.second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
        if (find(key) != NULL) return at(key);
        else {
            value_type value(key, T());
            insert(value);
        }
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
	    return at(key);
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
	    node * tmp = root;
	    while (tmp->left != NULL) tmp = tmp->left;
	    iterator ans(tmp);
	    return ans;
	}
	const_iterator cbegin() const {
        node * tmp = root;
        while (tmp->left != NULL) tmp = tmp->left;
        const_iterator ans(tmp);
        return ans;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
        iterator ans(tail);
        return ans;
	}
	const_iterator cend() const {
        const_iterator ans(tail);
        return ans;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const { return (length == 0);}
	/**
	 * returns the number of elements.
	 */
	size_t size() const { return length;}
	/**
	 * clears the contents
	 */
	void clear() {make_empty(root); length = 0;}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
	    insert(value, root);
	}


	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
        remove(pos->first);
	}

	void remove(const Key & x){
	    remove(x, root);
	}
	bool remove(const Key & x, node * t){
	    if (t == NULL) return true;
	    if (x == t->data.key){
	        if (t->left == NULL || t->right == NULL){
	            //t has 1 child
                node * OldNode = t;
                if (t->left != NULL) t = t->left;
                else t = t->right;
                delete OldNode;
                return false;//t->height has changed
	        }
	        else {//t has 2 child
                node * tmp = t->right;
                while(tmp->left != NULL) tmp = tmp->left;
                t->data = tmp->data;
                if (remove(tmp->data.first, t->right)) return true;
                return adjust(t, 1);
	        }
	    }
	    else if (Fewer(x, t->data.first)){
	        if (remove(x, t->left)) return true; //remove successfully
	            return adjust(t, 0);        //not successful, need to adjust. //
	            // may keep adjusting until ok
	    }
        else {
            if (remove(x, t->right)) return true;
            else return adjust(t, 1);
        }
	}


	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
	    size_t sum = 0;
	    node * t = root;
	    while (t != NULL){
	        if (!Fewer(key, t->data.first) && !Fewer(t->data.first, key) ) sum++;
	        if (Fewer(key, t->data.first)) {
	            t = t->left;
	        }
	        else {
	            t = t->right;
	        }
	    }
	    //if (sum != 0 && sum != 1) throw(index_out_of_bound());
	    return sum;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	static iterator find(const Key &key) {
	    node * t = Find(key);
	    iterator ans(t);
	    return ans;
	}
	static const_iterator find(const Key &key) const {
	    node * t = Find(key);
	    const_iterator ans(t);
	    return ans;
	}

private:
    node* copy(const node * other){
	    if (other == NULL) return NULL;
        auto tmp = new node(other->data, copy(other->left), copy(other->right), other->height);
	    return tmp;
	}

    void insert(const value_type &value, node * t){
        if (t == NULL) t = new node(value, NULL, NULL);
        else
        if(Fewer(value.first, t->data.first)){
            insert(value, t->left);
            if (t->left->height - t->right->height == 2){
                if (Fewer(value.first, t->left->data.first)) LL(t);
                else LR(t);
            }
        }
        else{
            insert(value, t->right);
            if (t->right->height - t->left->height == 2){
                if (Fewer(value.first, t->right->data.first)) RL(t);
                else RR(t);
            }
        }
        t->height = max(t->left->height, t->right->height) + 1;
    }

	void make_empty(node * p){
	    if (p == NULL) return;
	    make_empty(p->left);
	    make_empty(p->right);
	    delete p;
	}
	int height(node * t) const{ return t == NULL ? 0 : t->height; }
	node * Find(const Key &x) const{
	    node * t = root;
	    while(t != NULL && t->data != x){
	        if (Fewer(x, t->data.first)) t = t->left;
	        else t = t->right;
	    }
	    if (t == NULL) throw(index_out_of_bound());
        return t;
	}
	void LL(node * &t){
	    node * l = t->left;
	    t->left = l->right;
	    l->right = t;
	    t->height = max(t->right->height,t->left->height) + 1;
	    l->height = max(l->left->height, t->height);
        t = l;
	}
    void LR(node * &t){
	    RR(t->left);
	    LL(t);
	}
    void RL(node * &t){
	    LL(t->right);
	    RR(t);
	}
    void RR(node * &t){
	    node * r = t->right;
	    t->right = r->left;
	    r->left = t;
	    t->height = max(t->left->height, t->right->height) + 1;
	    r->height = max(t->height, r->left->height) + 1;
        t = r;
	}
	bool adjust(node * &t, int SubTree){
	    if (SubTree){
	        if (t->left->height - t->right->height == 1) return true;
	        if (t->left->height == t->right->height) {t->height-- ; return false;}
	        if (t->left->right->height > t->left->left){
	            LR(t); return false;
	        }
	        LL(t);
            return t->left->height != t->right->height;
	    }
	    else {
	        if (t->right->height - t->left->height == 1) return true;
	        if (t->left->height == t->right->height) {t->height--; return false;}
	        if (t->right->left->height > t->right->right->height){
	            RL(t); return false;
	        }
	        RR(t);
            return t->right->height != t->left->height;
	    }
	}

    int max(int a, int b) {return (a > b)?a: b;}
};



}

#endif
