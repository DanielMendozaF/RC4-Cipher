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

int main(){   

    // User input for same key used in encryption process
    string key_string;
    cout << "Enter the same 5 - 32 byte key used in the encryption of the ciphertext file: " << endl;
    getline(cin, key_string);

    // Condition to validate key length of 5 - 32 bytes long 
    while ((key_string.length() < 5) || (key_string.length() > 32)){
        cout << "Error! The key is incorrect. It must be 5 - 32 bytes in length. Try again: " << endl;
        getline(cin, key_string);
    }

    // User input for ciphertext file to decrypt 
    string cipherFile;
    cout << "Enter ciphertext file to decrypt: " << endl;
    getline(cin, cipherFile);
    ifstream inputFile(cipherFile, ios::binary);

    if (inputFile.is_open()){

        // Initialize state array and integer variables
        unsigned char state_array[256];
        int i = 0;
        int j = 0;

        // Call to key scheduling algorithm function
        KSA(state_array, key_string);

        // Call to function to discard initial 3072 bytes 
        discard_bytes(state_array, i, j);
\
        // User input for decrypted text output file
        string decryptedFile;
        cout << "Enter file for decryption output: " << endl;
        getline(cin, decryptedFile);
        ofstream outputFile(decryptedFile,ios::binary);

        // Condition to ensure output file is open
        if (outputFile.is_open()){   
            
            string hex_byte;
            
            // Condition to read hexadecimal bytes from input file 
            while (inputFile >> setw(2) >> hex_byte){

                // Line to convert hexadecimal byte to integer byte using stoi function from <string> 
                int int_byte = stoi(hex_byte, nullptr, 16);

                // Bitwise XOR operation between plaintext file characters and keystream from PRGA function
                char decrypt_character = int_byte ^ PRGA(state_array, i ,j);
                outputFile << decrypt_character;
            }

            // User feedback for decrpytion results
            cout << "The decryption was a success!" << endl;
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