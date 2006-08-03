      subroutine anfm03(h,ih,r,ir,z,iz,w,ipvt,n,m,ind,modo,io)  
C     SUBROUTINE ANFM03(H,IH,R,IR,Z,IZ,W,IPVT,N,M,IND,MODO,IO)
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
C                              ENERO 1988                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina calcula  la factorizacion de  Cholesky  (LL') de
C        una matriz del tipo  Z'HZ  con pivotacion de filas y columnas.
C        En realidad calcula la factorizacion de  PZ'HZP',  donde  P  es
C        una matriz de permutacion de filas.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        H     Matriz de dimension  (IH,N).  En  sus  N  primeras  filas
C              contiene la matriz  H. Es suficiente suministrar la parte
C              triangular inferior.El resto de la matriz no es utilizado
C
C        IH    Primera dimension de la matriz  H. IH >= N.
C
C        R     Matriz de dimension  (IR,NR), con  NR=M-IND. Si  IND > 0,
C              en sus  primeras  filas  contiene  una caja  de dimension
C              (IND,NR)  que  almacena  parte  de  la  factorizacion  ya
C              iniciada  (de la columna  IND+1  a la columna  M  de  la
C              matriz  L' de la factorizacion de  PZ'HZP').
C
C        IR    Primera dimension de la matriz  R. IR >= M.
C
C        Z     Matriz de dimension  (IZ,M).  En  las  N  primeras  filas
C              contiene la matriz  Z,(con sus columnas en orden inverso)
C
C        IZ    Primera dimension de la matriz  Z. IZ >= N.
C
C        W     Vector de trabajo de dimension n
C
C        IPVT  Vector  entero  de trabajo  de  dimension  M-IND. Informa
C              sobre  la permutacion  P  que  de entrada  puede  haberse
C              realizado :
C                 IPVT(I)=J, indica que la permutacion  coloca a la fila
C              J-esima de  Z'HZ  en el lugar  I-esimo (analogamente con
C              las columnas).
C
C        N     Numero de filas de la matriz  Z.
C
C        M     Numero de columnas de la matriz  Z.
C
C        IND   Indicador que puede tomar los valores:
C                 0  : Se comienza la factorizacion. Se procura evitar
C                      la pivotacion durante la factorizacion.
C               > 0  : La matriz  PZ'HZP' esta parcialmente factorizada.
C                      El numero de filas (de L') calculadas es igual al
C                      valor de la variable  IND.
C
C        MODO  Indicador que toma los valores:
C                 0  : No se realiza la factorizacion si la matriz es
C                      indefinida.
C              <> 0  : Se calcula la factorizacion  (total o parcial).
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        R      Si en entrada  IND > 0, R contiene en las primeras filas
C               la caja de la factorizacion de entrada  afectada  por la
C               permutacion  P  (CP'). A partir de la fila  IND+1  tiene
C               a la  matriz triangular superior de la factorizacion de
C               Cholesky  de la caja factorizada. (La factorizacion sera
C               parcial, en el caso indefinido).
C
C        IPVT   Indica la permutacion de filas y columnas realizada al
C               factorizar.
C
C        IND    Variable entera que toma los valores:
C                  N      :    No  se ha realizado la factorizacion  por
C                              ser la matriz indefinida (y  MODO=0).
C                (-1,ND+1):    Se ha finalizado la factorizacion. ND-IND
C                              es la dimension de la parte no nula de L'
C                (-ND-1,0):    No se ha finalizado la factorizacion, por
C                              ser la matriz indefinida:se ha encontrado
C                              un elemento negativo en el lugar  -IND de
C                              la diagonal de la matriz  L'.
C                (-2*ND-1,-ND):No se ha completado la factorizacion, por
C                              ser la matriz indefinida:  L' tiene en el
C                              lugar -IND-ND  de la diagonal un elemento
C                              nulo  y  el elemento  (-ND-IND,-ND-IND+1)
C                              es no nulo.
C               donde  ND  es la dimension  de la caja  a factorizar en
C               entrada.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: dipvtf,ddot,d1mach,zthz
C     FUNCIONES FORTRAN INTRINSECAS: abs,max,mod,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension h(ih,*),r(ir,*),z(iz,*),w(*),ipvt(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(ih.lt.n .or. ir.lt.m .or. iz.lt.n .or. (m-ind).lt.1 .or. ind.lt
     &   .0) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM03.'
         stop
      end if
C
C     Se inicializan algunas variables de trabajo
C
css      epsmch=d1mach(4)
      epsmch=dlamch('p')
      eps=epsmch*10.d0
      if(ind.eq.0) then
         ndim=m
         iifact=0
         do 10 i=1,ndim
10       ipvt(i)=i
      else
         ndim=m-ind
         iifact=1
      end if
      smax=1.d0
      nm1=m+1
C
C     Se calculan los elementos diagonales
C
      do 20 i=1,ndim
         if(ind.eq.0) then
            nj=nm1-i
            ii=i
         else
            nj=nm1-ipvt(i)
            ii=ind+i
         end if
         s=zthz(h,ih,z,iz,n,nj,nj)
         if(ind.gt.0) s=s-ddot(ind,r(1,i),1,r(1,i),1)
         if(modo.eq.0 .and. s.lt.-eps) then
            ind=n
            return
         end if
         r(ii,i)=s
         s=abs(s)
         smax=max(s,smax)
20    continue
C
C     Se estudia el caso unidimensional
C
      if(ndim.eq.1) then
         ik=ind+1
         s=r(ik,1)
         if(s.gt.eps) then
            r(ik,1)=sqrt(s)
            ind=0
         else if(s.lt.-eps) then
            ind=-1
         else
            ind=1
         end if
         return
      end if
C
C     Se procede a factorizar la matriz (si n > 1)
C
      eps0=epsmch*smax
      eps=eps0*ind
      beta=max(eps0*ndim*10,sqrt(smax)*1.2d0)
      iibeta=0
      s1=0
      do 80 k=1,ndim-1
         eps=eps+eps0
         kk=k+1
         ik=k
         if(ind.gt.0) ik=k+ind
         ik0=ik-1
C
C     En caso de no haberse realizado pivotacion (iifact=0), se calcula
C     el elemento diagonal siguiente o todos los elementos diagonales
C     que restan (si la caja que queda por factorizar no es definida
C     positiva)
C
         if(iifact.eq.0) then
            r(k,k)=r(k,k)-ddot(ik0,r(1,k),1,r(1,k),1)
            sk=r(k,k)
            if(s1.gt.beta .or. sk.le.eps) then
               iifact=1
               do 25 i=kk,ndim
25             r(i,i)=r(i,i)-ddot(ik0,r(1,i),1,r(1,i),1)
            end if
         else
            sk=r(ik,k)
         end if
         if(s1.gt.beta) then
            s=-1
            iibeta=1
         else if(iifact.eq.1) then
            j=k
            s=sk
            do 30 i=kk,ndim
               ii=i+ind
               rii=r(ii,i)
               if(rii.gt.s) then
                  j=i
                  s=rii
               end if
30          continue
         else
            s=sk
            j=k
         end if
C
C     Si el mayor elemento diagonal es positivo, se permutan las filas y
C     columnas adecuadas para que ocupe el lugar (ind+k,k)
C
         if(s.gt.eps) then
            if(iifact.eq.1) then
               call dipvtf(r,ir,ipvt,ik0,k,j)
               r(ind+j,j)=sk
               l=nm1-ipvt(k)
            else
               l=nm1-k
            end if
            sk=sqrt(s)
            r(ik,k)=sk
            do 35 i=1,n
               s=ddot(i,h(i,1),ih,z(1,l),1)
               if(i.lt.n) then
                  i1=i+1
                  w(i)=s+ddot(n-i,h(i1,i),1,z(i1,l),1)
               end if
35          continue
            w(n)=s
            s1=0
            do 40 i=kk,ndim
               j=nm1-ipvt(i)
               s=ddot(n,z(1,j),1,w,1)
               if(ik0.gt.0) s=s-ddot(ik0,r(1,i),1,r(1,k),1)
               rik=s/sk
               s1=max(s1,abs(rik))
               r(ik,i)=rik
               ii=i
               if(ind.gt.0) ii=ii+ind
               if(iifact.eq.1) r(ii,i)=r(ii,i)-rik*rik
40          continue
         else
C
C     Si el mayor elemento diagonal no es positivo, se busca el menor
C
            s=sk
            j=k
            do 50 i=kk,ndim
               ii=ind+i
               rii=r(ii,i)
               if(rii.lt.s) then
                  j=i
                  s=rii
               end if
50          continue
C
C      Si el menor elemento diagonal es negativo, se permutan filas y
C      columnas para que se situe en el lugar (ind+k,k)
C
            if(s.lt.-eps) then
               if(modo.eq.0) then
                  ind=n
                  return
               end if
               call dipvtf(r,ir,ipvt,ik0,k,j)
               r(ik,k)=s
               r(ind+j,j)=sk
               ind=-k
               if(iibeta.eq.1) ind=ind-10*iz
               return
            else
C
C      Si ese menor elemento  es  nulo, se averigua  como es  la matriz:
C      semidefinida positiva o indefinida
C
               do 70 j=k,ndim-1
                  nj=nm1-ipvt(j)
                  ij=j+ind
                  do 55 i=1,n
                     s1=ddot(i,h(i,1),ih,z(1,nj),1)
                     if(i.lt.n) then
                        i1=i+1
                        w(i)=s1+ddot(n-i,h(i1,i),1,z(i1,nj),1)
                     end if
55                continue
                  w(n)=s1
                  do 60 i=j+1,ndim
                     s1=ddot(n,w,1,z(1,nm1-ipvt(i)),1)
                     if(ik0.gt.0) s1=s1-ddot(ik0,r(1,i),1,r(1,j),1)
                     r(ij,i)=s1
                     s1=abs(s1)
                     if(s1.gt.s) then
                        s=s1
                        l=i
                     end if
60                continue
C
C     En caso de ser indefinida, se hace la permutacion correspondiente
C     para colocar en el lugar (ind+k,k+1) al elemento no nulo
C
                  if(s.gt.eps) then
                     if(modo.eq.0) then
                        ind=n
                        return
                     end if
                     call dipvtf(r,ir,ipvt,ik0,j,k)
                     call dipvtf(r,ir,ipvt,ik0,l,kk)
                     r(ik,kk)=r(ij,l)
                     ind=-ndim-k
                     return
                  end if
70             continue
C
C     Se calcula el numero de elementos diagonales nulos en el caso
C     semidefinido positivo
C
               ind=ndim-k+1
               return
            end if
         end if
80    continue
C
C     Se estudia el ultimo elemento diagonal
C
      eps=eps0+eps
      in=ndim+ind
      if(iifact.eq.0) r(in,in)=r(in,in)-ddot(in-1,r(1,in),1,r(1,in),1)
      s=r(in,ndim)
      if(s.gt.eps) then
         r(in,ndim)=sqrt(s)
         ind=0
      else if(s.lt.-eps) then
         ind=-ndim
      else
         ind=1
      end if
      end
