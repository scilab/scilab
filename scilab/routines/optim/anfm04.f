      subroutine anfm04(q,iq,r,ir,x,w,ipvt,n,m,ind,io)
C     SUBROUTINE ANFM04(Q,IQ,R,IR,X,W,IPVT,N,M,IND,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                               MAYO 1987                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina modifica, (mediante transformaciones de Givens),
C        la factorizacion  QR  de una matriz, cuando a esta se le a¤ade
C        una columna. (Columna que ocupara el ultimo lugar en la matriz)
C        Las rotaciones de Givens utilizadas son del tipo:
C
C                            C      S
C                            S     -C
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        Q      Matriz  de dimension  (IQ,N). Contiene  la matriz  Q  de
C               la factorizacion QR inicial (en sus N primeras filas).
C
C        IQ     Primera dimension de la matriz Q. IQ >= N.
C
C        R      Matriz  de dimension  (IR,M). Guarda la matriz  R  de la
C               factorizacion  QR inicial (en sus N primeras filas y M-1
C               primeras columnas). La parte subdiagonal no es utilizada
C
C        IR     Primera dimension de la matriz R. IR >= N.
C
C        X      Columna que se desea adjuntar a la matriz.
C
C        W      Vector  de  dimension   3*(N-M)+1. Las ultimas (N-M+1)
C               componentes son utilizadas como vector de trabajo.
C
C        IPVT   Vector  entero  de trabajo  de dimension  N-M. Indica el
C               orden  en el que se han  de realizar  las rotaciones  de
C               Givens;  asi, si  IPVT(I) > IPVT(J) , la  transformacion
C               de  Givens  correspondiente  afecta  a  las  coordenadas
C               ( N+1-IPVT(I), N+1-IPVT(J) ).
C
C        N      Numero de filas de la matriz a factorizar.
C
C        M      Numero de columnas de la matriz R.
C
C        IND    Indicador que toma los valores:
C                  J  : X contiene al j-esimo vector de la base canonica
C                 -J  : X contiene al vector j-esimo de la base canonica
C                       cambiado de signo.
C                  0  : X contiene a un vector cualquiera.
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        Q      Recoge  la matriz  ortogonal  correspondiente a la nueva
C               factorizacion QR.
C
C        R      En R se guarda la matriz triangular superior de la nueva
C               factorizacion QR.
C
C        W      Vector  que contiene  los coeficientes que identifican a
C               las matrices  de Givens utilizadas. En las primeras  N-M
C               coordenadas se tienen los elementos  C  de las distintas
C               matrices, y las restantes componentes del vector recogen
C               a los coeficientes  S.
C
C        IND    Indicador que toma los valores:
C                 -1  : La nueva columna  es linealmente  dependiente de
C                       las columnas ya existentes en la matriz.
C                  0  : El proceso se realiza sin problemas.
C
C        Esta subrutina  trabaja  en doble precision  via  una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: dcopy,ddot,dnrm2,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension q(iq,*),r(ir,*),x(*),w(*),ipvt(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(m.lt.1 .or. n.lt.2 .or. m.gt.n .or. iq.lt.n .or. ir.lt.n .or.
     &   ind.lt.-n .or. ind.gt.n) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM04.'
         stop
      end if
C
C     Se inicializan algunas variables de trabajo
C
css   epsmch=d1mach(4)
      epsmch=dlamch('p')
      eps=epsmch**0.75
      eps0=epsmch**0.9
      nm=n-m
      nm1=nm+1
      m1=m-1
      m2=2*nm+1
      m3=m2-m
      n1=n+1
C
C     Se calcula la columna de R correspondiente a la columna a¤adida
C
      k=0
      if(ind.lt.0) then
         k=1
         ind=-ind
      end if
      if(ind.eq.0) then
         do 10 i=1,m1
10       r(i,m)=ddot(n,q(1,i),1,x,1)
         do 20 i=m,n
20       w(m3+i)=ddot(n,q(1,i),1,x,1)
      else
         call dcopy(m1,q(ind,1),iq,r(1,m),1)
         call dcopy(nm1,q(ind,m),iq,w(m2),1)
      end if
      if(k.eq.1) then
         do 30 i=1,m1
30       r(i,m)=-r(i,m)
         do 40 i=m2,m2+nm
40       w(i)=-w(i)
      end if
C
C     Se averigua si la nueva columna es linealmente dependiente de las
C     que ya forman la matriz
C
      rnorma=dnrm2(nm1,w(m2),1)
      if(rnorma.lt.eps0) then
         ind=-1
         return
      end if
C
C     Si la columna es linealmente independiente,se procede a triangular
C     la matriz R y se adapta la matriz Q convenientemente
C
      ind=0
      if(m.eq.n) then
         r(m,m)=w(m2)
         return
      end if
      k1=n1-ipvt(1)
      do 60 i=2,nm1
         i1=i-1
         k2=n1-ipvt(i)
         if(k2 .lt. k1) then
            j=k1
            k1=k2
            k2=j
         end if
         j1=m3+k1
         j2=m3+k2
         t=sqrt(w(j1)*w(j1)+w(j2)*w(j2))
         if(t.lt.eps) then
            w(i1)=1
            w(nm+i1)=0
            do 45 j=1,n
45          q(j,k2)=-q(j,k2)
         else
            c=w(j1)/t
            s=w(j2)/t
            w(j1)=t
            w(j2)=0
            do 50 j=1,n
               a=q(j,k1)
               b=q(j,k2)
               q(j,k1)=a*c+b*s
               q(j,k2)=a*s-b*c
50          continue
            w(i1)=c
            w(nm+i1)=s
         end if
60    continue
      r(m,m)=t
      end
