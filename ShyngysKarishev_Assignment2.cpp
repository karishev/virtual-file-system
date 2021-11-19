//Shyngys Karishev sk8795
//mv could work with giving the path, form current node or from the root
//same with rm. Size works only with one of the children

#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<ctime>
#include<cctype>
#include<cmath>
#include<exception>

using namespace std;

//Vector class declaration
template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector();						//No argument constructor
		MyVector(int cap);				//One Argument Constructor
		~MyVector();					//Destructor
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element);
		T& at(int index); 				//return reference to the element at index
		const T& front();				//Returns reference to the first element in the vector
		const T& back();				//Returns reference to the Last element in the vector
		void forceUniqueValues();			
		void doublecap(int cap);		//The capacity doubles if the v_size = v_capacity
		int size() const;				//Return current size of vector
		void erase(int index);
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Rturn true if the vector is empty, False otherwise
		
};
//=========================================================================================
//Stack class declaration
template <typename E>
class CStack {
	enum { defcap = 100 };
	public:
		CStack(int cap = defcap); 
		int size() const; // returns the number of elements currently in the stack 
		bool empty() const; // returns “true” if the stack is empty
		const E& top() const ; // returns the top element
		void push(const E& e) ; // push element “e” onto stack
		void pop() ; // pop the stack (i.e., remove top element)
	private:
		E* Stackk; 
		int capacity; //maximum capacity 
		int ind; //index of the top 
};

//=========================================================================================
class QueueEmpty: public exception
{
	public:
		virtual const char* what() const throw()
		{
			return "Queue is Empty"; 
		}
};
//=================================================
class QueueFull: public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Queue is Full";
	}
};
//=================================================
//Queue class declaration
template <typename T>
class Queue
{
	private:
		T *array;
		int n;	//number of current elements in the Queue
		int N;	//Array Size (Capacity)
		int f;	//index of front element
		int r;	//index where a new element will be added
	public:
		Queue(int N);
		~Queue();
		void enqueue( T element) throw (QueueFull); 
		T dequeue()	throw (QueueEmpty);
		T& front() throw (QueueEmpty);
		bool isEmpty();
		bool isFull();
		void display();
};
//=========================================================================================
//Node class declaration
class Node
{
	private:
		string name; //name of the node
		int size; //size of the node
		string date; //date of the node
		Node* parent; //parent node conncetion
		MyVector<Node*>* children; //children of the node if folder, file also has children but it will always be empty
		bool folderorfile; //if true it is folder, if false it is a file;

	public:
		Node(string name, int size, string date, bool which); 
		MyVector<Node*>* getChildren();
		string getName();
		bool fileordir();
		int getSize();
		friend class Tree;
};
//=========================================================================================
//Tree class declaration
class Tree
{
	private:
		Node* root; //root fo the tree
		Node* ptr; //current node you are working with
		Queue<string> trash = Queue<string>(10); //bin for remove purposes
		Queue<Node*> bin = Queue<Node*>(10); //also the bin for remove purposes, but the first one just stores the path also
		CStack<Node*> past; //checking the history of the cd to go to previous node
	public:
		Tree(string date);
		Tree(string path, string date); 
		~Tree();
		void allnodes(Node* check, string& nodes);
		Node* getRoot();
		Node* getPtr();
		string pwd();
		void realpath(string file);
		void ls(string sort);
		void mkdir(string name, string date);
		void touch(string name, string size1, string date);
		void cd(string parameter);
		void find(string given,Node* check);
		void mv(string file, string folder);
		void rm(string check);
		void showbin();
		void emptybin();
		Node* checkpath(string path, int& dummy);
		int size(string object, Node* check);
};
//=========================================================================================

bool isinchildren(MyVector<Node*>* children, string object, int& index);
string slicestring(string k, string& p);
void slicer(string k, MyVector<string>& vect, char divider);

