#include "ipv4ToAsn.h"
#include <stdlib.h>
#include <string.h>

Trie buildTrieFromMap(char* fileName) {
	// Need to call freeTrie() function when finish using the the trie
	Trie trie;
	FILE* filePointer;
	unsigned long int ip1, ip2, asn, i;
	int binary_ip_1[INT_BIT_SIZE] = { 0 };
	int binary_ip_2[INT_BIT_SIZE] = { 0 };
	char tmpStr[MAX_LINE_SIZE];

	// Open file and exit if file is not valid
	filePointer = fopen(fileName, "r");
	checkFileReading(filePointer);

	trie = createTrieRoot();
	fscanf(filePointer, "%ld %ld %d", &ip1, &ip2, &asn);
	//Saving the first IP in map for the total range of all IP adrresses
	decimalToBinaryConvert(ip1, trie.trieInfo.first_ip); 
	while (!feof(filePointer)) {
		decimalToBinaryConvert(ip1, binary_ip_1);
		decimalToBinaryConvert(ip2, binary_ip_2);
		insertIPToTrie(trie.root, binary_ip_1, asn, true);
		insertIPToTrie(trie.root, binary_ip_2, asn, false);
		fgets(tmpStr, MAX_LINE_SIZE, filePointer); //Skipping the rest of the line
		fscanf(filePointer, "%ld %ld %d", &ip1, &ip2, &asn); //scanning the next values in file
	}
	//Saving the last IP in map for the total range of all IP adrresses
	memcpy(trie.trieInfo.last_ip, binary_ip_2, sizeof(trie.trieInfo.last_ip));
	fclose(filePointer);
	return trie;
}

int asnLookup(char* ipAddress, Trie trie) {
	int binaryIPAddress[INT_BIT_SIZE] = { 0 };
	int asn = NO_ASN;
	TrieTraversal traverseMethod = byAddress;
	
	if (validate_ip(ipAddress)) {
		dottedDecimalToBinaryConvert(ipAddress, binaryIPAddress);
		if (checkIPInRange(binaryIPAddress, trie.trieInfo.first_ip, trie.trieInfo.last_ip)) {
			asn = asnLookupRec(trie.root, binaryIPAddress, 0, &traverseMethod);
		}
		else {
			printf("IP address is not exist!\n");
		}
	}
	else {
		printf("Invalid IP address!\n");
	}
	return asn;
}

Trie createTrieRoot() {
	Trie trie;

	trie.root = createNewTrieNode();
	return trie;
}

TrieNode* createNewTrieNode() {
	TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
	checkMemoryAllocation(newNode);
	newNode->smallerIP = newNode->biggerIP = NULL;
	//Only the leaf nodes in the trie will store the asn 
	newNode->asn = NO_ASN;
	return newNode;
}

void decimalToBinaryConvert(unsigned long int ip, int binary_ip[]) {
	int i = 31;

	while (ip > 0) {
		binary_ip[i] = ip % 2;
		ip = ip / 2;
		i--;
	}
}

void insertIPToTrie(TrieNode* root, int binary_ip[], int asn) {
	TrieNode* currNode = root;
	int i;

	for (i = 0; i < INT_BIT_SIZE; i++) {
		if (binary_ip[i] == 0) {
			if (currNode->smallerIP == NULL) {
				currNode->smallerIP = createNewTrieNode();
			}
			currNode = currNode->smallerIP;
		}
		else {
			if (currNode->biggerIP == NULL) {
				currNode->biggerIP = createNewTrieNode();
			}
			currNode = currNode->biggerIP;
		}
	}
	currNode->asn = asn;
}

int asnLookupRec(TrieNode* node, int ipAddress[], int level, TrieTraversal* traverseMethod) {
	if (node->smallerIP == NULL && node->biggerIP == NULL) { //End of trie
		return node->asn;
	}
	if (*traverseMethod = byAddress) {
		if (ipAddress[level] == 0) { //Continue left
			if (node->smallerIP != NULL) {
				//Continue search by ip address
				return asnLookupRec(node->smallerIP, ipAddress, level + 1, traverseMethod);
			}
			else {
				//Turn right and then keep left in order to reach the first ip in range
				*traverseMethod = left;
				return asnLookupRec(node->biggerIP, ipAddress, level + 1, traverseMethod);
			}
		}
		else { //Continue right
			if (node->biggerIP != NULL) {
				//Continue search by ip address
				return asnLookupRec(node->biggerIP, ipAddress, level + 1, traverseMethod);
			}
			else {
				//Turn left and then keep right in order to reach the last ip in range
				*traverseMethod = right;
				return asnLookupRec(node->smallerIP, ipAddress, level + 1, traverseMethod);
			}
		}
	}
	//If you need to keep left or if the right node is NULL
	if (node->smallerIP != NULL && (*traverseMethod == left|| node->biggerIP == NULL)) {
		return asnLookupRec(node->smallerIP, ipAddress, level + 1, traverseMethod);
	}
	//If you need to keep right or if the left node is NULL
	else if (node->biggerIP != NULL && (*traverseMethod == right || node->smallerIP == NULL)) {
		return asnLookupRec(node->biggerIP, ipAddress, level + 1, traverseMethod);
	}
}

