      logical function  israt(il,ilnum,ilden,ildom)
c     given a scilab variable stored in the stack, beginning at istk(il)
c     israt checks is this variable is a rational fraction
c     if ok it also returns ilnum and ilden the adress of the beginning
c     of the variables (in istk) containing the numerator and the denominator

c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr, sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      israt=.false.

c     check if variable is a mlist or tlist (for compatibility)
      if(istk(il).ne.16.and.istk(il).ne.17) return

c     check if mlist has 4 elements
      if(istk(il+1).ne.4) return

c     check if first element is a character string vector
      ll=sadr(il+7)
      ill=iadr(ll)
      if(istk(ill).ne.10) return

c     check if first entry of the character string vector is 'r'
      if(abs(istk(ill+5+istk(ill+1)*istk(ill+2))).ne.27) return

c     check second and third tlist components
      ilnum=iadr(ll+istk(il+3)-1)

      if(istk(ilnum).gt.2) return

      ilden=iadr(ll+istk(il+4)-1)
      if(istk(ilden).gt.2) return

      ildom=iadr(ll+istk(il+5)-1)

      israt=.true.
      return
      end
