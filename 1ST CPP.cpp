#include <iostream>
#include <fstream>

using namespace std;

class Node {
public:
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

class LinkedList {
public:
    Node* node;
    LinkedList* next;

    LinkedList(Node* node) : node(node), next(nullptr) {}
};

Node* buildHuffmanTree(LinkedList*& list) {
    while (list->next != nullptr) {
        Node* left = list->node;
        list = list->next;

        Node* right = list->node;
        list = list->next;

        Node* internalNode = new Node('\0', left->freq + right->freq);
        internalNode->left = left;
        internalNode->right = right;

        LinkedList* newNode = new LinkedList(internalNode);
        newNode->next = list;
        list = newNode;
    }

    return list->node;
}

void buildHuffmanCodes(Node* root, string code, string codes[], string chars[]) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '\0') {
        codes[root->data] = code;
        chars[root->data] = root->data;
    }

    buildHuffmanCodes(root->left, code + "0", codes, chars);
    buildHuffmanCodes(root->right, code + "1", codes, chars);
}

string encodeText(const string& text, string codes[]) {
    string encodedText;
    for (char c : text) {
        encodedText += codes[c];
    }
    cout << "Encoded Text is  :  " << encodedText << endl;
    return encodedText;
}

void writeEncodedFile(const string& encodedText) {
    ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        outputFile << encodedText;
        outputFile.close();
    }
    else {
        cout << "Unable to open output.txt";
    }
}

void writeCodesFile(string codes[], string chars[]) {
    ofstream codesFile("codes.txt");
    if (codesFile.is_open()) {
        for (int i = 0; i < 128; ++i) {
            if (!codes[i].empty()) {
                codesFile << chars[i] << ": " << codes[i] << "\n";
            }
        }
        codesFile.close();
    }
    else {
        cout << "Unable to open codes.txt";
    }
}

int main() {
    // Step 1: Read the input text file or create it if it doesn't exist
    ifstream inputFile("sender.txt");
    if (!inputFile.is_open()) {
        cout << "File not found. Creating a new file.\n";
        ofstream newFile("sender.txt");
        newFile << "Default text for testing.";
        newFile.close();

        // Reopen the file for reading
        inputFile.open("sender.txt");
    }

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    // Step 2: Determine the frequency of each character
    int frequencies[128] = { 0 };  // Assuming ASCII characters
    for (char c : text) {
        frequencies[c]++;
    }

    // Step 3-5: Build Huffman tree
    LinkedList* list = nullptr;
    LinkedList* tail = nullptr;

    for (int i = 0; i < 128; ++i) {
        if (frequencies[i] > 0) {
            Node* newNode = new Node(static_cast<char>(i), frequencies[i]);

            if (list == nullptr) {
                list = new LinkedList(newNode);
                tail = list;
            }
            else {
                tail->next = new LinkedList(newNode);
                tail = tail->next;
            }
        }
    }

    Node* huffmanRoot = buildHuffmanTree(list);

    // Step 6: Build Huffman codes
    string codes[128];
    string chars[128];
    buildHuffmanCodes(huffmanRoot, "", codes, chars);

    // Step 7: Encode the text
    string encodedText = encodeText(text, codes);

    // Step 8: Write encoded text and codes to files
    writeEncodedFile(encodedText);
    writeCodesFile(codes, chars);

    // Cleanup
    delete huffmanRoot;

    cout << "Encoding completed successfully." << endl;

    return 0;
}
