// Stoica Rares-Tiberiu 311CA

#include "matrix.h"
#include "image.h"
#include "functions.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_FILENAME_LENGTH 100

int main(void)
{
	unsigned char **photo = NULL;
	image_t im;
	selected_t sel;
	char command[MAX_COMMAND_LENGTH + 1], filename[MAX_FILENAME_LENGTH + 1];

	// Se citesc si se efectueaza comenzi pana la intalnirea "EXIT"
	scanf("%s", command);
	while ((strcmp(command, "EXIT") != 0)) {
		if (strcmp(command, "LOAD") == 0) {
			scanf("%s", filename);
			photo = load(filename, &im, photo);
			select_all(&sel, &im);
		} else if (strcmp(command, "SAVE") == 0) {
			char data[MAX_COMMAND_LENGTH];
			fgets(data, MAX_COMMAND_LENGTH + 1, stdin);
			save(photo, data, &im);
		} else if (strcmp(command, "SELECT") == 0) {
			char data[MAX_COMMAND_LENGTH];
			fgets(data, MAX_COMMAND_LENGTH + 1, stdin);
			selection(&sel, &im, photo, data);
		} else if (strcmp(command, "CROP") == 0) {
			photo = crop(&im, &sel, photo);
			select_all(&sel, &im);
		} else if (strcmp(command, "ROTATE") == 0) {
			int angle;
			scanf("%d", &angle);
			photo = rotate(photo, &sel, &im, angle);
		} else if (strcmp(command, "SEPIA") == 0) {
			apply_sepia_filter(photo, &im, &sel);
		} else if (strcmp(command, "GRAYSCALE") == 0) {
			apply_grayscale_filter(photo, &im, &sel);
		} else {
			char invalid[MAX_COMMAND_LENGTH];
			fgets(invalid, MAX_COMMAND_LENGTH + 1, stdin);
			printf("Invalid command\n");
		}
		scanf("%s", command);
	}
	exiting(photo, &im);

	return 0;
}
