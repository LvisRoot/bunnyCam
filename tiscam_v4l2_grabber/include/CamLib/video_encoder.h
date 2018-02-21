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

#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include "CamLib/frame_encoder.h"
#include <CamLib/video_io.h>

namespace  rebvo{

class VideoEncoder : public FrameEncoder
{

	RGB24Pixel *data;
	Size2D f_size;

public:
    VideoEncoder(Size2D frame_size);

    VideoEncoder();
    ~VideoEncoder(){}

    int PopFrame(char *stream_buf, int sb_size);
    int PushFrame(RGB24Pixel *data);

    int GetEncoderType(){return FRAME_ENCODER_TYPE_RAW;}
};

}

#endif // VIDEO_ENCODER_H
