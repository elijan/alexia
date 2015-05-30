/*
* File:   main.cpp
* Author: Aleksia
*
* Created on 27 May 2015, 7:14 PM
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;


//CSV headers used in a file
const string CSV_HEADERS[11] = { "Job ID", "Length", "Start Height", "End Height", "Local Site", "Adequate Access", "Site Condition", "Number Of Sleepers", "Square Face", "Cost", "Sleeper Breakdown" };


const double SLEEPER_HEIGHT = 0.2; //in meters
const double SLEEPER_LENGTH = 2; //meters
const double SLEEPER_COST = 514.72;
const string FILE_NAME = "wallQutoe.csv";



char processStringInput(string validAnswers);
double processDoubleInput();

void displayFile();
void display(string s);
void display_error();
bool validate_char(string input, string validAnswers);
void write_to_file(ofstream fout);


//global variables
int job_id = 0;

double wall_length;
double wall_height_start;
double wall_height_end;


char is_wall_local;
char is_site_accessible;
char site_difficulty;


int base_number_of_sleepers;
double angle;
double bay_width;
double bay_height_diff;
double total_sleepers = 0;
double total_area;
double final_cost;
int number_of_bays;
double cost_addition;
double final_cost_with_extras;
double wall_local_ca;
double is_site_accessible_ca;
double sandy_ca;
double limestone_ca;
double bluestone_ca;

/*
*
*/
int main(int argc, char** argv) {

	char userInput;
	bool invalid = true;
	ofstream fout;

	
	//if file doesnt; exist
	if (!ifstream(FILE_NAME)){

		//create file and insret headers / column names
		fout.open(FILE_NAME, std::ios_base::app);
		for (int i = 0; i <11; i++) {

			fout << CSV_HEADERS[i];
			if (i < 10) {
				fout << ",";
			}
		}

		fout.close();
	}

	
	  //display welcome message

		cout << "\n===========================================================================================================\n";
		cout << "Welcome to the Quotation Assistant. \n";
		cout << "Use this application to generate the required number of sleepers for your job and the initial quotation.!\n";
		cout << "===========================================================================================================\n";

	do{
		
	
		
	 display("Would you like to generate a (N)ew quote or(R)ead in an existing file (N/R)?");
	 userInput = processStringInput("RN");

	

	if (userInput == 'R'){

		displayFile();
	}

	if (userInput == 'N'){

		//we can read the file here and grab the latest project id, and then icnrease by 1;
		job_id++;

		display("What is the length of the wall (in metres)?");
		wall_length = processDoubleInput();

		display("What is the start height of the wall (in metres)?");
		wall_height_start = processDoubleInput();

		display("What is the end height of the wall (in metres)?");
		wall_height_end = processDoubleInput();

		display("Is the wall to be built locally (Y/N)?");
		is_wall_local = processStringInput("YN");

		display("Is there adequate access to the site (Y/N)?");
		is_site_accessible = processStringInput("YN");

		display("What is the difficulty of the site (N)ormal, (S)andy, (L)imestone, or (B)luestone (N/S/L/B)?");
		site_difficulty = processStringInput("NSLB");

		bay_width = (wall_length - SLEEPER_LENGTH);
		angle = atan((wall_height_end - wall_height_start) / bay_width);

		bay_height_diff = (SLEEPER_LENGTH * tan(angle)); //in m
		number_of_bays = round(wall_length / (wall_length - bay_width));


		vector<double> number_of_sleepers(number_of_bays);
		

		for (int i = 0; i <number_of_bays; i++){

			if ((wall_height_start / SLEEPER_HEIGHT - floor(wall_height_start / SLEEPER_HEIGHT)) > 0.5){
				number_of_sleepers[i] = round(wall_height_start / SLEEPER_HEIGHT);
			}
			else{
				number_of_sleepers[i] = floor(wall_height_start / SLEEPER_HEIGHT);
			}

			wall_height_start = wall_height_start + bay_height_diff;

			if (wall_height_start > SLEEPER_HEIGHT / 2){

			}
			total_sleepers += number_of_sleepers[i];

		}

		total_area = total_sleepers * 0.4;

		final_cost = total_area * SLEEPER_COST;


		cout << "The wall will require " << total_sleepers << " sleepers ";
		cout << " will be " << total_area << " m2 and cost $" << final_cost;
		cout << " There will be 5 bays with the following sleeper totals: " << endl;

		//dsiplay number o sleeprs
		for (int i = 0; i <number_of_bays; i++){

			cout << number_of_sleepers[i] << " | ";
		}


		
		// The following if else statements are used to calculate the percentage that is added to the cost if the requirements of the wall exceed the basic needs
		if (is_wall_local == 'N'){
			wall_local_ca = final_cost*0.05;
		}
		else {
			wall_local_ca = 0;
		}

		if (is_site_accessible == 'N'){
			is_site_accessible_ca = final_cost*0.3;
		}
		else{
			is_site_accessible_ca = 0;
		}

		if (site_difficulty == 'S'){
			sandy_ca = final_cost*0.1;

		}
		else{
			sandy_ca = 0;
		}

		if (site_difficulty == 'L'){
			limestone_ca = final_cost*0.15;
		}
		else{
			limestone_ca = 0;
		}
		if (site_difficulty == 'B'){
			bluestone_ca = final_cost*0.3;
		}
		else{
			bluestone_ca = 0;
		}

		// Calculates and displays the overall cost of the quote. Done by adding the cost based on the measurements of the wall and by 
		//adding the extra cost if wall is not locally built, if there is inadequate access or specific building conditions
		final_cost_with_extras = (final_cost + wall_local_ca + is_site_accessible_ca + sandy_ca + limestone_ca + bluestone_ca);

		cout << "Your final cost with extras is $" << final_cost_with_extras << endl;


	
		//write to file
		
		fout.open(FILE_NAME, std::ios_base::app);

		fout << endl << job_id << "," << wall_length << "," << wall_height_start << "," << wall_height_end << "," << is_wall_local << "," << is_site_accessible << "," << site_difficulty;
		fout << "," << total_sleepers << "," << total_area << "," << final_cost <<",\"";

		for (int i = 0; i <number_of_bays; i++){

				fout  << number_of_sleepers[i] <<"," ;
		}

		fout << "\"";

		fout.close();

	}


	display("Would you like to begin a new quotation (Y/N)?");

     if (processStringInput("YN") == 'N'){

		display("Thank You, Good Bye!");
		return 0;

	}

	} while (true);

	return 0;
}

