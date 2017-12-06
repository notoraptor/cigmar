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

	template <typename Type, typename RootType = Type>
	class Node {
	public:
		typedef ArrayList<Node*> container_t;
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
	private:
		Node* m_parent;
		container_t m_children;
		size_t max_children;
		bool is_root;
	public:
		explicit Node(Node* parentNode = nullptr, bool isRoot = false, size_t maxChildren = SIZE_MAX, bool preallocate = false):
			m_parent(nullptr), m_children(), max_children(maxChildren), is_root(isRoot) {
			if (parentNode) {
				if (is_root)
					throw Exception("Node: a root cannot have a parent.");
				parentNode->add(this);
			}
			if (preallocate)
				m_children.resize(max_children, nullptr);
		}
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
		RootType& root() const {
			return m_parent ? m_parent->root() : dynamic_cast<RootType&>(this);
		}
	};
}

#endif //SRC_CIGMAR_TREE_HPP
