      subroutine readf(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     write read from a binary or formatted file
      include '../stack.h'
c     ipar(1) = lfil : file name length
c     ipar(2) = lfmt : format length (0) if binary file
c     ipar(3) = ievt  : 1 if each data have a an associated time
c     ipar(4) = N : buffer length
c     ipar(5:4+lfil) = character codes for file name
c     ipar(5+lfil:4+lfil+lfmt) = character codes for format if any
c     ipar(5+lfil+lfmt:5+lfil+lfmt+ny+ievt) = reading mask

c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c
      integer n
      integer mode(2)
c
      if(flag.eq.1) then
c     discrete state
         n=ipar(4)
         k=int(z(1))
         ievt=ipar(3)
         kmax=int(z(2))
         lunit=int(z(3))
         if(k+1.gt.kmax.and.kmax.eq.n) then
c     output
            call dcopy(ny,z(3+n*ievt+k),n,y,1)
c     .     read a new buffer
            no=(nz-3)/N
            call bfrdr(lunit,ipar,z(4),no,kmax,ierr)
            if(ierr.ne.0) goto 110
            z(1)=1.0d0
            z(2)=kmax
         elseif(k.lt.kmax) then
c     output
            call dcopy(ny,z(3+n*ievt+k),n,y,1)
            z(1)=z(1)+1.0d0
         endif
c
      elseif(flag.eq.3) then
         n=ipar(4)
         k=int(z(1))
         kmax=int(z(2))
         if(k.gt.kmax.and.kmax.lt.n) then
            tvec(1)=t-1.0d0
         else
            tvec(1)=z(3+k)
         endif
      elseif(flag.eq.4) then
c     file opening
         lfil=ipar(1)
         ievt=ipar(3)
         N=ipar(4)
         call cvstr(lfil,ipar(5),buf,1)
         lfmt=ipar(2)
         lunit=0
         if(lfmt.gt.0) then
            mode(1)=001
            mode(2)=0
            call clunit(lunit,buf(1:lfil),mode)
            if(err.gt.0) goto 100
         else
            mode(1)=101
            mode(2)=0
            call clunit(lunit,buf(1:lfil),mode)
            if(err.gt.0) goto 100
         endif
         z(3)=lunit
c     buffer initialisation
         no=(nz-3)/N
         call bfrdr(lunit,ipar,z(4),no,kmax,ierr)
         if(ierr.ne.0) goto 110
         z(1)=1.0d0
         z(2)=kmax
      elseif(flag.eq.5) then
         lfil=ipar(1)
         N=ipar(4)
         K=int(z(1))
         lunit=int(z(3))
         if(lunit.eq.0) then
            return
         endif
         call clunit(-lunit,buf(1:lfil),mode)
         if(err.gt.0) goto 100
         z(3)=0.0d0
      endif
      return
 100  continue
      err=0
      lfil=ipar(1)
      call basout(io,wte,'File '//buf(1:lfil)//' Cannot be opened')
      flag=-1
      return
 110  continue
      lfil=ipar(1)
      call cvstr(lfil,ipar(5),buf,1)
      call clunit(-lunit,buf(1:lfil),mode)
      call basout(io,wte,'Read error on file '//buf(1:lfil))
      flag=-1
      return
      end


      subroutine bfrdr(lunit,ipar,z,no,kmax,ierr)
c     buffered and masked read
      include '../stack.h'
      integer lunit,ipar(*),ierr
      double precision z(*)
      double precision tmp(100)
c
      ievt=ipar(3)
      N=ipar(4)
c      no=(nz-3)/N
c     maximum number of value to read
      imask=5+ipar(1)+ipar(2)
      if(ievt.eq.0) imask=imask+1
      mm=0
      do 10 i=0,no-1
         mm=max(mm,ipar(imask+i))
 10   continue
c
      lfmt=ipar(2)
      kmax=0
      if(lfmt.eq.0) then
c     unformatted read
         do 12 i=1,N
            read(lunit,err=100,end=20) (tmp(j),j=1,mm)
            do 11 j=0,no-1
               z(j*N+i)=tmp(ipar(imask+j))
 11         continue
            kmax=kmax+1
 12      continue
      else
c     formatted read
         call cvstr(ipar(2),ipar(5+ipar(1)),buf,1)
         do 14 i=1,N
            read(lunit,buf(1:lfmt),err=100,end=20) (tmp(j),j=1,mm)
            do 13 j=0,no-1
               z(j*N+i)=tmp(ipar(imask+j))
 13         continue
            kmax=kmax+1
 14      continue
      endif
 20   continue
      ierr=0
      return
 100  ierr=1 
      return
      end
