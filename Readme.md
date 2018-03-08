# CSE 389 Museum project

## Update: March 7th, 2018
We are currently working still working on the model loader in the
[`model-loader-dev` branch](https://github.com/DragonAxe/Ekros/tree/model-loader-dev)

We are using the `assimp` library for loading models, then converting from the assimp
scene structure to two arrays, one for vertices and one for indices. The loading of
the model from the obj file works, converting the assimp scene into the two arrays works,
but there are problems with loading the model into the GPU and rendering it correctly.

What we have been able to figure out so far is that rendering raw vertices as triangles
using the `glDrawArrays` command works, but rendering the vertices using indices
via the `glDrawElements` command doesn't work and we haven't had time to investigate
further.

We have also finished texturing the surrounding objects in our museum set up.
Below are some screen shots, and are not the final products. They will be touched up and tweaked over time.
![Boxes](MuseumModels/boxy.png)
![Pyramid](MuseumModels/Pyramid.png)
![Torus knot](MuseumModels/Torus_knot.png)
![Fence](MuseumModels/fency.png)
![pointy mountain](MuseumModels/pointymount.png)

## Update: March 5th, 2018
We have moved all the code pertaining to loading and rendering a model into a
separate class, that way it will be easy to load and manage multiple objects
all at once.

The next step is to actually implement the model loader, rather than just
rendering a couple of triangles.

## Update: February 14th, 2018

The museum blender model can be found here: [MuseumModels/museum.blend](MuseumModels/museum.blend)

![Museum Screenshot](MuseumModels/MuseumRender4.png)
![Museum Screenshot](MuseumModels/MuseumRender3.png)
![Museum Screenshot](MuseumModels/MuseumRender2.png)
![Museum Screenshot](MuseumModels/MuseumRender1.png)

## Update: February 2nd, 2018

> Where are the screenshots?

![Triangle Screenshot](TriangleScreenshot.png)
![Triangle Screenshot](TriangleWireScreenshot.png)

> a. What is the theme of your museum?

  The theme of our museum is reflecting on the tragedy of our decisions. There will be displays of anguish, pain and sleeplessness from our peers in their natural habitat, Cramer.

> b. How many rooms will your museum have?

  Our museum will have, tentatively, 4 rooms plus the outside "starting point" for the museum.

> c. Have you started looking for textures for your museum walls/floors? If so, what are your plans?

  We have looked at some nice marble textures and have plans to possibly make our own textures using Crazybump.(http://crazybump.com/)

> d. Will your museum have a ceiling?  

  Yes. And also, probably, a sky box.

> e. You must have at least ONE “sculpture” in your museum.  What are you planning to use for
    your sculpture(s)?  NOTE: You DO NOT have to create your own blender model/sculpture, you
    are free to search the internet for a pre-made model. It is more important, for me, that you
    understand how these models are represented in the computer, and how they are manipulated in
    3D space.

 We plan on using a 3d scanning software to scan in our peers as statues. There are a few nice scanning softwares with free versions. (we have not decided on which to use yet. some of the models are very low quality. Limited to 5000 polygons.)

> f. You must have at least FOUR “paintings” in your museum.  What were you planning to use for
    your pictures? Have you already started looking for textures that can be applied as “pictures”?  
   Will your pictures be hung on the wall? Will they be murals?  Do you have a completely different
    idea of how these paintings will be added to your museum

 We plan on doing a a few different "paintings" in the museum. One will only be able to be seen properly from a certain angle, one will be a normal picture, the other will be an inserted statue to look like a painting, the final one will be a top down view of a topographical map.

## License
> The MIT License (MIT)
>
> Copyright (c) 2015 Kevin Fung
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
