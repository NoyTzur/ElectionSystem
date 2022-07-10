#pragma once
#include"District.h"
#include"Party.h"
#include"List.h"
#include<vector>
namespace electionSystem {
	struct Date {
		int day, month, year;// date of the election
	};
	class Election
	{
	private:
		Date date;
		int amountOfCitizens;
		List<Citizen*> voterBook;
		int amountOfDistricts;
		List<DNode*> districts;
		int amountOfParties;
		List<PNode*> parties;

		void freeCitizens();
		void freeParties();
		void freeDistricts();

		Citizen* findCitizenById(int id); 
		Party* findPartyById(int id);
		District* findDistrictById(int id);

		void increaseTotalPartyVotes(int partyId);
		void increasePartyVotesInDistrict(int partyId, int districtId);
		void zeroTheElectors();

		vector<PNode*> makeSortedPartiesArray(const List<PNode*>& pLst, int flag);
		void printParties(const List<PNode*>& lst, District* district, const List<PNode*>& countElectorsLst);
		Party* findMaxVotes(int districtId);

		void setPC(ifstream& infile);
		void setPD(ifstream& infile);
		void setPP(ifstream& infile);
		
	public:
		Election(Date& date);
		Election(ifstream& infile);
		virtual ~Election();

		//Getters
		Date getDate()const;

		virtual void addDistrict(const string& name, int numOfRep, int districtType);// 1
		virtual void addCitizen(const string& name, int id, int YOB, int DId);// 2
		virtual void addParty(const string& name, int candidId);// 3
		virtual void addCitizenAsPartyCandid(int candidId, int partyId, int DId);// 4
		virtual void display(int x);// 5, 6, 7
		virtual void vote(int id, int partyId);//8
		virtual void displayResult();//9
		virtual void save(const string& name) const;//11
		void setP(ifstream& infile);

		template<class T>
		void printList(List<T>& lst)
		{
			for (auto curr = lst.begin(); curr != lst.end(); ++curr)
			{
				cout << *((*curr)->info) << endl;
			}
		}

	protected:
		virtual void saveE(ofstream& outfile) const;//11
	};
}