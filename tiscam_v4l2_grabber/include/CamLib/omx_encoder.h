/*
 * omx_encoder.h
 *
 *  Created on: Feb 19, 2018
 *      Author: l_vis
 */

#ifndef INCLUDE_CAMLIB_OMX_ENCODER_H_
#define INCLUDE_CAMLIB_OMX_ENCODER_H_

#include <CamLib/frame_encoder.h>

// Input data defines
#define IMAGE_WIDTH 1280	// JPEG image width
#define IMAGE_HEIGHT 720	// JPEG image height
#define IMAGE_CHANNELS 3	// JPEG image channels (RGB=3channels)

// Internal defines
#define OMX_SKIP64BIT
#define OMX_INIT_STRUCTURE(a) \
    memset(&(a), 0, sizeof(a)); \
    (a).nSize = sizeof(a); \
    (a).nVersion.nVersion = OMX_VERSION; \
    (a).nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
    (a).nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
    (a).nVersion.s.nRevision = OMX_VERSION_REVISION; \
    (a).nVersion.s.nStep = OMX_VERSION_STEP

// Color defines
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_ORANGE "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_CYAN "\033[3;36m"
#define COLOR_LIGHT_GREY "\033[0;37m"
#define COLOR_DARK_GREY "\033[1;30m"
#define COLOR_LIGHT_RED "\033[1;31m"
#define COLOR_LIGHT_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_NC "\033[0m"

// Includes
#include <iostream>
#include <string>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#include <bcm_host.h>
#include <interface/vcos/vcos_semaphore.h>
#include <interface/vmcs_host/vchost.h>
#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <IL/OMX_Video.h>
#include <IL/OMX_Broadcom.h>
#include <sstream>

using namespace std;

struct ApplicationContext{
	OMX_HANDLETYPE encoder;
	OMX_BUFFERHEADERTYPE* encoder_ppBuffer_in;
	OMX_BUFFERHEADERTYPE* encoder_ppBuffer_out;
	int encoder_input_buffer_needed;
	int encoder_output_buffer_available;
	int flushed;
	VCOS_SEMAPHORE_T handler_lock;
};

// Structs

namespace rebvo {

class OmxEncoder: public FrameEncoder {

	int quality;

	Size2D f_size;
	int nChannels;

	std::stringstream jpg_buf;

	ApplicationContext ctx;
	OMX_PARAM_PORTDEFINITIONTYPE encoder_portdef;

	OMX_IMAGE_PARAM_QFACTORTYPE qualityFactor;

	int omxInit();

public:
	OmxEncoder(const Size2D &f_size, const int nChannels, const int quality=50);
	virtual ~OmxEncoder();

    int PopFrame(char *stream_buf, int sb_size);
    int PushFrame(RGB24Pixel *data);

    int GetEncoderType(){return FRAME_ENCODER_TYPE_MJPEG;}
};

} /* namespace rebvo */

#endif /* INCLUDE_CAMLIB_OMX_ENCODER_H_ */
