// Stoica Rares-Tiberiu 311CA

#pragma once

#include "image.h"
#include "matrix.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

//  incarca imaginile, citind din fisier in functie de tipul acestora
unsigned char **load(char filename[], image_t *im, unsigned char **photo);

// memoreaza coordonatele nou introduse in cadrul functiei "selection",
// sau daca sunt incorecte, le pune inapoi pe cele anterior selectate
void memorize_coordinates(selected_t *sel, int coordinate1, int coordinate2,
						  char axis);

// verifica daca valorile date in cadrul functiei "selection" sunt incadrate
// in interiorul imaginii
void check_parameters(selected_t *sel, image_t *im, int x1, int x2,
					  int y1, int y2);

// selecteaza intreaga imagine
void select_all(selected_t *sel, image_t *im);

// verifica daca parametrii introdusi sunt valizi si daca sunt in numar de 4
// selecteaza intreaga imagine daca este prezent argumentul ALL
void selection(selected_t *sel, image_t *im, unsigned char **photo,
			   char data[]);

// verifica daca este selectata intreaga imagine
int is_all_selected(const selected_t *sel, const image_t *im);

// functia care taie imaginea intre coordonatele selectate
unsigned char **crop(image_t *im, selected_t *sel,
					 unsigned char **photo);

// interschimbare unsigned char
void swap_char(unsigned char *a, unsigned char *b);

// interschimbare int
void swap_int(int *a, int *b);

// face transpusa matricii-imagine pentru formatul grayscale(P2, P5)
void transpose(unsigned char **photo, const selected_t *sel);

// face transpusa matricii-imagine pentru formatul color(P3, P6)
void transpose_color(unsigned char **photo, const selected_t *sel);

// inverseaza coloanele matricii-imagine pentru formatul grayscale
void reverse_columns(unsigned char **photo, const selected_t *sel);

// inverseaza liniile matricii-imagine pentru formatul grayscale
void reverse_lines(unsigned char **photo, const selected_t *sel);

// inverseaza coloanele matricii-imagine pentru formatul color
void reverse_columns_color(unsigned char **photo, const selected_t *sel);

// inverseaza liniile matricii-imagine pentru formatul color
void reverse_lines_color(unsigned char **photo, const selected_t *sel);

// roteste imaginea grayscale selectata 90 de grade la dreapta
void right_90_gray(unsigned char **photo, const selected_t *sel);

// roteste imaginea grayscale selectata 90 de grade la stanga
void left_90_gray(unsigned char **photo, const selected_t *sel);

// roteste imaginea color selectata 90 de grade la stanga
void left_90_color(unsigned char **photo, const selected_t *sel);

// roteste imaginea color selectata 90 de grade la dreapta
void right_90_color(unsigned char **photo, const selected_t *sel);

// roteste imaginea grayscale selectata(integral) 180 de grade
void gray_180(unsigned char **photo, const selected_t *sel);

// roteste imaginea color selectata(integral) 180 de grade
void color_180(unsigned char **photo, const selected_t *sel);

// roteste imaginea grayscale integral 90 de grade la dreapta
unsigned char **right_all_90_gray(unsigned char **photo, image_t *im,
								  selected_t *sel);

// roteste imaginea grayscale integral 90 de grade la stanga
unsigned char **left_all_90_gray(unsigned char **photo, image_t *im,
								 selected_t *sel);

// roteste imaginea color integral 90 de grade la dreapta
unsigned char **right_all_90_color(unsigned char **photo, image_t *im,
								   selected_t *sel);

// roteste imaginea color integral 90 de grade la stanga
unsigned char **left_all_90_color(unsigned char **photo, image_t *im,
								  selected_t *sel);

// verifica daca ce s-a selectat anterior este patrat
int square_verify(const selected_t *sel);

// alege care functie de intoarcere sa aplice si pe ce fel de
// format(color/grayscale) daca unghiul dat este 90/-270
unsigned char **rotate_90_right(unsigned char **photo, image_t *im,
								selected_t *sel, int angle);

// alege care functie de intoarcere sa aplice si pe ce fel de
// format(color/grayscale) daca unghiul dat este -90/270
unsigned char **rotate_90_left(unsigned char **photo, image_t *im,
							   selected_t *sel, int angle);

// alege pe ce format(color/grayscale) sa aplice rotirea de 180 de grade
unsigned char **rotate_180(unsigned char **photo, image_t *im, selected_t *sel,
						   int angle);

// functia care primeste unghiul si le apeleaza pe cele mentionate anterior
unsigned char **rotate(unsigned char **photo, selected_t *sel,
					   image_t *im, int angle);

// functia care modifica valorile pixelilor pentru a obtine filtrul sepia
void apply_sepia_filter(unsigned char **photo, const image_t *im,
						const selected_t *sel);

// functia care modifica valorile pixelilor pentru a obtine filtrul grayscale
void apply_grayscale_filter(unsigned char **photo, const image_t *im,
							const selected_t *sel);

// functia care salveaza imaginea dupa caz
void save(unsigned char **photo, char data[], const image_t *im);

// functia care da free imaginii
void exiting(unsigned char **photo, const image_t *im);
