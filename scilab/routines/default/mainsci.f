      program mainsci
c     Copyright INRIA
      character*40 arg,display
      character*256 pname, path
      integer nos,now,idisp,mem
      integer p1,p2
      integer lpname, ldisp
      common /comnos/nos,mem
      common /starfi/path
      data now,idisp /0,0/
      data lpname,ldisp /256,40/
      nos=0
      mem=0
      path=' '
      nargs = iargc()
      call fgetarg(0,pname)
      call nofpex()
      i = 0
 10   continue
      i=i+1
      if (i.gt.nargs) goto 11
      call fgetarg(i,arg)
      if (arg.eq.'-ns') then 
         nos=1
      elseif (arg.eq.'-nw') then 
         now=1
      elseif (arg.eq.'-display') then 
         i=i+1
         call fgetarg(i,display)
         idisp=1
      elseif (arg.eq.'-pipes') then
         i=i+1
         call fgetarg(i,arg)
         read(arg,'(i5)') p1
         i=i+1
         call fgetarg(i,arg)
         read(arg,'(i5)') p2
         call initcom(p1,p2)
      elseif (arg.eq.'-mem') then
         i=i+1
         arg=' '
         call fgetarg(i,arg)
         read(arg,'(i10)') mem
      elseif (arg.eq.'-f') then
         i=i+1
         call fgetarg(i,path)         
      endif
      goto 10
 11   continue
      mem=max(mem,180000)
C     test enleve: jpc 18/01/2001 if ( nos.eq.0 ) 
      call settmpdir()
      if(now.eq.1) then
         call scilab(nos)
      else
         call winsci(pname,nos,idisp,display,lpname,ldisp)
      endif
      end
