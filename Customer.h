class Customer {
private: 
	
	std::string name;
	std::list<std::string> DVDs;
	std::list<std::string> preferences;
	std::list<std::string>::iterator p;
	
public:

	Customer();
	Customer(std::string& aName);

	//ACCESSORS
	std::string getName() const;
	int getNumDVDs() const;
	int getNumPref() const;
	std::list<std::string> getPref() const;
	std::string printDVDs();
	std::string printPrefs();
	bool findDVD(const std::string &dvd_name);
	
	//MODIFIERS
	void addMovie(const std::string& aMovie);
	int addMoviePref(const std::string& aMovie);
	std::string getOldest();
	std::string getNewest();
	void returnOldest();
	void returnNewest();
	void setName(const std::string& aName);
	void resetPrefIter();
	void resetDVDIter();
};

//default constructor
Customer::Customer() {
	std::string name;
	std::list<std::string> DVDs;
	std::list<std::string> preferences;
	std::list<std::string>::iterator p;
}

//formatted constructor
Customer::Customer(std::string& aName) {
	std::list<std::string> DVDs;
	std::list<std::string> preferences;
	std::list<std::string>::iterator p;
	name = aName;
}

//ACCESSORS
std::string Customer::getName() const { 
	return name;
}

int Customer::getNumDVDs() const {
	return DVDs.size();
}

int Customer::getNumPref() const {
	return preferences.size();
}

std::list<std::string> Customer::getPref() const {
	return preferences;
}

void Customer::resetPrefIter() {
	p = preferences.begin();
}
// ^ v I don't know how else to do this. Sorry!
void Customer::resetDVDIter() {
	p = DVDs.begin();
}

std::string Customer::printDVDs() {
	std::string dvd = *p;
	++p;
	return dvd;
}


std::string Customer::printPrefs() {
	std::string dvd = *p;
	++p;
	return dvd;
}

void Customer::setName(const std::string& aName) {
	name = aName;
}

void Customer::addMovie(const std::string& aMovie) { 
	DVDs.push_back(aMovie);
	preferences.remove(aMovie);
}

int Customer::addMoviePref(const std::string& aMovie) { 
	for (p = preferences.begin(); p != preferences.end(); ++p) {
		if (*p == aMovie)
			return 0;
	}
	preferences.push_back(aMovie);
	return 1;
}

void Customer::returnOldest() {
	DVDs.pop_front();
}

void Customer::returnNewest() {
	DVDs.pop_back();
}

std::string Customer::getNewest() {
	return DVDs.back();
}

std::string Customer::getOldest() {
	return DVDs.front();
}

bool Customer::findDVD(const std::string& dvd_name) {
	for(p = DVDs.begin(); p != DVDs.end(); ++p) {
		if (*p == dvd_name)
			return true;
	}
	return false;
}