	Interface for UMFPACK and TAUCS snmf and others sparse
	======================================================
		     utilities Toolbox for Scilab
		     ============================
	       the  scispt toolbox (version 1.4)
	       =======================================
	       (scispt stands for scilab sparse tools)

		  The scispt toolbox can be found at
	http://www.iecn.u-nancy.fr/~pincon/scilab/scilab.html

	    This toolbox has been written by Bruno Pincon
	    iecn, Universite Henri Poincare, Nancy, France
		    <Bruno.Pincon@iecn.u-nancy.fr>

                    with some contributions from:
 
          Antonio Manoel Ferreria Frasson <frasson@ele.ufes.br>
          Universidade Federal do Espírito Santo, Brazil. 
                    <frasson@ele.ufes.br>.
 
      (Antonio Manoel Ferreria Frasson have made the first 
       add-on to treat complex linear systems within the
      interface on the UMFPACK solver).   
  
	scispt is a Scilab interface onto the UMFPACK package
      of Tim Davis and onto the snmf (super nodal multi-frontal)
      Cholesky solver of Sivan Toledo plus some sparse utility 
                        scilab macros.

	    Information about UMFPACK can be found at
	       http://www.cise.ufl.edu/research/sparse
	  UMFPACK has been written by  Timothy A. Davis
	Copyright (c) 2003 by Timothy A. Davis, University of
	 Florida, <davis@cise.ufl.edu>.  All Rights Reserved.

	     Information about TAUCS v1.0 can be found at
		 http://www.tau.ac.il/~stoledo/taucs/
      TAUCS  Version  1.0, November 29, 2001. Copyright (c) 2001
      by Sivan Toledo,  Tel-Aviv  Univesity,  stoledo@tau.ac.il.
			 All Rights Reserved.

	       scispt Version 1.4, November, 2007.
	    Copyright (c) 2001-2007 by Bruno Pincon .
            Cecill licence


README CONTENTS :   A/ Introduction
                    B/ Installation
                    C/ File Contents
                    D/ To Do list
                    E/ Notes

A/ Introduction
   ============

This  toolbox  contains a Scilab interface  onto  the UMFPACK v4.x or v5.x  package of Tim
Davis to solve sparse linear systems, says A x = b and also an
interface onto the TAUCS snmf Cholesky solver of Sivan Toledo to do the
same thing with a s.p.d. (symetrix positive definite) matrix A (if your matrix
is s.p.d. then using this last one will be faster). 

First I would thanks Tim Davis and Sivan Toledo to distribute their respective
packages  under a "free software like license" (*).   Previus UMFPACK versions 
(UMFPACK   2.x) by Ian Duff &  Tim Davis,  written  in fortran, have  a  more 
restrictive license  than this  new C version due to Tim Davis. This toolbox 
is distributed under the Cecill licence, see the scispt_License.txt  file.  
Also I have  much appreciated the very  clear UMFPACK  User  Guide,  and 
I find UMFPACK very well  written.

I would also thank Sivan Toledo for his interest in this interface : he provides 
me the very last snmf version (before TAUCS 1.0 was realesed) and corrects very 
quickly a minor problem. We had also nice e-mail exchanges. Thanks Sivan !  

(*) see the UMFPACK_License.txt and TAUCS_License.txt file

UMFPACK seems to be the fastest free sparse solver for non // computer (there is
currently no // UMFPACK version  while SuperLU have  scalar  and // one's).  The
same seems also true for the TAUCS snmf routines (for symmetric positive matrices). 
The toolbox contains also the following utility scilab macros :

 PlotSparse   -> to plot the sparse pattern of a matrix
 ReadHBSparse -> to read an Harwell Boeing sparse matrix file
 condestsp    -> to compute the condition number of a sparse matrix
                 (algorithm from N. Higham & F. Tisseur)
 cond2sp      -> to compute the 2-norm condition number of a s.p.d. matrix
                 (basic algo : K2 = lM/lm with lM and lm the max and min 
                 eigenvalues computed with power and inverse power iterations).

