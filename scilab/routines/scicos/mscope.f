      subroutine mscope(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) = win_num
c     ipar(2) = number of subwindows (input ports)
c     ipar(3) = buffer size
c     ipar(4:5) : window position
c     ipar(6:7) : window dimension
c     ipar(8:7+ipar(2)) = input port sizes
c     ipar(8+ipar(2):7+ipar(2)+nu) = line type for ith curve
c     ipar(8+ipar(2)+nu) = acceptance of inherited events

c     rpar(1)=dt
c     rpar(2)=periode
c     rpar(3)=ymin_1
c     rpar(4)=ymax_1
c     ...
c     rpar(2*k+1)=ymin_k
c     rpar(2*k+2)=ymax_k

c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
c
      double precision dt,ymin,ymax,per,rect(4),tsave
      integer i,n,verb,cur,na,v,wid,nax(4)
      character*20 strf,buf
      double precision dv
      double precision frect(4)
      character*(4) logf
      character*4 name
      logical herited
      common /dbcos/ idb
      data cur/0/,verb/0/

c     
      if(idb.eq.1) then
         write(6,'(''MScope t='',e10.3,'' flag='',i1,''window='',i3)') t
     $        ,flag,ipar(1) 
      endif
c     
c     
      call dr1('xgetdr'//char(0),name,v,v,v,v,v,v,
     $     dv,dv,dv,dv)
      if(name(1:3).ne.'Rec') then
         call dr1('xsetdr'//char(0),'Rec'//char(0),v,v,v,v,v,v,
     $        dv,dv,dv,dv)
      endif
c

      wid=ipar(1)
      nwid=ipar(2)
      N=ipar(3)
      per=rpar(2)
      dt=rpar(1)
      if(nipar.lt.8+ipar(2)+nu) then
c     compatibility
         herited=.true.
      else
         herited=ipar(8+ipar(2)+nu).ne.0
      endif
c
      if (flag.le.2) then
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
         do 05 i=1,nu
           z(1+N+(i-1)*N+K)=u(i)
 05      continue
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
         call dr1('xset'//char(0),'use color'//char(0),1,0,0,
     &        0,0,v,dv,dv,dv,dv)
         call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &        0,0,v,dv,dv,dv,dv)

         ilt=8+ipar(2)

         it=0
c     loop on input ports
         if(K.gt.0) then
            do 11 kwid=1,nwid
               buf='xlines'//char(0)
               rect(1)=per*(n1)
               rect(2)=rpar(2*kwid+1)
               rect(3)=per*(1+n1)
               rect(4)=rpar(2*kwid+2)
               frect(1)=0.0d0
               frect(2)=(kwid-1)*(1.0d0/nwid)
               frect(3)=1.0d0
               frect(4)=(1.0d0/nwid)
               call  setscale2d(frect,rect,'nn'//char(0))
c     loop on input port elements
               do 10 i=1,ipar(7+kwid)
                  call dr1('xpolys'//char(0),'v'//char(0),v,v,
     $                 ipar(ilt+it),1,K,v,z(2),z(2+N+it*N),dv,dv)
                  it=it+1
 10            continue
 11         continue
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
            call dr1('xset'//char(0),'use color'//char(0),1,0,0,
     &           0,0,v,dv,dv,dv,dv)
            call dr('xstart'//char(0),'v'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
            buf='t@ @input and output'
            strf='011'//char(0)
            call dr1('xset'//char(0),'dashes'//char(0),0,0,0,
     &           0,0,v,dv,dv,dv,dv)

            do 16 kwid=1,nwid
               rect(1)=per*(1+n1)
               rect(2)=rpar(1+2*kwid)
               rect(3)=per*(2+n1)
               rect(4)=rpar(2*kwid+2)
               frect(1)=0.0d0
               frect(2)=(kwid-1)*(1.0d0/nwid)
               frect(3)=1.0d0
               frect(4)=(1.0d0/nwid)
               call  setscale2d(frect,rect,'nn'//char(0))
               call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
 16         continue
         endif
         t=tsave
c
      elseif(flag.eq.4) then

         nax(1)=2
         nax(2)=10
         nax(3)=2
         nax(4)=10
         n1=int(t)/per
         if(t.le.0.0d0) n1=n1-1
         call sciwin()
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         iwp=4
         if(ipar(iwp).ge.0) then
            call dr1('xset'//char(0),'wpos'//char(0),ipar(iwp),
     $           ipar(iwp+1),v,v,v,v,dv,dv,dv,dv)
         endif
         iwd=6
         if(ipar(iwd).ge.0) then
            call dr1('xset'//char(0),'wdim'//char(0),ipar(iwd),
     $           ipar(iwd+1),v,v,v,v,dv,dv,dv,dv)
         endif
         call dr1('xset'//char(0),'use color'//char(0),1,0,0,
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

         do 20 kwid=1,nwid
            rect(1)=per*(1+n1)
            rect(2)=rpar(1+2*kwid)
            rect(3)=per*(2+n1)
            rect(4)=rpar(2*kwid+2)
            frect(1)=0.0d0
            frect(2)=(kwid-1)*(1.0d0/nwid)
            frect(3)=1.0d0
            frect(4)=(1.0d0/nwid)
            call  setscale2d(frect,rect,'nn'//char(0))
            call plot2d(rect(1),rect(2),1,1,-1,strf,buf,rect,nax)
 20      continue
c
         z(1)=0.0d0
         z(2)=t
         call dset(nu*N,0.0d0,z(3),1)
      elseif(flag.eq.5) then

         K=int(z(1))
         if(K.le.1) return
         call dr1('xget'//char(0),'window'//char(0),verb,cur,na,v,v,v,
     $        dv,dv,dv,dv)
         if(cur.ne.wid) then
            call dr1('xset'//char(0),'window'//char(0),wid,v,v,v,v,v,
     $           dv,dv,dv,dv)
         endif
         call dr1('xset'//char(0),'use color'//char(0),1,0,0,
     &        0,0,v,dv,dv,dv,dv)
c     
         ilt=8+ipar(2)

         it=0
         n1=int(t)/per
         if(t.le.0.0d0) n1=n1-1
c     loop on input ports
         do 35 kwid=1,nwid
            rect(1)=per*(1+n1)
            rect(2)=rpar(1+2*kwid)
            rect(3)=per*(2+n1)
            rect(4)=rpar(2*kwid+2)
            frect(1)=0.0d0
            frect(2)=(kwid-1)*(1.0d0/nwid)
            frect(3)=1.0d0
            frect(4)=(1.0d0/nwid)
            call  setscale2d(frect,rect,'nn'//char(0))
c     loop on input port elements
            do 30 i=1,ipar(7+kwid)
               call dr1('xpolys'//char(0),'v'//char(0),v,v,ipar(ilt
     $              +it),1,K-1,v,z(2),z(2+N+it*N),dv,dv)
               it=it+1
 30         continue
 35      continue
      endif

      call dr1('xsetdr'//char(0),name,v,v,v,v,v,v,
     $     dv,dv,dv,dv)
 
      end
 
