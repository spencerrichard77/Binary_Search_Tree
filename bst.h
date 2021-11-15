#ifndef BST_H
#define BST_H

#include <utility>
#include <functional>
#include <stdexcept>

namespace pic10c {

	/**
	* templated binary search tree class
	* @param T the data type of binary search tree
	* @param compare_type the comparison function to compare the data
	*/
	template <typename T, typename compare_type = std::less<T>>
	class bst {

	public:

		/**
		* constructor which initializes: pred, root, and tree_size
		* @param pred_input the comparison function to compare the data
		*/
		bst(const compare_type& pred_input = compare_type()) :
			pred(pred_input), root(nullptr), tree_size(0) {}

		/**
		* iterator class declaration
		*/
		class iterator;

		/**
		* destructor which passes the implicit root to deleteTree
		*/
		~bst() { deleteTree(root); }

		/**
		* copy constructor
		* @param rhs copy-from tree
		*/
		bst(const bst& rhs);

		/**
		* move constructor
		* @param that rvalue reference to move-from tree
		*/
		bst(bst&& that) noexcept;

		/**
		* copy/move assignment operator
		* @param that copy/move-from tree
		*/
		bst& operator=(bst that) &;
		
		/**
		* checks if a tree contains a particular element
		* @param item the type T element to look for
		* @return iterator to the element
		*/
		iterator find(const T& item) const;

		/**
		* swaps two trees
		* @param other tree to swap the implicit "this" tree with
		*/
		void swap(bst& other);

		/**
		* returns an iterator to the node containing the "smallest" value
		* @return iterator to the farthest left node
		*/
		iterator begin() const;

		/**
		* returns an iterator to past-the-end position
		* @return iterator to nullptr
		*/
		iterator end() const;

		/**
		* adds given lvalue to the tree
		* @param value the element to be added
		*/
		void insert(const T& value);

		/**
		* adds given rvalue to the tree
		* @param value the element to be added
		*/
		void insert(T&& value);

		/**
		* removes given value from the tree
		* @param bad iterator to node containing value to remove
		*/
		void erase(iterator bad);

		/**
		* templated variadic function to construct T and
		* attempt to place it within the tree
		* @param args variadic list of arguments used to construct T
		*/
		template <typename... Types>
		void emplace(Types&&... args);

		/**
		* accessor to tree_size
		* @return tree_size
		*/
		size_t size() const;

	private:
		class node; // nested node class
		node* root; // root node of the bst
		compare_type pred; // comparison function to compare the data
		size_t tree_size; // number of elements in the bst
		void deleteTree(node*); // recursively delete elements of tree
		void traverseInsert(node*); // help with copying
	};

	//nested iterator class definition
	template <typename T, typename compare_type>
	class bst<T, compare_type>::iterator { //nested iterator class
		
		friend bst; //to allow iterator modifications by bst operations
	
	public:

		/**
		* overloaded prefix ++
		*/
		iterator& operator++() {

			if (curr->right) { // current node has right child
				
				curr = curr->right; // move to right child
				
				while (curr->left) { // current node has left child

					curr = curr->left; // move to left child
				}
			}
			else { // current node does not have right child
				
				auto p = curr->parent; // current node's parent
				
				// current node exists and is parent's right child
				while (p && (curr == p->right)) {

					curr = p; // move to parent node
					p = p->parent; // update parent
				}
				
				// move to parent node
				// if previously at farthest right node, nullptr specifies end
				curr = p;
			}
			return *this;
		}

		/**
		* overloaded postfix ++
		*/
		iterator operator++(int) {
			
			auto copy(*this); // copy of current node

			if (curr->right) { // current node has right child
				
				curr = curr->right; // move to right child
				
				while (curr->left) { // current node has left child

					curr = curr->left; // move to left child
				}
			}
			else { // current node does not have right

				auto p = curr->parent; // current node's parent

				// current node is parent's right child
				while (p && (curr == p->right)) {

					curr = p; // move to parent node
					p = p->parent; // update parent
				}
				
				// move to parent node
				// if previously at farthest right node, nullptr specifies end
				curr = p;
			}
			return copy;
		}

