/*
 * omx_encoder.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: l_vis
 */

#include <CamLib/omx_encoder.h>
#include <assert.h>
#include <chrono>

using namespace std;

static const char* dump_compression_format(OMX_IMAGE_CODINGTYPE c) {
    char *f;
    switch(c) {
        case OMX_IMAGE_CodingUnused:       return "not used";
        case OMX_IMAGE_CodingAutoDetect:   return "autodetect";
        case OMX_IMAGE_CodingJPEG:         return "JPEG/JFIF image";
        case OMX_IMAGE_CodingJPEG2K:       return "JPEG 2000 image";
        case OMX_IMAGE_CodingEXIF:         return "EXIF image";
        case OMX_IMAGE_CodingTIFF:         return "TIFF image";
        case OMX_IMAGE_CodingGIF:          return "GIF image";
        case OMX_IMAGE_CodingPNG:          return "PNG image";
        case OMX_IMAGE_CodingLZW:          return "LZW image";
        case OMX_IMAGE_CodingBMP:          return "Windows Bitmap";
        case OMX_IMAGE_CodingMax:          return "Max";

        default:
            f = (char *)calloc(23, sizeof(char));
            if(f == NULL) {
                cerr << "Failed to allocate memory for dumping compression format" << endl;
				exit(-1);
            }
            snprintf(f, 23 * sizeof(char) - 1, "format type 0x%08x", c);
            return f;
    }
}
static const char* dump_color_format(OMX_COLOR_FORMATTYPE c) {
    char *f;
    switch(c) {
        case OMX_COLOR_FormatUnused:                 return "OMX_COLOR_FormatUnused: not used";
        case OMX_COLOR_FormatMonochrome:             return "OMX_COLOR_FormatMonochrome";
        case OMX_COLOR_Format8bitRGB332:             return "OMX_COLOR_Format8bitRGB332";
        case OMX_COLOR_Format12bitRGB444:            return "OMX_COLOR_Format12bitRGB444";
        case OMX_COLOR_Format16bitARGB4444:          return "OMX_COLOR_Format16bitARGB4444";
        case OMX_COLOR_Format16bitARGB1555:          return "OMX_COLOR_Format16bitARGB1555";
        case OMX_COLOR_Format16bitRGB565:            return "OMX_COLOR_Format16bitRGB565";
        case OMX_COLOR_Format16bitBGR565:            return "OMX_COLOR_Format16bitBGR565";
        case OMX_COLOR_Format18bitRGB666:            return "OMX_COLOR_Format18bitRGB666";
        case OMX_COLOR_Format18bitARGB1665:          return "OMX_COLOR_Format18bitARGB1665";
        case OMX_COLOR_Format19bitARGB1666:          return "OMX_COLOR_Format19bitARGB1666";
        case OMX_COLOR_Format24bitRGB888:            return "OMX_COLOR_Format24bitRGB888";
        case OMX_COLOR_Format24bitBGR888:            return "OMX_COLOR_Format24bitBGR888";
        case OMX_COLOR_Format24bitARGB1887:          return "OMX_COLOR_Format24bitARGB1887";
        case OMX_COLOR_Format25bitARGB1888:          return "OMX_COLOR_Format25bitARGB1888";
        case OMX_COLOR_Format32bitBGRA8888:          return "OMX_COLOR_Format32bitBGRA8888";
        case OMX_COLOR_Format32bitARGB8888:          return "OMX_COLOR_Format32bitARGB8888";
        case OMX_COLOR_FormatYUV411Planar:           return "OMX_COLOR_FormatYUV411Planar";
        case OMX_COLOR_FormatYUV411PackedPlanar:     return "OMX_COLOR_FormatYUV411PackedPlanar: Planes fragmented when a frame is split in multiple buffers";
        case OMX_COLOR_FormatYUV420Planar:           return "OMX_COLOR_FormatYUV420Planar: Planar YUV, 4:2:0 (I420)";
        case OMX_COLOR_FormatYUV420PackedPlanar:     return "OMX_COLOR_FormatYUV420PackedPlanar: Planar YUV, 4:2:0 (I420), planes fragmented when a frame is split in multiple buffers";
        case OMX_COLOR_FormatYUV420SemiPlanar:       return "OMX_COLOR_FormatYUV420SemiPlanar, Planar YUV, 4:2:0 (NV12), U and V planes interleaved with first U value";
        case OMX_COLOR_FormatYUV422Planar:           return "OMX_COLOR_FormatYUV422Planar";
        case OMX_COLOR_FormatYUV422PackedPlanar:     return "OMX_COLOR_FormatYUV422PackedPlanar: Planes fragmented when a frame is split in multiple buffers";
        case OMX_COLOR_FormatYUV422SemiPlanar:       return "OMX_COLOR_FormatYUV422SemiPlanar";
        case OMX_COLOR_FormatYCbYCr:                 return "OMX_COLOR_FormatYCbYCr";
        case OMX_COLOR_FormatYCrYCb:                 return "OMX_COLOR_FormatYCrYCb";
        case OMX_COLOR_FormatCbYCrY:                 return "OMX_COLOR_FormatCbYCrY";
        case OMX_COLOR_FormatCrYCbY:                 return "OMX_COLOR_FormatCrYCbY";
        case OMX_COLOR_FormatYUV444Interleaved:      return "OMX_COLOR_FormatYUV444Interleaved";
        case OMX_COLOR_FormatRawBayer8bit:           return "OMX_COLOR_FormatRawBayer8bit";
        case OMX_COLOR_FormatRawBayer10bit:          return "OMX_COLOR_FormatRawBayer10bit";
        case OMX_COLOR_FormatRawBayer8bitcompressed: return "OMX_COLOR_FormatRawBayer8bitcompressed";
        case OMX_COLOR_FormatL2:                     return "OMX_COLOR_FormatL2";
        case OMX_COLOR_FormatL4:                     return "OMX_COLOR_FormatL4";
        case OMX_COLOR_FormatL8:                     return "OMX_COLOR_FormatL8";
        case OMX_COLOR_FormatL16:                    return "OMX_COLOR_FormatL16";
        case OMX_COLOR_FormatL24:                    return "OMX_COLOR_FormatL24";
        case OMX_COLOR_FormatL32:                    return "OMX_COLOR_FormatL32";
        case OMX_COLOR_FormatYUV420PackedSemiPlanar: return "OMX_COLOR_FormatYUV420PackedSemiPlanar: Planar YUV, 4:2:0 (NV12), planes fragmented when a frame is split in multiple buffers, U and V planes interleaved with first U value";
        case OMX_COLOR_FormatYUV422PackedSemiPlanar: return "OMX_COLOR_FormatYUV422PackedSemiPlanar: Planes fragmented when a frame is split in multiple buffers";
        case OMX_COLOR_Format18BitBGR666:            return "OMX_COLOR_Format18BitBGR666";
        case OMX_COLOR_Format24BitARGB6666:          return "OMX_COLOR_Format24BitARGB6666";
        case OMX_COLOR_Format24BitABGR6666:          return "OMX_COLOR_Format24BitABGR6666";
        case OMX_COLOR_Format32bitABGR8888:          return "OMX_COLOR_Format32bitABGR8888";
        case OMX_COLOR_Format8bitPalette:            return "OMX_COLOR_Format8bitPalette";
        case OMX_COLOR_FormatYUVUV128:               return "OMX_COLOR_FormatYUVUV128";
        case OMX_COLOR_FormatRawBayer12bit:          return "OMX_COLOR_FormatRawBayer12bit";
        case OMX_COLOR_FormatBRCMEGL:                return "OMX_COLOR_FormatBRCMEGL";
        case OMX_COLOR_FormatBRCMOpaque:             return "OMX_COLOR_FormatBRCMOpaque";
        case OMX_COLOR_FormatYVU420PackedPlanar:     return "OMX_COLOR_FormatYVU420PackedPlanar";
        case OMX_COLOR_FormatYVU420PackedSemiPlanar: return "OMX_COLOR_FormatYVU420PackedSemiPlanar";
        default:
            f = (char *)calloc(23, sizeof(char));
            if(f == NULL) {
                cerr << "Failed to allocate memory for dumping color format" << endl;
				exit(-1);
            }
            snprintf(f, 23 * sizeof(char) - 1, "format type 0x%08x", c);
            return f;
    }
}