//=========================================================================================
void listCommands()
{
	cout<<"List of available Commands:"<<endl
		<<" 0 - help                             : Display the list of available commands"<<endl
		<<" 1 - pwd                              : Prints current path to the inode"<<endl
		<<" 2 - realpath <filename>              : Prints the full path of a given file"<<endl
		<<" 3 - ls <sort>                        : Prints the children of the current inode if it is folder"<<endl
		<<" 4 - mkdir <foldername>               : Creates a folder under the current folder"<<endl
		<<" 5 - touch <filename> <size>          : Creates a file under the current inode location, size should be integer!"<<endl
		<<" 6 - cd                               : Change of the path with different commands"<<endl
		<<" 7 - find                             : Returns the path of the file (or the folder) if it exists"<<endl	
		<<" 8 - mv                               : view the list of all the movie screenings available for screening"<<endl
		<<" 9 - size                             : view the list of movie screenings that are not full"<<endl
		<<"10 - rm                               : Remove a node"<<endl
		<<"11 - emptybin                         : Empties the bin"<<endl
		<<"12 - showbin                          : Shows the oldest inode of the bin (including its path)"<<endl
		<<"13 - exit                             : Exit the Program"<<endl;
}
//=========================================================================================
int main()
{

	// current date/time based on current system
	time_t now = time(0);
	tm *ltm = localtime(&now);
	// print various components of tm structure.
	string year =to_string(1900 + ltm->tm_year);
	string month = to_string(1 + ltm->tm_mon);
	string day = to_string(ltm->tm_mday);
	string todaysdate = day+"-"+month+"-"+year; //getting the date we will write when creating nodes

	//creating the tree that reads from the vfs.dat
	Tree MyTree("vfs.dat", todaysdate);

	listCommands();
	string user_input;
	string command;
	string parameter1;
	string parameter2;
	Node* root = MyTree.getRoot();

	do
	{
		cout<<">";
		getline(cin,user_input);
		command = user_input.substr(0,user_input.find(" "));
		parameter1 = user_input.substr(user_input.find(" ")+1, user_input.find_last_of(" ") - user_input.find(" ") -1);
		parameter2 = user_input.substr(user_input.rfind(" ")+1);		

		if(command=="help" or command == "0")  									listCommands();
		else if(command=="pwd" or command=="1")			   	                    cout<<MyTree.pwd()<<endl;
		else if(command=="realpath" or command=="2")                            MyTree.realpath(parameter1);
		else if(command=="ls" or command=="3")		                            MyTree.ls(parameter1); 
		else if(command=="mkdir" or command == "4") 		                    MyTree.mkdir(parameter1, todaysdate);
		else if(command == "touch" or command == "5")		                    MyTree.touch(parameter1,parameter2, todaysdate);
		else if(command == "cd" or command == "6")                              MyTree.cd(parameter1);
		else if(command == "find" or command == "7")                            MyTree.find(parameter1,root);
		else if(command == "mv" or command == "8")                              MyTree.mv(parameter1, parameter2);
		else if(command == "size" or command == "9")                            cout<<MyTree.size(parameter1, MyTree.getPtr())<<" bytes"<<endl;
		else if(command == "rm" or command=="10") 			                    MyTree.rm(parameter1);
		else if(command == "emptybin" or command=="11")                         MyTree.emptybin();
		else if(command == "showbin" or command=="12")                          MyTree.showbin();
		else if(command == "exit" or command=="13"){ 
			MyTree.~Tree();
			break;
		}			
		else    cout<<"Invalid command"<<endl;
	} while(command!="exit" or command != "13");

	return 0;
}

//=========================================
Node::Node(string name, int size, string date, bool which) 
{
	this->name = name; //declaring the variables
	this->size = size;
	this->date = date;
	this->folderorfile = which;
	this->children = new MyVector<Node*>;
	this->parent = nullptr;
}

MyVector<Node*>* Node::getChildren() //sometimes we need to get them from private
{
	return children;
}

string Node::getName() //sometimes we need to get them from private
{
	return name;
}

