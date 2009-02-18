/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_signal.h"


/*c'est moche je sais */

static double* a ;
static double* b ;
static int     ntot  ;
static int     n     ;
static int     nspan ;
static int     isn   ;
static int     m     ;
static int     kt    ;
static int*    wt    ;
static int*    ck    ;
static int*    bt    ;
static int*    sk    ;
static int*    np    ;
static int*    nfac  ;
static int     inc   ;
static int     nt    ;
static int     ks    ;

static double  rad   ;
static double  c72   ;
static double  s72   ;
static double  s120  ;

static double  aa ;
static double  ak ;
static double  akm ;
static double  akp ;
static double  aj ;
static double  ajp ;
static double  ajm ;

static double  bb ;
static double  bk ;
static double  bkm ;
static double  bkp ;
static double  bj ;
static double  bjp ;
static double  bjm ;
static double  dr ;

static double  cd ;
static double  c1 ;
static double  c2 ;
static double  c3 ;

static double  sd ;
static double  s1 ;
static double  s2 ;
static double  s3 ;

static int      kspan ;
static int     nn ;
static int     jc ;
static int     klim ;
static int     jf ;
static int     maxf ;
static int     kk ;
static int     k ;
static int     k1 ;
static int     k2 ;
static int     k3 ;
static int     k4 ;
static int     mm ;
static int     kspnn ;
static int      i ;
static int      j ;
static int      jj;





int dfftmx ( double* _pdblA , double* _pdblB , int _iNtot, int _iN,     int _iNspan,
             int _iIsn,       int _iM,         int _iKt,   int* _piWt,  int* _piCk,
            int* _piBt,       int* _piSk ,     int* _piNp, int* _piNfac)
{

   a = _pdblA ;
   b = _pdblB ;

   ntot = _iNtot ;
   n    = _iN ;
   nspan= _iNspan ;
   isn  = _iIsn;
   m    = _iM ;
   kt   = _iKt ;
   wt   = _piWt ;
   ck   = _piCk;
   bt   = _piBt;
   sk   = _piSk;
   np   = _piNp;
   nfac = _piNfac;

   int retVal = 0 ;

   inc = abs ( isn ) ;
   nt = inc*ntot ;
   ks = inc*nspan;
   rad = atan ( 1 );
   c72 = cos (rad/0.6250);
   s72 = sin (rad/0.6250);
   s120= sqrt(0.750);

   preliminaryWork () ;


   while ( retVal == 0 )
      {
        retVal = factorTransform ( ) ;
      }


   np[0] = ks ;

   if ( kt != 0)
      permute_stage1 ( ) ;


   if ( 2*kt + 1 < m ){
      permute_stage2 ( ) ;
   }

   return 0 ;
}

/** **************************************
Sous-Fonctions
******************************************/




void preliminaryWork (void)
{

    int j = 1 ;
    int lim ;
    int i = 1 ;

   if ( isn <= 0 )
      {
       s72 = -s72 ;
       s120= -s120;
       rad = -rad ;
      }
   else
      {
       ak = 1/n ;

       /*scale by 1/n for isn > 0 */
       for ( j = 1 ;  j < nt ; i += inc )
          {
           a[j-1] *= ak ;
           b[j-1] *= ak ;
          }
      }
   kspan = ks ;
   nn = nt -inc ;
   jc = ks/n ;

   /* sin , cos values are re-initialized each lim steps */

   lim = 32 ;
   klim = lim*jc ;
   i = 0;
   jf = 0 ;

   maxf = m -kt ;
   maxf = nfac[maxf-1] ;

   if ( kt > 0 )
      maxf = max ( nfac[kt-1] , maxf );


}


