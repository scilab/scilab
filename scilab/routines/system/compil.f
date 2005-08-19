      logical function compil(code,val1,val2,val3,val4)
c
c     add  compiled instruction in compiled macro structure
c
c     Copyright INRIA
      integer val1(*),val2,val3,val4,l
      include '../stack.h'
      external getendian
      integer getendian
      integer code,sadr

c
      sadr(l)=(l/2)+1
c
      compil=.false.
      if (comp(1).eq.0) return
      compil=.true.
      l=comp(1)
      if(code.eq.1) then
c     put in stack  <1,nom>
         err=sadr(l+(nsiz+1))-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         call putid(istk(l+1),val1)
         comp(1)=l+1+nsiz
      elseif(code.eq.2) then
c     get from stack  <2 nom fin rhs>
         err=sadr(l+(nsiz+3))-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         call putid(istk(l+1),val1)
         istk(l+1+nsiz)=val2
         istk(l+2+nsiz)=val3
         comp(1)=l+3+nsiz
      elseif(code.eq.5) then
c     allops 
         err=sadr(l+4)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         istk(l+1)=val1(1)
         istk(l+2)=val2
         istk(l+3)=val3
         comp(1)=l+4
      elseif(code.eq.6) then
c     set num <6 ix(1),ix(2)>
         err=sadr(l+3)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         if(getendian().eq.1) then
            istk(l+1)=val1(1)
            istk(l+2)=val1(2)
         else
            istk(l+1)=val1(2)
            istk(l+2)=val1(1)
         endif
         comp(1)=l+3
      elseif(code.eq.16) then
c     set line number
         err=sadr(l+1)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         istk(l+1)=val1(1)
         comp(1)=l+2
      elseif(code.eq.18) then
c     mark named argument
         err=sadr(l+nsiz+1)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         call putid(istk(l+1),val1)
         comp(1)=l+nsiz+1
      elseif(code.eq.19) then
c     form recursive extraction list
         err=sadr(l+3)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         istk(l+1)=val1(1)
         istk(l+2)=val2
         comp(1)=l+3
      elseif(code.eq.22) then
c     set print mode
         err=sadr(l+1)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         istk(l+1)=val1(1)
         comp(1)=l+2
      elseif(code.eq.23) then
c     name2var
         err=sadr(l+nsiz+1)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         call putid(istk(l+1),val1)
         comp(1)=l+nsiz+1
      elseif(code.eq.25) then
c     profile
         err=sadr(l+3)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         istk(l+1)=val1(1)
         istk(l+2)=0
         comp(1)=l+3
      elseif(code.eq.27) then
c     varfunptr <27,fun,fin,id>
         err=sadr(l+(nsiz+3))-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         istk(l+1)=val2
         istk(l+2)=val3
         call putid(istk(l+3),val1)
         comp(1)=l+3+nsiz
      elseif(code.eq.29) then
c     affectation <29,n,print,name1,rhs1,...,name,rhsn>
         lhs=val1(1)
         err=sadr(l+lhs*(nsiz+1)+3)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code  
         istk(l+1)=lhs
         istk(l+2)=val2
         li=l+3
         do i=lhs,1,-1
            call putid(istk(li),ids(1,pt-lhs+i))
            istk(li+nsiz)=pstk(pt-lhs+i)
            li=li+(nsiz+1)
         enddo
         comp(1)=li
      elseif(code.eq.30) then
c     logical expression shortcircuit <30,it,n>
         if (val2.eq.0) then
            err=sadr(l+3)-lstk(bot)
            if(err.gt.0) goto 90
            istk(l)=code  
            istk(l+1)=val1(1)
            istk(l+2)=l+3
            comp(1)=l+3
         else
c     .     istk(l+2) value affectation at the end of term evaluation
            istk(val2)=comp(1)-istk(val2)
         endif

      elseif(code.ge.100) then
c     appel des fonctions <100*fun rhs lhs fin>
         err=sadr(l+(nsiz+3))-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         istk(l+1)=val1(1)
         istk(l+2)=val2
         istk(l+3)=val3
         comp(1)=l+4
      else
c     defmat:<4>
c     pause :<12>
c     break :<13>
c     abort :<14>
c     seteol:<15>
c     quit  :<17>
c     exit  :<20>
c     begrhs:<21>
c     deffnull:<24>
c     continue:<28>
c     return:<99>
         err=sadr(l+2)-lstk(bot)
         if(err.gt.0) goto 90
         istk(l)=code
         comp(1)=l+1
      endif

      return
 90   call error(17)
      return
      end

