      subroutine intgetenv(fname)
c     =============================
c     getenv('varname' [,'rep'])
      character*(*) fname
      logical checkrhs,checklhs,getsmat,checkval,cresmat2,bufstore
      include '../stack.h'
      rhs = max(0,rhs)
      lbuf = 1
      if(.not.checkrhs(fname,1,2)) return
      if(.not.checklhs(fname,1,1)) return
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      if(rhs.eq.2) then
         if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2)) 
     $        return
         top=top-1
      endif
      nc =bsiz
      call getenvc(ierr,buf(lbufi1:lbuff1),buf,nc,0)
      if(ierr.ne.0) then 
         if (rhs.eq.1) then
            buf='Undefined environment variable'
            call error(999)
         else
            call copyobj(fname,top+1,top)             
         endif
      else
         if(.not.cresmat2(fname,top,nc,ilrs)) return
         call cvstr(nc,istk(ilrs),buf,0)
      endif
      return
      end
