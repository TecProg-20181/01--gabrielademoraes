#include <stdio.h>

#define PIXELDIMENSION 512
#define RGB 3
#define RED 0
#define GREEN 1
#define BLUE 2
#define IMAGECOLOR 255

typedef struct properties_pixel {
	unsigned short int red;
	unsigned short int green;
	unsigned short int blue;
} Pixel;

typedef struct properties_image {
	//[width][height][rgb]
	unsigned short int pixel[PIXELDIMENSION][PIXELDIMENSION][RGB];
	unsigned int width;
	unsigned int height;
} Image;

Image gray_scale(Image img) {
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {
			int average = img.pixel[i][j][RED] + img.pixel[i][j][GREEN] + img.pixel[i][j][BLUE];
			average /= 3;

			img.pixel[i][j][RED] = average;
			img.pixel[i][j][GREEN] = average;
			img.pixel[i][j][BLUE] = average;
		}
	}
	return img;
}

void blur(unsigned int height, unsigned short int pixel[PIXELDIMENSION][PIXELDIMENSION][RGB], int size, unsigned int width) {
	for(unsigned int i = 0; i < height; ++i) {
		for(unsigned int j = 0; j < width; ++j) {
			Pixel average = {0, 0, 0};

			int min_height = (height - 1 > i + size/2) ? (i + size/2) : (height - 1);
			int min_width = (width - 1 > j + size/2) ? (j + size/2) : (width - 1);

			for(int x = ((0 > i - size/2) ? 0 : (i - size/2)); x <= min_height; ++x) {
				for(int y = ((0 > j - size/2) ? 0 : (j - size/2)); y <= min_width; ++y) {
					average.red += pixel[x][y][RED];
					average.green += pixel[x][y][GREEN];
					average.blue += pixel[x][y][BLUE];
				}
			}

			average.red /= size * size;
			average.green /= size * size;
			average.blue /= size * size;

			pixel[i][j][RED] = average.red;
			pixel[i][j][GREEN] = average.green;
			pixel[i][j][BLUE] = average.blue;
		}
	}
}

Image rotate_90degrees_right(Image img) {
	Image rotated;

	rotated.width = img.height;
	rotated.height = img.width;

	for(unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
		for(int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {

			rotated.pixel[i][j][RED] = img.pixel[x][y][RED];
			rotated.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
			rotated.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];

		}
	}
	return rotated;
}

void color_inversion(unsigned short int pixel[PIXELDIMENSION][PIXELDIMENSION][RGB], unsigned int width, unsigned int height) {
	for(unsigned int i = 0; i < height; ++i) {
		for(unsigned int j = 0; j < width; ++j) {
			pixel[i][j][RED] = IMAGECOLOR - pixel[i][j][RED];
			pixel[i][j][GREEN] = IMAGECOLOR - pixel[i][j][GREEN];
			pixel[i][j][BLUE] = IMAGECOLOR - pixel[i][j][BLUE];
		}
	}
}

Image crop_picture(Image img, int x, int y, int width, int height) {
	Image cropped;

	cropped.width = width;
	cropped.height = height;

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			cropped.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
			cropped.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
			cropped.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
		}
	}
	return cropped;
}

int main() {
	Image img;

	// read type of image
	char image_type[4];
	scanf("%s", image_type);

	// read width, height and color of image
	int max_color;
	scanf("%u %u %d", &img.width, &img.height, &max_color);

	// read all pixels of image
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {
			scanf("%hu %hu %hu", &img.pixel[i][j][RED], &img.pixel[i][j][GREEN], &img.pixel[i][j][BLUE]);
		}
	}

	int n_options;
	scanf("%d", &n_options);

	for(int i = 0; i < n_options; ++i) {
		int option;
		scanf("%d", &option);

		switch(option) {
			case 1: { // Gray Scale
				img = gray_scale(img);
				break;
			}
			case 2: { // Sepia
				for(unsigned int x = 0; x < img.height; ++x) {
					for(unsigned int j = 0; j < img.width; ++j) {
						unsigned short int pixel[3];
						pixel[0] = img.pixel[x][j][RED];
						pixel[1] = img.pixel[x][j][GREEN];
						pixel[2] = img.pixel[x][j][BLUE];

						int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
						int min_red = (IMAGECOLOR >  p) ? p : IMAGECOLOR;
						img.pixel[x][j][RED] = min_red;

						p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
						min_red = (IMAGECOLOR >  p) ? p : IMAGECOLOR;
						img.pixel[x][j][GREEN] = min_red;

						p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
						min_red = (IMAGECOLOR >  p) ? p : IMAGECOLOR;
						img.pixel[x][j][BLUE] = min_red;
					}
				}
				break;
			}
			case 3: { // Blur
				int size = 0;
				scanf("%d", &size);
				blur(img.height, img.pixel, size, img.width);
				break;
			}
			case 4: { // Rotation
				int number_of_times = 0;
				scanf("%d", &number_of_times);
				number_of_times %= 4;
				for(int j = 0; j < number_of_times; ++j) {
					img = rotate_90degrees_right(img);
				}
				break;
			}
			case 5: { // Mirroring
				int horizontal = 0;
				scanf("%d", &horizontal);

				int width = img.width, height = img.height;

				(horizontal == 1) ? (width /= 2) : (height /= 2);

				for(int i2 = 0; i2 < height; ++i2) {
					for(int j = 0; j < width; ++j) {
						int x = i2, y = j;

						(horizontal == 1) ? (y = img.width - 1 - j) : (x = img.height - 1 - i2);

						Pixel aux1;
						aux1.red = img.pixel[i2][j][RED];
						aux1.green = img.pixel[i2][j][GREEN];
						aux1.blue = img.pixel[i2][j][BLUE];

						img.pixel[i2][j][RED] = img.pixel[x][y][RED];
						img.pixel[i2][j][GREEN] = img.pixel[x][y][GREEN];
						img.pixel[i2][j][BLUE] = img.pixel[x][y][BLUE];

						img.pixel[x][y][RED] = aux1.red;
						img.pixel[x][y][GREEN] = aux1.green;
						img.pixel[x][y][BLUE] = aux1.blue;
					}
				}
				break;
			}
			case 6: { // Color Invertion
				color_inversion(img.pixel, img.width, img.height);
				break;
			}
			case 7: { // Crop picture
				int x, y;
				scanf("%d %d", &x, &y);
				int width, height;
				scanf("%d %d", &width, &height);

				img = crop_picture(img, x, y, width, height);
				break;
			}
		}
	}

	// print type of image
	printf("P3\n");
	// print width height and color of image
	printf("%u %u\n255\n", img.width, img.height);

	// print pixels of image
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {
			printf("%hu %hu %hu ", img.pixel[i][j][RED], img.pixel[i][j][GREEN], img.pixel[i][j][BLUE]);
		}
		printf("\n");
	}
	return 0;
}