bool Node::fileordir() //checking if the node is file or folder, if folder returns true, false otherwise
{
	if (folderorfile) return true;
	else return false;
}

int Node::getSize() //sometimes we need to get them from private
{
	return size;
}
//===========================================
Tree::Tree(string date) //if we are not reading from the file
{
	this->root = new Node("", 0, date, true); //creating root
	this->ptr = root; //ptr is at the start on the root
}

// To read from the file 
Tree::Tree(string path, string date)
{
	this->root = new Node("", 0, date, true); //creating the root
	this->ptr = root;
	ifstream myfile(path); //checking the file
	if (myfile.is_open()) {//opening the file
		string pathofnode,newelem,size,ddate,data;

		while (!myfile.eof()) { //checking every line in myfile
			getline(myfile, data); //putting one whole line into a string
			if (data == "" or data =="\n") break; //if the line is empty we simply break the loop
			MyVector<string> info; //creating the vector for slicer function purposes
			slicer(data, info, ','); //slicing the line by the ,
			pathofnode = info.at(0); //path is here
			size = info.at(1); //size is here
			ddate = info.at(2); //date is here
			if (pathofnode != "/") { //it is only for the first line, which is root declaration, so we can just get the date of root creation and change it then
				//so in other lines we are doing this
				pathofnode = slicestring(pathofnode, newelem); //this function simply slices the path by putting the last path element in another string so to create
				cd(pathofnode); //going to the path without last element, and since in the vfs dat everything is declared one by one, this should work
				if (newelem.find(".") == string::npos) { //finding if the given node is a file or a folder by checking the dot in it
					mkdir(newelem, ddate);
				}
				else {
					touch(newelem, size, ddate);
				}
			}
			else {
				root->date = ddate;//just changing the date if it not the same
			}
		}
		ptr = root; //current should on the root at the beginning
		myfile.close(); //closing the file
		
	}
}

string slicestring(string k, string& p) //explained above
{
	MyVector<string> path;
	string names;
	string t;
	stringstream ss(k); //to use the getline from the string
	while (getline(ss, names, '/')) { 
		path.push_back(names); //appending all the info one by one
	}
	p = path.at(path.size()-1);
	path.erase(path.size()-1);
	for (int i = 0; i<path.size(); i++)
	{
		if (path.at(i) == "" and i!=path.size()-1) t = t + path.at(i);
		else t = t +"/" + path.at(i);
	}
	return t;
}

void slicer(string k, MyVector<string>& vect, char divider) //explained above
{	
	stringstream ss(k);
	string data;
	while(getline(ss,data,divider)) {
		vect.push_back(data);
	}
}

Tree::~Tree() //just dumping everything into the file, if it is not created, we create it
{
	ofstream myfile("vfs.dat"); 
	if (myfile.is_open()) { //opening the file
		ptr = root; //puting the current is the root
		string nodes, line;
		nodes = pwd() + ",0," + ptr->date + "+"; //simply the first line of the .dat file, since it is the root declaration
		allnodes(root, nodes); //function below, that recursively puts every information in one string about all nodes splitting it by "+" to use getline
		stringstream ss(nodes); //to use getline
		while (getline(ss, line, '+')) {
			myfile<<line<<"\n"; //and so we just dump into the file everything line by line 
		}
		myfile.close(); //closing the file
	}
}

void Tree::allnodes(Node* check, string& nodes) //explained above
{
	
	if (check->children->empty()) return;
	for (int i = 0; i<check->children->size(); i++) {
		ptr = check->children->at(i);
		nodes = nodes + pwd() + "," + to_string(ptr->size) + "," + ptr->date + "+";
		allnodes(ptr, nodes);
	}
}

Node* Tree::getRoot() //get the root
{
	return root;
}

Node* Tree::getPtr() //get current
{
	return ptr;
}

