
#include "SymbolTable.h"
#include <string>
#include <iostream>

using namespace std;

	// Constructor
	SymbolTable::SymbolTable() {

		class_scope_.clear();
		subroutine_scope_.clear();
		for (int i =0;i < 4;i++) {
			count[i] = 0;
		} 
		//kinds[] = 

	}

	string SymbolTable::fromKind (ST::Kind k) {

		const char* kinds[5] = {"STATIC", "FIELD", "ARG", "VAR", "NONE"};

		return string (kinds[k]);
	}
	
	// Methods
	void SymbolTable::startSubroutine(void){
		subroutine_scope_.clear();
		count[ST::ARG] = 0;
		count[ST::VAR] = 0;

	}

	void SymbolTable::define(string name, string type, ST::Kind kind){
		
		//cout << "define:" << name << " " << type << " " << kind << endl;
		// class cope
		if (kind == ST::STATIC or kind == ST::FIELD) {
			class_scope_[name].type = type;
			class_scope_[name].kind = kind;
			class_scope_[name].index =count[kind];
		} else if (kind == ST::ARG or kind == ST::VAR) {
			subroutine_scope_[name].type = type;
			subroutine_scope_[name].kind = kind;
			subroutine_scope_[name].index = count[kind];
		}

		count[kind]++;

	}
	
	int SymbolTable::varCount(ST::Kind kind){
		return count[kind];
	}	
	

	//auxiliar
	bool hasKey(map<string, Symbol> & scope, string key){
		return !(scope.find(key) == scope.end());
	}

	
	bool SymbolTable::lookup(string name, Symbol& s){
		bool r = true;
		if(hasKey(subroutine_scope_, name))
			s = subroutine_scope_[name];
		else if(hasKey(class_scope_, name))
			s = class_scope_[name];
		else
			r = false ;
		return r;

	}

	ST::Kind SymbolTable::kindOf(string name){
		Symbol s;
		if (lookup (name, s)) {
			return s.kind;
		}
		return ST::NONE;

	}

	string SymbolTable::typeOf(string name){
		Symbol s;
		if (lookup (name, s)) {
			return s.type;
		}
		return "";

	}


	int SymbolTable::indexOf(string name){
		Symbol s;
		if (lookup (name, s)) {
			return s.index;
		}
		return -1;

	}

