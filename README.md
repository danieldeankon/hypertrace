# HyperTrace

Lobachevsky (hyperbolic) space ray tracer powered by OpenCL.

## Requirements

+ OpenCL 1.2
+ SDL2
+ SDL2_Image

## Setup

### Linux

+ Install GCC, Make and CMake.
+ Install some of `*-opencl-dev` and `*-opencl-icd` for your device.

The `*-opencl-dev` and `*-opencl-icd` means that you need to install only a pair of packages which names end with `-opencl-dev` and `-opencl-icd`.

Don't run `apt-get install *-opencl-dev *-opencl-icd`. This command will install all packages with matching names and this is not what you need.

The actual packages depends on your GPU. For example, if you use Nvidia GPU, you probably need to install `nvidia-opencl-dev` and `nvidia-opencl-icd`.

### Windows

+ Install [MSYS2](https://www.msys2.org/).

+ With MSYS2 install required packages:

```bash
pacman -S \
    gcc \
    make \
    cmake \
    git \
    mingw64/mingw-w64-x86_64-opencl-headers \
    mingw64/mingw-w64-x86_64-opencl-icd-git \
    mingw-w64-x86_64-SDL2 \
    mingw-w64-x86_64-SDL2_image
```

## How to use

To build and run example:

```bash
./script/run.sh main
```

You may use name of another example instead of `main` as well.

To select the OpenCL platform and device:

```bash
./script/run.sh <example-name> [platform-no] [device-no]
```

To list all available platforms and devices enter `-1` as `[platform-no]`.

## Control

In some examples you may fly around the scene using your keyboard and mouse.

+ Use your mouse to look around.
+ `W`, `A`, `S`, `D` - move forward, left, backward and right respectively.
+ `Space`, `C` - move up and down.
+ `Q`, `E` - tilt counter- and clockwise.
+ `Tab` - release/grab mouse pointer.
+ `Esc` - exit application.

## TODO

### Basics

- [ ] Euclidean geometry
- [x] Lobachevsky (hyperbolic) geometry
- [ ] Spherical geometry

### Materials

- [x] Specular material 
- [x] Lambertian material
- [x] Transparent material
- [ ] Refraction
- [ ] Diffusion on fog
- [ ] Arbitrary BRDF

### Surface tiling

#### Euclidean plane and horosphere

- [x] Square tiling
- [x] Hexagonal tiling

#### Hyperbolic plane

- [x] Pentagonal tiling
- [ ] Heptagonal tiling
- [ ] Apeirogonal tiling

### Effects

- [x] Lens blur (depth of field)
- [x] Motion blur
- [x] Motion blur on moving objects

### Post-processing

- [x] Gamma correction
- [ ] Noise reduction (maybe using DL)

### Other improvements

- [ ] Same RNG seed for work group to reduce divergence
