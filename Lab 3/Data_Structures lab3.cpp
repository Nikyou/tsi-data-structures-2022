#include <iostream>
#include <string>
#include <iso646.h>
#include <fstream>

const char DELIMETER = ',';
std::string DATASET_FILE_NODES = "asoiaf-book1-nodes.csv";
std::string DATASET_FILE_EDGES = "asoiaf-book1-edges.csv";

struct NodeData;
struct EdgeData;
struct ArrayEdge;
struct ArrayNode;
struct ArrayListEdge;
struct ArrayListNode;


std::string tokenize(std::string& str, const char del) {

	std::string result = "";
	int i = (int) str.find(del);
	if (i == std::string::npos)	//last element;
	{
		if (str.size() != 0) {
			i = (int)(str.size() - 1);
		}
		else {
			i = (int) str.size();
		}

	}

	if (i == std::string::npos) {
		result = str;
		str = "";
	}
	else {
		if (i != 0) {
			result = str.substr(0, i);
		}
		else {
			result = "";
		}
		str.erase(0, i + 1);
	}

	
	return result;
};

unsigned long hash(std::string& str)
{
	std::hash<std::string> hasher;
	return (unsigned long) hasher(str) % 50000;
}

struct NodeData {
	std::string value = "";;
	ArrayListEdge* edges = NULL;
	NodeData() {
		value = "";
		edges = NULL;
	};
	NodeData(std::string& value) {
		this->value = value;
		edges = NULL;
	};

	friend bool operator== (NodeData& ldata, NodeData& rdata) {
		if (&ldata == &rdata) {
			return true;
		}
		else {
			return false;
		}
	}
	friend bool operator!= (NodeData& ldata, NodeData& rdata) {
		if (&ldata != &rdata) {
			return true;
		}
		else {
			return false;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const NodeData& data)
	{
		if (data.edges != NULL) {
			os << data.value << " -> " << data.edges;
		}
		else
		{
			os << data.value << " -> " << "none";
		}

		return os;
	}
	/*std::string ToString()
	{
		std::string result = "";
		if (value != "") {
			result = result + value + ": " + edges->ToString() + '\n';
		}

		return result;
	}*/

};

struct EdgeData {
	NodeData* pointer = NULL;
	int weight = 0;
	EdgeData() {
		pointer = NULL;
		weight = 0;
	};
	EdgeData(NodeData* data, int& weight) {
		pointer = data;
		this->weight = weight;
	};

	friend bool operator== (EdgeData& ldata, EdgeData& rdata) {
		if (&ldata == &rdata) {
			return true;
		}
		else {
			return false;
		}
	}
	friend bool operator!= (EdgeData& ldata, EdgeData& rdata) {
		if (&ldata != &rdata) {
			return true;
		}
		else {
			return false;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const EdgeData& data)
	{
		os << data.pointer->value << ", " << data.weight;
		return os;
	}

};

struct ArrayEdge {
	ArrayEdge* nextArrayPtr = NULL;
	EdgeData* array = NULL;
	unsigned long int position[2];
	ArrayEdge(unsigned long int firstPos, unsigned long int secondPos) {
		array = new EdgeData[secondPos - firstPos];
		position[0] = firstPos;
		position[1] = secondPos;
	}

};

struct ArrayNode {
	ArrayNode* nextArrayPtr = NULL;
	NodeData* array = NULL;
	unsigned long int position[2];
	ArrayNode(unsigned long int firstPos, unsigned long int secondPos) {
		array = new NodeData[secondPos - firstPos];
		position[0] = firstPos;
		position[1] = secondPos;
	}

};

struct ArrayListEdge {
	unsigned long int totalLength = 70;	//default, do not manually change
	unsigned long int elementsLength = 0;	//default, do not manually change
	unsigned long int nextResize = totalLength * 2;	//default, do not manually change
	ArrayEdge firstArray = ArrayEdge(0UL, 70UL);
	EdgeData EmptyEdge = EdgeData();

	bool appendCheck = false;

	void Append(EdgeData element) {
		appendCheck = true;
		Insert(0, element);
		appendCheck = false;
	}

	EdgeData& Get(unsigned long int index) {
		if (index < 0 or index > totalLength) {
			std::cerr << "Array index out of bounds";
			return EmptyEdge;
		}
		ArrayEdge* curArrayPtr = &firstArray;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					std::cerr << "Array index out of bounds";
					return EmptyEdge;
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				return curArrayPtr->array[index - curArrayPtr->position[0]];
			}
		} while (true);
	}

