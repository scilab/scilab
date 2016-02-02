c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine readdoublefile(ID, dat, m, n, ierr)

      double precision dat(*)
      integer m,n
      integer ierr

      do 70 i=1,m
          read(ID,*,end=10,err=20) (dat(j*m+i),j=0, n-1)

 70   continue
      return

10      ierr = 1
      return
20      ierr = 2
      return

      end

      subroutine readdoublefileform(ID, form, dat,  m, n, ierr)

      double precision dat(*)
      integer m,n
      character form*(*)
      integer ierr

      do 71 i=1,m
          read(ID,form,end=10,err=20) (dat(j*m+i),j=0, n-1)

 71   continue
      return

10      ierr = 1
      return
20      ierr = 2
      return

      end

      subroutine readdoublelinefile(ID, dat, n, ierr)

      double precision dat(*)
      integer n
      integer ierr

      read(ID,*,end=10,err=20) (dat(j), j=1, n)

      return

10      ierr = 1
      return
20      ierr = 2
      return

      end

      subroutine readdoublelinefileform(ID, form, dat, n, ierr)

      double precision dat(*)
      integer n
      character form*(*)
      integer ierr

      read(ID,form,end=10,err=20) (dat(j), j=1, n)

      return

10      ierr = 1
      return
20      ierr = 2
      return

      end
      
      subroutine readstringfile(ID, form, dat, siz, ierr)

      parameter (lch=4096)
      character dat*(lch)
      character form*(*)
      integer siz,ierr

      read(ID,form,end=10,err=20) dat
      mn = lch
127   mn=mn-1
      if(dat(mn:mn).eq.' ') goto 127
            siz=max(1,mn)
      return

10      ierr = 1
      return
20      ierr = 2
      return


      end

      subroutine readstring(form, dat, siz, ierr)

      parameter (lch=4096)
      character string*(lch)
      character dat*(lch)

      character form*(*)
      integer siz,ierr

      string=' '
      call scilabread(string,len(string))
      read(string,form,end=10,err=20) dat
      mn = lch
128   mn=mn-1
      if(dat(mn:mn).eq.' ') goto 128
            siz=max(1,mn)
      return

10      ierr = 1
      return
20      ierr = 2
      return

      end


      subroutine readintfileform(ID, form, dat,  m, n, ierr)

      integer dat(*)
      integer m,n
      character form*(*)
      integer ierr

      do 72 i=1,m
          read(ID,form,end=10,err=20) (dat(j*m+i),j=0, n-1)

 72   continue
      return

10      ierr = 1
      return
20      ierr = 2
      return

      end

      subroutine readintlinefileform(ID, form, dat, n, ierr)

      integer dat(*)
      integer n
      character form*(*)
      integer ierr

      read(ID,form,end=10,err=20) (dat(j), j=1, n)

      return

10      ierr = 1
      return
20      ierr = 2
      return

      end
