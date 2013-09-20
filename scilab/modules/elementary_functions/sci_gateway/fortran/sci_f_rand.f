c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intrand(fname,id)
c     -------------------------------
c     Interface for rand function
      INCLUDE 'stack.h'
      character*(*) fname
      double precision sr,si,r
      save             si, r
      integer id(nsiz),tops,topk
      double precision urand
      logical checkrhs,checklhs,getsmat,getscalar,cremat
      logical cresmat2, getmat
      integer gettype, u1, v1
      character*(20) randtype
      logical phase
      integer iadr, mattyp1, areadr1, aimadr1
      save    phase
      data    phase /.true./
      
c
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
      if (.not.checklhs(fname,1,1)) return

      if(rhs.gt.3) then
c     .  more than 3 argument rand(n1,n2,n3,...) assumed
         call setfunnam(ids(1,pt+1),'%hm_rand',8)
         fun=-1
         return
      endif

      randtype='uniform'//char(0)
      topk=top
      tops=top

      if(rhs.eq.0) then
c     .  rand()         
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
*************** fix for bug 1826 (bruno)
         if ( ran(2).eq.0 ) then ! U(0,1) distribution
            stk(lr) = urand(ran(1))
         else                    ! N(0,1) distribution
            if (phase) then
 10            sr=2.d0*urand(ran(1)) - 1.d0
               si=2.d0*urand(ran(1)) - 1.d0
               t = sr*sr + si*si
               if (t .gt. 1.d0) go to 10
               r = sqrt(-2.d0*log(t)/t)
               stk(lr) = sr*r
            else
               stk(lr) = si*r
            endif
            phase = .not. phase
         endif
*************** end for bug fix
         return
      endif

c     checking first argument type
      itype=gettype(top-rhs+1) 
      if(itype.eq.10) then 
         if(.not.getsmat(fname,top,top-rhs+1,mt,nt,1,1,lrt,nlrt))return
         call cvstr(nlrt,istk(lrt),randtype,1)
         randtype(nlrt+1:nlrt+1)=char(0)

         if ( randtype(1:nlrt).eq.'seed') then 
c     .     handling seed
            if (.not.checkrhs(fname,1,2)) return
            if(rhs.eq.1) then
C     .        get seed
               if (.not.cremat(fname,top,0,1,1,lr,lc)) return
               stk(lr) = ran(1)            
            else
C     .        set seed
               if (.not.checkrhs(fname,2,2)) return
               if(.not.getscalar(fname,top,top-rhs+2,lr2))return
               ran(1) = max(int(stk(lr2)),0)
c              the following line added by bruno (19 nov 2004) to solve bug 1084
               phase = .true.   
               top=top-1
               call objvide(fname,top)
            endif
         elseif ( randtype(1:nlrt).eq.'info') then 
C     .     getting info 
            if (.not.checkrhs(fname,1,1)) return
            call randinfo(randtype,ilen) 
            if (.not.cresmat2(fname,top,ilen,lr)) return
            call cvstr(ilen,istk(lr),randtype,0) 
            return
         else
C     .     switching to an other law 
            if (.not.checkrhs(fname,1,1)) return
            call randswitch(randtype)
            call objvide(fname,top)
         endif
         return
      endif

c     checking last argument  for law option
      itype=gettype(top) 
      if(itype.eq.10) then 
c     .  a law is given
         if(rhs.gt.3) then
c     .     rand(n1,n2,n3,..,law) -> overloading
            call setfunnam(ids(1,pt+1),'%hm_rand',8)
            fun=-1
            return
         endif
         if(.not.getsmat(fname,top,top,mt,nt,1,1,lrt,nlrt))return
         call cvstr(nlrt,istk(lrt),randtype,1)
         randtype(nlrt+1:nlrt+1)=char(0)
         top=top-1
c     .  memorize that the law has been changed
         irt=1
         iran1kp=ran(2)
c     .  change the law temporarily
         call randswitch(randtype)
      else
         irt=0
      endif

      if ( rhs-irt.ge.3 ) then 
