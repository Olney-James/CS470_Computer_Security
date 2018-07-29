/******************************************************************************

*******************************************************************************/
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
using namespace std;

std::string encrypt (const std::string & cipherText, const std::string & password)
{
   char message[100], index;
  // we need to convert the passoword to an int
  std::stringstream pass (password);	
  int shiftNum;			
  pass >> shiftNum;
  if(shiftNum == 0){
      shiftNum = 7;
  }
  strcpy (message, cipherText.c_str ());
  for(int i = 0; message[i] != '\0'; ++i){
        index = message[i];
        
        if(index >= 'a' && index <= 'z'){
            index = index + shiftNum;
            
            if(index > 'z'){
                index = index - 'z' + 'a' - 1;
            }
            
            message[i] = index;
        }
        else if(index >= 'A' && index <= 'Z'){
            index = index + shiftNum;
            
            if(index > 'Z'){
                index = index - 'Z' + 'A' - 1;
            }
            
            message[i] = index;
        }
    }
    return message;
}

std::string decrypt (const std::string & cipherText, const std::string & password)
{
  char message[100], index;
  // we need to convert the passoword to an int
  std::stringstream pass (password);	
  int shiftNum;			
  pass >> shiftNum;		
  if(shiftNum == 0){
      shiftNum = 7;
  }
  strcpy (message, cipherText.c_str ());
  for (int i = 0; message[i] != '\0'; ++i)
    {
      index = message[i];

      if (index >= 'a' && index <= 'z')
	{
	  index = index - shiftNum;

	  if (index < 'a')
	    {
	      index = index + 'z' - 'a' + 1;
	    }

	  message[i] = index;
	}
      else if (index >= 'A' && index <= 'Z')
	{
	  index = index - shiftNum;

	  if (index < 'A')
	    {
	      index = index + 'Z' - 'A' + 1;
	    }

	  message[i] = index;
	}
    }
  return message;
}


int
main ()
{
    string encryptedMsg = encrypt("CIPHERS", "test");
    cout <<encryptedMsg << endl;
  cout << decrypt (encryptedMsg, "test") << endl;
  return 0;
}
