/***********************************************************************
* Program:
*    Unit 3, ACLs
*    Brother Wilson, CS470
* Author:
*    James Olney
* Summary:
*    This program tracks a collection of student grades
************************************************************************/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

enum Permission 
{ 
	PUBLIC, 	//cannot edit/view any grade
	STUDENT, 	//can only view own grade
	GRADER, 	//can edit any grade, can't view grades
	PROFESSOR   //can view/edit any grade
};

/**************************************************************
 * USER
 * All the users currently in the system
 *************************************************************/
struct User
{
	const char *name;
	const char *password;
}
const users[] =
{
   {"Bob", "passwordBob"},
   {"Hans","passwordHans"},
   {"Sam", "passwordSam"},
   {"Sue", "passwordSue"},
   {"Sly", "passwordSly"}
};

/***********************************************
* PERMISSIONLEVEL
* based on Bell-LaPadula Model
**********************************************/
class PermissionLevel
{
	public:
		PermissionLevel(int i);
		bool isFileAccessible(string name);
		Permission getPermissions();
		string getUser();
		int getUserId();
	private:
		Permission permission;
		int userIndex;
};

/***********************************************
* PERMISSIONLEVEL
* constructor
**********************************************/
PermissionLevel::PermissionLevel(int i)
{
	userIndex = i;
	
	if (users[userIndex].name == "Bob")
		permission = PROFESSOR;
	else if (users[userIndex].name == "Hans")
		permission = GRADER;
	else if (users[userIndex].name == "Sam" || users[userIndex].name == "Sue" || users[userIndex].name == "Sly")
		permission = STUDENT;
	else
		permission = PUBLIC;
}

/***********************************************
* GETPERMISSIONS
* getter for permissions private variable
**********************************************/
Permission PermissionLevel::getPermissions() 
{ 
	return permission; 
}

/***********************************************
* GETUSER
* getter for user private variable
**********************************************/
string PermissionLevel::getUser() 
{ 
	return users[userIndex].name; 
}

/**********************************************
* GETUSERID
* getter for user id private variable
**********************************************/
int PermissionLevel::getUserId() 
{ 
	return userIndex; 
}

/******************************************
 * FILE
 * contains filename
 *****************************************/
struct File
{
	const char *filename;
};

/****************************************
 * Files
 * All the files currently on the system
 ****************************************/
const File files[3] =
{
   { "/home/cs470/unit3/sam.txt" },
   { "/home/cs470/unit3/sue.txt" },
   { "/home/cs470/unit3/sly.txt" }
};


/*****************************************************************
 *****************************************************************
 *                       Student Grade class                     *
 *****************************************************************
 *****************************************************************/
struct Item
{
	float weight;
	float score;
};

/***************************************************
 * One instance of a student grade
 ***************************************************/
class StudentGrade
{
	public:
		StudentGrade(const File & files);
		~StudentGrade();
		string getLetterGrade(); 					// output letter grade B+
		float  getNumberGrade(); 					// integral number grade 88
		void   displayScores(); 					// display scores on screen
		bool   editScores(PermissionLevel b); 			// interactively edit score
		void   setScore(int iScore, float score);
		float  getScore(int iScore);
		void   setWeight(int iScore, float weight);
		float  getWeight(int iScore);
		string getName() { return name; };
	private:
		bool change;
		string name;                    			// student's name
		vector < Item > scores;         			// list of scores and weightings
		const char * filename;

		void editScore(int);  						// edit one score
};

/**********************************************
 * SET SCORE
 **********************************************/
void StudentGrade::setScore(int iScore, float score)
{
	assert(iScore >= 0 && iScore < scores.size());
	scores[iScore].score = score;
	change = true;
}

/**********************************************
 * GET SCORE
 **********************************************/
float StudentGrade::getScore(int iScore)
{
	assert(iScore >= 0 && iScore < scores.size());
	return scores[iScore].score;
}

/****************************************
 * SET WEIGHT
 ****************************************/
void StudentGrade::setWeight(int iScore, float weight)
{
	assert(iScore >= 0 && iScore < scores.size());
	if (weight >= 0.0)
	{
		scores[iScore].weight = weight;
		change = true;
	}
}

/**********************************************
 * GET WEIGHT
 **********************************************/
float StudentGrade::getWeight(int iScore)
{
	assert(iScore >= 0 && iScore < scores.size());
	return scores[iScore].weight;
}

