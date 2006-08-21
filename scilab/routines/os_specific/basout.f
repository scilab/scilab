c     ====================================           
      subroutine basout(io,lunit,string)
c     gestion des sorties sur le "standard output" de scilab.
c     Copyright INRIA
c     ====================================           
      include '../stack.h'
      character*(*) string
      character ch*1
c
      if(lunit.eq.wte) then
c        Sortie sur la sortie standard
         call xscion(iflag)
         io=0
         if(lct(1).eq.-1) return
         if(lct(2).gt.0) then
c           Gestion de la pagination
            if(lct(1)+3.gt.lct(2)) then
c              nombre maxi de ligne atteint,gestion du more
               lct(1)=0
               if (iflag.eq.0) then 
c                 scilab n'a pas de  fenetre propre
                  call writewtemore()
                  ch=' '
                  call readrtechar(ch)
                  if(ch.ne.' ') ich=1
               else
c                 scilab a une  fenetre  en propre
                  call xscimore(ich)
               endif
               if(ich.eq.1) then
                  lct(1)=-1
                  io=-1
                  return
               endif
            else
               lct(1)=lct(1)+1
            endif
         endif
         if(iflag.eq.0) then
            call writelunitstring(lunit,string)
         else
            call xscistring(string,len(string))
         endif
c        write to diary file if required
         call diary(string,len(string))
      else
c        sortie sur fichier
         if (lunit.eq.wio)  then
            call diary(string,len(string))
         else
            call writelunitstring(lunit,string)
         endif
      endif
      end
c     ====================================
      subroutine readrtechar(onechar)
      include '../stack.h'
      character onechar*1
      read(rte,'(a1)') onechar
      end
c     ====================================     
      subroutine writelunitstring(lunit,string)
      character*(*) string
      write(lunit,'(a)') string      
      end
c     ====================================           
      subroutine writewtemore()
      include '../stack.h'
      write(wte, '('' more ?'',$)')
      end
c     ====================================           
