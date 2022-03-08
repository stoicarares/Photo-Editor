// Stoica Rares-Tiberiu 311CA

#include "matrix.h"

#include <stdio.h>

unsigned char **alloc_matrix(int height, int width)
{
	unsigned char **photo = NULL;
	photo = (unsigned char **)malloc(height * sizeof(unsigned char *));
	if (!photo) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < height; ++i) {
		photo[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (!photo[i]) {
			fprintf(stderr, "malloc() failed\n");
			while (--i >= 0)
				free(photo[i]);
			free(photo);

			return NULL;
		}
	}

	return photo;
}

void read_matrix_ascii(int height, int width, FILE *in, unsigned char **photo)
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			fscanf(in, "%hhu", &photo[i][j]);
	}
}

void read_matrix_binary(int height, int width, FILE *in, unsigned char **photo)
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			fread(&photo[i][j], sizeof(unsigned char), 1, in);
	}
}

void print_matrix_binary(int height, int width, unsigned char **photo,
						 FILE *out)
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			fwrite(&photo[i][j], sizeof(unsigned char), 1, out);
	}
}

void print_matrix_ascii(int height, int width, unsigned char **photo, FILE *out)
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			fprintf(out, "%hhu ", photo[i][j]);
		fprintf(out, "\n");
	}
}

void free_matrix(int height, unsigned char **photo)
{
	for (int i = 0; i < height; ++i)
		free(photo[i]);
	free(photo);
}
