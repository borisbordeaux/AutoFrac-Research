# AutoFrac-Research

Software for conception and study of fractal structures.  
The main workflow is to produce a Python file, with this software, containing the topologic description of a fractal.
The fractal modeler [MODITERE](https://sourcesup.renater.fr/projects/moditere/) is needed to interpret the Python file and visualize the fractal.

## How to build
 
Replace the `{path_to_Qt6}` with the path to Qt. Be sure to have Qt6 installed.  
Replace `{NB_CORES}` by the number of cores to use to compile the project.

```bash
git clone https://github.com/borisbordeaux/AutoFrac-Research.git
cd AutoFrac-Research
mkdir build
cd build
export QT_PATH={path_to_Qt6}/gcc_64
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j {NB_CORES}
```

For an integration within an IDE, be sure to set the `QT_PATH` environment variable for the project.

## Main features

Main features are separated in several parts, each with its own GUI.
The following content of this file describes the main functionalities implemented on each part. 

### Fractal conception from their boundary

This software allows to conceive fractal faces by describing the fractal behavior of their edges.
See [[1]](#1) for more theoretical information on how it is done.

<img src="img_readme/interface_edit_fractal.png" alt="Interface edition of fractal" width="800px"/>

Eventually, it is possible to force the faces to be constrained in a 2D plane, allowing to edit control points position.
The edition is made in a separate window, in which it is possible to view the subdivision scheme.

<img src="img_readme/interface_edit_ctrl_pts.png" alt="Interface edition of control points" width="800px"/>
<img src="img_readme/interface_subd.png" alt="Interface view of subdivision scheme" width="800px"/>

Finally, the fractal can be exported in a Python file, which is the input of the fractal modeler MODITERE.
Here are some fractals conceived by this software.

<img src="img_readme/exemple_faces.png" alt="Example of faces conceived by this software" width="800px"/>

### Fractal conception from polyhedron circle packings

This software also allows to conceive fractals using polyhedron circle packings [[2]](#2).
Such packings need a polyhedron, which can be imported as an OBJ file.
It is possible to visualize the packing, on the plane or directly on the sphere.

<img src="img_readme/interface_edit_polyhedron.png" alt="Interface edit of polyhedron with circle packing on plane" width="800px"/>
<img src="img_readme/interface_edit_polyhedron_2.png" alt="Interface edit of polyhedron with circle packing on sphere" width="800px"/>

The main idea is to extract the topology of the fractal structure of such packings.
The associated fractal of the polyhedron can be automatically exported in a Python file, which is the input of the fractal modeler MODITERE.

<img src="img_readme/graphical_abstract.png" alt="Workflow of fractal exportation" width="800px"/>

### Measures for fractals 

This software proposes some measures on fractal structures in the form of OBJ files or images.

#### Compute the Fractal Dimension

Compute fractal dimension using the box counting method [[3]](#3) on an image.

<img src="img_readme/interface_fractal_dim.png" alt="Interface fractal dimension" width="700px"/>

#### Compute the Area and Perimeter

Compute the area and the perimeter of an image or an OBJ file.
In this context, the perimeter is the total length of edges separating the interior and the exterior of the structure.

<img src="img_readme/interface_area_perimeter.png" alt="Interface area and perimeter" width="700px"/>

#### Compute the Density

Compute the density of a structure on an image et displays for each pixel its density (normalized or not).

<img src="img_readme/interface_density.png" alt="Interface density" width="700px"/>
<img src="img_readme/density_normalized.png" alt="Image normalized density" width="700px"/>
<img src="img_readme/density_not_normalized.png" alt="Image not normalized density" width="700px"/>

#### Compute the Persistent Homology

Compute the persistent homology of a structure defined in an OBJ file and display the persistent cohomology diagram.

<img src="img_readme/interface_persistent_homology.png" alt="Interface persistent homology" width="700px"/>

## References

<a id="1">[1]</a>
Boris Bordeaux and Christian Gentil.
Automatic construction of fractal structures with locally controlled lacunarity.
Journal of WSCG, 32(1-2):1–12, 2024.

<a id="2">[2]</a>
Iván Rasskin.
Regular polytopes, sphere packings and apollonian sections.
Geometriae Dedicata, 218(6):105, 2024.

<a id="3">[3]</a>
Kenneth Falconer.
Fractal geometry : mathematical foundations and applications.
John Wiley & Sons, 2013.
