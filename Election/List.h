#pragma once
#include <iostream>
#include <fstream>
using namespace std;
namespace electionSystem {
	template<class T>
	class List
	{
		class Node
		{
		public:
			T info;
			Node* next;
			Node* prev;
			Node(const T& info, Node* next, Node* prev) :info(info), next(next), prev(prev) {}
		};
	private:
		Node* head;
		Node* tail;
		int len;
		Node* createNewNode(const T& info, Node* next, Node* prev)
		{
			try 
			{ 
				Node* newNode = new Node{ info, next, prev };
				return newNode;
			}
			catch (bad_alloc& ex) { throw ex; }
			
		}
	public:
		List() :head(nullptr), tail(nullptr), len(0) {}
		~List()
		{
			this->clear();
		}
	public:
		class iterator
		{
		private:
			Node* node;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			iterator(Node* node) : node(node) {}
			iterator(const iterator& other) : node(other.node) {}

			friend class const_iterator;

			const iterator& operator=(const iterator& other)
			{
				node = other.node;
				return *this;
			}
			bool operator==(const iterator& other) const
			{
				return (node == other.node);
			}
			bool operator!=(const iterator& other) const
			{
				return !(*this == other.node);
			}
			T operator*()
			{
				return node->info;
			}
			T* operator->()
			{
				return &(node->info);
			}
			iterator& operator++() {
				this->node = this->node->next;
				return *this;
			}
			iterator operator++(int)
			{
				iterator temp(*this);
				this->node = this->node->next;
				return temp;
			}
			iterator& operator--()
			{
				this->node = this->node->prev;
				return *this;
			}
			iterator operator--(int)
			{
				iterator temp(*this);
				this->node = this->node->prev;
				return temp;
			}
		};
		class const_iterator
		{
		private:
			const Node* node;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = Node;
			using pointer = Node*;
			using reference = Node&;

			const_iterator(const Node* node) :node(node) {}
			const_iterator(const iterator& other) :node(other.node) {}
			const_iterator(const_iterator& other) :node(other.node) {}
			const_iterator& operator=(const iterator& other)
			{
				this->node = other.node;
				return *this;
			}
			const_iterator& operator=(const_iterator& other)
			{
				this->node = other.node;
				return *this;
			}
			bool operator==(const_iterator& other) const
			{
				return (node == other.node);
			}
			bool operator!=(const_iterator& other) const
			{
				return !(*this == other);
			}
			const T& operator*()
			{
				return *node;
			}
			const T* operator->()
			{
				return node;
			}
			const_iterator& operator++() {
				this->node = this->node->next;
				return *this;
			}
			const_iterator operator++(int)
			{
				iterator temp(*this);
				this->node = this->node->next;
				return temp;
			}
			const_iterator& operator--()
			{
				this->node = this->node->prev;
				return *this;
			}
			const_iterator operator--(int)
			{
				iterator temp(*this);
				this->node = this->node->prev;
				return temp;
			}
		};

		iterator insert(const iterator& pos, const T& info)
		{
			Node* newNode = createNewNode(info, pos.node->next, pos.node);

			if (pos.node->next == nullptr)//insert to tail
			{
				push_back(info);
			}
			else //insert to head
			{
				push_front(info);
			}
			return iterator(newNode);
		}
		const iterator& erase(const iterator& itr)
		{
			if (itr.node == this->head)
			{
				pop_front();
				return iterator(this->head);
			}
			else if (itr.node == this->tail)
			{
				pop_back();
				return iterator(this->tail);
			}
			else
			{
				Node* curr = itr.node;
				Node* temp = curr.next;
				curr->prev->next = temp;
				curr->next->prev = curr->prev;
				delete curr;
				return iterator(temp);
			}
		}
		const iterator& erase(const iterator& first, const iterator& last)
		{
			iterator& temp = first;
			for (auto curr = first; curr != last; ++curr)
			{
				temp = erase(curr);
			}
			return temp;
		}
		iterator begin()const
		{
			return iterator(this->head);
		}
		iterator end()const
		{
			return iterator(nullptr);
		}
		iterator ByIndex(int index)const
		{
			iterator itr(this->head);
			for (int i = 1; i < index; i++)
			{
				itr++;
			}
			return itr;
		}
		const iterator cbegin()const
		{
			return this->begin();
		}
		const iterator cend()const
		{
			return this->end();
		}
		List(const List& lst)
		{
			*this = lst;
		}
		Node* getHead()const
		{
			return this->head;
		}
		Node* getTail()const
		{
			return this->tail;
		}
		void push_back(const T& info)
		{
			Node* newNode = createNewNode(info, nullptr, this->tail);
			if (empty())
			{
				this->tail = newNode;
				this->head = newNode;
			}
			else
			{
				this->tail->next = newNode;
				this->tail = newNode;
			}
			(this->len)++;
		}
	
