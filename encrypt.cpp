#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// Function for key scheduling algorithm
void KSA(unsigned char S[], const string& user_key){
    for (int i = 0; i <256; i++){
        S[i] = i; 
    }
    int j = 0;
    for (int i = 0; i < 256; i++){
        j = (j + S[i] + user_key[i % user_key.size()])% 256;
        swap(S[i], S[j]);
    }
}

// Function for pseudo random generating algorithm
unsigned char PRGA(unsigned char S[], int& i, int& j){

    i = (i+1) % 256;
    j = (j+S[i]) % 256;
    swap(S[i], S[j]);
    return S[(S[i] + S[j]) % 256];
}

// Function to discard the first 3072 bytes to strengthen the encryption
void discard_bytes(unsigned char S[], int&i, int& j){
    for (int k = 0; k < 3072; k++){
        PRGA(S, i, j);
    }
}

// Main function
int main(){   

    // User input for key as a string
    string key_string;
    cout << "Enter a 5 - 32 byte long key: " << endl;
    getline(cin, key_string);

    // Condition to validate key length of 5 - 32 bytes long 
    while ((key_string.length() < 5) || (key_string.length() > 32)){
        cout << "Error! Key must be 5 - 32 bytes in length. Try again: " << endl;
        getline(cin, key_string);
    }

    // User input for plaintext input file to encypt
    string plainFile;
    cout << "Enter plaintext file to encrypt: " << endl;
    getline(cin, plainFile);
    ifstream inputFile(plainFile, ios::binary);

    // Condition to ensure input file is open
    if (inputFile.is_open()){

        // Initialize state array and integer variables
        unsigned char state_array[256];
        int i = 0;
        int j = 0;

        // Call to key scheduling algorithm function
        KSA(state_array, key_string);

        // Call to function that discards initial 3072 bytes 
        discard_bytes(state_array, i, j);

        // User input for cipher text output file
        string cipherFile;
        cout << "Enter ciphertext output file: " << endl;
        getline(cin, cipherFile);
        ofstream outputFile(cipherFile,ios::binary);

        // Condition to ensure output file is open
        if (outputFile.is_open()){   

            // Variable for input plaintext file character
            char file_character;

            // While loop to read all the characters of the input plaintext file
            while (inputFile.get(file_character)){

                // Bitwise XOR operation between plaintext file characters and keystream from PRGA function
                unsigned char encrypt_character = file_character ^ PRGA(state_array, i, j);

                // Convert encrypted characters to hexadecimal using the hex output manipulator
                outputFile << setw(2) << setfill('0') << hex << (int)encrypt_character;
            }

            // Give user feedback on ecryption results
            cout << "The encryption was a success!" << endl;
        }
        // Condition if there is an error opening output file
        else{
            cout << "Error opening output file" << endl;
        }
        inputFile.close();
        outputFile.close();
    }
    // Condition if there is an error opening input file
    else{
        cout << "Error opening input file" << endl;
    }
    return 0;
}
