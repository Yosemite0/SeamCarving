# Seam Carving

This project implements seam carving for content-aware image resizing using C++ and OpenCV. The program dynamically reduces image dimensions by removing vertical and horizontal seams, while preserving important content based on an energy matrix.

## Features

- Resize images by reducing seams based on energy gradients.
- Visualize the process with OpenCV windows that show intermediate results.
- Support for both vertical and horizontal seam removal.

## Directory Structure
```

/project
├── seam_carving.cpp # Main source file
└── README.md

```

### Compilation
You can compile the program via

```bash
g++ <filename>.cpp -o <filename>.out `pkg-config --cflags --libs opencv4`
```

Replace `<filename>` with your source file name (e.g., `seam_carving.cpp`).

## Usage

To run the seam carving algorithm on an image, use the following command:

```bash
./<filename>.out <image_path>
```

After execution, it will prompt for the desired target width and height. The output image will be saved as `out_image.jpg`.

### Example:

```bash
./seam_carving.out input_image.jpg
```

The program will also display intermediate steps for visualizing the seam removal process.

## Dependencies

- OpenCV (version 4.0 or later)
- C++17 or later
- CMake for build configuration

## Installation of OpenCV

To install OpenCV, refer to the following link:
[Install OpenCV in C++](https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/)

Ensure that `pkg-config` is set up to link OpenCV libraries correctly during compilation.

## Output

The resized image is saved as `out_image.jpg` in the current directory.


```