----

# pypp-cli
# Copyright (C) June 2016 Tyler R. Drury, All Rights Reserved.

----

A C++ project is a trivial example of embedding the Python interpreter within a compiled C(++) application.

This project is adapted directly from the Official Python documentation for Visual Studio 2017,
however it can be quite difficult to follow for those new to either Python, C++ or Visual Studio,
or do not know what to do based on the cryptic linker errors during the process and most will quickly get disheartened and give up.

This repository serves as a quick start basis for those who want Python embedded within a C++ application right now
and want a working application without having to trudge through the murky swamps of linker error land alone.

---

## dependencies

* Visual Studio 2017
* Python 3.6

----

Repository developed by Tyler R. Drury,
with updates and language specific features for the MSVC 2017 compiler,
developed with Visual Studios 2017.

----

## Use

This trivial program creates a compiled C++ application which leverages the python interpreter embedded via the Python C API.
This example results in similar execution as if running the python interpreter directly.

    >pypp-cli math.py
    
Where math.py is a script local to the executable's directory,
which will be imported by python and is the same as if importing from within a script

    >import math

The only difference is that the script is embedded within a C++ application,
where the interpreter can be accessed and manipulated via the compiled C++ application,
and scripts can be supplied as json or xml format,
describing collections of individual scripts to import.
    
---

## Starting Project

Begin by opening Visual Studios, then create a new console application.
for this example it will be kept as basic as possible,
so select the option to create an empy project, using precompiled headers
isn't necessarily needed.

First, a quick hack has to be done before starting.
Add the include

```
<Python.h>
```

As Python does not ship with the debug source files,
when built/run in debug mode a linker error will occur.
To solve this open the file Python36\include\pyconfig.h

Then cntrl+F to seach for _DEBUG until this block is encountered

Note, the pound character is missing from the preprocessor directives,
as they are special characters in markdown.

    ifdef MS_COREDLL
    	ifndef Py_BUILD_CORE /* not building the core - must be an ext */
    		if defined(_MSC_VER)
                /* So MSVC users need not specify the .lib file in
                their Makefile (other compilers are generally
                taken care of by distutils.) */
    			if defined(_DEBUG)
    				pragma comment(lib,"python36_d.lib")
    			elif defined(Py_LIMITED_API)
    				pragma comment(lib,"python3.lib")
    			else
    				pragma comment(lib,"python36.lib")
    			endif /* _DEBUG */
    		endif /* _MSC_VER */
    	endif /* Py_BUILD_CORE */
    endif /* MS_COREDLL */

Simply alter the _DEBUG block from

    if defined(_DEBUG)
    	pragma comment(lib,"python36_d.lib")

to this

    if defined(_DEBUG)
    	pragma comment(lib,"python36.lib")

save the file and things will be good to go from here.

Essentially, what this does is supply the production library of python to the linker,
instead of the debug library.
This effectively does nothing, except when built in debug mode,
none of the bulky python debugging code is included,
but since we are not debugging the python interpreter itself,
the extra bloat really isn't needed nor is it provided in the released product.

Next, in the solution explorer window,
go to to Solution Explorer window,
then right click on the project (not the solution),
then select:

    Properties->VC++ Directories->Include Directories->Edit

then navigate to where Python 36 is installed locally (typically C:\Program Files\Python36\),
then add the include directory to the project.

The result should look like this

    C:\Program Files\Python36\include
    
Just remember, this path will be relative to where ever python was installed on the executing machine.
This tells Visual Studio where to look for the source code to be included in the live project.

Note-it may be helpful to set a script environment variable within the project to track what directory Python exists in,
sharing that across all other projects.

After that is complete, in the the Library Directories of the VC++ Directory panel, 
go to edit then add the library path

    C:\Program Files\Python36\libs

This tells Visual Studio where to look for the Python libraries.

After that, while still in the same panel, add another field

    C:\Program Files\Python36
    
Again, this will be relative to where python is installed,
and tells the linker where to search for the python executable.

Save the changes and close the window.

Then right click the solution again

    Add->Existing
    
Do to the directory where Python36 is installed then add

    include/python36.dll

