
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intex2f(fname)
      include 'stack.h'
c  -------------------------------
c
c   Accessing the Scilab Stack :
c   Creation of a Scilab Matrix "C"
c   from a name and an array of data
c
      character*(*) fname
      logical cwritemat
      logical checklhs,checkrhs

      double precision C(3)

      minlhs=1
      maxlhs=1
      minrhs=0
      maxrhs=0

      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      nrows=1
      ncols=3
      C(1)=10.0d0
      C(2)=20.0d0
      C(3)=30.0d0

c  Sending array C to Scilab as variable "C" (size [1,3]) 
      if(.not.cwritemat("C"//char(0), nrows, ncols, C)) return

c  No output
      lhsvar(1) = 0
      return
      end


