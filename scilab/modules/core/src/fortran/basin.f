c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine basin(ierr,lunit,string,fmt,menusflag)
c
c     gestion de la standard input
c ================================== ( Inria ) =============
c
      include 'stack.h'
c     --- for myback 
      integer lrecl,bkflag
      parameter (lrecl=4096) 
      character bckbuf*(lrecl)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /keepme/
cDEC$ ENDIF            
      common / keepme / bkflag,bckbuf
c     --- end 
      character string*(*),fmt*(*)
      integer ierr,lunit,status
c     to decide what has to be done with dynamic  menus actions  while reading
      integer menusflag
c
      status = 0
      ierr=0
      if(lunit.eq.rte) then
         string=' '
         call xscion(iflag)
         if (iflag.eq.0) then 
            if (intexmacs().eq.0) then
               call eventloopprompt(string, len(string), lline, status)
            else
               call texmacsin(string, len(string), lline, status)
            endif
         else
            call eventloopprompt(string, len(string), lline, status)
         endif
         if(status.ne.0) then
c     .     status>0 : eof, status<0 :read interrupted (callbacks),
            goto 10
         endif
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
c     this routine mimin the same behaviour without 
c     using backspace 
C     this routine is only used in getfun where lrecl 
C     is also set 
      include 'stack.h'
      integer lrecl,bkflag
      parameter (lrecl=4096) 
      character bckbuf*(lrecl)
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /keepme/
cDEC$ ENDIF            
      common / keepme / bkflag,bckbuf
      bckbuf = buf(1:lrecl)
      bkflag=1
      return
      end

