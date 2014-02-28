// ==================================================================
// Important Note: You are encouraged to read through this provided
//   code carefully and follow this structure.  You may modify the
//   file as needed to complete your implementation.
// ==================================================================

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include <cstdlib>
#include "Customer.h"
#include "DVD.h"

// several type defs to keep things neat
// (hint, these are STL lists)
typedef std::list<DVD> inventory_type;
typedef std::list<Customer> customers_type;
typedef std::list<std::string> preference_type;

// ==================================================================

// Helper function prototypes
std::string read_customer_name(std::istream &istr);
std::string read_dvd_name(std::istream &istr);
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name);

// The main algorithm for DVD distribution
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr);

// ==================================================================


void usage(const char* program_name) {
  std::cerr << "Usage: " << program_name << " <input_file> <output_file>" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --analysis" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved --analysis" << std::endl;
  exit(1);
}


int main(int argc, char* argv[]) {

  if (argc < 3 || argc > 5) {
    usage(argv[0]);
  }

  // open input and output file streams
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: Could not open " << argv[1] << " for reading." << std::endl;
    usage(argv[0]); 
  }
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: Could not open " << argv[2] << " for writing." << std::endl;
    usage(argv[0]); 
  }

  
 
  // optional parameters for extra credit
  bool use_improved_algorithm = false;
  bool print_analysis = false;
  for (int i = 3; i < argc; i++) {
    if (std::string(argv[i]) == "--improved") {
      use_improved_algorithm = true;
    } else if (std::string(argv[i]) == "--analysis") {
      print_analysis = true;
    } else {
      usage(argv[0]);
    }
  }


  // stores information about the DVDs
  inventory_type inventory;
  inventory_type::iterator inv_iter;
  // stores information about the customers
  customers_type customers;
  customers_type::iterator cus_iter;
  
	//initialize empty objects that we'll use frequently
  	Customer aCustomer;
	DVD aDVD;
	int inv_count;
	int checked_count;
	// used as logic gates below
	bool warning;
	bool warning_2;
	
  // read in and handle each of the 8 keyword tokens
  std::string token;
  while (istr >> token) {
		if (token == "dvd") {
			aDVD = DVD();
			std::string dvd_name = read_dvd_name(istr);
			int copies;
			istr >> copies;
			
			//The for loop below had occasional issues if I didn't
			//account for an empty list/initial value
			if (inventory.size() == 0) {
				aDVD.setTitle(dvd_name);
				aDVD.addCopies(copies);
				inventory.push_back(aDVD);
				if (copies > 1) 
					ostr << copies << " copies of " << dvd_name << " added" << std::endl;
				else
					ostr << copies << " copy of " << dvd_name << " added" << std::endl;
			}
			
			//DVD isn't here yet? add them.
			else if (find_DVD(inventory, dvd_name) == inventory.end()) {
				aDVD.setTitle(dvd_name);
				aDVD.addCopies(copies);
				inventory.push_back(aDVD);
				if (copies > 1) 
					ostr << copies << " copies of " << dvd_name << " added" << std::endl;
				else
					ostr << copies << " copy of " << dvd_name << " added" << std::endl;
			}
			//Otherwise, find it and add copies
			else {
				for(inv_iter = inventory.begin(); inv_iter != inventory.end(); ++inv_iter) {
					if (inv_iter->getTitle() == dvd_name) {
						(inv_iter->addCopies(copies));
						if (copies > 1) 
							ostr << copies << " copies of " << dvd_name << " added" << std::endl;
						else
							ostr << copies << " copy of " << dvd_name << " added" << std::endl;
					}
				} 
			}
		}
	
		else if (token == "customer") {
		
			warning = false;
			std::string customer_name = read_customer_name(istr);
			int num_movies;
			istr >> num_movies;
			
			// includes movies not in the DVD inventory.
			for(cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				// a customer of this name is already stored in the system 
				if (cus_iter->getName() == customer_name) {
					ostr << "WARNING: Already have a customer named " << customer_name << std::endl;
					warning = true;
					for (int i = 0; i < num_movies; i++) {
						//ignore the token request and push 
						//the token forward appropriately
						std::string dvd_name = read_dvd_name(istr);
					}
				}
			}	
		
			if (warning != true) {
				aCustomer = Customer(); //have to re-construct here to get rid of old values
				aCustomer.setName(customer_name);
				
				for (int i = 0; i < num_movies; i++) {
					std::string dvd_name = read_dvd_name(istr);
					if (find_DVD( inventory, dvd_name) == inventory.end()) 
						ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
					else if (aCustomer.addMoviePref(dvd_name) == 0) //preference list has duplicates,
						ostr << "WARNING: Duplicate movie " << dvd_name << " on preference list!" << std::endl;
					else
						aCustomer.addMoviePref(dvd_name);
				}
				customers.push_back(aCustomer);
				ostr << "new customer: " << customer_name << std::endl;
			}
		} 
	 
		else if (token == "ship") {
			shipping_algorithm(inventory,customers,ostr);
		} 
	
		else if (token == "return_oldest") {
			warning = true;
			std::string customer_name = read_customer_name(istr);
			
			for(cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				if (cus_iter->getName() == customer_name) {  //find the customer name.
					warning = false;									//there is no error now.
					if (cus_iter->getNumDVDs() > 0) {			//do we have any DVDs?
						ostr << customer_name << " returns " << cus_iter->getOldest() << std::endl;
						//find the DVD and add it back to stock
						for(inv_iter = inventory.begin(); inv_iter != inventory.end(); ++inv_iter) {
							if (inv_iter->getTitle() == cus_iter->getOldest()) 
								(inv_iter->addCopies(1));
						} 
						cus_iter->returnOldest();
					}
					else 
						ostr << "WARNING: " << customer_name << " has no DVDs to return!" << std::endl;
				}
			}
			if (warning == true)				
				ostr << "WARNING: No customer named " << customer_name << std::endl;
		}
		
		else if (token == "return_newest") {
			std::string customer_name = read_customer_name(istr);
			warning = true;
			
			for(cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				if (cus_iter->getName() == customer_name) { 
					warning = false;
					if (cus_iter->getNumDVDs() > 0) {
						ostr << customer_name << " returns " << cus_iter->getNewest() << std::endl;
						//find the DVD and add it back to stock
						for(inv_iter = inventory.begin(); inv_iter != inventory.end(); ++inv_iter) {
							if (inv_iter->getTitle() == cus_iter->getNewest()) 
								(inv_iter->addCopies(1));
						} 
						cus_iter->returnNewest();
					}
					else 
						ostr << "WARNING: " << customer_name << " has no DVDs to return!" << std::endl;
				}
			}
			if (warning == true)				
				ostr << "WARNING: No customer named " << customer_name << std::endl;
		}
		
		else if (token == "print_customer") {
			std::string customer_name = read_customer_name(istr);
			warning = true;
			for(cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				if (cus_iter->getName() == customer_name) { 
					warning = false; 									//we have a customer, no warnings.
					int num_movies = cus_iter->getNumDVDs();	//we use this often, easier to type.
					if (num_movies > 0) {
						ostr << customer_name << " has " << num_movies << " movies:" << std::endl;
						cus_iter->resetDVDIter();					//if we have movies, 
						for (int i = 0; i < num_movies; ++i) { //print them out.
							ostr << "    " << cus_iter->printDVDs() << std::endl;
						}
					}
					else 													//no movies
						ostr << customer_name << " has no movies" << std::endl;
					
					if (cus_iter->getNumPref() > 0) {			 //don't need to do anything if < 0
						cus_iter->resetPrefIter();					 //print out preferences (if any)
						ostr << "  preference list:" << std::endl;
						for (int x = 0; x < cus_iter->getNumPref(); ++x) {
							ostr << "    " << cus_iter->printPrefs() << std::endl;
						}
					}
				}
			}
			if (warning == true)				
				ostr << "WARNING: No customer named " << customer_name << std::endl;
		} 
		
		else if (token == "print_dvd") {
			inv_count = 0;
			checked_count = 0;
			std::string dvd_name = read_dvd_name(istr);
			warning = true;
			//check for DVDs in the inventory
			for(inv_iter = inventory.begin(); inv_iter != inventory.end(); ++inv_iter) {
				if (inv_iter->getTitle() == dvd_name) { 
					warning = false;
					inv_count += inv_iter->getCopies();
				}
			}
			//check for DVDs sent out
			for (cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				if (cus_iter->findDVD(dvd_name))
					checked_count += 1;
			}
			if (warning == true)				
				ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
				
			//massive wall of text of formatting logic gates
			////////FORMATTING
			else {
				ostr << dvd_name << ':' << std::endl;
				if (inv_count > 0) {
					if (checked_count > 1) {
						ostr << "  " << checked_count << " copies checked out ";
						if (inv_count == 0)
							ostr << std::endl;
					}
					else if (checked_count != 0) {
						ostr << "  " << checked_count << " copy checked out ";
						if (inv_count == 0)
							ostr << std::endl;	
					}
				}
				else {
					if (checked_count > 1) {
						ostr << "  " << checked_count << " copies checked out";
						if (inv_count == 0)
							ostr << std::endl;
					}
					else if (checked_count != 0) {
						ostr << "  " << checked_count << " copy checked out";
						if (inv_count == 0)
							ostr << std::endl;	
					}
				}
				
				if (checked_count > 0) {
					if (inv_count > 1)
						ostr << "and " << inv_count << " copies available" << std::endl;
					else if (inv_count != 0)
						ostr << "and " << inv_count << " copy available" << std::endl;
				}
				else {
					if (inv_count > 1)
						ostr << "  " << inv_count << " copies available" << std::endl;
					else if (inv_count != 0)
						ostr << "  " << inv_count << " copy available" << std::endl;
				}
			}
			/////END FORMATING
		}
		
		else if (token == "add_preference") {
			warning = true;
			warning_2 = false;
			std::string customer_name = read_customer_name(istr);
			std::string dvd_name = read_dvd_name(istr);
		
			//the movie is not in the DVD inventory.
			if (find_DVD( inventory, dvd_name) == inventory.end()) {
				ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
				warning_2 = true;
				continue; //don't do anything else if there's no DVD
			}
				
			for(cus_iter = customers.begin(); cus_iter != customers.end(); ++cus_iter) {
				if (cus_iter->getName() == customer_name) {
					warning = false;
					//customer already has this movie in their preference list
					if (cus_iter->findDVD(dvd_name)) {
						ostr << "WARNING: " << cus_iter->getName() <<  " currently has "
						<< dvd_name
						<< '!' << std::endl;
					}
					else if (cus_iter->addMoviePref(dvd_name) == 0) 
						ostr << "WARNING: " << customer_name <<" already has "  << dvd_name 
						<< " on his/her preference list!" << std::endl;
					else if (warning_2 != true)
						aCustomer.addMoviePref(dvd_name);
					}
			}
			if (warning == true)		//no customer with this name exists in the system
				ostr << "WARNING: No customer named " << customer_name << std::endl;
		}
		
		else {
      std::cerr << "ERROR: Unknown token " << token << std::endl;
      exit(1);
    }
  }
}


