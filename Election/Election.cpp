#include"Election.h"
#include"UnifiedDistrict.h"
#include"DividedDistrict.h"
#include<algorithm>
#include<iostream>
#include<fstream>
using namespace std;

namespace electionSystem {
	Election::Election(Date& date):date(date), amountOfDistricts(0), amountOfParties(0), amountOfCitizens(0)
	{
	}
	Election::Election(ifstream& infile)
	{
		try {
			int temp;
			infile.read(reinterpret_cast<char*>(&this->date), sizeof(Date)); //read the date
			infile.read(reinterpret_cast<char*>(&this->amountOfCitizens), sizeof(int));
			infile.read(reinterpret_cast<char*>(&temp), sizeof(int)); //read the size and ignore it

			//Load the voter book
			for (int i = 0; i < amountOfCitizens; i++)
			{
				Citizen* temp = new Citizen(infile);
				voterBook.push_back(temp);
			}
			infile.read(reinterpret_cast<char*>(&this->amountOfDistricts), sizeof(int));
			infile.read(reinterpret_cast<char*>(&temp), sizeof(int)); //read the size and ignore it
			//Load the districts
			for (int i = 0; i < amountOfDistricts; i++)
			{
				int type;
				District* temp;
				infile.read(reinterpret_cast<char*>(&type), sizeof(int)); //read the type
				if (type == 1)
				{
					temp = new UnifiedDistrict(infile);
				}
				else
				{
					temp = new DividedDistrict(infile);
				}
				DNode* dn = new DNode();
				dn->info = temp;
				districts.push_back(dn);
			}
			infile.read(reinterpret_cast<char*>(&this->amountOfParties), sizeof(int));
			infile.read(reinterpret_cast<char*>(&temp), sizeof(int)); //read the size and ignore it
			//Load the parties
			for (int i = 0; i < amountOfParties; i++)
			{
				Party* temp = new Party(infile);
				PNode* pn = new PNode();
				pn->info = temp;
				infile.read(reinterpret_cast<char*>(&pn->amountOfVotes), sizeof(int)); //read the size and ignore it
				parties.push_back(pn);
			}
		}
		catch (const char* msg) 
		{
			freeCitizens();
			freeDistricts();
			freeParties();
			throw msg;
		}
		catch(bad_alloc& ex)
		{
			freeCitizens();
			freeDistricts();
			freeParties();
			throw ex;
		}
	}
	Election::~Election()
	{
		freeCitizens();
		freeDistricts();
		freeParties();
	}

	void Election::freeCitizens()
	{
		auto curr = this->voterBook.begin();
		for(;curr!=voterBook.end();++curr)
		{
			delete (*curr);
		}
	}
	void Election::freeParties()
	{
		auto curr = this->parties.begin();
		for (; curr != parties.end(); ++curr)
		{
			delete (*curr)->info;
			delete (*curr);
		}
	}
	void Election::freeDistricts()
	{
		auto curr = this->districts.begin();
		for (; curr != districts.end(); ++curr)
		{
			delete (*curr)->info;
			delete (*curr);
		}
	}

	Date Election::getDate()const
	{
		return date;
	}

	void Election::addDistrict(const string& name, int numOfRep, int districtType)
	{
		District* temp;
		try {
			if (districtType == 1)
			{
				temp = new UnifiedDistrict(name, numOfRep);
			}
			else
			{
				temp = new DividedDistrict(name, numOfRep);
			}

			DNode* tempD = new DNode();
			tempD->info = temp;
			tempD->listOfParties = this->parties;
			this->districts.push_back(tempD);
			for (auto curr = parties.begin(); curr != parties.end(); ++curr)
			{
				CPDNode* tempCPD = new CPDNode();
				tempCPD->districId = amountOfDistricts;
				(*curr)->info->getCandidPerDistrictList().push_back(tempCPD);
			}
			(this->amountOfDistricts)++;
		}
		catch (bad_alloc& ex)
		{
			throw ex.what();
		}
	}

