#include "image_editor.h"
void load(FILE **p_file, image *p_image)
{
	// The parameter is the name of the input image file
	char *file_name = strtok(NULL, "\n");
	// Attempting to open the file for reading
	if (file_open(p_file, file_name, "r")) {
		read_metadata(p_file, p_image);
		switch (p_image->type[1]) {
		case '2':
			read_map(p_file, p_image, file_name, GRAYSCALE, ASCII);
			break;
		case '3':
			read_map(p_file, p_image, file_name, COLOR, ASCII);
			break;
		case '5':
			read_map(p_file, p_image, file_name, GRAYSCALE, BINARY);
			break;
		case '6':
			read_map(p_file, p_image, file_name, COLOR, BINARY);
			break;
		}
	} else {
		printf("Failed to load %s\n", file_name);
	}
}

void next_row(FILE **p_file)
{
	char c;
	do {
		c = fgetc(*p_file);
	} while (c != '\n');
}

char skip_comments(FILE **p_file)
{
	// First character of the line
	char first_char;
	// Skips lines that start with '#'
	do {
		fscanf(*p_file, "%c", &first_char);
		if (first_char == '#')
			next_row(p_file);
	} while (first_char == '#');
	// First not a '#' is the first not contained by a comment
	return first_char;
}

int allocate_map(image *p_image, int pixel_dim, char *file_name)
{
	p_image->map = malloc(p_image->height * sizeof(pixel *));
	if (!p_image->map) {
		// Each error code(0) is preceded by an error message so that
		// read_map() can be spared of that
		printf("Failed to load %s\n", file_name);
		return 0;
	}
	for (int i = 0; i < p_image->height; i++) {
		p_image->map[i] = malloc(p_image->width * sizeof(pixel));
		// If the allocation of one vector fails then all previous allocations
		// will be freed including the matrix pointer
		if (!p_image->map) {
			printf("Failed to load %s\n", file_name);
			for (int j = 0; j < i; j++)
				free(p_image->map[j]);
			free(p_image->map);
			return 0;
		}
	}
	// Allocating the space for each pixel
	for (int i = 0; i < p_image->height; i++)
		for (int j = 0; j < p_image->width; j++)
			p_image->map[i][j] = malloc(pixel_dim * sizeof(int));
	return 1;
}

void read_metadata(FILE **p_file, image *p_image)
{
	// Checking to make sure there will be no residual of an old map
	if (p_image->map)
		clear_map(p_image->width, p_image->height, &p_image->map);
	p_image->type[0] = skip_comments(p_file);
	p_image->type[1] = fgetc(*p_file);
	p_image->type[2] = '\0';
	fscanf(*p_file, "%d", &p_image->width);
	fscanf(*p_file, "%d", &p_image->height);
	fscanf(*p_file, "%d", &p_image->maxvalue);
}

void read_map(FILE **p_file,
			  image *p_image,
			  char *file_name,
			  char color_set,
			  char memory_set)
{
	// Moving the pointer in the file to the next line
	if (memory_set == BINARY)
		fseek(*p_file, 1, SEEK_CUR);
	if (color_set == GRAYSCALE && allocate_map(p_image, 1, file_name)) {
		for (int i = 0; i < p_image->height; i++)
			for (int j = 0; j < p_image->width; j++)
				if (memory_set == ASCII)
					fscanf(*p_file, "%d", p_image->map[i][j]);
				else
					// The other option is BINARY
					p_image->map[i][j][0] = fgetc(*p_file);
		printf("Loaded %s\n", file_name);
	}
	if (color_set == COLOR && allocate_map(p_image, 3, file_name)) {
		for (int i = 0; i < p_image->height; i++)
			for (int j = 0; j < p_image->width; j++)
				if (memory_set == ASCII) {
					for (int k = 0; k < 3; k++)
						fscanf(*p_file, "%d", &p_image->map[i][j][k]);
				} else {
					// The other option is BINARY
					for (int k = 0; k < 3; k++)
						p_image->map[i][j][k] = fgetc(*p_file);
				}
		printf("Loaded %s\n", file_name);
	}
	// Marking the selected area as the FULL image
	p_image->selected_area.x1 = 0;
	p_image->selected_area.y1 = 0;
	p_image->selected_area.x2 = p_image->width;
	p_image->selected_area.y2 = p_image->height;
}
