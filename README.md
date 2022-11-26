# create A motion

The application creates a video file based on json scenario file. User defines
frame settings (width, height and width in meters). Movable and statics objects 
can be described by several characteristics. For the movables - start and finish
positions in meters and speed in km/h. Object appearance delay can also be defined.
Static objects can be as lines (solid and dashed) or circles.

For movable objects its route can be written as an array of paths - initial 
and final point and a speed needed for getting the final destination. Several 
paths can generate a non-uniform object motion. Speed sign in each path is 
automatically calculated based on its points.

Examples:

![demo1.json](../demo/crazy_tachki.gif)

![demo2.json](../demo/fish_confuz.gif)

Scenario example:

![json_example.png](../demo/json_example.png)

## Build

Install opencv library 
Build with cmake

## Run

./create_a_motion demo1.json out.avi [optional]

