c Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
c
cccc
c   NAME
c     scipvmf77tosci
c   PURPOSE
c     convert F77complex to scilab ones
c   NOTES
c   HISTORY
c     fleury - Jun 29, 1999: Created.
c     $Log: scipvmf77tosci.f,v $
c     Revision 1.1  2001/04/26 07:49:01  scilab
c     Initial revision
c
c     Revision 1.1  1999/07/07 18:11:13  fleury
c     Ajout des routines de conversion de complexes
c
c
cccc
      subroutine scipvmf77tosci(k)
c     Given a scilab variable, stored in the stack at the position k (in
c     lstk) but containing F77 complex instead of internal scilab
c      representation,  this function convert this object into 
c     a scilab object.  That
c     is, if the object is composed with F77 complex it will be
c     composed then scilab complexes.

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

c      write(*,*) 'scipvmf77tosci:', istk(il),istk(il+1),istk(il+2),
c     $     istk(il+3) 
c     scalar type
      if((istk(il).eq.1).AND.(istk(il+3).eq.1)) then
         mn=istk(il+1)*istk(il+2)
c         write(*,*) 'scipvmf77tosci:', mn
         call F772SCI(stk(sadr(il+4)), mn)
c     polynomial type
      elseif((istk(il).eq.2).AND.(istk(il+3).eq.1)) then
         id=il+8
         mn=istk(il+1)*istk(il+2)
c         write(*,*) 'scipvmf77tosci:', mn, istk(id+mn)-1
         call F772SCI(stk(sadr(il+9+mn)), istk(id+mn)-1)
c     sparse scalar matrix
      elseif((istk(il).eq.5).AND.(istk(il+3).eq.1)) then
         nel=istk(il+4)
         m=istk(il+1)
         n=istk(il+2)
         call F772SCI(stk(sadr(il+5+m+nel)), nel)
c     list type
      elseif(istk(il).eq.15.or.istk(il).eq.16) then
         call listf772sci(il)
      endif
      return
      end
