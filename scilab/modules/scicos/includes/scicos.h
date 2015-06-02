/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
#ifndef __SCICOS_H
#define __SCICOS_H

#include "machine.h"
#include "import.h"

/* maximum value for sum of number of inputs and outputs ports of a given
block of type 2 */
#define SZ_SIZE 60

/* maximum value for sum of number of inputs and outputs of a given block
of type 0 */
#define TB_SIZE 500

#ifdef _MSC_VER
typedef void (*voidf) ();
#else
typedef void (*voidf) (void);
#endif


/* Blocks prototypes */
#define I int
#define IP int*
#define DP double*
#define DPP double**
#define VP void*
#define DB scicos_block*
#define F scicos_flag
#define FP scicos_flag*

/*                    flag  nclock ntvec  rpar  nrpar ipar  nipar  u  nu */
#define ARGS_scicosm1 IP,    IP,    IP,    DP,   IP,  IP,   IP,   DP, IP

/* flag  nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  ni  outabl no */
#define ARGS_scicos0 FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP

/*       flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */
#define ARGS_scicos FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out, funptr */
#define ARGS_scicos2 FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,VP

/*        flag   nclock t    xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out, funptr, g ng */
#define ARGS_scicos2z FP,IP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,VP,DP,IP

/*       flag   nclock t    residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar  intabl  .... */

#define ARGS_scicosi FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP,DP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out  */
#define ARGS_scicosi2 FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP

/*        flag   nclockf t   residual xd   x    nx   z   nz   tvec   ntvec  rpar  nrpar ipar  nipar   args_in sz_in, n_in  args_out sz_out, n_out g ng */
#define ARGS_scicosi2z FP,IP,DP,DP,DP,DP,IP,DP,IP,DP,IP,DP,IP,IP,IP,DPP,IP,IP,DPP,IP,IP,DP,IP

/*         block     flag*/
#define ARGS_scicos4 DB, F

typedef void (*ScicosFm1) (ARGS_scicosm1);
typedef void (*ScicosF0) (ARGS_scicos0);
typedef void (*ScicosF) (ARGS_scicos);
typedef void (*ScicosF2) (ARGS_scicos2);
typedef void (*ScicosF2z) (ARGS_scicos2z);
typedef void (*ScicosFi) (ARGS_scicosi);
typedef void (*ScicosFi2) (ARGS_scicosi2);
typedef void (*ScicosFi2z) (ARGS_scicosi2z);
typedef void (*ScicosF4) (ARGS_scicos4);

typedef struct
{
    char const* name;
    ScicosF fonc;
} OpTab;

/**
 * @TODO add comment
 *
 * @param x_in
 * @param xptr_in
 * @param z__
 * @param work
 * @param zptr
 * @param modptr_in
 * @param oz
 * @param ozsz
 * @param oztyp
 * @param ozptr
 * @param iz
 * @param izptr
 * @param uid the uid values (stack formatted)
 * @param uidptr the uid length : uid_length = uidptr($+1) - uidptr($)
 * @param t0_in
 * @param tf_in
 * @param tevts_in
 * @param evtspt_in
 * @param nevts
 * @param pointi_in
 * @param outtbptr_in
 * @param outtbsz_in
 * @param outtbtyp_in
 * @param outtb_elem_in
 * @param nelem1
 * @param nlnk1
 * @param funptr
 * @param funtyp_in
 * @param inpptr_in
 * @param outptr_in
 * @param inplnk_in
 * @param outlnk_in
 * @param rpar
 * @param rpptr
 * @param ipar
 * @param ipptr
 * @param opar
 * @param oparsz
 * @param opartyp
 * @param opptr
 * @param clkptr_in
 * @param ordptr_in
 * @param nordptr1
 * @param ordclk_in
 * @param cord_in
 * @param ncord1
 * @param iord_in
 * @param niord1
 * @param oord_in
 * @param noord1
 * @param zord_in
 * @param nzord1
 * @param critev_in
 * @param nblk1
 * @param ztyp
 * @param zcptr_in
 * @param subscr
 * @param nsubs
 * @param simpar
 * @param flag__
 * @param ierr_out
 * @return <ReturnValue>
 */
int C2F(scicos) (double *x_in, int *xptr_in, double *z__,
                 void **work, int *zptr, int *modptr_in,
                 void **oz, int *ozsz, int *oztyp, int *ozptr,
                 char **iz, int *izptr, char **uid, int *uidptr, double *t0_in,
                 double *tf_in, double *tevts_in, int *evtspt_in,
                 int *nevts, int *pointi_in, void **outtbptr_in,
                 int *outtbsz_in, int *outtbtyp_in,
                 outtb_el * outtb_elem_in, int *nelem1, int *nlnk1,
                 void** funptr, int *funtyp_in, int *inpptr_in,
                 int *outptr_in, int *inplnk_in, int *outlnk_in,
                 double *rpar, int *rpptr, int *ipar, int *ipptr,
                 void **opar, int *oparsz, int *opartyp, int *opptr,
                 int *clkptr_in, int *ordptr_in, int *nordptr1,
                 int *ordclk_in, int *cord_in, int *ncord1,
                 int *iord_in, int *niord1, int *oord_in,
                 int *noord1, int *zord_in, int *nzord1,
                 int *critev_in, int *nblk1, int *ztyp, int *zcptr_in, int *subscr, int *nsubs, double *simpar, int *flag__, int *ierr_out);

/**
 * \brief Discrete time counter type
 *
 * Used on [evtdly4.c](@ref evtdly4), [m_frequ.c](@ref m_frequ) or
 * [time_delay.c](@ref time_delay) to manage time difference.
 *
 * Warning: effect maximum simulation time (in term of max simulation step).
 */
typedef long long int time_counter_t;

/**
*
*/
void callf(double *t, scicos_block * block, scicos_flag * flag);

/**
*
*/
int get_phase_simulation(void);

/**
*
*/
void set_block_error(int err);

/**
*
*/
double get_scicos_time(void);

/**
*
*/
void set_scicos_time(double t);

/**
*
*/
void set_pointer_xproperty(int *pointer);

/**
*
*/
void do_cold_restart(void);

/**
*
*/
int get_block_number(void);

/**
*
*/
void end_scicos_sim(void);

/**
*
*/
void Coserror(const char *fmt, ...);

/**
*
*/
void* funnum2(char *fname);
#endif

