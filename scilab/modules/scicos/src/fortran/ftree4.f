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

      subroutine ftree4(vec,nb,nd,nnd,typr,outoin,outoinptr,
c     Copyright INRIA
     $     r1,r2,nr)
c make sure nb > 0
      integer vec(*),nb,nd(*),outoin(*),outoinptr(*)
      integer r1(*),r2(*),fini,typr(*)
c
      nr=0
      do 150 j=1,nb-1
         fini=1
         do 100 i=1,nb
            if(vec(i).gt.-1) then
               if(outoinptr(i+1)-outoinptr(i).ne.0) then
                  do 60 k=outoinptr(i),outoinptr(i+1)-1
                     ii=outoin(k)
                     if(typr(ii).eq.1) then
                        nprt=outoin(k+outoinptr(nb+1)-1)
                        if(nd(nprt+1+(ii-1)*nnd).eq.0) then
                           nr=nr+1
                           r1(nr)=ii
                           r2(nr)=nprt
                           fini=0
                           vec(ii)=0
                           nd(nprt+1+(ii-1)*nnd)=1
                        endif
                     endif
 60               continue
               endif
            endif
 100     continue
         if(fini.eq.1) goto 200
 150  continue
 200  continue
      return
      end

