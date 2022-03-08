// Stoica Rares-Tiberiu 311CA

#include "functions.h"
#include "image.h"
#include "matrix.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MAX_FILENAME_LENGTH 100

unsigned char **load(char filename[], image_t *im, unsigned char **photo)
{
	// verific daca exista o imagine incarcata, iar daca e eliberez matricea
	if (photo)
		free_matrix(im->height, photo);

	// deschid fisierul din care trebuie incarcata poza
	FILE *input = fopen(filename, "rb");
	if (!input) {
		printf("Failed to load %s\n", filename);
		return NULL;
	}

	printf("Loaded %s\n", filename);
	scan_image(im, input);

	// incarc imaginea in functie de tipul ei
	// daca matricea este color alod de 3 ori mai multa memorie pe coloane
	if (type_verify(im) == 5) {
		photo = alloc_matrix(im->height, im->width);
		read_matrix_binary(im->height, im->width, input, photo);
	} else if (type_verify(im) == 6) {
		photo = alloc_matrix(im->height, 3 * im->width);
		read_matrix_binary(im->height, 3 * im->width, input, photo);
	} else if (type_verify(im) == 2) {
		photo = alloc_matrix(im->height, im->width);
		read_matrix_ascii(im->height, im->width, input, photo);
	} else if (type_verify(im) == 3) {
		photo = alloc_matrix(im->height, 3 * im->width);
		read_matrix_ascii(im->height, 3 * im->width, input, photo);
	}

	fclose(input);

	return photo;
}

void memorize_coordinates(selected_t *sel, int coordinate1, int coordinate2,
						  char axis)
{
	// modifica si retine coordonatele pe axa data ca parametru
	if (axis == 'x') {
		sel->x1 = coordinate1;
		sel->x2 = coordinate2;
	} else if (axis == 'y') {
		sel->y1 = coordinate1;
		sel->y2 = coordinate2;
	}
}

void check_parameters(selected_t *sel, image_t *im, int x1, int x2,
					  int y1, int y2)
{
	// retine vechile coordonate selectate
	int backup_x1 = sel->x1, backup_x2 = sel->x2;

	// verific daca parametrii pentru comanda SELECT si ii ordonez
	if (x1 < x2) {
		if (x1 >= 0 && x2 <= im->width) {
			memorize_coordinates(sel, x1, x2, 'x');
		} else {
			printf("Invalid set of coordinates\n");
			return;
		}
	} else if (x1 > x2) {
		if (x2 >= 0 && x1 <= im->width) {
			memorize_coordinates(sel, x2, x1, 'x');
		} else {
			printf("Invalid set of coordinates\n");
			return;
		}
	} else {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 < y2) {
		if (y1 >= 0 && y2 <= im->height) {
			memorize_coordinates(sel, y1, y2, 'y');
		} else {
			printf("Invalid set of coordinates\n");
			memorize_coordinates(sel, backup_x1, backup_x2, 'x');
			return;
		}
	} else if (y1 > y2) {
		if (y2 >= 0 && y1 <= im->height) {
			memorize_coordinates(sel, y2, y1, 'y');
		} else {
			printf("Invalid set of coordinates\n");
			memorize_coordinates(sel, backup_x1, backup_x2, 'x');
			return;
		}
	} else {
		printf("Invalid set of coordinates\n");
		memorize_coordinates(sel, backup_x1, backup_x2, 'x');
		return;
	}

	printf("Selected %d %d %d %d\n", sel->x1, sel->y1, sel->x2, sel->y2);
}

int is_all_selected(const selected_t *sel, const image_t *im)
{
	if (((sel->x2 - sel->x1) != im->width) ||
		((sel->y2 - sel->y1) != im->height))
		return 0;

	return 1;
}

void select_all(selected_t *sel, image_t *im)
{
	sel->x1 = 0;
	sel->x2 = im->width;
	sel->y1 = 0;
	sel->y2 = im->height;
}

