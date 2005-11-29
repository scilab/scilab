      subroutine anfm01(q,iq,r,ir,x,w,n,m,ind,io)
C     SUBROUTINE ANFM01(Q,IQ,R,IR,X,W,N,M,IND,IO)
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
C                             FEBRERO 1987                             *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina modifica la factorizacion  QR  de una matriz,
C        cuando a esta se le a¤ade una columna. (Esta columna ocupara
C        el ultimo lugar en la matriz).
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        Q      Matriz de dimension (IQ,N). Contiene la matriz "q" de
C               la factorizacion QR inicial (en sus N primeras filas).
C
C        IQ     Primera dimension de la matriz Q. IQ >= N.
C
C        R      Matriz de dimension (IR,M). Guarda la matriz "r" de la
C               factorizacion QR inicial (en sus N primeras filas y M-1
C               primeras columnas). La parte subdiagonal no es utilizada
C
C        IR     Primera dimension de la matriz R. IR >= N.
C
C        X      Columna que se desea adjuntar a la matriz. Se suministra
C               cuando IND=0.
C
C        W      Vector de trabajo de dimension N-M+1.
C
C        N      Numero de filas de la matriz a factorizar.
C
C        M      Numero de columnas de la matriz R.
C
C        IND    Indicador que toma los valores:
C                  J  : La nueva columna es el j-esimo vector de la base
C                       canonica.
C                 -J  : La nueva columna es el j-esimo vector de la base
C                       canonica cambiado de signo.
C                  0  : La nueva columna, (contenida en X), es un vector
C                       cualquiera.
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        Q      Recoge la matriz ortogonal correspondiente a la nueva
C               factorizacion QR.
C
C        R      En R se guarda la matriz triangular superior de la nueva
C               factorizacion QR.
C
C        IND    Indicador que toma los valores:
C                 -1  : La nueva columna es linealmente dependiente de
C                       las columnas ya existentes en la matriz.
C                  0  : El proceso se realiza sin problemas.
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: daxpy,dcopy,ddot,dnrm2,dscal,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: mod,sign,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension q(iq,*),r(ir,*),x(*),w(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(m.lt.1 .or. n.lt.2 .or. m.gt.n .or. iq.lt.n .or. ir.lt.n .or.
     &   ind.lt.-n .or. ind.gt.n) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM01.'
         stop
      end if
C
C     Se calcula la columna de R correspondiente a la columna a¤adida
C
      m1=m-1
      nm=n-m1
      k=0
      if(ind.lt.0) then
         k=1
         ind=-ind
      end if
      if(ind.eq.0) then
         do 10 i=1,m1
10       r(i,m)=ddot(n,q(1,i),1,x,1)
         do 20 i=m,n
20       w(i-m1)=ddot(n,q(1,i),1,x,1)
      else
         call dcopy(m1,q(ind,1),iq,r(1,m),1)
         call dcopy(nm,q(ind,m),iq,w,1)
      end if
      if(k.eq.1) then
         do 30 i=1,m1
30       r(i,m)=-r(i,m)
         do 40 i=1,nm
40       w(i)=-w(i)
      end if
C
C     Se averigua si la nueva columna es linealmente dependiente de las
C     que ya forman la matriz
C
      rnorma=dnrm2(nm,w,1)
css      eps=d1mach(4)**0.9
      eps=dlamch('p')**0.9
      
      if(rnorma.lt.eps) then
         ind=-1
         return
      end if
C
C     Si la columna es linealmente independiente,se procede a triangular
C     la matriz R y se adapta la matriz Q convenientemente
C
      ind=0
      if(m.eq.n) then
         r(m,m)=w(1)
         return
      end if
      if(w(1).ne.0.d0) rnorma=sign(rnorma,w(1))
      w(1)=rnorma+w(1)
      s=sqrt(w(1)*rnorma)
      s=1/s
      call dscal(nm,s,w,1)
      do 50 j=1,n
         t=-ddot(nm,w,1,q(j,m),iq)
         call daxpy(nm,t,w,1,q(j,m),iq)
50    continue
      r(m,m)=-rnorma
      end
