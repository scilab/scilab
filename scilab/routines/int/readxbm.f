      subroutine readxbm
c     addition
      include '../stack.h'
      double precision imgptr,cmapptr
      external memused
      integer memused
      logical checkrhs,checklhs,getsmat,checkval
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1     
c
      if(.not.checkrhs('readgif',1,1)) return
      if(.not.checklhs('readgif',2,2)) return

      if(.not.getsmat('readgif',top,top,m,n,1,1,lr,nlr)) return
      if(.not.checkval('readgif',m*n,1)) return
      call cvstr(nlr,istk(lr),buf,1)
      buf(nlr+1:nlr+1)=char(0)

      call readxbmimg(buf,imgptr,m,n,ncol)
      if(m.lt.0) then
         buf='Incorrect xbm file'
         call error(999)
         return
      endif
c
      it=11
      ilp=iadr(lstk(top))
      lp=ilp+4
      lstk(top+1)=sadr(lp+memused(it,m*n))
      top=top+1
      ilcm=iadr(lstk(top))
      lcm=ilcm+4
      lstk(top+1)=sadr(lcm+memused(it,3*ncol))
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ilp)=8
      istk(ilp+1)=m
      istk(ilp+2)=n
      istk(ilp+3)=it
      call getgifpixels(imgptr,istk(lp))
      istk(ilcm)=8
      istk(ilcm+1)=ncol
      istk(ilcm+2)=3
      istk(ilcm+3)=it
      call getgifcmap(imgptr,istk(lcm))
      call deallocategifimg(imgptr)
      return
      end

      
c     
