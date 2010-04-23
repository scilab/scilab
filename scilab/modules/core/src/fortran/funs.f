c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine funs(id)
c     ====================================================================
c     scan primitive function and scilab code function lists for a given name
c     ====================================================================
      include 'stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      integer id(nsiz),istr(nlgh)
c
      logical cresmat
      integer srhs,percen,fptr,slhs,r
      integer iadr
      logical toreturn
      data nclas/29/,percen/56/
c
      iadr(l)=l+l-1
c
c     look only in scilab code function libraries
      if(fin.eq.-3) goto 35
      if(fin.eq.-4) goto 30
c
c
      if (comp(1).ne.0) then
c     if  compilation mode skip primitive functions
         fin=0
         fun=0
         return
      endif
c
c     look for name in primitive functions
      call funtab(id,fptr,1,'NULL_NAME',0)
      if(fptr.le.0) then
         if(comp(1).eq.0.and.fin.ne.-5) goto 30
         fin=0
         fun=0
      else
         fun = fptr/1000
         fin = mod(fptr,1000)
      endif
      return
c
c     is a scilab code function already loaded in the variables stack
 30   call sivars(id, toreturn)
      if(toreturn) then
         return
      endif
c
c     look in scilab code function libraries
 35   call siflibs(id, k, istr, lbibn, nbibn, ilp, nn, toreturn)
      if(toreturn) then
         return
      endif
c
c

c
c     load it in the variables stack

c     create a variable with the bin file path
      n=nbibn
c     get name and its length
      call  namstr(id,istr,nn,1)
      top=top+1
      if(.not.cresmat(' ',top,1,1,nbibn+4+nn)) return
      call getsimat(fname,top,top,mp,np,1,1,ilp,nlp)
c     path
      call icopy(nbibn,istk(lbibn),1,istk(ilp),1)
c     name
      call icopy(nn,istr,1,istk(ilp+nbibn),1)
c     extension
      call cvstr(4,istk(ilp+nbibn+nn),'.bin',0)
c     load variables stored in the given file
      srhs=rhs
      slhs=lhs
      fun=0
      rhs=1
      lhs=1
c     just avoid the case where rstk(pt) has been set to 906 by a
c     unterminated call to intload
      r=rstk(pt)
      rstk(pt)=0
      call intload(id,k)
      if(err.gt.0) return
      if (fun.eq.-1) then
         buf='Overloaded load cannot occur in this context'
         call error(999)
         return
      endif
      rstk(pt)=r
      rhs=srhs
      lhs=slhs
      top=top-1
      if(k.eq.0) then
c     .  requested varible not loaded
         fun=0
         fin=0
      else
         il=iadr(lstk(k))
         if(istk(il).ne.11.and.istk(il).ne.13.and.istk(il).ne.130) then
            fin=0
            fun=0
            return
         endif
         fun=-2
         fin=k
      endif

      return
c
      end