static void omx_die(OMX_ERRORTYPE error, const char* message) {
	const char* e;
	switch(error) {
		case OMX_ErrorNone:                     e = "no error";                                      break;
    	case OMX_ErrorBadParameter:             e = "bad parameter";                                 break;
		case OMX_ErrorIncorrectStateOperation:  e = "invalid state while trying to perform command"; break;
		case OMX_ErrorIncorrectStateTransition: e = "unallowed state transition";                    break;
		case OMX_ErrorInsufficientResources:    e = "insufficient resource";                         break;
		case OMX_ErrorBadPortIndex:             e = "bad port index, i.e. incorrect port";           break;
		case OMX_ErrorHardware:                 e = "hardware error";                                break;
		default:                                e = "(no description)";
    }
	cerr << "OMX ERROR: " << message << " - ";
	fprintf(stderr, "0x%08x", error);
	cerr << ":" << e << endl;
	exit(-1);
}

static void dump_event(OMX_HANDLETYPE hComponent, OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2) {
	const char* e;
	switch(eEvent) {
		case OMX_EventCmdComplete:          e = "command complete";                   break;
		case OMX_EventError:                e = "error";                              break;
		case OMX_EventParamOrConfigChanged: e = "parameter or configuration changed"; break;
		case OMX_EventPortSettingsChanged:  e = "port settings changed";              break;
		default:                            e = "(no description)";
	}
	fprintf(stderr, "Received event 0x%08x - %s, hComponent:0x%08x, nData1:0x%08x, nData2:0x%08x", eEvent, e, (unsigned int)hComponent, nData1, nData2);
	cerr << endl;
}

