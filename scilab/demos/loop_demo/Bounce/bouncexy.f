      subroutine bouncexy(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) = win_num
c     ipar(2) = mode : animated =0 fixed=1
c     ipar(3:3+nu-1) = colors of balls
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
      integer n,verb,cur,na,v,wid,nax(4),on
      character*40 strf,buf
      double precision dv
      double precision zz(10)
      double precision frect(4)
      character*(4) logf
      character*4 name

      data frect / 0.00d0,0.00d0,1.00d0,1.00d0/
      data cur/0/,verb/0/
      integer kfun
      common /curblk/ kfun

c     
      if(flag.eq.2) then
         wid=ipar(1)
         N=nu
c     
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
c     erase first point
         if(ipar(2).eq.0) then
            call dr1('xfarcs'//char(0),'v'//char(0),v,ipar(3),N,v,v,
     $           v,z(1),dv,dv,dv);
         endif
c     draw new point
         do i=1,nu
            z(6*(i-1)+1)=u(i)-z(6*(i-1)+3)/2
            z(6*(i-1)+2)=y(i)+z(6*(i-1)+4)/2
         enddo
         call dr1('xfarcs'//char(0),'v'//char(0),v,ipar(3),N,v,v,
     $        v,z(1),dv,dv,dv);
         call dr1('xset'//char(0),'wshow'//char(0),v,v,v,v,v,v,dv,dv,
     $        dv,dv)
      elseif(flag.eq.4) then
         wid=ipar(1)
         N=nu
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
         call dr1('xsetdr'//char(0),'X11'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
         on=1
         call dr1('xset'//char(0),'pixmap'//char(0),on,v,v,v,v,v,dv,
     $        dv,dv,dv)
         rect(1)=xmin
         rect(2)=ymin
         rect(3)=xmax
         rect(4)=ymax
         call dr1('xset'//char(0),'use color'//char(0),1,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'alufunction'//char(0),3,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xclear'//char(0),'v'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
         call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $        dv,dv,dv,dv)
         buf='t@ @input and output'
         strf='030'//char(0)
         call dr1('xset'//char(0),'thickness'//char(0),1,v,
     $        v,v,v,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
         call dr1('xset'//char(0),'wshow'//char(0),v,v,v,v,v,v,dv,dv,
     $        dv,dv)
         zz(1)=xmin
         zz(2)=xmin
         zz(3)=xmax
         zz(4)=xmax
         zz(5)=xmin
         zz(6)=ymax
         zz(7)=ymin
         zz(8)=ymin
         zz(9)=ymax
         zz(10)=ymax
         call dr1('xpolys'//char(0),'v'//char(0),v,v,1,
     &        1,5,v,zz(1),zz(6),dv,dv)
         nxname=40
         call getlabel(kfun,buf,nxname)
         if(nxname.gt.39) nxname=39
         buf(nxname+1:nxname+1)=char(0)
         if ((nxname.eq.1.and.buf(1:1).eq.' ').or.(nxname.eq.0)) then
         else
            call dr('xname'//char(0),buf,v,v,v,v,v,v,dv,dv,dv,dv)
         endif
         if(ipar(2).eq.0) then
            call dr1('xset'//char(0),'alufunction'//char(0),6,v,v,v,
     $           v,v,dv,dv,dv,dv)
         endif
         call sxevents()
c     first point drawing
         if(ipar(2).eq.0) then
            call dr1('xfarcs'//char(0),'v'//char(0),v,ipar(3),N,v,v,
     $           v,z(1),dv,dv,dv);
         endif
      endif
      
      end
      

