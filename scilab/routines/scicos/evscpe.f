      subroutine evscpe(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Event scope
c     ipar(1) = win_num
c     ipar(2) = 0/1 color flag
c     ipar(3:10) = color
c     ipar(nipar-3:nipar-2) = window position
c     ipar(nipar-1:nipar)= window position
c
c     rpar(1)=periode
c
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision ymin,ymax,per,rect(4),xx(2),yy(2)
      integer verb,cur,na,v,wid,nax(4)
      character*20 strf,buf
      double precision dv
      double precision frect(4)
      character*(4) logf
      character*4 name

      data frect / 0.00d0,0.00d0,1.00d0,1.00d0/
      data cur/0/,verb/0/
c      data yy / 0.00d0,0.80d0/
c     
c     
      call dr1('xgetdr'//char(0),name,v,v,v,v,v,v,
     $     dv,dv,dv,dv)
      if(name(1:3).ne.'Rec') then
         call dr1('xsetdr'//char(0),'Rec'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
      endif

c     
      if(flag.eq.2) then
         per=rpar(1)
         wid=ipar(1)
         if(t/per.ge.z(1)) then
            z(1)=int(t/per)+1.0d0
c     clear window
            nax(1)=2
            nax(2)=10
            nax(3)=2
            nax(4)=10
            call dr1('xget'//char(0),'window'//char(0),verb,cur,na,
     $		v,v,v,
     $          dv,dv,dv,dv)
            if(cur.ne.wid) then
               call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $              dv,dv,dv,dv)
            endif

            call dr1('xclear'//char(0),'v'//char(0),v,v,v,v,v,v,
     $           dv,dv,dv,dv)
            call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
     &           0,0,v,dv,dv,dv,dv)
            call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
            buf='t@ @input and output'
            strf='011'//char(0)
            rect(1)=per*(z(1)-1.0d0)
            rect(2)=0.0d0
            rect(3)=per*z(1)
            rect(4)=1.0d0
            call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &           0,0,v,dv,dv,dv,dv)
            call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
         endif
c
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,
     $		v,v,v,
     $          dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0,
     &        0,0,v,dv,dv,dv,dv)
         buf='xlines'//char(0)
         xx(1)=t
         xx(2)=t 

c  ------------------------------------------------------
         yy(1)=0.0
         yy(2)=0.8
         i=1
         nx=1
 10      if (iand(nevprt,nx) .ne. 0)  then
            call dr1('xpolys'//char(0),'v'//char(0),v,v,
     $           ipar(2+i),1,2,v,xx,yy,dv,dv)
            yy(1)=(yy(1)+yy(2))/2
         endif
         i=i+1
         nx=nx*2
         if(nx.le.nevprt) goto 10
         yy(1)=0.0
c ----------------------------------------------------------
      elseif(flag.eq.4) then
         wid=ipar(1)
         ymin=0.0d0
         ymax=1.0d0
         per=rpar(1)
         nax(1)=2
         nax(2)=10
         nax(3)=2
         nax(4)=10
         n1=int(t)/per
         if(t.le.0.0d0) n1=n1-1
         call sciwin()
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,
     $        v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
c         
         rect(1)=per*(1+n1)
         rect(2)=ymin
         rect(3)=per*(2+n1)
         rect(4)=ymax
         call  setscale2d(frect,rect,'nn'//char(0))
         
         iwp=nipar-3
         if(ipar(iwp).ge.0) then
            call dr1('xset'//char(0),'wpos'//char(0),ipar(iwp),
     $           ipar(iwp+1),v,v,v,v,dv,dv,dv,dv)
         endif
         iwd=nipar-1
         if(ipar(iwd).ge.0) then
            call dr1('xset'//char(0),'wdim'//char(0),ipar(iwd),
     $           ipar(iwd+1),v,v,v,v,dv,dv,dv,dv)
         endif
c

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
         call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
         call sxevents()
         z(1)=0.0d0
      endif

      call dr1('xsetdr'//char(0),name,v,v,v,v,v,v,
     $     dv,dv,dv,dv)
 
      end
