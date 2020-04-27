#Tracerr
##A simple raytracing program

![](./img/cover.png)


###Overview
This program was done as the final project for the Udacity C++ nanodegree.  
The program supports redenring of spheres, triangles and rectangles as basic primitive shapes. It also includes the capability to render .obj 3D models.  
The program is capable of simulating not only the basic phong illumination model (ambient, diffusion and specular light) but also shadows, reflections and refractions  
The code is inspired by [tinytracer](https://github.com/ssloy/tinyraytracer) by Dimitry Sokolov (specially some of the math behind raytracing). The rest of the code was done to practice the stuff that was covered during the nanodegree.    
I also chose to use Eigen for the linear algebra due to its easy to use, speed and built-in methods. 

###How to run
First you will need to install [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) for the linear algebra operations  
`sudo apt-get install libeigen3-dev`

It also uses the [STB library](https://github.com/nothings/stb) to encode the image files. The required files are already included in the repo (_include_ directory)  

Choose the parameters that you desire within the _main.cpp_ file (image size, scene to render, etc...)  
**NOTE**: bear in mind that in a i7 skylake machine it takes around 20s to render the demo scene shown in the cover image above (2000px x 1000px) (400 randomly generated spheres + 3D duck model)

Once you have it installed you can compile with CMake and Make  
**NOTE**: This code uses C++17 features  
`mkdir build  `  
`fstarstcd build  `  
`fstarstcmake ..  `  
`fstarstmake  `  
`fstarst./tracerr`    
The resulting image will be saved in the build directory under the filename that you specify ("_RenderTest.png_" as default)

###Code Overview
The include folder contains the STB library files  
The obj folder contains a sample 3D model to test the triangle mesh rendering  
The src folder contains most of the code  
The tests folder contains the gtest tests (WIP)  

####Classes

#####Render
Main class for the raytracing  
It owns the shapes to be rendered (stored in the heap)  
It casts a ray using an origin point + a direction unit vector  
Then it checks if the ray intersects with anything in the scene  
If no object is hit, the render saves the background color as the value for that pixel  
If an object is hit, the render calculates the reflections and refractions from that object recursively by casting rays from the hit point at the reflected and refracted angles  
To calculate shadows it checks if the hit point has line of sight with the different light sources, if not it is in a shadow  
![](./img/phong.png)  
The final calculation is done by taking into account the [phong illumination model](https://en.wikipedia.org/wiki/Phong_shading) (ambient, diffused and specular) values and reflections and refractions hits  
The raytracing can be run in series, in parallel using [OpenMP](https://en.wikipedia.org/wiki/OpenMP) or in parallel using our own thread pool implementation   
![](./img/render_call_diagram.png)

#####Image
Holds & owns the image bitmap (pixels_) in the heap  
It is in charge of writing the color values to every pixel and to save the image to disk using an instance of ImageFile

#####Light
Represents a point light source with a given intensity
  
#####Material
Represents a given material with certain properties relating to its color, diffusivity, specularity, reflectivity and refractivity

#####Shape
Pure virtual class to define the interface for all other shapes  
Used for dynamic polymorphism for the calculate intersection and calculate normal methods  
![](./img/shape_class_childs.png)          

#####Sphere
Shape child class that represents a sphere centered around a given point, with a certain radius and material  
Since its the child of a pure virtual class it defines the relevant methods to be able to be instantiated

#####Triangle
Shape child class that represents a triangle with three given points and a certain material. Similarly to sphere it implements its own intersection and normal calculation methods

#####Rectangle
Shape child class made out of two triangles. Takes advantage of composition to represent a more complex shape  
![](./img/rect_class.png)  
#####ObjLoader
Reads and parses a .obj file and creates the respective triangles for the render (including material and an optional translation)

#####ImageFile
RAII file handler for writing png image file. Opens the file in the constructor and frees the resource in the destructor

#####ObjFile
RAII file handler for reading the .obj file. Opens the file in the constructor and frees the resource in the destructor

#####ThreadPool
Thread pool design pattern
Implements a queue of work packages for a pool of threads  
Threads take a task from the queue, run it and when they are finished, return the result and check if there are any more tasks in the queue to perform




