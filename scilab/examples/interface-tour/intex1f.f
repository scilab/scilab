      subroutine intex1f(fname)
      include 'stack.h'
c ------------------------------------
c     interface for [x,y,z,t]=ex1f('chain',a,b,c);

      character*(*) fname
      logical getrhsvar, createvar
      logical checklhs,checkrhs

c     ex1f has 4 input and at most 5 outputs:
       minrhs=4
       maxrhs=4
       minlhs=1
       maxlhs=5
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c
c  1-Get the 4 input variables of ex1f:
c
c  Variable #1 is a chain ('c') , getrhsvar returns its
c  dimensions: m1 = # of rows = length of the chain, 
c  n1 = # of columns = 1 default value for chains.
c  The chain is chain=cstk(l1:l1+m1*n1)
       if(.not.getrhsvar(1,'c',m1,n1,l1)) return

c  a=variable #2 is an integer ('i') matrix, getrhsvar returns its
c  dimensions: m2 = # of rows , n2 = # of columns
c  values in matrix a are istk(l2), istk(l2+1), ... (columnwise)
       if(.not.getrhsvar(2,'i',m2,n2,l2)) return

c  b=variable #3 is an real ('r') matrix, getrhsvar returns its
c  dimensions: m3 = # of rows , n3 = # of columns
c  values in matrix a are sstk(l2), sstk(l2+1), ... (columnwise)
       if(.not.getrhsvar(3,'r',m3,n3,l3)) return

c  c=variable #4 is an double ('d') matrix, getrhsvar returns its
c  dimensions: m4 = # of rows , n4 = # of columns
c  values in matrix a are stk(l2), stk(l2+1), ... (columnwise)
       if(.not.getrhsvar(4,'d',m4,n4,l4)) return
c
c  2-Create  Scilab variable #5  as double matrix of
c  dimensions  m4 x n4. The first fourth parameters of
c  Createvar are input parameters. Createvar returns the adress l5
c  as output. 
c  The values of stk(l5), stk(l5+1), ... should be set
c  to desired values.
       if(.not.createvar(5,'d',m4,n4,l5)) return
c    Creating now variable #6
       if(.not.createvar(6,'d',m4,n4,l6)) return
c
       call ex1f(cstk(l1:l1+m1*n1),istk(l2),n2*m2,sstk(l3),n3*m3,
     $      stk(l4),m4,n4,stk(l5),stk(l6),ierr)
       if(ierr .gt. 0) then 
        buf = 'Internal Error in' 
        call error(998)
        return
       endif
c
c   Variables #5, #4, #3, #2, #1 are returned in that
c   order. For instance, w=ex1f('chain',a,b,c) puts
c   in w the variable #5 and [w,r]=ex1f('chain',a,b,c)
c   puts in w the variable #5 and in r the variable #4

       lhsvar(1)=5
       lhsvar(2)=4
       lhsvar(3)=3
       lhsvar(4)=2
       lhsvar(5)=1
c
       end

      subroutine ex1f(ch,a,ia,b,ib,c,mc,nc,d,w,err)
c     -----------------------------------------
c     -----------   EXAMPLE   -----------------
c     inputs:  ch, a,b and c; ia,ib and mc,nc
c     ch=character, a=integer, b=real and c=double 
c     ia,ib and [mc,nc] are the dimensions of a,b and c resp.
c     outputs: a,b,c,d
c     if ch='mul'   a,b and c = 2 * (a,b and c) 
c     and d of same dimensions as c with
c     d(i,j)=(i+j)*c(i,j)
c     if ch='add' a,b and c = 2 + (a,b and c)
c     d(i,j)=(i+j)+c(i,j)
c     w is a working array of size [mc,nc]
c     -------------------------------------------
      character*(*) ch
      integer a(*),err
      real b(*)
      double precision c(mc,*),d(mc,*),w(mc,*)
      err=0
      if(ch(1:3).eq.'mul') then
         do 1 k=1,ia
 1           a(k)=2*a(k)
         do 2 k=1,ib
 2           b(k)=2.0*b(k)
         do 3 i=1,mc
         do 3 j=1,nc
 3           c(i,j)=2.0d0*c(i,j)
         do 4 i=1,mc
         do 4 j=1,nc
            w(i,j)=dble(i+j)
 4          d(i,j)=w(i,j)*c(i,j)
      elseif(ch(1:3).eq.'add') then
         do 10 k=1,ia
 10         a(k)=2+a(k)
         do 20 k=1,ib
 20         b(k)=2.0+b(k)
         do 30 i=1,mc
         do 30 j=1,nc
 30         c(i,j)=2.0d0+c(i,j)
         do 40 i=1,mc
         do 40 j=1,nc
            w(i,j)=dble(i+j)
 40         d(i,j)=w(i,j)+c(i,j)
      else
         err=1
      endif
      end


