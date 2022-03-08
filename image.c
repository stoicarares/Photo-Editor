// Stoica Rares-Tiberiu 311CA

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100

void scan_image(image_t *im, FILE *in)
{
	memset(im, 0, sizeof(image_t));

	// acest for merge pana citesc toate cele 3 linii ce contin parametrii
	// imaginii
	for (int i = 0; i < 3; i++) {
		char c;

		// citesc primul caracter de pe fiecare linie
		fscanf(in, "%c", &c);
		// verific daca este un inceput de comentariu, recunoscand caracterul #
		// daca nu este, se muta cursorul o pozitie inapoi in fisier pentru
		// a putea citi parametrii corect
		if (c != '#') {
			fseek(in, -1, SEEK_CUR);
		} else {
			char command[MAX_COMMAND_LENGTH];
			fgets(command, MAX_COMMAND_LENGTH, in);
		}
		if (i == 0)
			fscanf(in, "%s", im->type);
		else if (i == 1)
			fscanf(in, "%d%d\n", &im->width, &im->height);
		else if (i == 2)
			fscanf(in, "%d\n", &im->max_value);
	}
}

int type_verify(const image_t *im)
{
	if (strcmp(im->type, "P2") == 0)
		return 2;
	if (strcmp(im->type, "P3") == 0)
		return 3;
	if (strcmp(im->type, "P5") == 0)
		return 5;

	return 6;
}

int is_color(const image_t *im)
{
	if (type_verify(im) == 5 || type_verify(im) == 2)
		return 0;

	return 1;
}
