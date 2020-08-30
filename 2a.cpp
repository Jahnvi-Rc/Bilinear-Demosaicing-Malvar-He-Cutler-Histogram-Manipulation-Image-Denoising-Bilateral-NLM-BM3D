//Linear uniform filtering for image denoising
// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:    The code assumes that the image is of size 256 x 256 and is in the
//          RAW format. You will need to make corresponding changes to
//          accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;
int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int width = 320;
    int height = 320;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            width = atoi(argv[4]);
            height = atoi(argv[5]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[height][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

    unsigned char extend_boundary[height+4][width+4][BytesPerPixel];

    int i,j,k;
    //add a boundary on top
    for(j=1;j<width+2;j++)
    {   for(k=0;k<1;k++)
            extend_boundary[0][j][k] = extend_boundary[2][j][k];  
    }
    //adding a boundary on left
    for(i=0;i<height+2;i++)
    {   for(k=0;k<1;k++)
            extend_boundary[i][0][k] = extend_boundary[i][2][k];
    }
    //adding a boundary below
    for(j=1;j<width+2;j++)
    {   for(k=0;k<1;k++)
            extend_boundary[height+1][j][k] = extend_boundary[height-1][j][k];
    }
    //adding a boundary to the right
    for(i=0;i<height+2;i++)
    {   for(k=0;k<1;k++)
            extend_boundary[i][width+1][k] = extend_boundary[i][width-1][k];
    }
    //get extended image
    for(i = 0;i<height;i++)
    {   for (j=0;j<width;j++)
            extend_boundary[i+1][j+1][1] = Imagedata[i][j][0];
    }
    unsigned char noiseless_img[height][width][BytesPerPixel];
    for(i=0;i<height;i++)
    {   for (int j = 0; j < width; j++)
        {   noiseless_img[i][j][BytesPerPixel] = (extend_boundary[i][j][BytesPerPixel] + extend_boundary[i][j+1][BytesPerPixel] + extend_boundary[i][j+2][BytesPerPixel]+
            extend_boundary[i+1][j][BytesPerPixel]+extend_boundary[i+1][j+1][BytesPerPixel]+ extend_boundary[i+1][j+2][BytesPerPixel]+
            extend_boundary[i+2][j][BytesPerPixel]+extend_boundary[i+2][j+1][BytesPerPixel]+ extend_boundary[i+2][j+2][BytesPerPixel])/9;
        };
    };
    int mse = 0;           
    float psnr;         

    for (i=0;i<height;i++)
    {   for (j=0;j<width;j++)
        {   for (k=0;k<BytesPerPixel;k++)
                mse += pow((noiseless_img[i][j][k]-Imagedata[i][j][k]),2);
        }
    }
    psnr = 10*(log10((255*255)/(mse/((float)(width*height)))));
    cout << "PSNR: " << psnr << endl;
    // Write image data (filename specified by second argument) from ima ge data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(noiseless_img, sizeof(unsigned char), width*height*BytesPerPixel, file);
    fclose(file);

    return 0;
}