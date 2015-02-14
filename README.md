
C_UTILS
=======
This includes 2 separate, independent libraries.

The first is c_utils which is
made up of c_utils.c/h and basic_types.h.  This includes utilities for reading/writing
to/from files and strings, splitting, trimming, and slicing strings as well as other misc. useful
C functions.

The second is a string library modeled after CVector(It actually started
as a generated vector_char).  It has many more functions than CVector, most taken/modeled
after C++ std::string, including find, replace, substr etc.  It also has it's own functions
for file reading/writing, splitting, trimming, slicing etc.

They are both written in clean C, ie they compile as C++.  The c and cpp files are identical,
they're just there so I can have C++ compile test.

The main difference between the 2 libraries is that c_utils is very general and has many
functions that are not I/O related and while all of it's I/O, and manipulation functions work on
regular strings, they use the c_array type and can be used to read binary data and manipulate
an array of types other than char.  On the other hand, rsw_cstr's are resizable strings like
std::string, guaranteed to be null terminated (actually c_array functions null terminate too
just so that you can use them to, for example read in a string and immediately print it).

A good comparison is the split functions of the 2 libraries.  The c_utils version returns
an array of c_arrays which point into the original string you're splitting.  In other words, they aren't
allocated, they aren't NULL terminated (though it'd be easy to NULL terminate) and you can't
resize them.  The rsw_cstr version returns an array of af cstrs which you can immediately
manipulate individually, erasing, extending, printing, whatever.  Both methods are shown in main.c.


Usage
=====
To actually use the library just copy the appropriate c/h file pair(s) to your project.
To get a general idea of how to use the library and see it in action and how it should
behave, look at the tests in main.c.

Documentation
=============
There is some sparse documentation in the comments but I need to expand it, maybe Doxygenize it.


LICENSE
=======
c_utils is licensed under the MIT License.

Copyright (c) 2013-2015 Robert Winkler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
