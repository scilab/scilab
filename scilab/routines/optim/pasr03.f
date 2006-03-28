      subroutine pasr03(a,ia,b,ci,cs,x,ro,w,ire,ipvt,ira,n,md,mif,mdf,
     &                  m,id,io)
C      SUBROUTINE PASR03(A,IA,B,CI,CS,X,RO,W,IRE,IPVT,IRA,N,MD,MIF,MDF,
C     &                  M,ID,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C      Copyright:       Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                             JULIO  1987                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina es auxiliar de OPTR03. Calcula el paso en la
C        direccion de descenso para una iteracion determinada.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        A      Matriz de dimension (IA,MIF+MDF). (Referencia en OPTR03)
C
C        IA     Primera dimension de la matriz  A. IA >= N.
C
C        B      Vector de dimension MIF+MDF. (Referencia en OPTR03).
C
C        CI     Vector de cotas inferiores. (Referencia en OPTR03).
C
C        CS     Vector de cotas superiores. (Refencia en OPTR03).
C
C        X      Vector N-dimensional. Contiene al punto de la iteracion
C               correspondiente de OPTR03.
C
C        W      Vector de trabajo de dimension  2*(N+MD+MIF+MDF).
C
C        IRE    Vector de dimension N+MD+MIF+MDF.(Referencia en OPTR03).
C
C        IPVT   Vector de trabajo de dimension MIF+MDF.
C               Referencia en OPTR03.
C
C        IRA   Variable que indica si existen restricciones de cota.
C              (Referencia en OPTR03).
C
C        N      Dimension del problema.
C
C        MD     Referencia en OPTR03.
C
C        MIF    Referencia en OPTR03.
C
C        MDF    Referencia en OPTR03.
C
C        M      Numero de restricciones activas.
C
C        ID     Indica el tipo de direccion de descenso suministrada
C               con los valores:
C               0,10,100: Direccion de curvatura negativa.
C                  1    : Direccion cualquiera.
C               2,12,102: Direccion  del  nucleo asociado al conjunto de
C                         restricciones activas.
C               Si 10<ID<100 nos encontramos ante un punto degenerado y
C               puede ser necesario alterar el gradiente.
C               Si ID>100 nos encontramos ante un punto degenerado para
C               el que la direccion obtenida no es de descenso.
C
C        RO     Variable que contiene (si ID=1) el posible paso optimo
C               de la iteracion correspondiente.
C
C        IO     Numero de canal de salida de resultados.
C
C
C     DE SALIDA:
C
C        RO     Variable que contiene el paso calculado.
C
C        IPVT   En su primera coordenada indica la restriccion asociada
C               con el paso  calculado, en caso de que tal  restriccion
C               exista.
C
C        ID     Indicador que toma los valores:
C                 -1  : No existe solucion acotada.
C                  0  : No  se  ha  calculado  el  paso  por  producirse
C                       ciclaje.
C                  1  : El paso calculado tiene una restriccion asociada
C                  2  : El   paso   calculado   conduce   a   un   punto
C                       estacionario: x+ro*d.
C                  3  : El  paso,  ademas  de  dar  lugar  a  un  punto
C                       estacionario, esta asociado con una restriccion.
C                 11    Indica  que  el paso  calculado  es  " demasiado
C               grande ". La cifra de la unidad  seðala  el tipo de paso
C               calculado segun el convenio anterior.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: ddot,d1mach,dnrm0,idamax
C     FUNCIONES FORTRAN INTRINSECAS: abs,min,mod,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension a(ia,*),b(*),ci(*),cs(*),x(*),w(*),ire(*),ipvt(*)
C
C     Se inicializan algunas variables de trabajo
C
      nmd=n+md
      nd1=nmd+1
      ndf=nmd+mif
      nd0=ndf+mdf
      nd=nd0+1
      icd0=nd0+n
      iad0=icd0+md
css      eps=d1mach(4)**0.75
css      gigant=d1mach(2)
      eps=dlamch('p')**0.75
      gigant=dlamch('o')
      gig1=sqrt(gigant)
      ind1=0
      if(id.ge.100) then
         ind1=2
         id=id-100
      else if(id.ge.10) then
         ind1=1
         id=id-10
      end if
      idi=id
      if(id.ne.1) then
         sd=dnrm0(n,w(nd),1)+1
         delta=d1mach(4)**(-0.5)
C modified 16-july-2005. Before: delta=1.d8
      else
         s0=ro
      end if
      r1=0
      r2=gigant
      k=0
C
C     Calculo del paso minimo, donde se satura una restriccion lineal,
C     segun la direccion de descenso calculada
C
      if(ira.gt.0) then
         do 10 i=1,n
            ii=nd0+i
            j=0
            if(ira.gt.1) then
               if(cs(i).le.gig1 .and. (ire(i).eq.0 .and. w(ii).gt.
     &            eps)) then
                  s=(cs(i)-x(i))/w(ii)
                  if(s.lt.r2 .and. s.gt.r1) then
                     r2=s
                     ip=i
                  end if
                  j=1
               end if
            end if
            if(j.eq.0 .and. ira.ne.2) then
               if(ci(i).ge.-gig1 .and. (ire(i).eq.0 .and. w(ii).lt.
     &            -eps)) then
                  s=(ci(i)-x(i))/w(ii)
                  if(s.lt.r2 .and. s .gt.r1) then
                     ip=-i
                     r2=s
                  end if
               end if
            end if
10       continue
      end if
      do 20 i=1,md
         ni=n+i
         i2=icd0+i
         if(ire(ni).eq.0) then
            if(w(i2).gt.eps) then
               s=-w(ni)/w(i2)
               if(s.le.r2 .and. s.gt.r1) then
                  r2=s
                  ip=ni
               end if
            end if
         end if
20    continue
C
C    Test de acotacion inferior para el funcional
C
      if(r2.eq.gigant .and. id.eq.0) then
         id=-1
         return
      end if
C
C     Calculo de los pasos donde se saturan las restricciones presentes
C     en el funcional
C
c      r3=r2
c     SS 03_2006 +0.0d0 added to fix an optimization bug with gfortran 4.0.2
      r3=r2+0.0d0
      ro=r2
      do 30 i=1,mif
         k1=k+1
         ni=nmd+i
         ii=iad0+i
         if(abs(ire(ni)).eq.2) s=w(ii)
         if((ire(ni).eq.2 .and. s.lt.-eps) .or.
     &      (ire(ni).eq.-2 .and. s.gt.eps)) then
            s1=-w(ni)/s
            if(s1.le.r2 .and. s1.gt.r1) then
               if(r3.eq.gigant .and. id.eq.2) then
                  w(nmd+k1)=s1
                  k=k1
                  ipvt(k)=ni
                  if(s1.lt.ro) then
                     ro=s1
                     ip=ni
                  end if
               else
                  r2=s1
                  ip3=ni
               end if
            end if
         end if
30    continue
      do 40 i=1,mdf
         k1=k+1
         ni=ndf+i
         if(ire(ni).ne.1) s=w(iad0+mif+i)
         if((ire(ni).eq.2 .and. s.lt.-eps) .or.
     &      (ire(ni).eq.0 .and. s.gt.eps)) then
            s1=-w(ni)/s
            if(s1.le.r2 .and. s1.gt.r1) then
               if(r3.eq.gigant .and. id.eq.2) then
                  w(nmd+k1)=s1
                  k=k1
                  ipvt(k)=ni
                  if(s1.lt.ro) then
                     ro=s1
                     ip=ni
                  end if
               else
                  r2=s1
                  ip3=ni
               end if
            end if
         end if
40    continue
C
C     Calculo del paso optimo en la direccion de descenso
C
      if(r3.eq.gigant .and. id.eq.2) then
         s=ddot(n,w,1,w(nd),1)
         if(k.gt.0) then
            id1=iad0-nmd
            ro1=0
            if(s.lt.-eps .and. ro1.lt.r2) then
5010           l=0
               r1=ro1
               ro1=r2
               do 50 i=1,k
                  ii=nmd+i
                  if(w(ii).le.ro1 .and. w(ii).gt.r1) then
                     if(w(ii).lt.ro1) l=0
                     ro1=w(ii)
                     w(ii)=w(nd1+l)
                     w(nd1+l)=ro1
                     l=l+1
                     j=ipvt(i)
                     ipvt(i)=ipvt(l)
                     ipvt(l)=j
                  end if
50             continue
C
C     Calculo de la derivada en la direccion de descenso en los puntos
C     x+ro*d  si la direccion de descenso es una direccion del nucleo
C     del conjunto de restricciones activas (ID=2)
C
               do 60 i=1,l
                  j=ipvt(i)
                  ii=i-1
                  i1=nmd+i
                  j1=j-nmd
                  if(j.le.ndf) then
                     if(ire(j).eq.2) then
                        s=s-2*w(j+id1)
                     else if(ire(j).eq.-2) then
                        s=s+2*w(j+id1)
                     end if
                  else
                     if(ire(j).eq.0) then
                        s=s+w(j+id1)
                     else if(ire(j).eq.2) then
                        s=s-w(j+id1)
                     end if
                  end if
60             continue
               if(s.ge.-eps .and. ind1.eq.1) then
                  iip=ipvt(m+1)-nmd
                  s1=ddot(n,w(nd),1,a(1,iip),1)
                  if(iip.gt.ndf .or. s1.gt.eps) s=s+s1
                  ind1=11
               end if
               if(s.lt.-eps .and. ro1.lt.r2) go to 5010
            end if
         end if
         if(s.lt.-eps) then
            id=-1
            return
         else
            if(ind1.eq.2) then
               id=0
               return
            else
               ipvt(1)=ip
               id=1
            end if
         end if
      else
         if(r2.lt.r3) ip=ip3
         if(id.eq.1) then
            s=min(r2,s0)
            if(s.eq.s0) then
               id=2
               if(r2.eq.s0) then
                  id=3
                  ipvt(1)=ip
               end if
            else
               ipvt(1)=ip
            end if
            ro=s
         else
            if(ind1.eq.2) then
               id=0
               return
            end if
            ro=r2
            id=1
            ipvt(1)=ip
         end if
      end if
C
C     Se comprueba si se verifica el test de acotacion sobre el paso en
C     caso de direccion de curvatura negativa o direccion del nucleo
C
      if(idi.ne.1) then
         s=ro*sd
         if(s.gt.delta) then
            id=id+10
         end if
      end if
      end
