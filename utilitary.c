#include "image_editor.h"
int file_open(FILE **p_file, char *file_name, char *type)
{
	// First it closes the old stream - if it exists
	if (*p_file) {
		fclose(*p_file);
		*p_file = NULL;
	}
	*p_file = fopen(file_name, type);
	if (!*p_file)
		return 0;
	return 1;
}

void clear_map(int width, int height, pixel ***map)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			free((*map)[i][j]);
		free((*map)[i]);
	}
	free(*map);
	*map = NULL;
}
