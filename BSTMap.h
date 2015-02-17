// BSTMap.h an implementation of binary search trees as a template

/*
 * A "map" is an abstract data structure that functions like a "dictionary". You store
 * information in the map (like definitions of words) and then use keys (i.e., the words themselves)
 * to retrieve that information. Our CustomerDB was a map. In that context, we used Strings (names)
 * as the keys, and we used Customer records as the information. 
 *
 * When a map is implemented as a standalone data structure, it typically separates the
 * key and value completely. So, unlike our CustomerDB where the key was Customer::name
 * (i.e., the key was a data component in the Customer struct), in a standalone map
 * the key will be a separate type.
 *
 * Maps can be based on Binary Search Trees, on Hash Tables or even on Vectors.
 * This implementation is a Binary Search Tree based map. However, you can use a map
 * in exactly the same way regardless of the physical data structure it is built on.
 *
 * The primary operations for a map are:
 *	BSTMap<KeyType, ValueType> myMap; // constructor -- creates an emtpy map
 *		Note that KeyType will be some type (e.g., String), and that will be the data type
 *		you use for all your keys. Similarly ValueType will be a type, and that will be the
 *		data type you use for all your information. For the CustomerDB, the KeyType would
 *		have been String, and the ValueType would have been Customer
 *  myMap.isMember(key) -- // checks to see if a key is stored in the map. For example
 *		you can check to see if there is a mapping for the String "Craig" with
 *		myMap.isMember("Craig") -- naturally this assumes that String is your KeyType
 *  myMap.insert(key, value) -- a low-level insert function. If key is not in the map already
 *		then key is added to the map and is stored with "value". If key is already in the map
 *		then the mapping for that key is changed -- the old mapping is deleted and the key
 *		now maps to value
 *	myMap[key] = value -- same as insert(key, value)
 *  ValueType v = myMap[key] -- reads the current mapping for key and stores it in v
 *  myMap.remove(key) -- removes any mapping stored for key. If there is no mapping for key
 *		then remove does not do anything.
 * 
 * BSTMap<KeyType, ValueType>::Iterator p = myMap.get_iterator()
 *		The get_iterator function returns an EE312-style iterator object for scanning the
 *		map. The iterator's type is a nested type called Iterator. So, to declare your
 *		iterator, you must use the full type of the map (including the KeyTypes and ValueTypes)
 *		and then append ::Iterator to that name. The get_iterator() function starts the scan
 *		at a reasonable starting point for the map (for trees, at the smallest key)
 *		To scan the entire map:
 *
 *		for (BSTMap<KeyType, ValueType>::Iterator p = myMap.get_iterator();
 *				! p.done(); p.advance()) {
 *			KeyType key = p->key;
 *			ValueType val = p->value;
 *		}
 * 
 *		Note that the iterator behaves sort of like a pointer, and you can read both
 *      the key and value stored at each mapping. You can use the iterator to change
 *      the value in any mapping. However, if you assign a new key using the iterator
 *      bad things may happen to your map.
 *			p->value = new_value; // OK
 *			p->key = new_key; // really, really bad.
 *
 * Finally, the map has a destructor, copy-constructor and (re)assignment operator
 * so you can make copies as you wish.
*/

#ifndef _BSTMap_h
#define _BSTMap_h 1
#include <stdint.h>
#include <assert.h>

template <typename Key, typename DataType>
struct BSTMap {
private:
	struct Entry;
	struct Node;
	struct Entry {
		Key key; // this is what you search for
		DataType value; // this is what you store in the database

		Entry(const Key& k) : key(k) {}

		bool operator<(const Entry& rhs) const {
			return *this->key < rhs.key;
		}


		bool operator==(const Entry& rhs) const {
			if (this->key < rhs.key) { return false;  }
			if (rhs.key < this->key) { return false;  }
			return true;
		}

		bool operator<(const Key& key) const {
			return this->key < key;
		}

		bool operator==(const Key& key) const {
			if (this->key < key) { return false;  }
			if (key < this->key) { return false;  }
			return true;
		}
	};

	struct Node {
	public:
		Entry payload;
		Node* left;
		Node* right;
		Node* parent;

		Node(const Entry& val) : payload(val) {
			left = right = parent = nullptr;
		}

		Node(const Key& key) : payload(Entry(key)) {
			left = right = parent = nullptr;
		}

		Node(const Node& rhs) : payload(rhs.payload) { recursiveCopy(rhs); }
		~Node(void) { recursiveDestroy(); }

	private:
		/* I do not want to accidentally assign nodes
		 * assigning pointers to nodes is fine, but reassigning an existing node
		 * has no utility for me and, if used unwisely, could cause major problems
		 * for me. to make sure I don't forget that fact, I'm making assignment
		 * 	a) private, so it can't be called outside the Node struct
		 * 	b) an instant crash, so that if I did call it, I'd hopefully realize I was being dumb
		 */
		void operator=(const Node& rhs) {
			assert(0);
		}


		void recursiveCopy(const Node& that) {
			if (that.right) {
				right = new Node(*that.right);
				right->parent = this;
			} else {
				right = nullptr;
			}

			if (that.left) {
				left = new Node(*that.left);
				left->parent = this;
			} else {
				left = nullptr;
			}

			parent = nullptr;
		}

		void recursiveDestroy(void) {
			delete left;
			delete right;
		}

	}; // end of Node struct for my BST

