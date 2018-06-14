//--------------------------------------------------------------
///
///@file pypp-cli.cpp
///@author Tyler R. Drury
///@date June 2016
///@copyright Tyler R. Drury, All Rights Reserved
///@brief Defines the entry point for the console application.
///@note Due to a bug in the Python Source, the C API can not call or import any code which accesses the thread library. This should hopefully be addressed in future releases of Python.
///
//--------------------------------------------------------------
#ifndef PYPP_CLI_MAIN
#define PYPP_CLI_MAIN
//--------------------------------------------------------------
//generate compiler error if not using C++ to compile
#ifndef __cplusplus
#error invalid compiler, C++ compiler required
#endif
//--------------------------------------------------------------
#include <Python.h>	//python
//--------------------------------------------------------------
//core stl includes for example
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
//--------------------------------------------------------------
//specific stl using statments, for convenience
using std::cout;
using std::cin;
//using std::cerr;
using std::string;
using std::ifstream;
using std::exception;
//--------------------------------------------------------------

///
///@brief parse python object as a C 32 bit signed integer
///
int parseInt(PyObject *obj) {
	int i(0);

	if (!PyArg_ParseTuple(obj, "i", &i)) {
		//exit(-1);
	}
	return i;
}
///
///@brief parse python object as a C 32 bit unsigned integer
///
unsigned parseUInt(PyObject *obj) {
	unsigned i(0);

	if (!PyArg_ParseTuple(obj, "I", &i)) {
		//exit(-1);
	}
	return i;
}
//--------------------------------------------------------------
///
///@brief standard application main accepting arguments on the command-line
///
///@details The compiled executable accepts a comma separated list of local python source files,
///which are to be sequentially executed using an embedded python interpreter,
///@arg argc - system generated value containing the size (number of elements) of argv
///@arg argv - array containing the arguments supplied on the command line
///
///@warning this is only a trivial example program and safety of the scripts being executed is not guaranteed
///
int main(int argc, char *argv[]) try
{
	if (argc < 1) {
		cout << "argc should always be 1 or more arguments, program exiting\n";
		return 0;
	}
	//argv[0] is program name
	//cout << "Welcome to, " << string(argv[0]);
	wchar_t *prog = Py_DecodeLocale(argv[0], NULL);

	if (prog == NULL) {
		//err << "Fatal error: can not decode argv[0]";
		exit(-1);
	}

	Py_SetProgramName(prog);

	static const std::streamsize SS(1024 * 64);	//limit scripts to 1kb
	static const size_t FS = (size_t)SS;    //max file size

	Py_Initialize();

	string s = "";

	for (unsigned i(1); (int)i < argc; i++) {
		const string arg(argv[i]);

		//c_str() returns * to first element(char) in string
		if (arg.size() == 1 && *(arg.c_str()) == '?') {
			//make sure this argument is only a single '?'
			cout << "opening help menu\n";
			cout << "this application is an example of a command line C++ program,\n"
				<< "which embeds the Python interpreter.\n"
				<< "All arguments supplied on the command line are file paths to python scripts local to the exe,\n"
				<< "executing each script\n";

			continue;
		}
		cout << "opening file at location [" << arg << "]\n";

		ifstream ifs(argv[i]);

		if (ifs) {
			//
			ifs.seekg(0, ifs.end);
			const std::streampos length = ifs.tellg();
			ifs.seekg(0, ifs.beg);
			//
			//guarantee file is less than 1kb
			//
			if ((size_t)length <= FS) {
				s.resize((unsigned)length);	//resize buffer to size of file

				ifs.read(&(s[0]), length);  //read in whole file into buffer

											//cout << s;

											//execute string as if directly through the interpreter via the command line
				PyRun_SimpleString(
					s.c_str()
					////inline test string
					//	"from time import time, ctime\n"
					//	"print('Today is', ctime(time()))\n"
				);
				//ensure no errors occured during execution
				PyObject *pyErr = PyErr_Occurred();

				if (pyErr != NULL) {
					//if(PyErr_ExceptionMatches()){
					//do stuff for specific type of exception
					//}
					//err
					PyErr_Clear();	//clears exception state, continueingwith processing scripts
									//break;
									//return;
				}
			}
			else {
				cout << "error, file size is larger than maximum allowable\n";
			}
			ifs.close();
		}
		else {
			cout << "could not open file [" << argv[i] << "]\n";
		}
		//global objects and function created/imported from executed scripts may be access in C++ after this point
	}

	PyMem_RawFree(prog);    //release allocated program
	prog = 0;
	//
	Py_Finalize();  //terminate interpreter session

	//if(Py_FinalizeEx() < 0) {
	//		err << "Fatal error: could not close python interpreter properly";
	//exit(-1);
	//}

	system("pause");    //pause system, waiting for user input before termination

	return 0;
}
catch (exception& e) {
	cout << e.what();
}
catch (...) {
	cout << "unknown exception thrown from int main() caught\n";
}
#endif