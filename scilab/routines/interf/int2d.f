      subroutine int2d
C     --------------------------------------------
c     Scilab intg 
c      implicit undefined (a-z)
c     Copyright INRIA
      include '../stack.h'
      character*(5) fname
      character*(nlgh+1) namef
      integer iero 
      common/iertwo/iero
      external bint2d,fint2d
      double precision tol,result,erro
      logical getexternal,getrmat,type ,cremat,checkval
      integer topk,lr,katop,kydot,top2,lra,lrb,lc
c
      integer iadr,sadr
      external setfint2d
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      fname='int2d'
      if(rhs.lt.3) then
         call error(39)
         return
      endif
      type=.false.
      top2=top
      topk=top
      if(rhs.eq.4) then
         if (.not.getrmat(fname,topk,top,m1,n1,lr))  return
         if(.not.checkval(fname,m1*n1,5)) return
         tol=max(stk(lr),0.0d0)
         iclose=stk(lr+1)
         maxtri=max(int(stk(lr+2)),1)
         mevals=max(int(stk(lr+3)),1)
         iflag=stk(lr+4)
         if((iclose.ne.0.and.iclose.ne.1) .or. 
     $        (iflag.ne.0.and.iflag.ne.1)) then
            err=4
            call error(36)
         endif
         top=top-1
      else
         tol=1.d-10
         iclose=1
         maxtri=50
         mevals=4000
         iflag=1
      endif
c     
      if (.not.getexternal(fname,topk,top,namef,type,setfint2d)) return
      kext=top
      top=top-1
      
      if (.not.getrmat(fname,topk,top,my,ny,ly))  return
      if(.not.checkval(fname,my,3)) return

      top=top-1
      kxtop=top

      if (.not.getrmat(fname,topk,top,mx,nx,lx))  return
      if(.not.checkval(fname,mx,3)) return


      if(.not.checkval(fname,nx,ny)) return

c     definition des variables pour l'external
      top=top2+1
      kxx=top
      if (.not.cremat(fname,top,0,1,1,lxx,lc)) return

      top=top+1
      kyy=top
      if (.not.cremat(fname,top,0,1,1,lyy,lc)) return

c     tableaux de travail 
      lw=9*maxtri
      top=top+1
      if (.not.cremat(fname,top,0,1,lw,ldata,lc)) return

c     tableau de travail entier necessaire 
      liw=2*maxtri
      top=top+1
      if (.not.cremat(fname,top,0,1,iadr(liw)+1,liwork,lc)) return

c
c     external scilab
c
      top=top+1
      ipal=iadr(lstk(top))
      istk(ipal)=1
      istk(ipal+1)=ipal+2
      istk(ipal+2)=kext
      istk(ipal+3)=kxx
      istk(ipal+4)=kyy
      lstk(top+1)=sadr(ipal+5)

      nu=0
      nd=0
      if(type) then 
         call TWODQ(fint2d,nx,stk(lx),stk(ly),tol,iclose,maxtri,mevals
     $        ,result,erro,nu,nd,nevals,iflag,stk(ldata),stk(liwork))

      else
         call TWODQ(bint2d,nx,stk(lx),stk(ly),tol,iclose,maxtri,mevals
     $        ,result,erro,nu,nd,nevals,iflag,stk(ldata),stk(liwork))

      endif

      if(err.gt.0.or.err1.gt.0)return
      if(iflag.gt.0) then
         if(iflag.eq.1) then
c     termination for lack of space to divide triangle
            call msgs(81,0)
         elseif(iflag.eq.2) then
c     termination because of roundoff noise
            call msgs(82,0)
         elseif(iflag.eq.3) then
c     termination for relative error <5.0*%eps
            call msgs(83,0)
         elseif(iflag.eq.4) then
c     termination: number of function evaluations > MEVALS
            call msgs(84,0)
         endif
      endif
      top=top2-rhs+1
      if (.not.cremat(fname,top,0,1,1,lr1,lc)) return
      stk(lr1)=result
      if(lhs.ge.2) then
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr2,lc)) return
         stk(lr2)=erro
      endif
      if(lhs.eq.3) then
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr2,lc)) return
         stk(lr2)=nevals
      endif
      return
      end

