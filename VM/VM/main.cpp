#include <vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

string get_type(string input,vector<string> line) {
	if (input == "local") {
		return "LCL";
	}
	else if (input == "this") {
		return "THIS";
	}
	else if (input == "argument") {
		return "ARG";
	}
	else if (input == "static") {
		return "static";
	}
	else if (input == "constant") {
		return "const";
	}
	else if (input == "that") {
		return "THAT";
	}
	else if (input == "temp") {
		return "5";
	}
	else if (input == "pointer") {
		if (line[2] == "0") {
			return "THIS";
		}
		else if (line[2] == "1") {
			return "THAT";
		}
		
	}
}

void process_Push(vector<string> line) {
	ofstream file;
	file.open("PointerTest.asm", std::ios_base::app);
	string type = get_type(line[1],line);
	string val = line[2];
	if (type != "const") {
		file << "@" + val + "\n";
		file << "D=A\n";
		file << "@" + type + "\n";
		file << "A=D+M\n";
		file << "D=M\n";
		file << "@SP\n";
		file << "A=M\n";
		file << "M=D\n@SP\nM=M+1\n";
	}
	else {
		file << "@" + val + "\n";
		file << "D=A\n";
		file << "@SP\n";
		file << "A=M\n";
		file << "M=D\n";
		file << "@SP\n";
		file << "M=M+1\n";
	}

}
void process_Pop(vector<string> line) {
	ofstream file;
	file.open("PointerTest.asm", std::ios_base::app);
	string type = get_type(line[1],line);
	string val = line[2];
	file << "@" + type + "\n";
	file << "D=M\n";
	file << "@" + val + "\n";
	file << "D=D+A\n";
	file << "@temp\n";
	file << "M=D\n";
	file << "@SP\n";
	file << "AM=M-1\n";
	file << "D=M\n";
	file << "@temp\n";
	file << "A=M\n";
	file << "M=D\n";

}
void process_Arith(vector<string> line) {
	ofstream file;
	file.open("PointerTest.asm", std::ios_base::app);
	if (line[0] == "not") {
		file << "@SP\n";
		file << "A=M\n";
		file << "A=A-1\n";
		file << "D=M\n";
		file << "D=!D\n";
		file << "M=D\n";
	}
	else {
		file << "@SP\n";
		file << "AM=M-1\n";
		file << "D=M\n";
		file << "A=A-1\n";
		if (line[0] == "add") { file << "M=M+D\n"; }
		else { file << "M=M-D\n"; }
	}
}

void process_comp(vector<string> line){
	ofstream file;
	file.open("PointerTest.asm", std::ios_base::app);
	file << "D=M\n";
	if (line[0] == "gt") {
		file << "@$RIP$nnn \n";
		file << "D=A\n";
		file << "@$$GT\n"; // or $$GT or $$LT 
		file << "0;JMP";
			file<<"($RIP$nnn)\n";
	}
	else if (line[0] == "eq") {
		file << "@$RIP$nnn \n";
		file << "D=A\n";
		file << "@$$EQ\n"; // or $$GT or $$LT 
		file << "0;JMP";
		file << "($RIP$nnn)\n";
	}
	else if (line[0] == "lt") {
		file << "@$RIP$nnn \n";
		file << "D=A\n";
		file << "@$$LT\n"; // or $$GT or $$LT 
		file << "0;JMP";
		file << "($RIP$nnn)\n";
	}
}
void processLine(string line) {
	istringstream stream(line);
	vector<string> keys;
	string temp;
	while (stream >> temp) {
		keys.push_back(temp);
	}
	if (keys[0] == "push") {
		process_Push(keys);
	}
	else if (keys[0] == "pop") {
		process_Pop(keys);
	}
	else if (keys[0] == "add" || keys[0] == "sub" || keys[0] == "neg") {
		process_Arith(keys);
	}
	else if (keys[0] == "lt" || keys[0] == "gt" || keys[0] == "eq") {
		process_comp(keys);
	}


}

int main() {
	remove("PointerTest.asm");
	string filename = "PointerTest.vm";
	ifstream file;
	file.open(filename);
	string s;
	while (getline(file, s)) {
		//cout << "S: " << s << endl << "S.length(): " << s.length() << endl;
		if (s.length() != 0) {
			if (s[0] != '/' && s.length()!=1) {
				processLine(s);
			}
		}


	}
}