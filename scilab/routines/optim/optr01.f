      subroutine optr01(c,ic,q,iq,r,ir,ci,cs,b,x,w,ipvt,ire,ira,n,m,mi,
     &                  mi1,md,ind,imp,io,modo)
C     SUBROUTINE OPTR01(C,IC,Q,IQ,R,IR,CI,CS,B,X,W,IPVT,IRE,IRA,N,M,MI,
C    &                  MI1,MD,IND,IMP,IO,MODO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                            FEBRERO 1987                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina calcula, segun el valor de la variable MODO :
C                 -Un punto admisible para un conjunto de restricciones
C        lineales, que pueden ser de igualdad,desigualdad y de acotacion
C                 -La factorizacion  QR  de una matriz que almacena los
C        coeficientes  de las restricciones  linealmente  independientes
C        de entre las restricciones  recomendadas, (MODO=22), o de entre
C        todas las restricciones que aparecen en el problema (MODO=21).
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        C      Matriz de dimension (IC,MI+MD).Guarda, en sus N primeras
C               filas, los coeficientes de las restricciones de igualdad
C               (en las MI primeras columnas), y los coeficientes de las
C               restricciones de desigualdad  (en el resto).
C
C        IC     Primera dimension de la matriz C. IC >= N.
C
C        Q      Matriz de trabajo de dimension (IQ,N). Si se trabaja con
C               los MODOS : 3,4,13 o 14, en las N primeras filas, recoge
C               a la matriz ortogonal de la factorizacion   QR   de las
C               restricciones activas .
C
C        IQ     Primera dimension de la matriz Q. IQ >= N.
C
C        R      Matriz de trabajo de dimension  (IR,MT), donde se tomara
C               MT=min(N, numero  maximo  de  restricciones  activas
C               posibles). En los  MODOS : 3,4,13,14 , en las N primeras
C               filas y M columnas, se suministra a la matriz triangular
C               superior  de la factorizacion   QR  de las restricciones
C               activas. La parte subdiagonal no es utilizada.
C
C        IR     Primera dimension de la matriz R. IR >= N.
C
C        CI     Si   IRA= 0  o  2  esta variable  no sera  utilizada. Si
C               IRA=  1  o  3,  CI  sera  un  vector  de  dimension   N
C               conteniendo las cotas inferiores de  X. Si  X(i) no esta
C               acotado inferiormente,  CI(i)  debera  ser  menor que la
C               raiz cuadrada negativa  de la constante real  mas grande
C               de la maquina.
C
C        CS     SI   IRA= 0  o  1  esta variable  no sera  utilizada. Si
C               IRA=  2  o  3,  CS   sera  un  vector  de  dimension   N
C               conteniendo las cotas superiores de  X. Si  X(i) no esta
C               acotado  superiormente, CS(i)  debera  ser  mayor  que
C               la raiz cuadrada  de la constante real  mas grande de la
C               maquina.
C
C        B      Vector de dimension  MI+MD. Contiene los coeficientes de
C               los  terminos  independientes  de  las  restricciones de
C               igualdad y desigualdad.
C
C        X      Vector N-dimensional que contiene un punto inicial.
C
C        W      Vector de trabajo,  cuya dimension ha de ser   N+MI   si
C               MODO > 20   o   3N+2MD  en otro caso.
C
C        IPVT   Vector entero de trabajo  de dimension  N+MI+MD+1  si la
C               variable MODO  toma un valor inferior a 20  o  max(N,MI)
C               en otro caso. Se facilita si se trabaja con los  MODOS :
C               3, 4, 13, 14.  En  sus  M  primeras  coordenadas  indica
C               las restricciones que son activas:
C               -En las MI1 primeras, recoge las de igualdad linealmente
C               independientes.
C               -En las demas coordenadas:
C                 IPVT(I) <= N: Indica  que  la  restriccion  i-esima es
C                               acotacion  inferior  si  IPVT(I) < 0,  y
C                               superior  si IPVT(I) > 0.
C                 IPVT(I) > N : Hace referencia  a la restriccion numero
C                               IPVT(I)-N  de desigualdad.
C
C        IRE    Vector de dimension  N+MD.Indica que restricciones deben
C               de ser activas al comienzo del proceso. Se facilita este
C               vector, si  MODO=  2,4,12,14  o  22, de acuerdo con los
C               siguientes valores:
C               -Para las restricciones de acotacion, en las N-primeras
C               componentes del vector:
C                  0  : No se recomiendan activas las restricciones que
C                       corresponden a la coordenada i-esima.
C                  1  : Se  recomienda, como activa , la  restriccion de
C                       acotacion superior.
C                 -1  : Se  recomienda  activa  la restriccion  de cota
C                       inferior.
C               -Para las restricciones de desigualdad, en las restantes
C               componentes del vector:
C                  0  : No se recomienda la restriccion.
C                  1  : Se  recomienda  activa  la  restriccion  I-N  de
C                       desigualdad.
C               IRE es un vector de trabajo.
C
C        IRA    Variable  que  indica  si  existen  restricciones  de
C               acotacion.  Puede tomar los valores:
C                  0  : No existen restricciones de acotacion.
C                  1  : Se  tienen  solo  restricciones  de  acotacion
C                       inferior.
C                  2  : Existen solo restricciones de acotacion superior
C                  3  : Se tienen ambos tipos de restriccion.
C
C        N      Numero de variables del problema.
C
C        M      Indica el numero  de restricciones activas  (linealmente
C               independientes)  del  problema.  Se  suministra  solo si
C               MODO= 3, 13, 4  o  14.
C
C        MI     Numero de restricciones de igualdad del problema.
C
C        MI1    Numero de restricciones de igualdad linealmente indepen-
C               dientes.  MI1  se facilita  siempre que se de tambien la
C               factorizacion  QR.
C
C        MD     Numero de restricciones de desigualdad en el problema.
C
C        IMP    Indicador del nivel de impresion de salida de resultados
C               Toma los valores:
C                 10  : No hay salida de resultados.
C                 11  : Escribe el motivo de finalizacion del  proceso.
C                 12  : Tambien se obtienen el numero de iteraciones que
C                       se ha realizado y las restricciones de igualdad
C                       que son linealmente independientes.
C                 13  : Ademas se tiene, en cada iteracion, el valor del
C                       funcional de restricciones violadas, si surge un
C                       punto degenerado y, el punto admisible calculado
C                       (si este se ha encontrado).
C                 14  : Tambien se escriben los puntos de cada iteracion
C
C        IO     Numero del canal de salida de resultados.
C
C        MODO   Variable que indica la forma de utilizacion del programa
C               Puede tomar los valores:
C                  1  : No se recomiendan  restricciones activas y no se
C                       suministra la factorizacion  QR  de la matriz de
C                       coeficientes de las restriciones de igualdad.
C                  2  : Hay recomendaciones en las restricciones,pero no
C                       se da la factorizacion.
C                  3  : No se recomiendan restricciones activas,pero se
C                       suministra la factorizacion  QR, (en Q y R), de
C                       la matriz de coeficientes de las restricciones
C                       de igualdad.
C                  4  : Hay recomendaciones sobre las restricciones y se
C                       da, en Q y R, la factorizacion  QR  de la matriz
C                       de coeficientes de restricciones activas.
C               Si toma un valor superior a 10 (10 < MODO < 15) se desea
C               calcular un vertice; el modo de utilizacion del programa
C               sera uno de los cuatro anteriores (indicado por la cifra
C               de la unidad).
C               Si toma un valor superior a  20, se desea calcular solo
C               la factorizacion  QR  de ciertas restricciones. Asi, si
C               toma los valores:
C                 21  : Se calcula la factorizacion de las restricciones
C                       linealmente independientes que se encuentran en
C                       el problema planteado.
C                 22  : Se factorizan las restricciones recomendadas que
C                       son linealmente independientes.
C
C     DE SALIDA:
C
C        X      Vector N-dimensional que contiene un punto admisible, en
C               caso de haberse encontrado. En el tambien se guardan los
C               puntos de las distintas iteraciones.
C
C        IPVT   Indica, en sus M primeras coordenadas, las restricciones
C               linealmente independientes, (que, en su caso,son activas
C               en el punto X).  Los valores de  IPVT siguen el convenio
C               antes citado.
C
C        IRE    Indica de que tipo son las restricciones de acotacion y
C               las de desigualdad en el punto X obtenido,tomando en las
C               coordenadas respectivas los valores:
C                  0  : Restriccion que se satisface.
C                  1  : Restriccion activa.
C                  2  : Restriccion violada.
C               Las coordenadas correspondientes a restricciones de cota
C               toman los valores anteriores para la restriccion de cota
C               superior y valores negativos para la inferior.
C
C        M      Variable que indica el numero de restricciones que son
C               linealmente independientes y ,en su caso, son activas en
C               el punto  X.
C
C        MI1    Numero de restricciones de igualdad linealmente indepen-
C               dientes.
C
C        IND    Variable que indica el motivo por el cual se finaliza el
C               proceso.Toma el valor:
C                  0  : Se ha finalizado el proceso sin problemas.
C                 -1  : Imcompatibilidad en el sistema de restricciones
C                       de igualdad.
C                 -2  : No existen puntos admisibles.
C                 -3  : Se encuentra un punto degenerado con ciclaje
C                       indefinido.
C                 -4  : Se ha realizado el limite de iteraciones sin
C                       encontrar un punto admisible.
C                 -5  : Los datos suministrados son incorrectos.
C               Si el objetivo de la subrutina es la factorizacion QR,
C               esta variable tomara uno de los dos primeros valores.
C
C
C        Esta subrutina trabaja en doble precicion via una sentencia
C     "implicit":
C                        -Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:   anfm01,anfm02,anrs01,auxo01,dadd,daxpy
C                        dcopy,ddif,ddot,dmmul,dnrm0,dnrm2,dscal,dswap,
C                        d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,min,mod,sign,sqrt
C
      implicit double precision (a-h,o-z)
      character car*30
      dimension c(ic,*),q(iq,*),r(ir,*),ci(*),cs(*),b(*),x(*),w(*),
     &          ipvt(*),ire(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      car='END OF  OPTR01.'
      if(mi.lt.0 .or. md.lt.0 .or. ira.lt.0 .or. ira.gt.3 .or. io.lt.1 .
     &   or. n.le.1 .or. modo.lt.1 .or. modo.gt.22 .or. ic.lt.n .or. iq.
     &   lt.n .or. ir.lt.n) then
                 if(io.le.0)print 1000,car,
     &      'INVALID NUMBER FOR THE WRITING CHANEL.'
         if(io.gt.0) write(io,1000) car,'INVALID INTEGER VARIABLES.'
         ind=-5
         return
      end if
C
C     Se toman algunos parametros de trabajo
C
      gigant=dlamch('o')

css   gigant=d1mach(2)
      gig1=sqrt(gigant)
      test0=gigant**0.25
css   eps=d1mach(4)**0.75
      eps=dlamch('p')**0.75

C
C     Se comprueba que los vectores CI,CS,IRE  toman valores correctos
C
      do 10 i=1,n
         if(ira.eq.3) then
            if(ci(i).ge.-gig1 .and. cs(i).le.gig1 .and. ci(i).gt.
     &         cs(i)) then
               write(io,2000) car,'CI(I).GT.CS(I) FOR I=',i
               ind=-5
               return
            end if
         end if
         if((modo.eq.2 .or. modo.eq.4 .or. modo.eq.12 .or. modo.eq.14
     &      .or. modo.eq.22 ) .and.ira.gt.0) then
            if(ire(i).lt.-1 .or. ire(i).gt.1) then
               write(io,1000) car,'INCORRECT VECTOR IRE.'
               ind=-5
               return
            end if
         else
            ire(i)=0
         end if
10    continue
      do 20 i=n+1,n+md
         if(modo.eq.2 .or. modo.eq.4 .or. modo.eq.12 .or. modo.eq.14
     &      .or. modo.eq.22) then
            if(ire(i).lt.0 .or. ire(i).gt.1) then
               write(io,1000) car,'INCORRECT VECTOR IRE.'
               ind=-5
               return
            end if
         else
            ire(i)=0
         end if
20    continue
C
C     Se inicializan algunas variables  enteras de trabajo
C
      n1=n+1
      n2=n1+n
      n3=n2+n
      nmd=n3+md
      mni=mi+1
      ind=0
      icont=0
      icicla=0
      icol1=0
      icol2=0
      mid=mi+md
      nmid=n+mid
      itemax=4*nmid
      s=dnrm0(n,x,1)
      if(s.eq.0.d0) then
         indx=0
      else
         indx=1
      end if
C
C     Se averigua cual es el objetivo del programa (MODO): el calculo de
C     la factorizacion QR ,(INF=1), o de un punto admisible ,(INF=0) ;
C     y dentro de este ultimo caso se distingue entre el calculo de un
C     vertice, (IOPT=0), o de un punto cualquiera (IOPT=1).
C
      iopt=0
      inf=0
      if(modo.gt.20) then
         modo=modo-20
         inf=1
      else if(modo.gt.10) then
         modo=modo-10
      else
         iopt=1
      end if
C
C     Si no existe factorizaion QR inicializamos la matriz Q a la
C     identidad
C
      if(modo.le.2) then
         if(mi.eq.0) m=0
         mi1=mi
         do 30 i=1,n
30       w(i)=0
         do 40 i=1,n
            w(i)=1
            call dcopy(n,w,1,q(1,i),1)
            w(i)=0
40       continue
      end if
C
C     Se procede a la factorizacion QR
C
      if(modo.le.2 .and. mi.ge.1) then
         mi1=1
         i2=mi
         do 50 i=1,mi
            call anfm01(q,iq,r,ir,c(1,i),w,n,mi1,ind,io)
            if(ind.lt.0) then
               ipvt(i2)=i
               ind=0
               i2=i2-1
            else
               ipvt(mi1)=i
               mi1=mi1+1
            end if
50       continue
         mi1=mi1-1
         if(mi1.gt.0) then
            if(indx.eq.1) then
               do 60 i=1,mi1
                  l=ipvt(i)
                  w(i)=b(l)-ddot(n,c(1,l),1,x,1)
60             continue
            else
               do 65 i=1,mi1
65             w(i)=b(ipvt(i))
            end if
            ind=1
            call anrs01(r,ir,mi1,w,w(n1),ind,io)
            ind=0
            call dmmul(q,iq,w(n1),mi1,w(n2),n,n,mi1,1)
            if(indx.eq.1) then
               call dadd(n,w(n2),1,x,1)
            else
               call dcopy(n,w(n2),1,x,1)
            end if
         end if
C
C     Si el rango de c(1,mi) no es mi se estudia la compatibilidad
C     de las restricciones de igualdad
C
         if(mi1.lt.mi) then
            do 70 i=mi1+1,mi
               l=ipvt(i)
               if(mi1.gt.0) then
                  s=b(l)-ddot(n,x,1,c(1,l),1)
               else
                  s=b(l)
               end if
               if(abs(s).gt.eps) then
                  ind=-1
                  if(imp.ge.11) write(io,1000) car,
     &        'THE SYSTEM OF EQUALITY CONSTRAINTS HAS NOT SOLUTION'
                  return
               end if
70          continue
         end if
         m=mi1
      end if
      if(imp.ge.12 .and. mi1.gt.0) write(io,3000) (n+ipvt(i),i=1,mi1)
C
C     Si se han recomendado ciertas restricciones de desigualdad o
C     acotacion y no se ha suministrado la factorizacion QR, se
C     a¤aden estas a la factorizacion hasta ahora obtenida
C
      if(modo.eq.2) then
         if(ira.gt.0) then
            i=1
            if(i.le.n .and. m.lt.n) then
500            if(ire(i).eq.1) ind=i
               if(ire(i).eq.-1) ind=-i
               if(ind.ne.0) then
                  m1=m+1
                  call anfm01(q,iq,r,ir,w,w,n,m1,ind,io)
                  if(ind.lt.0) then
                     ind=0
                     ire(i)=0
                  else
                     m=m1
                     ipvt(m)=ire(i)*i
                  end if
               end if
               i=i+1
               if(i.le.n .and. m.lt.n) then
                  go to 500
               else if(i.le.n) then
                  do 75 j=i,n+md
75                ire(j)=0
               end if
            end if
         end if
         if(md.gt.0) then
            i=1
            if(i.le.md .and. m.lt.n) then
525            if(ire(n+i).eq.1) then
                  m1=m+1
                  call anfm01(q,iq,r,ir,c(1,mi+i),w,n,m1,ind,io)
                  if(ind.lt.0) then
                     ind=0
                     ire(n+i)=0
                  else
                     m=m1
                     ipvt(m)=n+i
                  end if
               end if
               i=i+1
               if(i.le.md .and. m.lt.n) then
                  go to 525
               else if(i.le.md) then
                  do 80 j=i,md
80                ire(j+n)=0
               end if
            end if
         end if
      end if
      if(modo.eq.2 .and. inf.eq.1) then
         if(imp.ge.11) write(io,1000) car,
     &      'THE FACTORIZATION  QR  HAS BEEN OBTAINED.'
         return
      end if
C
C     Si se desea obtener un punto admisible (INF=0), no necesariamente
C     un vertice (iopt=1), se busca un punto satisfaciendo las
C     restricciones que hasta el momento son activas
C
      mr=0
      if(inf.eq.0) then
         do 95 i=1,mi1
95       w(i)=b(ipvt(i))
         do 100 i=mi1+1,m
            l=ipvt(i)
            if(l.lt.0) then
               w(i)=-ci(-l)
            else if(l.le.n) then
               w(i)=cs(l)
            else
               w(i)=b(mi+l-n)
            end if
100      continue
         mr=m
         if(iopt.eq.1) then
            if(m.gt.0 .and. (modo.gt.2 .or. m.gt.mi1)) then
               ind=1
               call anrs01(r,ir,m,w,w(n1),ind,io)
               call dmmul(q,iq,w(n1),m,x,n,n,m,1)
               ind=0
            end if
C
C     Se averigua si el punto calculado cumple las demas restricciones
C
            ind=0
            call auxo01(c(1,mni),ic,ci,cs,b(mni),x,w(n3),ire,ira,n,md,
     &                  ind,fun,iv)
         end if
      end if
C
C     Si iopt=1 y el punto calculado x es admisible (iv=0) se añaden a
C     la factorizacion las restricciones activas en x. En otro caso
C     se añaden tambien las violadas en x
C
      if(ira.gt.0) then
         i=1
         if(i.le.n .and. m.lt.n) then
550         iira=0
            if((ira.eq.1 .or. ira.eq.3) .and. x(i).le.ci(i)) then
               iira=1
               ind=-i
            end if
            if(iira.eq.0 .and. ira.ge.2 .and. x(i).ge.cs(i)) then
               iira=1
               ind=i
            end if
            if(iira.gt.0) then
               m1=m+1
               k=ind
               call anfm01(q,iq,r,ir,w,w(n1),n,m1,ind,io)
               if(ind.lt.0) then
                  ind=0
               else
                  m=m1
                  ipvt(m)=k
                  if(k.lt.0) then
                     ire(i)=-1
                  else
                     ire(i)=1
                  end if
               end if
            end if
            i=i+1
            if(i.le.n .and. m.lt.n) go to 550
         end if
      end if
      if(md.gt.0 .and. m.lt.n) then
         i=mi+1
         nii=n-mi
         if(i.le.mid .and. m.lt.n) then
575         s=b(i)-ddot(n,c(1,i),1,x,1)
            if(s.le.eps) then
               m1=m+1
               call anfm01(q,iq,r,ir,c(1,i),w(n1),n,m1,ind,io)
               if(ind.lt.0) then
                  ind=0
               else
                  ni=nii+i
                  m=m1
                  ire(ni)=1
                  ipvt(m)=ni
               end if
            end if
            i=i+1
            if(i.le.mid .and. m.lt.n) go to 575
         end if
      end if
      if(iv.eq.0 .and. iopt.eq.1) then
         if(imp.ge.11) write(io,1000) car,
     &      'A FEASIBLE POINT HAS BEEN FOUND (1)'
         if(imp.ge.13) write(io,7000) (x(i),i=1,n)
         return
      end if
C
C     Si se desea calular un vertice se continua añadiendo
C     restricciones
C
      if(ira.gt.0 .and. m.lt.n .and. iopt.eq.0) then
         i=1
         if(i.le.n .and. m.lt.n) then
600         j=ire(i)
            if(j.eq.0) then
               iira=0
               if(ira.eq.1 .and. ci(i).ge.-gig1) then
                  ind=-i
                  iira=1
               else if(ira.eq.2 .and. cs(i).le.gig1) then
                  ind=i
                  iira=1
               else
                  cii=ci(i)
                  csi=cs(i)
                  if(cii.ge.-gig1 .or. csi.le.gig1) then
                     xi=x(i)
                     iira=1
                     if((xi-cii).lt.(csi-xi)) then
                        ind=-i
                     else
                        ind=i
                     end if
                  end if
               end if
               if(iira.gt.0) then
                  m1=m+1
                  k=ind
                  call anfm01(q,iq,r,ir,w,w(n1),n,m1,ind,io)
                  if(ind.lt.0) then
                     ind=0
                  else
                     m=m1
                     ipvt(m)=k
                     if(k.gt.0) then
                        ire(i)=1
                     else
                        ire(i)=-1
                     end if
                  end if
               end if
            end if
            i=i+1
            if(i.le.n .and. m.lt.n) go to 600
         end if
      end if
      i=mi+1
      if(i.le.mid .and. m.lt.n .and. iopt.eq.0) then
625      ni=nii+i
         j=ire(ni)
         if(j.eq.0) then
            m1=m+1
            call anfm01(q,iq,r,ir,c(1,i),w(n1),n,m1,ind,io)
            if(ind.lt.0) then
               ind=0
            else
               m=m1
               ire(ni)=1
               ipvt(m)=ni
            end if
         end if
         i=i+1
         if(i.le.mid .and. m.lt.n .and. iopt.eq.0) go to 625
      end if
      if(modo.eq.1 .and. inf.eq.1) then
         if(imp.ge.11) write(io,*) car,
     &      'THE FACTORIZATION  QR  HAS BEEN OBTAINED.'
         return
      end if
C
C     Se busca un punto x satisfaciendo las restricciones activas
C
      m1=m+1
      do 110 i=mr+1,m
         l=ipvt(i)
         if(l.lt.0) then
            w(i)=-ci(-l)
         else if(l.le.n) then
            w(i)=cs(l)
         else
            w(i)=b(mi+l-n)
         end if
110   continue
      if((iopt.eq.1 .and. mr.lt.m) .or. iopt.eq.0) then
         ind=1
         call anrs01(r,ir,m,w,w,ind,io)
         call dmmul(q,iq,w,m,x,n,n,m,1)
      end if
      ind=0
      nm=n-m
C
C     Comienzan las iteraciones
C
      if(icont.le.itemax) then
C
C     Si x no es un punto degenerado se averigua cuales son las
C     restricciones violadas y se calcula el gradiente
C
650      if(icicla.eq.0) then
            ind=1
            call auxo01(c(1,mni),ic,ci,cs,b(mni),x,w,ire,ira,n,md,
     &                  ind,fun,iv)
            ind=0
C
C     Impresion de los datos correspondientes a la iteracion corriente
C     y fin de la subrutina si el punto x es admisible
C
            if(iv.eq.0) then
               if(imp.ge.11) write(io,1000) car,
     &            'A FEASIBLE POINT HAS BEEN FOUND'
               if(imp.ge.13) write(io,7000) (x(i),i=1,n)
               if(imp.ge.12) write(io,4000)'NUMBER OF ITERATIONS:',icont
               return
            else if(iv.eq.1 .and. mi1.eq.m) then
               ind=-2
               if(imp.ge.11) write(io,5000) car
               return
            else
               if(imp.ge.13)  then
                  write(io,6000) icont,fun
                  if(imp.ge.14) write(io,7000) (x(i),i=1,n)
               end if
            end if
         end if
         icont=icont+1
         if(m.lt.n) then
            j=n2
            do 135 i=m1,n
               w(j)=ddot(n,q(1,i),1,w,1)
               j=j+1
135         continue
            s=dnrm2(n-m,w(n2),1)
         else
            s=0
         end if
         if(s.lt.eps) then
C
C     Se calculan los multiplicadores de Lagrange
C
            do 140 i=1,m
140         w(n+i)=ddot(n,q(1,i),1,w,1)
            ind=2
            call anrs01(r,ir,m,w(n1),w(n2),ind,io)
C
C     Calculo del multiplicador de Lagrange mas peque¤o y test de
C     positividad
C
            ind=0
            icol=mi1+1
            s1=w(n2+mi1)
            do 150 i=mi1+1,m-1
               j=n2+i
               if(w(j).lt.s1) then
                  s1=w(j)
                  icol=i+1
               end if
150         continue
            if(s1.gt.-eps) then
               if(imp.ge.11) then
                  write(io,5000) car
                  write(io,8000) s1
               end if
               ind=-2
               return
            end if
C
C     Se elimina del conjunto activo la restriccion icol
C
            call anfm02(q,iq,r,ir,n,m,icol,io)
C
C     Se calcula la direccion de descenso
C
            s1=ddot(n,q(1,m),1,w,1)
            call dcopy(n,q(1,m),1,w(n1),1)
            if(s1.lt.0) then
               do 160 i=n1,n2-1
160            w(i)=-w(i)
            else
               s1=-s1
            end if
         else
            call dmmul(q(1,m1),iq,w(n2),n-m,w(n1),n,n,n-m,1)
            s1=-ddot(n,w,1,w(n1),1)
            icol=n1
         end if
C
C     Se averigua si estamos ante un punto degenerado
C
         k=0
         if(ira.gt.0) then
            i=0
            if(i.lt.n .and. k.eq.0) then
675            i=i+1
               ii=n+i
               if(ira.gt.1) then
                  if(cs(i).le.gig1 .and. ire(i).eq.0 .and. w(ii).gt.
     &            eps .and. x(i).ge.(cs(i)-eps)) then
                     k=1
                     icol3=i
                  end if
               end if
               if(k.eq.0 .and. ira.ne.2) then
                  if(ci(i).ge.-gig1 .and. ire(i).eq.0 .and.w(ii).lt.
     &                 -eps .and. x(i).le.(ci(i)+eps)) then
                     k=1
                     icol3=-i
                  end if
               end if
               if(i.lt.n .and. k.eq.0) go to 675
            end if
         end if
         i=0
         if(i.lt.md .and. k.eq.0) then
700         ii=nmd+i
            in=n3+i
            i=i+1
            ni=n+i
            if(ire(ni).ne.1) then
               w(ii)=ddot(n,c(1,mi+i),1,w(n1),1)
               if(ire(ni).eq.0 .and. w(in).ge.-eps .and. w(ii).gt.
     &            eps) then
                  k=1
                  icol3=ni
               end if
            end if
            if(i.lt.md .and. k.eq.0) go to 700
         end if
C
C     En caso de punto degenerado se examina si se esta produciendo
C     alternancia entre dos restricciones
C
         if(k.eq.1) then
            icicla=icicla+1
            if(icol.lt.n1) then
               if(icicla.gt.m .or. (icol3.eq.icol1.and.icol.eq.m)) then
                  if(imp.ge.11) write(io,9000) car
                  ind=-3
                  return
               end if
               icol1=icol2
               icol2=icol3
               i=ipvt(icol)
               if(i.gt.n) w(n2+i)=ddot(n,c(1,i-n+mi),1,x,1)-b(i-n+mi)
            end if
         else if(icicla.gt.0) then
            icicla=0
            icol1=0
            icol2=0
         end if
C
C     Se calculan los ros donde se saturan las restricciones segun la
C     direccion de descenso calculada
C
         k=0
         if(ira.gt.0) then
            i=0
            if(i.lt.n .and. icicla.eq.0) then
725            i1=i+1
               j=0
               ii=n1+i
               wii=w(ii)
               irei=ire(i1)
               if(ira.gt.1) then
                  if(cs(i1).le.gig1 .and. ((irei.eq.0 .and. wii.
     &               gt.eps) .or. (irei.eq.2.and.wii.lt.-eps)
     &               )) then
                     w(n2+k)=(cs(i1)-x(i1))/wii
                     k=k+1
                     ipvt(m+k)=i1
                     j=1
                  end if
               end if
               if(j.eq.0 .and. ira.ne.2) then
                  if(ci(i1).ge.-gig1 .and. ((irei.eq.0 .and. wii
     &               .lt.-eps).or. (irei.eq.-2.and.wii.gt.eps
     &               ))) then
                     w(n2+k)=(ci(i1)-x(i1))/wii
                     k=k+1
                     ipvt(m+k)=-i1
                  end if
               end if
               i=i1
               if(i.lt.n .and. icicla.eq.0) go to 725
            end if
         end if
         if(icol.lt.n1) then
            ipc=ipvt(icol)
            ia=abs(ipc)
            if(icicla.eq.0 .and. ia.le.n .and. ira.eq.3) then
               cii=ci(ia)
               csi=cs(ia)
               if(cii.ge.-gig1 .and. csi.le.gig1) then
                  k=k+1
                  if(ipc.lt.0) then
                     w(n2+k-1)=(csi-x(ia))/w(n+ia)
                     ipvt(m+k)=ia
                  else
                     w(n2+k-1)=(cii-x(ia))/w(n+ia)
                     ipvt(m+k)=-ia
                  end if
               end if
            end if
         end if
         i=0
         if(i.lt.md .and. icicla.eq.0) then
750         i1=i+1
            ii=nmd+i
            ni=n+i1
            ireni=ire(ni)
            wii=w(ii)
            if((ireni.eq.0 .and. wii.gt.eps) .or. (ireni.eq.2
     &      .and. wii.lt.-eps)) then
               w(n2+k)=-w(n3+i)/wii
               k=k+1
               ipvt(m+k)=ni
            end if
            i=i1
            if(i.lt.md .and. icicla.eq.0) go to 750
         end if
C
C     Calculo del paso optimo en la direccion de descenso
C
         r2=0
         if(icicla.eq.0 .and. s1.lt.-eps) then
775         l=0
            r1=r2
            r2=gigant
            do 170 i=1,k
               ii=n2+i-1
               wii=w(ii)
               if(wii.le.r2 .and. wii.gt.r1) then
                  if(wii.lt.r2) l=0
                  r2=wii
                  w(ii)=w(n2+l)
                  w(n2+l)=r2
                  ni=m+i
                  l=l+1
                  j=ipvt(ni)
                  ml=m+l
                  ipvt(ni)=ipvt(ml)
                  ipvt(ml)=j
               end if
170         continue
C
C     Calculo de la derivada en la direccion de descenso en los puntos
C     x+ro*d
C
            icol3=ipvt(m1)
            do 180 i=1,l
               j=ipvt(m+i)
               if(j.lt.0) then
                  j=-j
                  jj=n+j
                  if(ire(j).eq.-2) then
                     s1=s1+w(jj)
                     if(ira.eq.3) then
                        if(ci(j).ge.-gig1 .and. cs(j).le.gig1)
     &                  then
                           ire(j)=0
                           ipvt(m+i)=j
                           w(n2+i-1)=(cs(j)-x(j))/w(n+j)
                        end if
                     end if
                  else
                     s1=s1-w(jj)
                  end if
               else if(j.lt.n1) then
                  jj=n+j
                  if(ire(j).eq.2) then
                     s1=s1-w(jj)
                     if(ira.eq.3) then
                        if(ci(j).ge.-gig1 .and. cs(i).le.gig1)
     &                  then
                           ire(j)=0
                           ipvt(m+i)=-j
                           w(n2+i-1)=(ci(j)-x(j))/w(n+j)
                        end if
                     end if
                  else
                     s1=s1+w(jj)
                  end if
               else if(j.gt.n) then
                  jj=nmd+j-n1
                  if(ire(j).eq.2) s1=s1-w(jj)
                  if(ire(j).eq.0) s1=s1+w(jj)
               end if
180         continue
            if(icicla.eq.0 .and. s1.lt.-eps) go to 775
         end if
C
C     Se calcula un nuevo punto si no hay ciclaje
C
         if(icicla.eq.0) then
            ipvt(m1)=icol3
            call daxpy(n,r2,w(n1),1,x,1)
         end if
C
C     Se adaptan los vectores ire e ipvt y se cambia la columna icol
C     de la matriz de las restricciones activas por la restriccion
C     icol3
C
         if(icol3.lt.0) then
            ire(-icol3)=-1
         else
            ire(icol3)=1
         end if
         if(icol.lt.n1) then
            ire(ia)=0
            do 200 j=icol,m-1
200         ipvt(j)=ipvt(j+1)
            ipvt(m)=icol3
         else
            icicla=0
            m=m1
            m1=m1+1
         end if
         if(icol3.le.n) then
            ind=icol3
         else
            call dcopy(n,c(1,mi+icol3-n),1,w(n1),1)
         end if
         call anfm01(q,iq,r,ir,w(n1),w(n2),n,m,ind,io)
         ind=0
         if(icicla.ne.0 .and. imp.ge.13)write(io,4000)
     &   'A DEGENERATED POINT HAS BEEN FOUND IN THE ITERATION:',icont
         if(icont.le.itemax) go to 650
      end if
      if(imp.ge.11) write(io,10000) car
      ind=-4
1000  format(////,80('*'),///,10x,A,/,10x,A)
2000  format(////,80('*'),///,10x,A,/,10x,A,I5)
3000  format(////,80('*'),///,10x,
     &'THE INDEPENDENT LINEAR EQUALITY CONSTRAINTS ARE:',
     &//,(10x,20(2x,I4),/))
4000  format(///,10x,A,I4)
5000  format(////,80('*'),///,10x,A,/,10x,
     &'THERE ARE NOT FEASIBLE POINTS.')
6000  format(////,80('-'),///,10x,'ITERATION:',I4,//,10x,
     & 'OBJECTIVE FUNCTION :',F24.15)
7000  format(//,10x,'POINT COMPUTED: X =',(T31,sp,E22.16))
8000  format(//,10x,'SMALLEST LAGRANGE MULTIPLIER :',F19.14)
9000  format(////,80('*'),///,10x,A,/,10x,'INDEFINITE CICLE ON A DEGENE
     &RATED POINT.')
10000 format(////,80('*'),///,10x,A,/,10x,'THE LIMIT FOR THE ITERATION
     &NUMBER HAS BEEN PASSED WITHOUT',/,10X,'FINDING A FEASIBLE POINT.')
      end