The UMFPACK interface is made of several routines to replace the
scilab 's native ones (which  interface the Sparse  1.3 package) :

 scilab sparse lu stuff  |      new stuff 
 ------------------------+-----------------------------               
   x = A\b               |    x = umfpack(A,"\",b) 
   x = b/A               |    x = umfpack(b,"/",A)
 lup = lufact(A)         |  lup = umf_lufact(A)
   x = lusolve(lup,b)    |    x = umf_lusolve(lup,b) (umf_lusolve offers others things)
       ludel(lup)        |        umf_ludel(lup)
[L,U,P,Q]==luget(lup)    | [L,U,p,q,R] = umf_luget(lup)
                         | [OK,n,lnz,unz,udiag_nz,it] = umf_luinfo(lup)  (no direct equivalent)

 Some differences :

  1/ in umf_luget p and q are permutation vectors and not (sparse) permutation
     matrices as in luget

From the  speed point of  view, UMFPACK is actually very  superior to the Sparse
1.3 package (at least throw is  its scilab interface).  A small bench is exposed
in the file <PATH>/examples/bench.txt (*) (this file may be open with the demo).
A more serious bench (UMFPACK v3 versus SuperLU, UMFPACK 2.2.1 and the sparse lu
matlab stuff) is available at the UMFPACK home page (see after).
  

The TAUCS snmf interface routines replace the  scilab 's native ones (which  
interface the Ng Peyton sparse cholesky V0.3 solver) :

 scilab sparse chol stuff  |      new stuff 
 --------------------------+-----------------------------               
 spcho = chfact(A)         |  Cp = taucs_chfact(A)
     x = chsolve(spcho,b)  |   x = taucs_chsolve(Cp,b)
                           |       taucs_chdel(Cp)
 [C,p] = spchol(A) ?       | [Ct,p] = taucs_chget(Cp)
                           | [OK,n,cnz] = taucs_chinfo(Cp)

 Here the main difference is that chfact use the scilab stack to store 
 the factorization (spcho) while with taucs_chfact the factorization is
 outside : I think that this is more practical. From the speed point of
 view the taucs_snmf doesn't bring the same speed up over chfact than 
 umfpack versus Sparse 1.3. I note a gain about 5. Furthemore this gain
 is mainly due to the fact that chfact uses a scilab macro (sp2adj) which
 is far from optimal. By rewriting sp2adj the gain may be only about 1.5 (and
 it is not clear if this is true if we compile the Ng Peyton stuff with
 another fortran compiler than g77) but :

  - as chfact uses the scilab memory the user may set the stacksize with
    a large value (naturally if the user want to retrieve the Cholesky
    factorization at the scilab level (taucs_chget) the same amount of 
    memory for the scilab stack is needed... but generally this is not useful).

  - the taucs snmf solver has a free software status and it is currently
    in development (so we may hope new versions/corrections, etc..) while
    the status of the Ng peyton is not so clear...
    

(*) HERE AND IN THE FOLLOWING <PATH> STANDS FOR  THE PATH OF THE DIRECTORY 
    CONTAINING THIS README FILE

B/ Installation  for scilab-4.x
   ============
   see the INSTALL file


C/ Files Contents
   ==============

README             : this file
INSTALL            : installation instructions
UMFPACK_License.txt: UMFPACK License
TAUCS_License.txt  : TAUCS License
scispt_License.txt : scispt toolbox license
CHANGES            : changes log file
builder.sce        : scilab script which builds the (very simple)
                     Makefile then compile intscispt.c and taucs_scilab.c then
                     build loader_inc.sce the variable part of loader.sce
                     (loader.sce do an "exec loader_inc.sce")
buildhtml.sce      : scilab script to use if you want to rebuild the html
                     help pages from the xml one's (unnecessary)
Makefile           : generated
loader_inc.sce     : generated
intscispt.c        : C interface file (to umfpack and taucs snmf)
intscispt.o        : generated
taucs_scilab.c     : taucs snmf routines
taucs_scilab.h     : public interface for taucs_scilab.c
taucs_scilab.o     : generated
loader.sce         : installation script (load the interface with addinter,
                     the macros with a simple getf and the help pages)
sparse_util.sci    : file which contains all the scilab 's macro

manxml             : directory for xml and html help pages.

examples           : directory containing the demo stuff
     umfdem*.dem   : files used by the demo function
     arc130.rua    : sparse matrix file (in Harwell Boeing format)
     ex14.rua      : sparse matrix file (in Harwell Boeing format)
     bcsstk24.rsa  : sparse matrix file (in Harwell Boeing format)
     young1c.csa   : sparse matrix file (in Harwell Boeing format)
     bench.txt     : file containing the results of a comparison


D/ To Do List
   ==========

  After bugs corrections, here is a list of possible new features 
  (in no special order) that I (or you) may write :

  (i)    a better automatic installation (at least add an install for win) 
  (ii)   a macro to write an Harwell-Boeing sparse format file
  (iii)  provide replacement of %sp_l_s (A\b),  %s_r_sp (b/A), etc...  which 
         use umfpack in place of Sparse1.3 
  (iv)   add fine tuning in several places by using the Control
         arg of the UMFPACK routines (for instance when iterative
         raffinement is done 2 iterations are processed but we can
         choose what we want, also we can set the tolerance for the
         partial pivot strategy)
  (v)    add the possibility of choosing the column re-ordering
         (by a user choice) and the same for taucs_chfact
  (vi)   some macros to solve ultra classic p.d.e. on the unit square
         (elliptic, parabolic and wave) : to do a beautiful demo ! 

                  
E/ NOTES
   =====
        1) currently the macros are not organised as a scilab lib (loader.sce
           do a simple getf of the file sparse_util.sci which contains all
           the scilab macros).  

			    -------------
			     That 's all