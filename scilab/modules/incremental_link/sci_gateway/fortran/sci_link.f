      subroutine scilink(fname) 
C     ================================================================
C    link function  
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr
      integer m3,n3,lr3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      integer ilib,gettype,iv ,l1
      logical getwsmat,checkrhs,getsmat,getscalar,cremat
      character*(2) strf
      iadr(l)=l+l-1

      call ref2val
      strf='f'//char(0)
      rhs = max(0,rhs)
      if (rhs.eq.0) then
        call scilinknorhs
        return
      else
      if (.not.checkrhs(fname,1,3)) return
      topk=top  
C     third argument if present is a char 
      if (rhs.ge.3) then
         if(.not.getsmat(fname,topk,top,m3,n3,1,1,lr3,nlr3))return
         if (nlr3.ne.1) then
            buf=fname //' : flag has a wrong size, 1 expected'
            call error(999)
            return
         endif
         call cvstr(nlr3,istk(lr3),strf,1)
         top=top-1
      endif
C     second argument 
      if (rhs.ge.2) then
         if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
         top=top-1
      endif
C     first argument 
      itype=gettype(top) 
      if ( itype.eq.1) then 
         if (.not.getscalar(fname,topk,top,l1)) return
         if (rhs.eq.1) then 
            buf = fname // ': must have two arguments when '
     $           // 'first arg is an integer'
            call error(999)
            return
         endif
         ilib=int(stk(l1))
         iflag = 1
         call iscilink(iv,iv,iv,
     $        istk(il2),istk(ild2),m2*n2,strf,ilib,iflag,rhs)
      else 
         if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
         iflag = 0
         if ( rhs.eq.1) then 
            call iscilink(istk(il1),istk(ild1),m1*n1,
     $           iv,iv,iv,strf,ilib,iflag,rhs)
         else
            call iscilink(istk(il1),istk(ild1),m1*n1,
     $           istk(il2),istk(ild2),m2*n2,strf,ilib,iflag,rhs)
         endif
         if ( ilib.lt.0 ) then 
            if (ilib.eq.-1) then
               call error(236)
            elseif (ilib.eq.-2) then
               call error(239)
            elseif (ilib.eq.-3) then
               call error(238)
            elseif (ilib.eq.-4) then
               call error(237)
            elseif (ilib.eq.-5) then
               call error(235)
            elseif (ilib.eq.-6) then
               call error(235)
            else
               buf= fname // ': Error'
               call error(999)
            endif
            return
         endif
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = ilib
         return
      endif
      return

      endif
      
      end
