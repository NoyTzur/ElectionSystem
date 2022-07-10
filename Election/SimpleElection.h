#pragma once
#include "Election.h"
namespace electionSystem{
	class SimpleElection : public Election
	{
	public:
		SimpleElection(Date& date, int numOfElectors);
		SimpleElection(ifstream& infile);
		void addCitizen(const string& name, int id, int YOB, int DId);
		void addCitizenAsPartyCandid(int candidId, int partyId, int DId);
		void display(int x);
		void save(const string& name) const;
	};

}