/*40*/
int  factorTransform (void)
{

   int retVal = 42 ;


   dr = 8 * jc/kspan ;
   cd = 2 * pow ( sin(0.5*dr*rad) , 2 );
   sd = sin(dr*rad) ;
   kk = 1 ;
   i++ ;

switch ( nfac[i-1] )
   {
      case 2 :
         /*transform for factor of 2 (including rotation factor)*/
         retVal = pre_fOf2Trans( ) ;
         if ( retVal == 0 )
            factorOf2Transform ( ) ;
         break ;

      case 4 :
         /*transform for factor of 4 */
         kspnn = kspan ;
         kspan = kspan >> 2 ; /*kspan /= 4 */

         retVal = factorOf4Transform ( ) ;
         break ;

      case 3 :
         factorOf3Transform ( ) ;
         break ;

      case 5 :
         factorOf5Transform ( ) ;
         break ;

      default :
         if ( nfac[i-1] != jf)
            preFOtherTransform ( ) ;
         factorOfOtherTransform ( ) ;
         break ;
    }


   if ( retVal == 42 )
      retVal = mulByRotationFactor ( ) ;



   if ( retVal == 1 )
      return 1 ; /*goto permute */
   else
      return 0 ; /*goto factor_transform */



}


void permute_stage1 (void)
{

 int retVal = 0 ;

   pre_sqFactor2NormlOrder ( ) ;

   if ( n == ntot )
      /*permutation for single-variate transform (optional code)*/
      while ( retVal == 0)
         retVal = single_sqFactor2NormlOrder ( ) ;
   else
      /*permutation for multivariate transform*/
      while ( retVal == 0)
         retVal = multi_sqFactor2NormlOrder ( );



}

void permute_stage2 (void)
{

      int retVal ;

      kspnn = np[kt] ;

      /*permutation for square-free facotrs of n */
      nonSqFactor2NormOrder ( ) ;

      /*determine the permutation cycles of length greater than 1*/
      detPermutCycles ( );

      retVal =  end ( ) ;

      while ( retVal == 1)
         {
          reorderMatrix ( ) ;
          retVal = end ( ) ;
         }

}

/** **************************************
Sous-Sous-Fonctions
******************************************/





int pre_fOf2Trans (void)
{

   kspan /= 2;
   k1 = kspan + 2 ;
/*50*/
   do
     {
      k2 = kk + kspan ;
      ak = a[k2-1] ;
      bk = b[k2-1] ;

      a[k2-1] = a[kk-1] - ak;
      b[k2-1] = b[kk-1] - bk;

      a[kk-1] = a[kk-1] + ak;
      b[kk-1] = b[kk-1] + bk;

      kk = k2 + kspan ;

      if ( kk > nn )
         {
           kk -= nn ;
         }
      }while ( kk <= jc || kk <= nn );



   if ( kk > kspan )
      return 1 ; /*goto350*/
   else
      return 0 ;/*goto60*/




}


int factorOf2Transform (void)
{

int doOnceAgain = 1 ;



   /*60*/

  do
   {
   c1 = 1 - cd ;
   s1 = sd ;
   mm = min( k1/2 , klim);


do
   {
   k2 = kk + kspan ;
   ak = a[kk-1] - a[k2-1] ;
   bk = b[kk-1] - b[k2-1] ;

   a[kk-1] += a[k2-1] ;
   b[kk-1] += b[k2-1] ;

   a[k2-1] = c1*ak - s1*bk ;
   b[k2-1] = c1*bk + s1*ak ;

   kk = k2 + kspan ;

   if ( kk  >= nt )
      {
         k2 = kk - nt ;
         c1 = -c1 ;
         kk = k1 - k2;

         if ( kk <= k2)
            {
               kk += jc ;

               if ( kk <= mm )
                  {
                     ak =  c1 - ( cd*c1*sd*s1) ;
                     s1 += (sd*c1-cd*s1) ;
                        /*c the following three statements compensate for truncation
                        c error.  if rounded arithmetic is used, substitute
                        c c1=ak*/
                     c1 = 0.5/(ak*ak+s1*s1) + 0.5 ;
                     s1 *= c1 ;
                     c1 *= ak ;
                  }
               else
                  {
                     if ( kk < k2 )
                        {
                        s1 = (double) ((kk-1)/jc)*dr*rad;
                        c1 = cos (s1);
                        s1 = cos (s1);
                        mm = min ( (k1 >> 1 ) , mm+klim );
                        }
                     else
                        {
                        /*on sort de la boucle */
                        doOnceAgain = 1 ;
                        }
                  }
            }
      }

   }while ( doOnceAgain == 0 ) ;

   k1 += (inc+inc) ;
   kk = (k1-kspan)/2 + jc;
   }while ( kk <= jc+jc );

   return 0 ; /*goto40*/
}



