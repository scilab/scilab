c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) - INRIA - Serge Steer
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine cvwm(xr,xi,nx,m,n,maxc,mode,str,istr)
c!but 
c     cvwm transcrit une matrice reelle en une matrice de chaines
c     de caracteres scilab
c!liste d'appel
c     
c     subroutine cvwm(xr,xi,nx,m,n,maxc,mode,str,istr)
c     
c     double precision x(*)
c     integer nx,m,n,maxc,mode,str(*),istr(m*n+1)
c     
c     xr,xi : tableau contenant les coefficients de la matrice x
c     nx : entier definissant le rangement dans x
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c     representer un nombre
c     mode : si mode=1 representation variable
c     si mode=0 representation d(maxc).(maxc-7)
c     str : tableau contenant apres execution la suite des codes scilab
c     des caracteres.taille >= m*n*maxc
c     istr : tableau donnant la structure de str
c!    
      double precision xr(*),xi(*),ar,ai,eps,a,dlamch
      integer maxc,mode,fl,typ
      integer str(*),istr(*)
      character cw*256,sgn*1
c     
      eps=dlamch('p')
c     
      lstr=1
      istr(1)=1
      lp=-nx
      do 20 k=1,n
         lp=lp+nx
         do 20 l=1,m
c     
c     traitement du coeff (l,k)
            ar=xr(lp+l)
            if(m*n.gt.1.and.abs(ar).lt.eps.and.mode.ne.0) ar=0.0d+0
            ai=xi(lp+l)
            if(m*n.gt.1.and.abs(ai).lt.eps.and.mode.ne.0) ai=0.0d+0
            l1=1
            l0=1
            if (ar.ne.0.0d0) then
c     .        non zero real part
               a=abs(ar)
               typ=1
               if(mode.eq.1) then
                  call fmt(a,maxc,typ,n1,n2)
               else
                  if (isanan(a).eq.1) then
                     typ=-2
                  elseif (a.gt.dlamch('o')) then
                     typ=-1
                  endif
               endif

               if(typ.eq.2) then
                  ifmt=n2+32*n1
               elseif(typ.lt.0) then
                  ifmt=typ
               else
                  ifmt=1
               endif
c
               call formatnumber(a,ifmt,maxc,cw(l1:),fl)
               if (ar.lt.0.0d0)  cw(l1:l1)='-'
               l1=l1+fl
               if(typ.eq.2.and.n2.eq.0) l1=l1-1
               if (ai.ne.0.0d0) then
c     .           non zero imaginary part
                  sgn='+'
                  if(ai.lt.0) sgn='-'
                  ai=abs(ai)
                  cw(l1:l1+3)=sgn//'%i*'
                  l1=l1+4
                  a=abs(ai)
                  typ=1
                  if(mode.eq.1) then
                     call fmt(a,maxc,typ,n1,n2)
                  else
                     if (isanan(a).eq.1) then
                        typ=-2
                     elseif (a.gt.dlamch('o')) then
                        typ=-1
                     endif
                  endif

                  if(typ.eq.2) then
                     ifmt=n2+32*n1
                  elseif(typ.lt.0) then
                     ifmt=typ
                  else
                     ifmt=1
                  endif
                  call formatnumber(a,ifmt,maxc,cw(l1:),fl)
                  if (cw(l1:l1).eq.' ') then
c     .              remove leading blanks
                     cw(l1:l1+fl-2)=cw(l1+1:l1+fl-1)
                     cw(l1+fl-1:l1+fl-1)=' '
                     fl=fl-1
                  endif

                  l11=l1
                  l1=l1+fl
                  if(typ.eq.2.and.n2.eq.0.and.cw(l1-1:l1-1).eq.'.') then
c     .              remove the final dot
                     l1=l1-1
                     cw(l1:l1)=' '
                  endif
                  if (cw(l11:l1-1).eq.'1') then
                     cw(l11-1:l1-1)=' '
                     l1=l11-1
                  endif
               endif
            else
               if (ai.ne.0.0d0) then
c     .        imaginary case

                  if(ai.lt.0) then
                     cw(l1:l1+3)='-%i*'
                     l1=l1+4
                  else
                     cw(l1:l1+2)='%i*'
                     l1=l1+3
                  endif
                  a=abs(ai)
                  typ=1
                  if(mode.eq.1) then
                     call fmt(a,maxc,typ,n1,n2)
                  else
                     if (isanan(a).eq.1) then
                        typ=-2
                     elseif (a.gt.dlamch('o')) then
                        typ=-1
                     endif
                  endif
                  if(typ.eq.2) then
                     ifmt=n2+32*n1
                  elseif(typ.lt.0) then
                     ifmt=typ
                  else
                     ifmt=1
                  endif
                  call formatnumber(a,ifmt,maxc,cw(l1:),fl)
                  if (cw(l1:l1).eq.' ') then
c     .              remove leading blanks
                     cw(l1:l1+fl-2)=cw(l1+1:l1+fl-1)
                     cw(l1+fl-1:l1+fl-1)=' '
                     fl=fl-1
                  endif
                  l11=l1
                  l1=l1+fl
                  if(typ.eq.2.and.n2.eq.0.and.cw(l1-1:l1-1).eq.'.') then
c     .              remove the final dot
                     l1=l1-1
                     cw(l1:l1)=' '
                  endif
                  if (cw(l11:l1-1).eq.'1') then
                     cw(l1-1:l1-1)=' '
                     l1=l11-1
                  endif
               else
c     .           zero case
                  cw(l1:l1)='0'
                  l1=l1+1
               endif
            endif
            call cvstr(l1-l0,str(lstr),cw(l0:l1-1),0)
            lstr=lstr+l1-l0
            istr((k-1)*m+l+1)=lstr
 20      continue
         return
         end
