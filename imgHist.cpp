#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_axiu<8,2,5,6> uint_8_side_channel;
#define IMG_SIZE 320*240

void imgHist(hls::stream<uint_8_side_channel> &inStream, int histo[256])
{
#pragma HLS INTERFACE axis port=inStream
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE bram port=histo

	int imageSize = 320*240;

	for(int i=0;i<256;i++)
	{
#pragma HLS PIPELINE
		histo[i]=0;
	}
	for(int i=0;i<(IMG_SIZE);i++)
	{
		uint_8_side_channel currPixelSideChannel = inStream.read();
		histo[currPixelSideChannel.data]+=1;

	}
}