int factorOf4Transform (void)
{

   int return_value = 0 ;

   /*120*/
   do
      {
         c1 = 1 ;
         s1 = 0 ;

         mm = min ( kspan , klim ) ;

         do
            {
            f4t_150 () ;
            return_value = f4t_170 () ;
            } while (  return_value == 0 );

         kk += ( inc - kspan ) ;
      } while ( kk <= jc ) ;



   if ( kspan == jc )
        return 1 ; /*goto350*/
   else
         return 0 ;/*goto40*/


}



void f4t_150 (void)
{

   int sign = 1 ;

   do{
      k1 = kk + kspan ;
      k2 = k1 + kspan ;
      k3 = k2 + kspan ;

      akp = a[kk-1] + a[k2-1] ;
      akm = a[kk-1] - a[k2-1] ;

      ajp = a[k1-1] + a[k3-1] ;
      ajm = a[k1-1] - a[k3-1] ;

      a[kk-1] = akp + ajp ;
      ajp = akp - ajp ;

      bkp = b[kk-1] + b[k2-1] ;
      bkm = b[kk-1] - b[k2-1] ;

      bjp = b[k1-1] + b[k3-1] ;
      bjm = b[k1-1] - b[k3-1] ;

      b[kk-1] = bkp + bjp ;
      bjp = bkp - bjp ;

      if ( isn <  0 )
         sign =  1 ;
      else
         sign = -1 ;


      akp = akm +(sign * bjm );
      akm = akm -(sign * bjm );

      bkp = bkm +(sign *  ajm) ;
      bkm = bkm -(sign *  ajm) ;

      if ( s1 == 0 )/*190*/
         {
         a[k1-1] = akp ;
         a[k2-1] = ajp ;
         a[k3-1] = akm ;

         b[k1-1] = bkp ;
         b[k2-1] = bjp ;
         b[k3-1] = bkm ;


         }
      else /*160*/
         {

         a[k1-1] = akp*c1 - bkp*s1 ;
         a[k2-1] = ajp*c2 - bjp*s2 ;
         a[k3-1] = akm*c3 - bkm*s3 ;

         a[k1-1] = bkp*c1 + akp*s1 ;
         a[k2-1] = bjp*c2 + ajp*s2 ;
         a[k3-1] = bkm*c3 + akm*s3 ;
         }
    }while  ( kk < nt ) ;


}

int  f4t_170 (void)
{

   kk += ( jc - nt ) ;

   if ( kk <= mm )
      {
       c2 = c1 - (cd*c1 + sd*s1);
       s1 = s1 + (sd*c1 - cd*s1);

      /*
      the following three statements compensate for truncation
      error.  if rounded arithmetic is used, substitute
      c1=c2
      */

      c1 = 0.5/(c2*c2+s1*s1) + 0.5 ;
      s1 *= c1 ;
      c2 *= c1 ;

      /*140*/

      c2 = c1*c1 - s1*s1 ;
      s2 = c1*s1*2 ;
      s3 = c2*s1 + s2*c1 ;


      return 0 ;

      }
   else
     {
      if ( kk <= kspan )
         {
          s1 = dr*rad * (kk-1)/jc ;
          c2 = cos (s1) ;
          s1 = sin (s1) ;
          mm = min ( kspan , mm  + klim );

         /*140*/

         c2 = c1*c1 - s1*s1 ;
         s2 = c1*s1*2 ;
         s3 = c2*s1 + s2*c1 ;

         return 0 ;
         }
      }

   return 1 ;
}