/*
   Fucntion to read and display the file on the screen

  Since order fo columns in a the file and on the screen are differnet, we have to repostion the collumns
  and match them to the desired order
  Retunrs nothing  

*/
void displayFile(){

	ifstream  filestream(FILE_NAME);
	string line;
	///display width 
	const int DISPLAY_OFFSET = 15;
	int width_display;

	const string DISPLAY_HEADERS[6] = { "ID", "Length", "Sqr Face", "Cost", "Nm Slpr", "Slpr BreakDown" };



	for (int i = 0; i <= 5; i++) {
		/* calculate space bewteen rows so that spacing is even and we  end up with thsi effect
		   1312 |sdasda   |axadssa |
		   1    |1        |Ssa     |    
		*/
		
		// we remove charaters from duisplay offset constant
		width_display = DISPLAY_OFFSET - DISPLAY_HEADERS[i].size();

		cout << DISPLAY_HEADERS[i] << setw(width_display) << "|";
	}

	cout << "\n";


	//grab teh first line from the file, whcih is aheader
	//since we have custom headers, e skipp this
	//we just skip it an ddo anythign ith it
	getline(filestream, line);
	//loop through each line in a file
	//our  Slpr Berkadown is enclosed in quotes, adn has commas - so break that first

	while (getline(filestream, line, '"'))
	{
		//rows e need to display
		string rows[sizeof(DISPLAY_HEADERS) / sizeof(DISPLAY_HEADERS[0])]; //rows array size has to match the column size
		stringstream  lineStream(line);
		string        cell;

		int position = 0; //position of the cell in the csv file
		
		//check i teh column is sql Brekadown as it is wrapped in quotes
		if (getline(filestream, line, '"')){

			rows[5] = line;
		}

		//get the comma delimited values in the rest of the line
		while (getline(lineStream, cell, ','))
		{

			//we build displa rows  based on position of the cell in cvs ile
			//because rows in a file dont; match teh display order, we have to reshuffle by lopping through values and replacing positions
			switch (position){

			case 0: //Job ID
				rows[0] = cell;
				break;

			case 1: //Length
				rows[1] = cell;
				break;
			case 8: //Sqr face
				rows[2] = cell;
				break;

			case 9: //Cost
				rows[3] = cell;
				break;

			case 7: //Nmr od Sleepers
				rows[4] = cell;
				break;
			}

			position++;
		}


		//display rows as they are outlined in the asisment
		for (int i = 0; i <= 5; i++) {

			width_display = 15 - rows[i].size();
			cout << rows[i] << setw(width_display) << "|";

		}

		cout << "\n";
	}

}


/*
 Process user Input from comnad line adn displays error until valid values is entered

 @param string to check for vali answers, empty if we lok for double

 @return charaters from a string input
*/

char processStringInput(string validAnswers){

	string  input;

    do {
			cin >> input;
			if (validate_char(input, validAnswers)){
				
				return input[0];

			}

		//if any of conditiosn are invalid, display error
		

	} while (true);


}

/*

Process user inpout expecting double, displays erro until valid value is enterd
@return double from user input

*/
double processDoubleInput(){

	double number;

	do {

		cin >> number;
		if (cin.good()){

			return number;
		}

	  display_error();

	} while (true);

}

//
// function to display error and force user to re itner proper vaklue
//
void display_error(){

	cin.clear();
	//and empty it		
	cin.ignore(200, '\n');

	display("Invalid input; please re-enter.");
}

/*

function to wrap display to command line

*/
void display(string s){

	cout << s << endl;
}

/*

fucntion that validates string input
chekcs or upper case and validanswer istinrgs
@param string input - use input
@param string validAnswer - valid answers for gioven input

@return true or flase

*/
bool validate_char(string input, string validAnswers){

	if (input.length() == 1 && isupper(input[0])){ //if it is an upper
		
		for (int i = 0; i <= validAnswers.length(); i++){

			//if  answer matches the expectedValues
			if (validAnswers[i] == input[0]) {

				return true;

			}
		}


	}

	return false;
}