		/**
		* overloaded prefix --
		*/
		iterator& operator--() {
			
			if (curr->left) { // current node has left child
				
				curr = curr->left; // move to left child
				
				while (curr->right) { // current node has right child

					curr = curr->right; // move to right child
				}
			}
			else { // current node does not have left child
				
				auto p = curr->parent; // current node's parent
				
				// current node is parent's left child
				while (curr == p->left) {

					curr = p; // move to parent node
					p = p->parent; // update parent
				}
				
				curr = p; // current node is right child, move to parent
			}
			return *this;
		}

		/**
		* overloaded postfix --
		*/
		iterator operator--(int) {
			
			auto copy(*this); // copy of current node
			
			if (curr->left) { // current node has left child

				curr = curr->left; // move to left child

				while (curr->right) { // current node has right child

					curr = curr->right; // move to right child
				}
			}
			else { // current does not have left

				auto p = curr->parent; // current's parent

				while (curr == p->left) { // current is parent's left child

					curr = p; // move to parent node
					p = p->parent; // update parent
				}

				curr = p; // current is right child, move to parent
			}
			return copy;
		}

		/**
		* overloaded == comparison operator
		*/
		friend bool operator==(const iterator& left, const iterator& right) {
			
			/* whether left iterator's current node is
			right iterator's current node */
			return left.curr == right.curr;
		}

		/**
		* overloaded != comparison operator
		*/
		friend bool operator!=(const iterator& left, const iterator& right) {

			/* whether left iterator's current node is not
			right iterator's current node */
			return left.curr != right.curr;
		}

		/**
		* overload dereferencing operator (without modifying tree element)
		*/
		const T& operator*() const {
			return curr->value; // value contained by current node
		}

		/**
		* overload the operator arrow (without modifying tree element)
		*/
		const T* operator->() const {
			return &curr->value; // value contained by current node
		}

	private:

		/**
		* constructor which initializes curr and container
		* @param n the current node
		* @param c the tree container
		*/
		iterator(node* n = nullptr, const bst* c = nullptr) :
			curr(n), container(c) {}

		node* curr; //current position
		const bst* container; // holding container
	};

	// destructor which passes the implicit root to deleteTree
	template <typename T, typename compare_type>
	void bst<T, compare_type>::deleteTree(node* n) {
		
		if (n) { // node exists
			
			if (n->left) { // left child is not null
				deleteTree(n->left); // recurse on left node
			}
			
			if (n->right) { // right child is not null
				deleteTree(n->right); // recurse on right node
			}
			
			delete n; // delete node
		}
	}

	// to help with copying
	template <typename T, typename compare_type>
	void bst<T, compare_type>::traverseInsert(node* n) {

			insert(n->value); // insert value
			
			if (n->left) { // left node exists
				traverseInsert(n->left); // recurse on left node
			}
			
			if (n->right) { // right node exists
				traverseInsert(n->right); // recurse on right node
			}
	}

	// copy constructor
	template <typename T, typename compare_type>
	bst<T, compare_type>::bst(const bst& rhs) : root(nullptr), pred(rhs.pred) {
		
		traverseInsert(rhs.root); // helper function to recursively copy nodes
	}

	// move constructor
	template <typename T, typename compare_type>
	bst<T, compare_type>::bst(bst&& that) noexcept : bst() {
		
		(*this).swap(that); // swap implicit tree with given tree
	}

	// copy/move assignment operator
	template <typename T, typename compare_type>
	bst<T, compare_type>& bst<T, compare_type>::operator=(bst that) & {
		
		(*this).swap(that); // swap implicit tree with given tree
		return *this; // return implicit tree
	}

