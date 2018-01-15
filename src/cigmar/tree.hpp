//
// Created by HPPC on 10/12/2017.
//

#ifndef SRC_CIGMAR_NODE_HPP
#define SRC_CIGMAR_NODE_HPP

#include <cigmar/classes/String.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/exception/Exception.hpp>

namespace cigmar::tree {
	template <typename NodeType, typename RootType = NodeType>
	class Node {
		String node_name;
		Node* node_parent;
		ArrayList<NodeType*> node_children;
	public:
		typedef typename ArrayList<NodeType*>::iterator_t iterator_t;
		typedef typename ArrayList<NodeType*>::const_iterator_t const_iterator_t;
		Node(): node_name(), node_parent(nullptr), node_children() {
			if (min_children() > max_children())
				throw Exception("Node: expected min_children() < max_children().");
			if (preallocate() < min_children() || preallocate() > max_children())
				throw Exception("Node: expected min_children() <= preallocate() <= max_children().");
			if (preallocate())
				node_children.resize(preallocate(), nullptr);
		}
		Node(const Node&) = delete;
		Node(Node&&) noexcept = default;
		~Node() {
			std::cerr << "Destroying node " << node_name << std::endl;
			for (NodeType* node: node_children) if (node) delete node;
		}
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		virtual void run() {}
		virtual void with_node_added(NodeType* node, size_t position) {}
		virtual void with_node_replaced(NodeType* from, NodeType* to, size_t position) {}
		virtual void with_node_removed(NodeType* node, size_t position) {}
		virtual size_t min_children() const {return 0;};
		virtual size_t max_children() const {return std::numeric_limits<size_t>::max();};
		virtual size_t preallocate() const {return min_children();};
		virtual bool force_root() const {return false;};
		iterator_t begin() {return node_children.begin();}
		iterator_t end() {return node_children.end();}
		const_iterator_t begin() const {return node_children.begin();}
		const_iterator_t end() const {return node_children.end();}
		size_t size() const {return node_children.size();}
		bool isRoot() const {return !node_parent;}
		bool isLeaf() const {return !node_children;}
		bool isInternal() const {return node_parent && node_children;}
		bool hasAncestor(NodeType* node) const {
			if (node) {
				auto current_parent = node_parent;
				while(current_parent) {
					if (current_parent == node)
						return true;
					current_parent = current_parent->node_parent;
				}
			}
			return false;
		}
		const String& name() const {return node_name;}
		pos_t indexOf(const NodeType* node) const {
			return node ? node_children.indexOf(node) : pos_t();
		}
		const RootType* root() const {
			const Node* current = this;
			while (current->node_parent)
				current = current->node_parent;
			return &dynamic_cast<const RootType&>(*current);
		}
		const NodeType* parent() const {return &dynamic_cast<const NodeType&>(*node_parent);}
		const NodeType* child(size_t position) const {
			return node_children[position];
		}
		RootType* root() {
			Node* current = this;
			while (current->node_parent)
				current = current->node_parent;
			return &dynamic_cast<RootType&>(*current);
		}
		NodeType* parent() {return &dynamic_cast<NodeType&>(*node_parent);}
		NodeType* child(size_t position) {
			return node_children[position];
		}
		void setName(const String& newName) {node_name = newName;};
		void switchPositions(size_t posChild1, size_t posChild2) {
			node_children.switchPosition(posChild1, posChild2);
		};
		void moveUp(size_t childPosition, size_t offset) {
			node_children.moveUp(childPosition, offset);
		};
		void moveDown(size_t childPosition, size_t offset) {
			node_children.moveDown(childPosition, offset);
		};
		void moveTop(size_t childPosition) {
			node_children.moveUp(childPosition, childPosition);
		};
		void moveBottom(size_t childPosition) {
			node_children.moveDown(childPosition, node_children.size());
		};
		void add(NodeType* node) {
			if (node && node->node_parent != this) {
				if (max_children() == node_children.size())
					throw Exception("Node: reached maximum number (", max_children(), ") of allowed children.");
				if (node->force_root())
					throw Exception("Node: cannot set parent for a forced root.");
				if (hasAncestor(node))
					throw Exception("Node: cannot add a child that is already an ancestor.");
				if (node->node_parent)
					node->node_parent->remove(node);
				node_children.add(node);
				node->node_parent = this;
				with_node_added(node, node_children.size() - 1);
			}
		}
		NodeType* setChild(size_t position, NodeType* newNode) {
			if (position >= node_children.size())
				throw Exception("Node: cannot set child for an index out of bond (", position + 1, "/", node_children.size(), ").");
			NodeType* oldNode = nullptr;
			if (node_children[position]) {
				oldNode = node_children[position];
				oldNode->node_parent = nullptr;
				node_children[position] = nullptr;
			}
			if (newNode) {
				if (newNode->node_parent)
					newNode->node_parent->remove(newNode);
				node_children[position] = newNode;
				newNode->node_parent = this;
			}
			with_node_replaced(oldNode, newNode, position);
			return oldNode;
		}
		void remove(NodeType* node) {
			if (node && node->node_parent == this) {
				if (min_children() == node_children.size())
					throw Exception("Node: reached minimum number (", min_children(), ") of allowed children.");
				pos_t removedPosition = node_children.remove(node);
				node->node_parent = nullptr;
				with_node_removed(node, (size_t)removedPosition);
			}
		}
		void remove(size_t position) {
			if (position < node_children.size()) {
				if (min_children() == node_children.size())
					throw Exception("Node: reached minimum number (", min_children(), ") of allowed children.");
				if (node_children[position]) {
					node_children[position]->node_parent = nullptr;
					with_node_removed(node_children[position], position);
					delete node_children[position];
					node_children[position] = nullptr;
				}
				node_children.remove(position);
			}
		}
		NodeType* snatch(size_t position) {
			// Remove without deleting.
			NodeType* node = nullptr;
			if (position < node_children.size()) {
				if (min_children() == node_children.size())
					throw Exception("Node: reached minimum number (", min_children(), ") of allowed children.");
				if (node_children[position]) {
					node_children[position]->node_parent = nullptr;
					node = node_children[position];
					with_node_removed(node, position);
				}
				node_children.remove(position);
			}
			return node;
		}
	};

	template <typename NodeType, typename RootType>
	class Tree {
		RootType* root_node;
	public:
		explicit Tree(Node<NodeType, RootType>* node): root_node(nullptr) {
			if (node) root_node = node->root();
		}
		Tree(const Tree&) = delete;
		Tree(Tree&& moved) noexcept : root_node(moved.root_node) {
			moved.root_node = nullptr;
		}
		~Tree() {
			if (root_node) delete root_node;
		}
		Tree& operator=(const Tree&) = delete;
		Tree& operator=(Tree&& moved) noexcept {
			root_node = moved.root_node;
			moved.root_node = nullptr;
		}
		void operator()() {
			if (root_node) root_node->run();
		}
	};

	template <typename N, typename R>
	void tree(Node<N, R>* node) {
		Tree<N, R>{node}();
	};

	template <typename N, typename R>
	inline std::ostream& operator<<(std::ostream& o, const Node<N, R>* node) {
		if (node->name())
			o << node->name();
		else
			o << (void*)node;
		if (node->size()) {
			o << " (";
			auto it = node->begin(), end = node->end();
			o << *it;
			++it;
			while (it != end) {
				o << ", " << *it;
				++it;
			}
			o << ')';
		}
	};
}

#endif //SRC_CIGMAR_NODE_HPP
