/*
 *  PURPOSE
 *     clcg4 generator stuff
 *
 *  AUTHORS
 *     The following code is from L'Ecuyer and Andres "A Randow Number based
 *     on the combinaison of Four LCG" (distributed at the Pierre L'Ecuyer
 *     home page with a corresponding paper).
 *
 *  NOTES
 *     The original code was slightly modified by Bruno Pincon for inclusion
 *     in Scilab. 
 *
 *     list of main modifs :
 *
 *       - lot of routine 's names have changed to have some kind of
 *         uniformity with the others generators 
 *
 *       - add a var is_init so that initialisation is performed inside
 *         this module (to simplify the interface). And bring modif in
 *         the different routines :
 *            if (!is_init) then proceed to initialisation ...
 *
 *       - add a routine advance_state_clcg4 (for compatibility with the
 *         old package (Scilab used this feature))
 *
 *       - I have change the generator (clcg4 routine) so as it has the
 *         form (1) in place of (2) (see the joined paper of L'Ecuyer &
 *         Andres) :
 *
 *         From the 4 LCG :
 *
 *            x_{j,n} = a_j * x_{j,n-1} mod m_j    0 <= j <= 3
 *
 *         The output with form (2) (original form in this code) :
 *         
 *            z_n = ( sum_j  delta_j * x_{j,n} / m_j ) mod 1
 *
 *         have been changed in the form (1) :
 *
 *           z_n = ( sum_j  delta_j * x_{j,n} ) mod m_1 (then u_n = z_n / m_1)
 *         
 *         to have some "uniformity" with all the others generators (which
 *         gives integers). Also it is better for the uin(a,b) generation
 *         to start from integers.
 */


/*---------------------------------------------------------------------*/
/* clcg4.c   Implementation module                                     */
/*---------------------------------------------------------------------*/

#include "../graphics/Math.h" /* to use sciprint */
#include "clcg4.h"
#include <math.h>             /* for floor */

/***********************************************************************/
/* Private part.                                                       */
/***********************************************************************/

#define H   32768               /* = 2^15 : use in MultModM.           */

static long aw[4], avw[4],      /*   a[j]^{2^w} et a[j]^{2^{v+w}}.     */
            a[4] = { 45991, 207707, 138556, 49689 },
            m[4] = { 2147483647, 2147483543, 2147483423, 2147483323 };

static long Ig[4][Maxgen+1], Lg[4][Maxgen+1], Cg[4][Maxgen+1];
                     /* Initial seed, previous seed, and current seed. */


static int  is_init = 0;
static long v_default = 31;
static long w_default = 41;


static long MultModM (long s, long t, long M)
   /* Returns (s*t) MOD M.  Assumes that -M < s < M and -M < t < M.    */
   /* See L'Ecuyer and Cote (1991).                                    */
  {
  long R, S0, S1, q, qh, rh, k;

  if (s < 0)  s += M;
  if (t < 0)  t += M;
  if (s < H)  { S0 = s;  R = 0; }
  else
    {
    S1 = s/H;  S0 = s - H*S1;
    qh = M/H;  rh = M - H*qh;
    if (S1 >= H)
      {
      S1 -= H;   k = t/qh;   R = H * (t - k*qh) - k*rh;
      while (R < 0)  R += M;
      }
    else R = 0;
    if (S1 != 0)
      {
      q = M/S1;   k = t/q;   R -= k * (M - S1*q);
      if (R > 0)  R -= M;
      R += S1*(t - k*q);
      while (R < 0)  R += M;
      }
    k = R/qh;   R = H * (R - k*qh) - k*rh;
    while (R < 0) R += M;
    }
  if (S0 != 0)
    {
    q = M/S0;   k = t/q;   R -= k* (M - S0*q);
    if (R > 0)  R -= M;
    R += S0 * (t - k*q);
    while (R < 0)  R += M;
    }
  return R;
  }

void comp_aw_and_avw(long v, long w)
{
  int i, j;
  for (j = 0; j < 4; j++)
    {
      aw [j] = a [j];
      for (i = 1; i <= w; i++)
	aw [j]  = MultModM (aw [j], aw [j], m[j]);
      avw [j] = aw [j];
      for (i = 1; i <= v; i++)
	avw [j] = MultModM (avw [j], avw [j], m[j]);
    }
}

void init_clcg4(long v, long w)
{
  /* currently the scilab interface don't let the user chooses
   * v and w (always v_default and w_default) so this routine
   * is in the "private" part (also because initialisation is
   * always perform inside this module, depending of the var
   * is_init)
   */
  double sd[4] = {11111111., 22222222., 33333333., 44444444.};
  comp_aw_and_avw(v, w);
  set_initial_seed_clcg4(sd[0], sd[1], sd[2], sd[3]);
}

int verif_seeds_clcg4(double s0, double s1, double s2, double s3)
{
  /* verify that the seeds are "integers" and are in the good range */
  if ( s0 == floor(s0) && s1 == floor(s1) &&
       s2 == floor(s2) && s3 == floor(s3) &&
       1 <= s0  &&  s0 <= 2147483646      &&
       1 <= s1  &&  s1 <= 2147483542      &&
       1 <= s2  &&  s2 <= 2147483422      &&
       1 <= s3  &&  s3 <= 2147483322 )
    return ( 1 );
  else
    return ( 0 );
}
 