	void Election::addCitizen(const string& name, int id, int YOB, int DId)
	{
		District* district = findDistrictById(DId);
		if (district == nullptr) 
			throw "Error!! district doesnt exist!!";
		else if (findCitizenById(id) != nullptr)
			throw "Error!! the id is taken!!";
		try {
			Citizen* temp = new Citizen(name, id, YOB, district);
			this->voterBook.push_back(temp);
			district->getAllowedVotersList().push_back(temp);//add to allowed voters in district
		}
		catch (bad_alloc& ex) { throw ex; }
		(this->amountOfCitizens++);
	}

	void Election::addParty(const string& name, int candidId)
	{
		Citizen* candid = findCitizenById(candidId);
		if (candid == nullptr)
		{
			throw("wrong input!!! the candid does not exist");
		}
		try {
			Party* temp = new Party(name, candid, amountOfDistricts);
			this->parties.push_back(new PNode{ temp,0 });
			int i = 0;
			for (auto curr = districts.begin(); curr != districts.end(); ++curr)//for each district adds the new party
			{
				(*curr)->listOfParties.push_back(new PNode{ temp, 0 });
				CPDNode* tcpd = new CPDNode();
				tcpd->districId = i;
				temp->getCandidPerDistrictList().push_back(tcpd);
				i++;
			}
		}
		catch (bad_alloc& ex) { throw ex; }
		(this->amountOfParties)++;
	}

	void Election::addCitizenAsPartyCandid(int candidId, int partyId, int DId)
	{
		Citizen* candid = findCitizenById(candidId);
		Party* party = findPartyById(partyId);

		if (candid == nullptr && party == nullptr)
		{
			throw "wrong input!! cadidate and party are not exist.";
		}
		if (candid == nullptr)
		{
			throw "wrong input!! cadidate doesnt exist.";
		}
		if (party == nullptr)
		{
			throw "wrong input!! party doesnt exist.";
		}
		party->addCandidToDistrict(candid, DId);
	}
	
	void Election::display(int x)
	{
		switch (x)
		{
		case 5:
			printList<DNode*>(districts);
			break;
		case 6:
			for (auto curr= voterBook.begin();curr!=voterBook.end();++curr)
			{
				cout << *(*curr) << endl;
			}
			break;
		case 7:
			printList<PNode*>(parties);
			break;
		default:
			break;
		}
	}
	void Election::increaseTotalPartyVotes(int partyId)
	{
		bool found = false;
		auto curr = parties.begin();
		while (curr != nullptr && !found)
		{
			if ((*curr)->info->getId() == partyId)
				found = true;
			else
				++curr;
		}
		if (found)
			((*curr)->amountOfVotes)++;
	}

	void Election::increasePartyVotesInDistrict(int partyId, int districtId)
	{
		auto temp = districts.begin();
		for (int i = 0; i < districtId; i++) { ++temp; }
		auto curr = (*temp)->listOfParties.begin();
		int i = 0;
		while (curr != (*temp)->listOfParties.end() && i < partyId) { i++;	++curr; }
		((*curr)->amountOfVotes)++;
	}

	void Election::vote(int id, int partyId)
	{
		Citizen* citizen = findCitizenById(id);
		Party* party = findPartyById(partyId);
		if (citizen == nullptr)
		{
			throw "wrong input!! cadidate doesnt exist.";
		}
		if (party == nullptr)
		{
			throw "wrong input!! party doesnt exist.";
		}
		if (citizen->getVoted())
		{
			throw"the citizen has already voted";
		}
		District* district = findDistrictById(citizen->getDistrictId());
		district->increaseVoteCountByOne();
		citizen->setVoted();
		increaseTotalPartyVotes(partyId); ///increas total vote of the party
		increasePartyVotesInDistrict(partyId, citizen->getDistrictId());///increase party votes in district 
	}