/***********************************************
 * STUDENT GRADE
 * constructor: read the grades from a file
 **********************************************/
StudentGrade::StudentGrade(const File & file) : change(false)
{
	filename = file.filename;
	assert(filename && *filename);

	ifstream fin(filename); 					// open file
	if (fin.fail())
		return;

	getline(fin, name);							// read name

	Item item;
	while (fin >> item.score >> item.weight)
		scores.push_back(item);					// read scores

	fin.close();
}

/**************************************************
 * DESTRUCTOR
 * Write changes to file if anything changed
 *************************************************/
StudentGrade::~StudentGrade()
{
	assert(filename && *filename);

	if (!change)
		return;

	ofstream fout(filename);
	if (fout.fail())
		return;

	fout << name << endl;

	for (int iScore = 0; iScore < scores.size(); iScore++)
		fout << scores[iScore].score
		<< "\t"
		<< scores[iScore].weight
		<< endl;

	fout.close();
}

/****************************************
 * EDITSCORE
 ***************************************/
void StudentGrade::editScore(int iScoreEdit)
{
	float userInputtedWeight;
	assert(iScoreEdit >= 0 && iScoreEdit < scores.size());

	cout << "Enter grade: ";
	cin >> userInputtedWeight;

	// validate
	while (userInputtedWeight > 100 || userInputtedWeight < 0)
	{
		cout << "Invalid grade.  Select a number between 0 and 100: ";
		cin >> userInputtedWeight;
	}
	setScore(iScoreEdit, userInputtedWeight);

	cout << "Enter the weight for the score or (0) for unchanged: ";
	cin >> userInputtedWeight;

	// validate
	while (userInputtedWeight > 1.0 || userInputtedWeight < 0.0)
	{
		cout << "Invalid weight.  Select a number between 0 and 1: ";
		cin >> userInputtedWeight;
	}
	if (userInputtedWeight != 0.0)
		setWeight(iScoreEdit, userInputtedWeight);

	return;
}


/*********************************************
 * Edit scores until user says he is done
 *******************************************/
bool StudentGrade::editScores(PermissionLevel b)
{
	bool isAuthorized = (b.getPermissions() == PROFESSOR);

	cout << "Editing the scores of " << name << endl;
	cout << "Score list\n";
	cout << "\tScore \tWeight\n";

	for (int iScore = 0; iScore < scores.size(); iScore++)
	{
		cout << "(" << iScore + 1 << ")" << "\t";
		if (isAuthorized)
		{
			float score = getScore(iScore);
			float weight = getWeight(iScore);
			cout << score << "%" << "\t" << weight;
		}
		cout << endl;
	}
	cout << "(0)\tExit\n";

	bool done = false;
	while (!done)
	{
		int iScoreEdit;
		cout << "Which score would you like to edit (0-" << scores.size() << "): ";
		cin >> iScoreEdit;

		while (iScoreEdit > scores.size() || iScoreEdit < 0)
		{
			cout << "Invalid number.  Select a number between 0 and " << scores.size() << ": ";
			cin >> iScoreEdit;
		}
		iScoreEdit--;

		if (iScoreEdit != -1)
		{
			char response;
			editScore(iScoreEdit);
			cout << "Do you want to edit another score? (Y/N) ";
			cin >> response;
			done = (response == 'N' || response == 'n');
		}
		else
			done = true;
	}
	return isAuthorized;
}

/************************************************
 * DISPLAYSCORES
 ***********************************************/
void StudentGrade::displayScores()
{
	if (scores.size() == 0)
		return;
	cout << "Student name:\n\t" << name << endl;
	cout << "Grade:\n\t" << getNumberGrade() << "%"	<< " : " << getLetterGrade() << endl;
	cout << "Detailed score:\n" << "\tScore \tWeight\n";
	for (int iScore = 0; iScore < scores.size(); iScore++)
		cout << "\t" << getScore(iScore) << "%"	<< "\t" << getWeight(iScore) << endl;
	return;
}

/***************************************************
 * Letter Grade include A- and C+
 ***************************************************/
string StudentGrade::getLetterGrade()
{
	const char chGrades[] = "FFFFFFDCBAA";
	int nGrade = (int)getNumberGrade();
	string letterGrade;

	assert(nGrade >= 0.0 && nGrade <= 100.0);

	letterGrade += chGrades[nGrade / 10];

	// add the + and - as necessary
	if (nGrade % 10 >= 7 && nGrade / 10 < 9 && nGrade / 10 > 5)
		letterGrade += "+";
	if (nGrade % 10 <= 2 && nGrade / 10 < 10 && nGrade / 10 > 5)
		letterGrade += "-";
	return letterGrade;
}

