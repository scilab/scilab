      logical function isany(il)
c     given il the adress of the begining of a scilab variable 
c     in istk isany returns true if one of the variable entry is non
c     zero or true
      include '../stack.h'
      integer genisany
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(istk(il).lt.0) il=iadr(istk(il+1))
c
      if(istk(il).eq.1) then
         mn=istk(il+1)*istk(il+2)
         l=sadr(il+4)
         do 10 i=0,mn-1
            if(stk(l+i).eq.1.0d0) goto 30
 10      continue
      elseif(istk(il).eq.4) then
         mn=istk(il+1)*istk(il+2)
         do 20 i=1,mn
            if(istk(il+2+i).eq.1) goto 30
 20      continue
      elseif(istk(il).eq.8) then
         mn=istk(il+1)*istk(il+2)
         is=istk(il+3)
         it=genisany(is, mn,istk(il+4) ,1)
         isany=it.eq.1
      endif
      isany=.false.
      return
 30   isany=.true.
      return
      end
