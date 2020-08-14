//c++ blurry_detect.cpp -lz -lpng16

//Goal: diffuse effect which can take advantage of threading

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>
#include <vector>

using namespace std;

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  if (row_pointers) abort();

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!row_pointers) abort();

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}

void process_png_file() {


	// vector<vector<<int[4]> > image_info;

	// for(int y = 0; y < height; y++){

	// 	vector<int[4]> image_info_row;

	// 	for(int x = 0; x < width; x++) {

	// 		int[4] new_element;
	// 		new_element[0] = px[0];
	// 		new_element[1] = px[1];
	// 		new_element[2] = px[2];
	// 		new_element[3] = px[3];

	// 		image_info_row.push_back(new_element);
	// 	}

	// 	image_info.push_back(image_info_row);
	// }

	// for(int y = 0; y < height; y++){

	// 	png_bytep row = row_pointers[y];

 //    	for(int x = 0; x < width; x++) {

 //    		png_bytep px = &(row[x * 4]);

 //    		px[0] = image_info[y][x][0];
 //    		px[1] = image_info[y][x][1];
 //    		px[2] = image_info[y][x][2];
 //    		px[3] = image_info[y][x][3];
 //    	}
 //    }

  // //Creating the image vector
  //   for(int y = 0; y < height; y++){

  //   png_bytep row = row_pointers[y];

  //   vector<vector<int> > new_row;

  //   for(int x = 0; x < width; x++) {

  //     png_bytep px = &(row[x * 4]);
  //     // Do something awesome for each pixel here...
  //     //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);

  //     px[0] = px[0] / 2;
  //     px[1] = px[1] / 2;
  //     px[2] = px[2] / 2;

  //     cout << (square_dist(px[0],red)) << endl;

  //     if((square_dist(px[0],red)+square_dist(px[1],green)+square_dist(px[2],blue))<distance){
  //       px[0] = px[0] * 2;
  //       px[1] = px[1] * 2;
  //       px[2] = px[2] * 2;
  //     }


  //     px[3] = px[3];

  //   }


  // }
}

int main(int argc, char *argv[]) {
  //if(argc != 4) abort();

  // read_png_file(argv[1]);
  // process_png_file();
  // write_png_file(argv[2]);

  return 0;
}