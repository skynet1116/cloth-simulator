=================================================================
CLOTH SIMULATOR
=================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/cloth-simulator.git
=================================================================

RELEASE REQUIREMENTS: Windows 8+, Visual C++ Redistributable 2012 32-bit
BUILD REQUIREMENTS: Visual Studio 2015, Windows 8+, DirectX SDK

USER KEYS:
-----------------------------------------------------------------
LALT-CLICK:     Rotate the camera
LSHIFT-CLICK:   Move back/forwards with the camera
LCTRL-CLICK:    Move sideways with the camera
BACKSPACE:      Delete currently selected object
WASDQE:         Control the cloth in handle mode
1-4:            Toggle different cloth edge in handle mode

DEVELOPER KEYS
-----------------------------------------------------------------
[ ]:   Change the deltatime when in force time mode
+ -:   Change the amount of smoothing for the cloth
P:     Toggle force delta time mode
T:     Toggle text diagnostics
9:     Toggle wall collision models
8:     Toggle scene/mesh diagnostics
7:     Toggle cloth diagnostics
6:     Toggle octree diagnostics
5:     Toggle collision solver diagnostics
0:     Toggle mesh collision models
        - In collision: black
        - Not in collision: color of partition

DEVELOPMENT UPGRADE IDEAS
-----------------------------------------------------------------
� Switch to DirectX11
� Persistent contact caching for GJK/EPA to prevent jittering
� Switch between different cloth solvers
� Other convex shapes for collision
� Soft shadow mapping