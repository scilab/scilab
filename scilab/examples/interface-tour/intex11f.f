      subroutine intex11f(fname)
      include 'stack.h'
c     -----------------------------------
c     Executing the Scilab function "myfct" defined in ex11f.sce
      character*(*) fname
      logical getrhsvar, scistring
      logical checklhs,checkrhs
c
       minrhs=6
       maxrhs=6
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c
      if(.not.getrhsvar(1,'d',m1,n1,l1)) return
      if(.not.getrhsvar(2,'d',m2,n2,l2)) return
      if(.not.getrhsvar(3,'d',m2,n2,l2)) return
      if(.not.getrhsvar(4,'d',m2,n2,l2)) return
      if(.not.getrhsvar(5,'d',m2,n2,l2)) return
      if(.not.getrhsvar(6,'d',m2,n2,l2)) return

c     We receive 6 input variables indexed by (1,2,...6)
c     when the command ex11f(x1,x2,x3,x4,x5,x6) is issued.
 
c     We have a Scilab function "myfct" with mrhs=2 inputs 
c     and mlhs=3 outputs:

c     function [u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
c     To run myfct with input variables x5 and x6,
c     we must set ifirst=5. Variables passed to the function must
c     appear consecutively with index ifirst, ifirst+1,..., ifirst+mrhs.
      mlhs=3
      mrhs=2
      ifirst=5
c     Variables #5 (x5) and #6 (x6) are the two inputs (x,y above) of "myfct",
c     i.e. myfct(x5,x6) is executed now:
      if(.not.scistring(ifirst,'myfct',mlhs,mrhs)) return

c     Output variables u, v, and w of myfct 
c     are now indexed by ifirst, ifirst+1, ifirst+mlhs i.e.
c     u and v are indexed by 5 and 6 resp. and w (created by myfct)
c     is indexed by 7.
c     We return u v and w:
c     Caution: Variables with index larger than ifirst+mrhs cannot be 
c     returned to Scilab.

       lhsvar(1)=5
       lhsvar(2)=6
       lhsvar(3)=7
c
       end



