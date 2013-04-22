Interactive raytracer written by Kent Lui

To compile: 'make raytracer'

Output binary is located at bin/raytracer, and can be executed directly.

Most functionalities (ie. rendering a sequence of images that can be converted
to a video file) are not exposed in the GUI. Some may require early git
revisions to utilize. Early revisions also have no dependency on wxWidgets.

Compilation dependencies:
1. wxWidgets 2.8.12 (packages libwxgtk2.8-0 libwxgtk2.8-dbg libwxgtk2.8-dev)
2. lint.sh requires cpplint (pip install cpplint, which requires python pip)
3. g++ 4.6 or above is recommended.
4. icpc (Intel C++ compiler) is highly recommended, although proprietary.
