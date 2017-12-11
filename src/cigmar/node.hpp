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

	class Content {
		template <typename T> friend class Node;
		typedef Node<Content> node_t;
		typedef ArrayList<node_t> container_t;
		size_t refcount;
		String m_name;
		Content* m_parent;
		container_t children;
		void completeInit();
	protected:
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
		Content(const String& nodeName, Content* nodeParent): refcount(0), m_name(nodeName), m_parent(nodeParent), children() {
			completeInit();
		};
		Content(const char* nodeName, Content* nodeParent): refcount(0), m_name(nodeName), m_parent(nodeParent), children() {
			completeInit();
		}
		Content(const char* nodeName, node_t nodeParent);
		node_t getParent() const;
		node_t getRoot();
		node_t getChild(size_t position);
		void addNode(node_t child);
		void removeNode(node_t child);
		void removeNode(size_t position);
		void setChildNode(size_t position, node_t child);
		bool containsNode(node_t node) const;
		bool hasAncestorNode(node_t node) const;
		iterator_t beginChildren() { return children.begin(); };
		iterator_t endChildren() { return children.end(); };
		const_iterator_t beginChildren() const { return children.begin(); };
		const_iterator_t endChildren() const { return children.end(); };
	public:
		virtual size_t maxChildren() const {return std::numeric_limits<size_t>::max();};
		virtual bool forceRoot() const {return false;};
		virtual bool preallocate() const {return false;};
		bool isRoot() const { return !m_parent; };
		bool isLeaf() const { return !children; };
		bool isInternal() const { return m_parent && children; };
		size_t size() const { return children.size(); };
		const String& name() const { return m_name; };
		void setName(const String& name) {
			m_name = name;
			if (!m_name)
				m_name = String::write('<', this, '>');
		}
	};

	template <typename T> class Node: public Streamable {
		friend class Content;
		template <typename E> friend class Node;
		Content* internal;
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
		Content* get() {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		const Content* get() const {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		Node(std::nullptr_t): internal(nullptr) {};
		Node(Content& internalNode): internal(&internalNode) {
			++internal->refcount;
		};
	public:
		template <typename... Args>
		explicit Node(Args&&... contentArgs) {
			internal = new T(std::forward<Args>(contentArgs)...);
			++internal->refcount;
		}
		Node(const Node& copied): internal(copied.internal) {
			if (internal)
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
			if (internal)
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
		T* operator->() { return &dynamic_cast<T&>(*get()); };
		const T* operator->() const { return &dynamic_cast<T&>(*get()); };
		size_t count() const {return get()->refcount;}
		void toStream(std::ostream& o) const override {
			const Content* pointer = get();
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
			return internal == other.internal;
		};
		template <typename E>
		operator Node<E>() const {
			return Node<E>(*internal);
		}
		size_t s() const {return sizeof(T);}
	};

	Content::Content(const char* nodeName, node_t nodeParent): Content(nodeName, nodeParent.internal) {};

	void Content::completeInit() {
		if (m_parent) {
			if (forceRoot())
				throw Exception("Node: cannot add a parent to a root.");
			m_parent->addNode(node_t(*this));
		}
		if (preallocate())
			children.resize(maxChildren(), nullptr);
		if (!m_name)
			m_name = String::write('<', this, '>');
	};

	Content::node_t Content::getParent() const {
		return m_parent ? node_t(*m_parent): node_t(nullptr);
	};

	Content::node_t Content::getRoot() {
		Content* pointer = this;
		while (pointer->m_parent)
			pointer = pointer->m_parent;
		return node_t(*pointer);
	};

	Content::node_t Content::getChild(size_t position) {
		if (position >= children.size())
			throw Exception("Node: child position out of bounds (", position, " not in [0; ", children.size() - 1 , "]).");
		return children[position];
	};

	void Content::addNode(node_t child) {
		if (child) {
			if (child.internal->forceRoot())
				throw Exception("Node: a root cannot have a parent.");
			if (hasAncestorNode(child))
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

	void Content::removeNode(node_t child) {
		if (child && children.remove(child))
			child.internal->m_parent = nullptr;
	};

	void Content::removeNode(size_t position) {
		if (position < children.size()) {
			children[position].internal->m_parent = nullptr;
			children.remove(position);
		}
	};

	void Content::setChildNode(size_t position, node_t child) {
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

	bool Content::containsNode(node_t node) const {
		return node ? (bool)children.indexOf(node) : false;
	};

	bool Content::hasAncestorNode(node_t node) const {
		if (node) {
			Content* currentParent = m_parent;
			while (currentParent) {
				if (currentParent == node.internal)
					return true;
				currentParent = currentParent->m_parent;
			}
		}
		return false;
	};

	template <typename T>
	class TemplateContent: public Content {
		typedef Node<T> template_node_t;
	private:
		template <typename E>
		class Iterator {
			E i;
		public:
			explicit Iterator(const E& iterator): i(iterator) {}
			Iterator& operator++() {++i; return *this;}
			bool operator==(const Iterator& o) const {return i == o.i;}
			bool operator!=(const Iterator& o) const {return i != o.i;}
			template_node_t operator*() {return *i;}
		};
	public:
		typedef Iterator<Content::iterator_t> iterator_t;
		typedef Iterator<Content::const_iterator_t> const_iterator_t;
		TemplateContent(const String& name, T* parent): Content(name, parent) {}
		TemplateContent(const char* name, T* parent): Content(name, parent) {}
		TemplateContent(const char* name, template_node_t parent): Content(name, parent) {}
		template_node_t parent() const {return getParent();}
		template_node_t root() {return getRoot();}
		template_node_t child(size_t position) {return getChild(position);}
		void add(template_node_t child) {addNode(child);}
		void remove(template_node_t child) {removeNode(child);}
		void remove(size_t position) {removeNode(position);}
		void setChild(size_t position, template_node_t child) {setChildNode(position, child);}
		bool contains(template_node_t node) const {return containsNode(node);}
		bool hasAncestor(template_node_t node) const {return hasAncestorNode(node);}
		iterator_t begin() {return iterator_t(beginChildren());}
		iterator_t end() {return iterator_t(endChildren());}
		const_iterator_t begin() const {return const_iterator_t(beginChildren());}
		const_iterator_t end() const {return const_iterator_t(endChildren());}
	};

}

#endif //SRC_CIGMAR_NODE_HPP
