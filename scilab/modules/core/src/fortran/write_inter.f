c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine writedouble(form, dat, m, n, ierr)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n,ierr
      character form*(*)

      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 65 i=1,m
          write(buf,form,err=20) (dat(j*m+i),j=0, n-1)
          lb1=lch
 66       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 66

          io = 0
          call basout(io, 6, buf(1:lb1))
 65   continue
      return

20    ierr = 2
      return

      end

      subroutine writeint(form, dat, m, n, ierr)
c         write(buf(ib:),buf(1:nc),err=139)
c     &       (stk(li+j*m),j=0,n-1)
      parameter (lch=4096)
      character buf*(lch)
      integer dat(*)
      integer m,n,ierr
      character form*(*)

      !write (*, '(f)') dat(1)
      !write (*, '(f)') dat(2)
      do 67 i=1,m
          write(buf,form,err=20) (dat(j*m+i),j=0, n-1)
          lb1=lch
 68       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 68

          io = 0
          call basout(io, 6, buf(1:lb1))
 67   continue
      return

20    ierr = 2
      return

      end


      subroutine writestring(form, dat, ierr)

      parameter (lch=4096)
      character buf*(lch)
      character form*(*)
      character dat*(*)
      integer ierr


      write(buf,form,err=20) dat
      lb1=lch
 69   lb1=lb1-1
      if(buf(lb1:lb1).eq.' ') goto 69

      call basout(io, 6, buf(1:lb1))
      return

20    ierr = 2
      return

      end

      subroutine writestringfile(ID, form, dat, ierr)

      parameter (lch=4096)
      character buf*(lch)
      integer ID,ierr
      character form*(*)
      character dat*(*)

      do 99 i=1,len(dat),lch
          if(len(dat) < (i+lch-1)) then
              write(ID,form,err=20) dat(i:len(dat))
          else
              write(ID,form,err=20) dat(i:(i+lch-1))
          end if

99    continue
      return

20    ierr = 2
      return

      end

      subroutine writedoublefile(ID, form, dat,  m, n, ierr)

      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n,ierr
      character form*(*)


      do 71 i=1,m
          write(ID,form,err=20) (dat(j*m+i),j=0, n-1)

 71   continue
      return

20    ierr = 2
      return

      end

      subroutine writeintfile(ID, form, dat, m, n, ierr)

      parameter (lch=4096)
      integer dat(*)
      integer m,n,ierr
      character form*(*)

      do 72 i=1,m
          write(ID,form,err=20) (dat(j*m+i),j=0, n-1)

 72   continue
      return

20    ierr = 2
      return

      end

      subroutine writedoubleszsc(form, dat, szsc, m, n, ierr)

      parameter (lch=4096)
      character buf*(lch)
      character buf2*(lch)
      double precision dat(*)
      integer m,n,szsc,ierr
      character form*(*)

      do 73 i=1,m
          write(buf,form,err=20) (dat(j*m+i),j=0, n-1)
          lb1=lch
 74       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 74

      do 75 j=1,lb1,szsc
          if(lb1 < (j+szsr-1)) then
              write(buf2,*) buf(j:lb1)

          else
              write(buf2,*) buf(j:(j+szsc-1))
          end if

        io = 0
        call basout(io, 6, buf2(1:(j+szsc-1)))
 75   continue
 73   continue
      return

20    ierr = 2
      return

      end

      subroutine writedoubleszscfile(ID, dat, szsc,  m, n,ierr)

      parameter (lch=4096)
      character buf*(lch)
      double precision dat(*)
      integer m,n,szsc,ierr

      do 76 i=1,m
          write(ID,*,err=20) (dat(j*m+i),j=0, n-1)
 76   continue
      return

20    ierr = 2
      return

      end

      subroutine writeintszsc(form, dat, szsc, m, n, ierr)

      parameter (lch=4096)
      character buf*(lch)
      character buf2*(lch)
      integer*4 dat(*)
      integer*4 m,n,szsc,ierr
      character form*(*)

      do 79 i=1,m
          write(buf,form,err=20) (dat(j*m+i),j=0, n-1)
          lb1=lch
 80       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 80

      do 81 j=1,lb1,szsc
          if(lb1 < (j+szsr-1)) then
              write(buf2,*) buf(j:lb1)

          else
              write(buf2,*) buf(j:(j+szsc-1))
          end if

        io = 0
        call basout(io, 6, buf2(1:(j+szsc-1)))
 81   continue
 79   continue
      return

20    ierr = 2
      return

      end

      subroutine writeintszscfile(ID, form, dat, szsc,  m, n, ierr)

      parameter (lch=4096)
      character buf*(lch)
      integer dat(*)
      integer m,n,szsc,ierr
      character form*(*)

      do 82 i=1,m
          write(buf,form,err=20) (dat(j*m+i),j=0, n-1)
          lb1=lch
 83       lb1=lb1-1
          if(buf(lb1:lb1).eq.' ') goto 83

      do 84 j=1,lb1,szsc
          if(lb1 < (j+szsc-1)) then
              write(ID,*) buf(j:lb1)

          else
              write(ID,*) buf(j:(j+szsc-1))
          end if

 84   continue
 82   continue
      return

20    ierr = 2
      return

      end