string Tree::pwd(){ //printing current directory by putting every parent of the node in the stack and then appending the top to the string

	string path;
	Node* copy = ptr;
	CStack<string> reversepath;
	if (ptr == root) {
		return "/";
	}
	
	while (ptr->parent != nullptr) {
		reversepath.push(ptr->name);
		ptr = ptr -> parent;
	}
	while (!reversepath.empty()) {
		path = path +  "/" + reversepath.top();
		reversepath.pop();
	}
	ptr = copy;
	return path;
	
}

void Tree::realpath(string file) //checks the path of the file in current node children list
{
	int dummy = 5;
	int index;
	if (isinchildren(ptr->children, file, index)) //function that checks if a string name is in the children and returns an index of it too
	{
		Node* copy = ptr;
		ptr = checkpath(file,dummy);
		cout<<pwd()<<endl;
		ptr = copy;
	}
	else {
		cout<<"No such file!"<<endl;
	}
}

void Tree::ls(string sort){ //ls, string only for sort part
	if (sort != "sort") { //if no need to to sort, then simply print everything one by one
		if (!ptr->children->empty()) { //checks if the children is empty or not
			for (int i = 0; i < ptr->children->size(); i++) { 
				Node* copy = ptr->children->at(i); //creating a copy just to make it easier to write.
				if (copy->fileordir()) { //checking if it is a folder or file to write everything
					cout<<"dir "<<copy->getName()<<" "<< copy->date <<" "<<copy->getSize()<<"bytes" <<endl;
				}
				else {
					cout<<"file "<<copy->getName()<<" "<<copy->date<<" "<<copy->getSize()<<"bytes"<<endl;
				}
			}
		}
		else {
			cout<<"Empty node"<<endl;
		}
	}
	else { //if sort 
		int count = 1;
		while (count!=0) { //basically count counts the amount of the swaps in bubble sort method, if no swaps then everything is swapped in order 
			count = 0;
			for (int i = 0; i < ptr->children->size()-1; i++)
			{
				if(ptr->children->at(i)->size < ptr->children->at(i+1)->size) 
				{
					//swapping if the first is smaller than the second and count+1
					Node* copy = ptr->children->at(i+1);
					ptr->children->at(i+1) = ptr->children->at(i);
					ptr->children->at(i) = copy;
					count++;
				}
			}
		}
		ls("print"); //basically printing the nodes because now they are in order
	}
}

void Tree::mkdir(string name, string date) //creating a folder inside the current node
{
	for (char c : name) { //checking for alphanumerical characters
		if(!isalnum(c)) {
			cout<<"Name should be alphanumeric!"<<endl;
			return;
		}
	}
	if (name == ptr->name) //if the same name as the current node, then don't create a folder
	{
		cout<<"You can't create a folder with the same name"<<endl;
		return;
	}
	if (!ptr->children->empty()) {
		for (int i = 0; i<ptr->children->size();i++) {
			if (ptr->children->at(i)->name == name) { //if one of the children of the current node has the same name, we don't create a folder
				cout<<"You can't create a file with the same name"<<endl;
				return;
			} 
		}
	}
	//if function didn't abort, we create the folder
	Node* newnode = new Node(name, 10, date, true);
	newnode -> parent = ptr;
	ptr->children->push_back(newnode);
}

void Tree::touch(string name, string size1, string date) //same as mkdir, just checking for integer input and amount of dots
{
	for (char c : size1) {
		if (!isdigit(c)) {
			cout<<"Size should be integer!"<<endl;
			return;
		}
	}
	int size = stoi(size1);
	int countdot = 0;
	int count = 0;
	if (size<=0) {
		cout<<"File can't weight less than 0"<<endl;
		return;
	}
	if(name.find(".") == string::npos) {
		cout<<"You need to add an extension to file"<<endl;
		return;
	}

	for (char c : name) {
		if(!isalnum(c)) {
			if (c == '.') countdot++;
			else {
				cout<<"Name should be alphanumeric!"<<endl;
				return;
			}
		}
	}
	if (countdot>1) {
		cout<<"Name should be alphanumeric!"<<endl;
		return;
	}

	else if (name == ptr->name) 
	{
		cout<<"You can't create a file with the same name"<<endl;
		return;
	}
	else if (!ptr->children->empty()) {
		for (int i = 0; i<ptr->children->size();i++) {
			if (ptr->children->at(i)->name == name) {
				cout<<"You can't create a file with the same name"<<endl;
				return;
			} 
		}
	}

	Node* newnode = new Node(name, size, date, false);
	newnode -> parent = ptr;
	ptr->children->push_back(newnode);

}

