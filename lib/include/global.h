/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
	      Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2010-02-07
  
--------------------------------------------------------------------------*/

/*! \mainpage 

GeNN is a software package to enable neuronal network simulations on GPUs.

\section install_sec Installation

Installation of GeNN:

(i) unpack GeNN.zip in a convenient location 

(ii) define the environment variable "GeNNPATH" to point to the main GeNN 
     directory, e.g. if you extracted GeNN to /usr/local/GeNN, then you can add
     "export GeNNPATH=/usr/local/GeNN"
     to your login script.

(iii) define the environment variable "NVIDIASDKPATH" to point to the main 
      directory of an installation of the NVidia Computing SDK (install one
      if necessary). E.g. if the SDK resides in 
      "/usr/local/cuda-5.0/samples" then you can add
      "export NVIDIASDKPATH=/usr/local/cuda-5.0/samples/"
      to your login script.

(iv) add $GeNNPATH/lib/bin to your PATH variable, e.g.
     "export PATH=$PATH:$GeNNPATH/lib/bin"

(v)  modify Makefile examples under $GeNNPATH/lib/src/ and 
     $GeNNPATH/userproject/ 
     (if you use the example) to include the correct SMVERSIONFLAGS of your 
     GPU. 
     Also check the directories under the INCLUDES and LIB variables.

This completes the installation. If you use the example Makefile in 
"userproject", then the HOME, ROOTDIR and include line for Makefile also 
need to be adjusted.

\section how_to How to use GeNN

for a first test the system is best used with a full driver program
like tools/generate_run. To use it, navigate to the "test" directory and type
"../tools/generate_run 1 100 1000 20 100 0.0009 t1".

What this program does is: 
a) use some other tools to generate the appropriate connectivity
matrices and store them in files.

b) build the source code for the model by writing neuron numbers into
userproject/sizes.h, and executing "buildmodel MBody1".  

c) compile the generated code by invoking "make clean && make" d)
running the code, e.g. "linux/release/classol_sim r1 1".

This is already a quite highly integrated example. If one was to use
the library for GPU code generation only, the following would be done:

a) The model in question is defined in a file, say "Model1.cc".  

b) this file needs to 
  (i) define "DT" 
  (ii) include "modelSpec.h" and "modelSpec.cc"
  (iii) contains the model's definition in the form of a function 
  "void modelDefinition(NNmodel &model)" 
  ("MBody1.cc") shows a typical example)

c) The programmer defines his/her own modeling code along similar lines as
"map_classol.*" together with "classol_sim.*". In this code, 

- she defines the connectivity matrices between neuron groups. (In the
example here those are read from files).  

- she defines input patterns (e.g. for Poisson neurons like in the
example) 

- she uses "stepTimeGPU(x, y, z);" to run one time step on the GPU or
"stepTimeCPU(x, y, z);" to run one on the CPU. (both versions are
always compiled). However, mixing the two does not make too much
sense. The host version uses the same memory whereto results from the
GPU version are copied (see next point) 

- she uses functions like "copyStateFromDevice();" etc to obtain
results from GPU calculations.

- the simulation code is then produced in two steps:
"buildmodel Model1" and
"make clean && make"

\section quick_sec Quickstart

In order to get a quick start and run the provided example model, go to 
GeNN/tools

type "make"

type "./generate_run"

This will show you the command line parameters needed.

The tool generate_run will generate connectivity files for the model
MBody1, compiles this model for the GPU and executes it. The command
line parameters are the numbers of neurons in the different neuropils
of the model and an overall synaptic strength. A typical call would
for example be

./generate_run 1 100 1000 20 100 0.0009 test1

which would generate a model, and run it on the GPU, with 100 antennal
lobe neurons, 1000 mushroom body Kenyon cells, 20 lateral horn
interneurons and 100 mushroom body output neurons. All output files 
will be prefixed with "test1".

For more about this example model see 

T. Nowotny, R. Huerta, H. D. I. Abarbanel, and M. I. Rabinovich Self-organization in the olfactory system: One shot odor recognition in insects, Biol Cyber, 93 (6): 436-446 (2005), doi:10.1007/s00422-005-0019-7 
*/


#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include <iostream>
using namespace std; // replaced these two lines : problem with visual studio
#include <fstream>
#include <sstream>
#include <cuda.h>
//#include <cuda_runtime_api.h>
#include <cuda_runtime.h> //EY
//#include <cutil_inline.h> // does not exist anymore in CUDA 5.0
#include <helper_cuda.h>
//#include <helper_cuda_drvapi.h>

cudaDeviceProp *deviceProp;
int devN;
int optimiseBlockSize= 1;

#endif  // _GLOBAL_H_
