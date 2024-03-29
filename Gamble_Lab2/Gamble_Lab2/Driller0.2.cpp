// Gamble_Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;

struct drillingRecord {
	double nums[16]; // holds the numeric data, in column order
	std::string strings[2]; // holds the string data, in column order
};

struct drillingArray {
	int capacity; // maximum capacity, in records
	drillingRecord* data; // pointer to array of records
};

drillingArray* doubleDrillingArray(drillingArray* currentDrillingArray)
{
	drillingArray* doubledArray = new drillingArray;

	int newCap = currentDrillingArray->capacity * 2;
	doubledArray->capacity = newCap;
	doubledArray->data = new drillingRecord[newCap];

	for (int i = 0; i < currentDrillingArray->capacity; i++) 
	{
		doubledArray->data[i] = currentDrillingArray->data[i];
	}

	delete currentDrillingArray;
	return doubledArray;
}

// Declaring functions for main
bool hasDifferentDate(string dataValue, int row);
bool hasUniqueTime(string dataValue);
bool isValidFloatData(string dataValue);
void saveRecord(string data, drillingArray* array, int row);
void initiallizeDrillingArray(drillingArray* array);
void printArray(drillingArray* array, int elements);

int main(int argc, char *argv[])
{
	// Setting cin with the file stream as buffer.

	// Alternative way to redirect cin to file
	
	ifstream inFile(argv[1]);
	cin.rdbuf(inFile.rdbuf());
	

	// Declare array for records to store them
	drillingArray* drillData = new drillingArray;
	initiallizeDrillingArray(drillData);

	//Skip first line according to filetype & project specs.
	string line = "";
	getline(cin, line);
	line = "";

	// Loop through each row in the CSV.
	int row = 0;
	int validLines = 0;
	while (getline(cin, line))
	{
		//Increment row.
		row++;

		// Open stream to dataLine.
		stringstream dataLine(line);
		string dataValue = "";

		// Loop through each individual column.
		bool skipRow = false;
		int column = 0;
		while (getline(dataLine, dataValue, ','))
		{
			// Increment Colummn
			column++;

			bool differentDate = false;
			bool hasDuplicateTime = false;

			// Check if the data line has a different date.
			if (column == 1
				&& !hasDifferentDate(dataValue, row)
				&& row > 1)
			{
				cout << "Non-matching date stamp " << dataValue << " at line " << row << ".\n";
				differentDate = true;
				dataValue = "";
				skipRow = true;
				continue;
			}

			// Check if the data lines has duplicate time stamps.
			if (column == 2
				&& !differentDate
				&& hasUniqueTime(dataValue)
				&& row > 1
				&& !skipRow)
			{
				cout << "Duplicate time stamp " << dataValue << " at line " << row << ".\n";
				hasDuplicateTime = true;
				dataValue = "";
				skipRow = true;
				continue;
			}

			// Check if the data on the line is valid float-point data ( data > 0).
			if (column == 3
				&& !differentDate
				&& !hasDuplicateTime
				&& !isValidFloatData(line)
				&& !skipRow)
			{
				cout << "Invalid floating-point data at line " << row << ".\n";
				dataValue = "";
				skipRow = true;
				continue;
			}

			// Reset data Value for loop
			dataValue = "";
		}

		// Print all valid lines
		if (skipRow != true)
		{
			for (int i = 0; i < (int)line.size(); i++)
			{
				if (line.at(i) == ',')
				{
					line.at(i) = ';';
				}
			}
			saveRecord(line, drillData, validLines);
			validLines++;
		}
		// Reset line for loop
		line = "";
	}
	printArray(drillData, validLines);
	return 0;
}

// Function returns true is the date is different from the 1st date in the file.
string baseDate = "";
bool hasDifferentDate(string dataValue, int row)
{

	bool isDifferent = false;
	if (row == 1)
	{
		baseDate = dataValue.c_str();
		return false;
	}
	else
	{
		isDifferent = ((baseDate.compare(dataValue)) == 0);
		return isDifferent;
	}
}

// Function returns true is the time is unqiue.
set<string> timeStamps;
bool hasUniqueTime(string dataValue)
{
	// Find word in set or return last string in set
	set<string>::iterator timeIt = timeStamps.find(dataValue.c_str());

	// If the set is not at the end then the time exists
	if (timeIt != timeStamps.end())
	{
		return true;
	}
	else
	{
		timeStamps.insert(dataValue.c_str());
		return false;
	}
}


// Function that returns true if the argument passed through is considered valid float data (greater than 0).
bool isValidFloatData(string line)
{
	stringstream dataLine(line);

	bool isValid = true;

	string data = "";
	int count = 0;
	while (getline(dataLine, data, ','))
	{
		if (count >= 2)
		{
			float fVal = stof(data);

			if (fVal <= 0.0)
			{
				isValid = false;
			}
		}
		count++;
	}
	return isValid;
}

void saveRecord(string data, drillingArray* array, int lines) 
{
	// Create new drilling record
	drillingRecord* drillRecord = new drillingRecord;
	
	stringstream stringStrm(data);

	if (lines >= array->capacity) 
	{
		array = doubleDrillingArray(array); // Double array if needed
	}

	// Add data values to record
	string value = "";
	int index = 0;
	while (getline(stringStrm, value, ';')) 
	{
		if (index < 2) 
		{
			drillRecord->strings[index] = value;
		}
		else if (index < 18) 
		{
			drillRecord->nums[index - 2] = stof(value);
		}
		index++;
	}
	array->data[lines] = *drillRecord; // Add record to array
}

// initiallize array
void initiallizeDrillingArray(drillingArray* array) 
{
	array->capacity = 10;
	array->data = new drillingRecord[10];
}

// Print the array in reverse order
void printArray(drillingArray* array, int elements) 
{
	int index = elements;
	while(index >= 0){

		string line = "";
		drillingRecord record = array->data[index - 1];
		for (int j = 0; j < 18; j++) 
		{
			if (j < 2)
			{
				cout << record.strings[j].c_str() << ";";
			}
			else if (j < 15) 
			{
				cout << record.nums[j] << ";";
			}
			else if (j == 15) 
			{
				cout << record.nums[j] << "\n";
			}
		}
		index--;
	}
}