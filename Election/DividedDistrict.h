#pragma once
#include "District.h"
namespace electionSystem {
	class DividedDistrict : public District
	{
	public:
		DividedDistrict(const string& name, int amountOfRep);
		DividedDistrict(const DividedDistrict& other);
		DividedDistrict(ifstream& infile);
		const DividedDistrict& operator=(const DividedDistrict& other);
		void toOs(ostream& os)const;
		friend ostream& operator<<(ostream& os, const DividedDistrict& d);
		void save(ofstream& outfile)const;
	};
}