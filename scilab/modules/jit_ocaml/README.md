# jit_ocaml

A JIT in OCaml for Scilab (part of the Richelieu FUI project)

## Current Status

For now (Dec 15, 2012):

What has been implemented:

- Translation from C++ AST to OCaml AST
- Translation from OCaml AST to C++ AST
- OCaml AST Printer

What has to be implemented: 

- Direct Scilab Parser to OCaml AST
- Partial type inference on OCaml AST
- Pattern matching of classical Scilab errors on OCaml AST
- Pretty-printer of Scilab AST
- JIT Compiler

## How to build

- Checkout a new version of Scilab from your favorite GIT mirror

- Checkout this repository in the modules/ sub-directory of scilab

- This directory contains a patch file scilab-GITCOMMITHASH.diff, that
  should be applied on this particular GITCOMMITHASH version

   - In scilab, use "git checkout GITCOMMITHASH" to go to this particular 
     version

   - Apply the patch "patch -p1 < modules/jit_ocaml/scilab-GITCOMMITHASH.diff

- Now, build and install:

        ./configure --enable-maintainer-mode  --prefix MY_SCILAB_PREFIX
        make
        make install

- You might need some env variables to run:

        PATH=MY_SCILAB_PREFIX/bin:$PATH
        LD_LIBRARY_PATH=MY_SCILAB_PREFIX/lib/scilab:$LD_LIBRARY_PATH
        LD_RUN_PATH=MY_SCILAB_PREFIX/lib/scilab:$LD_RUN_PATH
        export PATH
        export LD_LIBRARY_PATH
        export LD_RUN_PATH

- Now, you can test it:

        scilab

  or

        scilab-cli

## How to modify

This plugin is composed of two parts:
- A dynamic library, containing the C++ code that serialize C++ ASTs
   to strings, and back
- Some static code, included in the 'scilab' and 'scilab-cli' binaries
  This static code contains:
   - the OCaml runtime (libasmrun.a)
   - some C stubs to initialize the OCaml runtime, and register a function
     to perform the analysis on the serialized AST
   - The OCaml code that does the analysis 
