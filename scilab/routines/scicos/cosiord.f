      subroutine cosiord(neq,x,xptr,z,zptr,iz,izptr,told,tf,
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
      integer nts
      parameter (nts=100)
C     
      integer neq(*)
C     neq must contain after #states all integer data for simblk and grblk
      double precision x(*),z(*),told,tf,tevts(*),rpar(*),outtb(*)
      double precision w(*),rhot(*)
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


      integer         nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
      common /cossiz/ nblk,nordptr,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
C     
c
      integer kfun
      common /curblk/ kfun
c     
      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c

      ntvec=0

c     initialisation (propagation of constant blocks outputs)
      if(niord.eq.0) goto 10
      do 05 jj=1,niord
         kfun=iord(jj)
         nclock = iord(jj+niord)
         flag=1
         call callf(kfun,nclock,funptr,funtyp,told,x,x,xptr,z,zptr,iz,
     $        izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk
     $        ,outptr,outlnk,lnkptr,outtb,flag) 
         if (flag .lt. 0) then
            ierr = 5 - flag
            return
         endif
 05   continue
 10   return
      end

 
