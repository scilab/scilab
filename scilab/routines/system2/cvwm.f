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
c     Copyright INRIA
      double precision xr(*),xi(*),ar,ai,eps,dlamch
      integer maxc,mode,fl,typ
      integer str(*),istr(*)
      character cw*256,sgn*1
      character*10 form(2)
c     
      eps=dlamch('p')
      write(form(1),130) maxc,maxc-7
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
               typ=1
               if(mode.eq.1) call fmt(abs(ar),maxc,typ,n1,n2)

               if(typ.eq.1) then
                  fl=maxc
                  write(cw(l1:l1+fl-1),form(1)) ar
               elseif(typ.eq.-1) then
                  if(ar.gt.0) then
                     fl=3
                     cw(l1:l1+fl-1)='Inf'
                  else
                     fl=4
                     cw(l1:l1+fl-1)='-Inf'
                  endif
                  n2=1
               elseif(typ.eq.-2) then
                  fl=3
                  cw(l1:l1+fl-1)='Nan'
                  n2=1
               else
                  fl=n1
                  if(ar.lt.0.0d0) fl=fl+1
                  write(form(2),120) fl,n2
                  write(cw(l1:l1+fl-1),form(2)) ar
               endif
               if (cw(l1:l1).eq.' ') then
                  cw(l1:l1+fl-2)=cw(l1+1:l1+fl-1)
                  cw(l1+fl-1:l1+fl-1)=' '
                  fl=fl-1
               endif
               l1=l1+fl
               if(n2.eq.0) l1=l1-1
               if (ai.ne.0.0d0) then
c     .           non zero imaginary part
                  sgn='+'
                  if(ai.lt.0) sgn='-'
                  ai=abs(ai)
                  cw(l1:l1+3)=sgn//'%i*'
                  l1=l1+4
                  typ=1
                  if(mode.eq.1) call fmt(abs(ai),maxc,typ,n1,n2)
                  if(typ.eq.1) then
                     fl=maxc
                     write(cw(l1:l1+fl-1),form(1)) ai
                  elseif(typ.eq.-1) then
                     fl=3
                     cw(l1:l1+fl-1)='Inf'
                     n2=1
                  elseif(typ.eq.-2) then
                     fl=3
                     cw(l1:l1+fl-1)='Nan'
                     n2=1
                  else
                     fl=n1
                     write(form(2),120) fl,n2
                     write(cw(l1:l1+fl-1),form(2)) ai
                  endif
                  l11=l1
                  if (cw(l1:l1).eq.' ') then
                     cw(l1:l1+fl-2)=cw(l1+1:l1+fl-1)
                     cw(l1+fl-1:l1+fl-1)=' '
                     fl=fl-1
                  endif
                  l1=l1+fl
                  if(n2.eq.0) then
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
                     ai=abs(ai)
                     cw(l1:l1+3)='-%i*'
                     l1=l1+4
                  else
                     cw(l1:l1+2)='%i*'
                     l1=l1+3
                  endif
                  typ=1
                  if(mode.eq.1) call fmt(abs(ai),maxc,typ,n1,n2)
                  if(typ.eq.1) then
                     fl=maxc
                     write(cw(l1:l1+fl-1),form(1)) ai
                  elseif(typ.eq.-1) then
                     fl=3
                     cw(l1:l1+fl-1)='Inf'
                     n2=1
                  elseif(typ.eq.-2) then
                     fl=3
                     cw(l1:l1+fl-1)='Nan'
                     n2=1
                  else
                     fl=n1
                     write(form(2),120) fl,n2
                     write(cw(l1:l1+fl-1),form(2)) abs(ai)
                  endif
                  if (cw(l1:l1).eq.' ') then
                     cw(l1:l1+fl-2)=cw(l1+1:l1+fl-1)
                     cw(l1+fl-1:l1+fl-1)=' '
                     fl=fl-1
                  endif
                  l11=l1
                  l1=l1+fl
                  if(n2.eq.0) then
                     l1=l1-1
                     cw(l1:l1)=' '
                  endif
                  if (cw(l11:l1-1).eq.'1') then
                     cw(l11-1:l1-1)=' '
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
 120     format('(f',i2,'.',i2,')')
 130     format('(1pd',i2,'.',i2,')')
         end