void Tree::cd(string parameter) //changing directory 
{
	if (parameter == "..") //going to the parent node
	{
		if (ptr->parent != nullptr) {
			past.push(ptr);  //pushing into the stack so then we can access it in cd -
			ptr = ptr->parent;
		}
		else cout<<"Can't go back"<<endl;
	}
	else if (parameter == "-") //going to the previous
	{
		if (!past.empty()) {
			Node* copy = ptr;
			ptr = past.top(); //pushing it also in the past, so if do cd - constantly we create a loop
			past.push(copy);
		}
		else cout<<"Can't go to previous"<<endl;
	}
	else if (parameter == "" or parameter == "cd" or parameter == "/") //going to the root and also pushing to the directory
	{
		if (ptr != root) {
			past.push(ptr);
			ptr = root;
		}
		else cout<<"Already on the root"<<endl;
	}
	else //when it is given a path or foldername/filename
	{
		Node* copy = ptr;
		int dummy;
		ptr = checkpath(parameter, dummy); //this function checks the path and even the foldername/filename
		if (copy != ptr) past.push(copy); //the function returns ptr in the case the path was given incorrectly
	}
	
}

void Tree::find(string given, Node* check) //finding by recursion, nothing will be printed if we don't find
{
	if (check->name == given) {
		Node* copy = ptr;
		ptr = check;
		cout<<pwd()<<endl;
		ptr = copy;
	}
	else {
		if (!check->children->empty()) {
			for (int i = 0; i<check->children->size();i++) {
				find(given, check->children->at(i));
			}
		}
		else return;
	}
}

void Tree::mv(string file, string folder) //move works as for path, so to only children nodes
{
	int dummy = 5;
	Node* movefile = checkpath(file,dummy); //dummy is 5, because in the checkpath we access the file only in this case, otherwise we can't go to a file
	
	if (movefile == ptr and dummy == 0) 
	{
		cout<<"Can't do this operation, no such file or folder under this node"<<endl;
		return;
	}
	dummy = 5;
	Node* movefolder = checkpath(folder, dummy);
	if (movefolder == ptr and dummy == 0) 
	{
		cout<<"Can't do this operation, no such file or folder under this node"<<endl;
		return;
	}
	//if the function didn't abort, everything is okay and we simply move 
	movefolder->children->push_back(movefile);
	isinchildren(movefile->parent->children, movefile->name, dummy);
	movefile->parent->children->erase(dummy);
	movefile->parent==movefolder;
}

void Tree::rm(string check) //remove works as for path, so for the children of the current
{
	if (bin.isFull()) { //if bin is full we simply abort
		cout<<"BIN IS FULL"<<endl;
		return;
	}
	int dummy;
	dummy = 5;
	Node* removenode = checkpath(check,dummy); //same logic as in mv
	if (removenode == ptr) 
	{
		cout<<"Can't do this operation"<<endl;
		return;
	}
	//if the function didn't abort, everything is okay and we simply remove and put into the bin
	Node* parentt = removenode->parent;
	int index;
	Node* copy = ptr;
	ptr = removenode;
	bin.enqueue(removenode);
	trash.enqueue(pwd());
	isinchildren(parentt->children, removenode->name, index);
	removenode->parent->children->erase(index);
	removenode->parent = nullptr;
	ptr = copy;
	

}

