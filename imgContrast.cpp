#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<8,2,5,6> uint8_sc;
typedef hls::stream<uint8_sc> AXI_STREAM;

#define IMG_SIZE 320*240

void imgContrast(AXI_STREAM inStream,
		AXI_STREAM outStream,
		unsigned char xMin,
		unsigned char xMax)
{
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL
#pragma HLS INTERFACE s_axilite port=xMax bundle=CRTL
#pragma HLS INTERFACE s_axilite port=xMin bundle=CRTL
#pragma HLS INTERFACE axis register both port=outStream
#pragma HLS INTERFACE axis register both port=inStream
	float xMax_xMin = xMax-xMin;

	for(int i=0;i<(IMG_SIZE);i++)
	{
#pragma HLS PIPELINE
		uint8_sc currPixelSc = inStream.read();
		uint8_sc dataOutSc;
		unsigned char x_t = currPixelSc.data;
		float y_t_float = ((x_t-xMin)/(xMax_xMin))*255;
		unsigned char y_t =y_t_float;
		dataOutSc.data = y_t;
		dataOutSc.keep = currPixelSc.keep;
		dataOutSc.strb = currPixelSc.strb;
		dataOutSc.dest = currPixelSc.dest;
		dataOutSc.id = currPixelSc.id;
		dataOutSc.last = currPixelSc.last;
		dataOutSc.user = currPixelSc.user;
		outStream.write(dataOutSc);

	}

}
