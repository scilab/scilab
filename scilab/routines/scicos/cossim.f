      subroutine cossim(neq,x,xptr,z,zptr,iz,izptr,told,tf,
c     Copyright INRIA
     $     tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr,
     $     outlnk,lnkptr,clkptr,ordptr,nptr,
     $     ordclk,nordcl,ztyp,cord,iord,niord,oord,zord,
     $     critev,rpar,rpptr,ipar,
     $     ipptr,funptr,funtyp,rhot,ihot,outtb,jroot,w,iwa,ierr) 
C     
C     
      include '../stack.h'
C..   Parameters .. 
c     maximum number of clock output for one block
      integer nts
      parameter (nts=100)
C     
      character ch*4,ch2*10
      common /cosdebug/ cosd
      integer cosd
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*),rhot(*),rhotmp
      integer iwa(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr,ztyp(*)
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ihot(*),jroot(*),ierr
c
      logical hot,stuck
      integer i,k,ierr1,iopt,istate,itask,j,jdum,jt,
     &     ksz,flag,keve,kpo,nord,nclock
      double precision t
      double precision tvec(nts)
c
      external grblk,simblk

      integer otimer,ntimer,stimer
      external stimer

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
      double precision scale
      common /rtfactor/ scale  
c     
      integer solver
      common /cmsolver/ solver

      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c
      save otimer
      data otimer/0/
c
      halt=0
      ierr = 0
      hot = .false.
      stuck=.false.
      call xscion(inxsci)
C     initialization
      call iset(niwp,0,ihot,1)
      call dset(nrwp,0.0d0,rhot,1)
      call realtimeinit(told, scale)
 
      ntvec=0


c    .  saving zcross block number in jroot(ng+xx)
      jj=0
      do 03 kfun = 1,nblk
         if (ztyp(kfun).eq.1) then
            jj=jj+1
            jroot(ng+jj)=kfun
         endif
 03   continue
c     . Il fault:  ng >= jj
      if (jj .ne. ng)  jroot(ng+jj+1)=-1
c     .  saving zcross block number in jroot(ng+xx)
      
c     initialisation (propagation of constant blocks outputs)
      if(niord.eq.0) goto 10
      do 05 jj=1,niord
         kfun=iord(jj)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            nclock = iord(jj+niord)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,x(xptr(nblk+1)),
     $           x,x,xptr,z,zptr,iz,
     $           izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk
     $           ,outptr,outlnk,lnkptr,outtb,flag) 
            if (flag .lt. 0) then
               ierr = 5 - flag
               return
            endif
         endif
 05   continue
C     main loop on time
 10   continue
      if (told .ge. tf) return
      if (inxsci.eq.1) then
         ntimer=stimer()
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
c     .     sxevents can modify halt            
         endif
      endif
      if (halt.ne.0) then
         halt=0
         return
      endif
      if (pointi.eq.0) then
         t = tf
      else
         t = tevts(pointi)
      endif
      if (abs(t-told) .lt. ttol) then
         t = told
C     update output part
      endif
      if (told .gt. t) then
C     !  scheduling problem
         ierr = 1
         return
      endif
      if (told .ne. t) then
         if (xptr(nblk+1) .eq. 1) then
C     .     no continuous state
            if(told+deltat+ttol.gt.t) then
               told=t
            else
               told=told+deltat
            endif
c     .     update outputs of 'c' type blocks with no continuous state
            if (ncord.eq.0) goto 343
            if (told .ge. tf) then
c     .     we are at the end, update continuous part before leaving            
               call cdoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $              ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $              ,outlnk,lnkptr,clkptr,ordptr,nptr
     $              ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $              rpar,rpptr,ipar
     $              ,ipptr,funptr,funtyp,outtb,w,hot,ierr) 
c     .        if(ierr.ne.0) return
               return
            endif
 343        continue
C     
         else
C     integrate
            if (hot) then
               istate = 2
            else
               istate = 1
            endif
            itask = 4
