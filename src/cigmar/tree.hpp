//
// Created by notoraptor on 17-12-06.
//

#ifndef SRC_CIGMAR_TREE_HPP
#define SRC_CIGMAR_TREE_HPP

#include <type_traits>
#include <cigmar/interfaces/Collection.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/HashSet.hpp>

namespace cigmar::tree {

	class Node {
	public:
		typedef ArrayList<Node*> container_t;
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
	private:
		Node* m_parent;
		bool is_root;
		size_t max_children;
		container_t m_children;
	protected:
		explicit Node(Node* parentNode, bool isRoot, size_t maxChildren, bool preallocate):
			m_parent(nullptr), is_root(isRoot), max_children(maxChildren), m_children() {
			if (parentNode) {
				if (is_root)
					throw Exception("Node: a root cannot have a parent.");
				parentNode->add(this);
			}
			if (preallocate)
				m_children.resize(max_children, nullptr);
		}
	public:
		explicit Node(Node& parentNode, size_t maxChildren = SIZE_MAX, bool preallocate = false):
			Node(&parentNode, false, maxChildren, preallocate) {};
		explicit Node(Node* parentNode = nullptr, size_t maxChildren = SIZE_MAX, bool preallocate = false):
			Node(parentNode, false, maxChildren, preallocate) {};
		size_t max() const {
			return max_children;
		}
		size_t size() const {
			return m_children.size();
		}
		iterator_t begin() {return m_children.begin();}
		iterator_t end() {return m_children.end();}
		const_iterator_t begin() const {return m_children.begin();}
		const_iterator_t end() const {return m_children.end();}
		Node& add(Node* child) {
			if (child) {
				if (child->is_root)
					throw Exception("Node: a root cannot have a parent.");
				if (hasAncestor(child))
					throw Exception("Node: cannot add an ancestor.");
				if (!m_children.indexOf(child)) {
					if (m_children.size() == max_children)
						throw Exception("Node: reached maximum number of allowed children (", max_children, ").");
					if (child->m_parent) {
						child->m_parent->m_children.remove(child);
						child->m_parent = nullptr;
					}
					m_children.add(child);
					child->m_parent = this;
				}
			}
			return *this;
		}
		Node& remove(Node* child) {
			if (child && m_children.remove(child))
				child->m_parent = nullptr;
			return *this;
		}
		Node& setParent(Node* newParent) {
			if (is_root)
				throw Exception("Node: cannot set parent for a root.");
			if (newParent)
				newParent->add(this);
			else
				m_parent->remove(this);
			return *this;
		}
		Node* parent() const {
			return m_parent;
		}
		Node& root() {
			if (m_parent)
				return m_parent->root();
			return *this;
		}
		bool contains(Node* child) const {
			return (bool)m_children.indexOf(child);
		}
		bool hasAncestor(Node* node) const {
			if (!node)
				return true;
			Node* currentParent = m_parent;
			while (currentParent) {
				if (currentParent == node)
					return true;
				currentParent = currentParent->m_parent;
			}
			return false;
		}
		bool isRoot() const {return !m_parent;};
		bool isLeaf() const {return !m_children;};
		bool isInternal() const {return m_parent && m_children;};
	};

	struct Root: public Node {
		Root(size_t maxChildren = SIZE_MAX, bool preallocate = false): Node(nullptr, true, maxChildren, preallocate) {
			if (!maxChildren)
				throw Exception("Cannot create a root with max children count 0.");
		}
	};

	struct Leaf: public Node {
		explicit Leaf(Node& parentNode): Node(&parentNode, false, 0, false) {}
	};
}

#endif //SRC_CIGMAR_TREE_HPP