// OMX calls this handler for all the events it emits
static OMX_ERRORTYPE event_handler(
OMX_HANDLETYPE hComponent,
OMX_PTR pAppData,
OMX_EVENTTYPE eEvent,
OMX_U32 nData1,
OMX_U32 nData2,
OMX_PTR pEventData) {
	dump_event(hComponent, eEvent, nData1, nData2);
	ApplicationContext* ctx = (ApplicationContext*)pAppData;

	switch(eEvent) {
		case OMX_EventCmdComplete:
			vcos_semaphore_wait(&ctx->handler_lock);
			if(nData1 == OMX_CommandFlush) {
				ctx->flushed=1;
			}
			vcos_semaphore_post(&ctx->handler_lock);
			break;
		case OMX_EventError:
			omx_die((OMX_ERRORTYPE)nData1, "Error event received");
			break;
		default:
			break;
	}
	return OMX_ErrorNone;
}

// Called by OMX when the encoder component requires
// the input buffer to be filled with RAW image data
static OMX_ERRORTYPE empty_input_buffer_done_handler(
OMX_HANDLETYPE hComponent,
OMX_PTR pAppData,
OMX_BUFFERHEADERTYPE* pBuffer) {
	ApplicationContext* ctx = (ApplicationContext*)pAppData;
	vcos_semaphore_wait(&ctx->handler_lock);
	// The main loop can now fill the buffer from input
	ctx->encoder_input_buffer_needed=1;
	vcos_semaphore_post(&ctx->handler_lock);

	return OMX_ErrorNone;
}

// Called by OMX when the encoder component has filled
// the output buffer with JPEG data
static OMX_ERRORTYPE fill_output_buffer_done_handler(
OMX_HANDLETYPE hComponent,
OMX_PTR pAppData,
OMX_BUFFERHEADERTYPE* pBuffer) {
	ApplicationContext* ctx = (ApplicationContext*)pAppData;
	vcos_semaphore_wait(&ctx->handler_lock);
	// The main loop can now flush the buffer to output
	ctx->encoder_output_buffer_available=1;
	vcos_semaphore_post(&ctx->handler_lock);

	return OMX_ErrorNone;
}

static void block_until_port_changed(OMX_HANDLETYPE hComponent, OMX_U32 nPortIndex, OMX_BOOL bEnabled) {
	OMX_ERRORTYPE r;
	OMX_PARAM_PORTDEFINITIONTYPE portdef;
	OMX_INIT_STRUCTURE(portdef);
	portdef.nPortIndex=nPortIndex;
	OMX_U32 i=0;
	while(i++ == 0 || portdef.bEnabled != bEnabled) {
		if((r=OMX_GetParameter(hComponent, OMX_IndexParamPortDefinition, &portdef)) != OMX_ErrorNone) {
			omx_die(r, "Failed to get port definition");
		}
		if(portdef.bEnabled != bEnabled) {
			usleep(8000);
		}
	}
}