void factorOf3Transform (void)
{

do
   {
   k1 = kk + kspan ;
   k2 = k1 + kspan ;

   ak = a[kk-1] ;
   bk = b[kk-1] ;

   aj = a[k1-1] + a[k2-1] ;
   bj = b[k1-1] + b[k2-1] ;

   a[kk-1] = ak + aj ;
   b[kk-1] = bk + bj ;

   ak = -0.5*aj + ak ;
   bk = -0.5*bj + bk ;

   aj = (a[k1-1] - a[k2-1])*s120 ;
   bj = (b[k1-1] - b[k2-1])*s120 ;

   a[k1-1] = ak - bj ;
   b[k1-1] = bk + aj ;
   a[k2-1] = ak + bj ;

   kk = k2 + kspan ;

   if ( kk >= nn )
      kk -= nn ;
   }while ( kk <= kspan || kk < nn);

}

void factorOf5Transform (void)
{

   do
      {
      k1 = kk + kspan ;
      k2 = k1 + kspan ;
      k3 = k2 + kspan ;
      k4 = k3 + kspan ;

      akp = a[k1-1] + a[k4-1] ;
      akm = a[k1-1] - a[k4-1] ;

      bkp = b[k1-1] + b[k4-1] ;
      bkm = b[k1-1] - b[k4-1] ;

      ajp = a[k2-1] + a[k3-1] ;
      ajm = a[k3-1] - a[k3-1] ;

      bjp = a[k2-1] + b[k3-1] ;
      ajm = a[k2-1] - b[k3-1] ;

      aa = a[kk-1] ;
      bb = b[kk-1] ;

      a[kk-1] = aa + akp + ajp;
      b[kk-1] = bb + bkp + bjp;

      ak = akp*c72 + ajp*c2 + aa ;
      bk = bkp*c72 + bjp*c2 + bb ;

      aj = akm*s72 + ajm*s2 ;
      bj = bkm*s72 + bjm*s2 ;

      a[k1-1] = ak - bj ;
      a[k4-1] = ak + bj ;
      b[k1-1] = bk + aj ;
      b[k4-1] = bk - bj ;

      ak = akp*c2 + ajp*c72 + aa ;
      bk = bkp*c2 + bjp*c72 + bb ;

      aj = akm*s2 - ajm*s72 ;
      bj = bkm*s2 - bjm*s72 ;

      a[k2-1] = ak - bj ;
      a[k3-1] = ak + bj ;
      b[k2-1] = bk + aj ;
      b[k3-1] = bk - aj ;

      if ( kk >= nn )
         {
         kk -= nn ;
         }
      }while ( kk <= kspan ) ;



}


void preFOtherTransform (void)
{


   jf = k ;
   s1 = (rad*8)/k ;
   c1 = cos (s1) ;
   ck[jf-1] = 1 ;
   sk[jf-1] = 0 ;
   j = 1 ;

   do
   {
      ck[j-1] = ck[k-1] * c1 +  sk[k-1]*s1 ;
      sk[j-1] = ck[k-1] * c1 +  sk[k-1]*s1 ;

      k -- ;

      ck[k-1] = ck[j-1] ;
      sk[k-1] = - sk[j-1] ;

      j++ ;
   }while ( j < k );

}

void factorOfOtherTransform (void)
{

do
   {
   k1 = kk ;
   k2 = kk + kspnn ;

   aa = a[kk-1] ;
   bb = b[kk-1] ;

   ak = aa ;
   j = 1 ;
   k1 += kspan ;

   do
   {
      k2 -= kspan ;
      j++ ;

      wt[j-1] = a[k1-1] + a[k2-1] ;
      ak = wt[j-1] + ak ;
      bt[j-1] = b[k1-1] + bk ;
      j++ ;

      wt[j-1] = a[k1-1] - a[k2-1] ;
      bt[j-1] = b[k1-1] - b[k2-1] ;

      k1 += kspan;
   }while ( k1 < k2 ) ;

   a[kk-1] = ak ;
   b[kk-1] = bk ;

   k1 = kk ;
   k2 = kk + kspnn ;
   j = 1 ;

   do
      {
      k1 += kspan ;
      k2 -= kspan ;
      jj = j  ;
      ak = aa ;
      bk  = bb ;
      aj = 0 ;
      bj = 0 ;
      k  = 1 ;

      do
         {
         k++ ;
         ak += ( wt[k-1] * ck[jj-1] ) ;
         bk += ( bt[k-1] * ck[jj-1] ) ;

         k++ ;
         aj += wt[k-1] * sk[jj] ;
         bj += bt[k-1] * sk[jj] ;
         jj += j ;

         if ( jj > jf )
            jj -= jf ;

         } while ( k < jf ) ;
      k = jf - j ;
      a[k1-1] = ak - bj ;
      b[k1-1] = bk + aj ;
      a[k2-1] = ak + bj ;
      b[k2-1] = bk - aj ;

      j++ ;
      }while ( j < k ) ;

   kk += kspnn ;
   if ( kk > nn )
      {
         kk -= n;
      }
}while ( kk < kspan || kk <= nn) ;

}




