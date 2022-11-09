#include <iostream>
#include <string>
#include <iso646.h>
#include <fstream>

const char DELIMETER = ',';
const std::string DATASET_FILE = "dataset.csv";
const unsigned long int SIZE = 887;

std::string tokenize(std::string &str, const char del) {

	std::string result = "";
	int i = str.find(del);
	if (i == std::string::npos)	//last element;
	{
		if (str.size() != 0) {
			i = (int) (str.size() - 1);
		}
		else {
			i = str.size();
		}
		
	}
	if (i != 0) {
		result = str.substr(0, i);
	}
	else {
		result = "";
	}
	
	str.erase(0, i+1);
	return result;
}


struct TitanicData {
	bool survived;
	unsigned short int passangerClass;  //1-3
	std::string name;
	std::string gender;
	unsigned short int age; //1+
	unsigned short int siblings; //0+	Siblings/Spouses Aboard
	unsigned short int parents;//0+	Parents/Children Aboard
	double fare;    //>0

	TitanicData() {
		survived = 0;
		passangerClass = 0;
		name = "";
		gender = "";
		age = 0;
		siblings = 0;
		parents = 0;
		fare = 0;
	}

	friend std::ostream& operator <<(std::ostream& os, const TitanicData& data)
	{
		os << data.survived << ',' << data.passangerClass << ',' << data.name << ',' << data.gender << ',' << data.age << ',' << data.siblings << ',' << data.parents << ',' << data.fare;
		return os;
	}
};

struct ArrayTitanic {
	ArrayTitanic* nextArrayPtr = NULL;
	TitanicData* array = NULL;
	unsigned long int position[2];
	ArrayTitanic(unsigned long int firstPos, unsigned long int secondPos) {
		array = new TitanicData[secondPos - firstPos];
		position[0] = firstPos;
		position[1] = secondPos;
	}

};

struct ArrayListTitanic {

	unsigned long int totalLength = 10;	//default, do not manually change
	unsigned long int elementsLength = 0;	//default, do not manually change
	unsigned long int nextResize = totalLength * 2;	//default, do not manually change
	ArrayTitanic firstArray = ArrayTitanic(0UL, 10UL);

	bool appendCheck = false;

	void Append(TitanicData element) {
		appendCheck = true;
		Insert(0, element);
		appendCheck = false;
	}

	TitanicData Get(unsigned long int index) {

		if (index < 0) {
			std::cerr << "Array index out of bounds";
			TitanicData emptyData = TitanicData();
			return emptyData;
		}
		ArrayTitanic* curArrayPtr = &firstArray;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					std::cerr << "Array index out of bounds";
					TitanicData emptyData = TitanicData();
					return emptyData;
				}
				else {
					curArrayPtr = curArrayPtr->nextArrayPtr;
				}
			}
			else {
				if (curArrayPtr->array[index - curArrayPtr->position[0]].passangerClass == 0) {	//empty data
					std::cerr << "Index does not contain any data";
					return curArrayPtr->array[index - curArrayPtr->position[0]];
				}
				else {	//existing data		
					return curArrayPtr->array[index - curArrayPtr->position[0]];
				}
			}
		} while (true);

	}

	void Delete(unsigned long int index) {

		if (index < 0) {
			std::cerr << "Array index out of bounds";
			return;
		}
		ArrayTitanic* curArrayPtr = &firstArray;
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
				if (curArrayPtr->array[index - curArrayPtr->position[0]].passangerClass == 0) {	//empty data
					return;
				}
				else {	//existing data
					TitanicData emptyData = TitanicData();
					curArrayPtr->array[index - curArrayPtr->position[0]] = emptyData;
					elementsLength = elementsLength - 1;
					return;
				}
			}
		} while (true);

	}

	unsigned long int Length() {
		return elementsLength;
	}

	void Insert(unsigned long int index, TitanicData element) {

		if (index < 0) {
			std::cerr << "Array index is negative";
			return;
		}
		ArrayTitanic* curArrayPtr = &firstArray;
		ArrayTitanic* tempArrayPtr = NULL;
		do {
			if (curArrayPtr->position[1] < index) {
				if (curArrayPtr->nextArrayPtr == NULL) {
					tempArrayPtr = new ArrayTitanic(totalLength + 1, nextResize);
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
				if (curArrayPtr->array[index - curArrayPtr->position[0]].passangerClass == 0) {	//empty data
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

	TitanicData FindUnluckiest() {

		TitanicData unluckiest;
		ArrayTitanic* curArrayPtr = &firstArray;
		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			else {
				if (curArrayPtr->array[i - curArrayPtr->position[0]].passangerClass != 0) {	//non empty data
					if ((curArrayPtr->array[i - curArrayPtr->position[0]].survived == false) and (curArrayPtr->array[i - curArrayPtr->position[0]].fare > unluckiest.fare)) {
						unluckiest = curArrayPtr->array[i - curArrayPtr->position[0]];
					}
				}
			}
		}
		if (unluckiest.passangerClass != 0) {	//non empty data
			return unluckiest;
		}
		else {
			std::cerr << "No valid data found";
			return unluckiest;
		}

	}

	TitanicData FindLuckiest() {

		TitanicData luckiest;
		luckiest.fare = 999999;
		ArrayTitanic* curArrayPtr = &firstArray;
		for (unsigned long int i = 0; i < totalLength; i++) {
			if (curArrayPtr->position[1] < i) {
				curArrayPtr = curArrayPtr->nextArrayPtr;
			}
			else {
				if (curArrayPtr->array[i - curArrayPtr->position[0]].passangerClass != 0) {	//non empty data
					if ((curArrayPtr->array[i - curArrayPtr->position[0]].survived == true) and (curArrayPtr->array[i - curArrayPtr->position[0]].fare < luckiest.fare)) {
						luckiest = curArrayPtr->array[i - curArrayPtr->position[0]];
					}
				}
			}
		}
		if (luckiest.passangerClass != 0) {	//non empty data
			return luckiest;
		}
		else {
			std::cerr << "No valid data found";
			return luckiest;
		}

	}

};

ArrayListTitanic ReadFileTitanic(const std::string filename, unsigned long int size, const char del) {
	ArrayListTitanic array = ArrayListTitanic();
	std::fstream file;
	file.open(filename, std::ios::in);
	std::string *lines = new std::string[size];
	unsigned long int i = 0;
	if (!file.is_open())
	{
		std::cerr << "There was en error opening file\n";
		return array;
	}
	while (std::getline(file, lines[i]))
	{
		i++;
		if (i == size) {
			break;
		}
	}
	std::string token;
	TitanicData data;
	for (unsigned long int n = 0; n < i; n++) {

		data = TitanicData();

		token = tokenize(lines[n], del);
		if (token != "") {
			data.survived = (token != "0");
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.passangerClass = std::stoi(token);
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.name = token;
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.gender = token;
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.age = std::stoi(token);
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.siblings = std::stoi(token);
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.parents = std::stoi(token);
		}
		token = tokenize(lines[n], del);
		if (token != "") {
			data.fare = std::stod(token);
		}

		array.Append(data);
	}

	return array;
}

int main()
{
	ArrayListTitanic array = ReadFileTitanic(DATASET_FILE, SIZE, DELIMETER);
	std::cout << "Element length: " << array.Length() << '\n';
	std::cout << "Length: " << array.totalLength << '\n';
	std::cout << "Unluckiest: " << array.FindUnluckiest() << '\n';
	std::cout << "Luckiest: " << array.FindLuckiest() << '\n';
	system("pause");
	return 0;
}