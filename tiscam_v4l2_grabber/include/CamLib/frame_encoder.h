/******************************************************************************

   REBVO: RealTime Edge Based Visual Odometry For a Monocular Camera.
   Copyright (C) 2016  Juan José Tarrio
   
   Jose Tarrio, J., & Pedre, S. (2015). Realtime Edge-Based Visual Odometry
   for a Monocular Camera. In Proceedings of the IEEE International Conference
   on Computer Vision (pp. 702-710).
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

 *******************************************************************************/

#ifndef FRAME_ENCODER_H
#define FRAME_ENCODER_H


#include <CamLib/video_io.h>

namespace  rebvo{

enum FrameEncoderType {FRAME_ENCODER_TYPE_RAW=0x00,FRAME_ENCODER_TYPE_MJPEG=0x01,FRAME_ENCODER_TYPE_MFC=0x02};

class FrameEncoder
{

public:
    //FrameEncoder(Size2D frame_size);

    FrameEncoder(){};
    virtual ~FrameEncoder(){};

    virtual int PopFrame(char *stream_buf, int sb_size) = 0;
    virtual int PushFrame(RGB24Pixel *data) = 0;

    virtual int GetEncoderType() = 0;
};

}

#endif // VIDEO_ENCODER_H