void selection(selected_t *sel, image_t *im, unsigned char **photo, char data[])
{
	if (!photo) {
		printf("No image loaded\n");
		return;
	}

	// decupez stringul citit pentru a obtine parametrii si verific daca
	// e o comanda valida
	char *p = strtok(data, "\n ");
	int x1, y1, x2, y2, number_of_parameter = 0;

	while (p) {
		number_of_parameter++;
		// verifica daca primul parametru este ALL
		if (number_of_parameter == 1 && strcmp(p, "ALL") == 0) {
			select_all(sel, im);
			printf("Selected ALL\n");
			return;
		}
		// verifica daca sunt date coordonate negative
		for (unsigned int i = 0; i < strlen(p); i++) {
			if (p[i] == '-' && isdigit(p[i + 1]) > 0) {
				printf("Invalid set of coordinates\n");
				return;
			}
			// daca un parametru nu este numar, comanda este invalida
			if (isdigit(p[i]) == 0) {
				printf("Invalid command\n");
				return;
			}
		}
		if (number_of_parameter == 1)
			x1 = atoi(p);
		else if (number_of_parameter == 2)
			y1 = atoi(p);
		else if (number_of_parameter == 3)
			x2 = atoi(p);
		else if (number_of_parameter == 4)
			y2 = atoi(p);
		p = strtok(NULL, "\n ");
	}

	// daca sunr mai mult sau mai putin de 4 parametrii, comanda este invalida
	if (number_of_parameter != 4) {
		printf("Invalid command\n");
		return;
	}

	check_parameters(sel, im, x1, x2, y1, y2);
}

unsigned char **crop(image_t *im, selected_t *sel,
					 unsigned char **photo)
{
	if (!photo) {
		printf("No image loaded\n");
		return NULL;
	}

	// Daca e selectata poza integral, nu are ce sa taie, doar afiseaza mesajul
	if (is_all_selected(sel, im)) {
		printf("Image cropped\n");
		return photo;
	}

	unsigned char **s = NULL;

	// aloc o noua matrice in care copiez pixelii aflati in portiunea
	// selectata, care ii va deveni noua imagine dupa crop.
	// in funtie de tipul imaginii, aloc diferit
	if (!is_color(im)) {
		s = alloc_matrix(sel->y2 - sel->y1, sel->x2 - sel->x1);
		int new_line = 0;
		for (int i = sel->y1; i < sel->y2; ++i) {
			int new_col = 0;
			for (int j = sel->x1; j < sel->x2; ++j)
				s[new_line][new_col++] = photo[i][j];
			new_line++;
		}

		free_matrix(im->height, photo);
		im->height = sel->y2 - sel->y1;
		im->width = sel->x2 - sel->x1;
	} else if (is_color(im)) {
		s = alloc_matrix(sel->y2 - sel->y1, 3 * (sel->x2 - sel->x1));
		int new_line = 0;
		for (int i = sel->y1; i < sel->y2; ++i) {
			int new_col = 0;
			for (int j = 3 * sel->x1; j < 3 * sel->x2; ++j)
				s[new_line][new_col++] = photo[i][j];
			new_line++;
		}

		free_matrix(im->height, photo);
		im->height = sel->y2 - sel->y1;
		im->width = sel->x2 - sel->x1;
	}

	printf("Image cropped\n");

	return s;
}

unsigned char **rotate(unsigned char **photo, selected_t *sel,
					   image_t *im, int angle)
{
	if (!photo) {
		printf("No image loaded\n");
		return NULL;
	}
	// daca unghiul este 360 sau 0, nu se aplica nicio modificare
	if (angle == 0 || angle == 360 || angle == -360)
		printf("Rotated %d\n", angle);
	else if (angle == 90 || angle == -270)
		photo = rotate_90_right(photo, im, sel, angle);
	else if (angle == 180 || angle == -180)
		photo = rotate_180(photo, im, sel, angle);
	else if (angle == 270 || angle == -90)
		photo = rotate_90_left(photo, im, sel, angle);
	else
		printf("Unsupported rotation angle\n");

	return photo;
}