static void init_component_handle(
const char* name,
OMX_HANDLETYPE* hComponent,
OMX_PTR pAppData,
OMX_CALLBACKTYPE* callbacks) {
	OMX_ERRORTYPE r;
	char fullname[32];

	// Getting the handle
	memset(fullname, 0, sizeof(fullname));
	strcat(fullname, "OMX.broadcom.");
	strncat(fullname, name, strlen(fullname)-1);
	cerr << "Init component: " << fullname << endl;
	if((r=OMX_GetHandle(hComponent, fullname, pAppData, callbacks)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get handle for above component");
	}

	// Disable ports
	OMX_INDEXTYPE types[] = {
		OMX_IndexParamAudioInit,
		OMX_IndexParamVideoInit,
		OMX_IndexParamImageInit,
		OMX_IndexParamOtherInit
	};
	OMX_PORT_PARAM_TYPE ports;
	OMX_INIT_STRUCTURE(ports);
	OMX_GetParameter(*hComponent, OMX_IndexParamImageInit, &ports);

	int i;
	for(i=0; i<4; i++) {
		if(OMX_GetParameter(*hComponent, types[i], &ports) == OMX_ErrorNone) {
			OMX_U32 nPortIndex;
			for(nPortIndex=ports.nStartPortNumber; nPortIndex<ports.nStartPortNumber+ports.nPorts; nPortIndex++) {
				cerr << "Disabling port " << nPortIndex << " for the above component" << endl;
				if((r=OMX_SendCommand(*hComponent, OMX_CommandPortDisable, nPortIndex, NULL)) != OMX_ErrorNone) {
					omx_die(r, "Failed to disable the above port for the above component");
				}
				block_until_port_changed(*hComponent, nPortIndex, OMX_FALSE);
			}
		}
	}
}

static void dump_portdef(OMX_PARAM_PORTDEFINITIONTYPE* portdef) {
	cerr << "Port " << portdef->nPortIndex <<
	" is " << (portdef->eDir ==  OMX_DirInput ? "input" : "output") <<
	", " << (portdef->bEnabled == OMX_TRUE ? "enabled" : "disabled") <<
	", buffers wants: " << portdef->nBufferCountActual <<
	" needs: " << portdef->nBufferCountMin <<
	", size: " << portdef->nBufferSize <<
	", populated: " << portdef->bPopulated <<
	", aligned: " << portdef->nBufferAlignment << endl;

	OMX_IMAGE_PORTDEFINITIONTYPE* imgdef = &portdef->format.image;
	switch(portdef->eDomain) {
		case OMX_PortDomainVideo:
			cerr << "Sorry, this program only designed to dump image formats." << endl;
			exit(-1);
			break;
		case OMX_PortDomainImage:
			cerr << "Image type:" << endl;
			cerr << "\tWidth:\t\t" << imgdef->nFrameWidth << endl;
			cerr << "\tHeight:\t\t" << imgdef->nFrameHeight << endl;
			cerr << "\tStride:\t\t" << imgdef->nStride << endl;
			cerr << "\tSliceHeight:\t" << imgdef->nSliceHeight << endl;
			cerr << "\tError hiding:\t" << (imgdef->bFlagErrorConcealment == OMX_TRUE ? "yes" : "no") << endl;
			cerr << "\tCodec:\t\t" << dump_compression_format(imgdef->eCompressionFormat) << endl;
			cerr << "\tColor:\t\t" << dump_color_format(imgdef->eColorFormat) << endl;
			break;
		default:
			break;
	}
}

static void dump_port(OMX_HANDLETYPE hComponent, OMX_U32 nPortIndex, OMX_BOOL dumpformats) {
	OMX_ERRORTYPE r;
	OMX_PARAM_PORTDEFINITIONTYPE portdef;
	OMX_INIT_STRUCTURE(portdef);
	portdef.nPortIndex=nPortIndex;
	if((r=OMX_GetParameter(hComponent, OMX_IndexParamPortDefinition, &portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for above port");
	}
	cerr << COLOR_GREEN << "********************PORT DEFS********************" << COLOR_NC << endl;
	dump_portdef(&portdef);
	if(dumpformats) {
		cerr << COLOR_GREEN << "********************PORT FMTS********************" << COLOR_NC << endl;
		OMX_IMAGE_PARAM_PORTFORMATTYPE portformat;
		OMX_INIT_STRUCTURE(portformat);
		portformat.nPortIndex=nPortIndex;
		portformat.nIndex=0;
		r=OMX_ErrorNone;
		cerr << "The above port supports the following image formats: " << endl;
		while(r == OMX_ErrorNone) {
			if((r = OMX_GetParameter(hComponent, OMX_IndexParamImagePortFormat, &portformat)) == OMX_ErrorNone) {
				cerr << "\t" << dump_color_format(portformat.eColorFormat) << ", compression: " << dump_compression_format(portformat.eCompressionFormat) << endl;
				portformat.nIndex++;
			}
		}
	}
}

static void block_until_state_changed(OMX_HANDLETYPE hComponent, OMX_STATETYPE wanted_eState) {
	OMX_STATETYPE eState;
	int i=0;
	while(i++ == 0 || eState != wanted_eState) {
		OMX_GetState(hComponent, &eState);
		if(eState != wanted_eState) {
			usleep(8000);
		}
	}
}

int round_up (int value, int divisor){
  return (divisor + value - 1) & ~(divisor - 1);
}


int rebvo::OmxEncoder::omxInit() {

	cerr << COLOR_PURPLE << "**************************************************" << COLOR_NC << endl;
	cerr << COLOR_PURPLE << "*               Pre-initialization               *" << COLOR_NC << endl;
	cerr << COLOR_PURPLE << "**************************************************" << COLOR_NC << endl;
	// Vars
	OMX_ERRORTYPE r;

	// Init bcm_host
	cerr << COLOR_YELLOW << "Init: bcm_host" << COLOR_NC << endl;
	bcm_host_init();

	// Init OMX
	cerr << COLOR_YELLOW << "Init: OMX" << COLOR_NC << endl;
	if((r = OMX_Init()) != OMX_ErrorNone) {
		omx_die(r, "Init. failed");
	}

	// Init ApplicationContext
	memset(&ctx, 0, sizeof(ctx));

	// Create handler lock semaphore
	cerr << COLOR_YELLOW << "Create: Handler lock semaphore" << COLOR_NC << endl;
	if(vcos_semaphore_create(&ctx.handler_lock, "handler_lock", 1) != VCOS_SUCCESS) {
		cerr << "Handler lock semaphore creation failed." << endl;
		exit(-1);
	}

	// Init component handles
	cerr << COLOR_YELLOW << "Init: Component handles" << COLOR_NC << endl;
	OMX_CALLBACKTYPE callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.EventHandler = event_handler;
	callbacks.EmptyBufferDone = empty_input_buffer_done_handler;
	callbacks.FillBufferDone = fill_output_buffer_done_handler;
	init_component_handle("image_encode", &ctx.encoder, &ctx, &callbacks);

	// Dump default encoder configuration
	cerr << COLOR_YELLOW << "Configure: JPEG encoder" << COLOR_NC << endl;
	//cerr << COLOR_CYAN << "Default configured encoder input port 340" << COLOR_NC << endl;
	//dump_port(ctx.encoder, 340, OMX_TRUE);
	//cerr << COLOR_CYAN << "Default configured encoder output port 341" << COLOR_NC << endl;
	//dump_port(ctx.encoder, 341, OMX_TRUE);

	OMX_INIT_STRUCTURE(encoder_portdef);

	cerr << COLOR_YELLOW << "Configure: JPEG encoder input port" << COLOR_NC << endl;
	// Input port definition
	encoder_portdef.nPortIndex=340; // Input port
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder input port 340");
	}

	encoder_portdef.format.image.nFrameWidth=f_size.w;
	encoder_portdef.format.image.nFrameHeight=f_size.h;
	encoder_portdef.format.image.nSliceHeight= f_size.h;//16;//round_up (f_size.w, 16);//16;
	encoder_portdef.format.image.nStride= 0;//round_up (f_size.w, 16);//0;
	encoder_portdef.format.image.bFlagErrorConcealment=OMX_FALSE;
	encoder_portdef.format.image.eColorFormat=OMX_COLOR_Format24bitBGR888;
	encoder_portdef.format.image.eCompressionFormat=OMX_IMAGE_CodingUnused;
	//encoder_portdef.nBufferSize=f_size.w*encoder_portdef.format.image.nSliceHeight*nChannels;
	if((r=OMX_SetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to set port definition for encoder input port 340");
	}
	OMX_INIT_STRUCTURE(encoder_portdef);
	encoder_portdef.nPortIndex=340; // Input port
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder input port 340");
	}

	cerr << COLOR_YELLOW << "Configure: JPEG encoder output port" << COLOR_NC << endl;
	// Output port definition
	OMX_INIT_STRUCTURE(encoder_portdef);
	encoder_portdef.nPortIndex=341; // Output port
	// Very weirded here:
	// I need to copy the default encoder output port(341) and modify based on it in order to work
	// Tried use the copy of the modified encoder input port(340) just above, but i received bad parameter error
	// This is used to work on video_encode, but not on image_encode
	// Maybe some other flags in 340 is/are incompatible with 341?
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder output port 341");
	}

	encoder_portdef.nPortIndex=341; // Output port
	encoder_portdef.format.image.nFrameWidth=f_size.w;
	encoder_portdef.format.image.nFrameHeight=f_size.h;
	encoder_portdef.format.image.bFlagErrorConcealment=OMX_FALSE;
	encoder_portdef.format.image.eColorFormat=OMX_COLOR_FormatYUV420PackedPlanar;
	encoder_portdef.format.image.eCompressionFormat=OMX_IMAGE_CodingJPEG;
	if((r=OMX_SetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to set port definition for encoder output port 341");
	}

	//Quality
	OMX_IMAGE_PARAM_QFACTORTYPE omxQuality;
	OMX_INIT_STRUCTURE (omxQuality);
	omxQuality.nPortIndex = 341;
	omxQuality.nQFactor = quality;

	if((r=OMX_SetParameter(ctx.encoder, OMX_IndexParamQFactor, &omxQuality)) != OMX_ErrorNone) {
		omx_die(r, "Failed to set port definition for encoder output port 341");
	}

	// Switch components to idle state
	cerr << COLOR_YELLOW << "Switching: Encoder component to idle..." << COLOR_NC << endl;
	if((r=OMX_SendCommand(ctx.encoder, OMX_CommandStateSet, OMX_StateIdle, NULL)) != OMX_ErrorNone) {
		omx_die(r, "Failed to switch encoder component state to idle");
	}
	block_until_state_changed(ctx.encoder, OMX_StateIdle);

	// Enabling ports
	cerr << COLOR_YELLOW << "Enabling: Ports..." << COLOR_NC << endl;
	if((r=OMX_SendCommand(ctx.encoder, OMX_CommandPortEnable, 340, NULL)) != OMX_ErrorNone) {
		omx_die(r, "Failed to enable encoder input port 340");
	}
	block_until_port_changed(ctx.encoder, 340, OMX_TRUE);
	if((r=OMX_SendCommand(ctx.encoder, OMX_CommandPortEnable, 341, NULL)) != OMX_ErrorNone) {
		omx_die(r, "Failed to enable encoder input port 341");
	}
	block_until_port_changed(ctx.encoder, 341, OMX_TRUE);

	// Allocating encoder input and output buffers
	cerr << COLOR_YELLOW << "Allocating: Encoder input and output buffers..." << COLOR_NC << endl;
	OMX_INIT_STRUCTURE(encoder_portdef);
	encoder_portdef.nPortIndex=340;
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder input port 340");
	}
	if((r=OMX_AllocateBuffer(ctx.encoder, &ctx.encoder_ppBuffer_in, 340, NULL, encoder_portdef.nBufferSize)) != OMX_ErrorNone) {
		omx_die(r, "Failed to allocate buffer for encoder input port 340");
	}
	OMX_INIT_STRUCTURE(encoder_portdef);
	encoder_portdef.nPortIndex=341;
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder output port 341");
	}
	if((r=OMX_AllocateBuffer(ctx.encoder, &ctx.encoder_ppBuffer_out, 341, NULL, encoder_portdef.nBufferSize)) != OMX_ErrorNone) {
		omx_die(r, "Failed to allocate buffer for encoder output port 341");
	}

	// Switch state of the components before start the encoding porcess in the main loop
	cerr << COLOR_YELLOW << "Switching: Encoder component to executing..." << COLOR_NC << endl;
	if((r=OMX_SendCommand(ctx.encoder, OMX_CommandStateSet, OMX_StateExecuting, NULL)) != OMX_ErrorNone) {
		omx_die(r, "Failed to switch encoder component state to idle");
	}
	block_until_state_changed(ctx.encoder, OMX_StateExecuting);

	// Dump the configured encoder
	cerr << COLOR_CYAN << "Configured encoder input port 340" << COLOR_NC << endl;
	dump_port(ctx.encoder, 340, OMX_TRUE);
	cerr << COLOR_CYAN << "Configured encoder output port 341" << COLOR_NC << endl;
	dump_port(ctx.encoder, 341, OMX_TRUE);

	// Get buffer size
	OMX_INIT_STRUCTURE(encoder_portdef);
	encoder_portdef.nPortIndex=340;
	if((r=OMX_GetParameter(ctx.encoder, OMX_IndexParamPortDefinition, &encoder_portdef)) != OMX_ErrorNone) {
		omx_die(r, "Failed to get port definition for encoder input port 340");
	}

	ctx.encoder_input_buffer_needed=1;
	ctx.encoder_output_buffer_available=0;

	if((r = OMX_FillThisBuffer(ctx.encoder, ctx.encoder_ppBuffer_out)) != OMX_ErrorNone)
			omx_die(r, "Failed to request filling of the output buffer on encoder output port 341");

//	while(!ctx.encoder_input_buffer_needed) usleep(10);
//	ctx.encoder_input_buffer_needed = 0;
//	if((r = OMX_EmptyThisBuffer(ctx.encoder, ctx.encoder_ppBuffer_in)) != OMX_ErrorNone)
//		omx_die(r, "Failed to request emptying of the input buffer on encoder input port 340");


	///////////////////////////////////////////////////////
	// Fin inicialización
	///////////////////////////////////////////////////////
}

