# lab-env
C++11 based application base for developing apps.

# Requirements
1. CMake 3+.
2. Compiler with support for lambda functions (C++11), such as VS 2013.
  
# Usage
The lab environment contains two classes, Window and App. Override the App class for your project to implement your own update loop. The Window class is used to open a window and connect input handlers to it. 

This is done by assigning a lambda function to a callback matching the input you require. It is possible to have more than one window open at a time, although one must select which window to render to using the function called Window::MakeCurrent prior to doing any GL calls.
  
# Example
The example project is simply an example implementation which displays a colored triangle on a grey background.

# GrafikAlgoritmer
Lab-env-master is the environment we got to use for all our projects, while this is the project I created. it contains all my code and resources to set up a skeleton, add animations to it as well as skin it. The skeleton was created by parsing a XML-file. The animations were created by parsing a file called NAX3 and the skinning was created by parsing a NVX2 file.