c     .  rand(n1,n2,n3,...) -> overloading
         call setfunnam(ids(1,pt+1),'%hm_rand',8)
         fun=-1
         return
      endif
C     
      itres=0
      if( rhs-irt.eq.2) then
c     . rand(n1,n2)
         call getdimfromvar(top,2,n)
         if(err.gt.0.or.err1.gt.0) return
         il=iadr(lstk(top))
         if(istk(il).eq.1.or.istk(il).eq.-1) then
            if (.not.getmat('rand', tops, top, 
     +            mattyp1, u1, v1, areadr1, aimadr1)) return !To have the dimensions of argument #2 
            if(u1.eq.-1.or.v1.eq.-1) then !detect ':' or eye
               call error(21)
               return
            endif
         endif
         
         top=top-1
         call getdimfromvar(top,1,m)
         if(err.gt.0.or.err1.gt.0) return
         il=iadr(lstk(top))
         if(istk(il).eq.1.or.istk(il).eq.-1) then
            if (.not.getmat('rand', tops, top, 
     +            mattyp1, u1, v1, areadr1, aimadr1)) return !To have the dimensions of argument #1 
            if(u1.eq.-1.or.v1.eq.-1) then !detect ':' or eye
               call error(21)
               return
            endif
         endif
      else
c     . rand(A)
         if(gettype(top).le.10) then
            il=iadr(lstk(top))
            if(istk(il).lt.0) il=iadr(istk(il+1))
            m=istk(il+1)
            n=istk(il+2)
            if(m.eq.-1.and.n.eq.-1) then !To avoid rand(:)
               call error(21)
               return
            endif
            if(gettype(top).le.2.or.gettype(top).eq.5) then
c     .        ask for result of the same real/complex type
               itres=istk(il+3)
            endif
         else
            top=tops
            il=iadr(lstk(top-rhs+1))
            if(istk(il).lt.0) il=iadr(istk(il+1))
            call funnam(ids(1,pt+1),'rand',il)
            fun=-1
            return
         endif
      endif

      if (m.eq.0.or.n.eq.0) then
         if (.not.cremat(fname,top,0,0,0,lr,lc)) return
         return
      endif

C     random generation 
      if (.not.cremat(fname,top,itres,m,n,lr,lc)) return
      if ( ran(2).eq.0 ) then 
c     .  U(0,1) random numbers
         do j = 0, (itres+1)*m*n-1
            stk(lr+j) = urand(ran(1))
         enddo
      elseif (ran(2).eq.1) then 
c     .  N(0,1) random numbers (modified by Bruno 11/10/2001
c     .  to use si*r and to correct the bug in the complex case)
         do j = 0, (itres+1)*m*n-1
            if (phase) then
 75            sr=2.d0*urand(ran(1)) - 1.d0
               si=2.d0*urand(ran(1)) - 1.d0
               t = sr*sr + si*si
               if (t .gt. 1.d0) go to 75
               r = sqrt(-2.d0*log(t)/t)
               stk(lr+j) = sr*r
            else
               stk(lr+j) = si*r
            endif
            phase = .not. phase
         enddo
      endif

C     switching back to the default randvalue
      if ( irt.ge.1) then 
         ran(2)=iran1kp
      endif
      return
      end
      

      subroutine randswitch(randtype)
      character*(20) randtype
      INCLUDE 'stack.h'
      if ( randtype(1:1).eq.'u') then 
         ran(2)=0
      else if ( randtype(1:1).eq.'g') then 
         ran(2)=1
      else if ( randtype(1:1).eq.'n') then 
         ran(2)=1
      else 
         ran(2)=0
      endif
      return
      end

      subroutine randinfo(randtype,ilen) 
      INCLUDE 'stack.h'
      character*(20) randtype
      integer ilen 
      if ( ran(2).eq.0) then 
         randtype='uniform'
         ilen=7
      else if ( ran(2).eq.1) then 
         randtype='normal'
         ilen=6
      endif
      return
      end
c     -------------------------------
