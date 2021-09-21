#include "image_editor.h"
int main(void)
{
	// The empty variable is marked by the NULL character
	FILE *in_file = NULL;
	image loaded_image;
	// Marking the map as empty
	loaded_image.map = NULL;
	char command[50];
	// command without parameters
	char raw_command[50];
	int exit_status = 0;
	do {
		fgets(command, 50, stdin);
		strcpy(raw_command, command);
		// Separating the raw command from parameters
		strtok(raw_command, " ");
		if (!strcmp(raw_command, "LOAD")) {
			load(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "SELECT")) {
			select(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "ROTATE")) {
			rotate(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "CROP\n")) {
			crop(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "GRAYSCALE\n")) {
			grayscale(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "SEPIA\n")) {
			sepia(&in_file, &loaded_image);
		} else if (!strcmp(raw_command, "SAVE")) {
			save(&in_file, &loaded_image);
		} else if (!strcmp(command, "EXIT") || !strcmp(command, "EXIT\n")) {
			exit_program(&in_file, &loaded_image);
			exit_status = 1;
		} else {
			printf("Invalid command\n");
		}
	} while (!exit_status);
	return 0;
}
