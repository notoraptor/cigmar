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
		typedef container_t::iterator_t iterator_t;
		typedef container_t::const_iterator_t const_iterator_t;
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
				parentNode->addNode(this);
			}
			if (preallocate)
				m_children.resize(max_children, nullptr);
		}
	public:
		explicit Node(Node* parentNode = nullptr, size_t maxChildren = SIZE_MAX, bool preallocate = false):
			Node(parentNode, false, maxChildren, preallocate) {};
		size_t max() const {
			return max_children;
		}
		size_t size() const {return m_children.size();}
		iterator_t begin() {return m_children.begin();}
		iterator_t end() {return m_children.end();}
		const_iterator_t begin() const {return m_children.begin();}
		const_iterator_t end() const {return m_children.end();}
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
		void addNode(Node* child) {
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
		}
		void setChildNode(size_t pos, Node* newChild) {
			if (pos >= m_children.size())
				throw Exception("Node: cannot set a child: index out of bound (", pos + 1, "/", m_children.size(), ")");
			if (m_children[pos]) {
				m_children[pos]->m_parent = nullptr;
				m_children[pos] = nullptr;
			}
			if (newChild) {
				if (newChild->m_parent) {
					newChild->m_parent->m_children.remove(newChild);
					newChild->m_parent = nullptr;
				}
				m_children[pos] = newChild;
				newChild->m_parent = this;
			}
		}
		void removeNode(Node* child) {
			if (child && m_children.remove(child))
				child->m_parent = nullptr;
		}
		void setParentNode(Node* newParent) {
			if (is_root)
				throw Exception("Node: cannot set parent for a root.");
			if (newParent)
				newParent->addNode(this);
			else
				m_parent->removeNode(this);
		}
		Node* getParent() const {
			return m_parent;
		}
		Node* getRoot() {
			return m_parent ? m_parent->getRoot() : this;
		}
		Node* getChild(size_t pos) {
			return m_children[pos];
		}
	};

	template <typename NodeType, typename RootType=NodeType>
	class NodeWrapper: public Node {
		using Node::Node;

		template <typename I>
		struct NodeWrapperIterator {
			I m_iterator;
			NodeWrapperIterator(I iterator): m_iterator(iterator) {}
			bool operator!=(const  NodeWrapperIterator& o) const {return m_iterator != o.m_iterator;}
			NodeWrapperIterator& operator++() {++m_iterator;}
			NodeType& operator*() {return *m_iterator;}
			const NodeType& operator*() const {return *m_iterator;}
		};
	public:

		typedef NodeWrapperIterator<Node::children_t::iterator_t> iterator_t;
		typedef NodeWrapperIterator<Node::const_children_t::iterator_t> const_iterator_t;

		iterator_t begin() {return iterator_t(children().begin());}
		iterator_t end() {return iterator_t(children().end());}
		const_iterator_t begin() const {return const_iterator_t(children().begin());}
		const_iterator_t end() const {return const_iterator_t(children().end());}

		NodeType* parent() const {
			return (NodeType*)getParent();
		}
		RootType* root() {
			return (RootType*)getRoot();
		}
		NodeType* child(size_t pos) {
			return (NodeType*)getChild(pos);
		}
	};
}

#endif //SRC_CIGMAR_TREE_HPP