	Party* Election::findMaxVotes(int districtId)
	{
		auto dist = districts.begin();
		for (int i = 0; i < districtId; i++) { ++dist; }
		auto curr =(*dist)->listOfParties.begin();
		int max = (*curr)->amountOfVotes;
		Party* maxParty = (*curr)->info;
		++curr;
		while (curr != (*dist)->listOfParties.end())
		{
			if ((*curr)->amountOfVotes > max)
			{
				max = (*curr)->amountOfVotes;
				maxParty = (*curr)->info;
			}
			++curr;
		}
		return maxParty;
	}

	
	vector<PNode*> Election::makeSortedPartiesArray(const List<PNode*>& pLst, int flag)
	{
		vector<PNode*> arr;
		auto curr = pLst.begin();
		while (curr != pLst.end())
		{
			arr.push_back((*curr));
			++curr;
		}
		if (flag)//final sort
			sort(arr.begin(), arr.end(), [](PNode* pn1, PNode* pn2)->bool { return (pn1->info->getAmountOfElectors() > pn2->info->getAmountOfElectors()); });
		else
			sort(arr.begin(), arr.end(), [](PNode* pn1, PNode* pn2)->bool { return (pn1->amountOfVotes >= pn2->amountOfVotes); });

		return arr;
	}

	void Election::zeroTheElectors()
	{
		auto currP = parties.begin();
		while (currP != parties.end())
		{
			(*currP)->info->setElectors(0);
			++currP;
		}
	}

	void Election::printParties(const List<PNode*>& lst, District* district, const List<PNode*>& countElectorsLst)
	{
		auto curr = lst.begin();
		auto currE = countElectorsLst.begin();
		while (curr != lst.end())
		{
			float precentage;
			int amountOfElectors;
			if (district->getCountVotes() != 0)
			{
				precentage = (static_cast<float>((*curr)->amountOfVotes) / district->getCountVotes());
			}
			else
				precentage = 0;
			cout << "Party: " << (*curr)->info->getName() << endl;
			cout << "Amount of votes: " << (*curr)->amountOfVotes << endl;
			cout << "Percentage: " << precentage * 100 << "%" << endl;
			cout << "the candidates are: ";
			List<Citizen*>& candidates = (*curr)->info->getDistrictCandidates(district->getId());
			amountOfElectors = static_cast<int>(district->getAmountOfRep() * precentage);
			if (!district->printAddToElected(candidates, amountOfElectors))
				cout << "there are not enough candidates" << endl;
			DividedDistrict* temp = dynamic_cast<DividedDistrict*>(district);
			if (temp)
			{
				(*curr)->info->addElectors(amountOfElectors);
				(*currE)->amountOfVotes=amountOfElectors;   //actually electors
			}
			++curr;
			if (!countElectorsLst.empty())
			{
				++currE;
			}
		}
	}

