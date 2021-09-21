#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constant defines
#define GRAYSCALE 'g'
#define COLOR 'c'
#define ASCII 't'
#define BINARY 'b'

// type defines
typedef struct {
	int x1, y1, x2, y2;
} rectangle;

typedef int *pixel;
typedef struct {
	// file format
	char type[4];
	int maxvalue;
	int width;
	int height;
	// Map of pixels - the actual image is represented as a map
	pixel **map;
	rectangle selected_area;
} image;

// load command
void load(FILE **p_file, image *p_image);
// Moves the pointer in the file to the next row
void next_row(FILE **p_file);
// Skips comments from the file and returns the first value not included by one
char skip_comments(FILE **p_file);
// Dynamically allocates the map of the image and checks for any errors
int allocate_map(image *p_image, int pixel_dim, char *file_name);
// Reads and loads the information about the structure of the map
void read_metadata(FILE **p_file, image *p_image);
// Reads and loads the map
void read_map(FILE **p_file,
			  image *p_image,
			  char *file_name,
			  char color_set,
			  char memory_set);

// select command
void select(FILE **p_file, image *p_image);
// Reads one coordinate from the buffer and converts it into integer
int read_coordinate(void);

// rotate command
void rotate(FILE **p_file, image *p_image);
// Rotates a square area by 90d
void rotate_90_square(image *p_image);
// Rotates the whole image by 90d and check for errors
int rotate_90_all(image *p_image);
// Checks whether an area can be rotated and expresses the type of the area
int valid_rotate_area(rectangle area, int width, int height);
// Rotates a square area and checks for errors
int full_rotation_square(image *p_image, char *parameter);
// Rotates the whole image and checks for errors
int full_rotation_all(image *p_image, char *parameter);

// crop command
void crop(FILE **p_file, image *p_image);
// grayscale command
void grayscale(FILE **p_file, image *p_image);
// sepia command
void sepia(FILE **p_file, image *p_image);

//save command
void save(FILE **p_file, image *p_image);
// Writes all the information about the output image in the output file
void write_image(FILE **p_file, image *p_image, int pixel_dim, char memory_set);

// Opens a file and checks whether the opening was successful
int file_open(FILE **p_file, char *file_name, char *type);
// Frees the memory ocuppied by a map
void clear_map(int width, int height, pixel ***map);
//exit command
void exit_program(FILE **p_file, image *p_image);
