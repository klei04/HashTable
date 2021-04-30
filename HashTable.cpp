/* 
This is a student list program using a hash table
Author: Kevin Lei
Date: 4/17/2021
 */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <math.h>
#include <fstream>

using namespace std;

//Student struct, contains info and pointer to next Student
struct Student {
  int ID;
  char lastName[20];
  char firstName[20];
  float GPA;
  Student* next = NULL;
};

//Function prototypes
int hashFunction(int hashTableLength, Student* student);
void ADD(Student* newStudent, Student** & hashTable, int & hashTableLength);
void PRINT(Student** hashTable, int hashTableLength);
void DELETE(int deleteID, Student** & hashTable, int hashTableLength);
void rehash(Student** & hashTable, int & hashTableLength);
void GENERATE(int num, Student** & hashTable, int & hashTableLength);

//Main Function
int main() {

  cout << "Welcome to Student List" << endl;

  //Creating initial hash table with a length of 100 and populating with NULL
  int hashTableLength = 100;
  Student** hashTable = new Student*[hashTableLength];
  for (int i = 0; i < hashTableLength; i++) {
    hashTable[i] = NULL;
  }

  char input[10];

  //While loop will run as long as the user doesn't type QUIT
  while (strcmp(input, "QUIT") != 0) {
    cout << "Type ADD, PRINT, DELETE, GENERATE, or QUIT" << endl;
    cin >> input;

    if (strcmp(input, "ADD") == 0) {
      Student* newStudent = new Student;
      cout << "Type the ID of the new student: ";
      cin >> newStudent->ID;
      cout << "Type the last name of the new student: ";
      cin >> newStudent->lastName;
      cout << "Type the first name of the new student: ";
      cin >> newStudent->firstName;
      cout << "Type the GPA of the new student: ";
      cin >> newStudent->GPA;
      ADD(newStudent, hashTable, hashTableLength);
    } else if (strcmp(input, "PRINT") == 0) {
      PRINT(hashTable, hashTableLength);
    } else if (strcmp(input, "DELETE") == 0) {
      int deleteID;
      cout << "Type the ID of the student to be deleted: ";
      cin >> deleteID;
      DELETE(deleteID, hashTable, hashTableLength);
    } else if (strcmp(input, "GENERATE") == 0) {
      int num;
      cout << "Type the number of students to be generated: ";
      cin >> num;
      GENERATE(num, hashTable, hashTableLength);
    } else if (strcmp(input, "QUIT") != 0) {
      cout << "Invalid command" << endl;
    }
  }
  
  cout << "Goodbye" << endl;
  return 0;
}

//Hash function that returns the index of the array
int hashFunction(int hashTableLength, int ID) {
  int index = ID % hashTableLength;
  return index;
}

//ADD function that adds a student to the hash table
void ADD(Student* newStudent, Student** & hashTable, int & hashTableLength) {
  int index = hashFunction(hashTableLength, newStudent->ID);

  //If the position in the array is empty
  if (hashTable[index] == NULL) {
    hashTable[index] = newStudent;
  }
  //Chaining occurs if the position in the array is not empty
  else {
    Student* current = hashTable[index];
    int chainLength = 1;
    while (current->next != NULL) {
      current = current->next;
      chainLength++;
    }
    current->next = newStudent;

    //If the chain length exceeds 3, then the hash table is rehashed
    if (chainLength == 3) {
      rehash(hashTable, hashTableLength);
    }
  }
}

//PRINT function to print out all students in the hash table
void PRINT(Student** hashTable, int hashTableLength) {
  for (int i = 0; i < hashTableLength; i++) {
    Student* current = hashTable[i];
    while (current != NULL) {
      cout << "ID: " << current->ID << endl
	   << "Last Name: " << current->lastName << endl
	   << "First Name: " << current-> firstName << endl
	   << "GPA: " << fixed << setprecision(2) << current->GPA << endl << endl;
      current = current->next;
    }
  }
}

