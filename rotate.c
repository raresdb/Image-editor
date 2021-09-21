#include "image_editor.h"
void rotate(FILE **p_file, image *p_image)
{
	// The parameter is the angle of rotation
	char *parameter = strtok(NULL, "\n");
	if (!*p_file) {
		printf("No image loaded\n");
	} else {
		switch (valid_rotate_area(p_image->selected_area, p_image->width,
								  p_image->height)) {
		case 0:
			// invalid area
			printf("The selection must be square\n");
			break;
		case 1:
			// square area
			if (full_rotation_square(p_image, parameter))
				printf("Rotated %s\n", parameter);
			break;
		case 2:
			// full image
			if (full_rotation_all(p_image, parameter))
				printf("Rotated %s\n", parameter);
			break;
		}
	}
}

void rotate_90_square(image *p_image)
{
	int x1 = p_image->selected_area.x1;
	int x2 = p_image->selected_area.x2;
	int y1 = p_image->selected_area.y1;
	int y2 = p_image->selected_area.y2;
	int pixel_dim;
	if (p_image->type[1] == '3' || p_image->type[1] == '6')
		// color
		pixel_dim = 3;
	else
		// grayscale
		pixel_dim = 1;
	int area[y2 - y1][x2 - x1][pixel_dim];
	// Build the new rotated area
	for (int i = 0; i < y2 - y1; i++)
		for (int j = 0; j < x2 - x1; j++)
			for (int k = 0; k < pixel_dim; k++)
				area[i][j][k] = p_image->map[y2 - y1 - 1 - j + y1][i + x1][k];
	// Transcribe the information from the new area into the original map
	for (int i = 0; i < y2 - y1; i++)
		for (int j = 0; j < x2 - x1; j++)
			for (int k = 0; k < pixel_dim; k++)
				p_image->map[i + y1][j + x1][k] = area[i][j][k];
}

int rotate_90_all(image *p_image)
{
	int pixel_dim;
	if (p_image->type[1] == '3' || p_image->type[1] == '6')
		// color
		pixel_dim = 3;
	else
		// grayscale
		pixel_dim = 1;
	pixel **final_map;
	pixel **auxiliary;
	final_map = malloc(p_image->width * sizeof(pixel *));
	if (!final_map) {
		printf("Not enough memory\n");
		return 0;
	}
	for (int i = 0; i < p_image->width; i++) {
		final_map[i] = malloc(p_image->height * sizeof(pixel));
		// Free the memory that has been previously allocated
		if (!final_map[i]) {
			printf("Not enough memory\n");
			for (int k = 0; k < i; k++)
				free(final_map[k]);
			free(final_map);
			return 0;
		}
	}
	// Allocate the pixels
	for (int i = 0; i < p_image->width; i++)
		for (int j = 0; j < p_image->height; j++)
			final_map[i][j] = malloc(pixel_dim * sizeof(int));
	// Build the new rotated map
	for (int i = 0; i <= p_image->width - 1; i++)
		for (int j = 0; j <= p_image->height - 1; j++)
			for (int k = 0; k < pixel_dim; k++)
				final_map[i][j][k] =
					p_image->map[p_image->height - 1 - j][i][k];
	// Save the new map as the current map and free the old map
	auxiliary = p_image->map;
	p_image->map = final_map;
	clear_map(p_image->width, p_image->height, &auxiliary);
	// Updating metadata
	int aux;
	aux = p_image->width;
	p_image->width = p_image->height;
	p_image->height = aux;
	p_image->selected_area.x2 = p_image->width;
	p_image->selected_area.y2 = p_image->height;
	return 1;
}

int valid_rotate_area(rectangle area, int width, int height)
{
	// full image - 2
	if (area.x1 == 0 && area.y1 == 0 && area.x2 == width && area.y2 == height)
		return 2;
	// square area - 1
	else if (area.x2 - area.x1 == area.y2 - area.y1)
		return 1;
	// invalid area - 0
	return 0;
}

int full_rotation_square(image *p_image, char *parameter)
{
	// The full rotation is realised by using multiple 90d rotations
	if (!strcmp(parameter, "0") || !strcmp(parameter, "360") ||
		!strcmp(parameter, "-360")) {
		return 1;
	} else if (!strcmp(parameter, "90") || !strcmp(parameter, "-270")) {
		rotate_90_square(p_image);
	} else if (!strcmp(parameter, "180") || !strcmp(parameter, "-180")) {
		rotate_90_square(p_image);
		rotate_90_square(p_image);
	} else if (!strcmp(parameter, "270") || !strcmp(parameter, "-90")) {
		rotate_90_square(p_image);
		rotate_90_square(p_image);
		rotate_90_square(p_image);
	} else {
		printf("Unsupported rotation angle\n");
		return 0;
	}
	return 1;
}

int full_rotation_all(image *p_image, char *parameter)
{
	// The full rotation is realised by using multiple 90d rotations
	if (!strcmp(parameter, "0") || !strcmp(parameter, "360") ||
		!strcmp(parameter, "-360"))
		return 1;
	else if (!strcmp(parameter, "90") || !strcmp(parameter, "-270")) {
		if (!rotate_90_all(p_image))
			return 0;
	} else if (!strcmp(parameter, "180") || !strcmp(parameter, "-180")) {
		for (int i = 0; i < 2; i++)
			if (!rotate_90_all(p_image))
				return 0;
	} else if (!strcmp(parameter, "270") || !strcmp(parameter, "-90")) {
		for (int i = 0; i < 3; i++)
			if (!rotate_90_all(p_image))
				return 0;
	} else {
		printf("Unsupported rotation angle\n");
		return 0;
	}
	return 1;
}
