/*------------------------------------------------------------------------------------------------

	Author:		Boyer, Destiny
	Date:		12/8/2016

	Templatized Binary Search Tree. Class has fucntionality to read from a file and build a
	binary search tree. All responsibility on handling file stream data is on the objects
	that will be inserted into the binary search tree. Class uses recursion to insert,
	delete, and find objects in the tree.

------------------------------------------------------------------------------------------------*/

#ifndef binarysearchtree_h
#define binarysearchtree_h


#include <string>
#include <iostream>

template<class NodeData>

class BinarySearchTree {

	private:

	class Node {
		
		public:

		NodeData* data;		//pointer to data
		Node* left;			//pointer to left subtree
		Node* right;		//pointer to right subtree

		Node(void) : data(nullptr), left(nullptr), right(nullptr) {};
		Node(NodeData* d) : data(d), left(nullptr), right(nullptr) {};
		Node(NodeData* d, Node* l, Node* r) : data(d), left(l), right(r) {};
	};

	public:

	BinarySearchTree(void);	
	~BinarySearchTree(void);

	bool insert(NodeData* toInsert);
	bool retrieve(const NodeData& target, NodeData*& retrieved) const;
	void display(void) const;
	void clear(void);
	bool isEmpty(void) const;
	void buildTree(ifstream& inFile);

	private:

	Node* root;

	Node* insertHelper(Node* current, NodeData* toInsert);
	void clearHelper(Node* current);
	void displayHelper(Node* current) const;
	bool retrieveHelper(Node* current, const NodeData& target, NodeData*& retrieved) const;
};

#endif // !binarysearchtree_h

///////////////////////////////////////////////////////////////////////////////////////////////////
//																								 //
//										Public Methods											 //
//																								 //
///////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------

	Default no-args constructor.

	POSTCONDITIONS:
		- instantiates BinarySearchTree object
		- initializes root to NULL

-------------------------------------------------------------------------------------------------*/

template <typename NodeData>
BinarySearchTree<NodeData>::BinarySearchTree(void) {
	root = NULL;
};

