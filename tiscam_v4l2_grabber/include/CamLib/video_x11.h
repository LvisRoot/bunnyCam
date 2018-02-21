#ifndef VIDEO_X11_H
#define VIDEO_X11_H

#include <X11/Xlib.h>
namespace  rebvo{

struct XVideoContext{
     Display 	*display;
     int 		defscreen;
     Window 	window;
     XWindowAttributes  windowAttributes;
     XEvent 	event;
     XImage 	*img;
     Pixmap     pixmap;
     GC 		gc;
     __u32 		width;
     __u32 		height;
     __u32		key;
     Atom 		wmDeleteMessage;
};
typedef int DrawFrameFunc(struct XVideoContext *xvc,void *param);

int IniciarVideoOutput(struct XVideoContext *xvc,__u32 width,__u32 height);
int MostrarVideo(struct XVideoContext *xvc,union RGB24Pixel *data,DrawFrameFunc *fc,void *param);
int TerminarVideoOutput(struct XVideoContext *xvc);
__u8 GetKey(struct XVideoContext *xvc);
}
#endif // VIDEO_X11_H
