// Simple tool to print the results of reciter as json object for use in the javascript tests.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "reciter.h"
#include "debug.h"

extern unsigned char GetRuleByte(unsigned short mem62, unsigned char Y);
extern void PrintRule(int offset);

void PrintUsage()
{
	printf("usage: recite Word1 Word2 ....\n");
}

int debug = 0;

int main(int argc, char **argv)
{
	int i;
	char input[256];
	char output[256];
	for(i=0; i<256; i++) input[i] = 0;
	if (argc <= 1)
	{
		PrintUsage();
		return 1;
	}

	i = 1;
	while(i < argc)
	{
		strncat(input, argv[i], 256);
		strncat(input, " ", 256);
		i++;
	} //while

	for(i=0; input[i] != 0; i++) {
		input[i] = toupper((int)input[i]);
		output[i] = input[i];
	}

	strncat(output, "[", 256);
	if (!TextToPhonemes(output)) return 1;
	for(i=0; i<256; i++) {
		if (output[i] & 128) output[i] = 0;
	}
	printf("{\n");
	printf("  input: \"%s\",\n", input);
	printf("  output: \"%s\"\n", output);
	printf("},\n");

	return 0;
}
