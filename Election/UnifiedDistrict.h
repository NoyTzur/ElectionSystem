#pragma once
#include "District.h"
namespace electionSystem {
	class UnifiedDistrict : public District
	{
	public:
		UnifiedDistrict(const string& name, int amountOfRep);
		UnifiedDistrict(const UnifiedDistrict& other);
		UnifiedDistrict(ifstream& infile);
		const UnifiedDistrict& operator=(const UnifiedDistrict& other);
		void toOs(ostream& os)const;
		friend ostream& operator<<(ostream& os, const UnifiedDistrict& d);
		void save(ofstream& outfile)const;
	};
}