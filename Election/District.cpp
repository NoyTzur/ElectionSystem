#include "District.h"
using namespace std;
namespace electionSystem {
	int District::countId = 0;
	class DividedDistrict;
	District::District(const string& name, int aor) :id(countId), countVotes(0), name(name), presentOfVoters(0), amountOfRep(aor)
	{
		countId++;
	}
	District::District(const District& other)
	{
		(*this) = other;
	}
	District::District(ifstream& infile)
	{
		int nLen;
		infile.read(reinterpret_cast<char*>(&nLen), sizeof(int));
		char* temp = new char[nLen + 1];
		infile.read((temp), sizeof(char) * nLen);
		temp[nLen] = '\0';
		this->name = temp;
		delete[] temp;
		infile.read(reinterpret_cast<char*>(&id), sizeof(int));
		infile.read(reinterpret_cast<char*>(&amountOfRep), sizeof(int));
		infile.read(reinterpret_cast<char*>(&presentOfVoters), sizeof(float));
		infile.read(reinterpret_cast<char*>(&countVotes), sizeof(int));
		if (!infile.good())
		{
			throw "Error occurred during attempting to read a district from file...";
		}
	}
	District::~District()
	{
	}

	const District& District::operator=(const District& other)
	{
		if (this != &other)
		{
			this->name = other.name;
			this->id = other.id;
			this->amountOfRep = other.amountOfRep;
			this->presentOfVoters = other.presentOfVoters;
			this->allowedVoters = other.allowedVoters;
			this->elected = other.elected;
		}
		return *this;
	}
	bool District::operator!=(const District& d)const
	{
		return this->id != d.id;
	}
	//Getter
	const string& District::getName()const
	{
		return this->name;
	}
	int District::getId()const
	{
		return this->id;
	}
	float District::getPresentOfVoters()const
	{
		return this->presentOfVoters;
	}
	int District::getCountVotes()const
	{
		return this->countVotes;
	}
	List<Citizen*>& District::getAllowedVotersList()const
	{
		return const_cast<List<Citizen*>&>(this->allowedVoters);
	}
	List<Citizen*>& District::getElectedList()const
	{
		return const_cast<List<Citizen*>&>(this->elected);
	}
	int District::getAmountOfRep()const
	{
		return this->amountOfRep;
	}

	//Setter
	bool District::setName(const string& name)
	{
		this->name = name;
		return true;
	}

	bool District::setPresentOfVoters(float POV)
	{
		this->presentOfVoters = POV;
		return true;
	}
	bool District::setAllowedVotersList(const List<Citizen*>& lst)
	{
		this->allowedVoters = lst;
		return true;
	}
	bool District::setElectedList(const List<Citizen*>& lst)
	{
		this->elected = lst;
		return true;
	}

	bool District::setAmountOfRep(int aor)
	{
		this->amountOfRep = aor;
		return true;
	}

	bool District::printAddToElected(const List<Citizen*>& candidates, int x)
	{
		int i = 0;
		auto curr = candidates.begin();
		if (candidates.empty())
		{
			cout << "NO CANDIDATES! ";
		}
		while (i < x && curr != candidates.end())
		{
			this->elected.push_back((*curr));
			cout << (*curr)->getName() << ",";
			i++;
			++curr;
		}
		cout << '\b' << ".";
		cout << endl;
		if (i == x)
			return true;
		return false;
	}


	bool District::increaseVoteCountByOne()
	{
		(this->countVotes)++;
		return true;
	}

	ostream& operator<<(ostream& os, District& d)
	{
		d.toOs(os);
		os << d.getId() << " - Name: " << d.getName() << ", amount of representative: " << d.getAmountOfRep() << endl;
		return os;
	}

	void District::save(ofstream& outfile)const
	{
		int nLen = name.size();
		outfile.write(reinterpret_cast<const char*>(&nLen), sizeof(int));
		outfile.write(name.c_str(), sizeof(char) * nLen);
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
		outfile.write(reinterpret_cast<const char*>(&amountOfRep), sizeof(int));
		outfile.write(reinterpret_cast<const char*>(&presentOfVoters), sizeof(float));
		outfile.write(reinterpret_cast<const char*>(&countVotes), sizeof(int));
		if (!outfile.good()) { throw"Error occurred during attempting to write a district to a file..."; }
	}
	void District::pSerialSave(ofstream& outfile)const
	{
		try { allowedVoters.serialSave(outfile); }
		catch (const char* msg) { throw msg; }
		try { elected.serialSave(outfile); }
		catch (const char* msg) { throw msg; }
	}
	
}