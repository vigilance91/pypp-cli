//--------------------------------------------------------------
///
///@file pypp-cli.cpp
///@author Tyler R. Drury
///@copyright June 2018, Tyler R. Drury. All Rights Reserved
///@brief Defines the entry point for the console application.
///@note Due to a bug in the Python Source, the C API can not call or import any code which accesses the thread library. This should hopefully be addressed in future releases of Python.
///
#ifndef PYPP_CLI_MAIN
#define PYPP_CLI_MAIN
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
//#pragma comment(lib, "pypp36.lib")
//#pragma comment(lib, "python36.lib")
//
#include "pypp36.h"

//#include "types\\types.h"
//--------------------------------------------------------------
//python fundemental types
//
typedef py::dictionary dict;
typedef py::list li;
typedef py::set set;
typedef py::tuple tup;
typedef py::number num;
typedef py::floatObject floatObj;
typedef py::longObject longObj;
typedef py::module pyMod;
typedef py::unicode unicdoe;
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
int main(int argc, char *argv[])try
{	//--------------------------------------------------------------
	using py::init;
	using py::finailze;
	using py::_set::programName;
	using namespace py::run;
	using py::eval::get::globals;
	using py::eval::get::locals;
	//typedef py::dictionary dict;
	//namespace runSimple = py::run::simple;	//namespace alias
	//--------------------------------------------------------------

	if(argc < 1){
		cout << "argc should always be 1 or more arguments, program exiting\n";
		return 0;
	}
	//argv[0] is program name
	//cout << "Welcome to, " << string(argv[0]);
	wchar_t *prog = Py_DecodeLocale(argv[0], NULL);	///<decoded program name

	if(prog == NULL){
		//err << "Fatal error: can not decode argv[0]";
		exit(-1);
	}
	
	programName(prog);

	static const std::streamsize SS(1024 * 64);	//limit scripts to 1kb
	static const size_t FS = (size_t)SS;    //max file size

	init();

	string s = "";

	for(unsigned i(1); (int)i < argc; i++){
		const string arg(argv[i]);

		//c_str() returns * to first element(char) in string
		if(arg.size() == 1 && *(arg.c_str()) == '?'){
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

		if(ifs){
			//
			ifs.seekg(0, ifs.end);	//set file pointer to end of file
			const std::streampos length = ifs.tellg();	//get file size
			ifs.seekg(0, ifs.beg);	//reset to begining
			//
			//guarantee file is less than 1kb
			//
			if((size_t)length > FS){
				//file too large, ignore and move on to next
				cout << "error, file size is larger than maximum allowable\n";
				ifs.close();
				continue;
			}
			//
			s.resize((unsigned)length);	//resize buffer to size of file
			//
			ifs.read(&(s[0]), length);  //read in whole file into buffer, not recommended for production but practical for controlled example
			//
			//cout << s;	//output buffer to console for testing
			//
			//execute string as if directly through the interpreter via the command line
			if(!simple::string(s.c_str())){
				cout << "error executing python script";
				break;	//exit loop if anything fails
			}

			ifs.close();
		}
		else{
			//file does not exist or could not be opened, typically because another file hold a lock on it
			cout << "could not open file [" << argv[i] << "]\n";
		}
	}
	//
	//global objects and function created/imported from executed scripts may be access in C++ after this point
	//
	//if(!simple::string("import test")){
	//	cout << "error executing python script";
	//	return(-1);	//exit loop if anything fails
	//}


	//pyMod mod("test");

	dict main(PyImport_AddModule("__main__"));
		//*pyFX = PyImport_ImportModule("makeFX"),

	if(PyErr_Occurred()){
		cout << "error ocurred while loading module";
	}

	if( main.get() /*&& pyFX*/){
		try {
			PyObject *m = main.get();
			//dict
				//g(main.getDict());	//get main module's globals as a dictionary
				//gMod(mod.getDict());
				//parser(globals["parser"]);

			PyObject
				//*pyGlobals = PyModule_GetDict(mod),
				//*key = PyUnicode_FromString("parser"),
				//*parser = PyDict_GetItemString(pyGlobals, "parser"),
				//*nt = globals["nt"];    //PyDict_GetItemString(pyGlobals, "nt");

				*pyInt = main.getAttrString("pyInt"),
				*pyUStr = main.getAttrString("pyUStr"),
				*pyList = main.getAttrString("pyList"),
				//*pyTuple = [1, (8, 5), 3]
				*pyDict = main.getAttrString("pyDict");

			cout << "pyInt: " << longObj(pyInt).asLong() << "\n";
			//cout << "pyUStr: " << unicode(pyUStr);

			cout << "pyList:[\n";

			li l(pyList);

			for (unsigned i(0); i < l.size(); i++) {
				PyObject *e = l[i];
				
				longObj n(e);

				cout << "\t" << n.asLong() << "\n";
			}
			cout << "]\n\n";

			cout << "pyDict:{\n";
			{
				dict d(pyDict);

				PyObject *k = d["key"];

				longObj p(d["prop"]);

				cout << "\t"
					"key:" << ",\n\t"
					"prop:" << p.asLong();
			}
			cout << "\n}\n\n";
			//

			int a = 0;
		}
		catch (...) {
			cout << "error";
		}
		int a = 0;
	}
	else {
		cout << "failed to load module";
	}
	//PyObject *_ptr = nullptr;

	//dict g(globals());	//get globals as a dictionary, as that is how it is stored

	
	//
	//pyNum i(g['pyInt']);	//as arbitrary percision floating point number
	//pyFloat f(g['pyInt']);	//as float
	//pyLong l(g['pyInt']);	//as long
	//pyUnicode uStr(g['pyUStr']));
	//pyList list(g['pyList']));
	//pyTuple tup(g['pyTuple']));
	//pyDict dict(g['pyDict']);
	//
	//python object do not need to be free,
	//as their reference count is held internally by interpreter
	//

	//
	//Build Python values from C++
	//

	//
	//python objects created from application must be decref'ed when finished.
	//
	PyMem_RawFree(prog);    //release allocated program
	prog = 0;
	//
	finailze();  //terminate interpreter session
	//
	system("pause");    //pause system, waiting for user input before termination

	return 0;
}
catch(exception& e){
	cout << e.what();
}
catch(...){
	cout << "unknown exception thrown from int main() caught\n";
}
#endif