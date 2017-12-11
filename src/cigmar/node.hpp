//
// Created by HPPC on 10/12/2017.
//

#ifndef SRC_CIGMAR_NODE_HPP
#define SRC_CIGMAR_NODE_HPP

#include <type_traits>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::node {
	template <typename T> class Node;
	struct Content;

	template <typename T> class Internal {
		friend class Node<T>;
		friend class Content;
		static_assert(std::is_base_of<Content, T>{}, ""); // TODO: error message.
		typedef Node<T> node_t;
		typedef ArrayList<node_t> container_t;
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
		size_t refcount;
		T* content;
		Content* ptr_content;
		String m_name;
		Internal* m_parent;
		container_t children;
		template <typename... Args>
		Internal(const String& nodeName, Internal* nodeParent, Args&&... contentArgs):
				refcount(0), m_name(nodeName), m_parent(nodeParent), children() {
			content = new T(std::forward<Args>(contentArgs)...);
			ptr_content = content;
			setPtrContent();
			if (!m_name)
				m_name = String::write('<', this, '>');
		};
		~Internal() {
			delete content;
		}
		void setPtrContent();
		size_t maxChildren() const;
		bool forceRoot() const;
		iterator_t begin() { return children.begin(); };
		iterator_t end() { return children.end(); };
		const_iterator_t begin() const { return children.begin(); };
		const_iterator_t end() const { return children.end(); };
		size_t size() const { return children.size(); };
		const String& name() const { return m_name; };
		virtual bool isRoot() const { return !m_parent; };
		bool isLeaf() const { return !children; };
		bool isInternal() const { return m_parent && children; };
		bool contains(node_t node) const {
			return node ? (bool)children.indexOf(node) : false;
		};
		bool hasAncestor(node_t node) const {
			if (node) {
				Internal* currentParent = m_parent;
				while (currentParent) {
					if (currentParent == node.internal)
						return true;
					currentParent = currentParent->m_parent;
				}
			}
			return false;
		};
		node_t parent() const {
			return m_parent ? node_t(*m_parent): node_t(nullptr);
		};
		node_t root() {
			Internal* pointer = this;
			while (pointer->m_parent)
				pointer = pointer->m_parent;
			return node_t(*pointer);
		};
		node_t child(size_t position) {
			if (position >= children.size())
				throw Exception("Node: child position out of bounds (", position, " not in [0; ", children.size() - 1 , "]).");
			return children[position];
		};
		void add(node_t child) {
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
		void remove(node_t child) {
			if (child && children.remove(child))
				child.internal->m_parent = nullptr;
		};
		void remove(size_t position) {
			if (position < children.size()) {
				children[position].internal->m_parent = nullptr;
				children.remove(position);
			}
		};
		void setChild(size_t position, node_t child) {
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
		template <typename E>
		operator Internal<E>&() const {
			return *(Internal<E>*)(this);
		}
	};

	template <typename T> class Node: public Streamable {
		friend class Content;
		friend class Internal<T>;
		template <typename E> friend class Node;
		typedef Internal<T> internal_t;
		internal_t* internal;
		void clear() {
			if (internal) {
				if (internal->refcount)
					--internal->refcount;
				if (!internal->refcount) {
					std::cerr << "Deleting: " << internal->name() << std::endl;
					delete internal;
				}
				internal = nullptr;
			}
		}
		internal_t* get() {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		const internal_t* get() const {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		Node(std::nullptr_t): internal(nullptr) {};
		Node(internal_t& internalNode): internal(&internalNode) {
			++internal->refcount;
		};
	public:
		template <typename... Args>
		Node(const String& name, Node parent, Args&&... contentArgs) {
			internal = new internal_t(name, parent.internal, std::forward<Args>(contentArgs)...);
			++internal->refcount;
		}
		template <typename... Args>
		explicit Node(const String& name, Args&&... contentArgs) {
			internal = new internal_t(name, nullptr, std::forward<Args>(contentArgs)...);
			++internal->refcount;
		}
		Node(const String& name, std::nullptr_t) {
			internal = new internal_t(name, nullptr);
			++internal->refcount;
		}
		Node(const Node& copied): internal(copied.internal) {
			++internal->refcount;
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
			++internal->refcount;
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
		Node* operator&() = delete;
		explicit operator bool() const {return (bool)internal;}
		T* operator->() { return get()->content; };
		const T* operator->() const { return get()->content; };
		size_t count() const {return get()->refcount;}
		void toStream(std::ostream& o) const override {
			const internal_t* pointer = get();
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
		template <typename E>
		bool operator==(const Node<E>& other) const {
			return (void*)internal == (void*)other.internal;
		};
		template <typename E>
		operator Node<E>() const {
			std::cerr << "Converting 1" << std::endl;
			return Node<E>((Internal<E>&)(*internal));
		}
	};

	struct Content {
		template <typename E>
		friend class Internal;
		typedef Content default_type;
		typedef Internal<default_type> internal_node_t;
		typedef Node<default_type> node_t;
		typedef typename internal_node_t::iterator_t iterator_t;
		typedef typename internal_node_t::const_iterator_t const_iterator_t;
		virtual size_t maxChildren() const {return std::numeric_limits<size_t>::max();};
		virtual bool forceRoot() const {return false;};
		virtual bool preallocate() const {return false;};
		bool isRoot() const { return node->isRoot(); };
		bool isLeaf() const { return node->isLeaf(); };
		bool isInternal() const { return node->isInternal(); };
		const String& name() const { return node->name(); };
		iterator_t begin() { return node->begin(); };
		iterator_t end() { return node->end(); };
		const_iterator_t begin() const { return node->begin(); };
		const_iterator_t end() const { return node->end(); };
		size_t size() const { return node->size(); };
		node_t parent() const { return node->parent(); };
		node_t root() const { return node->root(); };
		node_t child(size_t position) { return node->child(position); };
		void add(node_t child) { node->add(child); };
		void remove(node_t child) { node->remove(child); };
		void remove(size_t position) { node->remove(position); };
		void setChild(size_t position, node_t child) { node->setChild(position, child); };
	private:
		internal_node_t* node;
	};

	template <typename T>
	size_t Internal<T>::maxChildren() const {
		return ptr_content->maxChildren();
	};

	template <typename T>
	bool Internal<T>::forceRoot() const {
		return ptr_content->forceRoot();
	};

	template <typename T>
	void Internal<T>::setPtrContent() {
		std::cerr << "Converting 2" << std::endl;
		ptr_content->node = &(Content::internal_node_t&)(*this);
		std::cerr << "Converting 3" << std::endl;
		if (m_parent) {
			if (forceRoot())
				throw Exception("Node: cannot add a parent to a root.");
			m_parent->ptr_content->add(node_t(*this));
		}
		if (ptr_content->preallocate())
			children.resize(maxChildren(), nullptr);
	};

}

#endif //SRC_CIGMAR_NODE_HPP
