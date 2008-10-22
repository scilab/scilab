c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      logical function isinstring(lin,l)
c     this function returns true if character l in lin belongs to a
c     string syntax
      integer lin(*)
c     lin is a vector of scilab codes
c
      integer dot,blank,quote,right,rparen
      integer qcount,pchar

      data dot/51/,blank/40/,quote/53/,right/55/,rparen/42/
c
      il=0
      qcount=0
      pchar=blank
 27   il=il+1
      if(il.ge.l) then
         if(qcount.eq.0) then
c     .  Not in string
            isinstring=.false.
            return
         else
            qcount=0
c     .     is part of a string
            isinstring=.true.
            return
         endif
      elseif(abs(lin(il)).eq.quote) then
         if(qcount.eq.0) then
            if(pchar.lt.blank.or.pchar.eq.right.or.
     $           pchar.eq.rparen.or.pchar.eq.dot) then
c     .          quote marks a tranposition
            else
               qcount=qcount+1
            endif
         else
c     .     a quote in a string
            if(abs(lin(il+1)).eq.quote) then
               il=il+1
            else
c     .        end of string
               qcount=0
            endif
         endif
      endif
      pchar=lin(il)
      if(pchar.eq.-blank) pchar=blank
      goto 27
      end

