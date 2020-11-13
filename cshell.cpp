//Alex King
//
//
//reference: https://en.cppreference.com/w/cpp/utility/program/getenv
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unistd.h>
#include <sys/wait.h> 
#define DN "0"
using namespace std; 
int pid_num;
vector<string> procs, dprocs;
vector<int> pids;
map<string,string> ShowTok;
vector<char*> variables;
void handler(int sig)  {
	pid_t p;
	if ((p = waitpid(-1, &sig, WNOHANG)) != -1) {
		pid_num = p;
		for (unsigned int i = 0; i < pids.size(); i++) {
			if (p == pids[i]) {
				dprocs.push_back(procs[i]);
			}
		}
	}
}
int dir(vector <string> tokens) {
	char cwd[256];
	string holder;
	char* temp = const_cast<char*>(tokens[1].c_str());
	if (tokens.size() == 2) {
    		cout << "No Directory given \n";
  		return -1;
  	}else if(tokens.size() > 3){
	 	cout << "Junk after the cd  \n" ;  	   
		return -1;
	}else if(tokens[1][0] == '.'){		  	
		chdir(getcwd(cwd, sizeof(cwd)));
	}
	if (chdir(temp) != 0) {
      		perror(temp);
    }
  
  return -1;
}

int newprompt(vector <string> tokens, string & prompt) {
		if( tokens[1] != "0"){
			prompt = tokens[1];
		}else {
			prompt = "novsh > ";
		}
		return -1;
}
int listprocs() {
	cout << "Background Processes: " << endl;
	for (unsigned int i = 0; i < procs.size(); i++) {
		cout << "         " << procs[i] << endl;
	}

	return -1;
}
int bye(vector <string> tokens) {
	if (tokens.size() > 3) {
		cout << "Too many parameters to exit." << endl;
		return -1;
	}
	else {
		int retval;
		if (tokens[1] == DN) {
			return 0;
		}
	}
}
int showTokens(vector <string> tokens){
	if( ShowTok["on"] == "1"){
		for (unsigned int i = 0; i < tokens.size() - 1; i++) {
			cout << "Token :  " << tokens[i] << endl;
		}
		cout << "-----------" << endl;
	}
	return -1;
}
int comment(vector <string> tokens){
	return 0;
}
void assignto(vector <string> tokens){
	string unParse;
	for(unsigned int i = 2; i < tokens.size() - 1; i++){
		unParse += tokens[i] + " ";
	}
	cout << unParse;
	setenv(const_cast<char*>(tokens[1].c_str()),unParse.c_str(),1);
	
	return;
}

vector <string> tokenScanner(string input) {
	int count = -1;
	string token = "";
	vector <string> tokens;
	char temp, holder;
	char *vari;
	bool running = true;		
	while (running) {
		count++;
		temp = input[count];
		
		if (temp == ' ') {
			while (temp == ' ') {
				count++;
				temp = input[count];
			}
		}
		if (temp == '"') {
			count++;
			temp = input[count];
			while (temp != '"' && temp != '\0') {
				token = token + temp;
				count++;
				temp = input[count];
			}
			tokens.push_back(token);
			token = "";
		}
		else if(temp == '$'){
			count++;
			temp = input[count];
			while(temp != ' ' && temp != '\0'){
				token = token + temp;
				count++;
				temp = input[count];
			}
			if(token != ""){
				vari = const_cast<char*>(token.c_str());
				if( getenv(vari) == NULL ){
					cout << "Variable " << token << " not found" << endl;
					token = "";
				}else{token = getenv(vari);}
			}
			tokens.push_back(token);
			token = "";
		}
		else if (temp != ' ' && temp != '\0') {
			while ((temp != ' ') && (temp != '\0')) {
				if( temp == '$'){ break;}
				token = token + temp;	
				count++;
				temp = input[count];
			}
			tokens.push_back(token);
			token = "";
			temp = input[count];
		}
		if (temp == '\0') {
			running = false;
		}

	}
	tokens.push_back(DN); 
	return tokens; 
}

void assignments(vector <string> tokens){
	if(tokens[0] == "ShowTokens"){
		if(tokens[2] == "0"){
			ShowTok["on"] = "0";
		}else if(tokens[2] == "1"){
			ShowTok["on"] = "1";
		}else{ cout << "Unregonized value" << endl;}		
	}
	setenv(const_cast<char*>(tokens[0].c_str()),const_cast<char*>(tokens[2].c_str()),1);	
	
	return;

}
string read(string input){
	string tmp, output;
	for (unsigned int i = 0; i<input.size();i++) {
		if (input[i] == '^') {
			int j = i + 1;
			while (input[j] != ' ' && input[j] != '"' && input[j] != '\0') {
				tmp = tmp + input[j];
				j++;
			}
			if (input[j] == ' ') tmp = tmp + ' ';
			output = output + tmp;
			i = j;
		}
		else {
			output = output + input[i];
		}
	}
	return output;
}

int run(vector<string> tokens) {
	char* parse[256];  
	pid_t pid;
	string fix;
	vector <string> temp;
	int ret, status;
	string proc = tokens[1];
	int bg = 0;
	if(tokens[tokens.size()-2] == "<bg>"){
		bg = 1;
		tokens.erase(tokens.begin() + tokens.size() - 2);
	}
	for (unsigned int i = 0; i < tokens.size() - 1; i++) {
		parse[i] = const_cast<char*>(tokens[i + 1].c_str());
	}
	


	parse[tokens.size() - 2] = 0;
	pid = fork();
	if(pid == 0){
   		ret = execvp(parse[0],parse);
   	if (ret == -1) {
		perror("exec");
		exit(0);
	}
   	return -1;
	} 
	else if (pid > 0) {
		if (bg == 0 ) {
			do {
	 		 waitpid(pid, &status, WUNTRACED);
	    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}	
		if (bg == 1) {
			do {
				waitpid(pid, &status, WNOHANG);
	    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			procs.push_back(proc);
			pids.push_back(pid);
	}
	return -1;
	}
	else {
	return -1;
	}

}


int functions(vector <string> tokens, string & prompt) {
	int exval, bg;
	if (tokens[0] == "newprompt") {
		newprompt(tokens, prompt);
	}
	else if (tokens[0] == "dir") {
		dir(tokens);
	}
	else if(tokens[0] == "!") {
		comment(tokens);
	}
	else if (tokens[0] == "listprocs") {
		listprocs();
	}
	else if (tokens[0] == "bye") {
		exval = bye(tokens);
		return exval;
	}
	else if (tokens[0] == "run") {
		run(tokens);
	}
	else if (tokens[0] == "0") {
	} 
	else if (tokens[1] == "="){
		assignments(tokens); 
	}
	else if (tokens[0] == "assignto"){
		assignto(tokens);
	}
	else {
		cout << "unrecongized command: " << tokens[0] << endl;
	}
	signal(SIGCHLD, handler);
	while (dprocs.size() > 0) {
		cout << "Reaping Job with pid " << pid_num << endl;
		dprocs.pop_back();
	}
	cout << prompt;
	return -1;
}

//command loop function
void cmd_loop() {
	setenv("PATH","/bin:/usr/bin",1);
	string input, prompt = "novsh > "; 
	vector <string> tokens;
	int status = -1;
	cout << prompt;
	//Ask user for command and token
	while (status < 0 && getline(cin, input)) {
		input = read(input); 
		tokens = tokenScanner(input);
		showTokens(tokens);
		status = functions(tokens, prompt);
		tokens.clear();
	}
}


int main(){
	//loops through commands until exit state
	cmd_loop();
	return 0;
}
