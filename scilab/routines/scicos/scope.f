      subroutine scope(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu)
c     Copyright INRIA
c     Scicos block simulator
c     ipar(1) = win_num
c     ipar(2) = 0/1 color flag
c     ipar(3) = buffer size

c     ipar(4:11) = line type for ith curve
c
c     ipar(12:13) : window position
c     ipar(14:15) : window dimension
c     ipar(16) : acceptance of inherited events
c
c     rpar(1)=dt
c     rpar(2)=ymin
c     rpar(3)=ymax
c     rpar(4)=periode
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu

c
c
      double precision dt,ymin,ymax,per,rect(4),tsave
      integer i,n,verb,cur,na,v,wid,nax(4)
      character*20 strf,buf
      character*4 name
      double precision dv
      double precision frect(4)
c      character*(4) logf
      logical herited

      data frect / 0.00d0,0.00d0,1.00d0,1.00d0/
      data cur/0/,verb/0/

c     
      if(nipar.lt.16) then
c     compatibility
         herited=.true.
         iwp=1+nipar-4
         iwd=1+nipar-2
      else
         herited=ipar(16).ne.0
         iwp=1+nipar-5
         iwd=1+nipar-3
      endif
c     

      if(flag.eq.2) then
         dt=rpar(1)
         ymin=rpar(2)
         ymax=rpar(3)
         per=rpar(4)
         wid=ipar(1)
         N=ipar(3)

         K=int(z(1))
         if(K.gt.0) then
            n1=int(z(1+K)/per)
            if(z(1+K).lt.0.0d0) n1=n1-1
         else
            n1=0
         endif
c     
         tsave=t
         if(dt.gt.0.0d0) t=z(1+K)+dt
c     
         n2=int(t/per)
         if(t.lt.0.0d0) n2=n2-1
c     
c     add new point to the buffer
         K=K+1
         z(1+K)=t
         do 01 i=1,nu
           z(1+N+(i-1)*N+K)=u(i)
 01      continue
         z(1)=K
         if(n1.eq.n2.and.K.lt.N) then
            t=tsave
            return
         endif
c     
c     plot 1:K points of the buffer
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
c         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
c     &        0,0,v,dv,dv,dv,dv)
         if(K.gt.0) then
            do 10 i=1,nu
c               if(ipar(3+i).ge.0.or.flag.eq.1) then
                  call dr1('xpolys'//char(0),'v'//char(0),v,v,
     &                 ipar(3+i),1,K,v,z(2),z(2+N+(i-1)*N),dv,dv)
c               endif
 10         continue
         endif
c     shift buffer left
         z(2)=z(1+K)
         do 15 i=1,nu
            z(1+N+(i-1)*N+1)=z(1+N+(i-1)*N+K)
 15      continue
         z(1)=1.0d0
         if(n1.ne.n2) then
c     clear window
            nax(1)=2
            nax(2)=10
            nax(3)=2
            nax(4)=10
            call dr1('xclear'//char(0),'v'//char(0),v,v,v,v,v,v,
     $           dv,dv,dv,dv)
            call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
     &           0,0,v,dv,dv,dv,dv)
            call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
            buf='t@ @input and output'
            strf='011'//char(0)
            rect(1)=per*(1+n1)
            rect(2)=ymin
            rect(3)=per*(2+n1)
            rect(4)=ymax
            call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &           0,0,v,dv,dv,dv,dv)
c            call dr1('xset'//char(0),'clipping-p'//char(0),-1.0d0,
c     &           -1.0d0,200000.0d0,200000.0d0,v,dv,dv,dv,dv)
            call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax,4,21)
c            call dr1('xset'//char(0),'clipping'//char(0),rect(1),ymin,per,
c     &           ymax,v,dv,dv,dv,dv)
         endif
         t=tsave
c
      elseif(flag.eq.4) then
         wid=ipar(1)
         N=ipar(3)
         ymin=rpar(2)
         ymax=rpar(3)
         per=rpar(4)
         nax(1)=2
         nax(2)=10
         nax(3)=2
         nax(4)=10
         n1=int(t)/per
         if(t.le.0.0d0) n1=n1-1
         call sciwin()
         call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $        dv,dv,dv,dv)
         call dr1('xsetdr'//char(0),'Rec'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
         if(ipar(iwp).ge.0) then
            call dr1('xset'//char(0),'wpos'//char(0),ipar(iwp),
     $           ipar(iwp+1),v,v,v,v,dv,dv,dv,dv)
         endif
         if(ipar(iwd).ge.0) then
            call dr1('xset'//char(0),'wdim'//char(0),ipar(iwd),
     $           ipar(iwd+1),v,v,v,v,dv,dv,dv,dv)
c     to force dimensions update 
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         rect(1)=per*(1+n1)
         rect(2)=ymin
         rect(3)=per*(2+n1)
         rect(4)=ymax
         call  setscale2d(frect,rect,'nn'//char(0))
         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'alufunction'//char(0),3,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xclear'//char(0),'v'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
         call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $        dv,dv,dv,dv)

         buf='t@ @input and output'//char(0)
         strf='011'//char(0)
         call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax,4,21)
         z(1)=0.0d0
         z(2)=t
         call dset(nu*N,0.0d0,z(3),1)
      elseif(flag.eq.5) then
         wid=ipar(1)
         N=ipar(3)
         K=int(z(1))
         if(K.le.1) return
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
c         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
c     &        0,0,v,dv,dv,dv,dv)
         do 30 i=1,nu
            call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(3+i),
     &           1,K,v,z(2),z(2+N+(i-1)*N),dv,dv)
 30      continue
      endif
      end
