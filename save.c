#include "image_editor.h"
void save(FILE **p_file, image *p_image)
{
	if (!*p_file) {
		printf("No image loaded\n");
		return;
	}
	// Separating the parameters in case they are 2
	char *file_name = strtok(NULL, " ,\n");
	char *parameter = strtok(NULL, "\n");
	FILE *out_file = NULL;
	if (!file_open(&out_file, file_name, "w")) {
		printf("Failed to open the output file\n");
		return;
	}
	int pixel_dim;
	if (p_image->type[1] == '3' || p_image->type[1] == '6')
		// color
		pixel_dim = 3;
	else
		// grayscale
		pixel_dim = 1;
	if (!parameter)
		// no parameter
		write_image(&out_file, p_image, pixel_dim, BINARY);
	else if (!strcmp(parameter, "ascii"))
		write_image(&out_file, p_image, pixel_dim, ASCII);
	printf("Saved %s\n", file_name);
	fclose(out_file);
}

void write_image(FILE **p_file, image *p_image, int pixel_dim, char memory_set)
{
	if (memory_set == ASCII) {
		if (pixel_dim == 1)
			// grayscale
			fprintf(*p_file, "P2\n");
		else
			// color
			fprintf(*p_file, "P3\n");
		fprintf(*p_file, "%d %d\n", p_image->width, p_image->height);
		fprintf(*p_file, "%d\n", p_image->maxvalue);
		for (int i = 0; i < p_image->height; i++) {
			for (int j = 0; j < p_image->width; j++)
				for (int k = 0; k < pixel_dim; k++)
					fprintf(*p_file, "%d ", p_image->map[i][j][k]);
			fprintf(*p_file, "\n");
		}
	} else {
		// BINARY
		if (pixel_dim == 1)
			// grayscale
			fprintf(*p_file, "P5\n");
		else
			// color
			fprintf(*p_file, "P6\n");
		fprintf(*p_file, "%d %d\n", p_image->width, p_image->height);
		fprintf(*p_file, "%d\n", p_image->maxvalue);
		for (int i = 0; i < p_image->height; i++)
			for (int j = 0; j < p_image->width; j++)
				for (int k = 0; k < pixel_dim; k++)
					fprintf(*p_file, "%c", p_image->map[i][j][k]);
	}
}
