      subroutine cosend(x,xptr,z,zptr,iz,izptr,told,inpptr,inplnk,
c     Copyright INRIA
     $     outptr,outlnk,lnkptr,cord,rpar,rpptr,ipar,ipptr,funptr,
     $     funtyp,outtb,w,ierr) 
C     ending  (flag 5)
      double precision x(*),z(*),told,rpar(*),outtb(*),w(*)
      integer xptr(*),zptr(*),iz(*),izptr(*)
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer cord(*)
      integer rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*),ierr
c
      integer flag,nclock,ntvec
      double precision tvec(1)
c
      integer         nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
      common /cossiz/ nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
C     
      integer kfun
      common /curblk/ kfun
c
      integer solver
      common /cmsolver/ solver
C     
      ierr = 0

C     loop on blocks
      tvec(1)=0.0d0
      ntvec=0
      do 5 kfun=1,nblk
         flag=5
         if (funtyp(kfun).ge.0) then
            call callf(kfun,ntvec,funptr,funtyp,told,x(xptr(nblk+1))
     $           ,x,x,xptr,z,zptr,iz,
     $           izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk,
     $           outptr,outlnk,lnkptr,outtb,flag) 
            if(flag.lt.0.and.ierr.eq.0) then
               ierr=5-flag
               kfune=kfun
            endif
         endif
 5    continue
      if(ierr.ne.0) then
         kfun=kfune
         return
      endif
      return
      end
