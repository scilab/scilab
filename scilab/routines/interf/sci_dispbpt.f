      subroutine intdispbpt
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs
c
      rhs=max(rhs,0)
      if(.not.checklhs('dispbpt',1,1)) return
      if(.not.checkrhs('dispbpt',0,0)) return

      if(nmacs.gt.0) then
         do 375 kk=1,nmacs
            call cvname(macnms(1,kk),buf(1:nlgh),1)
            call msgs(27,0)
            do 373 kl=lgptrs(kk),lgptrs(kk+1)-1
               write(buf(1:10),'(5x,i5)') bptlg(kl)
               call basout(io,wte,buf(1:10))
 373        continue
 375     continue
      endif
      top=top+1
      call objvide('dispbpt',top)
      return
      end