	void Election::displayResult()
	{
		auto currD = districts.begin();
		zeroTheElectors();
		while (currD != districts.end()) {
			cout << endl << *((*currD)->info) << endl; //prints the District name and its AmountOfRep
			int size = (*currD)->info->getAllowedVotersList().size(); //amount of citizens in district
			int count = (*currD)->info->getCountVotes(); //amount of citizens who actually vote in district
			if (size == 0)
				throw"ERROR!! divided by zero!!";
			(*currD)->info->setPresentOfVoters(static_cast<float>(count) / size);//update the vote precentage

			List<PNode*> countElectors;
			DividedDistrict* dTemp = dynamic_cast<DividedDistrict*>((*currD)->info);
			if (dTemp)
			{
				for (auto curr = (*currD)->listOfParties.begin(); curr != (*currD)->listOfParties.end(); ++curr)
				{
					try { countElectors.push_back(new PNode{ (*curr)->info,(*curr)->amountOfVotes }); }
					catch (bad_alloc& ex) { throw ex; }
				}
			}

			//sort parties for each district and print the Head and the number of electors
			printParties((*currD)->listOfParties, (*currD)->info, countElectors); //print the parties of the district
			if (dTemp)
			{
				vector<PNode*> arr = makeSortedPartiesArray(countElectors, 0);
				for (int i = 0; i < amountOfParties && arr[i]->amountOfVotes>0; i++)
				{
					cout << "candidate: " << arr[i]->info->getCandid()->getName();
					cout << " " << arr[i]->amountOfVotes << " electors " << endl;
				}
			}
			cout << "vote prcentage in district: " << ((*currD)->info->getPresentOfVoters()) * 100 << "%" << endl;
			UnifiedDistrict* temp = dynamic_cast<UnifiedDistrict*>((*currD)->info);
			if (temp)
			{
				////find the party with the max votes
				///add to the winning party the amount of seats that the district is worth!
				Party* districtWinner = findMaxVotes((*currD)->info->getId());
				cout << "Candidate for PM: " << districtWinner->getCandid()->getName() << endl;
				districtWinner->addElectors((*currD)->info->getAmountOfRep());
			}
			for (auto curr = countElectors.begin(); curr != countElectors.end(); ++curr){ delete (*curr); } 
			++currD;
		}
		vector<PNode*> arr = makeSortedPartiesArray(parties, 1);
		///all the parties will be displayed from the highest electors to the lowest
		///for each party display the head and the total electors, total votes, 

		for (int i = 0; i < amountOfParties; i++)
		{
			cout << "The PM candidate: " << arr[i]->info->getCandid()->getName() << endl;
			cout << "Amount of electors: " << arr[i]->info->getAmountOfElectors() << endl;
			cout << "Amount of votes: " << arr[i]->amountOfVotes << endl;
		}
	}

	Citizen* Election::findCitizenById(int id)
	{
		bool hasFound = false;
		auto curr = this->voterBook.begin();
		while (curr != voterBook.end() && !hasFound)
		{
			if ((*curr)->getId() == id)
				hasFound = true;
			else
				++curr;
		}
		if (hasFound)
			return (*curr);
		return nullptr;
	}

	Party* Election::findPartyById(int id)
	{
		bool found = false;
		auto curr = parties.begin();
		while (curr != nullptr && !found)
		{
			if ((*curr)->info->getId() == id)
				found = true;
			else
				++curr;
		}
		if (found)
			return (*curr)->info;
		return nullptr;
	}
	District* Election::findDistrictById(int id)
	{
		auto curr = districts.begin();
		bool hasFound = false;
		while (curr != districts.end() && !hasFound)
		{
			if ((*curr)->info->getId() == id)
				hasFound = true;
			else
				++curr;
		}
		if (hasFound)
			return (*curr)->info;
		return nullptr;
	}

	void Election::saveE(ofstream& outfile) const
	{
		try {
			outfile.write(reinterpret_cast<const char*>(&this->date), sizeof(Date)); //write the date
			outfile.write(reinterpret_cast<const char*>(&this->amountOfCitizens), sizeof(int));
			voterBook.save(outfile);
		}
		catch (const char* msg) { throw msg; }
		try {
			outfile.write(reinterpret_cast<const char*>(&this->amountOfDistricts), sizeof(int));
			districts.save(outfile);
		}
		catch (const char* msg) { throw msg; }
		try{
		outfile.write(reinterpret_cast<const char*>(&this->amountOfParties), sizeof(int));
		parties.save(outfile);
		}
		catch (const char* msg) { throw msg; }

		try { voterBook.pSerialSave(outfile); }
		catch (const char* msg) { throw msg; }
		try { districts.pSerialSave(outfile); }
		catch (const char* msg) { throw msg; }
		try { parties.pSerialSave(outfile); }
		catch (const char* msg) { throw msg; }
	}

