#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include"Node.h"
using namespace std;

class Helper
{
public:
	// the function will find the tablesize with prime number 
	static int CalculateTableSize()
	{
		// if prime number is even then find the next primme number
		int number = CalculateNumberOfFiles();
		bool primeNumber = false;

		for (int i = 2; i < number / 2 && !primeNumber; i++)
		{
			if (number % i == 0)
			{
				while (!primeNumber)
				{
					number++;
					bool check = false;
					for (int i = 1; i < number / 2; i++)
					{
						if (number % i == 0)
						{
							check = true;
						}
					}
					if (!check)
						primeNumber = true;
				}
			}
		}

		return number;
	}

	// the function will calculate the total number of files.
	static int CalculateNumberOfFiles()
	{

		ifstream file("inputfile.txt");
		int counter = 0;
		string temp;

		while (!file.eof())
		{
			getline(file, temp);
			counter++;
		}
		return counter;
	}
};
template<class T>
class HashTable
{
	int TableSize;	//Table Size
	list<File<T>* >* List;	//Keeping the track of all the Files
public:
	//Constrctor of Hash Table
	HashTable(int _TableSize)
	{
		TableSize = _TableSize;
		List = new list<File<T>*>[TableSize];
	}

	//Destructor of Hash Table
	~HashTable()
	{
		/*for(int i=0;i<TableSize;i++)
		{
			delete [] List[i];
		}
		delete [] List;*/

	}

	//The function check that the file already exists or not
	File<T>* isFileAlreadyExist(int FileID)
	{
		int counter = 0;
		while (counter < TableSize)
		{
			// inner for loop iterate through every index i.e List[0],List[1].................
			for (auto it = List[counter].begin(); it != List[counter].end(); ++it)
			{
				File<T>* N = *it;
				if (N->FileID == FileID)// if the file id entered by the user is matched with file id that is present in the list
				{
					return N;				// if file exists then return the pointer of that 
											// particular file from the system  
				}
			}
			counter++;
		}
		return nullptr;
	}

	void FileDelete(int FileID)
	{
		int counter = 0;
		//iterating through the tablesize
		while (counter < TableSize)
		{
			// inner for loop iterate through every index i.e List[0],List[1].................
			for (auto it = List[counter].begin(); it != List[counter].end(); ++it)
			{
				File<T>* N = *it;
				if (N->FileID == FileID)// if the file id entered by the user is matched with file id that is present in the list
				{
					List[counter].erase(it);// if file exists then erase it from the system and retrun
					return;
				}
			}
			counter++;
		}
	}

	// the function will find the index using hash function based on file id
	int HashFunction(int FileID)
	{
		int sum = 0;
		while (FileID > 0)
		{
			sum = sum + (FileID % 10);
			FileID = FileID / 10;
		}
		return sum % TableSize;
	}

	//Loading the data from the file
	void LoadData()
	{
		ifstream file("inputfile.txt");

		int FileID = -1;
		int UserID = -1;
		int Priority = -1;
		char OperateType = '#';

		while (!file.eof())
		{

			string temp;
			getline(file, temp);

			string str = "";
			bool check = true;
			char space;
			int i = 0;

			//this for loop will take file id as we have concatenating the data until we reach a comma(,).
			for (; temp[i] != '\0' && check; i++)
			{
				if (temp[i] == ',')
				{
					FileID = stoi(str);// converting string to integer

					check = false;
				}
				str = str + temp[i];
			}
			space = temp[i]; i++;//taking the spaace after the comma
			str = "";
			check = true;

			//this for loop will take user id as we have concatenating the data until we reach a comma(,).
			for (; temp[i] != '\0' && check; i++)
			{
				if (temp[i] == ',')
				{
					UserID = stoi(str);// converting string to integer
					check = false;
				}
				str = str + temp[i];
			}
			space = temp[i]; i++;//taking the spaace after the comma
			str = "";
			check = true;

			//this for loop will take priority number as we have concatenating the data until we reach a comma(,).
			for (; temp[i] != '\0' && check; i++)
			{
				if (temp[i] == ',')
				{
					Priority = stoi(str);// converting string to integer
					check = false;
				}
				str = str + temp[i];
			}
			space = temp[i]; i++; //taking the spaace after the comma

			//at last we take operation type either read or write
			OperateType = temp[i];

			//if the file is already exists in system then directly insert it in the hash table
			File<T>* N = isFileAlreadyExist(FileID);

			if (N != nullptr)
			{
				N->H->Insert(UserID, Priority, OperateType);
			}

			//if the file is not already exists in system then we make a newfile and insert it in the hash table based on id
			else
			{
				int index = HashFunction(FileID);
				File<T>* newFile = new File<T>(FileID, UserID, Priority, OperateType);
				List[index].push_back(newFile);
			}
		}
	}
	
	//Displaying the top of the Heap
	void TopofHeap()
	{
		auto it = List[0].begin();
		File* N = *it;
		cout << N->H->max();
	}

	//Inserting the File in the Hash Table
	void Insert_File()
	{
		int FileID = -1;
		//The function will insert the new file in a system by taking the fileid from user
		cout << "Enter FILE ID please: ";
		cin >> FileID;

		File<T>* N = isFileAlreadyExist(FileID);// this function will check if file is already exist in system
												// then returns N!=nullptr else if not already exists then return nullptr
		if (N == nullptr)
		{
			int index = HashFunction(FileID);// the function to get index from the hash function based on file id
			File<T>* newFile = new File<T>(FileID);
			List[index].push_back(newFile);// insert the new file in array of list (built-in).
		}
	}