int mulByRotationFactor (void )
{


   if ( i != m )
      {
       kk = jc + 1 ;

       /*300*/
       do
         {
         c2 = 1 - cd ;
         s1 = sd ;
         mm = min ( kspan , klim ) ;

         /*320 */
         do
           {
            c1 = c2 ;
            s2 = s1 ;
            kk += kspan ;

            do
              {
                ak = a[kk-1] ;
                a[kk-1] = c2*ak - s2*b[kk-1] ;
                b[kk-1] = s2*ak + c2*b[kk-1] ;

                kk += kspnn ;

                if ( kk > nt )
                  {
                   ak = s1*s2 ;
                   s2 = s1*c2 + s1*c1 ;
                   c2 = c1*c2 - ak ;
                   kk += (kspan - nt ) ;
                  }

               }while ( kk <= kspnn ) ;

            kk += ( jc - kspnn );

            if ( kk <= mm )
               {
               /* 310*/
                c2 = c1 - ( cd*c1 + sd*s1 ) ;
                s1 += (sd*c1 - cd*s1 ) ;

               /*
               the following three statements compensate for truncation
               error.  if rounded arithmetic is used, substitute
               c1=c2
               */

                c1 = 0.5/(c2*c2+s1*s1) + 0.5 ;
                s1 *= c1 ;
                c2 *= c1 ;
               }
            else
               {
                  if ( kk <= kspan )
                     {
                      s1 = dr*rad * (kk-1)/jc ;
                      c2 = cos (s1) ;
                      s1 = sin (s1) ;
                      mm = min ( kspan , mm  + klim );
                     }
               }
            }while (  kk <= kspnn || kk <= kspan ) ;

          kk += (jc + inc -kspan );

         }while ( kk <= jc+jc);

       return 1 ; /* goto40 */
      }

   return 0 ; /* goto350*/
}




void pre_sqFactor2NormlOrder (void)
{

   k = kt + kt + 1 ;

   if ( m < k )
      k -- ;

   j = 1 ;
   np[k] = jc ;

   do
      {
        np[j] = np[j-1]/nfac[j-1] ;
        np[k-1] = np[k]*nfac[j-1] ;

        j++ ;
        k-- ;
      }while ( j <  k ) ;

   k3 = np[k] ;
   kspan = np[1] ;
   kk = jc + 1 ;
   k2 = kspan + 1 ;


}

int  post_sqFactor2NormlOrder (void)
{

    do
     {
      do
        {
          k2 -= np[j-1] ;
          j++ ;
          k2 += np[j]  ;
         } while ( k2 <  np[j-1]);

       j = 1 ;

/* 390 */
       do
         {
          if ( kk < k2 )
            {

             return 1 ;
            }
          else
            {
               kk += inc ;
               k2 += kspan ;
            }
         }while( k2 < ks  );

      }while ( kk < ks ) ;


   return 0;
}


