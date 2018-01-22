# d3d.to.webgl
I wrote a Direct3D 9 demo way back in 2004 using Microsoft Visual Studio .NET 2002 for Windows XP. The demo featured several advanced HLSL shaders for the time including bump mapping in tandem with a Phong shading/lighting model. In addition, I also added alpha blending to allow translucent objects and made use of the stencil buffer to model a reflective hardwood floor. The demo displays several interesting objects and also allows the camera(s) to be easily repositioned by dragging the mouse around. The various light sources can also be adjusted via a modeless dialog box.

Fast-forward to 2017, I wanted to port this demo to run inside a web browser and at the same time try out several web based technologies including HTML5, CSS, Javascript, and WebGL. The ultimate aim is to get this demo up and running on Windows, MacOSX and Linux without having to build a specific version for each operating system. 

However, before doing this I discovered I could no longer build the original C++ source code on Windows 10. The DirectX 9 SDK has been relegated to the annals of history so it seems so I ported the demo to Direct3D 11 using Microsoft Visual Studio 2015 to allow close study of a running instance of the demo at the source level. Along the way I took advantage of some modern day conveniences like C++11, git LFS and gitHub, which didn't exist back then.

