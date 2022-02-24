#include "ipv4ToAsn.h"

void main() {
	//Testing
	int res;
	Trie trie;

	trie = buildTrieFromMap(FILE_NAME);
	res = asnLookup("1.1", trie); //Too short
	res = asnLookup("1.2.3.4.5", trie); //Too long
	res = asnLookup("300.-3.100.2", trie); //Invalid numbers
	res = asnLookup("1.2.3.400", trie); //Invalid numbers
	res = asnLookup("0.0.0.0", trie); //Below range
	res = asnLookup("255.255.255.255", trie); //Above range
	res = asnLookup("2.16.104.0", trie); //The first IP in a range. Should be 16625
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("1.253.95.255", trie); //The last IP in a range. Should be 55618
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("1.253.85.16", trie); //Valid IP in between range. Should be 55618
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("165.140.165.5", trie); //Valid IP in between range. Should be 0
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("204.9.247.254", trie); //Valid IP in between range, very big number. Should be 33132
	printf("The IP address is in ASN: %d\n", res); 
	res = asnLookup("1.0.64.20", trie); //Valid IP in between range. Should be 18144
	printf("The IP address is in ASN: %d\n", res); 
	res = asnLookup("2.56.166.240", trie); //Valid IP in between range. Should be 3280
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("1.0.0.0", trie); //First IP in map. Should be 13335
	printf("The IP address is in ASN: %d\n", res);
	res = asnLookup("223.255.254.255", trie); //Last IP in map. Should be 55415
	printf("The IP address is in ASN: %d\n", res);
	freeTrie(trie);
}