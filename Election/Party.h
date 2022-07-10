#pragma once
#include"Citizen.h"
#include"List.h"
#include<iostream>
using namespace std;
namespace electionSystem {
	struct CPDNode {
		int districId;
		List<Citizen*> listOfCandidates;
		void serialSave(ofstream& outfile) 
		{ 
			try { listOfCandidates.serialSave(outfile); } 
			catch (const char* msg) { throw msg; }
		}
	};
	class Party
	{
	private:
		static int countId;
		string name;
		int id;
		int amountOfElectors; //how many seats in the knesset
		Citizen* candid;
		List<CPDNode*> candidPerDistrict;

	public:
		Party(const string& name, Citizen* candid, int amountOfDistricts);
		Party(ifstream& infile);
		Party(const Party& other);
		~Party();
		friend ostream& operator<<(ostream& os, const Party& p);
		bool operator!=(const Party& p)const;
		const Party& operator=(const Party& other);

		//Getters
		const string& getName()const;
		const Citizen* getCandid()const;
		int getId()const;
		List<CPDNode*>& getCandidPerDistrictList();
		List<Citizen*>& getDistrictCandidates(int districtId);
		int getAmountOfElectors()const;

		//Setters
		bool setCandid(Citizen* candid);
		bool setElectors(int num);
		bool addElectors(int num);
		void addCandidToDistrict(Citizen* candid, int DId);

		string candidPerDistrictToString() const;

		void save(ofstream& outfile)const;
		void serialSave(ofstream& outfile)const;
		void pSerialSave(ofstream& outfile)const;
	};

	struct PNode {
		Party* info;
		int amountOfVotes;
		void save(ofstream& outfile)
		{
			try { info->save(outfile); }
			catch (const char* msg) { throw msg; }
			outfile.write(reinterpret_cast<const char*>(&amountOfVotes), sizeof(int));
			if (!outfile.good())
			{
				throw"Error occurred during attempting to write a Party to a file...";
			}
		}
		void serialSave(ofstream& outfile)
		{
			try { info->serialSave(outfile); }
			catch (const char* msg) { throw msg; }
		}
			
		void serialAndAmountSave(ofstream& outfile)
		{
			try { this->info->serialSave(outfile); }
			catch (const char* msg) { throw msg; }
			outfile.write(reinterpret_cast<const char*>(&amountOfVotes), sizeof(int));
			if (!outfile.good())
			{
				throw"Error occurred during attempting to write a Party to a file...";
			}
		}

		void pSerialSave(ofstream& outfile) 
		{
			try { info->pSerialSave(outfile); }
			catch (const char* msg) { throw msg; }
		}
	};
}