C     Compute tcrit (rhot(1))
c            rhot(1)=tf+ttol
c            kpo=pointi
c 20         if(critev(kpo).eq.1) then
c               rhot(1)=tevts(kpo)
c               goto 30
c            endif
c            kpo=evtspt(kpo)
c            if(kpo.ne.0) goto 20
c 30         continue
c
            rhotmp=tf+ttol
            kpo=pointi
 20         if(critev(kpo).eq.1) then
               rhotmp=tevts(kpo)
               goto 30
            endif
            kpo=evtspt(kpo)
            if(kpo.ne.0) goto 20
 30         continue
            if (rhotmp.lt.rhot(1)) istate=1
            rhot(1)=rhotmp
c

            iopt = 0
c
            jt = 2
            t=min(told+deltat,min(t,tf+ttol))
c     
c
            call lsodar(simblk,neq,x,told,t,1,rtol,atol,itask,
     &           istate,iopt,rhot,nrwp,ihot,niwp,jdum,jt,grblk,
     &           ng,jroot)
            if (istate .le. 0) then
               if (istate .eq. -3) then
                  if(stuck) then
                     ierr= 2
                     return
                  endif
                  itask = 2
                  istate = 1
                  call lsoda(simblk,neq,x,told,t,
     &                 1,rtol,atol,itask,
     &                 istate,iopt,rhot,nrwp,ihot,niwp,jdum,jt)
                  hot = .false.
                  stuck=.true.
                  if (istate .gt. 0) goto 38
               endif
C     !        integration problem
               ierr = 100-istate
               return
            endif
            hot = .true.
            stuck=.false.
 38         continue
c     .     update outputs of 'c' type  blocks
            nclock = 0
            ntvec=0
            if (ncord.gt.0) then
               if (told .ge. tf) then
c     .        we are at the end, update continuous part before leaving
                  call cdoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $                 ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $                 ,outlnk,lnkptr,clkptr,ordptr,nptr
     $                 ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $                 rpar,rpptr,ipar
     $                 ,ipptr,funptr,funtyp,outtb,w,hot,ierr) 
c                  if(ierr.ne.0) return
                  return
               endif
            endif
            if (istate .eq. 3) then
               
c     ......... update inputs to zcross blocks .......
               call grblk(neq,told,x,ng,w)
c     .  now the sign of jroot is saved in w(i) like the following example
c     do 39 jj = 1,ng
c     if (jroot(jj) .eq. 0 ) jroot(jj)=sign(2.0,w(jj))
c     39            continue
c     ....................... mis a jours les portes entres ............     
               
C     .        at a least one root has been found
               ig = 1
               do 50 jj = 1,ng
                  kfun=jroot(ng+jj)
                  if (kfun .eq. -1)  goto 51  
c     .       boucle sur les zcross blocks, ils ont sauves dans jroot(ng+xx)
c     .              loop on block input ports
                  ksz=0
                  do 42 kport=inpptr(kfun),inpptr(kfun+1)-1
c     .                 get corresponding link pointer 
                     klink=inplnk(kport)
                     ksz=ksz+lnkptr(klink+1)-lnkptr(klink)
 42               continue
                  ig=ig+ksz
                  ntvec=clkptr(kfun+1)-clkptr(kfun)
c     .           ...............................................
                  kev=0
                  do 44 j = 1,ksz
                     kev=2*kev+abs(jroot(ig-j))
 44               continue
                  if (kev .ne. 0) then
                     do 45 j = 1,ksz 
                        kev=2*kev
                        if (jroot(ig-j).eq.-1) kev=kev+1
 45                  continue
                     flag=3
                     ntvec=clkptr(kfun+1)-clkptr(kfun)
c     .              call corresponding block to determine output event (kev)
                     call callf(kfun,kev,funptr,funtyp,
     $                    told,x(xptr(nblk+1)),x,x,xptr,z,
     $                    zptr,iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $                    ntvec,inpptr,inplnk,outptr,outlnk,lnkptr,
     $                    outtb,flag) 
                     if(flag.lt.0) then
                        ierr=5-flag
                        return
                     endif
c     .              update event agenda
                     do 47 k=1,clkptr(kfun+1)-clkptr(kfun)
                        if (tvec(k).ge.told) then
                           if (critev(k+clkptr(kfun)-1).eq.1)
     $                             hot=.false.
                           call addevs(tevts,evtspt,nevts,pointi,
     &                          tvec(k),k+clkptr(kfun)-1,ierr1)
                           if (ierr1 .ne. 0) then
