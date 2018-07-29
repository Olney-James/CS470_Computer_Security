/***********************************************************
*	Program:
*		Week 04, Lab 04
*		Brother Wilson, CS470
*	Author: 
*		James Olney
*	Created: 
*		02/02/2018
*	Revisions: 
*		N/A
*	Summary: 
*		prompts the user for input. runs the input through a sanitization filter to
*		remove all constructs not on the "whiteList". Finally, it shall display
*		the sanitized result
*******************/
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

string sanitize(string whiteList[], string input);
string getInput();
string whiteList[] = { "a","abbr","acronym","b","blockquote","cite","code","del","em","i","q","strike","strong" };
string input = "";

int main(){   
    while (input != "quit"){
      input = getInput();
      string sanitizedInput = sanitize(whiteList, input);
      cout << "\t" + sanitizedInput << endl;
   }
   return 0;
}

string sanitize(string whiteList[], string input){
   vector<string> nestedwhiteList;
   for (int i = 0; i < input.length(); ++i){
      if (input[i] == '<'){
         string tag;
         bool containsSlash = false;
         for (int j = i + 1; j < input.length(); ++j){
            if ((input[j] == '>') || (input[j] ==' ')){
               if (containsSlash == false){
                  nestedwhiteList.push_back(tag); 
               }
               if (std::find(whiteList, whiteList + 13, tag) != (whiteList + 13)){
                  if ((containsSlash == true) && (nestedwhiteList.back() != tag)){
                     return input.substr(0, i) + "&lt;" + (containsSlash == true ? "/" : "") + tag + "&gt;" + input.substr(j+1) + "</" + tag + ">";
                  }
               }
               else{
                  if (i == 0){
                     input = "&lt;" + tag + "&gt;" + input.substr(j+1);
                  }
                  else if ((j-1) == input.length()){
                     input = input.substr(0, i) + "&lt;" + (containsSlash == true ? "/" : "") + tag + "&gt;";
                  }
                  else{
                     input = input.substr(0, i) + "&lt;" + (containsSlash == true ? "/" : "") + tag + "&gt;" + input.substr(j+1);
                  }
				  containsSlash = false;
                  i += (8 + tag.length()); //adjust i for new length
               }

               if (containsSlash == true){
                  nestedwhiteList.pop_back();
               }
               break; //look for next tag
            }

            if (input[j] == '/'){
               containsSlash = true;
            }
            if (input[j] != '/'){
               tag += input[j];
            }
         }
      }
   }
   return input;
}

string getInput(){
	string input;
	cout << "> ";
    getline (cin, input);
	return input;
	}