	//Requesting file to Access and grant the permission
	void RequestFileAccess()
	{
		int FileID = -1;
		int UserID = -1;
		int Priority = -1;
		char OperateType = '#';

		cout << "1.FileAccess by Priority Number" << endl << "2.FileAccess by Highest/Lowest Priority " << endl;
		int option = -1;
		cout << "Enter Your Choice: ";
		cin >> option;

		if (option == 1)
		{
			cout << "Enter the File ID please: ";
			cin >> FileID;
			cout << "Enter the User ID: ";
			cin >> UserID;
			cout << "Enter the Priority Number: ";
			cin >> Priority;
			cout << "Enter the Operation Type: ";
			cin >> OperateType;

			File<T>* N = isFileAlreadyExist(FileID);

			if (N == nullptr)
			{
				cout << "File doesn't Exist in the database" << endl << endl;
			}
			else
			{
				N->H->Insert(UserID, Priority, OperateType);
			}
		}
		else if (option == 2)
		{
			cout << "Enter the File ID please: ";
			cin >> FileID;
			cout << "Enter the User ID: ";
			cin >> UserID;
			cout << "Enter the Operation Type: ";
			cin >> OperateType;

			cout << "Press 1 to Highest Priority" << endl << "Press 2 to Lowest Priority" << endl;
			cout << "Enter your choice: ";
			cin >> option;
			File<T>* N = isFileAlreadyExist(FileID);
			if (option == 1 && N != nullptr)
			{
				UserNode<T>* NOD = N->H->FindMaxH();
				Priority = NOD->getPriority();
			}
			if (option == 2 && N != nullptr)
			{
				UserNode<T>* NOD = N->H->FindMinH();
				Priority = NOD->getPriority();
			}
			if (N == nullptr)
			{
				cout << "File doesn't Exist in the database" << endl << endl;
			}
			else
			{
				N->H->Insert(UserID, Priority, OperateType);
			}
		}
	}

	//Deleting the file from the Hash Table
	void DeleteFile()
	{
		// this function will delete the file by taking the fileid from the user.
		int FileID = -1;
		bool isGrantedEmpty;
		cout << "Enter FILE ID please: ";
		cin >> FileID;

		File<T>* N = isFileAlreadyExist(FileID);// checks whether file exists or not in the system 
												// if not exists then returns nullptr else return *N
		if (N != nullptr)
		{
			if (N->H != nullptr)
			{
				isGrantedEmpty = N->H->isEmptyGrantedUsers();// this function check the size2 which is the size
														   //of GrantedUsers if someone is accessing the file then show 
														  //error else delete the file
				if (isGrantedEmpty == true)
				{
					FileDelete(FileID);
					cout << "Successfully Deleted :)" << endl;
				}
				if (isGrantedEmpty == false)// if a file exists but someone is currently accessing it
				{
					cout << "Can't delete the file because someone is accessing\n";
				}
			}
		}
		else if (N == nullptr)			// if there is no any file that user entered the file id
		{
			cout << "Can't delete the file because someone is accessing\n";
		}
	}

	//This function is Releasing the File from the user
	void ReleaseFile()
	{
		int FileID = -1;	//Declartion of File ID
		int UserID = -1;	//Declartion of File ID

		cout << "Enter the File ID please: ";
		cin >> FileID;	//Asking user for File ID
		cout << "Enter the User ID:";
		cin >> UserID;	//Asking user for File ID

		//Checking if the File Already Exist or not
		File<T>* N = isFileAlreadyExist(FileID);
		if (N != nullptr)
		{
			//If File Found then release file from the User
			N->H->Release(UserID);
			//If Users Waiting Queue is not empty and Current Users Queue is empty then grant the
			//waiting user access
			N->H->GrantPermission();
		}
	}

	//Priniting the Hash Table
	void Print()
	{
		int counter = 0;

		// Iterating the entire list with the size of table
		while (counter < TableSize)
		{
			for (auto it = List[counter].begin(); it != List[counter].end(); ++it)
			{
				cout << "H" << counter + 1 << "-->";
				File<T>* N = *it;
				cout << "File " << N->FileID << "....";
				cout << "Access Granted to ";
				N->H->PrintGrantedUsers();
				bool check = N->H->PrintNextUsers();
				if (check == false)
				{
					cout << "\t\t Next none" << endl;
					cout << "\t\t Waiting none" << endl;
				}

			}
			counter++;
		}
	}
};


int main()
{
	int TableSize = 11;//Helper::CalculateTableSize();
	HashTable<int> Table(TableSize);

	cout << "------------------------------File System---------------------------" << endl;

	int choice = -1;
	bool AlreadyLoad = false;

	while (choice != 7)
	{
		cout << endl << "-------File System Menu--------" << endl;
		cout << "1.Print HashTable" << endl << "2.Load Data" << endl << "3.Insert File" << endl << "4.Delete File" << endl
			<< "5.Request File" << endl << "6.Release" << endl << "7.Exit" << endl;
		cout << "Enter the Choice please: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			Table.Print();
			break;
		case 2:
			if (AlreadyLoad == false)
			{
				Table.LoadData();
				AlreadyLoad = true;
			}
			else if (AlreadyLoad == true)
					cout << "The file is already loaded\n";
			break;
		case 3:
			Table.Insert_File();
			break;
		case 4:
			Table.DeleteFile();
			break;
		case 5:
			Table.RequestFileAccess();
			break;
		case 6:
			Table.ReleaseFile();
			break;
		case 7:
			return 0;
		}
	}
	return 0;
}