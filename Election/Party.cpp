#include"Party.h"
namespace electionSystem {
	int Party::countId = 0;

	Party::Party(const string& name, Citizen* candid, int amountOfDistricts) :id(countId), name(name), candid(candid) , amountOfElectors(0)
	{
		countId++;
	}
	Party::Party(const Party& other)
	{
		*this = other;
	}
	Party::Party(ifstream& infile)
	{
		int nLen;
		infile.read(reinterpret_cast<char*>(&nLen), sizeof(int));
		char* temp = new char[nLen + 1];
		infile.read((temp), sizeof(char) * nLen);
		temp[nLen] = '\0';
		this->name = temp;
		delete[] temp;
		infile.read(reinterpret_cast<char*>(&id), sizeof(int));
		infile.read(reinterpret_cast<char*>(&amountOfElectors), sizeof(int));
		this->candid = nullptr;
		if (!infile.good())
		{
			throw "Error occurred during attempting to read a party from file...";
		}
	}

	Party::~Party()
	{
		for (auto curr = candidPerDistrict.begin(); curr != candidPerDistrict.end(); ++curr)
		{
			delete (*curr);
		}
	}

	const Party& Party::operator=(const Party& other)
	{
		if (this != &other)
		{
			this->name = other.name;
			this->id = other.id;
			this->amountOfElectors = other.amountOfElectors;
			this->candid = other.candid;
		}
		return *this;
	}
	bool Party::operator!=(const Party& p)const
	{
		return this->id != p.id;
	}


	//Getters
	const string& Party::getName()const
	{
		return this->name;
	}
	const Citizen* Party::getCandid()const
	{
		return this->candid;
	}
	int Party::getId()const
	{
		return this->id;
	}
	List<CPDNode*>& Party::getCandidPerDistrictList()
	{
		//const_cast<List<CPDNode*>&>
		return (candidPerDistrict);
	}
	int Party::getAmountOfElectors()const
	{
		return this->amountOfElectors;
	}

	List<Citizen*>& Party::getDistrictCandidates(int districtId)
	{
		int i = 0;
		auto curr = candidPerDistrict.begin();
		while (curr != candidPerDistrict.end() && i < districtId)
		{
			i++;
			++curr;
		}
		return (*curr)->listOfCandidates;
	}

	//Setters
	bool Party::setCandid(Citizen* candid)
	{
		this->candid = candid;
		return true;
	}

	bool Party::setElectors(int num)
	{
		this->amountOfElectors = num;
		return true;
	}

	bool Party::addElectors(int num)
	{
		(this->amountOfElectors) += num;
		return true;
	}

	void Party::addCandidToDistrict(Citizen* candid, int DId)
	{
		auto curr = candidPerDistrict.begin();
		for (int i = 0; i < DId; i++)
		{
			++curr;
		}
		(*curr)->listOfCandidates.push_back(candid);
	}

	string Party::candidPerDistrictToString() const
	{
		string cpdSTR;

		for (auto curr = candidPerDistrict.begin(); curr != candidPerDistrict.end(); ++curr)
		{
			auto currlst = (*curr)->listOfCandidates;
			cpdSTR = cpdSTR + "District No. " + to_string((*curr)->districId)+ ": ";
			if (currlst.empty())
			{
				cpdSTR += "There arent candidates.";
			}
			for(auto currC=currlst.begin();currC!=currlst.end();++currC)
			{
				cpdSTR = cpdSTR + (*currC)->getName() + ",";
			}
			cpdSTR = cpdSTR + '\b' + "." + '\n';	
		}
		return cpdSTR;
	}


	ostream& operator<<(ostream& os, const Party& p)
	{
		os << p.getId() << " - Name: " << p.getName() << ", Candidate for PM: " << p.getCandid()->getName() << endl << " The Representatives are: " << endl;
		os << p.candidPerDistrictToString() << endl;
		return os;
	}

	void Party::save(ofstream& outfile)const
	{
		int nLen = name.size();
		outfile.write(reinterpret_cast<const char*>(&nLen), sizeof(int));
		outfile.write(name.c_str(), sizeof(char) * nLen);
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
		outfile.write(reinterpret_cast<const char*>(&amountOfElectors), sizeof(int));
		if (!outfile.good()) { throw"Error occurred during attempting to write a party to a file..."; }
	}
	void Party::serialSave(ofstream& outfile)const
	{
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
		if (!outfile.good()) { throw"Error occurred during attempting to write a pParty to a file..."; }

	}
	void Party::pSerialSave(ofstream& outfile)const
	{
		try { candid->serialSave(outfile); }
		catch (const char* msg) { throw msg; }
		try { candidPerDistrict.serialSave(outfile); }
		catch (const char* msg) { throw msg; }
	}
}