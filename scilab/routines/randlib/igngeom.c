/*
 *  PURPOSE
 *     generate a random deviate from G(p) : the geometric
 *     law. If a r.v. X ~ G(p), X is the number of Bernouilli trials
 *     (B(p)) until succes is met. So X take its values in
 *
 *         {1, 2, 3, ...., } 
 *
 *     and  P(X=i) = p * (1-p)^(i-1) 
 *
 *  METHOD 
 *     inversion of the cdf leads to :
 *
 *     (1)   X = 1 + floor( log(1-u) / log(1-p) )
 *
 *     u being a random deviate from U[0,1).
 *
 *     by taking into account that 1-u follows also U(0,1)) this may be 
 *     replaced with X = ceil( log(u) / log(1-p) ) or 1 + floor(log(u)/log(1-p))
 *     which needs less work. But as ranf() provides number in [0,1[ , 0 may be 
 *     gotten and these formulae may give then +oo.
 * 
 *     With ranf() the max number is 1 - 2^(-32). This let us choose a safe min
 *     value for p (to avoid a +oo due to log(1-p)) in the following manner :
 *
 *        the max is gotten for  M = log(2^(-32)) / (-p)
 *
 *     (for very small |x|, the accurate func logp1(x):=log(1+x) return simply x)
 *
 *     and we want  M <= Rmax (near 1.798+308 in ieee 754 double)
 *
 *     so p >= 32 log(2)/Rmax which is near 1.234e-307 ; Says pmin = 1.3e-307.
 *     (anyway the results gotten for such small values of p are certainly 
 *      not meaningful...)
 *
 *  NOTE
 *     this function returns a double instead of an integer type : this is 
 *     to avoid an extra conversion because in scilab it will be a double.
 *
 *  ASSUMPTION
 *     p must be in [pmin,1]  (to do at the calling level).
 *
 *  AUTHOR
 *     Bruno Pincon (<Bruno.Pincon@iecn.u-nancy.fr>)
 *
 */
#include "../stack-c.h"
#include <math.h>

/* the external functions used  here : */
double F2C(logp1)(double *x);  /* celle-ci est ds SCI/routines/calelm/watan.f */
double C2F(ranf)();


double igngeom(double p)
{
  static double p_save = 1.0, ln_1_m_p = 0.0;
  double u;

  if ( p == 1 ) 
    return ( 1.0 );
  else if ( p != p_save )   /* => recompute log(1-p) */
    {
      p_save = p; u = -p; ln_1_m_p = F2C(logp1)(&u);
    };
  
  u = -C2F(ranf)();
  return ( floor( 1.0 + F2C(logp1)(&u)/ln_1_m_p) );
}      
    
