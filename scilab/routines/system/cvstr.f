      subroutine cvstr(n,line,str,job)
c     ------------------------------------------------
c     converts from ascii to Scilab internal coding
c     call cvstr(n,line,str,job)
c     n: integer, length of the string to be converted entier
c     line: integer array (where Scilab coded string are stored )
c     string: string 
c     job: integer flag 
c         1: code-->ascii
c         0: ascii-->code
c     Copyright INRIA/ENPC
c     ------------------------------------------------
      integer line(*)
      character str*(*)
      if(job.eq.0) then 
         call asciitocode(n,line,str,1)
      else 
         call codetoascii(n,line,str)
      endif
      return
      end

      subroutine cvstr1(n,line,str,job)
c     ------------------------------------------------
c     very similar to cvstr but the conversion 
c     ascii->code is performed from end to the begining
c     ------------------------------------------------
      integer line(*)
      character str*(*)
      if(job.eq.0) then 
         call asciitocode(n,line,str,-1)
      else 
         call codetoascii(n,line,str)
      endif
      return
      end



      subroutine codetoascii(n,line,str)
c     ---------------------------------------------
c     converts from Scilab internal coding to ascii
c     Copyright INRIA/ENPC
c     ---------------------------------------------
      include '../stack.h'
      integer eol
c     
      integer line(*)
      character str*(*),mc*1
      data eol/99/
c     conversion code ->ascii
      do 30 j=1,n
         m=line(j)
         if(m.eq.eol) then
            goto 10
         elseif(abs(m).gt.csiz) then
            if(m.gt.eol) then
               str(j:j)=char(m-(eol+1)) 
            else
               str(j:j)='!'
            endif
         elseif(m.lt.0) then
            str(j:j)=alfb(abs(m)+1)
         else
            str(j:j)=alfa(m+1)
         endif
         goto 30
 10      str(j:j)='!'
 30   continue
      return
      end

      subroutine asciitocode(n,line,str,flag)
c     ---------------------------------------------
c     converts from ascii to  Scilab internal coding
c     flag can be 1 or -1 and this is used when the 
c     conversion is made with line and str stored at overlapping 
c     memory zone 
c     Copyright INRIA/ENPC
c     ---------------------------------------------
      include '../stack.h'
      integer getcode
      integer flag
      integer line(*)
      character str*(*)
      if ( flag.eq.1) then 
         do 50 j=1,n
            line(j)= getcode(str(j:j))
 50      continue
      else
         do 51 j=n,1,-1
            line(j)= getcode(str(j:j))
 51      continue
      endif
      return 
      end

      integer function getcode(mc)
c     ---------------------------------------------
c     converts one ascii to Scilab internal code
c     Copyright INRIA/ENPC
c     ---------------------------------------------
      include '../stack.h'
      integer eol,k,blank
      character mc*1
      data eol/99/,blank/40/
      do 45 k=1,csiz
         if(mc.eq.alfa(k)) then
            getcode = k-1
            return 
         elseif(mc.eq.alfb(k)) then
            getcode =-(k-1)
            return 
         endif
 45   continue
c     special characters 
c     -----------------
      if(ichar(mc).eq.0) then
c     0-> eol 
         getcode=eol
      elseif(ichar(mc).eq.9) then
c     \t -> ' '
         getcode=blank+1
      elseif(ichar(mc).eq.10) then
c     \n remplace par un eol
         getcode=eol
      else
         getcode=ichar(mc)+eol+1
      endif
      return 
      end
      

      

