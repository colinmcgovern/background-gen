#include <iostream>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <vector>
#include <math.h>

using namespace std;

double square_dist(double a, double b){
	return pow(a-b,2);
}

const char* INPUT_FILENAME = "input2.bmp";
const char* OUTPUT_FILENAME = "output2.bmp";

const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);

vector<vector<int[3]> > readBMP(const char* filename){

	vector<vector<int[3]> > output;

    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];

    // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f); 

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    cout << width;
    cout << height;

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size];

    // read the rest of the data at once
    fread(data, sizeof(unsigned char), size, f); 
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            // flip the order of every 3 bytes
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;

            
    }

    return data;
}

void generateBitmapImage(unsigned char *image, int height, int width, const char* imageFileName){

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

int main(){

	unsigned char* bmp = readBMP(INPUT_FILENAME);

	for(int i=0;i< 3 * 352 * 264;i+=3){
		cout << " " << (int)bmp[i] << " " << (int)bmp[i+1] << " " << (int)bmp[i+2] << endl;
	}

	// unsigned char* test = malloc(512 * 512);
	// for(int i=0;i<512;i++){
	// 	for(int j=0;j<512;j++){
	// 		test[i+j] = i%256;
	// 	}
	// }

	generateBitmapImage(bmp,264,352,OUTPUT_FILENAME);

    return 0;

	// cv::Mat img = cv::imread("file.jpg");
	// vector<vector<int> > image_vector;
	// for(int i=0;i<img.cols;i++){

	// 	vector<int> new_row;
		
	// 	for(int j=0;j<img.rows;j++){
	// 		new_row.push_back(img.at<int>(i,j));
	// 	}

	// 	image_vector.push_back(new_row);
	// }

	// vector<vector<double> > distances;
	// for(int i=0;i<image_vector.cols;i++){

	// 	vector<int> new_row;

	// 	for(int j=0;j<image_vector.rows;j++){
	// 		new_row.push_back(-1)
	// 	}

	// 	distances.push_back(new_row);
	// }

	// for(int i=0;i<img.cols;i++){
	// 	for(int j=0;j<img.rows;j++){

	// 		double dist_count = 0;

	// 		if(i!=img.cols-2){
	// 			distances[i][j] += square_dist(image_vector[i][j],image_vector[i+1][j]);
	// 			dist_count++;
	// 		}
			
	// 		if(j!=0){
	// 			distances[i][j] += square_dist(image_vector[i][j],image_vector[i-1][j]);
	// 			dist_count++;
	// 		}	

	// 		if(i!=img.rows-2){
	// 			distances[i][j] += square_dist(image_vector[i][j],image_vector[i][j+1]);
	// 			dist_count++;
	// 		}

	// 		if(j!=0){
	// 			distances[i][j] += square_dist(image_vector[i][j],image_vector[i][j-1]);
	// 			dist_count++;
	// 		}	

	// 		distances[i][j] /= dist_count;


	// 	}
	// }




	cout << "done";
}