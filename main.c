#include <stdio.h>
#include <stdlib.h>


typedef struct bitmap_file_header
{
    char biType[2];
    unsigned int biSize;
    int biReserved1n2;
    unsigned int biOffset;
}BMP_FILE_HEADER;

typedef struct DIB_Header
{
    unsigned int biSize;
    long biWidth;
    long biHeight;
    unsigned short int biPlanes;                   
    unsigned short int biBitCount;
    unsigned int biCompression;       
    unsigned int biImageSize;
    long biXResolution;     
    long biYResolution;
    unsigned int biColorUsed;               
    unsigned int biColorImportant;

}BMP_INFO_HEADER;

typedef struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
}RGB;

typedef struct BMP_Image {
    BMP_FILE_HEADER fileHeader;
    BMP_INFO_HEADER informationHeader;
    RGB **pixelArray;
}Image;



void createNewBMPFile(BMP_FILE_HEADER fileHeader, BMP_INFO_HEADER infoHeader, RGB **rgb) {
    FILE* fpnew = fopen("newImage1.bmp","wb");
    if(fpnew==NULL) {
        printf("Failed to create new file.");
        return;
    }
    fwrite(&fileHeader.biType,2,1,fpnew);
    fwrite(&fileHeader.biSize,3*sizeof(BMP_FILE_HEADER),1,fpnew);
    fwrite(&infoHeader,sizeof(BMP_INFO_HEADER),1,fpnew);

    for(int i=0; i<infoHeader.biHeight; i++) {
        fwrite(rgb[i],infoHeader.biWidth*sizeof(struct RGB),1, fpnew);
    }
    printf("File created successfully!");
    fclose(fpnew);
    return;
}


void openBMPImg(char *fileName) {

    FILE *fptr=fopen(fileName,"rb");
    if(fptr==NULL) {
        printf("\nFailed to open file.");
        return;
    }

    BMP_FILE_HEADER fileHeader;
    // **Reading bitmap file fileHeader and displaying the same**
    fread(&fileHeader.biType,2,1,fptr);
    fread(&fileHeader.biSize,3*sizeof(int),1,fptr);

    printf("\nBMP file fileHeader : \nFile type : %c%c\nImage Size : %d\nReserved : %d\nOffset bits : %d\n",
    fileHeader.biType[0],fileHeader.biType[1],fileHeader.biSize,fileHeader.biReserved1n2,fileHeader.biOffset);





    // **Reading bitmap information fileHeader and displaying the same**
    BMP_INFO_HEADER infoHeader;
    printf("%d\n",sizeof(BMP_INFO_HEADER));

    fread(&infoHeader.biSize, sizeof(BMP_INFO_HEADER), 1, fptr);

    printf("\nDIB information fileHeader : \nHeader Size : %d\nWidth : %ld\nHeight : %ld\nColor planes : %d\nBits per pixel : %d\nComperssion : %d\nImage Size: %d\nColors in the palette : %d\nImportant colors : %d\n",
    infoHeader.biSize,infoHeader.biWidth,infoHeader.biHeight,infoHeader.biPlanes,infoHeader.biBitCount,infoHeader.biCompression,infoHeader.biImageSize,infoHeader.biColorUsed,infoHeader.biColorImportant);




    // **Setting the file pointer to starting psition of the file + offsetbits(where the actual array of pixel information of the image starts) and reading it**
    fseek(fptr, fileHeader.biOffset, SEEK_SET);
    RGB **rgb=(struct RGB**) malloc(infoHeader.biHeight*sizeof(void*)); //allocating memory for 2D RGB array to read data of each pixel
    for(int i=0; i < infoHeader.biHeight; i++) {
        rgb[i]=(struct RGB*) malloc(infoHeader.biWidth*sizeof(struct RGB));
        fread(rgb[i],infoHeader.biWidth*sizeof(struct RGB),1,fptr);
    }

    createNewBMPFile(fileHeader,infoHeader,rgb);
    
    fclose(fptr);
    return;
}

int main() {

    openBMPImg("image1.bmp");

    return 0;

}