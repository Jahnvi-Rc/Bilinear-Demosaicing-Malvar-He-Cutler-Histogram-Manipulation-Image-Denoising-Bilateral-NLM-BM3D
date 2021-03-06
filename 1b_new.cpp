// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:    The code assumes that the image is of size 256 x 256 and is in the
//          RAW format. You will need to make corresponding changes to
//          accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int width = 600;
    int height = 532;

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


    unsigned char MHC_img[height][width][3];
    int a = 2;
    int b = 5;
    int c = 0.5;

    for(i=0;i<height;i++)
    {   for(j=0;j<width;j++)
        {   if(i%2!=0 )
            {   
                if (j%2!=0)
                {
                    MHC_img[i][j][1] = extend_boundary[i+2][j+2][1];
                    MHC_img[i][j][0] = (-extend_boundary[i][j+2][1]-extend_boundary[i+4][j+2][1]-extend_boundary[i+1][j+1][1]-extend_boundary[i+3][j+1][1]-extend_boundary[i+1][j+3][1]-extend_boundary[i+3][j+3][1]+extend_boundary[i+1][j+2][1]*(a*2)+extend_boundary[i+3][j+2][1]*(a*2)+extend_boundary[i+2][j+2][1]*b+extend_boundary[i+2][j][1]*c+extend_boundary[i+2][j+4][1]*c)/8;
                    MHC_img[i][j][2] = (-extend_boundary[i+2][j+4][1]-extend_boundary[i+1][j+1][1]-extend_boundary[i+3][j+1][1]-extend_boundary[i+1][j+3][1]-extend_boundary[i+3][j+3][1]-extend_boundary[i+2][j][1]+extend_boundary[i+2][j+1][1]*(a*2)+extend_boundary[i+2][j+3][1]*(a*2)+extend_boundary[i+2][j+2][1]*b+extend_boundary[i][j+2][1]*c+extend_boundary[i+4][j+2][1]*c)/8;
                }
                else
                {
                    MHC_img[i][j][2] = extend_boundary[i+2][j+2][1];
                    MHC_img[i][j][0] = (extend_boundary[i+1][j+1][1]*a+extend_boundary[i+3][j+1][1]*a+extend_boundary[i+1][j+3][1]*a+extend_boundary[i+3][j+3][1]*a+extend_boundary[i+2][j+2][1]*(a*3)+extend_boundary[i+2][j][1]*(c*3)+extend_boundary[i+2][j+4][1]*(c*3)+extend_boundary[i][j+2][1]*(c*3)+extend_boundary[i+4][j+2][1]*(c*3))/8;
                    MHC_img[i][j][1] = (-extend_boundary[i+4][j+2][1]-extend_boundary[i+2][j+4][1]-extend_boundary[i][j+2][1]-extend_boundary[i+2][j][1]+extend_boundary[i+2][j+1][1]*a+extend_boundary[i+2][j+3][1]*a+extend_boundary[i+1][j+2][1]*a+extend_boundary[i+3][j+2][1]*a+extend_boundary[i+2][j+2][1]*(a*2))/8;
                }
               
            }
            else
            {   if (j%2!=0)
                {
                    MHC_img[i][j][0] = extend_boundary[i+2][j+2][1];
                    MHC_img[i][j][1] = (-extend_boundary[i+4][j+2][1]-extend_boundary[i+2][j][1]-extend_boundary[i+2][j+4][1]-extend_boundary[i][j+2][1]+extend_boundary[i+1][j+2][1]*a+extend_boundary[i+3][j+2][1]*a+extend_boundary[i+2][j+3][1]*a+extend_boundary[i+2][j+1][1]*a+extend_boundary[i+2][j+2][1]*(a*2))/8;
                    MHC_img[i][j][2] = (extend_boundary[i+1][j+1][1]*a+extend_boundary[i+3][j+1][1]*a+extend_boundary[i+1][j+3][1]*a+extend_boundary[i+3][j+3][1]*a+extend_boundary[i+2][j+2][1]*(a*3)-extend_boundary[i+2][j][1]*(c*3)-extend_boundary[i+2][j+4][1]*(c*3)+-extend_boundary[i][j+2][1]*(c*3)-extend_boundary[i+4][j+2][1]*(c*3))/8;
                }
                else
                {
                    MHC_img[i][j][1] = extend_boundary[i+2][j+2][1];
                    MHC_img[i][j][0] = (-extend_boundary[i+2][j+4][1]-extend_boundary[i+1][j+1][1]-extend_boundary[i+3][j+1][1]-extend_boundary[i+1][j+3][1]-extend_boundary[i+3][j+3][1]-extend_boundary[i+2][j][1]+extend_boundary[i+2][j+1][1]*(a*2)+extend_boundary[i+2][j+3][1]*(a*2)+extend_boundary[i+2][j+2][1]*b+extend_boundary[i][j+2][1]*c+extend_boundary[i+4][j+2][1]*c)/8;
                    MHC_img[i][j][2] = (-extend_boundary[i][j+2][1]-extend_boundary[i+4][j+2][1]-extend_boundary[i+1][j+1][1]-extend_boundary[i+3][j+1][1]-extend_boundary[i+1][j+3][1]-extend_boundary[i+3][j+3][1]+extend_boundary[i+1][j+2][1]*(a*2)+extend_boundary[i+3][j+2][1]*(a*2)+extend_boundary[i+2][j+2][1]*b+extend_boundary[i+2][j][1]*c+extend_boundary[i+2][j+4][1]*c)/8;
                }
            }
        };
    };

    // Write image data (filename specified by second argument) from ima ge data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(MHC_img, sizeof(unsigned char), width*height*3, file);
    fclose(file);

    return 0;
}