int Tree::size(string object, Node* check) //checking the size recursively of the child of the current node, didn't have time for the path
{
	int index,sum;
	sum = 10;
	if (!isinchildren(check->children, object, index)) 
	{
		cout<<"No such file or folder"<<endl;
		return 0;
	}
	else
	{
		if (!check->children->at(index)->fileordir()) { //if file we return the file size
			return check->children->at(index)->size;
		}
		else if (check->children->at(index)->children->empty()){ //if folder without kids, we just return 10
			return 10;
		}
		else 
		{
			for (int i = 0; i<check->children->at(index)->children->size(); i++) {
				sum = sum + size(check->children->at(index)->children->at(i)->name, check->children->at(index));
			}
		}
	}
	return sum;

}

void Tree::emptybin() //just clear the bin by enqueuing
{
	if (bin.isEmpty()) 
	{
		cout<<"Bin is empty"<<endl;
		return;
	}
	while(!bin.isEmpty() and !trash.isEmpty()) {
		bin.dequeue();
		trash.dequeue();
	}
	cout<<"Bin is successufully cleared"<<endl;
}

void Tree::showbin() //showing the top node if any
{
	if (bin.isEmpty()) {
		cout<<"Bin is empty"<<endl;
		return;
	}
	cout<<"Next element to remove: "<<trash.front()<<" ("<<bin.front()->size<<" bytes "<< bin.front()->date <<")"<<endl;
}

Node* Tree::checkpath(string path, int& dummy) //the checking path function
{
	MyVector<string> path1;
	string names;
	stringstream ss(path); //to use the getline from the string

	if (path.find("/") != string::npos) {
		while (getline(ss, names, '/')) {
			path1.push_back(names); //appending all the info one by one
		}
	}
	else path1.push_back(path);
	//above we pushed everything into a vector, if it is a foldername or filename, we just put one element there, otherwise 
	//we slice everything by '/' and put into the vector
	Node* check = ptr;
	if (path1.at(0) == "") { //if it is the path from the root, we make the check node the root, otherwise it stays at current
		check = root;
		path1.erase(0);
	}

	int index;
	for (int i = 0; i<path1.size(); i++) {
		if (!check->children->empty() and isinchildren(check->children, path1.at(i), index)) {
			if (check->children->at(index)->fileordir()) {
				check = check->children->at(index);
			}
			else {
				if (dummy == 5 and i == path1.size()-1) return check->children->at(index); //basically we put dummy 5, so then we can return the file node
				cout<<"Can't go to a file"<<endl;
				dummy = 0;
				return ptr;
			}
		}
		else {
			cout<<"Invalid input"<<endl;
			dummy = 0;
			return ptr;
		}
	}
	dummy = 1;
	return check;
}

bool isinchildren(MyVector<Node*>* children, string object, int& index) { //checking if the given name is in children and also returns the index
	for (int i = 0; i<children->size(); i++) {
		if (object == children->at(i)->getName()) {
			index = i;
			return true;
		}
	}
	return false;
}
//================================================
//Queue implementation
template<typename T> Queue<T>::Queue(int capacity)
{
	N = capacity;
	array = new T[capacity];
	n = 0;	//number of current elements in the Queue
	f = 0;	//index of front element
	r = 0;	//index where a new element will be added
}


template<typename T> Queue<T>::~Queue()
{
	delete[] array;
}

template<typename T> void Queue<T>::enqueue(T element) throw (QueueFull)
{
	if (isFull()) throw QueueFull();
	array[r] = element;
	n = n+1;
	r = (r+1)%N;
}

template<typename T> T Queue<T>::dequeue() throw (QueueEmpty)
{
	if (isEmpty()) throw QueueEmpty();
	T firstelem = array[f];
	n = n-1;
	f = (f+1)%N;
	return firstelem;
}

template<typename T> T& Queue<T>::front() throw (QueueEmpty)
{
	if (isEmpty()) throw QueueEmpty();
	return array[f];
}
		