//DELETE function to delete a student from the hash table using the ID as a key
void DELETE(int deleteID, Student** &hashTable, int hashTableLength) {
  int index = hashFunction(hashTableLength, deleteID);
  Student* current = hashTable[index];

  if (current == NULL) {
    cout << "Student not found" << endl;
    return;
  }
  
  if ((current != NULL) && (current->ID == deleteID)) {
    Student* temp = current->next;
    delete current;
    hashTable[index] = temp;
  }
						
  while (current->next != NULL) {
    if (current->next->ID == deleteID) {
      Student* temp = current->next;
      current->next = current->next->next;
      delete temp;
      return;
    }
    current = current->next;
  }
  cout << "Student not found" << endl;
}

//Function that rehashes the hash table in a new hash table with double the length
void rehash(Student** & hashTable, int & hashTableLength) {
  cout << "Rehashing..." << endl;
  
  hashTableLength = hashTableLength * 2;
  Student** newHashTable = new Student*[hashTableLength];
  for (int i = 0; i < hashTableLength; i++) {
    newHashTable[i] = NULL;
  }

  for (int i = 0; i < hashTableLength/2; i++) {
    if (hashTable[i] != NULL) {
      Student* temp = new Student;
      temp->ID = hashTable[i]->ID;
      strcpy(temp->lastName, hashTable[i]->lastName);
      strcpy(temp->firstName, hashTable[i]->firstName);
      temp->GPA = hashTable[i]->GPA;
      temp->next = hashTable[i]->next;

      ADD(hashTable[i], newHashTable, hashTableLength);
      newHashTable[hashFunction(hashTableLength, temp->ID)]->next = NULL;

      while (temp->next != NULL) {
	temp = temp->next;
	Student* current = new Student;
	current->ID = temp->ID;
	strcpy(current->lastName, temp->lastName);
	strcpy(current->firstName, temp->firstName);
	current->GPA = temp->GPA;
	current->next = NULL;

	ADD(current, newHashTable, hashTableLength);
      }
    }
    /*Student* current = hashTable[i];
    while (current != NULL) {
      Student* temp = current;
      current = current->next;
      ADD(temp, newHashTable, hashTableLength);
      //current = current->next;
      }*/
  }

  //delete [] hashTable;
  hashTable = newHashTable;
}

//Function that generates a specific number of random students and adds it to the hash table
void GENERATE(int num, Student** & hashTable, int & hashTableLength) {
  Student* generatedStudents[20];
  for (int i = 0; i < 20; i++) {
    generatedStudents[i] = new Student;
  }

  ifstream lastnames;
  ifstream firstnames;
  srand(time(0));

  firstnames.open("firstnames.txt");
  lastnames.open("lastnames.txt");
  if (firstnames == NULL) {
    cout << "Unable to open firstnames.txt" << endl;
    exit(1);
  } if (lastnames == NULL) {
    cout << "Unable to open lastnames.txt" << endl;
    exit(1);
  }

  char name[20];
  int total = 0;
  while (lastnames >> name) {
    strcpy(generatedStudents[total]->lastName, name);
    total += 1;
  }
  total = 0;
  while (firstnames >> name) {
    strcpy(generatedStudents[total]->firstName, name);
    total += 1;
  }

  //Creating the random students and adding them to the hash table
  int currentID = 0;
  for (int i = 0; i < num; i++) {
    float randomGPA = static_cast<float> (rand()) / static_cast<float> (RAND_MAX/4);
    int randomLastName = rand() % total;
    int randomFirstName = rand() % total;

    Student* newRandomStudent = new Student;
    newRandomStudent->ID = currentID;
    strcpy(newRandomStudent->lastName, generatedStudents[randomLastName]->lastName);
    strcpy(newRandomStudent->firstName, generatedStudents[randomFirstName]->firstName);
    newRandomStudent->GPA = randomGPA;
    
    ADD(newRandomStudent, hashTable, hashTableLength);
    currentID++;
  }

  cout << num << " students added" << endl;
}
