enum OperationType
{
	READ, WRITE, NONE
};

template<class T>
class UserNode
{
	template<class U>
	friend class MaxHeap;
	int Priority;
	T UserID;
	OperationType AccessType;
public:
	UserNode(int _UserID, int _Priority, char _AccessType)
	{
		UserID = _UserID;
		Priority = _Priority;
		if (_AccessType == 'W')
			AccessType = WRITE;
		else if (_AccessType == 'R')
			AccessType = READ;
		else
			AccessType = NONE;
	}

	UserNode(int _UserID, int _Priority, OperationType _AccessType)
	{
		UserID = _UserID;
		Priority = _Priority;
		AccessType = _AccessType;
	}

	int getPriority()
	{
		return Priority;
	}
};