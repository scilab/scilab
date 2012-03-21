c     Scicos
c     
c     Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
c     
c     This program is free software; you can redistribute it and/or modify
c     it under the terms of the GNU General Public License as published by
c     the Free Software Foundation; either version 2 of the License, or
c     (at your option) any later version.
c     
c     This program is distributed in the hope that it will be useful,
c     but WITHOUT ANY WARRANTY; without even the implied warranty of
c     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c     GNU General Public License for more details.
c     
c     You should have received a copy of the GNU General Public License
c     along with this program; if not, write to the Free Software
c     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
c     
c     See the file ./license.txt
c     

      subroutine ftree3(vec,nb,deput,typl,bexe,boptr,
c     Copyright INRIA
     $     blnk,blptr,kk,ord,nord,ok)
c     make sure nb > 0
      integer vec(*),nb,deput(*),typl(*),bexe(*),boptr(*)
      integer blnk(*),blptr(*),ord(*),nord,ok,kk(*),fini
c     

      ok=1
      do 10 i=1,nb
         if ( (vec(i).eq.0).and.(typl(i).eq.1) )   vec(i)=1
 10   continue
      
      do 150 j=1,nb+2 
         fini=1
         if (j.eq.(nb+2)) then
            ok=0
            nord=0
            return
         endif      
         
         do 100 i=1,nb  
            if ((vec(i).gt.-1).AND.(typl(i).ne.-1)) then
               if (typl(i).eq.1) then 
                  nkk=boptr(i+1)-boptr(i)
                  if (nkk.ne.0) then
                     do 50 m=1,nkk
                        ii=bexe(m+boptr(i)-1)
                        if (typl(ii).eq.1) then
                           if (vec(ii).lt.(vec(i)+2)) then
                              fini=0
                              vec(ii)=vec(i)+2
                           endif
                        else
                           if (vec(ii).lt.(vec(i)+1)) then
                              fini=0
                              vec(ii)=vec(i)+1
                           endif 
                        endif
 50                  continue
                  endif
               else
                  nkk=blptr(i+1)-blptr(i)
                  if (nkk.ne.0) then
                     do 60 m=1,nkk
                        ii=blnk(m+blptr(i)-1)
                        if ((vec(ii).gt.-1).AND.((deput(ii).eq.1)
     $                       .OR.(typl(ii).eq.1))) then
                           if (vec(ii).lt.vec(i)) then
                              fini=0
                              vec(ii)=vec(i)
                           endif
                        endif
 60                  continue
                  endif
               endif
            endif
 100     continue
         if (fini.eq.1)     goto 200 
c          write(6,'(      "vec"  ,e10.3,"flag=",i1           )') t,flag
 150  continue
C     loop J finished     
 200  continue
      
      do 202 m=1,nb
         vec(m)=-vec(m)
 202  continue 
      
      call isort(vec,nb,ord)
      do 300 m=1,nb
         if (vec(m).lt.1) then
            if (m.eq.1) then
               nord=nb
               return
            else
               nord=nb-m+1
               do 250 mm=1,nord
                  ord(mm)=ord(mm+nb-nord)
 250           continue
               return
            endif
         endif
 300  continue
      nord=0
      return
      end
      
