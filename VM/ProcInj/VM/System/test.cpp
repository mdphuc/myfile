
// C++ Program to split a string by a delimiter 
  
#include <iostream> 
#include <sstream> 
#include <vector> 
  
using namespace std; 
  
int main() 
{ 
    // Input string 
    string inputString = "geeks,for,geeks"; 

    stringstream ss(inputString); 
  
    // Tokenize the input string by comma delimiter 
    string token; 
    vector<string> tokens; 
  
    while (getline(ss, token, ',')) { 
        tokens.push_back(token); 
    } 
  
    // Output the string after splitting 
    cout << "String after splitting: " << endl; 
    for (const auto& part : tokens) { 
        cout << part << endl; 
    } 
  
    return 0; 
}