// Simple tool to print the results of parsing as json object for use in the javascript tests.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "reciter.h"
#include "sam.h"
#include "debug.h"

extern unsigned char phonemeIndexOutput[60]; //tab47296
extern unsigned char stressOutput[60]; //tab47365
extern unsigned char phonemeLengthOutput[60]; //tab47416

void PrintUsage()
{
	printf("usage: sam [options] Word1 Word2 ....\n");
	printf("options\n");
	printf("	-phonetic 		enters phonetic mode. (see below)\n");
	printf("	-debug			print additional debug messages\n");
	printf("\n");

	printf("     VOWELS                            VOICED CONSONANTS	\n");
	printf("IY           f(ee)t                    R        red		\n");
	printf("IH           p(i)n                     L        allow		\n");
	printf("EH           beg                       W        away		\n");
	printf("AE           Sam                       W        whale		\n");
	printf("AA           pot                       Y        you		\n");
	printf("AH           b(u)dget                  M        Sam		\n");
	printf("AO           t(al)k                    N        man		\n");
	printf("OH           cone                      NX       so(ng)		\n");
	printf("UH           book                      B        bad		\n");
	printf("UX           l(oo)t                    D        dog		\n");
	printf("ER           bird                      G        again		\n");
	printf("AX           gall(o)n                  J        judge		\n");
	printf("IX           dig(i)t                   Z        zoo		\n");
	printf("				       ZH       plea(s)ure	\n");
	printf("   DIPHTHONGS                          V        seven		\n");
	printf("EY           m(a)de                    DH       (th)en		\n");
	printf("AY           h(igh)						\n");
	printf("OY           boy						\n");
	printf("AW           h(ow)                     UNVOICED CONSONANTS	\n");
	printf("OW           slow                      S         Sam		\n");
	printf("UW           crew                      Sh        fish		\n");
	printf("                                       F         fish		\n");
	printf("                                       TH        thin		\n");
	printf(" SPECIAL PHONEMES                      P         poke		\n");
	printf("UL           sett(le) (=AXL)           T         talk		\n");
	printf("UM           astron(omy) (=AXM)        K         cake		\n");
	printf("UN           functi(on) (=AXN)         CH        speech		\n");
	printf("Q            kitt-en (glottal stop)    /H        a(h)ead	\n");
}

int debug = 0;

int main(int argc, char **argv)
{
	int i;
	int phonetic = 0;

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
				PrintUsage();
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
		PrintUsage();
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

	printf("},\n");

	return 0;
}