C     .                       nevts too small
                              ierr = 3
                              return
                           endif
                        endif
 47                  continue
                  endif
 50            continue
 51            continue
            endif
c     !     save initial signs of zero crossing surface
c     
         endif
         call realtime(told)
      else
C     .  t==told
c
         if(cosd.ge.1) then
            write(ch,'(i3)') pointi
            write(ch2,'(f9.3)') told
            call basout(io,wte,'Event:'//ch//' activated at t='//ch2)
         endif
c
         call ddoit(neq,x,xptr,z,zptr,iz,izptr,told,tf,
     $        tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr,
     $        outlnk,lnkptr,clkptr,ordptr,nptr,
     $        ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $        rpar,rpptr,ipar,
     $        ipptr,funptr,funtyp,outtb,w,iwa,hot,ierr) 
         if(cosd.ge.1) then
            call basout(io,wte,'End of activation')
         endif
         if(ierr.ne.0) return
C     
      endif
C     end of main loop on time
      goto 10
      end

 
      subroutine cossimdassl(neq,x,xptr,z,zptr,iz,izptr,told,tf,
c     Copyright INRIA
     $     tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr,
     $     outlnk,lnkptr,clkptr,ordptr,nptr,
     $     ordclk,nordcl,ztyp,cord,iord,niord,oord,zord,
     $     critev,rpar,rpptr,ipar,
     $     ipptr,funptr,funtyp,rhot,ihot,outtb,jroot,w,iwa,ierr) 
C     
C     
C..   Parameters .. 
c     maximum number of clock output for one block
      include '../stack.h'
      character ch*4,ch2*10
      common /cosdebug/ cosd
      integer cosd
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*),rhot(*),rhotmp
      integer iwa(*)
C     X must contain after state values all real data for simblk and grblk
      integer xptr(*),zptr(*),iz(*),izptr(*),evtspt(nevts),nevts,pointi
      integer inpptr(*),inplnk(*),outptr(*),outlnk(*),lnkptr(*)
      integer clkptr(*),ordptr(nptr),nptr,ztyp(*)
      integer ordclk(nordcl,2),nordcl,cord(*),iord(*),oord(*),zord(*)
      integer critev(*),rpptr(*),ipar(*),ipptr(*),funptr(*),funtyp(*)
      integer ihot(*),jroot(*),ierr
c
      logical hot
c     logical stuck
      integer i,k,ierr1,iopt,istate,itask,j,jdum,jt,
     &     ksz,flag,keve,kpo,nord,nclock
      integer info(20),nng
      double precision t,tkeep
      double precision tvec(nts)
c
      external grblkdassl,simblkdassl

      integer otimer,ntimer,stimer
      external stimer

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
      double precision scale
      common /rtfactor/ scale  
c     
      integer solver
      common /cmsolver/ solver

      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c
      save otimer
      data otimer/0/
c    
      halt=0
      ierr = 0
c     hot = .false.
      info(1)=0
c      stuck=.false.
      call xscion(inxsci)
C     initialization
      call iset(niwp,0,ihot,1)
      call dset(nrwp,0.0d0,rhot,1)
      call realtimeinit(told, scale)
c     ATOL and RTOL are scalars
      info(2)=0
c     derivatives automatically computed by numerical differences
      info(5)=0
c     full jac matrx
      info(6)=0
      ntvec=0
c     max step size not given (handled by deltat)
      info(7)=0
c     code determines initial step size
      info(8)=0
C     MAXORD=5
      info(9)=0
c     no info on solution sign available
      info(10)=0
c     initial t,y,yprime are not assumed to be consistent
      info(11)=1
c     direc method instead of Dcrylof method
      info(12)=0
      info(13)=0
      info(14)=0
      info(15)=0
      info(16)=0
      info(17)=0
      info(18)=0
      do 02 jj=1,neq(1)
         ihot(40+jj) = 1
 02   continue


c    .  saving zcross block number in jroot(ng+xx)
      jj=0
      do 03 kfun = 1,nblk
         if (ztyp(kfun).eq.1) then
            jj=jj+1
            jroot(ng+jj)=kfun
         endif
 03   continue
c     . Il fault:  ng >= jj
      if (jj .ne. ng)  jroot(ng+jj+1)=-1
c    .  saving zcross block number in jroot(ng+xx)
     
