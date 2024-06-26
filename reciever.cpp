#include <iostream>
#include <fstream>

using namespace std;

class Node {
public:
    char data;
    Node* left;
    Node* right;

    Node(char data) : data(data), left(nullptr), right(nullptr) {}
};

Node* buildHuffmanTree(ifstream& codesFile) {
    char character;
    string code;

    Node* root = new Node('\0');
    Node* current;

    while (codesFile >> character >> code) {
        current = root;

        for (char bit : code) {
            if (bit == '0') {
                if (current->left == nullptr) {
                    current->left = new Node('\0');
                }
                current = current->left;
            }
            else {
                if (current->right == nullptr) {
                    current->right = new Node('\0');
                }
                current = current->right;
            }
        }

        current->data = character;
    }

    return root;
}

void decodeText(Node* root, const string& encodedText, ofstream& outputFile) {
    Node* current = root;

    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        }
        else {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            // Found a leaf node, output the character
            outputFile << current->data;
            current = root;  // Reset to the root for the next character
        }
    }
}

int main() {
    // Step 1: Read the encoded text file and codes file
    ifstream encodedFile("output.txt");
    ifstream codesFile("codes.txt");

    if (!encodedFile.is_open() || !codesFile.is_open()) {
        cout << "Unable to open files.";
        return 1;
    }

    string encodedText((istreambuf_iterator<char>(encodedFile)), istreambuf_iterator<char>());

    // Step 2: Build Huffman tree using codes file
    Node* huffmanRoot = buildHuffmanTree(codesFile);

    // Step 3: Decode the encoded text
    ofstream outputFile("receiver.txt");
    if (outputFile.is_open()) {
        decodeText(huffmanRoot, encodedText, outputFile);
        outputFile.close();
    }
    else {
        cout << "Unable to open receiver.txt";
    }

    // Cleanup
    delete huffmanRoot;

    cout << "Decoding completed successfully." << endl;

    return 0;
}
