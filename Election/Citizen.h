#pragma once
#include<iostream>
#include<string>
using namespace std;
namespace electionSystem {
	class District;
	class Citizen {
	private:
		string name;
		int id;
		int yearOfBirth;
		District* district;
		bool voted;

	public:
		Citizen(const string& name, int id, int YOB, District* district);
		Citizen(const Citizen& other);
		Citizen(ifstream& infile);
		~Citizen();
		friend ostream& operator<<(ostream& os, const Citizen& c);
		const Citizen& operator=(const Citizen& other);

		//Getters
		const string& getName()const;
		int getId()const;
		int getYearOfBirth()const;
		int getDistrictId()const;
		bool getVoted()const;
		
		//Setters
		bool setName(const string& name);
		bool setDistrict(District* district);
		bool setVoted();
		
		void save(ofstream& outfile)const;
		void serialSave(ofstream& outfile)const;
		void pSerialSave(ofstream& outfile)const;
	};
}