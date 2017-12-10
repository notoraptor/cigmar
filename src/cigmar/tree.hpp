//
// Created by notoraptor on 17-12-06.
//

#ifndef SRC_CIGMAR_TREE_HPP
#define SRC_CIGMAR_TREE_HPP

#include <cstddef>
#include <type_traits>
#include <cigmar/interfaces/Collection.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/interfaces/Streamable.hpp>

namespace cigmar::tree {

	template <typename T>
	class NodeHandler: public Streamable {
		T* ptr;
		size_t* ptr_count;
		bool external;
		void clear() {
			if (ptr) {
				if (*ptr_count)
					--(*ptr_count);
				if (*ptr_count == 0) {
					sys::err::println("Deleting an object", ptr->name());
					delete ptr;
					if (!external) {
						sys::err::println("Deleting internal counter");
						delete ptr_count;
					}
				}
				ptr = nullptr;
				ptr_count = nullptr;
			}
		}
		NodeHandler(T& object, size_t& count, bool isExternal): ptr(&object), ptr_count(&count), external(isExternal) {
			++(*ptr_count);
		}
	public:
		NodeHandler(): ptr(nullptr), ptr_count(nullptr), external(false) {}
		NodeHandler(std::nullptr_t): NodeHandler() {}
		NodeHandler(T& reference): ptr(&reference), ptr_count(nullptr), external(false) {
			ptr_count = new size_t(1);
		};
		NodeHandler(T& reference, size_t& refcount): ptr(&reference), ptr_count(&refcount), external(true) {
			++(*ptr_count);
		}
		NodeHandler(const NodeHandler& copied): ptr(copied.ptr), ptr_count(copied.ptr_count), external(copied.external) {
			if (ptr)
				++(*ptr_count);
		};
		NodeHandler(NodeHandler&& moved): ptr(moved.ptr), ptr_count(moved.ptr_count), external(moved.external) {
			moved.ptr = nullptr;
			moved.ptr_count = nullptr;
			moved.external = false;
		};
		~NodeHandler() {
			clear();
		};
		NodeHandler& operator=(const NodeHandler& copied) {
			clear();
			ptr = copied.ptr;
			ptr_count = copied.ptr_count;
			external = copied.external;
			if (ptr)
				++(*ptr_count);
			return *this;
		};
		NodeHandler& operator=(NodeHandler&& moved) {
			clear();
			ptr = moved.ptr;
			ptr_count = moved.ptr_count;
			external = moved.external;
			moved.ptr = nullptr;
			moved.ptr_count = nullptr;
			moved.external = false;
			return *this;
		};
		NodeHandler& operator=(std::nullptr_t) {
			clear();
			return *this;
		}
		NodeHandler* operator&() = delete;
		template<typename E>
		operator NodeHandler<E>() {
			if (ptr)
				return NodeHandler<E>(dynamic_cast<E&>(*ptr), *ptr_count, external);
			return NodeHandler<E>();
		}
		explicit operator bool() const {return (bool)ptr;}
		bool operator==(const NodeHandler& other) const {return ptr == other.ptr;}
		bool operator!=(const NodeHandler& other) const {return ptr != other.ptr;}
		bool operator==(const T* pointer) const {return ptr == pointer;}
		T* operator->() {return ptr;}
		const T* operator->() const {return ptr;}
		size_t count() const { return ptr ? *ptr_count : 0; }
		void toStream(std::ostream& o) const override {
			if (ptr)
				o << (*ptr);
			else
				o << "nullptr";
		}
	};

	class Node: public Streamable {
	public:
		typedef NodeHandler<Node> handler_t;
		typedef ArrayList<handler_t> container_t;
		typedef container_t::iterator_t iterator_t;
		typedef container_t::const_iterator_t const_iterator_t;
	private:
		String m_name;
		Node* m_parent;
		bool is_root;
		size_t max_children;
		container_t m_children;
	public:
		size_t m_refcount;
	protected:
		explicit Node(const String& name, handler_t parentNode, bool isRoot, size_t maxChildren, bool preallocate):
			m_name(name), m_refcount(0), m_parent(nullptr), is_root(isRoot), max_children(maxChildren), m_children() {
			if (parentNode) {
				if (is_root)
					throw Exception("Node: a root cannot have a parent.");
				parentNode->add(handler_t(*this, m_refcount));
			}
			if (preallocate)
				m_children.resize(max_children, nullptr);
			if (!m_name)
				m_name = String::write('<', this, '>');
		}
	public:
		static handler_t node(const String& name = String(), handler_t parentNode = nullptr, size_t maxChildren = SIZE_MAX, bool preallocate = false) {
			Node* node = new Node(name, parentNode, false, maxChildren, preallocate);
			return handler_t(*node, node->m_refcount);
		}
		size_t max() const {
			return max_children;
		}
		size_t size() const {return m_children.size();}
		const String& name() const {return m_name;}
		iterator_t begin() {return m_children.begin();}
		iterator_t end() {return m_children.end();}
		const_iterator_t begin() const {return m_children.begin();}
		const_iterator_t end() const {return m_children.end();}
		bool contains(handler_t child) const {
			return (bool)m_children.indexOf(child);
		}
		bool hasAncestor(handler_t node) const {
			if (!node)
				return false;
			Node* currentParent = m_parent;
			while (currentParent) {
				if (node == currentParent)
					return true;
				currentParent = currentParent->m_parent;
			}
			return false;
		}
		bool isRoot() const {return !m_parent;};
		bool isLeaf() const {return !m_children;};
		bool isInternal() const {return m_parent && m_children;};
		void add(handler_t child) {
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
		void setChild(size_t pos, handler_t newChild) {
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
		void remove(handler_t child) {
			if (child && m_children.remove(child))
				child->m_parent = nullptr;
		}
		void setParent(handler_t newParent) {
			if (is_root)
				throw Exception("Node: cannot set parent for a root.");
			if (newParent)
				newParent->add(handler_t(*this, m_refcount));
			else
				m_parent->remove(handler_t(*this, m_refcount));
		}
		handler_t getParent() const {
			return m_parent ? handler_t(*m_parent, m_parent->m_refcount) : handler_t();
		}
		handler_t getRoot() {
			return m_parent ? m_parent->getRoot() : handler_t(*this, m_refcount);
		}
		handler_t getChild(size_t pos) {
			return m_children[pos];
		}
		void toStream(std::ostream& o) const override {
			o << m_name;
			if (m_children) {
				size_t count = 0;
				o << " (";
				for (auto x: m_children) {
					if (count) o << ", ";
					o << x;
					++count;
				}
				o << ")";
			}
		}
	};
}

#endif //SRC_CIGMAR_TREE_HPP
