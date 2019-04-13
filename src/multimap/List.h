#ifndef LIST_H
#define LIST_H

///  Lightweight wrapper for list

template<typename T>
class ListNode {
public:
	ListNode(const T& t) : data(t) {}
	~ListNode() = default;

private:
	T data;
	ListNode* next;

	template<typename T> friend class List<T>;
};

template<typename T>
class List {
public:
	List();
	~List();

private:

};

#endif // LIST_H
