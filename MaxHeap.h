#include<vector>
using namespace std;
#include"UserNode.h"

template<class T>
class MaxHeap
{
	vector<UserNode<T>* > Heap;//Size
	vector<UserNode<T>* > MinimumHeap;// Size1
	vector<UserNode<T>* > GrantedUsers;//Size2
	int Size;
	int Size1;
	int Size2;
public:

	MaxHeap()
	{
		Heap.resize(1);
		MinimumHeap.resize(1);
		GrantedUsers.resize(1);
		Size = 0;
		Size1 = 0;
		Size2 = 0;
	}

	MaxHeap(int UserID, int Priority, char OperateType)
	{
		Heap.resize(1);
		MinimumHeap.resize(1);
		GrantedUsers.resize(1);
		Size = 1;
		Size1 = 1;
		Size2 = 1;
	}

	~MaxHeap()
	{

	}

	inline int parent(int index)
	{
		return index / 2;
	}

	inline int leftChild(int index)
	{
		return index * 2;
	}

	inline int rightChild(int index)
	{
		return (index * 2) + 1;
	}

	int size()
	{
		return Size;
	}

	bool isEmptyHeap()
	{
		if (Size > 0)
			return false;
		return true;
	}

	bool isEmptyGrantedUsers()
	{
		if (Size2 > 0)
			return false;
		return true;
	}

	inline bool leaf(int index)
	{
		return leftChild(index) > size();
	}

	void PerculateDownMax(int index)
	{
		bool isok = false;
		while (!leaf(index) && !isok)
		{
			int mini = index;
			if (Heap[index]->Priority < Heap[leftChild(index)]->Priority)
				mini = leftChild(index);
			if (rightChild(index) < size() && Heap[mini]->Priority < Heap[rightChild(index)]->Priority)
				mini = rightChild(index);

			if (index != mini)
			{
				swap(Heap[index], Heap[mini]);
				index = mini;
			}
			else
			{
				isok = true;
			}

		}
	}

	void PerculateUpMax(int index)
	{
		while (index > 0)
		{
			if (parent(index) > 0)
			{
				if (Heap[parent(index)]->Priority < Heap[index]->Priority)
					swap(Heap[parent(index)], Heap[index]);
			}
			index = parent(index);
		}
	}

	void PerculateDownMin(int index)
	{
		bool isok = false;
		while (!leaf(index) && !isok)
		{
			int mini = index;
			if (Heap[index]->Priority > Heap[leftChild(index)]->Priority)
				mini = leftChild(index);
			if (rightChild(index) < size() && Heap[mini]->Priority > Heap[rightChild(index)]->Priority)
				mini = rightChild(index);

			if (index != mini)
			{
				swap(Heap[index], Heap[mini]);
				index = mini;
			}
			else
			{
				isok = true;
			}

		}
	}

	void PerculateUpMin(int index)
	{
		while (index > 0)
		{
			if (parent(index) > 0)
			{
				if (MinimumHeap[parent(index)]->Priority > MinimumHeap[index]->Priority)
					swap(MinimumHeap[parent(index)], MinimumHeap[index]);
			}
			index = parent(index);
		}
	}

	void BuildHeapMax(vector<UserNode<T>* >& Heap)
	{
		int index = size() / 2;
		while (index > 1)
		{
			int temp = index;
			if (Heap[index]->Priority < Heap[leftChild(index)]->Priority)
				PerculateDownMax(index);
			else if (rightChild(index) < size())
				if (Heap[index]->Priority < Heap[rightChild(index)]->Priority)
					PerculateDownMax(index);

			if (Heap[index]->Priority > Heap[parent(index)]->Priority)
				PerculateUpMax(index);

			index = temp;
			index--;
		}
	}

	UserNode<T>* FindMaxH()
	{
		if (Size > 0)
		{
			return Heap[1];
		}
	}

	UserNode<T>* FindMaxG()
	{
		if (Size2 > 0)
		{
			return GrantedUsers[1];
		}
	}

	UserNode<T>* FindMinH()
	{
		if (Size1 > 0)
		{
			return MinimumHeap[1];
		}
	}

	UserNode<T> ExtractMaximum()
	{
		if (Size > 0)
		{
			UserNode<T> NewN = Heap[1];
			Heap.erase(Heap[1]);
			Size--;
			return NewN;
		}
	}

	int max()
	{
		return Heap[1]->Priority;
	}

