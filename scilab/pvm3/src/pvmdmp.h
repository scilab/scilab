
/* $Id: pvmdmp.h,v 1.1 2001/04/26 07:47:11 scilab Exp $ */

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
 *	pvmdmp.h
 *
$Log: pvmdmp.h,v $
Revision 1.1  2001/04/26 07:47:11  scilab
Initial revision

 * Revision 1.4  1997/06/25  22:09:33  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.3  1997/03/06  21:10:59  pvmsrc
 * 		- moved TIDONNODE, TIDISNODE  macro to pvmmimd.h
 * 		- mpp_load prototype changed
 * 		- mpp_input protoype changed
 *
 * Revision 1.2  1997/01/28  19:28:20  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:27  pvmsrc
 * Initial revision
 *
 * Revision 1.6  1995/07/25  17:37:26  manchek
 * mpp_output returns int
 *
 * Revision 1.5  1995/06/16  16:12:47  manchek
 * declare special mpp_load for PGON
 *
 * Revision 1.4  1995/05/30  17:48:30  manchek
 * mpp_free() takes struct task instead of tid.
 * Refined TIDISNODE macro
 *
 * Revision 1.3  1995/02/01  21:32:54  manchek
 * added nenv and envp args to mpp_load
 *
 */
#include "pvmmimd.h"

#define TIMEOUT     10000       /* for select(); in usec */

void mpp_init __ProtoGlarp__((int *argc, char **argv));
void mpp_free __ProtoGlarp__((struct task *tp));
#if defined(IMA_PGON)
int mpp_load __ProtoGlarp__(( struct waitc_spawn *wxp));
#else
int mpp_load __ProtoGlarp__((int flags, char *name, char **argv, int count,
    int *tids, int ptid, int nenv, char **envp));
#endif
int mpp_input __ProtoGlarp__((void));
int mpp_mcast __ProtoGlarp__((struct pkt *pp, int *tids, int ntask));
int mpp_output __ProtoGlarp__((struct task *tp, struct pkt *pp));
int mpp_probe __ProtoGlarp__((void));
void mpp_kill __ProtoGlarp__((struct task *tp, int signum));
void mpp_cleanup __ProtoGlarp__((void));

void wakesig __ProtoGlarp__((int sig));
