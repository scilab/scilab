      subroutine matio
c     ====================================================================
c     
c     file handling and other i/o
c     
c     ====================================================================
c     
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matio '//buf(1:4))
      endif

c
      if(int(rstk(pt)/100).ne.9) goto 01
      goto(10,20,54,240,250,999,200,10) rstk(pt)-901

c     
c     functions/fin
c     
c     load read  getf exec lib   diary save write print mac  deff rat
c     1    2     3    4     5     6    7     8     9    10   11   12
c     file hosts readb writb execstr  disp  getpid getenv read4b write4b
c     13   14    15    16     17      18     19     20     22      23
c     Save Load mgetl fileinfo getio  setenv xls_open xls_read
c     24   25    26     27      28     29       30      31
 01   goto ( 35, 120, 54,10, 130,27, 30,60, 25, 160,
     +       50, 45, 140,170,190,180,20,200,205,210,
     +       170,220,230,240,250,260,270,280,290,300,
     +       310),fin
c     exec
 10   continue
      call intexec
      return

c     execstr
 20   continue
      call intexecstr
      return
c     
c     print
 25   continue
      call intprint
      return
c     
c     diary
 27   continue
      call intdiary
      return
c
c     oldsave
 30   continue
      call oldsave
      return
c     
c     oldload
 35   continue
      call oldload
      return
c     
c     rat
 45   continue
      call intrat
      return

c     deff
 50   continue
      call intdeff
      return
c     
c     getf
 54   continue
      call intgetf
      return
c     
c     write formatte
 60   continue
      call intwrite
      return

c
c     read formatte
 120  continue
      call intread
      return

c     
c     lib
 130  continue
      call intlib
      return
c     
c     filemgr
c     
 140  continue
      call intfile
      return
c     
c     macsym
 160  continue
      return
c     
c     host, unix
 170  continue
      call inthost
      return
c     
c     write binaire
 180  continue
      call intwritb
      return

c     
c     read binaire
 190  continue
      call intreadb
      return
c
c     disp
c     ----
 200  continue
      call intdisp
      return
c     
c     getpid get process id 
 205  continue
      call intgetpid
      return
c
c     getenv
 210  continue
      call intgetenv("getenv")
      return
c
c     read4b      
 220  call intread4b
      return

c     write4b
 230  call intwrite4b
      return
c     
c     save
 240  call intsave
      return
c
c     load
 250  call intload(40,k1)
      return
c
c     mgetl
 260  call intmgetl
      return
c
c     fileinfo
 270  call intfilestat('fileinfo')
      return
c     getio
 280  call intgetio('getio')
      return
c     setenv
 290  call intsetenv('setenv')
      return
c     xls_open
 300  call intopenxls('xls_open')
      return
c     xls_read
 310  call intreadxls('xls_read')
      return


 999  return
      end


c

