class DVD {
private: 
	
	std::string title;
	int copies;
	
public:

	DVD();
	DVD(std::string aTitle, int aCopies);

	//ACCESSORS
	std::string getTitle() const;
	int getCopies() const;
	
	//MODIFIERS
	void removeCopies();
	void addCopies(const int & aCopies);
	void setTitle(const std::string& aTitle);
};

//default constructor
DVD::DVD() {
	copies = 0;
}

//formatted constructor
DVD::DVD(std::string aTitle, int aCopies) {
	title = aTitle;
	copies = aCopies;
}

//ACCESSORS
std::string DVD::getTitle() const { 
	return title;
}

int DVD::getCopies() const {
	return copies;
}

//add to inventory/return a DVD
void DVD::addCopies(const int & aCopies) {
	copies += aCopies;
}

void DVD::setTitle(const std::string& aTitle) {
	title = aTitle;
}

//rent a DVD
void DVD::removeCopies() {
	copies -= 1;
}