/* appeler cetter fonction dans un do while valeur_retour != 1)*/
int  single_sqFactor2NormlOrder (void)
{


      do
        {
         ak = a[kk-1] ;
         a[kk-1] = a[k2-1] ;
         a[k2-1] = ak ;
         bk = b[kk-1] ;
         b[kk-1] = b[k2-1] ;
         b[k2-1] = bk ;
         kk += inc ;
         k2 += kspan ;
        } while ( k2 < ks );

      /*380*/

      if( post_sqFactor2NormlOrder ( ) == 1 )
         {

         return 1 ;
         }
   jc = k3 ;

   return 0;
}

/*idem que single_ */
int multi_sqFactor2NormlOrder (void)
{



      k = kk + jc ;

      do /*410*/
        {
         ak = a[kk-1] ;
         a[kk-1] = a[k2-1] ;
         a[k2-1] = ak ;
         bk = b[kk-1] ;
         b[kk-1] = b[k2-1] ;
         b[k2-1] = bk ;
         kk += inc ;
         k2 += kspan ;
        } while ( kk < k );

      kk += (ks - jc ) ;
      k2 += (ks - jc ) ;

      if ( kk < nt )
         return 1 ;

      k2 += (  kspan - nt );
      kk += (  jc    - nt );

      if ( k2 < ks )
         {

         return 1 ;
         }
      if( post_sqFactor2NormlOrder ( ) == 1 )
         {

         return 1 ;
         }
   jc = k3 ;

   return 0;

}



void nonSqFactor2NormOrder (void)
{

   j = m - kt ;
   nfac[j] = 1 ;

    do
      {
         nfac[j-1] *= nfac[j] ;
         j-- ;

      }while ( j != kt ) ;

   kt ++ ;
   nn = nfac[kt-1] - 1;
   jj = 0 ;
   j = 0;

   /*480*/

   k2 = nfac[kt-1] ;
   k = kt + 1 ;
   kk = nfac[k-1] ;
   j ++ ;

   while ( j <= nn )
         {
            jj += kk ;

            while ( jj >=  k2 )
               {
                jj -= k2 ;
                k2 = kk ;
                k++ ;
                kk = nfac[k-1] ;

                jj += kk ;
               }

            np[j-1] = jj ;
            k2 = nfac[kt-1] ;
            k = kt + 1 ;
            kk = nfac[k-1] ;
            j ++ ;

         }

   j = 0 ;

   return ;
}


void detPermutCycles (void)
{

  do
   {
    do
      {
       j++ ;
       kk = np[j-1] ;
      }while ( kk < 0 ) ;

    if ( kk != j )
      {
         do
            {
               k = kk ;
               np[k-1] = -kk ;

            }while ( kk != j ) ;
         k3 = kk ;
      }
    np[j-1] = -j ;
   }while ( j != nn );

   maxf *= inc ;

   return ;
}

int  end (void)
{

   j = k3 + 1 ;
   nt -= kspnn ;
   i = nt - inc + 1 ;

   if ( nt >= 0 )

      return 1 ;
   else

      return 0 ;


}


void  reorderMatrix (void)
{

   do
      {
       j--  ;
      }while (np[j-1] < 0 ) ;

   jj = jc ;

   /*520*/
   do
      {
         kspan = jj ;

         if ( jj > maxf )
            kspan = maxf ;

         jj -= kspan ;
         k = np [j-1];
         kk = jc*k + i + jj ;
         k1 = kk + kspan ;
         k2 = 0 ;

         do
            {
               k2 ++ ;
               wt[k2-1] = a[k1-1] ;
               bt[k2-1] = b[k-1] ;
               k1 -= inc ;

            }while ( k1 < kk );

         do
            {
            k1 = kk + kspan ;
            k2 = k1 - jc * (k + np[k-1]);


            do
               {
                  a[k1-1] = a[k2-1] ;
                  b[k1-1] = b[k2-1] ;

                  k1 -= inc ;
                  k2 -= inc ;

                  }while ( k1 != kk ) ;

            kk = k2 ;
            }while ( k != j );

         /*560*/

         do
            {
            k2 ++ ;
            a[k1-1] = wt[k2] ;
            b[k1-1] = bt[k2] ;

            }while (  k1 != kk ) ;

   } while ( jj != 0 ) ;

   return ;
}