c     initialisation (propagation of constant blocks outputs)
      if(niord.eq.0) goto 10
      do 05 jj=1,niord
         kfun=iord(jj)
         if(outptr(kfun+1)-outptr(kfun).gt.0) then
            nclock = iord(jj+niord)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,x(xptr(nblk+1)),
     $           x,x,xptr,z,zptr,iz,
     $           izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk
     $           ,outptr,outlnk,lnkptr,outtb,flag) 
            if (flag .lt. 0) then
               ierr = 5 - flag
               return
            endif
         endif
 05   continue
C     main loop on time
 10   continue
      if (told .ge. tf) return
      if (inxsci.eq.1) then
         ntimer=stimer()
         if (ntimer.ne.otimer) then
            call sxevents()
            otimer=ntimer
c     .     sxevents can modify halt
            
         endif
      endif
      if (halt.ne.0) then
         halt=0
         return
      endif
      if (pointi.eq.0) then
         t = tf
      else
         t = tevts(pointi)
      endif
      if (abs(t-told) .lt. ttol) then
         t = told
C     update output part
      endif
      if (told .gt. t) then
C     !  scheduling problem
         ierr = 1
         return
      endif
      if (told .ne. t) then
         if (xptr(nblk+1) .eq. 1) then
C     .     no continuous state
            if(told+deltat+ttol.gt.t) then
               told=t
            else
               told=told+deltat
            endif
c     .     update outputs of 'c' type blocks with no continuous state
            if (ncord.eq.0) goto 343
            if (told .ge. tf) then
c     .     we are at the end, update continuous part before leaving
c               hot=info(1).eq.1
               call cdoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $              ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $              ,outlnk,lnkptr,clkptr,ordptr,nptr
     $              ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $              rpar,rpptr,ipar
     $              ,ipptr,funptr,funtyp,outtb,w,hot,ierr) 
c               if(ierr.ne.0) return
               return
c               if(.NOT.hot) info(1)=0
            endif
 343        continue
C     
         else
            info(4)=1
            info(3)=0
C     Compute tcrit (rhot(1))
c            rhot(1)=tf+ttol
            rhotmp=tf+ttol
            
            kpo=pointi
 20         if(critev(kpo).eq.1) then
               rhotmp=tevts(kpo)
               goto 30
            endif
            kpo=evtspt(kpo)
            if(kpo.ne.0) goto 20
 30         continue
            if (rhotmp.lt.rhot(1)) info(1)=0
            rhot(1)=rhotmp
c     
            iopt = 0
c
            jt = 2
            t=min(told+deltat,min(t,tf+ttol))
c            if (stuck) then
c               info(3)=1
c               nng=0
c            else
c               nng=ng
c               info(3)=0
c            endif
            if(info(1).eq.0) then
               do 36 kfun=1,nblk
                  if(xptr(kfun+1)-xptr(kfun).gt.0) then
                     flag=7
                     nclock=0
                     call callf(kfun,nclock,funptr,funtyp,told,
     $                    x(xptr(nblk+1)),x,x(xptr(nblk+1))
     $                    ,xptr,z,zptr,iz,izptr,rpar,
     $                    rpptr,ipar,ipptr,tvec,ntvec,inpptr,
     $                    inplnk,outptr,outlnk,lnkptr,outtb,flag) 
                     if (flag .lt. 0) then
                        ierr = 5 - flag
                        return
                     endif
                  endif
 36            continue
            endif

c     Warning rpar and ipar are used here as dummy pointers
            call DDASKR (simblkdassl, neq, told, x, x(neq(1)+1),
     *           t, info, rtol , atol, istate, rhot, nrwp, ihot,
     *           niwp, rpar, ipar,jdum, rpar, grblkdassl, 
     *           ng,jroot)  

            if (istate .le. 0) then
C     !        integration problem (to be revised)
               ierr = 100-istate
               return
            endif
c            if (stuck) then
c               stuck=.false.
c               info(1)=0
c            endif
c            hot = .true.
 38         continue
c     .     update outputs of 'c' type  blocks
            nclock = 0
            ntvec=0
            if (ncord.gt.0) then
               if (told .ge. tf) then
