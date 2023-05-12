#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

/*
┌──(root㉿KaliC2)-[/home/kali/C2]
└─# g++ -Wl,-z,relro -o rc4 rc4.cpp
                                                                                                                                                                                                  
┌──(root㉿KaliC2)-[/home/kali/C2]
└─# ./rc4          
Usage: ./rc4 <key> <filename>

*/
using namespace std;

// Define the RC4 encryption algorithm.
// Takes a vector of unsigned characters (data) and a string (key).
// Returns a vector of unsigned characters (encrypted).
vector<unsigned char> rc4(vector<unsigned char> data, string key) {
    int keylen = key.length();
    vector<unsigned char> s(256);
    
    // Initialize s with values 0-255.
    for (int i = 0; i < 256; i++) {
        s[i] = i;
    }
    
    int j = 0;
    
    // Initialize s with key.
    for (int i = 0; i < 256; i++) {
        j = (j + s[i] + key[i % keylen]) % 256;
        swap(s[i], s[j]);
    }
    
    int i = 0;
    j = 0;
    
    vector<unsigned char> encrypted;
    
    // Generate the encrypted data using the RC4 algorithm.
    for (int n = 0; n < data.size(); n++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i], s[j]);
        encrypted.push_back(data[n] ^ s[(s[i] + s[j]) % 256]);
    }
    
    return encrypted;
}

int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments are provided.
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <key> <filename>" << endl;
        return 0;
    }
    
    // Get the key and filename from the command line arguments.
    string key = argv[1];
    string filename = argv[2];
    
    // Open the input file in binary mode and get the file size.
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return 0;
    }
    streampos size = file.tellg();
    
    // Create a vector of unsigned characters to hold the data from the input file.
    vector<unsigned char> data(size);
    
    // Seek to the beginning of the file and read the data into the vector.
    file.seekg(0, ios::beg);
    file.read((char*)&data[0], size);
    
    // Close the input file.
    file.close();
    
    // Encrypt the data using the RC4 algorithm.
    vector<unsigned char> encrypted = rc4(data, key);
    
    // Open the output file in binary mode and write the encrypted data to it.
    ofstream outfile(filename + ".enc", ios::binary);
    outfile.write((char*)&encrypted[0], encrypted.size());
    outfile.close();
    
    // Print a message indicating that the output file has been written.
    cout << "Written " << filename << ".enc" << endl;
    
    return 0;
}
