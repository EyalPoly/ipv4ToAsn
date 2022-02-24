# ipv4ToAsn
ASN lookup by IPv4

IPv4 addresses on the internet are 32 bits. These addresses are allocated by the Internet Assigned Numbers Authority (IANA) to organizations which are called “Autonomous Systems”. Each such organization has a unique Autonomous System Number - ASN.

In this link you can find an updated map of the IP addresses to ASN allocation:
https://iptoasn.com/data/ip2asn-v4-u32.tsv.gz
Download the file to the relevant folder and unzip it.

Call buildTrieFromMap() function with the file name as input in order to build Trie data structure.
This function assumes the first IP is the start of ip address range in the first line in file and the last IP is the end of ip address range in the last line in file.
After it call asnLookup() function. This function will get as an input an IP address and a trie data structue. The function returns the ASN it is allocated to. Return -1 if the ASN wasn't found or IP is invalid
Need to call freeTrie() function when finish using the the trie.
