#include<iostream>
#include<fstream>
#include"Citizen.h"
#include"Party.h"
#include"District.h"
#include"Election.h"
#include"SimpleElection.h"
#include"UnifiedDistrict.h"
#include <typeinfo>
using namespace std;
using namespace electionSystem;

enum PrimeMenu { CreateNew = 1, Load, getOut };

enum Menu {
	AddDistrict = 1,
	AddCitizen,
	AddParty,
	AddCitizenAsPartyCandid,
	DispalyDistricts,
	DisplayCitizens,
	DisplayParties,
	Vote,
	DisplayResults,
	Exit,
	SaveElection,
	LoadExistElection
};

bool validDate(Date& date)
{
	if (date.year <= 0)
		return false;
	if (date.month < 1 || date.month >12)
		return false;
	if (date.month == 2)
	{
		return (date.day >= 1 && date.day <= 28);
	}
	else if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)
	{
		return (date.day >= 1 && date.day <= 30);
	}
	else
	{
		return (date.day >= 1 && date.day <= 31);
	}
}

void load(string& name, Election** pElection, ifstream& infile)
{
	Election* election = nullptr;
	cout << "Please enter file name to load: ";
	cin >> name;
	infile.open(name, ios::binary);
	if (!infile)
	{
		cout << "ERROR! The file could not be opened! " << endl;
		cout << "ERROR! The Election could not be loaded! " << endl;
		exit(1);
	}
	int type;
	infile.read(reinterpret_cast<char*>(&type), sizeof(int));
	if (!infile.good())
	{
		cout << "ERROR! The reading has FAILD! " << endl;
		exit(1);
	}
	if (type == 1)
	{
		try { election = new Election(infile); }
		catch (const char* msg)
		{
			cout << msg << endl;
		}
		catch (bad_alloc& ex)
		{
			cout << ex.what() << endl;
			exit(1);
		}
	}
	else //type == 2
	{
		try { election = new SimpleElection(infile); }
		catch (const char* msg)
		{
			cout << msg << endl;
		}
		catch (bad_alloc& ex)
		{
			cout << ex.what() << endl;
			exit(1);
		}
	}
	try { election->setP(infile); }
	catch (const char* msg)
	{
		cout << msg << endl;
	}
	catch (bad_alloc& ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
	*pElection = election;
}

int main()
{
	int choice;
	string name;
	int id, partyId, yearOfBirth, districtId, numOfRep, repId;
	int electionType, districtType;
	Election* election = nullptr;
	ifstream infile;
	cout << "Hello and welcome" << endl <<
		"this is an interactive system" << endl;
	cout << endl << "Prime Menu:" << endl <<
		"1. Create new Election Round" << endl <<
		"2. Load exist Election round from file" << endl <<
		"3. Exit" << endl;
	cout << "please enter your choice: ";
	cin >> choice;
	switch (choice)
	{
	case CreateNew:
		Date date;
		do {
			cout << "Please enter a date for the election (day, month, year): ";
			cin >> date.day;
			cin >> date.month;
			cin >> date.year;
		} while (!validDate(date));
		cout << "Please enter the type of the Election: " << endl
			<< "1 for Regular Election and 2 for Simple Election: ";
		cin >> electionType;
		if (electionType == 1)
		{
			try { election = new Election(date); }
			catch (bad_alloc& ex) { cout << ex.what(); exit(1); }
		}
		else
		{
			cout << "please enter the amount of rep: ";
			cin >> numOfRep;
			while (numOfRep < 0)
			{
				cout << "wrong input" << endl;
				cout << "Please enter a Valid amount of rep: ";
				cin >> numOfRep;
			}
			try { election = new SimpleElection(date, numOfRep); }
			catch (bad_alloc& ex) { cout << ex.what(); exit(1); }
		}
		break;
	case Load:
		delete election;
		try { load(name, &election, infile); }
		catch (const char* msg)
		{
			cout << msg << endl;
		}
		catch (bad_alloc& ex)
		{
			cout << ex.what() << endl;
			exit(1);
		}
		break;
	case getOut:
		exit(0);
		break;
	default:
		cout << "ERROR! wrong input!" << endl;
		break;
	}

	while (true)
	{
		cout << endl << "Menu:" << endl <<
			"1. Add District" << endl <<
			"2. Add Citizen" << endl <<
			"3. Add Party" << endl <<
			"4. Add Citizen as a Party member" << endl <<
			"5. Print all Districts" << endl <<
			"6. Print all Citizens" << endl <<
			"7. Print all Parties" << endl <<
			"8. Vote" << endl <<
			"9. Display results" << endl <<
			"10. Exit" << endl <<
			"11. Save Election" << endl <<
			"12. Load Exist Election" << endl;
		cout << "please enter your choice: ";
		try {
			cin >> choice;
			switch (choice)
			{
			case AddDistrict:
				////Distruct type: Unified / Divided
				if (dynamic_cast<SimpleElection*>(election))
				{
					cout << "ERROR! you can't add Districts in this type of Election! " << endl;
					break;
				}
				do {
					cout << "please enter the District Type: " << endl << "1 for Unified and 2 for Divided: ";
					cin >> districtType;
				} while (districtType != 1 && districtType != 2);

				cout << "Please enter a name: ";
				cin >> name;
				cout << "please enter the amount of rep: ";
				cin >> numOfRep;
				while (numOfRep <= 0)
				{
					cout << "wrong input" << endl;
					cout << "Please enter a Valid amount of rep: ";
					cin >> numOfRep;
				}
				try { election->addDistrict(name, numOfRep, districtType); }
				catch (const char* msg) {
					cout << msg;
					exit(1);
				}
				break;
			case AddCitizen:
				cout << "Please enter a name: ";
				cin >> name;

				cout << "please enter an id: ";
				cin >> id;
				while (id < 100000000 || id>999999999)
				{
					cout << "the id should be 9 digits!!" << endl;
					cout << "please enter valid id: ";
					cin >> id;
				}

				cout << "please enter year of birth: ";
				cin >> yearOfBirth;
				while ((election->getDate().year - yearOfBirth) < 18)
				{
					cout << "only 18+ can be a citizen!! " << endl;
					cout << "please enter a valid year of birth: ";
					cin >> yearOfBirth;
				}
				cout << "please enter district id: ";
				cin >> districtId;
				try { election->addCitizen(name, id, yearOfBirth, districtId); }
				catch (const char* msg)
				{
					cout << msg << endl;
				}
				catch (bad_alloc& ex)
				{
					cout << ex.what();
					exit(1);
				}
				break;
			case AddParty:
				cout << "Please enter a name: ";
				cin >> name;
				cout << "please enter the id of the candid for the PM: ";
				cin >> id;
				try { election->addParty(name, id); }
				catch (const char* msg)
				{
					cout << msg << endl;
				}
				catch (bad_alloc& ex)
				{
					cout << ex.what();
					exit(1);
				}
				break;
			case AddCitizenAsPartyCandid:
				cout << "please enter the rep id: ";
				cin >> repId;
				cout << "please enter the party id: ";
				cin >> partyId;
				cout << "please enter district id: ";
				cin >> districtId;
				try { election->addCitizenAsPartyCandid(repId, partyId, districtId); }
				catch (const char* msg)
				{
					cout << msg << endl;
				}
				break;
			case DispalyDistricts:
				election->display(5);
				break;
			case DisplayCitizens:
				election->display(6);
				break;
			case DisplayParties:
				election->display(7);
				break;
			case Vote:
				cout << "please enter your id: ";
				cin >> id;
				cout << "please enter the party id: ";
				cin >> partyId;
				try { election->vote(id, partyId); }
				catch (const char* msg)
				{
					cout << msg << endl;
				}
				break;
			case DisplayResults:
				cout << "Date: " << election->getDate().day << "/" << election->getDate().month << "/" << election->getDate().year << endl;
				try { election->displayResult(); }
				catch (const char* msg)
				{
					cout << msg << endl;
					cout << "Error!! could not display the result properly" << endl;
				}
				catch (bad_alloc& ex)
				{
					cout << ex.what() << endl;
					exit(1);
				}
				break;
			case Exit:
				delete election;
				exit(0);
				break;
			case SaveElection:
				cout << "Please enter file name: ";
				cin >> name;
				try { election->save(name); }
				catch (const char* msg) {
					cout << msg << endl;
					exit(1);
				}
				break;
			case LoadExistElection:
				delete election;
				try { load(name, &election, infile); }
				catch (const char* msg)
				{
					cout << msg << endl;
				}
				catch (bad_alloc& ex)
				{
					cout << ex.what() << endl;
					exit(1);
				}
				break;
			default:
				cout << "WRONG INPUT!!" << endl;
				break;
			}
		}
		catch (...)
		{
			cout << "unexpected Error has occurred!! " << endl;
		}
	}
	delete election;
}