/*-------------------------------------------------------------------------------------------------
	
	Destructor. Calls clear to deallocate all memory.

	POSTCONDITIONS:
		- deallocates all dynamically allocated memory and deletes all nodes in the tree
		- sets root to NULL

	NOTES:	It is the responsibility of the NodeData to deallocate any memory that it has
			allocated.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
BinarySearchTree<NodeData>::~BinarySearchTree(void) {
	clear();
};

/*------------------------------------------------------------------------------------------------

	Method takes in a NodeData* and calls retrieve to ensure that the NodeData is not already inserted
	in the binary search tree. If this is false recInsert is called to create a new node and
	insert the Object in to the tree.

	PRECONDITIONS:
		- toInsert cannot be NULL
		- toInsert cannot already be in the binary search tree

	POSTCONDITIONS:
		- adds new Node containing toInsert to the BST
		- returns a bool value indicating success

	NOTES:	It is the responsibility of the Object to provide functionality for comparing
	equality.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
bool BinarySearchTree<NodeData>::insert(NodeData* toInsert) {
	if (toInsert == NULL) {
		return false;
	}
	
	NodeData* temp;					//temp objects to use for retrieve
	NodeData temp2 = *toInsert;

	if (retrieve(temp2, temp)) {	//if a Node containing the same NodeData
		return false;				//is already in the tree, return false
	}

	root = insertHelper(root, toInsert);	//calls recInsert to insert a new node in the tree
	return true;
};

/*------------------------------------------------------------------------------------------------

	Calls recRetrieve to recusively traverse nodes in the tree and compare them to the
	target. If a Node containing a NodeData that is equal to the target is found, then
	retrieved is set equal to that Node's NodeData. If it is not found then retrieved is
	set equal to nullptr.

	POSTCONDITIONS:
		- second parameter is set equal to found NodeData or nullptr
		- returns bool indicating if the NodeData was found

	NOTES:	Equality comparisons are the responsibility of the NodeData object. Method
			cannot change any data members.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
bool BinarySearchTree<NodeData>::retrieve(const NodeData& target, NodeData*& retrieved) const {
	return retrieveHelper(root, target, retrieved);
};

/*------------------------------------------------------------------------------------------------

	Method calls displayHelper() to perform and in order traversal and print the contents of
	each Node's NodeData to the console.

	NOTES:	Method cannot change any data members.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
void BinarySearchTree<NodeData>::display(void) const {
	if (isEmpty()) {
		return;
	}
	displayHelper(root);
};

/*------------------------------------------------------------------------------------------------

	Calls clearHelper() to recusively delete each node in the binary search tree and dealloacate
	all dynamically allocated memory.

	POSTCONDITIONS:
		- deallocates all dynamically allocated memory and deletes all nodes in the tree
		- sets root to nullptr

	NOTES:	It is the responsibility of the NodeData's destructors to deallocate any
			memory dynamically allocated by the NodeData in the Node.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
void BinarySearchTree<NodeData>::clear(void) {
	if (isEmpty()) {
		return;
	}
	clearHelper(root);
	root = nullptr;
};

/*------------------------------------------------------------------------------------------------

	Returns a bool value indicating if the binary search tree is empty (root == NULL).

	POSTCONDITIONS:
		- returns a bool value indicating if root == NULL

	NOTES:	Method cannot change any data members.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
bool BinarySearchTree<NodeData>::isEmpty(void) const {
	return root == NULL;
};

/*------------------------------------------------------------------------------------------------

	This method takes in a file stream and hands the file stream to NodeData objects.
	Obejcts have the responsibility to properly handle the file stream to populate data members.
	NodeData object are then inserted into the Binary Search Tree by calling insert().

	PRECONDITIONS:
		- file stream must contain data formatted for NodeData objects

	POSTCONDITIONS:
		- builds Binary Search Tree of NodeData objects.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
void BinarySearchTree<NodeData>::buildTree(ifstream& infile) {
	NodeData* ptr;	//creates new Object*
	bool isValid;	//bool indicating if the data was valid for the Object

					//loops until the end of file is reached, creating new objects
					//setting data and attempting to insert them in to the BST
	for (;;) {
		ptr = new NodeData;
		isValid = ptr->setData(infile);	//hands filestream to Object to initialize

		if (infile.eof()) {	//breaks out of loop if the end of file is reached
			delete ptr;
			break;
		}

		if (isValid) {		//if object was populated with data
			insert(ptr);	//insert into the Binary Search Tree
		} else {
			delete ptr;		//otherwise delete the NodeData object 
		}
	}
};

/*------------------------------------------------------------------------------------------------

	Method recursively moves over each Node in the BST printing each Node's information.
	Method cannot change any data members.

	POSTCONDITIONS:
		- must be called by display

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
void BinarySearchTree<NodeData>::displayHelper(Node* current) const {
	if (current == NULL) {	//checks if we have reached an empty leaf
		return;
	} else {
		displayHelper(current->left);			//prints all information if left subtrees
		std::cout << *current->data << "  ";	//prints current subtree
		displayHelper(current->right);			//prints all information in right subtrees
	}
};

/*------------------------------------------------------------------------------------------------

	Takes in an Object* and recursively traverses the tree comparing the Object with the current
	node until the appropriate spot for insertion is reached. Creates a new node and
	inserts the Object in to the tree.

	PRECONDITIONS:
		- must be called by insert()

	POSTCONDITIONS:
		- adds new Node containing toInsert to the BST

	NOTES:	It is the responsibility of the Object to provide functionality for comparing
			equality.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
typename BinarySearchTree<NodeData>::Node* BinarySearchTree<NodeData>::insertHelper(Node* current, NodeData* toInsert) {
	if (current == NULL) {	//chceks if we have reached the bottom of the tree
		current = new Node(toInsert, NULL, NULL);	//creates a new node
	} else if (*current->data > *toInsert) {	//checks if toInsert is greater than current
		current->left = insertHelper(current->left, toInsert);	//if so goes to the left
	} else {	//otherwise recursively travles to the right
		current->right = insertHelper(current->right, toInsert);
	}
	return current;	//returns current
};


/*------------------------------------------------------------------------------------------------

	Recusively traveses the BST deleting each node in the binary search tree and dealloacating
	any dynamically allocated memory.

	POSTCONDITIONS:
		- deallocates all dynamically allocated memory and deletes all nodes in the tree

	NOTES:	It is the responsibility of the Object's destructors to know how to deallocate any
			memory dynamically allocated by the Object in the Node.

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
void BinarySearchTree<NodeData>::clearHelper(Node* current) {
	if (current != NULL) {	//base case
		clearHelper(current->right);	//recursively travels to the right
		clearHelper(current->left);	//recursively travels the left subtree
		delete current->data;				//deletes current
		delete current;
	}
};

/*------------------------------------------------------------------------------------------------

	Calls recRetrieve to recusively traverse nodes in the tree and compare them to the
	first parameter. The second parameter is set equal to the node that contains the target
	object, or NULL if it cannot be found.

	POSTCONDITIONS:
		- recursively searches tree for Node containing object equal to first parameter
		- second parameter is set equal to found object or NULL
		- returns bool indicating if Object was found

	NOTES:	It is the responsibility of the Object to know how to compare for equality

------------------------------------------------------------------------------------------------*/

template <typename NodeData>
bool BinarySearchTree<NodeData>::retrieveHelper(Node* current, const NodeData& target, NodeData*& retrieved) const {
	bool successful = false;
	if (current == NULL) {	//checks if we have reached the bottom of the tree
		return false;		//returns false
	}

	if (target == *current->data) {	//checks if current is equal to target
		retrieved = current->data;	//sets second parameter equal to current
		successful = true;				//returns true
	} else if (*current->data < target) {	//if current is less than target search the right
		retrieveHelper(current->right, target, retrieved);
	} else {	//otherwise search to the left
		retrieveHelper(current->left, target, retrieved);
	}
	return successful;	//returns whether we found the Obejct in the tree
};

