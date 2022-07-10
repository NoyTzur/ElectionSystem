#include "DividedDistrict.h"
namespace electionSystem {
	DividedDistrict::DividedDistrict(const string& name, int amountOfRep):District(name, amountOfRep)
	{
	}
	DividedDistrict::DividedDistrict(ifstream& infile):District(infile)
	{
	}
	DividedDistrict::DividedDistrict(const DividedDistrict& other):District(other)
	{
		(*this) = other;
	}
	const DividedDistrict& DividedDistrict::operator=(const DividedDistrict& other)
	{
		if (this != &other)
		{
			District::operator=(other);
		}
		return *this;
	}
	ostream& operator<<(ostream& os, const DividedDistrict& d)
	{
		os << "DividedDistrict: " << d.getId() << " - Name: " << d.getName() << ", amount of representative: " << d.getAmountOfRep() << endl;
		return os;
	}
	void DividedDistrict::toOs(ostream& os)const
	{
		os << "DividedDistrict No.";
	}
	void DividedDistrict::save(ofstream& outfile)const
	{
		int type = 2;
		outfile.write(reinterpret_cast<const char*>(&type), sizeof(int));
		try { District::save(outfile); }
		catch (const char* msg) { throw msg; }
	}

}