      function cmplxt(istk,ni)
c!but
c     etant donne le code (ou une portion de code correspondant 
c     a un ensemble d'"operations") d'une macro compilee de scilab 
c     cette  fonction en retourne le nombre d'"operations" au 
c     niveau 1.
c!
c     Copyright INRIA
      integer istk(ni),cmplxt
c     

      parameter (nsiz=6)
      integer op
c     
      if(ni.eq.0) then
         cmplxt=0
         return
      endif
c
      icount=0
      lc=1
 10   continue
      if(lc.le.ni)  then
         op=istk(lc)
         icount=icount+1
c     
         if(op.eq.0) then
c     nop
            icount=icount-1
            lc=lc+istk(lc+1)
            goto 10
         elseif(op.eq.1) then
c     stackp
            lc=lc+1+nsiz
            goto 10
         elseif(op.eq.2) then
c     stackg
            lc=lc+nsiz+3
            goto 10
         elseif(op.eq.3) then
c     string
            lc=lc+2+istk(lc+1)
            goto 10
         elseif(op.eq.4) then
c     defmat
            lc=lc+1
            goto 10
         elseif(op.eq.5) then
c     allops
            lc=lc+4
            goto 10
         elseif(op.eq.6) then
c     num
            lc=lc+3
            goto 10
         elseif(op.eq.7) then
c     for
            nc=istk(lc+1)
            lc=lc+nc+2
            nc=istk(lc)
            lc=lc+1+nsiz+nc
            goto 10
         elseif(op.eq.8.or.op.eq.9) then
c     if - while
            if(istk(lc+1).gt.0) then
c     ancienne version
               lc=lc+2
               nc=istk(lc)+istk(lc+1)+istk(lc+2)
               lc=lc+3+nc
            else
c     nouvelle version               
               nc=-istk(lc+1)
               lc=lc+nc
            endif
            goto 10
         elseif(op.eq.10) then
c     select
            nc=istk(lc+1)
            lc=lc+nc
            goto 10
         elseif(op.eq.11) then
c     try/catch
            nc=istk(lc+1)+istk(lc+2)
            lc=lc+3+nc
            goto 10
         elseif((op.ge.12.and.op.le.15).or.op.eq.28) then
c     pause,break,abort,eol,continue
            lc=lc+1
            goto 10
         elseif(op.eq.16) then 
c     set line number (ignored)
            lc=lc+2
            icount=icount-1
            goto 10
         elseif(op.eq.18) then 
c     mark named argument
            lc=lc+1+nsiz
            goto 10
         elseif(op.eq.19) then 
c     form recursive extraction list
            lc=lc+3
            goto 10
         elseif(op.eq.20) then 
c     exit
            lc=lc+1
            goto 10

         elseif(op.eq.21) then 
c     begrhs (ignored)
            lc=lc+1
            icount=icount-1
            goto 10
         elseif(op.eq.22) then 
c     printmode (ignored)
            lc=lc+2
            icount=icount-1
            goto 10
         elseif(op.eq.23) then 
c     name2var
            lc=lc+1+nsiz
            goto 10
         elseif(op.eq.24) then
c     defnull
            lc=lc+1
            goto 10
         elseif(op.eq.25) then
c     profile
            lc=lc+3
            goto 10
         elseif(op.eq.26) then
c     vector of strings
            n=istk(lc+1)*istk(lc+2)
            nc=istk(lc+4+n)-1
            lc=lc+5+n+nc
            goto 10
         elseif(op.eq.27) then
c     varfun
            lc=lc+3+nsiz
            goto 10
         elseif(op.eq.29) then
c     affectation
            n=istk(lc+1)
            lc=lc+3+n*(nsiz+1)
            goto 10
        elseif(op.eq.30) then
c     skip logical ops (not reported by tradsl)
            lc=lc+3
            icount=icount-1
            goto 10
        elseif(op.eq.31) then
c     comment
            lc=lc+2+istk(lc+1)
            goto 10

         elseif(op.ge.100) then
c     matfns
            lc=lc+4
            goto 10
         elseif(op.ge.99) then
c     matfns
            lc=lc+1
            goto 10
         else
c     code errone
            cmplxt=-1
            write(6,'(''cmplxt : code erronne :'',i10)') op
            return
         endif
      endif
      
      cmplxt=icount
      end