	long int Get(EdgeData element) {	//returns -1 if none found

		ArrayEdge* curArrayPtr = &firstArray;
		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			if (curArrayPtr->array[i - curArrayPtr->position[0]] == element) {
				return i;
			};
		}
		return -1;
	}

	void Delete(unsigned long int index) {

		if (index < 0) {
			std::cerr << "Array index out of bounds";
			return;
		}
		ArrayEdge* curArrayPtr = &firstArray;		
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					std::cerr << "Array index out of bounds";
					return;
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				if (curArrayPtr->array[index - curArrayPtr->position[0]].pointer == NULL) {	//empty data
					return;
				}
				else {	//existing data
					curArrayPtr->array[index - curArrayPtr->position[0]] = EmptyEdge;
					elementsLength = elementsLength - 1;
					return;
				}
			}
		} while (true);

	}

	unsigned long int Length() {
		return elementsLength;
	}

	void Insert(unsigned long int index, EdgeData element) {

		if (index < 0) {
			std::cerr << "Array index is negative";
			return;
		}
		ArrayEdge* curArrayPtr = &firstArray;
		ArrayEdge* tempArrayPtr = NULL;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					tempArrayPtr = new ArrayEdge(totalLength + 1, nextResize);
					totalLength = nextResize;
					nextResize = totalLength * 2;
					curArrayPtr->nextArrayPtr = tempArrayPtr;	//Add pointer to next array
					curArrayPtr = tempArrayPtr;
					tempArrayPtr = NULL;	//Clear temp pointer
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				if (curArrayPtr->array[index - curArrayPtr->position[0]].pointer == NULL) {	//empty data

					elementsLength = elementsLength + 1;
					curArrayPtr->array[index - curArrayPtr->position[0]] = element;
					return;
				}
				else {	//replace data
					if (appendCheck) {	//happened during Append()
						index = index + 1;
					}
					else {
						curArrayPtr->array[index - curArrayPtr->position[0]] = element;
						return;
					}
				}
			}
		} while (true);
	}

	friend std::ostream& operator<<(std::ostream& os, ArrayListEdge& data)	//TBD
	{
		EdgeData* edge;
		for (unsigned long int i = 0; i < data.totalLength; i++) {
			edge = &data.Get(i);
			if (edge->pointer != NULL) {
				os << &edge->pointer->value << ", " << &edge->weight << "; ";
			}
		}
		return os;
	}

	std::string ToString()	//TBD
	{
		std::string result = "";
		EdgeData* edge;
		if (this != nullptr) {

		
		for (unsigned long int i = 0; i < totalLength; i++) {
			edge = &Get(i);
			if (edge->pointer != NULL) {
				result = result + edge->pointer->value + ", " + std::to_string(edge->weight) + "; ";
			}
		}
		return result;
		}
		return "";
	}
};

struct ArrayListNode {
	unsigned long int totalLength = 10;	//default, do not manually change
	unsigned long int elementsLength = 0;	//default, do not manually change
	unsigned long int nextResize = totalLength * 2;	//default, do not manually change
	ArrayNode firstArray = ArrayNode(0UL, 10UL);
	NodeData EmptyNode = NodeData();

	bool appendCheck = false;

	void Append(NodeData element) {
		appendCheck = true;
		Insert(0, element);
		appendCheck = false;
	}

