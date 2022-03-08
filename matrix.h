// Stoica Rares-Tiberiu 311CA

#pragma once

#include <stdio.h>
#include <stdlib.h>

// aloca matricea-imagine
unsigned char **alloc_matrix(int height, int width);

// citeste matricea in format ascii
void read_matrix_ascii(int height, int width, FILE *in, unsigned char **photo);

// citeste matricea in format binar
void read_matrix_binary(int height, int width, FILE *in, unsigned char **photo);

// afiseaza matricea in format binar
void print_matrix_binary(int height, int width, unsigned char **a, FILE *out);

// afiseaza matricea in format ascii
void print_matrix_ascii(int height, int width, unsigned char **a, FILE *out);

// elibereaza memoria
void free_matrix(int height, unsigned char **photo);