	/* a Link is a convenient way to represent the double-links that include the
	 * regular "down" pointers (left and right) and the parent "up" pointer
	 * Every node is doubly-linked to its parent. The Link captures both pointers.
	 */
	struct Link {
		Node*& down;
		Node* up;
		Node* operator->() { return down; }
		void operator=(Node* p) { down = p;
			if (p) { p->parent = up; }
		}
		Link(Node*& d, Node* u) : down(d), up(u) {}

		operator bool(void) const { return down != nullptr;  }
		operator Node*(void) const { return down; }
	};


	Node* root; // pointer to the root of the tree, nullptr when tree is empty
	uint32_t length; // number of values in the tree

public:
	BSTMap(void) { root = nullptr; length = 0; }
	~BSTMap(void) { destroy(); }
	BSTMap(const BSTMap<Key, DataType>& rhs) { copy(rhs); }
	BSTMap<Key, DataType>& operator=(const BSTMap<Key, DataType>& rhs) {
		if (this != &rhs) {
			destroy();
			root = 0; length = 0;
			copy(rhs);
		}
		return *this;
	}

	uint32_t size(void) const { return length; }

	bool isMember(const Key& v) {
		return find(v);
	}

	DataType& operator[](const Key& v) {
		Link p = find(v);
		if (p) { return p->payload.value; }

		Node* child = new Node(v);
		p = child;
		length += 1;
		return child->payload.value;
	}

	/* add a (key, value) pair.
	 * This map does not permit duplicate keys. Hence, if you've already inserted
	 * this key into the map, we will not insert a second copy of the key. Instead
	 * if there is already a key in the map that matches the key param, then
	 * we simply update the mapping for the key from the old value to the new value.
	 *
	 * For example, in Project6, we'd add
	 * a (String, Customer) pair. The String is the name of the customer, and the Customer
	 * is the Customer struct that stores his/her information. Note that the customer's
	 * name is stored twice, once as the "key" and stored redundantly as one of the fields
	 * in the Customer struct. That's not optimally efficient, but it does keep the
	 * interface cleaner. The way I've designed this map, the data structure doesn't have
	 * to know that your DataType has a field called "name" or anything like that.
	 * The data structure just remembers stores each value with it's corresponding key
	 * and then lets you search for the values based on their keys. Most of the time
	 * your value will have a redundant copy of the key stored inside it ('cause it will be
	 * useful for you to have that key).
	 */
	void insert(const Key& key, const DataType& value) {
		(*this)[key] = value;
	}


	/*
	 * locate the key in the map and remove the (key, value) pair for that key
	 * If the key cannot be found in the map, then do nothing
	 */
	void remove(const Key& key) {
		Link p = find(key);
		if (!p) { return; }

		Node* victim = nullptr;
		/* standard BST remove function -- two cases based on whether right subtree exists */
		if (p->right) { // hard case, p has right subtree
			Link s = successor(p);
			victim = s;
			p->payload = s->payload;
			// successor has no left child, so it's easy to remove
			s = s->right;
		} else { // easy case, p has no right subtree, stitch around p
			victim = p;
			p = p->left;
		}
		victim->left = victim->right = nullptr;
		delete victim;
	}

	struct Iterator {
		Node* ptr;
		Iterator(void) { ptr = nullptr; }
		Iterator(Node* p) { ptr = p; }
		bool done(void) const { return ptr == nullptr; }
		void advance(void)  {
			if (ptr->right) {
				ptr = ptr->right;
				while (ptr->left) {
					ptr = ptr->left;
				}
			} else {
				while (ptr->parent && ptr->parent->left != ptr) {
					ptr = ptr->parent;
				}
				ptr = ptr->parent;
			}
		}
		Entry& operator*(void) { return ptr->payload; }
		Entry* operator->(void) { return &ptr->payload; }
	};

	Iterator get_iterator(void) {
		if (!root) { return Iterator(nullptr); }
		Node* p = root;
		while (p->left) {
			p = p->left;
		}
		return Iterator(p);
	}

private:
	Link find(const Key& v) {
		if (!root) { return Link(root, nullptr); }
		if (root->payload == v) { return Link(root, nullptr); }

		Node* p = root;
		while (true) {
			if (p->payload < v) {
				if (p->right && !(p->right->payload == v)) { p = p->right; }
				else { return Link(p->right, p); }
			} else {
				if (p->left && !(p->left->payload == v)) { p = p->left; }
				else { return Link(p->left, p); }
			}
		}
		/* NOT REACHED */
		assert(0);
		return Link(root, 0);
	}

	Link successor(Link p) {
		if (p->right) { // successor is smallest node in right subtree
			Node* s = p->right;
			if (! s->left) { return Link(p->right, p.down); }
			while (s->left->left) {
				s = s->left;
			}
			return Link(s->left, s);
		} else { // successor is closest ancestor where p is a left descendant
			Node* s = p.down;
			while (s->parent && s->parent->left != s) {
				s = s->parent;
			}
			if (s->parent) { return Link(s->parent, s->parent->parent); }
			else { return Link(s->parent, nullptr); }
		}
	}

	/* destroy assumes that *this will not be used after destroy is called
	 * i.e., destroy returns *this into an uninitialized object */
	void destroy(void) {
		delete root;
	}

	/* copy assumes that *this is uninitialized */
	void copy(const BSTMap<Key,DataType>& that) {
		if (that.root) {
			root = new Node(*that.root);
		} else {
			root = nullptr;
		}
		length = that.length;
	}
};

#endif /* _BSTMap_h */
