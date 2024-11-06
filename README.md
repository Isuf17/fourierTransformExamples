# fourierTransformExamples

This project demonstrates Fourier Transform techniques in C++ with OpenCV, specifically applying **high-pass** and **low-pass filters**. It allows you to experiment with frequency-domain filtering for edge detection and image smoothing.

Credit: [OpenCV Fourier Transform Tutorial](https://docs.opencv.org/4.x/d8/d01/tutorial_discrete_fourier_transform.html)

## Features

- **High-Pass Filtering**: Emphasizes edges and fine details in an image.
- **Low-Pass Filtering**: Smooths out fine details, ideal for blurring or noise reduction.

## Requirements

- **C++14** or later
- **CMake 3.10** or later
- **OpenCV 4.x**

Make sure OpenCV and CMake are installed on your system. You can install OpenCV via package managers or by building from source. Here are some common installation commands:

- **Ubuntu**: `sudo apt install cmake libopencv-dev`
- **macOS**: `brew install cmake opencv`
- **Windows**: Follow the [OpenCV installation guide](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html).

## Getting Started

### Clone the Repository

To clone this repository, run:

```bash
git clone https://github.com/Isuf17/fourierTransformExamples.git
cd fourierTransformExamples
```

Create a build directory and cd into it
```bash
mkdir build
cd build
```

Run CMake, complile the project and run the program
```bash
cmake ..
cmake --build .
./main
```

chatGPT 4o helped me with the README!