template<typename T> bool Queue<T>::isEmpty()
{
	return (n == 0);
}

template<typename T> bool Queue<T>::isFull()
{
	return (n==N);
}

//==============================================
//Stack implementaion
//constructor
template <typename E> CStack<E>::CStack(int cap) {
	Stackk = new E[cap];
	capacity = cap;
	ind = -1;
}
// returns the number of items in the stack
template <typename E> int CStack<E>::size() const {
	return (ind + 1);
}
// is the stack empty?
template <typename E> bool CStack<E>::empty() const {
	return (ind < 0);
}
// return top of stack
template <typename E> const E& CStack<E>::top() const  {
	if (empty()) cout<<"Top of empty stack"<<endl;
	return Stackk[ind];
}

// push element onto the stack
template <typename E> void CStack<E>::push(const E& e) 
{
	if (size() == capacity) cout<<"Push to full stack"<<endl;
	Stackk[++ind] = e;
}
// pop the stack
template <typename E> void CStack<E>::pop() 
{
	if (empty()) cout<<"Pop from empty stack"<<endl;
	ind=ind-1;
}

//=================================================
//Vector based implementation
template <typename T> MyVector<T>::MyVector(){ //No argument constructor
	v_size = 0;
	v_capacity = 0;
	data  = new T[v_capacity];
}	

template <typename T> MyVector<T>::~MyVector(){ //Destructor
	delete[] data;
}

template <typename T> void MyVector<T>::push_back(T element){ //Add an element at the end of vector
	if (v_size >= v_capacity) {
		doublecap(max(1, 2*v_capacity)); //if capacity is zero it will start with 0
	}
	data[v_size] = element;
	v_size = v_size+1;
}	

template <typename T> void MyVector<T>::insert(int index, T element){
	if (index > v_size-1 or index < 0) throw runtime_error("Index out of range");
	if (v_size >= v_capacity) {
		doublecap(max(1, 2*v_capacity));
	}
	int k = v_size-1;
	for (int ind = k; ind >= index; ind--) { //move every element by one
		data[ind+1] = data[ind];
	}
	data[index] = element; // put element at index
	v_size++;
}

template <typename T> T& MyVector<T>::at(int index){ //return reference to the element at index
	if (index > v_size-1 or index < 0) throw runtime_error("Index out of range");
	return data[index];
}

template <typename T> const T& MyVector<T>::front(){ //Returns reference to the first element in the vector
	if (empty()) throw runtime_error("Vector is empty");
	return data[0];
}	

template <typename T> const T& MyVector<T>::back(){ //Returns reference to the Last element in the vector
	if (empty()) throw runtime_error("Vector is empty");
	return data[v_size-1];
}	

template <typename T> int MyVector<T>::size() const{ //Return current size of vector
	return v_size;
}	
			
template <typename T> int MyVector<T>::capacity() const{ //Return capacity of vector
	return v_capacity;
}	
		
template <typename T> bool MyVector<T>::empty() const{ //Return true if the vector is empty, False otherwise
	return v_size==0;
} 
template <typename T> void MyVector<T>::erase(int index){
	for (int i = index; i<v_size-1; i++) {
		data[i] = data[i+1];
	}
	v_size--;
}		
template <typename T> void MyVector<T>::forceUniqueValues() {

	int k = 1;
	for (int i=0; i<v_size-1; i++) {
		if (data[i+1]!=data[i]) {
			data[k] = data[i+1];
			k++;
		}
	}
	v_size = k;
	
}
template <typename T> void MyVector<T>::doublecap(int cap) {
	if (v_capacity >= cap) return;
	T* datacopy = new T[cap]; 					// resize the array
	for (int i = 0; i < v_size; i++) 			// copy contents to new array
		datacopy[i] = data[i];
	if (data != NULL) delete[] data;			// discard old array
	data = datacopy; 						    
	v_capacity = cap; 			    			// set new n_capacity
}