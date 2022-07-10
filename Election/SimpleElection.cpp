#include "SimpleElection.h"
namespace electionSystem {
	SimpleElection::SimpleElection(Date& date, int numOfElectors) :Election(date)
	{
		this->Election::addDistrict("UnitedDistricts", numOfElectors, 2);
	}
	SimpleElection::SimpleElection(ifstream& infile) : Election(infile)
	{
	}

	void SimpleElection::addCitizen(const string& name, int id, int YOB, int DId)
	{
		this->Election::addCitizen(name, id, YOB, 0);
	}

	void SimpleElection::addCitizenAsPartyCandid(int candidId, int partyId, int DId)
	{
		this->Election::addCitizenAsPartyCandid(candidId, partyId, 0);
	}

	void SimpleElection::display(int x)
	{
		switch (x)
		{
		case 5:
			cout << "ERROR!" << endl;
			break;
		case 6:
			Election::display(6);
			break;
		case 7:
			Election::display(7);
			break;
		default:
			break;
		}
	}

	void SimpleElection::save(const string& name) const
	{
		ofstream outfile(name, ios::binary | ios::trunc);
		if (!outfile) { throw"Error occurred during attempting to create a file..."; }
		int type = 2;
		outfile.write(reinterpret_cast<const char*>(&type), sizeof(int)); //write the type
		if (!outfile.good())
		{
			throw"Error occurred during attempting to write to a file...";
		}
		try { saveE(outfile); }
		catch (const char* msg) { throw msg; }
		outfile.close();
	}
}