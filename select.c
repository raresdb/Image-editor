#include "image_editor.h"
void select(FILE **p_file, image *p_image)
{
	if (!*p_file) {
		printf("No image loaded\n");
	} else {
		int x1, x2, y1, y2;
		char *area = strtok(NULL, "\n,\0");
		if (!strcmp(area, "ALL") || !strcmp(area, "ALL ")) {
			p_image->selected_area.x1 = 0;
			p_image->selected_area.y1 = 0;
			p_image->selected_area.x2 = p_image->width;
			p_image->selected_area.y2 = p_image->height;
			printf("Selected ALL\n");
		} else {
			x1 = 0;
			// Loads the first coordinate as a string
			char *load;
			load = strtok(area, " ");
			for (size_t i = 0; i < strlen(load); i++) {
				// Converting every numeric character into a decimal digit
				x1 += (load[i] - 48) * pow(10, strlen(load) - i - 1);
			}
			y1 = read_coordinate();
			x2 = read_coordinate();
			y2 = read_coordinate();
			if (x1 > x2) {
				int aux;
				aux = x1;
				x1 = x2;
				x2 = aux;
			}
			if (y1 > y2) {
				int aux;
				aux = y1;
				y1 = y2;
				y2 = aux;
			}
			if (x1 == -2 || x2 == -2 || y1 == -2 || y2 == -2) {
				// Error code -2 : invalid description of coordinates
				printf("Invalid command\n");
			} else if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 ||
					   x1 > p_image->width || x2 > p_image->width ||
					   y1 > p_image->height || y2 > p_image->height ||
					   x1 == x2 || y1 == y2) {
				// Default error : invalid set of coordinates
				printf("Invalid set of coordinates\n");

			} else {
				p_image->selected_area.x1 = x1;
				p_image->selected_area.x2 = x2;
				p_image->selected_area.y1 = y1;
				p_image->selected_area.y2 = y2;
				printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
			}
		}
	}
}

int read_coordinate(void)
{
	int coordinate = 0;
	// Loads the next coordinate as a string
	char *load;
	load = strtok(NULL, " ,\n");
	if (!load)
		// Occurs when there are not enough coordinates (error code -2)
		return -2;
	for (size_t i = 0; i < strlen(load); i++) {
		if (load[i] == '-')
			// Negative numbers will be converted into -1 and seen as invalid
			// by default
			return -1;
		if (load[i] < '0' || load[i] > '9')
			// Occurs when there are letters in the parameters (error code -2)
			return -2;
		coordinate += (load[i] - 48) * pow(10, strlen(load) - i - 1);
	}
	// Returns an error code if failed or the coordinate in case of success
	return coordinate;
}
