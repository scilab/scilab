      subroutine scopxy(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) = win_num
c     ipar(2) = 0/1 color flag
c     ipar(3) = buffer size
c     ipar(4) = dash,color or mark choice
c     ipar(5) = line or mark size
c     ipar(6) = mode : animated =0 fixed=1
c     ipar(7:8) = window position
c     ipar(9:10) = window size
c
c     rpar(1)=xmin
c     rpar(2)=xmax
c     rpar(3)=ymin
c     rpar(4)=ymax
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c
      double precision xmin,xmax,ymin,ymax,rect(4)
      integer n,verb,cur,na,v,wid,nax(4)
      character*40 strf,buf
      double precision dv
      double precision frect(4)
      character*(4) logf
      character*4 name

      integer kfun
      common /curblk/ kfun

      data frect / 0.00d0,0.00d0,1.00d0,1.00d0/
      data cur/0/,verb/0/

c     
      if(flag.eq.2) then
         wid=ipar(1)
         N=ipar(3)
c     
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
c     erase first point
         if(ipar(6).eq.0) then
            z(1)=z(1)+1.0d0
            if(ipar(4).lt.0) then
               call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &              1,1,v,z(2),z(2+N),dv,dv)
            else
               call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &              1,2,v,z(2),z(2+N),dv,dv)
            endif
         endif
c     shift buffer left
         call unsfdcopy(N-1,z(3),1,z(2),1)
         z(N+1)=u(1)
         call unsfdcopy(N-1,z(N+3),1,z(N+2),1)
         z(2*N+1)=u(2)
c     draw new point
         if(ipar(4).lt.0) then
            call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &           1,1,v,z(1+N),z(1+2*N),dv,dv)
         else
            call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &           1,2,v,z(N),z(2*N),dv,dv)
         endif
         if(int(z(1)).gt.N.and.ipar(6).eq.0) then
c     erase memory
            call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv) 
            xmin=rpar(1)
            xmax=rpar(2)
            ymin=rpar(3)
            ymax=rpar(4)   
            rect(1)=xmin
            rect(2)=ymin
            rect(3)=xmax
            rect(4)=ymax
            call  setscale2d(frect,rect,'nn'//char(0))
            z(1)=0.0d0
         endif
      elseif(flag.eq.4) then
         wid=ipar(1)
         N=ipar(3)
         xmin=rpar(1)
         xmax=rpar(2)
         ymin=rpar(3)
         ymax=rpar(4)
         nax(1)=2
         nax(2)=10
         nax(3)=2
         nax(4)=10
         call sciwin()
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         call dr1('xsetdr'//char(0),'Rec'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
         iwp=7
         if(ipar(iwp).ge.0) then
            call dr1('xset'//char(0),'wpos'//char(0),ipar(iwp),
     $           ipar(iwp+1),v,v,v,v,dv,dv,dv,dv)
         endif
         iwd=9
         if(ipar(iwd).ge.0) then
            call dr1('xset'//char(0),'wdim'//char(0),ipar(iwd),
     $           ipar(iwd+1),v,v,v,v,dv,dv,dv,dv)
         endif
         rect(1)=xmin
         rect(2)=ymin
         rect(3)=xmax
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
         buf='t@ @input and output'
         strf='011'//char(0)
         call dr1('xset'//char(0),'thickness'//char(0),1,v,
     $        v,v,v,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'alufunction'//char(0),3,v,v,v,v,v,
     $        dv,dv,dv,dv)
         call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
         nxname=40
         call getlabel(kfun,buf,nxname)
         if(nxname.gt.39) nxname=39
         buf(nxname+1:nxname+1)=char(0)
         if ((nxname.eq.1.and.buf(1:1).eq.' ').or.(nxname.eq.0)) then
         else
            call dr('xname'//char(0),buf,v,v,v,v,v,v,dv,dv,dv,dv)
         endif
         call dr1('xset'//char(0),'alufunction'//char(0),6,v,v,v,v,v,
     $        dv,dv,dv,dv)
         call sxevents()
c first point drawing
         if(ipar(4).lt.0) then
            call dr1('xset'//char(0),'mark'//char(0),ipar(4),ipar(5),
     $        v,v,v,v,dv,dv,dv,dv)
            call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &           1,1,v,z(2),z(2+N),dv,dv)
         else
            call dr1('xset'//char(0),'thickness'//char(0),ipar(5),v,
     $        v,v,v,v,dv,dv,dv,dv)
            call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(4),
     &           1,2,v,z(2),z(2+N),dv,dv)
         endif
         z(1)=0
         
      elseif(flag.eq.5) then
         wid=ipar(1)
         N=ipar(3)
c     
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         call dr1('xset'//char(0),'alufunction'//char(0),3,v,v,v,v,v,
     $        dv,dv,dv,dv)
      endif

      end
