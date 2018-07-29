#include <bits/stdc++.h>
#include <iostream>
using namespace std;

std::string encrypt (const std::string & plainText, const std::string & password)
{
    int len = plainText.length();
    char cipherText[len];
    strcpy(cipherText, plainText.c_str());
    // perform XOR operation of key
    // with every caracter in string
    for (int i = 0; i < len; i++)
    {
        cipherText[i] = cipherText[i] ^ password[0];
    }
return cipherText;
}

std::string decrypt (const std::string & plainText, const std::string & password)
{
    int len = plainText.length();
    char cipherText[len];
    strcpy(cipherText, plainText.c_str());
    // perform XOR operation of key
    // with every caracter in string
    for (int i = 0; i < len; i++)
    {
        cipherText[i] = cipherText[i] ^ password[0];
    }
    return cipherText;
}


int
main ()
{
    string encryptedMsg = encrypt("ciphers", "P");
    cout <<encryptedMsg << endl;
  cout << decrypt (encryptedMsg, "P") << endl;
  return 0;
}

