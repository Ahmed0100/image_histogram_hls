#include <stdio.h>
#include <hls_opencv.h>
#include <hls_stream.h>

#include <ap_axi_sdata.h>
typedef ap_axiu<8,2,5,6> uint_8_side_channel;

void imgHist(hls::stream<uint_8_side_channel> &inStream, int histo[256]);

#define INPUT_IMAGE_CORE "/home/amustafa/projects_hls/img_historgram/testImage.bmp"
#define FILE_HISTOGRAM "/home/amustafa/projects_hls/img_historgram/hist.txt"

char outImage [320][240];

int histo[256];
int lutMult[256];

void saveHist(const char *filename, int*histPointer)
{
	FILE* pFile;
	pFile=fopen(filename,"w");
	if(pFile != NULL)
	{
		for(int i=0;i<256;i++)
		{
			fprintf(pFile,"Bin[%d]=%d\n",i,histPointer[i]);
		}
	}
}
int main()
{
	printf("Load image %s\n",INPUT_IMAGE_CORE);
	IplImage *src;
	IplImage *img;
	src = cvLoadImage(INPUT_IMAGE_CORE);
	img = cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
	cvCvtColor(src,img,CV_BGR2GRAY);
	unsigned char * data = (unsigned char*)img->imageData;
	//convert to greyscale
	printf("Image rows:%d cols:%d\n",src->height,src->width);
	hls::stream<uint_8_side_channel> inputStream;

	for(int i=0;i<src->width*src->height;i++)
	{

			uint_8_side_channel valIn;
			valIn.data = data[i];
			valIn.keep = 1; valIn.strb=1; valIn.user=1; valIn.id=0;
			valIn.dest = 0;
			inputStream<<valIn;

	}
	imgHist(inputStream,histo);
	saveHist(FILE_HISTOGRAM,histo);
	cvReleaseImage(&src);
	cvReleaseImage(&img);
	return 0;
}
