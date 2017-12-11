//
// Created by HPPC on 10/12/2017.
//

#ifndef SRC_CIGMAR_NODE_HPP
#define SRC_CIGMAR_NODE_HPP

#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::tree {
	template <typename T> class Node;

	template <typename Type, typename RootType=Type, typename ParentType=Type, typename ChildType=Type>
	class Content {
		friend class Node<Type>;
		typedef Node<Type> node_t;
		typedef Node<RootType> root_t;
		typedef Node<ParentType> parent_t;
		typedef Node<ChildType> child_t;
		typedef ArrayList<child_t> container_t;
		size_t refcount;
		String m_name;
		Content<ParentType>* m_parent;
		container_t children;
	public:
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
		Content(const String& nodeName, const parent_t& nodeParent): refcount(0), m_name(nodeName), m_parent(nodeParent.internal), children() {
			if (m_parent) {
				if (forceRoot())
					throw Exception("Node: cannot add a parent to a root.");
				m_parent->add(node_t(*this));
			}
			if (preallocate())
				children.resize(maxChildren(), nullptr);
			if (!m_name)
				m_name = String::write('<', this, '>');
		};
		Content(): Content(String(), nullptr) {}
		iterator_t begin() { return children.begin(); };
		iterator_t end() { return children.end(); };
		const_iterator_t begin() const { return children.begin(); };
		const_iterator_t end() const { return children.end(); };
		size_t size() const { return children.size(); };
		virtual size_t maxChildren() const {return std::numeric_limits<size_t>::max();};
		virtual bool forceRoot() const {return false;};
		virtual bool preallocate() const {return false;};
		bool isRoot() const { return !m_parent; };
		bool isLeaf() const { return !children; };
		bool isInternal() const { return m_parent && children; };
		bool contains(const child_t& node) const {
			return node ? (bool)children.indexOf(node) : false;
		};
		bool hasAncestor(const parent_t& node) const {
			if (node) {
				Content<ParentType>* currentParent = m_parent;
				while (currentParent) {
					if (currentParent == node.internal)
						return true;
					currentParent = currentParent->m_parent;
				}
			}
			return false;
		};
		parent_t parent() const { return m_parent ? parent_t(*m_parent): parent_t(nullptr); };
		root_t root() {
			Content* pointer = this;
			while (pointer->m_parent)
				pointer = pointer->m_parent;
			return root_t(*pointer);
		};
		child_t child(size_t position) const {
			if (position >= children.size())
				throw Exception("Node: child position out of bounds (", position, " not in [0; ", children.size() - 1 , "]).");
			return children[position];
		};
		const String& name() const { return m_name; };
		void add(const child_t& child) {
			if (child) {
				if (child.internal->forceRoot())
					throw Exception("Node: a root cannot have a parent.");
				if (hasAncestor(child))
					throw Exception("Node: cannot add an ancestor.");
				if (!children.indexOf(child)) {
					if (maxChildren() == children.size())
						throw Exception("Node: reached maximum number of allowed children (", maxChildren(), ").");
					if (child.internal->m_parent) {
						child.internal->m_parent->children.remove(child);
						child.internal->m_parent = nullptr;
					}
					children.add(child);
					child.internal->m_parent = this;
				}
			}
		};
		void remove(const child_t& child) {
			if (child && children.remove(child))
				child.internal->m_parent = nullptr;
		};
		void remove(size_t position) {
			if (position < children.size()) {
				children[position].internal->m_parent = nullptr;
				children.remove(position);
			}
		};
		void setChild(size_t position, const child_t& child) {
			if (position >= children.size())
				throw Exception("Node: cannot set a child: index out of bound (", position + 1, "/", children.size(), ")");
			if (children[position]) {
				children[position].internal->m_parent = nullptr;
				children[position] = nullptr;
			}
			if (child) {
				if (child.internal->m_parent) {
					child.internal->m_parent->children.remove(child);
					child.internal->m_parent = nullptr;
				}
				children[position] = child;
				child.internal->m_parent = this;
			}
		};
	};

	template <typename T> class Node: public Streamable {
		friend class Content<T>;
		Content<T>* internal;
		void clear() {
			if (internal) {
				if (internal->refcount)
					decrement();
				if (!internal->refcount) {
					std::cerr << "Deleting: " << internal->name() << std::endl;
					delete internal;
				}
				internal = nullptr;
			}
		}
		Content<T>* get() {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		const Content<T>* get() const {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		void increment() {
			++internal->refcount;
			// std::cerr << '+' << internal->name() << ':' << internal->refcount << std::endl;
		}
		void decrement() {
			--internal->refcount;
			// std::cerr << '-' << internal->name() << ':' << internal->refcount << std::endl;
		}
		Node(std::nullptr_t): internal(nullptr) {};
		explicit Node(Content<T>& otherInternal): internal(&otherInternal) {
			increment();
		};
	public:
		template <typename... Args>
		explicit Node(Args&&... contentArgs) {
			internal = new T(std::forward<Args>(contentArgs)...);
			increment();
		}
		Node(const Node& copied): internal(copied.internal) {
			if (internal)
				increment();
		}
		Node(Node&& moved) noexcept : internal(moved.internal) {
			moved.internal = nullptr;
		}
		~Node() {
			clear();
		}
		Node& operator=(const Node& copied) {
			clear();
			internal = copied.internal;
			if (internal)
				increment();
			return *this;
		}
		Node& operator=(Node&& moved) noexcept {
			clear();
			internal = moved.internal;
			moved.internal = nullptr;
			return *this;
		}
		Node& operator=(std::nullptr_t) {
			clear();
			return *this;
		}
		explicit operator bool() const {return (bool)internal;}
		T* operator->() { return &dynamic_cast<T&>(*get()); };
		const T* operator->() const { return &dynamic_cast<T&>(*get()); };
		bool operator==(const Node& other) const { return internal == other.internal; };
		size_t refcount() const {return get()->refcount;}
		void toStream(std::ostream& o) const override {
			const Content<T>* pointer = get();
			o << pointer->m_name;
			if (pointer->children) {
				o << " (";
				size_t count = 0;
				for (auto& x: pointer->children) {
					if (count) o << ", ";
					o << x;
					++count;
				}
				o << ')';
			}
		};
		size_t typesize() const {return sizeof(T);}
	};

}

#endif //SRC_CIGMAR_NODE_HPP
