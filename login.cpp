#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <limits>

struct Account {
	int ID = 0;
	int Privilege;
	std::string _name = "";
	std::string _email = "";
	std::string _passw = "";
};

Account *list[1024];

class _AccountManager {
	private:
		int _ucount;
	public:
		_AccountManager (int);
		void RegisterAcc(int, std::string, std::string, std::string);
		void LoginAcc(std::string, std::string);
		void WriteChanges();
};

_AccountManager::_AccountManager(int _trash){
	bool has_file = true;

	std::ifstream inputFile("db.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error :: Could Not open file\n";
        has_file = false;
    }
	if (has_file) {
    	std::string line;
    	while (std::getline(inputFile, line)) {
        	std::string _cname, _cemail, _cpass;
        	std::stringstream ss(line);
        	std::getline(ss, _cname, '$');
        	std::getline(ss, _cemail, '$');
        	std::getline(ss, _cpass, '$');
			RegisterAcc(0, _cname, _cemail, _cpass);
			// std::cout << " {debug} Registry found: " << &_cname << ": " << _cname << ":" << _cemail << ":" << _cpass << ":\n";
    	}
    	inputFile.close();
	}
}

void _AccountManager::RegisterAcc(int _Priv, std::string _Uname, std::string _Email, std::string _Passw){
	Account *newacc = new Account();
		newacc->Privilege = _Priv;
		newacc->_name = _Uname;
		newacc->_email = _Email;
		newacc->_passw = _Passw;
	list[_ucount] = newacc;
	_ucount++;
}

void _AccountManager::LoginAcc(std::string _uname, std::string _upass) {
	// STEP 1 :: SEARCH FOR THE USER
	Account *aux = NULL;
	Account *fnd = NULL;
	bool Granted = false;

	for (int i = 0; i < _ucount; i++) {
		aux = list[i];
		if (strcmp (_uname.c_str(), aux->_name.c_str()) == 0){
			std::cout << "Account Does Exist" << std::endl;
			fnd = aux;
			// STEP 2 :: CHECK CREDENTIALS
			if (strcmp (_upass.c_str(), aux->_passw.c_str()) == 0) {
				Granted = true;
				std::cout << "success!" <<std::endl;
			}
		} else {
			std::cout << "Account Does NOT Exist" << std::endl;
		}	
	}	
	// STEP 3 :: DEFINE IF GRANT ACCESS
	if (fnd == NULL ) { std::cout << "Account Not exist: error 404" << std::endl; goto failed; }
	if (!Granted) { std::cout << "Access Denied" << std::endl; goto failed;}
	
	std::cout << "Login Granted!! Welcome " << fnd->_name << std::endl;
	failed:
	std::cout << "Login Failed!! Goodbye! " << std::endl;
} 

void _AccountManager::WriteChanges() {
	// STEP 1:: REMOVE THE OLD FILE	
	system ("rm ./db.txt"); 
	// STEP 2:: WRITE USERS INTO FILE
	Account *aux = NULL;
	std::ofstream MyFile("db.txt");
	for (int i = 0; i < _ucount; i++) {
		aux = list[i];
			MyFile << aux->_name << "$" << aux->_email << "$" << aux-> _passw << std::endl;
	}	
	MyFile.close();
} 

int main(){
	_AccountManager *MyUsers = new _AccountManager(0);
	int option = 0;
	std::string _aname, _apass, _amail;
	int _apriv = 0;

	do {
		std::cout << "Please Select an option:\n 0. Write Changes\n 1. Register Account\n 2. Login Account\n 3. exit" << std::endl;
		std::cin >> option;
		switch(option){
			case 0:
				std::cout << "Saving File . . . . ." << std::endl;
				MyUsers->WriteChanges();
			break;
			case 1:
				std::cout<<"Add Account:" << std::endl; 
					std::cout<<"Input first Name: "<<std::endl; std::cin >> _aname;
					std::cout<<"Input Email Addr: "<<std::endl; std::cin >> _amail;
					std::cout<<"Input a Password: "<<std::endl; std::cin >> _apass;
				MyUsers->RegisterAcc(_apriv, _aname, _amail, _apass );
			break;
			case 2:
				std::cout<<"Login Account:" << std::endl; 
					std::cout<<"Input first Name: "<<std::endl; std::cin >> _aname;
					std::cout<<"Input a Password: "<<std::endl; std::cin >> _apass;
				MyUsers->LoginAcc(_aname, _apass);
			break;
			case 3:std::cout<<"exit" << std::endl; break;
			default: std::cout << "Invalid Option" << std::endl; option = 0;break;		
		}
		// if (option > 65) { std::cout << "use numbers!!!" << std::endl;}
		 if (std::cin.fail() || !std::isdigit(option)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
	} while (option != 3);
	return 0;
}
