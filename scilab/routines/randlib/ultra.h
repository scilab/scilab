/* 
FSU - ULTRA	The greatest random number generator that ever was
		or ever will be.  Way beyond Super-Duper.
		(Just kidding, but we think its a good one.)

Authors:	Arif Zaman (arif@stat.fsu.edu) and
		George Marsaglia (geo@stat.fsu.edu).

Date:		27 May 1992

Version:	1.05

Copyright:	To obtain permission to incorporate this program into
		any commercial product, please contact the authors at
		the e-mail address given above or at

		Department of Statistics and
		Supercomputer Computations Research Institute
		Florida State University
		Tallahassee, FL 32306.

See Also:	README		for a brief description
		ULTRA.DOC	for a detailed description

-----------------------------------------------------------------------
*/ 
/*
  File:	ULTRA.H
*/

extern float	uni(void),	vni(void);
extern double	duni(void),	dvni(void);
extern long	i32bit(void),	i31bit(void);
extern short	i16bit(void),	i15bit(void);
extern char	i8bit(void),	i7bit(void),	i1bit(void);
extern void	rinit(long unsigned congx, long unsigned shrgx);
extern char	swbstate[];
extern int	swbsize;
