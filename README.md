# RayTracingInOneWeekend

This is a C implementation of Peter Shirley's "Ray Tracing in One Weekend" book.  

Thanks for Peter Shirley's kindness, the book is now available to the public for free online [here](https://raytracing.github.io/books/RayTracingInOneWeekend.html).  
And Peter Shirley has made his raytracer available on his [GitHub](https://github.com/petershirley/raytracinginoneweekend).

## Build and Run
Run make to build:
```
$ make
```

Run the example usage of `raytracing`:
```
$ make test
```

To run unit tests, give "check" as an argument:
```
$ make check
```

To run OpenMP version to improve raytracer performance, give "OPT=1" as an argument:
```
$ make OPT=1
```
And the output executable file called `raytracing_opt`

To make Gprof reports detailed information, give "PROFILE=1" as an argument:
```
$ make PROFILE=1
```

## How to Use
The arguments of the raytracer are listed below:
```
$ ./raytracing [input TXT file] [output PPM file]
```
For example:
```
$ ./raytracing trace1.txt out1.ppm
```
Run `$ ./raytracing` can also see the list of command-line options

## Input File Formant
The input file contains the following information:
1. The length and width of the image.
2. The number of spheres to be drawn.
3. The location, radius, material, vector of the sphere.

Summarize:
```
[length] [width]
[number of spheres]
[location x] [location y] [location z] [radius] [material] [vector x] [vector y] [vector z]
```

For example:
```
1200 800
5
0 0 -1 0.5 lambertian 0.1 0.2 0.5
0 -100.5 -1 100 lambertian 0.8 0.8 0.0
1 0 -1 0.5 metal 0.8 0.6 0.2
-1 0 -1 0.5 dielectric 1.5
-1 0 -1 -0.45 dielectric 1.5
```

## License
MIT License - See LICENSE file.
