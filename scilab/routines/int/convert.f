      subroutine convert(it)
C     ---------------------------------------------
C     converts the first argument 
C     (on top of stack)
C     it: is the new requested type 
C     0,1,2,4,11,12,14 : 
C     (double,char, short,int,uchar, ushort,uint)
C     ----------------------------------------------

      include '../stack.h'
      logical checkrhs,checklhs
      integer iadr,sadr,memused
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(.not.checkrhs('convert',1,1)) return
      if(.not.checklhs('convert',1,1)) return
      il=iadr(lstk(top))
      if(istk(il).eq.1) then
c     .  double to intn
         if(it.ne.0) then
            if(istk(il+3).ne.0) then
               err=1
               call error(52)
            endif
            mn=istk(il+1)*istk(il+2)
            lr=il+4
            l=sadr(lr)
            istk(il)=8
            istk(il+3)=it
            call tpconv(0,it,mn,stk(l),1,istk(lr),1)
            lstk(top+1)=sadr(lr+memused(it,mn))
         endif
      elseif(istk(il).eq.8) then
c     intn to intm or double
         mn=istk(il+1)*istk(il+2)
         is1=istk(il+3)
         l=il+4
         if(it.eq.0) then
            istk(il)=1
            istk(il+3)=0
            lr=sadr(l)
            call tpconv(is1,0,mn,istk(l),-1,stk(lr),-1)
            lstk(top+1)=lr+mn
         else
            istk(il)=8
            istk(il+3)=it
            lr=l
            inc=-1
            if (mod(is1,10).gt.(mod(it,10)))  inc=1
            call tpconv(is1,it,mn,istk(l),inc,istk(lr),inc)
            lstk(top+1)=sadr(lr+memused(it,mn))
         endif
      else
         err=1
         call error(53)
      endif
      return
      end
