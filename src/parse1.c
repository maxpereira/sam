// Simple tool to print the results of parser1() as json object for use in the javascript tests.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "sam.h"
#include "debug.h"

void PrintUsage()
{
	printf("usage: parse1 PHONEMES...\n");
}
int debug = 0;

extern unsigned char stress[256]; //numbers from 0 to 8
extern unsigned char phonemeindex[256];
extern char input[256];

unsigned char position;

int main(int argc, char **argv)
{
	int i;
	char input[256];
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

	for(i=0; input[i] != 0; i++)
		input[i] = toupper((int)input[i]);

	strncat(input, "\x9b", 256);

	SetInput(input);

	if (!Parser1()) return 1;

	for(i=0; i<256; i++) {
		if (input[i] & 128) input[i] = 0;
	}
	printf("{\n");
	printf("    \"input\": \"%s\",\n", input);
//	printf("    \"output\": \"%s\",\n", phonemeindex);
	printf("    \"output\": [\n");
	printf("        ");
	for(i=0; i<position; i++) {
		printf("%d", phonemeindex[i]);
		if (i<255) {
			printf(", ");
      if (!((i+1) % 16)) {
        printf("\n        ");
      }
		}
	}
	printf("\n    ],\n");
	printf("    \"stress\": [\n");
	printf("        ");
	for(i=0; i<position; i++) {
		printf("%d", stress[i]);
		if (i<255) {
			printf(", ");
      if (!((i+1) % 16)) {
        printf("\n        ");
      }
		}
	}

	printf("\n    ]\n");
	printf("},\n");

	return 0;
}
