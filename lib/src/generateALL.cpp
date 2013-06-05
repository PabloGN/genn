/*--------------------------------------------------------------------------
   Author: Thomas Nowotny
  
   Institute: Center for Computational Neuroscience and Robotics
              University of Sussex
	      Falmer, Brighton BN1 9QJ, UK 
  
   email to:  T.Nowotny@sussex.ac.uk
  
   initial version: 2010-02-07
  
--------------------------------------------------------------------------*/

/*! \file generateALL.cpp
  \brief Main file combining the code for code generation. 

  The file includes separate files for generating kernels (generateKernels.cc),
  generating the CPU side code for running simulations on either the CPU or GPU (generateRunner.cc) and for CPU-only simulation code (generateCPU.cc).
*/

#include <cstdlib> 

//#define GeNNMODELINCLUDE getenv("GeNNMODELINCLUDE")

#include "global.h"
#include "utils.h"


#include "currentModel.cc"

#include "generateKernels.cc"
#include "generateRunner.cc"
#include "generateCPU.cc"

/*! \brief This function will call the necessary sub-functions to generate the code for simulating a model.

 */

void generate_model_runner(NNmodel &model, //!< Model description

			   string path) //!< Path where the generated code
			               //!< will be deposited
{
  string cmd, name;
  
  #ifdef _WIN32
  cmd= toString("mkdir ")+ path + toString("\\") + model.name + toString("_CODE");
#else
  cmd= toString("mkdir -p ")+ path + toString("/") + model.name + toString("_CODE");
#endif
  cerr << cmd << endl;
  system(cmd.c_str());

  // general/ shared code for GPU and CPU versions
  name= path + toString("/") + model.name + toString("_CODE/runner.cc");
  // cerr << name.c_str() << endl;
  ofstream os(name.c_str());
  genRunner(model, os, cerr);
  os.close();

  // GPU specific code generation
  name= path + toString("/") + model.name + toString("_CODE/runnerGPU.cc");
  os.open(name.c_str());
  genRunnerGPU(model, os, cerr);
  os.close();
  
  name= path + toString("/") + model.name + toString("_CODE/neuronKrnl.cc");
  os.open(name.c_str());
  genNeuronKernel(model, os, cerr);
  os.close();
  
  name= path + toString("/") + model.name + toString("_CODE/synapseKrnl.cc");
  os.open(name.c_str());
  genSynapseKernel(model, os, cerr);
  os.close();

  // CPU specific code generation
  name= path + toString("/") + model.name + toString("_CODE/runnerCPU.cc");
  os.open(name.c_str());
  genRunnerCPU(model, os, cerr);
  os.close();

  name= path + toString("/") + model.name + toString("_CODE/neuronFnct.cc");
  os.open(name.c_str());
  genNeuronFunction(model, os, cerr);
  os.close();
  
  name= path + toString("/") + model.name + toString("_CODE/synapseFnct.cc");
  os.open(name.c_str());
  genSynapseFunction(model, os, cerr);
  os.close();
}

/*! \brief Main entry point for the generateALL executable that generates
  the code for GPU and CPU.

  The main function is the entry point for the code generation engine. It 
  prepares the system and then invokes generate_model_runner to inititate
  the different parts of actual code generation.
*/

int main(int argc, //!< number of arguments; expected to be 2
	 char *argv[] //!< Arguments; expected to contain the name of the  
	              //!< target directory for code generation.
	 )
{
  if (argc != 2) {
    cerr << "usage: generateALL <target dir>" << endl;
    exit(1);
  }
  cerr << "call was ";
  for (int i= 0; i < argc; i++) {
    cerr << argv[i] << " ";
  }
  cerr << endl;

  string path= toString(argv[1]);
  prepare(cerr);
  prepareStandardModels();

  NNmodel model;
  	  
  modelDefinition(model);
  cerr << "model allocated" << endl;
	
  generate_model_runner(model, path);
  
  return 0;
}

