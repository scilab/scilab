      subroutine v2unit(k,mode,lunit,opened,ierr)
c     given variable #k (scalar or string) and mode 
c     v2unit return a  logical unit attached to corresponding file

      INCLUDE '../stack.h'
c
      logical opened
      integer mode(2)
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      ierr=0
      il=iadr(lstk(k))
      if (istk(il).eq.1) then
         lunit = int(stk(sadr(il+4)))
         if (istk(il+1)*istk(il+2).ne.1.or.istk(il+3).ne.0.or.
     $        lunit.lt.0) then
            err=1
            ierr=1
            call error(36)
            return
         endif
         call getfileinfo(lunit,ifa,iswap,ltype,mod,buf,lb,info)
         if(info.eq.0.and.ltype.eq.2) then
            call error(243)
            return
         endif
         opened=.true.
      elseif(istk(il).eq.10) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            ierr=1
            call error(36)
            return
         endif


         kk = abs(mode(1))
         ifor = kk / 100
         kk    = kk - 100 *ifor
         iacc = kk / 10
         if(mode(2).eq.0.and.iacc.eq.1) then
            ierr=1
            call error(242)
            return
         endif

         mn=istk(il+5)-1
         call cvstr(mn,istk(il+5+istk(il+1)*istk(il+2)),buf,1)
         lunit = 0
         call clunit(lunit,buf(1:mn),mode)
         if(err.gt.0) then
            ierr=1
            buf(mn+1:)=' '
            call error(err)
            return
         endif
         opened=.false.
      else
         err=1
         ierr=1
         call error(36)
         return
      endif
      end