	NodeData& Get(unsigned long int index) {
		if (index < 0 or index > totalLength) {
			std::cerr << "Array index out of bounds";
			return EmptyNode;
		}
		ArrayNode* curArrayPtr = &firstArray;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					std::cerr << "Array index out of bounds";
					return EmptyNode;
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				return curArrayPtr->array[index - curArrayPtr->position[0]];
			}
		} while (true);
	}

	long int Get(NodeData element) {	//returns -1 if none found

		ArrayNode* curArrayPtr = &firstArray;
		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			if (curArrayPtr->array[i - curArrayPtr->position[0]] == element) {
				return i;
			};
		}
		return -1;
	}

	void Delete(unsigned long int index) {

		if (index < 0) {
			std::cerr << "Array index out of bounds";
			return;
		}
		ArrayNode* curArrayPtr = &firstArray;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					std::cerr << "Array index out of bounds";
					return;
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				if (curArrayPtr->array[index - curArrayPtr->position[0]].value == "") {	//empty data
					return;
				}
				else {	//existing data
					curArrayPtr->array[index - curArrayPtr->position[0]] = EmptyNode;
					elementsLength = elementsLength - 1;
					return;
				}
			}
		} while (true);
	}

	void DeleteByName(std::string name) {

		if (name == "") {
			std::cerr << "Invalid name";
			return;
		}
		ArrayNode* curArrayPtr = &firstArray;

		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			if (curArrayPtr->array[i - curArrayPtr->position[0]].value == name) {
				EdgesCleanUp(curArrayPtr->array[i - curArrayPtr->position[0]]);
				curArrayPtr->array[i - curArrayPtr->position[0]] = EmptyNode;
			};
		}
	}

	unsigned long int Length() {
		return elementsLength;
	}

	void Insert(unsigned long int index, NodeData element) {

		if (index < 0) {
			std::cerr << "Array index is negative";
			return;
		}
		ArrayNode* curArrayPtr = &firstArray;
		ArrayNode* tempArrayPtr = NULL;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					tempArrayPtr = new ArrayNode(totalLength + 1, nextResize);
					totalLength = nextResize;
					nextResize = totalLength * 2;
					curArrayPtr->nextArrayPtr = tempArrayPtr;	//Add pointer to next array
					curArrayPtr = tempArrayPtr;
					tempArrayPtr = NULL;	//Clear temp pointer
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				if (curArrayPtr->array[index - curArrayPtr->position[0]].value == "") {	//empty data
					elementsLength = elementsLength + 1;
					curArrayPtr->array[index - curArrayPtr->position[0]] = element;
					return;
				}
				else {	//replace data
					if (appendCheck) {	//happened during Append()
						index = index + 1;
					}
					else {
						curArrayPtr->array[index - curArrayPtr->position[0]] = element;
						return;
					}
				}
			}
		} while (true);

	}


	friend std::ostream& operator<<(std::ostream& os, ArrayListNode& data)	//TBD
	{
		NodeData* node;
		for (unsigned long int i = 0; i < data.totalLength; i++) {
			node = &data.Get(i);
			if (node->value != "") {
				os << node->value << ": " << node->edges << '\n';
			}
		}

		return os;
	}
	std::string ToString()
	{
		std::string result = "";
		NodeData* node;
		for (unsigned long int i = 0; i < totalLength; i++) {
			node = &Get(i);
			if (node->value != "") {
				result = result + node->value + ": " + node->edges->ToString() + '\n';
			}
			std::cout << result;
		}

		return result;
	}

	private: void EdgesCleanUp(NodeData node) {
		ArrayNode* curArrayPtr = &firstArray;
		NodeData* tempnode;
		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			if (curArrayPtr->array[i - curArrayPtr->position[0]].value != "") {
				tempnode = &curArrayPtr->array[i - curArrayPtr->position[0]];
				ArrayEdge* curArrayEdgePtr = &tempnode->edges->firstArray;
				for (unsigned long int j = 0; j < tempnode->edges->totalLength; j++) {
					if (curArrayEdgePtr->position[1] < j) {
						curArrayEdgePtr = curArrayEdgePtr->nextArrayPtr;
					}

					if (curArrayEdgePtr->array[j - curArrayEdgePtr->position[0]].pointer->value == node.value) {
						curArrayEdgePtr->array[j - curArrayEdgePtr->position[0]] = tempnode->edges->EmptyEdge;
						tempnode->edges->elementsLength = tempnode->edges->elementsLength - 1;
						break;
					}
				}
			}
		}
	}
};

ArrayListNode ReadFileNode(const std::string filename, unsigned long int size, const char del) {	// size == 0 to read whole file
	ArrayListNode array = ArrayListNode();
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string line = "";
	unsigned long int i = 0;
	if (!file.is_open())
	{
		std::cerr << "There was en error opening file\n";
		return array;
	}
	std::string token;
	unsigned long int index = 1;
	NodeData data;
	while (std::getline(file, line))
	{
		i++;
		if (i == size) {
			break;
		}
		data = NodeData();
		token = tokenize(line, del);
		if (token != "") {
			index = hash(token);
		}
		token = tokenize(line, del);
		if (token != "") {
			data.value = token;
		}
		array.Insert(index, data);
	}
	file.close();
	return array;
}

void ReadFileEdge(const std::string filename, ArrayListNode& nodes, unsigned long int size, const char del) {	// size == 0 to read whole file
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string line = "";
	unsigned long int i = 0;
	if (!file.is_open())
	{
		std::cerr << "There was en error opening file\n";
		return;
	}
	std::string token;
	int weight;
	std::string sourcename;
	NodeData* source = NULL;
	std::string targetname;
	NodeData* target = NULL;
	while (std::getline(file, line))
	{
		i++;
		if (i == size) {
			break;
		}
		token = tokenize(line, del);
		if (token != "") {
			sourcename = token;
		}
		token = tokenize(line, del);
		if (token != "") {
			targetname = token;
		}
		source = &(nodes.Get(hash(sourcename)));
		target = &(nodes.Get(hash(targetname)));
		try {
			weight = std::stoi(line);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		try {
			if (source->edges == NULL) {
				ArrayListEdge array;
				source->edges = &array;
			}
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		try {
			source->edges->Append(*new EdgeData(target, weight));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		try {
			if (target->edges == NULL) {
				ArrayListEdge array;
				target->edges = &array;
			}
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		try {
			target->edges->Append(*new EdgeData(source, weight));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
	}
	file.close();
	return;
}

int main()
{
	std::cout << "Before nodes\n";
	ArrayListNode nodes = ReadFileNode(DATASET_FILE_NODES, 0, DELIMETER);
	std::cout << "Before edges\n";
	try {
	ReadFileEdge(DATASET_FILE_EDGES, nodes, 0, DELIMETER);
	}
	catch (const std::exception& ex) {
		std::cout << ex.what();
	}
	std::cout << "Before pause\n";
	system("pause");
	return 0;
}