// ==================================================================


// A customer name is simply two strings, first & last name
std::string read_customer_name(std::istream &istr) {
  std::string first, last;
  istr >> first >> last;
  return first + " " + last;
}


// A movie name is one or more strings inside of double quotes
std::string read_dvd_name(std::istream &istr) {
  std::string token;
  istr >> token;
  assert (token[0] == '"');
  std::string answer = token;
  while (answer[answer.size()-1] != '"') {
    istr >> token;
    answer += " " + token;
  }
  return answer;
}


// A helper function to find a DVD from the inventory
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name) {
  for (inventory_type::iterator inventory_itr = inventory.begin(); 
       inventory_itr != inventory.end(); inventory_itr++) {
	 if (inventory_itr->getTitle() == dvd_name) {
      return inventory_itr;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return inventory.end();
}


// ==================================================================

//
// A basic algorithm for determining which movies to ship to each customer
//
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr) {
 
  ostr << "Ship DVDs" << std::endl;

  // Loop over the customers in priority order
  //
  // Note that we edit the customers list as we go, to add customers
  // to the back of the list if they receive a DVD.  This allows a
  // customer to receive multiple DVDs in one shipment cycle, but only
  // after all other customers have had a chance to receive DVDs.
  //
  customers_type::iterator customer_itr = customers.begin();
  while (customer_itr != customers.end()) {

    // skip this customer if they already have 3 movies or if their
    // preference list is empty (no outstanding requests)
    if (customer_itr->getNumDVDs() >= 3 || customer_itr->getNumPref() == 0) {
      // move on to the next customer
      customer_itr++;
      continue;
    }

    // a helper flag variable
    bool sent_dvd = false;
 
    // loop over the customer's preferences
    const preference_type &preferences = customer_itr->getPref();
    for (preference_type::const_iterator preferences_itr = preferences.begin(); 
         preferences_itr != preferences.end(); preferences_itr++) {
				
      // locate this DVD in the inventory
      inventory_type::iterator inventory_itr = find_DVD(inventory,*preferences_itr);
      if (inventory_itr != inventory.end() && inventory_itr->getCopies() > 0) { 
		
        // if the DVD is available, ship it to the customer!
			
        ostr << "  " << customer_itr->getName() << " receives " << *preferences_itr << std::endl;
        inventory_itr->removeCopies();
        customer_itr->addMovie(*preferences_itr);
		  
        
        // move this customer to the back of the priority queue
        // they will get a chance to receive another DVD, but only
        // after everyone else gets a chance
        customers.push_back(*customer_itr);
        customer_itr = customers.erase(customer_itr);

        // after setting the flag to true, leave the iteration over preferences
        sent_dvd = true;        
        break;
      }
    }

    // if no DVD was sent to this customer, then we move on to the next customer 
    // (do not change this customer's priority for tomorrow's shipment)
    if (!sent_dvd) {
      customer_itr++;
    }
  }


}

// ==================================================================
