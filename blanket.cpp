#include <iostream>
#include <cstdint>
#include<vector>
#include<fstream>
using namespace std;

typedef uint32_t u32;

struct Blanket {
	int cube_size;
	vector<u32> sets;
	void showSet(int);
	Blanket operator*(const Blanket&);
	bool operator<(const Blanket&);
	void join(int, int);
	void addVal(vector<int>);
	void showAll();
};



/* konwencja: -1 oznacza don't care */

struct Fun {
	int inputsNo, outputsNo;
	int maxVal; 
	vector<vector<int> > cube;
	Fun(int, int);
	void addVal(vector<int>);
	void showCube();
	Blanket createBlanket(int);
	void fromFile(const string&);

};

Fun::Fun(int inp, int outp) {
	inputsNo = inp;
	outputsNo = outp;
	maxVal = 5;
	cube.reserve(inputsNo+outputsNo);
};

void Fun::addVal(vector<int> row) {
	for(int i =0; i < cube.size(); ++i) {
		cube[i].push_back(row[i]);
	};
};

void Fun::showCube() {
	cout << "Tabela prawdy dla funkcji: " << endl;
	for(int i=0; i < cube[0].size(); ++i) {
		for(int j=0; j < inputsNo + outputsNo; ++j) {
			cout << cube[j][i] << " ";
		};
		cout << endl;
	};
	cout << endl;
};

	
	

Blanket Fun::createBlanket(int column) {
	Blanket b;
	int pow2 = 1;
	int bitvect = 0;
	for(int val=0; val < maxVal; ++val) {
		for(int i=0; i < cube[column].size(); ++i) {
			if(cube[column][i]==val || cube[column][i]==-1) {
				bitvect += pow2;
			};
			pow2 *= 2;
		};
		pow2 = 1;
		if(bitvect) {
			cube[column].push_back(bitvect);
		};
	};	
	return b;		
};
	
	
/* 
 * format pliku: liczbawejsc liczbawyjsc
 * pozycja_1_z_cube
 * pozycja_2_z_cube
 * [...]
 */	
void Fun::fromFile(const string& fileName) {
	ifstream f;
	f.open(fileName.c_str());
	f >> inputsNo;
	f >> outputsNo;
	int val;
	cout << "Wczytano funkcje o " << inputsNo << "wejsciach i " 
		<< outputsNo << " wyjsciach z pliku" << fileName << endl;
	vector<int> row[inputsNo+outputsNo];
	while(!f.eof()) {
		for(int i = 0; i < inputsNo + outputsNo; ++i) { 
			f >> val;
			cube[i].push_back(val);
		};
	};	
	f.close();
};

	

void Blanket::showSet(int i) {
	u32 s = sets[i];
	int j = 1;
	cout << "[ ";
	while(s) {
		if(s & 1)
			cout << j << " ";
		++j;
		s = s >> 1;
	};
	cout << "]";
};

void Blanket::showAll() {
	cout << "[ ";
	for(int j=0; j < sets.size(); ++j) {
		showSet(j);
		if(j < sets.size()-1)
			cout << ", ";
	};
	cout << "]" << endl;
};

Blanket Blanket::operator*(const Blanket& b) {
	Blanket product;
	int intersection;
	for(int i=0; i < sets.size(); ++i) {
		for(int j=0; j < b.sets.size(); ++j) {
			intersection = sets[i] & b.sets[j];
			if(intersection != 0)
				product.sets.push_back(intersection);
		};
	};
	return product;
};

bool Blanket::operator<(const Blanket& b) {
	bool is_contained = false;
	for(int i=0; i < sets.size(); ++i) {
		for(int j=0; j < b.sets.size(); ++j) {
			if((sets[i] & b.sets[j]) == sets[i]) {
				is_contained = true;
			};
		};
		if(!is_contained) {
			return false;
		};
		is_contained = false;
	};
	return true;	
};

void Blanket::join(int i, int j) {
	sets[i] = sets[i] | sets[j];
	sets.erase(sets.begin() + j);  // wypadaloby doptymalizowac, std::vector<> jest beznadziejny przy usuwaniu ze srodka
};





int main(void) {
	Blanket b1, b2;
	b1.sets.push_back(0b0101);
	b2.sets.push_back(0b0101);
	b1.sets.push_back(0b0110);
	b2.sets.push_back(0b1000);
	if(b1<b2)
		cout << "b1<b2";
	if(b1<b1)
		cout << "b1<b1";
	
	cout << "Blanket 1: ";
	b1.showAll();
	cout << "Blanket 2: ";
	b2.showAll();
	Blanket b3 = b1 * b2;
	cout << "Iloczyn: ";
	b3.showAll();
	return 0;

};
		
	
	
