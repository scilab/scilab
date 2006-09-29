      subroutine scidint(fname) 
C     ================================================================
C     addinter 
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr,gettype
      logical getwsmat,checkrhs,getsmat,lib_cpp,getscalar
      character strf*25, c_cpp*10

      iadr(l)=l+l-1
c
      call ref2val
c
      if (.not.checkrhs(fname,3,4)) return
      topk=top

      lib_cpp=.false.
      strf=' '
      len=0
      if (rhs .eq. 4) then
        if(.not.getsmat(fname,topk,top,m4,n4,1,1,lr4,nlr4))return
        if (n4 .ne. 1) then
          buf=fname//' : option name has a wrong size, string expected'
          call error(999)
          return
        endif

        call cvstr(nlr4,istk(lr4),strf,1)
        if ((strf(1:3) .eq. 'c++') .or. 
     $      (strf(1:3) .eq. 'C++')) then
           lib_cpp=.true.
           c_cpp='none'
           len=4
           if ((nlr4 .gt. 3) .and. (strf(4:4) .eq. '=')) then
              if (nlr4 .eq. 4) then
                 buf=fname//' : none C++ compiler name !!'
                 call error(999)
                 return
              endif
              c_cpp=strf(5:nlr4) 
              len=nlr4-4
           endif
        else
           buf=fname//' : bad option name'
           call error(999)
           return
        endif
        top=top-1
      endif
      c_cpp(len+1:len+1)=char(0)

      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top=top-1
      if(.not.getsmat(fname,topk,top,m2,n2,1,1,lr2,nlr2))return
      if ( m2*n2.ne.1) then
         buf=fname //' : ename has a wrong size, 1x1 expected'
         call error(999)
         return
      endif
      if ( nlr2.gt.24) then 
         buf=fname //' : ename max size 24'
         call error(999)
         return
      endif
      call cvstr(nlr2,istk(lr2),strf,1)
      strf(nlr2+1:nlr2+1)=char(0)
      top=top-1
C     first argument 
C     jpc on accepte un entier 
      itop = gettype(top)
      if ( itop .eq. 1 ) then 
         if (.not.getscalar(fname,topk,top,il1)) return
         ilib=int(stk(il1))
         call addinter(ilib,iv,iv,iv,strf,
     $        istk(il3),istk(ild3),m3*n3,c_cpp,lib_cpp,ierr)
      else
         if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
         ilib=-1
         call addinter(ilib,istk(il1),istk(ild1),m1*n1,strf,
     $        istk(il3),istk(ild3),m3*n3,c_cpp,lib_cpp,ierr)
      endif
      if(ierr.ne.0) then
         if (ierr.eq.-1) then
            call error(236)
         elseif (ierr.eq.-2) then
            call error(239)
         elseif (ierr.eq.-3) then
            call error(238)
         elseif (ierr.eq.-4) then
            call error(237)
         elseif (ierr.eq.-5) then
            call error(231)
         elseif (ierr.eq.-6) then
            call error(234)
         elseif (ierr.eq.1) then
            call error(233)
         elseif (ierr.eq.2) then
            call error(232)
         else
            buf = fname // ': Error '
            call error(999)
         endif
         return
      endif
      call objvide(fname,top)
      return
      end
      