	// swap two Trees (member function)
	template <typename T, typename compare_type>
	void bst<T, compare_type>::swap(bst& other) {
		
		// swap root of implicit tree with given tree
		std::swap(this->root, other.root);

		// swap predicate of implicit tree with given tree
		std::swap(this->pred, other.pred);

		// swap tree_size of implicit tree with given tree
		std::swap(this->tree_size, other.tree_size);
	}

	// swap two Trees (free function)
	template <typename T, typename compare_type>
	void swap(bst<T, compare_type>& first, bst<T, compare_type>& second) {

		first.swap(second); // use member function to swap first tree with second
	}

	// iterator to begin position (farthest left node)
	template <typename T, typename compare_type>
	typename bst<T, compare_type>::iterator bst<T, compare_type>::begin() const {
		
		if (!root) { // root is null (tree is empty)

			// return iterator to past-the-end position (nullptr)
			return iterator(nullptr, this);
		}
		
		node* n = root; // start at the root
		
		while (n->left) { // left child is not null
			n = n->left; // go left
		}
		
		return iterator(n, this); // iterator to node containing smallest value
	}

	// iterator to past-the-end position (nullptr)
	template <typename T, typename compare_type>
	typename bst<T, compare_type>::iterator bst<T, compare_type>::end() const {
		return iterator(nullptr, this); // iterator to nullptr
	}

	// to add a value to the tree (lvalue)
	template <typename T, typename compare_type>
	void bst<T, compare_type>::insert(const T& val) {

		if (!root) { // root is null (tree is empty)
			
			root = new node(val); // create a new node
			tree_size = tree_size + 1; // increment size of tree
		}

		else { // root node exists

			node* n = new node(val); // create a new node

			// recursively pass it node to node until in place
			// true if node was sucessfully inserted
			bool insert_successful = root->insertNode(n, pred); 
			
			if (insert_successful) { // node was sucessfully inserted
				tree_size = tree_size + 1; // increment size of tree
			}
		}
	}

	// to add a value to the tree (rvalue)
	template <typename T, typename compare_type>
	void bst<T, compare_type>::insert(T&& val) {

		if (!root) { // root is nullptr

			root = new node(std::move(val)); //create a new node
			tree_size = tree_size + 1; // increment size of tree
		}
		else { // root is not nullptr

			node* n = new node(std::move(val)); // create a new node

			// recursively pass it node to node until in place
			// true if node was sucessfully inserted
			bool insert_successful = root->insertNode(n, pred);

			if (insert_successful) { // node was sucessfully inserted
				tree_size = tree_size + 1; // increment size of tree
			}
		}
	}

	// nested node class definition
	template <typename T, typename compare_type>
	class bst<T, compare_type>::node {
		
		friend bst; // tree member functions may search through nodes
		friend iterator; // to be able to advance by checking node values
	
	private:

		/**
		* constructor which initializes value, left, right, and parent
		*/
		node(T val) : value(std::move(val)), left(nullptr),
			right(nullptr), parent(nullptr) {}

		node* left; // left child node
		node* right; // right child node
		node* parent; // parent node

		T value; // data value stored

		/**
		* insert() helper function to insert node into tree
		* @param n the node to be inserted
		* @param p the predicate function to compare nodes
		* @return whether the node was successfully inserted
		*/
		bool insertNode(node* n, compare_type p);
	};

	// helper function to insert node into tree
	template<typename T, typename compare_type>
	bool bst<T, compare_type>::node::insertNode(node* n, compare_type pred) {
		
		// value is less than new node value
		if (pred(value, n->value)) {

			if (right) { // right child exists

				right->insertNode(n, pred); // recurse on the right child
			}

			else { // right child does not exist

				right = n; // set right child to node
				n->parent = this; // set parent for node
				return true; // indicate insertion was successful
			}
		}

		// value is greater than new node value
		else if (pred(n->value, value)) {

			if (left) { // left child exists

				left->insertNode(n, pred); // recurse on the left child
			}
			else { // left child does not exist

				left = n; // set left child to node
				n->parent = this; // set parent for node
				return true; // indicate insertion was successful
			}
		}

		// value neither less than or greater than new node value
		else {

			delete n; // free the allocated node memory
			return false; // indicate insertion was not successful
		}
	}

