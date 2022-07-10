#include "Citizen.h"
#include "District.h"
namespace electionSystem {
	Citizen::Citizen(const string& name, int id, int YOB, District* district) :id(id), yearOfBirth(YOB), name(name), voted(false), district(district)
	{
	}

	Citizen::Citizen(const Citizen& other)
	{
		(*this) = other;
	}

	Citizen::Citizen(ifstream& infile)
	{
		int nLen;
		infile.read(reinterpret_cast<char*>(&nLen), sizeof(int));
		char* temp = new char[nLen + 1];
		infile.read((temp), sizeof(char)*nLen);
		temp[nLen] = '\0';
		this->name = temp;
		delete[] temp;
		infile.read(reinterpret_cast<char*>(&id), sizeof(int));
		infile.read(reinterpret_cast<char*>(&yearOfBirth), sizeof(int));
		this->district = nullptr;
		infile.read(reinterpret_cast<char*>(&voted), sizeof(bool));
		if (!infile.good())
		{
			throw "Error occurred during attempting to read a citizen from file...";
		}
	}
	Citizen::~Citizen()
	{
	}

	const Citizen& Citizen::operator=(const Citizen& other)
	{
		if (this != &other)
		{
			this->name = other.name;
			this->id = other.id;
			this->yearOfBirth = other.yearOfBirth;
			this->district = other.district;
			this->voted = other.voted;
		}
		return *this;
	}

	//Getters
	const string& Citizen::getName()const
	{
		return this->name;
	}
	int Citizen::getId()const
	{
		return this->id;
	}
	int Citizen::getYearOfBirth()const
	{
		return this->yearOfBirth;
	}
	int Citizen::getDistrictId()const
	{
		return this->district->getId();
	}
	bool Citizen::getVoted()const
	{
		return this->voted;
	}


	//Setters
	bool Citizen::setName(const string& name)
	{
		this->name = name;
		return true;
	}

	bool Citizen::setDistrict(District* district)
	{
		this->district = district;
		return true;
	}

	bool Citizen::setVoted()
	{
		this->voted = true;
		return true;
	}

	ostream& operator<<(ostream& os, const Citizen& c)
	{
		os << c.getName() << " - ID: " << c.getId() << ", year of birth: " << c.getYearOfBirth() << ", district id: " << c.getDistrictId() << endl;
		return os;
	}

	void Citizen::save(ofstream& outfile)const
	{
		int nLen = name.size();
		outfile.write(reinterpret_cast<const char*>(&nLen), sizeof(int));
		outfile.write(name.c_str(), sizeof(char) * nLen);
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
		outfile.write(reinterpret_cast<const char*>(&yearOfBirth), sizeof(int));
		outfile.write(reinterpret_cast<const char*>(&voted), sizeof(bool));
		if (!outfile.good())
			throw "Error occurred during attempting to write a citizen to a file...";
	}
	void Citizen::serialSave(ofstream& outfile)const
	{
		outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
		if (!outfile.good())
			throw "Error occurred during attempting to write citizen id to a file...";
	}
	void Citizen::pSerialSave(ofstream& outfile)const
	{
		int dId = this->district->getId();
		outfile.write(reinterpret_cast<const char*>(&dId), sizeof(int));
		if (!outfile.good())
			throw "Error occurred during attempting to write citizen district id to a file...";
	}
}