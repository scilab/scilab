1. Introduction
===============

This document describes the Modelica compiler Modelicac.
Modelicac is a tool that compiles a subset of the Modelica 2.0 language (see
section 4). This subset allows the description of continuous-time physical
models that can be simulated under AMESim.


2. How to compile Modelicac
===========================

Be sure to have a recent Objective Caml (v.3.06 or later) properly installed
on the machine.
In the source directory, type:

  make depend

then:

  make (to compile a bytecode version of Modelicac)
or:
  make opt (to compile a native-code version of Modelicac)

Ocaml code HTML documentation can be automatically generated from module types
by typing:

 make doc

This will create a directory named "doc" in the current directory. "index.html"
is the entry point of the documentation.


3. How to use Modelicac
=======================

Modelicac compiles Modelica files whose name ends by ".mo".
The modelicac command, when invoked with the appropriate options, may produce:
- A C file containing a function suitable to be called by AMESim in
  order to perform a model simulation;
- A "*.moc" file which is the format of a precompiled Modelica class stored for
  later instantiation.

It is required that each "*.mo" file contains exactly one Modelica class
(see section 4) and that the name of the class matches the name of the file that
contains its definition.

By default, Modelicac removes every variable that is not reinitialized in a
"when" section and for which it can express its value with respect to the
remaining variables of the system. It is possible to disable this option by
specifying "-keep-all-variables" when calling Modelicac (see below).

Usage
-----

modelicac [-c] [-o <outputfile>] <inputfile> [other options]

-c: Compile only, do not instantiate. Modelicac produces a "*.moc" file when
    invoked with that option.
-o <outputfile>: Set output file name to <outputfile> (this option also works
                 with -c option but is somewhat useless because of the class
                 name restrictions given above).
Other options include:
-L <directory>: Add <directory> to the list of directories to be searched when
                producing a C file (no effect when used with -c).
-hpath <directory>: Specify a path to be added to #include directives in the
                    generated C code.
-keep-all-variables: Do not remove any variable from the initial system.
-jac: Generate analytic jacobian matrix code.
-no-parameter-removal: Do not remove any parameter
-no-simplifs: Same as -keep-all-variables -no-parameter-removal
-xml: Generate an XML version of the model instead of target code
-with-init-in <filename>: Generate code for 'separate initialization' mode
                          (where initialization data is loaded from
                          <filename>)
-with-init-out <filename>: Generate code for 'separate initialization' mode
                           (where initialization data is saved in
                           <filename>)

Examples
-------

+------------------------------------------------------------------------------+
| Modelicac invokation         | Result                                        |
+------------------------------+-----------------------------------------------+
| modelicac foo.mo             | Produces a file named "foo.c" containing a    |
|                              | C function named "foo" to be called by AMESim.|
+------------------------------+-----------------------------------------------+
| modelicac -c foo.mo          | Produces a file named "foo.moc" containing a  |
|                              | precompiled class named "foo".                |
+------------------------------+-----------------------------------------------+
| modelicac -o dir/bar.c       | Same as "modelicac foo.mo", but output file   |
|  foo.mo                      | name is "bar.c" and the resulting file is     |
|                              | located in directory "dir".                   |
+------------------------------+-----------------------------------------------+
| modelicac -L dir1 -L dir2 ...| Same as "modelicac foo.mo", but if some       |
| -L dirN foo.mo               | precompiled class "bar" needed by class "foo" |
|                              | isn't found in the current directory (i.e.    |
|                              | there is no file named "bar.moc" in the       |
|                              | current directory), it is searched into       |
|                              | "dir1", and, if not found, into "dir2", ...,  |
|                              | "dirN" until a file named "bar.moc" is found. |
+------------------------------+-----------------------------------------------+


3. The compiled Modelica subset
===============================

 The Modelicac compiler compiles a subset of the Modelica language that allows
the description of some countinuous equational models. Each Modelica class is
stored in its own file whose name is the name of the class followed by the "mo"
extension.

Restrictions on the declaration of a modelica class header
----------------------------------------------------------
 - only the keyword "class" is allowed to declare a Modelica class ("function"
   is allowed to define functions, but in a very restrictive way, see below);
 - "within" is not allowed ;
 - a class cannot be "final" ;
 - short class definitions (type declarations) are not allowed ;
 - inheritance is not allowed ;
 - "encapsulated" and "partial" classes are not allowed ;

Restrictions on the declaration of the components of a class
------------------------------------------------------------
 - imports are not allowed ;
 - inner classes are not allowed ;
 - "inner", "outer" are not allowed ;
 - "protected" component lists are not allowed ;
 - "final" and "replaceable" are not allowed ;
 - "external" is restricted (see "Restrictions on external function
   definitions") ;
 - "constant" is not allowed ;
 - "input" and "output" may only be used to define I/O ports of the toplevel
   class beeing compiled to C code (see example below) ;
 - "algorithm" sections are not allowed ;
 - arrays must contain numerical types.

Restrictions on modifications
-----------------------------
 - modifications may only apply to base types, scalar or not ;
 - selections of subarrays are not allowed (i.e. a[:].b = ...) ;
 - "redeclare", "each" and "final" are not allowed.

Restrictions on equations
-------------------------
 - equational "if" is not allowed in the specification of an equation.

Restrictions on expressions
-----------------------------
 - "for" expressions must have an integer range (since algorithms are not
   allowed) ;
 - selection of subarrays is restricted to numerical arrays ;
 - array concatenation (using "[" and "]") is not allowed.

Restrictions on external function definitions
---------------------------------------------
 Only functions taking zero or more Integer scalars, String scalars,
Real scalars or Real arrays and returning exactly one
Real scalar are supported.
 External functions must be declared in the Modelica file that
contains models that use them.
The compiler assumes a corresponding C function with the same
name to be provided by the simulation environment. For example:

function Blackbox
  input Real u[:];
  output Real y;
external;
end Blackbox;

 This function can be called from a Modelica model using the following
syntax:

...Blackbox(u)...

 The corresponding C function is declared with the following signature:

double blackbox(double *, int );

(the last argument will be the size of the array whose first element
is pointed to by the first argument, as specified in the Modelica
Language Specification)
