#include "UnifiedDistrict.h"
namespace electionSystem {
	UnifiedDistrict::UnifiedDistrict(const string& name, int amountOfRep):District(name, amountOfRep)
	{
	}
	UnifiedDistrict::UnifiedDistrict(ifstream& infile) : District(infile)
	{
	}
	UnifiedDistrict::UnifiedDistrict(const UnifiedDistrict& other) : District(other)
	{
		(*this) = other;
	}
	const UnifiedDistrict& UnifiedDistrict::operator=(const UnifiedDistrict& other)
	{
		if (this != &other)
		{
			District::operator=(other);
		}
		return *this;
	}
	ostream& operator<<(ostream& os, const UnifiedDistrict& d)
	{
		os << "UnifiedDistrict: " << d.getId() << " - Name: " << d.getName() << ", amount of representative: " << d.getAmountOfRep() << endl;
		return os;
	}
	void UnifiedDistrict::toOs(ostream& os)const
	{
		os << "UnifiedDistrict No.";
	}
	void UnifiedDistrict::save(ofstream& outfile)const
	{
		int type = 1;
		outfile.write(reinterpret_cast<const char*>(&type), sizeof(int));
		try { District::save(outfile); }
		catch (const char* msg) { throw msg; }
	}
}