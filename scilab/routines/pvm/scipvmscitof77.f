c Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
c
cccc
c   NAME
c     scipvmscitof77
c   PURPOSE
c     convert scilab complex to F77 ones
c   NOTES
c   HISTORY
c     fleury - Jun 29, 1999: Created.
c     $Log: scipvmscitof77.f,v $
c     Revision 1.1  2001/04/26 07:49:01  scilab
c     Initial revision
c
c     Revision 1.2  1999/07/07 18:11:14  fleury
c     Ajout des routines de conversion de complexes
c
c     Revision 1.1  1999/07/05 15:58:07  fleury
c     Ajout de la transformation de scilab vers f77
c
cccc
      subroutine scipvmscitof77(k)
c     Given a scilab variable, stored in the stack at the position k (in
c     lstk) this function convert this object in a "f77" object. That
c     is, if the object is composed with scilab complex it will be
c     composed then with f77 like complexes.

      INCLUDE '../stack.h'
c
      integer k, ierr
      integer iadr,sadr, il
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      ierr=0

      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))

c      write(*,*) 'scipvmscitof77:', istk(il),istk(il+1),istk(il+2),
c     $     istk(il+3) 
c     scalar type
      if((istk(il).eq.1).AND.(istk(il+3).eq.1)) then
         mn=istk(il+1)*istk(il+2)
c         write(*,*) 'scipvmscitof77:', mn
         call SCI2F77(stk(sadr(il+4)), mn)
c     polynomial type
      elseif((istk(il).eq.2).AND.(istk(il+3).eq.1)) then
         id=il+8
         mn=istk(il+1)*istk(il+2)
c         write(*,*) 'scipvmscitof77:', mn, istk(id+mn)-1
         call SCI2F77(stk(sadr(il+9+mn)), istk(id+mn)-1)
c     sparse scalar matrix
      elseif((istk(il).eq.5).AND.(istk(il+3).eq.1)) then
         nel=istk(il+4)
         m=istk(il+1)
         n=istk(il+2)
         call SCI2F77(stk(sadr(il+5+m+nel)), nel)
c     list type
      elseif(istk(il).eq.15.or.istk(il).eq.16) then
         call listsci2f77(il)
      endif
      return
      end

c      integer function odd(n)
c      n1=n
c     if (2*int(n1/2).ne.n1) n1=n1+1
c      odd=n1
c      return
c      end
