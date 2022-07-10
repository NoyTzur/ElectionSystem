#pragma once
#include"List.h"
#include"Party.h"
#include<iostream>
#include<fstream>
#include<typeinfo>
using namespace std;
namespace electionSystem {
	
	class District
	{
	private:
		static int countId;
		string name;
		int id;
		int amountOfRep;
		float presentOfVoters;
		List<Citizen*> allowedVoters;
		List<Citizen*> elected;
		int countVotes; //how many citizen voted in the district

	public:
		District(const string& name, int amountOfRep);
		District(const District& other);
		District(ifstream& infile);
		virtual ~District();
		friend ostream& operator<<(ostream& os, District& d);
		bool operator!=(const District& d)const;
		const District& operator=(const District& other);
		virtual void toOs(ostream& os)const = 0;

		//Getter
		const string& getName()const;
		int getId()const;
		float getPresentOfVoters()const;
		int getAmountOfRep()const;
		int getCountVotes()const;
		List<Citizen*>& getAllowedVotersList()const;
		List<Citizen*>& getElectedList()const;

		//Setter
		bool setName(const string& name);
		bool setAmountOfRep(int aor);
		bool setPresentOfVoters(float POV);
		bool setAllowedVotersList(const List<Citizen*>& lst);
		bool setElectedList(const List<Citizen*>& lst);
		bool increaseVoteCountByOne();

		bool printAddToElected(const List<Citizen*>& candidates, int x);

		virtual void save(ofstream& outfile)const;
		void pSerialSave(ofstream& outfile)const;

	};
	struct DNode {
		District* info=nullptr;
		List<PNode*> listOfParties;
		~DNode()
		{
			for (auto curr = listOfParties.begin(); curr != listOfParties.end(); ++curr)
			{
				delete (*curr);
			}
		}
		void save(ofstream& outfile) 
		{ 
			try { info->save(outfile); }
			catch (const char* msg) { throw msg; }
		}
		void pSerialSave(ofstream& outfile)
		{
			try { info->getAllowedVotersList().serialSave(outfile); }
			catch (const char* msg) { throw msg; }
			try { info->getElectedList().serialSave(outfile); }
			catch (const char* msg) { throw msg; }
			int size = listOfParties.size();
			outfile.write(reinterpret_cast<const char*>(&size), sizeof(int)); //write the size
			auto curr = listOfParties.begin();
			while (curr != nullptr)
			{
				try { (*curr)->info->serialSave(outfile); }
				catch (const char* msg) { throw msg; }
				outfile.write(reinterpret_cast<const char*>(&((*curr)->amountOfVotes)), sizeof(int));
				++curr;
			}
			if (!outfile.good())
			{
				throw"Error occurred during attempting to write a pDistrict to a file...";
			}
		}
		
	};
}


