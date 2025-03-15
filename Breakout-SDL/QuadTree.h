#pragma once
#include "Vector2f.h"
#include "DoublyLL.h"
#include "Shape.h"
#include <unordered_set>

class QuadElement {
public:
	QuadElement(Shape& shp);
	~QuadElement();
	Shape* GetShape();
	void AddLeaf(void* leaf);

	Vector2f GetPos(); // THIS NEEDS TO ACCESS THE OUTER RECT!!! NOT THE SHAPES ACTUAL POS AND DIMENSIONS
	Vector2f GetDims();

	Shape& shape;
	DoublyLL<void*> leaves; // need to know what nodes i belong to when adjusting position

};

class QuadNode {
public:
	QuadNode(const Vector2f& position, const Vector2f& dimensions, QuadNode* par = NULL);
	~QuadNode();

	void AddChild(QuadElement* element);

	QuadNode* firstChild;
	DoublyLL<QuadElement*> elements;
	QuadNode* parent;
	QuadNode* next;
	int elementCount;
	Vector2f pos;
	Vector2f dims;
};

class QuadTree {
public:
	QuadTree();
	~QuadTree();

	// fill empty tree with elements
	void FillTree(std::vector<QuadElement*>& elements);
	void AddElement(QuadElement* element);
	void AddElement(QuadElement* element, QuadNode* node);
	void RemoveElement(QuadElement* element);
	void RemoveElement(QuadElement* element, QuadNode* node);

	std::vector<QuadNode*> GetLeaves();
	// implement recursively as well ^
	// for any objects that have moved, adjust their position in the tree if necessary

	// go through all of the leaves
	// any leaves with elements that don't intersect with them, remove the element from the node and add it again
	void AdjustTree();
	/*
	filltree:
		for each object to insert:
			if quadtree node is a leaf
				add the element
				if the count exceeds the limit
					partition the area and add the children to insert

	each element is going to want access to it's bounding rectangle
	when the element changes position, we need to test if it is still in that rectangle

	recursive deletion process:
		when four leaves of a partition all don't have elements, the parent
		should deallocate, then indicate that it is a leaf with zero children
		Note: this should happen recursively until there are no sets of leaf partitions
		with no elements
	
	*/

	//DoublyLL<QuadElement*> elements; // need to be able to index (can't use doublyll unless it supports that)
	//DoublyLL<QuadNode*> nodes; // when I partition, I will allocate four nodes
	std::unordered_set<QuadNode*> leaves; // can use a doublyLL for this
	DoublyLL<QuadNode*> nodes;
	QuadNode* root;

	void* toFree;
	int maxDepth;
	int maxLeaves;
	int capacity;
};