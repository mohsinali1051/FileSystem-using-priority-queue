#include"MaxHeap.h"

template<class T>
class File
{
	template<class U>
	friend class HashTable;
	T FileID;
	MaxHeap<T>* H;

public:
	File(int _FileID, int UserID, int Priority, char OperateType)
	{
		FileID = _FileID;
		H = new MaxHeap<T>;
		H->Insert(UserID, Priority, OperateType);
	}
	File(int _FileID)
	{
		FileID = _FileID;
		H = new MaxHeap<T>;
	}
	~File()
	{
		delete H;
	}
};