	void Election::save(const string& name) const
	{
		ofstream outfile(name, ios::binary | ios::trunc);
		if (!outfile) { throw"Error occurred during attempting to create a file..."; }
		int type = 1;
		outfile.write(reinterpret_cast<const char*>(&type), sizeof(int)); //write the type
		if (!outfile.good()) { throw"Error occurred during attempting to write to a file..."; }
		try { saveE(outfile); }
		catch (const char* msg) { throw msg; }
		outfile.close();
	}
	void Election::setPC(ifstream& infile)
	{
		try {
			int dId;
			auto curr = voterBook.begin();
			infile.read(reinterpret_cast<char*>(&dId), sizeof(int));//reads the size and ignore it
			while (curr != voterBook.end())
			{
				infile.read(reinterpret_cast<char*>(&dId), sizeof(int));
				(*curr)->setDistrict(findDistrictById(dId));
				++curr;
			}
		}
		catch (...) { throw; }
	}
	void Election::setPD(ifstream& infile)
	{
		try {
			int size, id;
			infile.read(reinterpret_cast<char*>(&size), sizeof(int)); //reads the size and ignore it
			auto curr = districts.begin();
			while (curr != districts.end())
			{
				infile.read(reinterpret_cast<char*>(&size), sizeof(int)); //reads the size of allowedVoters CList
				for (int i = 0; i < size; i++)
				{
					infile.read(reinterpret_cast<char*>(&id), sizeof(int)); //reads the id
					(*curr)->info->getAllowedVotersList().push_back(findCitizenById(id));
				}
				infile.read(reinterpret_cast<char*>(&size), sizeof(int)); //reads the size of elected CList
				for (int i = 0; i < size; i++)
				{
					infile.read(reinterpret_cast<char*>(&id), sizeof(int)); //reads the id
					(*curr)->info->getElectedList().push_back(findCitizenById(id));
				}
				infile.read(reinterpret_cast<char*>(&size), sizeof(int)); //reads the size of listOfParties in district
				for (int i = 0; i < size; i++)
				{
					infile.read(reinterpret_cast<char*>(&id), sizeof(int)); //reads the id
					PNode* pn = new PNode();
					pn->info = findPartyById(id);
					infile.read(reinterpret_cast<char*>(&pn->amountOfVotes), sizeof(int)); //reads the amountOfVotes
					(*curr)->listOfParties.push_back(pn);
				}
				++curr;
			}
		}
		catch (...) { throw; }
	}
	void Election::setPP(ifstream& infile)
	{
		try {
			int size, id;
			auto curr = parties.begin();
			infile.read(reinterpret_cast<char*>(&size), sizeof(int));//reads the size and ignore it
			while (curr != nullptr)
			{
				infile.read(reinterpret_cast<char*>(&id), sizeof(int));
				(*curr)->info->setCandid(findCitizenById(id));

				for (int j = 0; j < amountOfDistricts; j++)
				{
					CPDNode* cpd = new CPDNode();
					cpd->districId = j;
					(*curr)->info->getCandidPerDistrictList().push_back(cpd);
				}
				auto CPDcurr = (*curr)->info->getCandidPerDistrictList().begin();
				infile.read(reinterpret_cast<char*>(&size), sizeof(int));//reads the size and ignore
				while (CPDcurr != nullptr)
				{

					infile.read(reinterpret_cast<char*>(&size), sizeof(int));//reads the size of the candid CList

					for (int i = 0; i < size; i++)
					{
						infile.read(reinterpret_cast<char*>(&id), sizeof(int));
						(*CPDcurr)->listOfCandidates.push_back(findCitizenById(id));
					}
					++CPDcurr;
				}
				++curr;
			}
		}
		catch (...) { throw; }
	}
	void Election::setP(ifstream& infile)
	{
		try {
			setPC(infile);
			setPD(infile);
			setPP(infile);
		}
		catch (...) { throw; }
	}
}