c     .        we are at the end, update continuous part before leaving
c                  hot=info(1).eq.1
                  call cdoit(neq,x,xptr,z,zptr,iz,izptr,told,tf
     $                 ,tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr
     $                 ,outlnk,lnkptr,clkptr,ordptr,nptr
     $                 ,ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $                 rpar,rpptr,ipar
     $                 ,ipptr,funptr,funtyp,outtb,w,hot,ierr) 
c                  if(ierr.ne.0) return
                  return
c                  if(.NOT.hot) info(1)=0
               endif
            endif
            if (istate .eq. 5) then
c               info(1)=0
c     ......... update inputs to zcross blocks .............
               call grblkdassl(neq,told,x,x(neq(1)+1),ng,w,rpar,ipar)
c     .  now the sign of jroot is saved in w(i) like the following example
c     do 39 jj = 1,ng
c     if (jroot(jj) .eq. 0 ) jroot(jj)=sign(2.0,w(jj))
c     39            continue
c     ....................... mis a jours les portes entres ............     
 

C     .        at a least one root has been found
               ig = 1
               do 50 jj = 1,ng
                  kfun=jroot(ng+jj)
                  if (kfun .eq. -1)  goto 51  
c     .       boucle sur les zcross blocks, ils ont sauves dans jroot(ng+xx)
c     .              loop on block input ports
                  ksz=0
                  do 42 kport=inpptr(kfun),inpptr(kfun+1)-1
c     .                 get corresponding link pointer 
                     klink=inplnk(kport)
                     ksz=ksz+lnkptr(klink+1)-lnkptr(klink)
 42               continue
                  ig=ig+ksz
                  ntvec=clkptr(kfun+1)-clkptr(kfun)
c     .           ...............................................
                  kev=0
                  
c     .           ++++++++++++++++++++++++++++++++++++++
                  do 44 j = 1,ksz
                     kev=2*kev+abs(jroot(ig-j))
 44               continue
                  if (kev .ne. 0) then
                     do 45 j = 1,ksz 
                        kev=2*kev
                        if (jroot(ig-j).eq.-1) kev=kev+1
 45                  continue
                     flag=3
                     ntvec=clkptr(kfun+1)-clkptr(kfun)
c     .              call corresponding block to determine output event (kev)
                     call callf(kfun,kev,funptr,funtyp,
     $                    told,x(xptr(nblk+1)),x,x,xptr,z,zptr,
     $                    iz,izptr,rpar,rpptr,ipar,ipptr,tvec,
     $                    ntvec,inpptr,inplnk,outptr,outlnk,
     $                    lnkptr,outtb,flag) 
                     if(flag.lt.0) then
                        ierr=5-flag
                        return
                     endif
c     .              update event agenda
                     do 47 k=1,clkptr(kfun+1)-clkptr(kfun)
                        if (tvec(k).ge.told) then
                           if (critev(k+clkptr(kfun)-1).eq.1)
     $                          info(1)=0
                           call addevs(tevts,evtspt,nevts,
     $                          pointi,tvec(k),
     $                          k+clkptr(kfun)-1,ierr1)
                           if (ierr1 .ne. 0) then
C     .                       nevts too small
                              ierr = 3
                              return
                           endif
                        endif
 47                  continue
                  endif
 50            continue
 51            continue
            endif
c     !     save initial signs of zero crossing surface
c     
         endif
         call realtime(told)
      else
C     .  t==told
         if(cosd.ge.1) then
            write(ch,'(i4)') pointi
            write(ch2,'(f9.3)') told
            call basout(io,wte,'Event:'//ch//' activated at t='//ch2)
         endif
c
         hot=info(1).eq.1
         call ddoit(neq,x,xptr,z,zptr,iz,izptr,told,tf,
     $        tevts,evtspt,nevts,pointi,inpptr,inplnk,outptr,
     $        outlnk,lnkptr,clkptr,ordptr,nptr,
     $        ordclk,nordcl,cord,iord,niord,oord,zord,critev,
     $        rpar,rpptr,ipar,
     $        ipptr,funptr,funtyp,outtb,w,iwa,hot,ierr) 
         if(cosd.ge.1) then
            call basout(io,wte,'End of activation')
         endif
         if(ierr.ne.0) return
         if(.NOT.hot) info(1)=0
C     
      endif
C     end of main loop on time
      goto 10
      end