	// finds value in tree
	template <typename T, typename compare_type>
	typename bst<T, compare_type>::iterator bst<T, compare_type>::find(const T& val) const {
		
		if (!root) { // root is null (tree is empty)
			
			return iterator(nullptr, this); // past-the-end iterator
		}
		
		node* n = root; // start at the root

		while (true) { // loop until an iterator is returned

			// current node value contains correct value
			if (!pred(val, n->value) && !pred(n->value, val)) {

				return iterator(n, this); // iterator to node
			}

			// value less than current node value and left child exists
			else if (pred(val, n->value) && (n->left)) {

				n = n->left; // move left
			}

			// value greater than current node value and right child exists
			else if (pred(n->value, val) && (n->right)) {

				n = n->right; // move right
			}

			// value is not in tree
			else {

				return iterator(nullptr, this); // past-the-end iterator
			}
		}
	}

	// removes given value from the tree
	template <typename T, typename compare_type>
	void bst<T, compare_type>::erase(iterator i) {
		
		node* n = i.curr; // node to be removed

		if (n->parent) { // node has parent
			
			// node has no children
			if ((!n->left) && (!n->right)) {

				if (n->parent->left == n) { // node is left child

					// set parent's left pointer to null
					n->parent->left = nullptr;
				}

				else { // node is right child

					// set parent's right pointer to null
					n->parent->right = nullptr;
				}

				delete n; // delete node
			}

			// node has one child
			else if ((!n->left) ^ (!n->right)) {
				
				if (n->left) { // node has left child

					// set given left child's parent to given node's parent
					n->left->parent = n->parent;

					/* set given node's parent's left child to
					given node's left child */
					n->parent->left = n->left;
				}
				else { // node has right child

					// set given right child's parent to given node's parent
					n->right->parent = n->parent;

					/* set given node's parent's right child to
					given node's right child */
					n->parent->right = n->right;
				}
				delete n; // delete node
			}
			else { // node has two children
				
				node* temp = n; // copy of node

				temp = temp->right; // move to copy's right child

				while (temp->left) { // copied node has left child
					temp = temp->left; // move to copy's left child
				}

				// set given node's value with the right-far-left value
				n->value = temp->value;
				
				// construct iterator to right-far-left node
				iterator recurse(temp, this);

				// remove the right-far-left node with zero or one child
				erase(recurse);
			}
		}
		else { // node does not have a parent
			
			// node has no children
			if ((!n->left) && (!n->right)) {

				delete n; // delete node
			}

			// node has one child
			else if ((!n->left) ^ (!n->right)) {

				if (n->left) { // node has left child

					// set given left child's parent to nullptr
					n->left->parent = nullptr;

					root = n->left; // set root to left child
				}
				else { // node has right child

					// set given right child's parent to nullptr
					n->right->parent = nullptr;

					root = n->right; // set root to right child
				}
				delete n; // delete node
			}

			// node has two children
			else {
				
				node* temp = n; // copy of node
				temp = temp->right; // move to copy's right child

				while (temp->left) { // copied node has left child
					temp = temp->left; // move to copy's left child
				}

				// set given node's value with the right-far-left value
				n->value = temp->value;

				// construct iterator to right-far-left node
				iterator recurse(temp, this);

				// remove the right-far-left node with zero or one child
				erase(recurse);
			}
		}
	}

	/* accepts variadic listand constructs a T and
	attempt to place within the tree */
	template <typename T, typename compare_type>
	template <typename... Types>
	void bst<T, compare_type>::emplace(Types&&... args) {

		// construct and insert an object of type T
		insert(T(std::forward<Types>(args)...));
	}

	// accessor to tree_size
	template <typename T, typename compare_type>
	size_t bst<T, compare_type>::size() const {
		return tree_size;
	}
}

#endif