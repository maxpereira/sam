// Simple tool to print the results of rendering as json object for use in the javascript tests.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "sam.h"
#include "debug.h"
#include "reciter.h"
#include "render.h"

extern unsigned char phonemeIndexOutput[60]; //tab47296
extern unsigned char stressOutput[60]; //tab47365
extern unsigned char phonemeLengthOutput[60]; //tab47416
extern unsigned char pitches[256]; // tab43008
extern unsigned char sampledConsonantFlag[256];
extern unsigned char frequency1[256];
extern unsigned char frequency2[256];
extern unsigned char frequency3[256];
extern unsigned char amplitude1[256];
extern unsigned char amplitude2[256];
extern unsigned char amplitude3[256];
extern unsigned char freq1data[];
extern unsigned char freq2data[];
extern unsigned char freq3data[];

int debug = 0;

int main(int argc, char **argv)
{
	int i;
	int phonetic = 0;

	char input[256];

	for(i=0; i<256; i++) input[i] = 0;

	if (argc <= 1)
	{
		printf("Error: No input!");
		return 1;
	}

	i = 1;
	while(i < argc)
	{
		if (argv[i][0] != '-')
		{
			strncat(input, argv[i], 256);
			strncat(input, " ", 256);
		} else
		{
			if (strcmp(&argv[i][1], "phonetic")==0)
			{
				phonetic = 1;
			} else
			if (strcmp(&argv[i][1], "debug")==0)
			{
				debug = 1;
			} else
			{
				printf("Error: unknown argument! %s\n", argv[i]);
				return 1;
			}
		}

		i++;
	} //while

	for(i=0; input[i] != 0; i++)
		input[i] = toupper((int)input[i]);

	if (debug)
	{
		if (phonetic) printf("phonetic input: \"%s\"\n", input);
		else printf("text input: \"%s\"\n", input);
	}

	if (!phonetic)
	{
		strncat(input, "[", 256);
		if (!TextToPhonemes(input)) return 1;
		if (debug)
			printf("phonetic input: \"%s\"\n", input);
	} else strncat(input, "\x9b", 256);

	SetInput(input);
	if (!SAMMain())
	{
		printf("rendering failed!\n");
		return 1;
	}

	int last = 255;
	for(i=0; i<256; i++) {
		if (input[i] & 128) {
			input[i] = 0;
			last = i;
			break;
		}
	}
	printf("{\n");
	printf("    \"input\": \"%s\",\n", input);

	last = 255;
	for(i=0; i<256; i++) {
		if (phonemeIndexOutput[i] & 128) {
			last = i;
			break;
		}
	}
	printArray("output", phonemeIndexOutput, last);
	printArray("length", phonemeLengthOutput, last);
	printArray("stress", stressOutput, last);

	last = 255;
	for(i=0; i<256; i++) {
		if (!pitches[i]
			&& !sampledConsonantFlag[i]
			&& !frequency1[i]
			&& !frequency2[i]
			&& !frequency3[i]
			&& !amplitude1[i]
			&& !amplitude2[i]
			&& !amplitude3[i]) {
			last = i;
			break;
		}
	}


	printArray("pitches", pitches, last);
	printArray("sampledConsonantFlag", sampledConsonantFlag, last);
	printArray("frequency1", frequency1, last);
	printArray("frequency2", frequency2, last);
	printArray("frequency3", frequency3, last);
	printArray("amplitude1", amplitude1, last);
	printArray("amplitude2", amplitude2, last);
	printArray("amplitude3", amplitude3, last);
	printArray("freq1data", freq1data, 80);
	printArray("freq2data", freq2data, 80);
	printArray("freq3data", freq3data, 80);

	printf("},\n");

	return 0;
}