		void push_front(const T& info)
		{
			Node* newNode = createNewNode(info, this->head, nullptr);
			if (empty())
			{
				this->tail = newNode;
				this->head = newNode;
			}
			else
			{
				this->head->prev = newNode;
				this->head = newNode;
			}
			(this->len)++;
		}
		void pop_back()
		{
			Node* curr = this->tail;
			if (!empty())
			{
				if (this->len == 1)
				{
					delete curr;
					this->len = 0;
					this->head = nullptr;
					this->tail = nullptr;
				}
				else
				{
					this->tail = curr->prev;
					delete curr;
					this->tail->next = nullptr;
					(this->len)--;
				}
			}
		}
		void pop_front()
		{
			Node* curr = this->head;
			if (!empty())
			{
				if (this->len == 1)
				{
					delete curr;
					this->len = 0;
					this->head = nullptr;
					this->tail = nullptr;
				}
				else
				{
					this->head = curr->next;
					delete curr;
					this->head->prev = nullptr;
					(this->len)--;
				}
			}
		}
		bool empty()const
		{
			return this->len == 0;
		}
		T front()const//return copy of the first info node, T must have copy c'tor
		{
			return this->head->info;
		}
		T back()
		{
			return this->tail->info;
		}
		int size()const
		{
			return this->len;
		}
		void clear()
		{
			Node* curr = head;
			while (curr != nullptr)
			{
				curr = curr->next;
				delete head;
				head = curr;
			}
			this->head = nullptr;
			this->tail = nullptr;
			this->len = 0;
		}
		const List& operator=(const List& lst)
		{
			Node* curr = lst.head;
			while (curr != nullptr)
			{
				push_back(curr->info);
				curr = curr->next;
			}
			this->len = lst.len;
			return *this;
		}
		bool operator==(const List& lst)
		{
			Node* curr1 = this->head;
			Node* curr2 = lst.head;

			bool flag = true;
			if (this->len != lst.len)
				flag = false;
			while (curr1 != nullptr && flag)
			{
				if (curr1->info != curr2->info)
					flag = false;
				curr1 = curr1->next;
				curr2 = curr2->next;
			}
			return flag;
		}
		void save(ofstream& outfile)const
		{
			outfile.write(reinterpret_cast<const char*>(&len), sizeof(int)); //write the size
			Node* curr = this->head;
			while (curr!=nullptr)
			{
				try { curr->info->save(outfile); }
				catch (const char* msg) { throw msg; }
				curr = curr->next;
			}
		}
		void serialSave(ofstream& outfile)const
		{
			outfile.write(reinterpret_cast<const char*>(&len), sizeof(int)); //write the size
			Node* curr = head;
			while (curr != nullptr)
			{
				try { curr->info->serialSave(outfile); }
				catch (const char* msg) { throw msg; }
				curr = curr->next;
			}
		}
		void pSerialSave(ofstream& outfile)const
		{
			outfile.write(reinterpret_cast<const char*>(&len), sizeof(int)); //write the size
			Node* curr = head;
			while (curr != nullptr)
			{
				try { curr->info->pSerialSave(outfile); }
				catch (const char* msg) { throw msg; }
				curr = curr->next;
			}
		}
		
	};
}