void display_info_clcg4()
{
  /* display the seeds range (in case of error) */
  sciprint("\n\r bad seeds for clcg4, must be integers with  s1 in [1, 2147483646]");
  sciprint("\n\r                                             s2 in [1, 2147483542]");
  sciprint("\n\r                                             s3 in [1, 2147483422]");
  sciprint("\n\r                                             s4 in [1, 2147483322]");
}


/*---------------------------------------------------------------------*/
/* Public part.                                                        */
/*---------------------------------------------------------------------*/


int set_seed_clcg4(int g, double s0, double s1, double s2, double s3)
{
  if (! is_init ) {init_clcg4(v_default,w_default); is_init = 1; };

  if ( verif_seeds_clcg4(s0, s1, s2, s3) )
    {
      Ig [0][g] = (long) s0; Ig [1][g] = (long) s1;
      Ig [2][g] = (long) s2; Ig [3][g] = (long) s3;
      init_generator_clcg4(g, InitialSeed);
      sciprint("\n\r => be aware that you have may lost synchronization");
      sciprint("\n\r    between the virtual gen %d and the others !", g);
      sciprint("\n\r    use grand(\"setall\", s1, s2, s3, s4) if you want recover it.");
      return ( 1 );
    }
  else
    {
      display_info_clcg4();
      return ( 0 );
    }
}

void get_state_clcg4(int g, double s[4])
{
  int j;
  if (! is_init ) {init_clcg4(v_default,w_default); is_init = 1; };
  for (j = 0; j < 4; j++)  s [j] = (double) Cg [j][g];
}

void init_generator_clcg4(int g, SeedType Where)
{
  int j;
  if (! is_init ) {init_clcg4(v_default,w_default); is_init = 1; };
  for (j = 0; j < 4; j++)
    {
      switch (Where)
	{
	case InitialSeed :
	  Lg [j][g] = Ig [j][g];   break;
	case NewSeed :
	  Lg [j][g] = MultModM (aw [j], Lg [j][g], m [j]);   break;
	case LastSeed :
	  break;
	}
      Cg [j][g] = Lg [j][g];
    }
}

void advance_state_clcg4(int g, int k)
{
  long int b[4];
  int i, j;

  if (! is_init ) {init_clcg4(v_default,w_default); is_init = 1; };

  for ( j = 0 ; j < 4 ; j++ )
    {
      b[j] = a[j];
      for ( i = 1 ; i <= k ; i++ )
	b[j] = MultModM( b[j], b[j], m[j]);
      Ig[j][g] = MultModM ( b[j], Cg[j][g], m[j] );
    }
  init_generator_clcg4(g, InitialSeed);
}
  
int set_initial_seed_clcg4(double s0, double s1, double s2, double s3)
{
  int g, j;

  if (! is_init )  comp_aw_and_avw(v_default,w_default);

  if ( ! verif_seeds_clcg4(s0, s1, s2, s3) )
    {
      display_info_clcg4();
      return ( 0 );
    };

  is_init = 1;
  Ig [0][0] = (long) s0;
  Ig [1][0] = (long) s1;
  Ig [2][0] = (long) s2;
  Ig [3][0] = (long) s3;
  init_generator_clcg4(0, InitialSeed);
  for (g = 1; g <= Maxgen; g++)
    {
      for (j = 0; j < 4; j++)
	Ig [j][g] = MultModM (avw [j], Ig [j][g-1], m [j]);
      init_generator_clcg4(g, InitialSeed);
    }
  return ( 1 );
}

unsigned long clcg4(int g)
{
  /* Modif Bruno : the generator have now the form (1) in place of (2) */

  long k,s;
  double u;

  if (! is_init ) {init_clcg4(v_default,w_default); is_init = 1; };

  /*  advance the 4 LCG */
  s = Cg [0][g];  k = s / 46693;
  s = 45991 * (s - k * 46693) - k * 25884;
  if (s < 0) s = s + 2147483647;  Cg [0][g] = s;
 
  s = Cg [1][g];  k = s / 10339;
  s = 207707 * (s - k * 10339) - k * 870;
  if (s < 0) s = s + 2147483543;  Cg [1][g] = s;

  s = Cg [2][g];  k = s / 15499;
  s = 138556 * (s - k * 15499) - k * 3979;
  if (s < 0) s = s + 2147483423;  Cg [2][g] = s;

  s = Cg [3][g];  k = s / 43218;
  s = 49689 * (s - k * 43218) - k * 24121;
  if (s < 0) s = s + 2147483323;  Cg [3][g] = s;

  /*  final step */
  u = (double)(Cg[0][g] - Cg[1][g]) + (double)(Cg[2][g] - Cg[3][g]);
  /*  we must do  u mod 2147483647 with u in [- 4294966863 ; 4294967066 ] : */
  if (u < 0) u += 2147483647;
  if (u < 0) u += 2147483647;
  if (u >= 2147483647) u -= 2147483647;
  if (u >= 2147483647) u -= 2147483647;

  return ((unsigned long) u );

}














