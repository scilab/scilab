      subroutine basin(ierr,lunit,string,fmt)
c
c     gestion de la standard input
c ================================== ( Inria ) =============
c
c     Copyright INRIA
      include '../stack.h'
c     --- for myback 
      integer lrecl,bkflag
      parameter (lrecl=512) 
      character bckbuf*(lrecl)
      common / keepme / bkflag,bckbuf
c     --- end 
      character string*(*),fmt*(*)
      integer ierr,lunit,status
c
      status = 0
      ierr=0
      if(lunit.eq.rte) then
         string=' '
         call xscion(iflag)
         if (iflag.eq.0) then 
            if (intexmacs().eq.0) then
               call zzledt(string,len(string),lline,status)
            else
               call texmacsin(string,len(string),lline,status)
            endif
         else
            call zzledt1(string,len(string),lline,status)
         endif
         if(status.ne.0) goto 10
         if (lline.eq.0) then
            string(1:1)=' '
            lline=1
	 endif
         if(fmt(1:1).ne.'*'.and.fmt(1:3).ne.'(a)') then
            read(string(1:lline),fmt,end=10,err=20) string
         else
           string(lline+1:)=' '
        endif
      else
         if ( bkflag.eq.1 ) then 
            string = bckbuf(1:lrecl)
            bkflag=0
            return
         endif
         if(fmt(1:1).eq.'*') then
            read(lunit,'(a)',end=10,err=20) string
         else
            read(lunit,fmt,end=10,err=20) string
         endif
      endif
      return
c     ierr=1 : eof, ierr=-1 :read interrupted (callbacks),
 10   ierr=1
      if(status.lt.0) ierr=-1
      return
C     ierr=2:erroneous string read
 20   ierr=2
      return
      end

      subroutine myback(lunit)
C     backspace has erratic behaviour with cygwin32 
c     this routine mimin the same behaviour without 
c     using backspace 
C     this routine is only used in getfun where lrecl 
C     is also set 
      include '../stack.h'
      integer lrecl,bkflag
      parameter (lrecl=512) 
      character bckbuf*(lrecl)
      common / keepme / bkflag,bckbuf
      bckbuf = buf(1:lrecl)
      bkflag=1
      return
      end