/***************************************************
 * GETNUMBERGRADE
 * Number grade guaranteed to be between 0 - 100
 ***************************************************/
float StudentGrade::getNumberGrade()
{
	float possible = 0.0;
	float earned = 0.0;
	for (int iScore = 0; iScore < scores.size(); iScore++)
	{
		earned += scores[iScore].score * scores[iScore].weight;
		possible += scores[iScore].weight;
	}

	if (possible == 0.0)
		return 0.0;
	else
		return (earned / possible);
}

/*****************************************************************
 *****************************************************************
 *                           INTERFACE                           *
 *****************************************************************
 *****************************************************************/
class Interface
{
	public:
		Interface();
		void display();
		void interact(PermissionLevel b);
	private:
		int promptForStudent();
		vector<StudentGrade> students;
};


/*************************************************
 * PROMPTFORSTUDENT
 * Prompt the user for student 
 * Return -1 for none
 *************************************************/
int Interface::promptForStudent()
{
	int userSelection;

	cout << "Which student's grade would you like to review?\n";
	for (int iStudent = 0; iStudent < students.size(); iStudent++)
		cout << '\t' << iStudent + 1 << ".\t" << students[iStudent].getName() << endl;
	cout << "\t0.\tNo student, exit\n" << "> ";

	cin >> userSelection;
	while (userSelection < 0 || userSelection > students.size())
	{
		cout << "Invalid selection.  Please select a number between 1 and "	<< students.size() << " or select -1 to exit\n";
		cout << ">";
		cin >> userSelection;
	}
	return --userSelection;
}

void unauthorizedError(int userSelection)
{
	cout << "Unable to read from the resources for ";
	switch (userSelection)
	{
		case 0:
			cout << "Samual Stevenson" << endl;
			break;
		case 1:
			cout << "Susan Bakersfield" << endl;
			break;
		case 2:
			cout << "Sylvester Stallone" << endl;
			break;
		default:
			cout << "Invalid input" << endl;
			break;
	}
}
/***********************************************
 * INTERACT 
 * update the student records interactively
 ***********************************************/
void Interface::interact(PermissionLevel b)
{
	switch (b.getPermissions())
	{
		case PROFESSOR:
		case GRADER:
			int userSelection;
			while (-1 != (userSelection = promptForStudent()))
			{
				bool isAuthorized = students[userSelection].editScores(b);

				if (isAuthorized) 
				{
					students[userSelection].displayScores();
					cout << "---------------------------------------------------\n";
				}
				else
					unauthorizedError(userSelection);
			}
			break;
		case STUDENT:
			if (b.getUser() == "Sam")
				students[0].displayScores();
			else if (b.getUser() == "Sue")
				students[1].displayScores();
			else
				students[2].displayScores();
	}
	return;
}

/*****************************************************
 * INTERFACE CONSTRUCTOR
 * Populate the grades list from a file
 ****************************************************/
Interface::Interface()
{
	for (int i = 0; i < sizeof(files) / sizeof(File); i++)
	{
		StudentGrade student(files[i]);
		students.push_back(student);
	}
}

/**************************************************
 * DISPLAY
 * Display stuff
 *************************************************/
void Interface::display()
{
	for (int i = 0; i < students.size(); i++)
		students[i].displayScores();
}

/**********************************************
 * PermissionLevel
 * authenticate the user
 *********************************************/
PermissionLevel authenticate()
{
	string name;
	string password;
	bool userAuthentic = false;
	int userIndex = 0;
	
	cout << "Username: ";
	cin >> name;
	cout << "Password: ";
	cin >> password;

	for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) 						//compare with each user
		if (name == string(users[i].name) && password == string(users[i].password)) //is this user existing and password good?
		{
			userAuthentic = true;
			userIndex = i;
		}
	if (!userAuthentic)
		cout << "Failed to authenticate " << name << endl;

	PermissionLevel b(userIndex);
	return b;
}

/*********************************************
 * Main:
 *  open files
 *  edit scores
 *  save files
 ********************************************/
int main()
{
	PermissionLevel b = authenticate();
	Interface interface;
	interface.interact(b);
}