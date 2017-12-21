//
// Created by HPPC on 10/12/2017.
//

#ifndef SRC_CIGMAR_NODE_HPP
#define SRC_CIGMAR_NODE_HPP

#include <cigmar/classes/String.hpp>
#include <cigmar/exception/Exception.hpp>

namespace cigmar::tree {
	template <typename T> class Node;

	extern size_t nodes_count;

	template <typename Type, typename RootType=Type, typename ParentType=Type, typename ChildType=Type>
	class Content {
	public:
		typedef Content<Type, RootType, ParentType, ChildType> content_t;
		typedef Node<Type> node_t;
		typedef Node<RootType> root_t;
		typedef Node<ParentType> parent_t;
		typedef Node<ChildType> child_t;
		typedef ArrayList<child_t> container_t;
		friend node_t;
		friend root_t;
		friend parent_t;
		friend child_t;
	private:
		size_t refcount;
		String m_name;
		Content<ParentType, RootType, ParentType, Type>* m_parent;
		container_t children;
	public:
		typedef typename container_t::iterator_t iterator_t;
		typedef typename container_t::const_iterator_t const_iterator_t;
		Content(const String& nodeName, const parent_t& nodeParent):
				refcount(0), m_name(nodeName), m_parent(nullptr), children() {
			if (minChildren() > maxChildren())
				throw Exception("Node: minimum number of children must be <= maximum number of children.");
			if (minChildren() && !preallocate())
				throw Exception("Node: a minimum number of children is required, so children container must be preallocated.");
			if (nodeParent) {
				if (forceRoot())
					throw Exception("Node: cannot add a parent to a root.");
				nodeParent.internal->add(node_t(*this));
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
		virtual size_t minChildren() const {return 0;}
		virtual size_t maxChildren() const {return std::numeric_limits<size_t>::max();};
		virtual bool forceRoot() const {return false;};
		virtual bool preallocate() const {return false;};
		bool isRoot() const { return !m_parent; };
		bool isLeaf() const { return !children; };
		bool isInternal() const { return m_parent && children; };
		bool contains(const child_t& node) const {
			return node && node.internal->m_parent == this;
		};
		bool hasAncestor(const parent_t& node) const {
			if (node) {
				auto* currentParent = m_parent;
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
		pos_t indexOf(const child_t& child) const {
			return child ? pos_t() : children.indexOf(child);
		}
		void switchPositions(size_t posChild1, size_t posChild2) {
			children.switchPosition(posChild1, posChild2);
		};
		void moveUp(size_t childPosition, size_t offset) {
			children.moveUp(childPosition, offset);
		};
		void moveDown(size_t childPosition, size_t offset) {
			children.moveDown(childPosition, offset);
		};
		void moveTop(size_t childPosition) {
			children.moveUp(childPosition, childPosition);
		};
		void moveBottom(size_t childPosition) {
			children.moveDown(childPosition, children.size());
		};
		void add(const child_t& child) {
			if (child) {
				if (child.internal->forceRoot())
					throw Exception("Node: a root cannot have a parent.");
				if (hasAncestor(child))
					throw Exception("Node: cannot add an ancestor.");
				if (child.internal->m_parent != this) {
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
			if (child) {
				if (minChildren() == children.size())
					throw Exception("Node: reached minimum number of allowed children (", minChildren(), ").");
				if (children.remove(child))
					child.internal->m_parent = nullptr;
			}
		};
		void remove(size_t position) {
			if (position < children.size()) {
				if (minChildren() == children.size())
					throw Exception("Node: reached minimum number of allowed children (", minChildren(), ").");
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
		node_t node() {return node_t(*this);}
	};

	template <typename ContentType>
	class Node: public Streamable {
		typedef typename ContentType::content_t content_t;
		friend content_t;
		template <typename E> friend class Node;
		content_t* internal;
		void clear() {
			if (internal) {
				if (internal->refcount)
					decrement();
				if (!internal->refcount) {
					std::cerr << "Deleting: " << internal->name() << std::endl;
					delete internal;
					--nodes_count;
				}
				internal = nullptr;
			}
		}
		content_t* get() {
			if (!internal) throw Exception("Node: cannot dereference a null content.");
			return internal;
		}
		const content_t* get() const {
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
		explicit Node(content_t& otherInternal): internal(&otherInternal) {
			increment();
		};
	public:
		template <typename... Args>
		explicit Node(Args&&... contentArgs) {
			internal = new ContentType(std::forward<Args>(contentArgs)...);
			increment();
			++nodes_count;
		}
		Node(std::nullptr_t): internal(nullptr) {};
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
		ContentType* operator->() { return &dynamic_cast<ContentType&>(*get()); };
		const ContentType* operator->() const { return &dynamic_cast<ContentType&>(*get()); };
		bool operator==(const Node& other) const { return internal == other.internal; };
		template <typename E>
		bool operator!=(const Node<E>& other) const { return internal != other.internal; };
		size_t refcount() const {return get()->refcount;}
		void toStream(std::ostream& o) const override {
			const content_t* pointer = get();
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
		size_t typesize() const {return sizeof(ContentType);}
	};

}

#endif //SRC_CIGMAR_NODE_HPP
