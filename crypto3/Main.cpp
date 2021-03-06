// Copyright (c) 2015 Mateusz Pyzik, all rights reserved.
#include "Keystore.hpp"
#include "Echo.hpp"
#include <string.h>
#include <openssl/evp.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		fputs("wrong number of arguments\n", stderr);
		return 1;
	}
	string action = argv[1];
	int enc = 1;
	if(action == "gen")
	{
		if(argc != 5)
		{
			fputs("wrong number of arguments\n", stderr);
			return 1;
		}
		Keystore keystore(argv[3], argv[2]);
		const char *keyIdentifier = argv[4];
		string password;
		setEcho(false);
		getline(cin, password);
		setEcho(true);
		keystore.createKey(keyIdentifier, password);
	}
	else if(action == "enc" || (enc = 0, action == "dec"))
	{
		if(argc != 8)
		{
			fputs("wrong number of arguments\n", stderr);
			return 1;
		}
		Keystore keystore(argv[4], argv[2]);
		const char *keyIdentifier = argv[5];
		const char *inputFile = argv[6], *outputFile = argv[7];
		string password;
		setEcho(false);
		getline(cin, password);
		setEcho(true);
		keystore.setMode(argv[3]);
		keystore.loadKey(keyIdentifier, password);
		FILE *ifp = stdin, *ofp = stdout;
		if(strcmp(inputFile, "-") != 0 && (ifp = fopen(inputFile, "rb")) == NULL)
		{
			fputs("input file could not be opened\n", stderr);
			exit(1);
		}
		if(strcmp(outputFile, "-") != 0 && (ofp = fopen(outputFile, "wb")) == NULL)
		{
			fputs("output file could not be opened\n", stderr);
			exit(1);
		}
		keystore.cipher(ifp, ofp, enc);
		if(ifp != stdin)
			fclose(ifp);
		if(ofp != stdout)
			fclose(ofp);
	}
	else
	{
		fputs("unsupported action\n", stderr);
		exit(1);
	}
	return 0;
}