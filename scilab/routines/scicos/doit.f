      subroutine doit(neq,x,xptr,z,zptr,iz,izptr,told,tf
c     Copyright INRIA
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,w,hot,urg,ierr) 
C     
C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr
c     
      logical hot
      integer i,ierr1,flag,keve,nord,nclock,urg
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
      do 80 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            nclock=ordclk(ii,2)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,
     $           x(xptr(nblk+1)),x,x,xptr,z,
     $           zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
         endif
 80   continue
c     
      do 60 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
c     .     Initialize tvec
         ntvec=clkptr(kfun+1)-clkptr(kfun)
         if(ntvec.gt.0) then
            if(funtyp(kfun).lt.0)  then
               if(funtyp(kfun).eq.-1)  then
                  if (outtb(lnkptr(inplnk(inpptr(kfun)))).le.0) then
                     ntvec=2
                  else
                     ntvec=1
                  endif
               elseif (funtyp(kfun).eq.-2)  then
                  ntvec=max(min(int(outtb(
     $                 lnkptr(inplnk(inpptr(kfun))))),
     $                 ntvec),1)
               endif
               urg=urg+1
               call putevs(tevts,evtspt,nevts,pointi,
     &              told,ntvec+clkptr(kfun)-1,ierr1)
               if (ierr1 .ne. 0) then
C     !                 event conflict
                  ierr = 3
                  return
               endif
            endif
         endif
 60   continue
      end








      subroutine cdoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,w,hot,ierr) 
C     

C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr
c     
      logical hot
      integer i,ierr1,flag,nclock,urg
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
      do 19 jj=1,ncord
         kfun=cord(jj)
         nclock = cord(jj+ncord)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,
     $           x(xptr(nblk+1)),x,x,xptr,z,zptr,iz,izptr,rpar,
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
            if(funtyp(kfun).lt.0) then
c     
               if (funtyp(kfun).eq.-1) then
                  if (outtb(lnkptr(inplnk(inpptr(kfun)))).le.0) then
                     ntvec=2
                  else
                     ntvec=1
                  endif
               elseif (funtyp(kfun).eq.-2) then
                  ntvec=max(min(int(outtb(
     $                 lnkptr(inplnk(inpptr(kfun))))),
     $                 ntvec),1)
               endif
               urg=urg+1
               call putevs(tevts,evtspt,nevts,pointi,
     &              told,ntvec+clkptr(kfun)-1,ierr1)
               if (ierr1 .ne. 0) then
C     !                 event conflict
                  ierr = 3
                  return
               endif
            endif
         endif
 19   continue
      if (urg.le.0)  return
 20   call doit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,w,hot,urg,ierr) 
      if (ierr .ne. 0) then
         return
      endif
      if (urg.gt.0) goto 20
      return
      end





      subroutine ddoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,w,iwa,hot,ierr) 
C     
C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*)
      integer iwa(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr

c     
      logical hot
      integer i,k,ierr1,j,flag,keve,nord,nclock,urg,kiwa
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
      kiwa=0
      urg=1
      if (urg.gt.0)  then
 43      call edoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $        ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $        ,outlnk,lnkptr,clkptr,ordptr,nptr
     $        ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $        rpar,rpptr,ipar
     $        ,ipptr,funptr,funtyp,outtb,w,hot,urg,ierr,iwa,kiwa) 
         if (ierr .ne. 0) then
            return
         endif
         if(urg.gt.0) goto 43
      endif

c     .  update continuous and discrete states on event
      if (kiwa.eq.0) then
         return
      endif

      do 61 i=1,kiwa
         keve=iwa(i)
         do 160 ii=ordptr(keve),ordptr(keve+1)-1
            kfun=ordclk(ii,1)
C     If continuous state jumps, do cold restart
            if(xptr(kfun+1)-xptr(kfun)+zptr(kfun+1)-zptr(kfun)
     $           .gt.0) then 
c     Solution not satisfying but... Have to find a better test
c     to know if state can jump. If we only leave the first test
c     it sets hot to false at every event!
               if(xptr(kfun+1)-xptr(kfun).gt.0.and.ordclk(ii,2).ne.0)
     $              hot=.false.
               flag=2
               call callf(kfun,ordclk(ii,2),funptr,funtyp,told,
     $              x(xptr(nblk+1)),x,x,
     $              xptr,z,zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $              ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $              outtb,flag) 
               if(flag.lt.0) then
                  ierr=5-flag
                  return
               endif
            endif
 160     continue
 61   continue
      end






      subroutine edoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $     ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $     ,outlnk,lnkptr,clkptr,ordptr,nptr
     $     ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $     rpar,rpptr,ipar
     $     ,ipptr,funptr,funtyp,outtb,w,hot,urg,ierr,iwa,kiwa) 
C     
C     
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      integer neq(*),iwa(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ierr
c     
      logical hot
      integer i,ierr1,flag,keve,nord,nclock,urg,kiwa
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
      kiwa=kiwa+1
      iwa(kiwa)=keve


      do 80 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
c 
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            nclock=ordclk(ii,2)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,
     $           x(xptr(nblk+1)),x,x,xptr,z,
     $           zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $           ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $           outtb,flag) 
            if(flag.lt.0) then
               ierr=5-flag
               return
            endif
         endif
 80   continue
c     
      do 60 ii=ordptr(keve),ordptr(keve+1)-1
         kfun=ordclk(ii,1)
c     .     Initialize tvec
         ntvec=clkptr(kfun+1)-clkptr(kfun)
         if(ntvec.gt.0) then
            if (funtyp(kfun).ge.0) then
               call dset(ntvec,told-1.0d0,tvec,1)
c     
               flag=3
               call callf(kfun,ordclk(ii,2),funptr,funtyp,told,
     $              x(xptr(nblk+1)),x,x,
     $              xptr,z,zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $              ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $              outtb,flag) 
               if(flag.lt.0) then
                  ierr=5-flag
                  return
               endif
c     
               if (ntvec.ge.1) then
                  do 70 i = 1,ntvec
                     if (tvec(i) .ge. told) then
                        call addevs(tevts,evtspt,nevts,pointi,
     &                       tvec(i),i+clkptr(kfun)-1,ierr1)
                        if (ierr1 .ne. 0) then
C     !                 event conflict
                           ierr = 3
                           return
                        endif
                     endif
 70               continue
               endif

            else
               if (funtyp(kfun).eq.-1) then
                  if (outtb(lnkptr(inplnk(inpptr(kfun)))).le.0) then
                     ntvec=2
                  else
                     ntvec=1
                  endif
               elseif (funtyp(kfun).eq.-2) then
                  ntvec=max(min(int(outtb(
     $                 lnkptr(inplnk(inpptr(kfun))))),
     $                 ntvec),1)
               endif
               urg=urg+1
               call putevs(tevts,evtspt,nevts,pointi,
     &              told,ntvec+clkptr(kfun)-1,ierr1)
               if (ierr1 .ne. 0) then
C     !                 event conflict
                  ierr = 3
                  return
               endif
            endif
         endif
 60   continue
      end