	void DeleteMax()
	{
		Heap[1] = Heap[size()];
		int index = 1;
		PerculateDownMax(index);
		Heap.pop_back();
		Size--;
	}

	void GrantPermission()
	{
		//Checking if the size of waiting Queue is grater than 0 and size of GrantedUsers is equal to 0
		if (Size2 == 0 && Size > 0)
		{
			//User to be inserted in the GrantedUers Queue
			UserNode<T>* UN = new UserNode<T>(Heap[1]->UserID, Heap[1]->Priority, Heap[1]->AccessType);
			if (UN->AccessType == WRITE)
			{
				GrantedUsers.push_back(UN);
				Size2++;
				DeleteMax();
			}
			else
			{
				bool check = true;
				//THIS while loop will all the users with READ Access until we got a user with
				//WRITE Access
				while (UN->AccessType == READ && check)
				{
					GrantedUsers.push_back(UN);
					Size2++;
					DeleteMax();

					if (Size > 0)
						UN = new UserNode<T>(Heap[1]->UserID, Heap[1]->Priority, Heap[1]->AccessType);
					else
						check = false;
				}
			}
		}
		//Checking if the size of waiting Queue is grater than 0 and size of GrantedUsers is equal to 0
		else if (Size2 > 0 && Size > 0)
		{
			//Just for checking purpose
			UserNode<T>* New = GrantedUsers[1];
			if (New->AccessType == READ && Heap[1]->AccessType == READ)
			{
				//User to be inserted in the GrantedUers Queue
				UserNode<T>* UN = new UserNode<T>(Heap[1]->UserID, Heap[1]->Priority, Heap[1]->AccessType);
				GrantedUsers.push_back(UN);
				Size2++;
				DeleteMax();
			}
			else
			{
				if (New->Priority <= Heap[1]->Priority && New->AccessType == READ && Heap[1]->AccessType == READ)
				{
					UserNode<T>* UN = new UserNode<T>(Heap[1]->UserID, Heap[1]->Priority, Heap[1]->AccessType);
					GrantedUsers.push_back(UN);
					Size2++;
					DeleteMax();
				}
			}
		}

	}

	void Release(int UserID)
	{
		//Checking if size of Granted Users is grater than 0
		if (Size2 > 0)
		{
			for (auto it = GrantedUsers.begin() + 1; it != GrantedUsers.end(); it++)
			{
				UserNode<T>* N = *it;
				//If User's ID is equal to User's Entered ID then remove the access of the user from the Queue
				if (N->UserID == UserID)
				{
					//Removing the user from the Granted User's Queue
					GrantedUsers.erase(it);
					Size2--;
					cout << "Successfully Released the File :)" << endl;
					return;
				}
			}
		}
		//"Queue is Empty. "
	}

	void PrintGrantedUsers()
	{
		if (Size2 > 0)
		{
			for (unsigned int i = 1; i <= Size2; i++)
			{
				cout << "User " << GrantedUsers[i]->UserID << ", ";
			}
			if (GrantedUsers[1]->AccessType == WRITE)
			{
				cout << "Write" << endl;
			}
			else
			{
				cout << "Read" << endl;
			}
		}
		else
		{
			cout << "none" << endl;
		}
	}

	bool PrintNextUsers()
	{
		int counter = 1;
		if (Size > 0)
		{
			if (Heap[1]->AccessType == WRITE)
			{
				cout << "\t\t Next User " << Heap[1]->UserID << ", Write" << endl;
				cout << "\t\t Waiting " << Size - counter << " Users" << endl;
			}
			else
			{
				bool check = true;

				cout << "\t\t Next Users ";
				while (Heap[counter]->AccessType == READ && check)
				{
					cout << Heap[counter]->UserID << ", ";
					counter++;
					if (counter > Size)
					{
						cout << "Read" << endl;
						cout << "\t\t Waiting 0 Users" << endl;
						break;
					}
					if (Heap[counter]->AccessType == WRITE)
					{
						cout << "Read" << endl;
						cout << "\t\t Waiting " << Size - counter - 1 << " Users" << endl;
						check = false;
					}
				}
			}
			return true;
		}
		return false;
	}

	void Insert(int UserID, int Priority, char AccessType)
	{
		UserNode<T>* UN = new UserNode<T>(UserID, Priority, AccessType);
		Heap.push_back(UN);
		MinimumHeap.push_back(UN);
		Size++;
		Size1++;
		int index = size();
		PerculateUpMax(index);
		PerculateUpMin(index);
		GrantPermission();
	}

};