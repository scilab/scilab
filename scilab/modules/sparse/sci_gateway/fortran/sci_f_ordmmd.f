c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c SCILAB function ordmmd
      subroutine intordmmd(id)
c**********************************************************
      logical getrhsvar, createvar, putlhsvar
      include 'stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************
       minrhs=3
       maxrhs=3
       minlhs=3
       maxlhs=3
c
       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar(1,'i',m1,n1,l1)) return
       if(.not.getrhsvar(2,'i',m2,n2,l2)) return
       if(.not.getrhsvar(3,'i',m3,n3,l3)) return
c*******************************************************
c      1.1-Verify the input.
c*******************************************************
       if(istk(l3).ne.m1-1) then
        buf = ' The provided "n" does not correspond to the matrix'//
     + ' defined by xadj and iadj'
        call error(9999)
        return
       endif
c*****************************************************
c      2-If necessary, create additional variables
c          (working arrays, default values, ...)
c*****************************************************

       k=maxrhs+1
       if(.not.createvar(k,  'i',istk(l3),1,l4)) return
       if(.not.createvar(k+1,'i',istk(l3),1,l5)) return
       if(.not.createvar(k+2,'i',1,1,l6)) return
       if(.not.createvar(k+3,'i',4*istk(l3),1,l7)) return
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call ordmmd(istk(l3),istk(l1),istk(l2),istk(l5),
     $ istk(l4),4*istk(l3),istk(l7),istk(l6),iflag)

c******************************************************
c      4-Display error message(s)
c******************************************************
       if(iflag .NE. 0) then
        buf = ' insufficient working storage'
        call error(9999)
        return
       endif
c
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar(1)=4
       lhsvar(2)=5
       lhsvar(3)=6
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c
       return
       end
c			======================================
