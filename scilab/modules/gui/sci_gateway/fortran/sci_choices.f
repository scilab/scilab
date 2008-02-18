c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scichoice(fname)
C     ================================================================
c     SCILAB function : x_choices
C     ================================================================ 
      character*(*) fname
cc    implicit undefined (a-z)
      include 'stack.h'
      integer topk,m1,n1,lr1,il1,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,rep
      logical getrvect,getwsmat,checkrhs
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,3,3)) return
      topk=top
C     third argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      if (m3.ne.1.and.n3.ne.1) then 
         buf= fname// ': Third argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C     third argument is a string matrix 
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      top=top-1
c     first argument is an int vector
      if(.not.getrvect(fname,topk,top,m1,n1,lr1))return      
      il1=iadr(lr1)
      call entier(m1*n1,stk(lr1),istk(il1))
      call xchoices(istk(il2),istk(ild2),m2*n2,
     $     istk(il3),istk(ild3),m3*n3, istk(il1),m1*n1,rep)
      if(rep.eq.-1) then 
         buf= fname// ': Error'
         call error(999)
         return
      endif
      if(rep.eq.0) then
c     return an empty matrix when canceled  
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
      else
         call entier2d(m1*n1,stk(lr1),istk(il1))
      endif
      return
      end
C     ================================================================

