      subroutine bexec(str,ns,ierr)
c     ==========================================================
c     This routine prepare execution of a scilab  instruction (given 
c     in a character string str) within a C 
c     or fortran procedure
c     ==========================================================
c     Copyright INRIA
c     Bug 1054 corrected by Francois VOGEL, October 2004
      include '../stack.h'
      integer sadr,iadr
      character*(*) str
      character*5 tmp
      integer retu(6),comma,eol
c
      data retu/27,14,29,30,27,23/,comma/52/,eol/99/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      ierr=0
      if (ddt .eq. 4) then
         write(tmp,'(i4)') top
         call basout(io,wte,' bexec2  top:'//tmp(1:4))
      endif
c
      mrhs=0
      fin=0
c
c     create a scilab variable of type string in the top of the stack
c     this variable will contain the input string concatenate with 
c     the string of length 11 :   '<eol>,return,<eol><eol>'
      top=top+1
      il=iadr(lstk(top))
      l=il+6
      err=sadr(l+ns+11)-lstk(bot)
      if(err.gt.0) then 
         call error(17)
         return
      endif
c     create the header
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      istk(il+5)=1+ns+11
c     copy the input string
      call cvstr(ns,istk(l),str,0)
c     concatenate '<eol>,return,<eol><eol>'
      l1=l+ns
      istk(l1)=eol
      l1=l1+1
      istk(l1)=comma
      l1=l1+1
      call icopy(6,retu,1,istk(l1),1)
      l1=l1+6
      istk(l1)=comma
      l1=l1+1
      istk(l1)=eol
      l1=l1+1
      istk(l1)=eol
c     set the end of the variable
      lstk(top+1)=sadr(l1)+1
c     inform scilab of the position of the variable containing
c     instructions 
      fin=lstk(top)
      return
      end


