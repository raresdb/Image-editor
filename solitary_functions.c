#include "image_editor.h"
void crop(FILE **p_file, image *p_image)
{
	if (!*p_file) {
		printf("No image loaded\n");
		return;
	}
	int pixel_dim;
	if (p_image->type[1] == '3' || p_image->type[1] == '6')
		// color
		pixel_dim = 3;
	else
		// grayscale
		pixel_dim = 1;
	int x1 = p_image->selected_area.x1;
	int x2 = p_image->selected_area.x2;
	int y1 = p_image->selected_area.y1;
	int y2 = p_image->selected_area.y2;
	pixel **cropped_map;
	pixel **auxiliary;
	// Allocate the new map
	cropped_map = malloc((y2 - y1) * sizeof(pixel *));
	if (!cropped_map) {
		printf("Not enough memory\n");
		return;
	}
	// In case of failure free what has been allocated
	for (int i = 0; i < (y2 - y1); i++) {
		cropped_map[i] = malloc((x2 - x1) * sizeof(pixel));
		if (!cropped_map[i]) {
			printf("Not enough memory\n");
			for (int k = 0; k < i; k++)
				free(cropped_map[k]);
			free(cropped_map);
			return;
		}
	}
	// Allocate the pixels
	for (int i = 0; i < (y2 - y1); i++)
		for (int j = 0; j < (x2 - x1); j++)
			cropped_map[i][j] = malloc(pixel_dim * sizeof(int));
	// Build the cropped map
	for (int i = 0; i < (y2 - y1); i++)
		for (int j = 0; j < (x2 - x1); j++)
			for (int k = 0; k < pixel_dim; k++)
				cropped_map[i][j][k] = p_image->map[i + y1][j + x1][k];
	// Set the new map as current and free the old map
	auxiliary = p_image->map;
	p_image->map = cropped_map;
	clear_map(p_image->width, p_image->height, &auxiliary);
	// Update the metadata
	p_image->width = x2 - x1;
	p_image->height = y2 - y1;
	p_image->selected_area.x1 = 0;
	p_image->selected_area.y1 = 0;
	p_image->selected_area.x2 = p_image->width;
	p_image->selected_area.y2 = p_image->height;
	printf("Image cropped\n");
}

void grayscale(FILE **p_file, image *p_image)
{
	if (!*p_file) {
		printf("No image loaded\n");
		return;
	}
	if (p_image->type[1] == '2' || p_image->type[1] == '5') {
		// It's already grayscale
		printf("Grayscale filter not available\n");
		return;
	}
	for (int i = p_image->selected_area.y1; i < p_image->selected_area.y2; i++)
		for (int j = p_image->selected_area.x1; j < p_image->selected_area.x2;
			 j++) {
			double oldred = p_image->map[i][j][0];
			double oldgreen = p_image->map[i][j][1];
			double oldblue = p_image->map[i][j][2];
			double newcolor = (oldred + oldgreen + oldblue) / 3;
			p_image->map[i][j][0] = round(newcolor);
			p_image->map[i][j][1] = round(newcolor);
			p_image->map[i][j][2] = round(newcolor);
		}
	printf("Grayscale filter applied\n");
}

void sepia(FILE **p_file, image *p_image)
{
	if (!*p_file) {
		printf("No image loaded\n");
		return;
	}
	if (p_image->type[1] == '2' || p_image->type[1] == '5') {
		// It's a grayscale image
		printf("Sepia filter not available\n");
		return;
	}
	for (int i = p_image->selected_area.y1; i < p_image->selected_area.y2; i++)
		for (int j = p_image->selected_area.x1; j < p_image->selected_area.x2;
			 j++) {
			double oldred = p_image->map[i][j][0];
			double oldgreen = p_image->map[i][j][1];
			double oldblue = p_image->map[i][j][2];
			int newred =
				round(0.393 * oldred + 0.769 * oldgreen + 0.189 * oldblue);
			int newgreen =
				round(0.349 * oldred + 0.686 * oldgreen + 0.168 * oldblue);
			int newblue =
				round(0.272 * oldred + 0.534 * oldgreen + 0.131 * oldblue);
			// Assuring the case when the new value > maxvalue
			p_image->map[i][j][0] =
				p_image->maxvalue < newred ? p_image->maxvalue : newred;
			p_image->map[i][j][1] =
				p_image->maxvalue < newgreen ? p_image->maxvalue : newgreen;
			p_image->map[i][j][2] =
				p_image->maxvalue < newblue ? p_image->maxvalue : newblue;
		}
	printf("Sepia filter applied\n");
}

void exit_program(FILE **p_file, image *p_image)
{
	if (!*p_file)
		printf("No image loaded\n");
	else
		fclose(*p_file);
	if (p_image->map)
		clear_map(p_image->width, p_image->height, &p_image->map);
}
