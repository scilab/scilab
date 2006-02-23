/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 *
 *	from: @(#)types.h 1.18 87/07/24 SMI
 *	from: @(#)types.h	2.3 88/08/15 4.0 RPCSRC
 */

/*
 * Rpc additions to <sys/types.h>
 */
#ifndef _RPC_TYPES_H
#define _RPC_TYPES_H

#define	bool_t	int
#define	enum_t	int
#define __dontcare__	-1

#ifndef FALSE
#	define FALSE	(0)
#endif
#ifndef TRUE
#	define TRUE	(1)
#endif
#ifndef NULL
#	define NULL	0
#endif

#define mem_alloc(bsize)	malloc(bsize)
#define mem_free(ptr, bsize)	free(ptr)

#ifndef makedev /* ie, we haven't already included it */
#ifndef __ABSC__
#include <sys/types.h>
#endif
#if (defined __MSC__) || (defined __ABSC__) || (defined __MINGW32__)
#undef FALSE
#undef TRUE 
#include <winsock.h>
typedef char * caddr_t;
#define bzero(x,n) memset(x,0,n)
#define bcopy(x,y,n) memcpy(x,y,n)
#define IEEEFP
#endif /* __MSC__ && __ABSC__ */

#endif /* makedev */

#if !(defined __MSC__) && !(defined __ABSC__) && ! (defined __MINGW32__)
#include <sys/time.h>
#endif
#ifdef __ABSC__
#include <time.h>
#endif

#ifndef INADDR_LOOPBACK
#define	INADDR_LOOPBACK		(u_long)0x7F000001
#endif

/** not useful for gcwin32 
#ifdef __GCWIN32__
#ifndef PASCAL 
#define PASCAL      __stdcall
#endif
u_long PASCAL htonl (u_long hostlong);
u_long PASCAL ntohl (u_long netlong);
#endif 
**/
/** for MINGWIN32   **/

#ifdef __MINGW32__
#ifndef PASCAL 
#define PASCAL      __stdcall
#endif
u_long PASCAL htonl (u_long hostlong);
u_long PASCAL ntohl (u_long netlong);
#endif 

#endif /* !_RPC_TYPES_H */