namespace rebvo{

OmxEncoder::OmxEncoder(const Size2D &f_size, const int nChannels,
		const int quality) {

	this->f_size = f_size;
	this->nChannels = nChannels;

	this->quality = quality;

	omxInit();
}

OmxEncoder::~OmxEncoder() {
	// TODO Auto-generated destructor stub
}

int OmxEncoder::PopFrame(char* stream_buf, int sb_size) {

	//cerr << COLOR_YELLOW << "Encoding Done!" << COLOR_NC << endl;
	assert(jpg_buf.str().size() <= sb_size);

	memcpy(stream_buf,jpg_buf.str().data(),jpg_buf.str().size());

	return jpg_buf.str().size();

}

int OmxEncoder::PushFrame(RGB24Pixel* data) {

	OMX_ERRORTYPE r;

	//cerr << COLOR_YELLOW << "Starting Encoding..." << COLOR_NC << endl;

	unsigned char* imgMatPtr = (unsigned char*) data;//imgMat.data;
	unsigned int imgMatPtrPos = 0;
	unsigned int imgMatSize = f_size.w*f_size.h*nChannels;

	jpg_buf.str(string());

	assert(encoder_portdef.nBufferSize >= imgMatSize);

	while(true){

		if(ctx.encoder_output_buffer_available) {
			ctx.encoder_output_buffer_available=0;

			//cerr << COLOR_YELLOW << "Writing Jpeg..." << COLOR_NC << endl;

			jpg_buf.write((const char *)(ctx.encoder_ppBuffer_out->pBuffer + ctx.encoder_ppBuffer_out->nOffset),
		  								ctx.encoder_ppBuffer_out->nFilledLen);

			if((ctx.encoder_ppBuffer_out->nFlags&OMX_BUFFERFLAG_ENDOFFRAME)){
				if((r = OMX_FillThisBuffer(ctx.encoder, ctx.encoder_ppBuffer_out)) != OMX_ErrorNone)
					omx_die(r, "Failed to request filling of the output buffer on encoder output port 341");

				return 1;
			}

			if((r = OMX_FillThisBuffer(ctx.encoder, ctx.encoder_ppBuffer_out)) != OMX_ErrorNone)
				omx_die(r, "Failed to request filling of the output buffer on encoder output port 341");
		}

		if(ctx.encoder_input_buffer_needed){
			ctx.encoder_input_buffer_needed = 0;

			for (int y = 0; y < f_size.h; ++y)
				memcpy(&ctx.encoder_ppBuffer_in->pBuffer[y*encoder_portdef.format.image.nStride], &data[y * f_size.w], f_size.w*nChannels);

			ctx.encoder_ppBuffer_in->nOffset = 0;
			ctx.encoder_ppBuffer_in->nFilledLen = encoder_portdef.format.image.nStride * f_size.h;

			if((r = OMX_EmptyThisBuffer(ctx.encoder, ctx.encoder_ppBuffer_in)) != OMX_ErrorNone)
				omx_die(r, "Failed to request emptying of the input buffer on encoder input port 340");

			//cerr << COLOR_YELLOW << "Image Loaded!" << COLOR_NC << endl;

			return 0;
		}

	}
	return 0;
}
} /* namespace rebvo */
