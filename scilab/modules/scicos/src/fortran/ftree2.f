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

      subroutine ftree2(vec,nb,deput,outoin,outoinptr,
c     Copyright INRIA
     $     ord,nord,ok)
c make sure nb > 0
      integer vec(*),nb,deput(*),outoin(*),outoinptr(*)
      integer ord(*),nord,ok,fini
c
      ok=1
      do 150 j=1,nb+2
         fini=1
         do 100 i=1,nb
            if(vec(i).eq.j-1) then
               if(j.eq.nb+2) then
                  ok=0
                  nord=0
                  return
               endif
               if(outoinptr(i+1)-outoinptr(i).ne.0) then
                  do 60 k=outoinptr(i),outoinptr(i+1)-1
                     ii=outoin(k)
                     if((vec(ii).gt.-1).AND.(deput(ii).eq.1)
     $                    ) then
                        fini=0
                        vec(ii)=j
                     endif
 60               continue
               endif
            endif
 100     continue
         if(fini.eq.1) goto 200
 150  continue
 200  continue
      do 202 m=1,nb
         vec(m)=-vec(m)
 202  continue 
      call isort(vec,nb,ord)
      do 300 m=1,nb
      if(vec(m).lt.1) then
         if(m.eq.1) then
            nord=nb
            return
         else
            nord=nb-m+1
            do 250 mm=1,nord
               ord(mm)=ord(mm+nb-nord)
 250        continue
            return
         endif
      endif
 300  continue
      nord=0
      return
      end