void apply_sepia_filter(unsigned char **photo, const image_t *im,
						const selected_t *sel)
{
	if (!photo) {
		printf("No image loaded\n");
		return;
	}

	// daca imaginea nu e color, nu se poate aplica filtrul
	if (!is_color(im)) {
		printf("Sepia filter not available\n");
		return;
	}

	for (int i = sel->y1; i < sel->y2; i++)
		for (int j = sel->x1 * 3; j < sel->x2 * 3 - 2; j += 3) {
			int new_red = 0, new_green = 0, new_blue = 0;
			int old_red = 0, old_green = 0, old_blue = 0;
			// convertesc pixelii din unsigned char in int ca sa le pot calcula
			// noua valoare
			old_red = (int)photo[i][j];
			old_green = (int)photo[i][j + 1];
			old_blue = (int)photo[i][j + 2];
			// calculez noua valoare a fiecarui pixel si o rotunjesc
			new_red = round(0.393 * old_red + 0.769 * old_green +
				0.189 * old_blue);
			new_green = round(0.349 * old_red + 0.686 * old_green +
				0.168 * old_blue);
			new_blue = round(0.272 * old_red + 0.534 * old_green +
				0.131 * old_blue);

			// se verifica daca noile valori depasesc valoarea mxima permisa
			if (new_red > im->max_value)
				photo[i][j] = (unsigned char)im->max_value;
				else
					photo[i][j] = (unsigned char)new_red;
			if (new_green > im->max_value)
				photo[i][j + 1] = (unsigned char)im->max_value;
				else
					photo[i][j + 1] = (unsigned char)new_green;
			if (new_blue > im->max_value)
				photo[i][j + 2] = (unsigned char)im->max_value;
				else
					photo[i][j + 2] = (unsigned char)new_blue;
		}

	printf("Sepia filter applied\n");
}

void apply_grayscale_filter(unsigned char **photo, const image_t *im,
							const selected_t *sel)
{
	if (!photo) {
		printf("No image loaded\n");
		return;
	}

	// daca imaginea nu e color, nu se poate aplica filtrul
	if (!is_color(im)) {
		printf("Grayscale filter not available\n");
		return;
	}

	for (int i = sel->y1; i < sel->y2; i++)
		for (int j = sel->x1 * 3; j < sel->x2 * 3 - 2; j += 3) {
			int old_red = 0, old_green = 0, old_blue = 0, new_grey = 0;

			old_red = (int)photo[i][j];
			old_green = (int)photo[i][j + 1];
			old_blue = (int)photo[i][j + 2];
			new_grey = round(old_red + old_green + old_blue) / 3;
			photo[i][j] = (unsigned char)new_grey;
			photo[i][j + 1] = (unsigned char)new_grey;
			photo[i][j + 2] = (unsigned char)new_grey;
		}

	printf("Grayscale filter applied\n");
}

void save(unsigned char **photo, char data[], const image_t *im)
{
	char *p = NULL, filename[MAX_FILENAME_LENGTH + 1];
	int argument_number = 0, verify_ascii = 0;

	// separ stringul citit in cuvinte
	p = strtok(data, "\n ");
	argument_number++;
	// primul argument este numele fisierului pe care il retin
	strcpy(filename, p);
	while (p) {
		// verific daca al doilea argument este ascii
		if (argument_number == 2)
			if (strcmp(p, "ascii") == 0)
				verify_ascii = 1;
		argument_number++;
		p = strtok(NULL, "\n ");
	}

	FILE *output = fopen(filename, "wb");
	if (!output)
		return;

	if (!photo) {
		printf("No image loaded\n");
		fclose(output);
		return;
	}

	// daca se citeste si parametrul ascii, se salveaza in format ascii,
	// daca nu se salveaza in format binar, in functie de tipul imaginii
	if (verify_ascii == 0) {
		if (!is_color(im)) {
			fprintf(output, "P5\n%d %d\n%d\n", im->width, im->height,
					im->max_value);
			print_matrix_binary(im->height, im->width, photo, output);
		} else if (is_color(im)) {
			fprintf(output, "P6\n%d %d\n%d\n", im->width, im->height,
					im->max_value);
			print_matrix_binary(im->height, 3 * im->width, photo, output);
		}
		printf("Saved %s\n", filename);
	} else {
		if (!is_color(im)) {
			fprintf(output, "P2\n%d %d\n%d\n", im->width, im->height,
					im->max_value);
			print_matrix_ascii(im->height, im->width, photo, output);
		} else if (is_color(im)) {
			fprintf(output, "P3\n%d %d\n%d\n", im->width, im->height,
					im->max_value);
			print_matrix_ascii(im->height, 3 * im->width, photo, output);
		}
		printf("Saved %s\n", filename);
	}
	fclose(output);
}

void exiting(unsigned char **photo, const image_t *im)
{
	if (photo)
		free_matrix(im->height, photo);
	else
		printf("No image loaded\n");
}
