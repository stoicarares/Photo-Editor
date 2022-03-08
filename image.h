// Stoica Rares-Tiberiu 311CA

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAX_TYPE_LENGTH 2

typedef struct {
	char type[MAX_TYPE_LENGTH + 1];
	int height;
	int width;
	int max_value;
} image_t;

typedef struct {
	int x1;
	int x2;
	int y1;
	int y2;
} selected_t;

// citeste parametrii imaginii de tipul image_t
void scan_image(image_t *im, FILE *in);

// verifica tipul imaginii(P2/P3/P5/P6)
int type_verify(const image_t *im);

// verifica daca imaginea este color sau nu
int is_color(const image_t *im);
