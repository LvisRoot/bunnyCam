# BunnyCam
Bunny-Inspired Stereo Inertial Camera

## Introduction
Most stereo cameras possess a rather large stereo field of view (S-FOV) similar to human vision, so that stereo image processing/mapping algorithms extract as much depth information from the scene as possible. While this is very useful, it could also be expensive in computing terms, given the large amount of data to be process within the S-FOV.

On the other hand, a wide range of monocular algorithms are computationally inexpensive but are unable to estimate scale directly.

This project tries to build a stereo camera inspired by bunny's FOV (Fig. 1), which shows a very narrow S-FOV, thus the name "BunnyCam".

![alt tag](https://github.com/LvisRoot/bunnyCam/tree/master/readme/images/bunnyFov.png)
Fig. 1: Bunny vs. Human field of view (src: vr lense lab)

Monocular algoritms can largely benefit from the device's broad monocular FOV (M-FOV), combined with a direct scale estimation obtained by processing the narrow S-FOV, which in turn is less expensive compared to regular stereo cameras.

Moreover, the camera is also equipped with an IMU so that visual inertia algorithms can be assessed.

The main goal of this project is to develop a flexible platform capable of generating high quality datasets, with synchronized camera and IMU measurements, in order to test novel navigation-oriented algorithms.
