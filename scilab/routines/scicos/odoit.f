      subroutine oodoit(neq,xd,x,xptr,z,zptr,iz,izptr,told
c     Copyright INRIA
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,urg,ierr,iwa) 
C     
C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100) 
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tevts(*),rpar(*),outtb(*),xd(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr
      integer iwa(*)
c     
      integer urg
      integer i,k,ierr1,iopt,istate,itask,j,jdum,jt,
     &     ksz,flag,keve,kpo,nord,nclock
      double precision t
      double precision tvec(nts)
c     
      integer         nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
      common /cossiz/ nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
C     
      integer halt
      common /coshlt/ halt
c     
      integer kfun
      common /curblk/ kfun
c     
      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c     
c     
      urg=urg-1
      keve = pointi
      pointi=evtspt(keve)
      evtspt(keve)=-1
c     
      nord=ordptr(keve+1)-ordptr(keve)
      if(nord.eq.0) return
c
      do 12 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
         iwa(kfun)=ordclk(ii,2)
 12   continue
c
c
      do 60 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            nclock=ordclk(ii,2)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,x,x,xptr,z,
     $           zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
         endif

c     .     Initialize tvec
         ntvec=clkptr(kfun+1)-clkptr(kfun)
         if(ntvec.gt.0) then
            call dset(ntvec,told-1.0d0,tvec,1)
c     
            flag=3
            call callf(kfun,ordclk(ii,2),funptr,funtyp,told,x,x,
     $           xptr,z,zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
c     
            if (ntvec.ge.1) then 
               if(funtyp(kfun).eq.-1) then
                  urg=urg+1
                  call putevs(tevts,evtspt,nevts,pointi,
     &                 told,ntvec+clkptr(kfun)-1,ierr1)
                  if (ierr1 .ne. 0) then
C     !                 event conflict
                     ierr = 3
                     return
                  endif
               endif
            endif
         endif
 60   continue
      
      
      do 65 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
         if(xptr(kfun+1)-xptr(kfun).gt.0) then
            nclock=ordclk(ii,2)
            flag=0
            call callf(kfun,nclock,funptr,funtyp,told,xd,x,xptr,z,
     $           zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
         endif
 65   continue
      end

      subroutine odoit(neq,xd,x,xptr,z,zptr,iz,izptr,told
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,oord,zord,critev
     $     ,rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,iwa,ierr) 
C     

C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tevts(*),rpar(*),outtb(*),xd(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr
      integer iwa(*)
c     
      integer i,k,ierr1,iopt,istate,itask,j,jdum,jt,urg,
     &     ksz,flag,keve,kpo,nord,nclock
      double precision t
      double precision tvec(nts)
c     
      integer         nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
      common /cossiz/ nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
C     
      integer halt
      common /coshlt/ halt
c     
      integer kfun
      common /curblk/ kfun
c     
      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c     
      urg=0
c      
      call iset(nblk,-1,iwa,1)
      do 11 ii=1,noord
         kfun=oord(ii)
         iwa(kfun)=oord(ii+noord)
 11   continue
c
      do 19 jj=1,noord
         kfun=oord(jj)
         nclock = oord(jj+noord)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,
     $           x,x,xptr,z,zptr,iz,izptr,rpar,
     $           rpptr,ipar,ipptr,tvec,ntvec,inpptr,
     $           inplnk,outptr,outlnk,lnkptr,outtb,flag) 
            if (flag .lt. 0) then
               ierr = 5 - flag
               return
            endif
         endif
c     
         ntvec=clkptr(kfun+1)-clkptr(kfun)
         if(ntvec.gt.0) then
            call dset(ntvec,told-1.0d0,tvec,1)
c     
            flag=3
            call callf(kfun,nclock,funptr,funtyp,told,x,x,
     $           xptr,z,zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
c  
              if (ntvec.ge.1) then
               if(funtyp(kfun).eq.-1) then
                  urg=urg+1
                  call putevs(tevts,evtspt,nevts,pointi,
     &                 told,ntvec+clkptr(kfun)-1,ierr1)
                  if (ierr1 .ne. 0) then
C     !                 event conflict
                     ierr = 3
                     return
                  endif
               endif
            endif
         endif
  
 19   continue
c

      if (urg.gt.0)  then
 21      call oodoit(neq,xd,x,xptr,z,zptr,iz,izptr,told
     $        ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $        ,outlnk,lnkptr,clkptr,ordptr,nptr
     $        ,ordclk,nordcl,cord,oord,zord,critev,
     $        rpar,rpptr,ipar
     $        ,ipptr,funptr,funtyp,outtb,urg,ierr,iwa) 
         if (urg.gt.0) goto 21
      endif

c     .  update states derivatives
      do 61 kfun=1,nblk
         if(iwa(kfun).ne.-1) then
            if(xptr(kfun+1)-xptr(kfun).gt.0) then
               flag=0
               nclock=iwa(kfun)
               call callf(kfun,nclock,funptr,funtyp,told,
     $              xd,x,xptr,z,zptr,iz,izptr,rpar,
     $              rpptr,ipar,ipptr,tvec,ntvec,inpptr,
     $              inplnk,outptr,outlnk,lnkptr,outtb,flag) 
               if (flag .lt. 0) then
                  ierr = 5 - flag
                  return
               endif
            endif
         endif
 61   continue
      end

