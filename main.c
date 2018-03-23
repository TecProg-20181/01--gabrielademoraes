#include <stdio.h>

#define IMAGEDIMENSION 512
#define IMAGECOLOR 255

typedef struct properties_pixel {
	unsigned short int red;
	unsigned short int green;
	unsigned short int blue;
} Pixel;

typedef struct properties_image {
	//[width][height]
	Pixel pixel[IMAGEDIMENSION][IMAGEDIMENSION];
	unsigned int width;
	unsigned int height;
} Image;

int min (int, int);
Image gray_scale(Image);
Image sepia(Image);
Image blur(Image, int);
Image rotate_90degrees_right(Image);
Image mirroring(Image, int);
Image color_inversion(Image);
Image crop_picture(Image, int, int, int, int);

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
			scanf("%hu %hu %hu", &img.pixel[i][j].red, &img.pixel[i][j].green, &img.pixel[i][j].blue);
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
				img = sepia(img);
				break;
			}
			case 3: { // Blur
				int blur_size = 0;
				scanf("%d", &blur_size);
				img = blur(img, blur_size);
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
				img = mirroring(img, horizontal);
				break;
			}
			case 6: { // Color Inversion
				img = color_inversion(img);
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
			printf("%hu %hu %hu ", img.pixel[i][j].red, img.pixel[i][j].green, img.pixel[i][j].blue);
		}
		printf("\n");
	}
	return 0;
}

int min(int first_term, int second_term) {
		return (first_term < second_term) ? first_term : second_term;
}

Image gray_scale(Image img) {
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {
			int average = img.pixel[i][j].red + img.pixel[i][j].green + img.pixel[i][j].blue;
			average /= 3;

			img.pixel[i][j].red = average;
			img.pixel[i][j].green = average;
			img.pixel[i][j].blue = average;
		}
	}
	return img;
}

Image sepia(Image img) {
	for(unsigned int x = 0; x < img.height; ++x) {
		for(unsigned int j = 0; j < img.width; ++j) {
			unsigned short int pixel[3];
			pixel[0] = img.pixel[x][j].red;
			pixel[1] = img.pixel[x][j].green;
			pixel[2] = img.pixel[x][j].blue;

			int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
			int min_p = min(IMAGECOLOR, p);
			img.pixel[x][j].red = min_p;

			p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
			min_p = min(IMAGECOLOR, p);
			img.pixel[x][j].green = min_p;

			p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
			min_p = min(IMAGECOLOR, p);
			img.pixel[x][j].blue = min_p;
		}
	}
	return img;
}

Image blur(Image img, int blur_size) {
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {

			Pixel average = {0, 0, 0};

			int min_height = min((img.height -1), (i + blur_size/2));
			int min_width = min((img.width -1), (j + blur_size/2));

			for(unsigned int x = ((0 > i - blur_size/2) ? 0 : (i - blur_size/2)); x <= min_height; ++x) {
				for(unsigned int y = ((0 > j - blur_size/2) ? 0 : (j - blur_size/2)); y <= min_width; ++y) {
					average.red += img.pixel[x][y].red;
					average.green += img.pixel[x][y].green;
					average.blue += img.pixel[x][y].blue;
				}
			}

			average.red /= blur_size * blur_size;
			average.green /= blur_size * blur_size;
			average.blue /= blur_size * blur_size;

			img.pixel[i][j].red = average.red;
			img.pixel[i][j].green = average.green;
			img.pixel[i][j].blue = average.blue;
		}
	}
	return img;
}

Image rotate_90degrees_right(Image img) {
	Image rotated;

	rotated.width = img.height;
	rotated.height = img.width;

	for(unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
		for(int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {

			rotated.pixel[i][j].red = img.pixel[x][y].red;
			rotated.pixel[i][j].green = img.pixel[x][y].green;
			rotated.pixel[i][j].blue = img.pixel[x][y].blue;

		}
	}
	return rotated;
}

Image mirroring(Image img, int horizontal) {

	int w = img.width, h = img.height;

	(horizontal == 1) ? (w /= 2) : (h /= 2);

	for(int i2 = 0; i2 < h; ++i2) {
		for(int j = 0; j < w; ++j) {
			int x = i2, y = j;

			(horizontal == 1) ? (y = img.width - 1 - j) : (x = img.height - 1 - i2);

			Pixel aux1;
			aux1.red = img.pixel[i2][j].red;
			aux1.green = img.pixel[i2][j].green;
			aux1.blue = img.pixel[i2][j].blue;

			img.pixel[i2][j].red = img.pixel[x][y].red;
			img.pixel[i2][j].green = img.pixel[x][y].green;
			img.pixel[i2][j].blue = img.pixel[x][y].blue;

			img.pixel[x][y].red = aux1.red;
			img.pixel[x][y].green = aux1.green;
			img.pixel[x][y].blue = aux1.blue;
		}
	}
	return img;
}

Image color_inversion(Image img) {
	for(unsigned int i = 0; i < img.height; ++i) {
		for(unsigned int j = 0; j < img.width; ++j) {
			img.pixel[i][j].red = IMAGECOLOR - img.pixel[i][j].red;
			img.pixel[i][j].green = IMAGECOLOR - img.pixel[i][j].green;
			img.pixel[i][j].blue = IMAGECOLOR - img.pixel[i][j].blue;
		}
	}
	return img;
}

Image crop_picture(Image img, int x, int y, int width, int height) {
	Image cropped;

	cropped.width = width;
	cropped.height = height;

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			cropped.pixel[i][j].red = img.pixel[i + y][j + x].red;
			cropped.pixel[i][j].green = img.pixel[i + y][j + x].green;
			cropped.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
		}
	}
	return cropped;
}