then repeat, adding a new field with

    libs/python36.lib

Finally, the last configuration setting is to ensure the project is built for x64 architecture(64-bit operating systems).
Python 3.6 libraries are built for 64-bit operating systems and a linker warning is omitted if you attempt to use it to build x86(32-bit os) applications.

Now, back in the application main.cpp,
add the desired C++ includes and the following main function.

    int main(int argc, char *argv[])
        if(argc < 1){
            cout << "argc should always be 1 or more arguments, program exiting\n";
            return 0;
        }
        //argv[0] is program name
        //cout << "Welcome to, " << string(argv[0]);
        wchar_t *prog = Py_DecodeLocale(argv[0], NULL);

        if(prog == NULL){
            //err << "Fatal error: can not decode argv[0]";
            exit(-1);
        }
	
        Py_SetProgramName(prog);
        
        Py_Initialize();
        //
        //add custom code here!
        //
        PyMem_RawFree(prog);    //release allocated program
        prog = 0;
        //
        Py_Finalize();  //terminate interpreter session, freeing resources

        system("pause");    //pause system, waiting for user input before termination
    
        return 0;   //success, standard return from main
    }
    
Or just copy this projects application main and modify for personal use.

After being building, the application can be then accessed through the command line by first navigating
to the build directory (either release or debug) in the projects x64 folder.

Before you can execute the program, a copy of python36.dll and all application python scripts,
must be included in all build directories.

This can be done manually by copying and pasting files from the development folder,
however this is tedious and time consuming with large projects.
Using a script to automatically move the required files to the appropriate directory when the project is built,
is a much more convenient, flexible and programmatic approach to this problem.

The second option is typically the best choice.

After ensuring the application can access the python dll and scripts,
the program can be executed using the command:

    >pypp-cli script.py
    
or lists of script names can be included within a single file such as json or xml:

    >pypp-cli scripts.json

with the JSON file taking the form
    
    {"scripts":[
            "scriptOne.py",
            "scriptTwo.py",
            "scriptThree.py"
        ]
    }

or with an XML file

    >pypp-cli scripts.xml
    
in the form
    
    <scripts>
        <script src="scriptOne.py"></script>
        <script src="scriptTwo.py"></script>
        <script src="scriptThree.py"></script>
    </scripts>
    
Providing a comma separated list of multiple markup files and/or python files is also acceptable.

    >pypp-cli testScript.py, scripts0.json, scripts1.xml
    
The C++ application will open each of the local files,
executing each script serially, using PyRun_SimpleString on the source.

However, given the many options,
the most convenient method is simply to import a single script,
which internally imports all other required scripts directory using python,
instead of the C API.

----
### note

This method is trivial, provided as is, for learning purposes,
and by no means intended to be secure,
as the interpreter directly executes the contents of the string supplied,
modification or removal of scripts by untrusted or inexperienced sources
will result in at least the failure of the application or at worse a major security violation of the executing environment.

Thus it is on the onus of all developers NOT to introduce malicious or otherwise buggy code to their applications
and to do their best to prevent others from doing so.

Using embedded python scripts directly within a compiled C++ application is one solution to this problem.

This is one place where using Python embedded within a C++ application really shines.
As compiled programs come with the major security feature of not being modifiable after compilation,
the executing program can be setup to either generate its own script files each time its run,
or simply execute constant strings embedded into the application (typically via the preprocessor) without the need for external files.

This technique is employed by games such a Sid Meier's Civilization franchise,
where the game engine is built on python and the required scripts are generated during execution,
so that if files have been modified to be anything other than what the product was shipped with,
the scripts are automatically replaced with the correct version embedded statically within the compiled application.

Another helpful suggestion is to execute the application within either a virtual development environment,
or from a virtual machine, both of which will contain any potential malicious or unintended bugs to a temporary sandbox environment,
which is isolated from the main host environment.

---

After being thoroughly tested and satisfied with the end result,
the application can then be bundled by a Windows Installer
and distributed as is.    

---

## Contributors
   * Tyler R. Drury
   
---

## Related Projects

Similar projects for various platforms can also be found here


---

## Official Developer Reference

[pypp Reference]()

---