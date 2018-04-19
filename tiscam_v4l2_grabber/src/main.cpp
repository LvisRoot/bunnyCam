/*
 * main.cpp
 *
 *  Created on: Feb 16, 2018
 *      Author: l_vis
 */


#include <CamLib/v4lcam.h>
#include <CamLib/video_x11.h>
#include <CamLib/image.h>
#include <CamLib/v4lcam.h>
#include <UtilLib/util.h>
#include <CamLib/video_encoder.h>
#include <CamLib/omx_encoder.h>
#include <fstream>
#include <assert.h>
#include <CamLib/mjpeg_encoder.h>
#include <signal.h>
#include <queue>

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}


using namespace rebvo;

const bool debug_time=false;
int main(int argc, char * argv[]){

	signal(SIGINT, intHandler);

	if (argc!= 9){
		std::cout <<"Usage: tiscam_v4l_grabber device width heigth quality fps trigger out_type folder_name\n";
		std::cout <<"out_type: 0=x11 1=ppm(raw) 2=jpg(using OMX) 3=jpg(using gd)\n";
		return -1;
	}

	int out_type=atoi(argv[7]);

	bool use_x11=out_type==0;




    Size2D frameSize{uint(atoi(argv[2])),uint(atoi(argv[3]))};
	v4lCam camara(argv[1],frameSize,atoi(argv[5]),(bool)atoi(argv[6]));

	assert(!camara.Error());

	FrameEncoder *encoder=nullptr;
	const char *file_end=out_type==1?".ppm":".jpg";

	switch (out_type) {
	case 1:
		encoder = new VideoEncoder(frameSize);
		break;
	case 2:
		encoder = new OmxEncoder(frameSize,3,atoi(argv[4]));//frameSize);
		break;
	case 3:
		encoder = new MJPEGEncoder(frameSize,atoi(argv[4]));
		break;
	}

    XVideoContext 	xvc;
	if(use_x11 && IniciarVideoOutput(&xvc,frameSize.w,frameSize.h)){
	            printf("\ntiscam_v4l_grabber: No podder iniciar el Videoooouu\n");
	            return 0;
	        }

	RGB24Pixel *data=nullptr;
	Image<RGB24Pixel> imagen(frameSize);
	double t;

	util::timer tmr;
	util::timer tmr_proc;
	tmr.start();
	int fn=0;

	char image_file_name[1024];
	char *image_buffer=new char[int(1e7)];

	std::queue<double> tStampQ;

	 while (keepRunning) {

		while ((data = camara.GrabBuffer(t, false)) == nullptr) {
			if ( camara.Error()) {
				std::cout << "bye bye cruel world" << std::endl;
				return -1;

			}
		}

		imagen.copyFrom(data);
        camara.ReleaseBuffer();

        if(encoder){
        	tmr_proc.start();

        	while(encoder->PushFrame(imagen.Data())){

        		assert(tStampQ.size()>0);

        		snprintf(image_file_name,1024,"%s/img%lf%s",argv[8],tStampQ.front(),file_end);

        		tStampQ.pop();

        		int	img_jpeg_size = encoder->PopFrame(image_buffer,1e7);


				if(debug_time)
					std::cout <<"Conv Time:"<<tmr_proc.stop()<<"\n";

				tmr_proc.start();
				std::ofstream out_file(image_file_name);
				assert(out_file.is_open());

				out_file.write(image_buffer,img_jpeg_size);
				out_file.close();

				if(debug_time)
					std::cout <<"Save Time:"<<tmr_proc.stop()<<"\n";
        	}
			tStampQ.push(t);
        }
        if(use_x11)
        	MostrarVideo(&xvc,imagen.Data(),nullptr,nullptr);

		if((++fn)>=25){

			double elap=tmr.stop();
			tmr.start();

			std::cout <<"\r FPS: "<<fn/elap;
			fflush(stdout);

			fn=0;

		}
	}

	 std::cout <<"Cam closed";

	return 0;
}
