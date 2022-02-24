#ifndef __ASNLOOKUP_H
#define __ASNLOOKUP_H

#include <stdio.h>
#include <stdbool.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#define FILE_NAME "ip2asn-v4-u32.tsv"
#define MAX_LINE_SIZE 256
#define INT_BIT_SIZE 32
#define NO_ASN -1
#define MAX_IP_LENGTH 16

// Enum to sepecify how to traverse the trie when searching the ASN
typedef enum trie_traversal{
	left, right, byAddress
} TrieTraversal;

typedef struct trie_node { 
	struct trie_node* smallerIP;
	struct trie_node* biggerIP; 
	int asn;
} TrieNode;

typedef struct trie_info {
	//Will be stored in binary
	int first_ip[INT_BIT_SIZE];
	int last_ip[INT_BIT_SIZE];
} TrieInfo;

typedef struct trie {
	TrieNode* root;
	TrieInfo trieInfo;
} Trie;


/*TODO:
// Public

// Build Trie data structure from a map of IP ranges and their matching ASN number
// This function assumes the first IP is the start of ip address range in the first line in file
// and the last IP is the end of ip address range in the last line in file
// Need to call freeTrie() function when finish using the the trie
Trie buildTrieFromMap(char* fileName);

// This function will get an IP address and a trie
// This function returns the ASN it is allocated to. Return -1 if the ASN wasn't found or IP is invalid
int asnLookup(char* ipAddress, Trie trie);

//*Private*

//*Creating the tree helper functions*/

// Create new empty Trie data structure
Trie createTrieRoot(); 
// Creating new trie node
TrieNode* createNewTrieNode(); 
// Converting a decimal number to binary int array
void decimalToBinaryConvert(unsigned long int ip, int binary_ip[]); 
// Inserting an IP address and it's matching ASN to a trie
void insertIPToTrie(TrieNode* root, int binary_ip[], int asn);

//*Lookup ASN helper functions*//

// Searching for the matching ASN of the given IP
int asnLookupRec(TrieNode* node, int ipAddress[], int level, TrieTraversal* traverseMethod);
// Converting a string representing a dotted decimal IP address to binary array
void dottedDecimalToBinaryConvert(char* ipAddress, int binaryIpAddress[]);
// Inserting the byte to the suitable place in the binary number
void ByteToIpArr(int ipAddress[], int ipIndex, int num);
// Free all the nodes in the Trie
void freeTrie(Trie tr); 
// Free all the nodes in the Trie recursively
void freeTrieRec(TrieNode* node); 


//*check edge cases*//

//Validating if an IP is valid
int validate_ip(char* str);
//Validating if the string is a number
int validateNumber(char str[]);
//Checks if the given IP is in the range of the IPs map
bool checkIPInRange(int ipAddress[], int first_ip[], int last_ip[]); 
//Checking if the given IP is below or above the addresses bounderis
bool checkBound(int ipAddress[], int edgeIP[], int (*compare)(int, int)); 
//Return true if the given IP bit is 0 and the first IP is 1, indicating the given IP is smaller than the first IP
int smallerThanFirstIP(int givenIPBit, int firstIPBit); 
// Return true if the given IP bit is 1 and the first IP is 0, indicating the given IP is bigger than the last IP
int biggerThanLastIP(int givenIPBit, int lastIPBit);
 

//*Check file opening and memory allocation*//

// Check iF file path is not valid or file can not be opened- exit run 
void checkFileReading(void* fp);
// Check if the memory allocation was successful
void checkMemoryAllocation(void* ptr); 

#endif //__ASNLOOKUP_H