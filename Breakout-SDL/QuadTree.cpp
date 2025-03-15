#include "QuadTree.h"
QuadElement::QuadElement(Shape& shp) : shape(shp) {}
QuadElement::~QuadElement() {}
Shape* QuadElement::GetShape() { return &shape; }
void QuadElement::AddLeaf(void* leaf) { leaves.PushBack(leaf); }
Vector2f QuadElement::GetPos() { return shape.GetPos(); }
Vector2f QuadElement::GetDims() { return shape.GetDims(); }


QuadNode::QuadNode(const Vector2f& position, const Vector2f& dimensions, QuadNode* par) : pos(position), dims(dimensions), parent(par), elementCount(0), next(NULL) {}
QuadNode::~QuadNode() {}

void QuadNode::AddChild(QuadElement* element) {
	elements.PushBack(element);
}
//C:\Users\dpali\Documents\C++ Projects\Breakout\SDL-Game-Engine\Breakout-SDL\QuadTree.cpp
QuadTree::QuadTree() {}
QuadTree::~QuadTree() {}

bool Intersect(const Vector2f& posA, const Vector2f& dimsA, const Vector2f& posB, const Vector2f& dimsB) {
	return posA.x <= posB.x + dimsB.x && posA.x + dimsA.x >= posB.x && posA.y <= posB.y + dimsB.y && posA.y + dimsA.y >= posB.y;
}

bool Intersect(QuadElement* element, QuadNode* node) {
	return Intersect(element->pos, element->dims, node->pos, node->dims);
}

// fill empty tree with elements
void QuadTree::FillTree(std::vector<QuadElement*>& elements) {
	// start from the root
	// if the node is a leaf, add the element
	// if the count exceeds, make a partition
	// for each partition of the node
	//	if the rect intersects, go to that node
	//	if
	std::vector<std::pair<QuadNode*, QuadElement*>> stack;
	for (auto& element : elements) stack.push_back({ root, element });
	QuadNode* node;
	QuadElement* element;
	float x;
	float y;
	float w;
	float h;
	Vector2f newDims;
	QuadNode* childNode;
	while (!stack.empty()) {
		node = stack.back().first;
		element = stack.back().second;
		stack.pop_back();
		newDims.x = node->dims.x / 2;
		newDims.y = node->dims.y / 2;
		if (node->elementCount >= 0) { // leaf
			node->AddChild(element);
			node->elementCount++;
			if (node->elementCount > capacity) { // partition
				leaves.erase(node);
				node->elementCount = -1;
				for (auto& element : elements) {
					stack.push_back({ node, element });
				}
				childNode = new QuadNode(node->pos, newDims, node);
				node->firstChild = childNode;
				for (auto& newPos : std::vector<Vector2f>({
					{ node->pos.x + newDims.x, node->pos.y },
					{ node->pos.x, node->pos.y + newDims.y },
					{ node->pos.x + newDims.x, node->pos.y + newDims.y }
					})) {
					leaves.insert(childNode);
					childNode->next = new QuadNode(newPos, newDims, node);
					childNode = childNode->next;
				}
				leaves.insert(childNode);
			}
		}
		if (node->elementCount < 0) { // if we're not a leaf
			childNode = node->firstChild;
			while (childNode) {
				if (Intersect(element, childNode)) stack.push_back({ childNode, element });
				childNode = childNode->next;
			}
		}
	}
}
void QuadTree::AddElement(QuadElement* element) {
	AddElement(element, root);
}
void QuadTree::AddElement(QuadElement* element, QuadNode* node) {
	std::vector<QuadNode*> stack;
	stack.push_back({ node });
	QuadNode* curNode;
	QuadNode* childNode;
	float x, y, w, h;
	Vector2f newDims;
	while (!stack.empty()) {
		curNode = stack.back();
		stack.pop_back();
		if (curNode->elementCount >= 0) { // if is leaf
			curNode->AddChild(element);
			if (curNode->elementCount > capacity) { // partition
				newDims.x = curNode->dims.x / 2;
				newDims.y = curNode->dims.y / 2;
				leaves.erase(curNode);
				curNode->elementCount = -1;
				for (auto& elem : curNode->elements) 
				childNode = new QuadNode(curNode->pos, newDims, curNode);
				curNode->firstChild = childNode;
				leaves.insert(childNode);
				for (auto& position : std::vector<Vector2f>({
					{curNode->pos.x + newDims.x, curNode->pos.y},
					{curNode->pos.x, curNode->pos.y + newDims.y},
					{curNode->pos.x + newDims.x, curNode->pos.y + newDims.y}
				})) {
					childNode->next = new QuadNode(position, newDims, curNode);
					childNode = childNode->next;
					leaves.insert(childNode);
				}
				for (auto& element : curNode->elements) {
					AddElement(element, curNode);
				}
			}
		}
		// FINISH THIS - need to think of a good way to split and re add elements that have been removed
	}
}

// for any objects that have moved, adjust their position in the tree if necessary
void QuadTree::AdjustTree() {
	std::unordered_set<QuadNode*> toClean;
	// leaves need to know their parent
	std::vector<QuadElement*> displacedElements;
	bool clean;
	QuadNode* tempNode;
	for (auto& leaf : leaves) {
		std::vector<QuadElement*> toDel;
		for (auto& e : leaf->elements) {
			if (!Intersect(e, leaf)) {
				displacedElements.push_back(e);
				toDel.push_back(e);
			}
		}
		for (auto& e : toDel) leaf->elements.Erase(e);
		if (leaf->parent) {
			clean = true;
			tempNode = leaf->parent;
			while (tempNode && clean) {
				if (tempNode->elementCount > 0) clean = false;
				tempNode = tempNode->next;
			}
			if (clean) toClean.insert(leaf->parent);
		}
	}
	FillTree(displacedElements);
	for (auto& node : toClean) {
		clean = true;
		tempNode = node;
		while (tempNode && clean) {
			if (tempNode->elementCount > 0) clean = false;
			tempNode = tempNode->next;
		}
		if (clean) {
			// deallocated child nodes
			node->elementCount = 0;
		}
	}
}