// Stoica Rares-Tiberiu 311CA

#include "functions.h"
#include "image.h"
#include "matrix.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

void swap_char(unsigned char *a, unsigned char *b)
{
	unsigned char tmp = *a;
	*a = *b;
	*b = tmp;
}

void swap_int(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void transpose(unsigned char **photo, const selected_t *sel)
{
	int index_col = 0;

	for (int i = sel->y1; i < sel->y2; i++) {
		int new_col = sel->x1 + index_col, index_line = 0;
		for (int j = sel->x1 + index_col; j < sel->x2; j++)
			swap_char(&photo[i][j], &photo[i + index_line++][new_col]);
		index_col++;
	}
}

void transpose_color(unsigned char **photo, const selected_t *sel)
{
	int add_by_three = 0;

	for (int i = sel->y1; i < sel->y2; i++) {
		int new_line = i, new_col = 3 * sel->x1 + 3 * add_by_three;
		for (int j = sel->x1 * 3 + add_by_three * 3; j < 3 * sel->x2; j++) {
			// noile valori ale colanelor se repeta din 3 in 3, avand valori
			// diferite de la linie la linie
			if (j % 3 != 0 || j == 3 * sel->x1 + 3 * add_by_three) {
				swap_char(&photo[new_line][new_col++], &photo[i][j]);
			} else {
				// cand indicele liniei este divizivil cu 3, se revine la
				// valoarea initiala.
				new_col = 3 * sel->x1 + 3 * add_by_three;
				swap_char(&photo[++new_line][new_col++], &photo[i][j]);
			}
		}

		// pentru a nu face o modificare de 2 ori, verifica din 3 in 3 coloane
		add_by_three++;
	}
}

void reverse_columns_color(unsigned char **photo, const selected_t *sel)
{
	int line_index = 0, new_col = 0;

	// se inverseaza coloanele cate 3
	for (int j = sel->x1 * 3; j < ((sel->x2 + sel->x1) * 3) / 2; j++) {
		if (j % 3 == 0) {
			new_col = (sel->x2 - line_index - 1) * 3;
			line_index++;
		} else {
			new_col++;
		}
		for (int i = sel->y1; i < sel->y2; i++)
			swap_char(&photo[i][j], &photo[i][new_col]);
	}
}

void reverse_columns(unsigned char **photo, const selected_t *sel)
{
	int p = sel->x1, q = sel->x2 - 1;

	for (int j = 0; j < (sel->x2 - sel->x1) / 2; ++j) {
		for (int i = sel->y1; i < sel->y2; ++i)
			swap_char(&photo[i][p], &photo[i][q]);
		p++;
		q--;
	}
}

void reverse_lines_color(unsigned char **photo, const selected_t *sel)
{
	int p = sel->y1, q = sel->y2 - 1;

	// inverseaza coloanele cate 3
	for (int i = 0; i < (sel->y2 - sel->y1) / 2; ++i) {
		for (int j = sel->x1 * 3; j < sel->x2 * 3; ++j)
			swap_char(&photo[p][j], &photo[q][j]);
		p++;
		q--;
	}
}

void reverse_lines(unsigned char **photo, const selected_t *sel)
{
	int p = sel->y1, q = sel->y2 - 1;

	for (int i = 0; i < (sel->y2 - sel->y1) / 2; ++i) {
		for (int j = sel->x1; j < sel->x2; ++j)
			swap_char(&photo[p][j], &photo[q][j]);
		p++;
		q--;
	}
}

void right_90_gray(unsigned char **photo, const selected_t *sel)
{
	transpose(photo, sel);
	reverse_columns(photo, sel);
}

void left_90_gray(unsigned char **photo, const selected_t *sel)
{
	transpose(photo, sel);
	reverse_lines(photo, sel);
}

void gray_180(unsigned char **photo, const selected_t *sel)
{
	reverse_columns(photo, sel);
	reverse_lines(photo, sel);
}

void color_180(unsigned char **photo, const selected_t *sel)
{
	reverse_columns_color(photo, sel);
	reverse_lines_color(photo, sel);
}

void left_90_color(unsigned char **photo, const selected_t *sel)
{
	transpose_color(photo, sel);
	reverse_lines_color(photo, sel);
}

void right_90_color(unsigned char **photo, const selected_t *sel)
{
	transpose_color(photo, sel);
	reverse_columns_color(photo, sel);
}

unsigned char **right_all_90_gray(unsigned char **photo, image_t *im,
								  selected_t *sel)
{
	unsigned char **s = alloc_matrix(im->width, im->height);

	// prima linie devine ultima coloana, ultima linie devine prima coloana
	for (int i = 0; i < im->height; ++i)
		for (int j = 0; j < im->width; ++j)
			s[j][im->height - i - 1] = photo[i][j];
	free_matrix(im->height, photo);
	swap_int(&im->height, &im->width);
	swap_int(&sel->x2, &sel->y2);

	return s;
}

unsigned char **left_all_90_gray(unsigned char **photo, image_t *im,
								 selected_t *sel)
{
	unsigned char **s = alloc_matrix(im->width, im->height);

	for (int i = 0; i < im->height; ++i)
		for (int j = 0; j < im->width; ++j)
			s[im->width - j - 1][i] = photo[i][j];
	free_matrix(im->height, photo);
	swap_int(&im->height, &im->width);
	swap_int(&sel->x2, &sel->y2);

	return s;
}

unsigned char **right_all_90_color(unsigned char **photo, image_t *im,
								   selected_t *sel)
{
	unsigned char **s = alloc_matrix(im->width, 3 * im->height);

	// elementele de pe prima linie se pun pe ultimele 3 coloane, iar
	// elementele de pe ultima coloana pe primele 3 linii,
	// in ordine crescatoare

	for (int i = 0; i < im->height; ++i) {
		int new_col = 3 * (im->height - i - 1), new_row = 0;
		for (int j = 0; j < im->width * 3; ++j)
			if ((j % 3) != 0 || j == 0) {
				s[new_row][new_col++] = photo[i][j];
			} else {
				new_col = 3 * (im->height - i - 1);
				s[++new_row][new_col++] = photo[i][j];
			}
	}

	free_matrix(im->height, photo);
	swap_int(&im->height, &im->width);
	swap_int(&sel->x2, &sel->y2);

	return s;
}

unsigned char **left_all_90_color(unsigned char **photo, image_t *im,
								  selected_t *sel)
{
	unsigned char **s = alloc_matrix(im->width, 3 * im->height);

	// elementele de pe prima linie se pun pe primele 3 coloane, in ordine
	// crescatoare de jos in sus
	for (int i = 0; i < im->height ; i++) {
		int new_row = 3 * i, new_line = im->width - 1;
		for (int j = 0; j < 3 * im->width; j++) {
			if ((j % 3) != 0 || j == 0) {
				s[new_line][new_row++] = photo[i][j];
			} else {
				new_row = 3 * i;
				s[--new_line][new_row++] = photo[i][j];
			}
		}
	}

	free_matrix(im->height, photo);
	swap_int(&im->height, &im->width);
	swap_int(&sel->x2, &sel->y2);

	return s;
}

int square_verify(const selected_t *sel)
{
	if ((sel->x2 - sel->x1) != (sel->y2 - sel->y1))
		return 0;

	return 1;
}

unsigned char **rotate_90_right(unsigned char **photo, image_t *im,
								selected_t *sel, int angle)
{
	if (square_verify(sel) == 0) {
		// daca selectia nu e patrata, dar este selectata toata imaginea
		if (is_all_selected(sel, im) == 0) {
			printf("The selection must be square\n");
		} else {
			// aplica functiile de rotire integrala, dupa tipul imaginii
			if (!is_color(im))
				photo = right_all_90_gray(photo, im, sel);
			else
				photo = right_all_90_color(photo, im, sel);

			printf("Rotated %d\n", angle);
		}
	} else {
		// aplica functiile de rotire selectata, dupa tipul imaginii
		if (!is_color(im))
			right_90_gray(photo, sel);
		else
			right_90_color(photo, sel);

		printf("Rotated %d\n", angle);
	}

	return photo;
}

unsigned char **rotate_90_left(unsigned char **photo, image_t *im,
							   selected_t *sel, int angle)
{
	if (square_verify(sel) == 0) {
		if (is_all_selected(sel, im) == 0) {
			printf("The selection must be square\n");
		} else {
			if (!is_color(im))
				photo = left_all_90_gray(photo, im, sel);
			else
				photo = left_all_90_color(photo, im, sel);

		printf("Rotated %d\n", angle);
		}
	} else {
		if (!is_color(im))
			left_90_gray(photo, sel);
		else
			left_90_color(photo, sel);

		printf("Rotated %d\n", angle);
	}

	return photo;
}

unsigned char **rotate_180(unsigned char **photo, image_t *im,
						   selected_t *sel, int angle)
{
	if (square_verify(sel) == 0) {
		if (is_all_selected(sel, im) == 0) {
			printf("The selection must be square\n");
		} else {
			if (!is_color(im))
				gray_180(photo, sel);
			else
				color_180(photo, sel);

		printf("Rotated %d\n", angle);
		}
	} else {
		if (!is_color(im))
			gray_180(photo, sel);
		else
			color_180(photo, sel);

		printf("Rotated %d\n", angle);
	}

	return photo;
}