void dottedDecimalToBinaryConvert(char* str, int binaryIpAddress[]) {
	int byteNum = 1, num, ipIndex;
	char* ptr;
	char ipAddress[MAX_IP_LENGTH];

	sscanf(str, "%s", ipAddress);
	ptr = strtok(ipAddress, ".");
	while (ptr) {
		num = atoi(ptr); //convert substring to number
		ipIndex = (byteNum * 8) - 1; //The least significant bit index
		//Inserting the byte to the suitable place in the binary number
		ByteToIpArr(binaryIpAddress, ipIndex, num);
		ptr = strtok(NULL, "."); //cut the next part of the string
		byteNum++;
	}
}

void ByteToIpArr(int ipAddress[], int ipIndex, int num) {
	while (num > 0) {
		ipAddress[ipIndex] = num % 2;
		num = num / 2;
		(ipIndex)--;
	}
}

void freeTrie(Trie tr) {
	freeTrieRec(tr.root);
}

void freeTrieRec(TrieNode* node) {
	if (node == NULL) { //Empty trie
		return;
	}
	freeTrieRec(node->smallerIP); //Free left sub trie
	freeTrieRec(node->biggerIP); //Free right sub trie
	free(node); //Free root
}

int validate_ip(char* str) { //check whether the IP is valid or not
	int i, num, dots = 0;
	char* ptr;
	char ipAddress[MAX_IP_LENGTH + 1];

	sscanf(str, "%s", ipAddress);
	if (ipAddress == NULL || strlen(ipAddress) > MAX_IP_LENGTH)
		return false;
	ptr = strtok(ipAddress, "."); //cut the string using dor delimiter
	if (ptr == NULL)
		return false;
	while (ptr) {
		if (!validateNumber(ptr)) //check whether the sub string is holding only number or not
			return false;
		num = atoi(ptr); //convert substring to number
		if (num >= 0 && num <= 255) {
			ptr = strtok(NULL, "."); //cut the next part of the string
			if (ptr != NULL)
				dots++; //increase the dot count
		}
		else
			return false;
	}
	if (dots != 3) //if the number of dots are not 3, return false
		return false;
	return true;
}

int validateNumber(char str[]) {
	bool res = true;;

	while (*str) {
		if (!isdigit(*str)) { //if the character is not a number, return false
			res = false;
		}
		str++; //pointer to next character
	}
	return res;
}

bool checkIPInRange(int ipAddress[], int first_ip[], int last_ip[]) {
	bool res;

	res = checkBound(ipAddress, first_ip, smallerThanFirstIP); //If is in bound
	if (res == false) {
		res = checkBound(ipAddress, last_ip, biggerThanLastIP);
	}
	return res;
}

bool checkBound(int ipAddress[], int edgeIP[], int (*compare)(int, int)) {
	int i;
	bool found = false;

	for (i = 0; i < INT_BIT_SIZE && found == false; i++) {
		if (ipAddress[i] != edgeIP[i]) { 
			found = true;
			if (compare(ipAddress[i], edgeIP[i]) > 0) {//Check if the given IP is smaller then the first IP or bigger then the last IP
				return false;
			}
		}
	}
	return true;
}

int smallerThanFirstIP(int givenIPBit, int firstIPBit) {
	return firstIPBit - givenIPBit;
}

int biggerThanLastIP(int givenIPBit, int lastIPBit) {
	return  givenIPBit - lastIPBit;
}

void checkFileReading(void* fp) {
	if (fp == NULL) {
		printf("Failed open the file!\n");
		exit(1);
	}
}

void checkMemoryAllocation(void* ptr) {
	if (ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}