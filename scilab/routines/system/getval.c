/* getval.f -- translated by f2c (version 19961017).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

IMPORT struct {
    doublereal stk[2];
} stack_;

#define stack_1 stack_

IMPORT struct {
    integer bot, top, idstk[60000]	/* was [6][10000] */, lstk[10000], leps, bbot, bot0, infstk[10000], gbot, gtop, isiz;
} vstk_;

#define vstk_1 vstk_

IMPORT struct {
    integer ids[24576]	/* was [6][4096] */, pstk[4096], rstk[4096], pt, niv, macr, paus, icall, krec;
} recu_;

#define recu_1 recu_

IMPORT struct {
    integer ddt, err, lct[8], lin[65536], lpt[6], hio, rio, wio, rte, wte;
} iop_;

#define iop_1 iop_

IMPORT struct {
    integer err1, err2, errct, toperr, errpt, ieee, catch__;
} errgst_;

#define errgst_1 errgst_

IMPORT struct {
    integer sym, syn[6], char1, fin, fun, lhs, rhs, ran[2], comp[3];
} com_;

#define com_1 com_

IMPORT struct {
    char alfa[63], alfb[63], buf[4096];
} cha1_;

#define cha1_1 cha1_

struct {
    integer wmac, lcntr, nmacs, macnms[600]	/* was [6][100] */, lgptrs[101], bptlg[1000];
} dbg_;

#define dbg_1 dbg_

IMPORT struct {
    integer lbot, ie, is, ipal, nbarg, ladr[1024];
} adre_;

#define adre_1 adre_

IMPORT struct {
    integer nbvars, iwhere[1024], nbrows[1024], nbcols[1024], itflag[1024], ntypes[1024], lad[1024], ladc[1024], lhsvar[1024];
} intersci_;

#define intersci_1 intersci_

/* Table of constant values */

static doublereal c_b2 = 10.;
static integer c__1 = 1;
static integer c__5 = 5;

/* Subroutine */ int getval_(s, dotdet)
doublereal *s;
logical *dotdet;
{
    /* Initialized data */

    static doublereal toto = 0.;
    static integer dot = 51;
    static integer d__ = 13;
    static integer e = 14;
    static integer plus = 45;
    static integer minus = 46;

    /* System generated locals */
    integer i__1;
    icilist ici__1;

    /* Builtin functions */
    double pow_di();
    /* Subroutine */ int s_copy();
    integer s_wsfi(), do_fio(), e_wsfi(), s_rsli(), do_lio(), e_rsli();

    /* Local variables */
#define cstk ((char *)&stack_1)
#define istk ((integer *)&stack_1)
    static integer expo;
#define sstk ((real *)&stack_1)
    static integer code0;
#define zstk ((doublecomplex *)&stack_1)
    static integer i__, k;
    extern /* Subroutine */ int getch_();
    static integer digit[25], ndgrec;
    static logical detdot;
    static integer ndgexp, expcor, sgnexp;
    static char string[31];
    static integer ndg;


/*     PURPOSE */
/*        get a number : on output s must be a double float */
/*                       which must be very close from the decimal */
/*                       number represented by the pattern of char */

/*        this subroutine is called by getsym when this last one */
/*        have detected the beginning of a lexical token which corresponds */
/*        to a positive number (an integer or a float). There are two */
/*        cases (whom this routine is informed by the logical */
/*        dotdet (as "dot detected")) : */

/*          1/ the token begins with a digit d (in [0-9]) : in this case */
/*             dotdet = .false. */

/*          2/ the token begins with a point following by a digit .d : in */
/*             this case  dotdet = .true. */

/*        On entry, the global var char1 contains the first digit of the number */
/*        Getting the next "char" is done by a call to getch (which put the */
/*        next char is the global var char1). */

/*     MOTIVATION */
/*        Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so as */
/*        to replace the old string -> number method used by Scilab */
/*        which was not accurate enough (even in some easy cases we can got */
/*        the near float s (and the float gotten may be 3 or 4 floats after */
/*        or before the optimal one) : in fact contrarily to the old method */
/*        this subroutine computes actually the float s only when the string */
/*        pattern is such that only one (hoped correctly) rounded float */
/*        operation will be done in computing s, all others operations being */
/*        exact (and so we are sure to get the nearer floating point number). */
/*        In the others cases we call an "intrinsic" function of Fortran */
/*        (as strtod in C) to do the job (what fortran called an internal file). */
/*        The overhead comes from the fact that at this level the "scilab characters" */
/*        are actually integers (a first convertion string -> integer is already */
/*        done) so a "reconversion" to a string is necessary. */

/*     A BRIEF EXPLANATION */
/*        On an example, suppose that the "string" pattern is 1234.56789012345e+23  : */

/*        1/ the mantissa is red and the digits are recorded inside the array */
/*           digit = [1 2 3 4 5 6 7 8 9 0 1 2 3 4 5]  (only the first ndgmax */
/*           digits of the mantissa are red) */
/*           -> the dot is detected and a correction of -11 will be */
/*              bring in the exponent */
/*        2/ the exponent is computed directly in integer arithmetic, then the */
/*           correction is brought to got the final exponent : 23-11 = 12 */

/*        All that to say that the string number to convert is equal to */
/*           123456789012345 * 10^12 and in general  x = integer 10^expo */

/*        So if integer <= 2^53 (all integer n such that |n| <= 2^53 belong */
/*        in the ieee754 double float number) then we must compute this integer */
/*        (from the digit array) exactly in double precision (if all intermediary */
/*        computed quantities are integers <= 2^53 which is the case). */
/*        A simple way to impose this condition is the following : */
/*            2^53 = 9007199254740992 > 8 10^15 > 10^15 */
/*        So that if our integer has 15 digits (or 16 digits with d1 <= 8) then */
/*        it is OK. */

/*        For the exponent : 10^0, 10^1, 10^2, ...., 10^22 are all exactly representable */
/*        in double ieee 754 (10^22 = 5^22 * 2^22  and 5^22=2384185791015625 < 2^53 */
/*        but 5^23 > 2^53 so that 10^23 is not a float point number) */

/*        Conclusion : */
/*          (i) if our integer have less than 15 digits and if |expo|<= 22 */
/*              then only one non exact operation (a multiplication or a division */
/*              depending the sign of expo) will be done (eventually) and we */
/*              got the near float ; */
/*         (ii) one other trivial case are also considered (see explanation at */
/*              the end of this file). */
/*        (iii) If not we form a string as  123456789012345.d+12 and we call */
/*              a fortran intrinsic routine to do the job (via internal file) */

/*     A LAST REMARK : this routine doesn't change the syntax of tokens considered */
/*        as numbers in Scilab, in particular 1.d- or 1.d+ are still valid (the exponent */
/*        is taken as 0) */
/*     PARAMETER */
/*     GLOBAL SCILAB VAR (in fact only char1 is used) */
/*     LOCAL VAR */
/* Copyright INRIA */
/* *------------------------------------------------------------------ */
/* vsiz  size of internal scilab stack */

/* bsiz  size of internal chain buf */

/* isizt  maximum number of scilab variables global and local */
/* isiz maximum number of scilab local variables */
/* psiz  defines recursion size */
/* lsiz  dim. of vector containing the command line */
/* nlgh  length of variable names */
/* csiz  used for character coding */
/* intersiz used in interfaces */
/* *------------------------------------------------------------------- */
/* c (DLL Digital Visual Fortran) */
/* DEC$ IF DEFINED (FORDLL) */
/* DEC$ ATTRIBUTES DLLIMPORT:: /stack/, /vstk/, /recu/, /iop/ */
/* DEC$ ATTRIBUTES DLLIMPORT:: /errgst/, /com/, /adre/ */
/* DEC$ ATTRIBUTES DLLIMPORT:: /intersci/ ,/cha1/ */
/* DEC$ ENDIF */
/*     --------------------------------------------------------------- */

/* *------------------------------------------------------------------ */

/*     detdot : a var to put the value of the argument dotdet */
/*              (this is because at the call, dotdet is a constant */
/*              (.true. or .false.) and in this subroutine detdot */
/*              may change of value */

/*     ndgmax : maximun number of recorded digits (=> when */
/*              the mantissa have more than ndgmax digit, */
/*              it may result a relative error of 10^(1-ndgmax) */
/*              between the initial number and the number that */
/*              this routine converts as a machine number */
/*              (this last one may suffer of a relative error */
/*              of epsm = (approx) 1.11 10^(-16))) */
/*     digit  : array of length ndgmax to record the mantissa 's digits */
/*     ndgrec : number of recorded digits (<= ndgmax) */
/*     ndg    : to count the number of digits of the integer part of */
/*              the mantissa (which may be superior to ndgmax => in */
/*              this case a correction must be bring in the exponent) */
/*     sgnexp : sign of the exponent part (see SYMBOL AFTER) */
/*     expcor : correction to bring in the exponent (because all */
/*              the mantissa begin an integer, p.e. 123.456 => 123456 */
/*              in this case the correction is -3) */
/*     ndgexp : number of digits of the exponent (to control spurious */
/*              integer overflow if the exponent is something like */
/*              e+2147483648  (=2^31  (= -2^31 with the usual 32 bits */
/*              integer arithmetic ))) */
/*     expo   : the exponent (directly computed with integer arithmetic */
/*              but ndgexp may control integer "overflow") */
/*     code0  : integer code of the character "0" */
/*     string : string to hold the "number" to be converted in double */
/*              when the "number" is such that a direct straitforward */
/*              conversion will be not enough accurate */
/*     toto   : a var to got an inf with 1/(toto-toto) */
/*     CONSTANTS  (to adapt eventualy ...) */
/*     EXPMAX may be such that 10^EXPMAX > max positive float num */
/*     EXPMIN may be such that 10^EXPMIN < min positive float num */
/*     NDEMAX may be such that 10^5 <= 10^NDEMAX < MAX_INTEGER : */
/*            when we compute (with integer arithmetic) the exponent */
/*            the number of digits of the exponent is recorded in */
/*            ndgexp and the test  ndgexp <= NDEMAX validate this */
/*            calculus. */
/*     DGLIM  all integers with a number of digits <= DGLIM must be */
/*            exactly representable as double float (DGLIM = 15 for */
/*            ieee 754) */
/*     EXPLIM all power of 10 up to EXPLIM (included) must be exactly */
/*            representable as double float (EXPLIM = 22 for ieee 754) */
/*     SOME CHAR SYMBOLS (scilab char are integers) */
/*     STATEMENT FUNCTION */
/*     DATA */
/*     this is from the original getval code... don't know the reason */
    com_1.fin = 0;
/*     beginning of the code */
    detdot = *dotdet;
    ndg = 0;
    ndgrec = 0;
    if (! detdot) {
/*        1) got the integer part of the mantissa of the pattern */
/*        1-a) may be there is some 0 at the beginning */
	while(com_1.char1 == 0) {
	    getch_();
	}
/*        1-b) now record the digits (inside the digit array) */
/*             (but we record a maximum of ndgmax digits) */
	while(abs(com_1.char1) <= 9) {
	    ++ndg;
	    if (ndgrec < 25) {
		++ndgrec;
		digit[ndgrec - 1] = com_1.char1;
	    }
	    getch_();
	}
/*        1-c) at this point we have detected something which is not a digit */
/*             may be a point, may be a d,D,e,E, or something else */
/*             here we only test for the dot and let the others cases */
/*             to be treated after ... */
	if (abs(com_1.char1) == dot) {
	    detdot = TRUE_;
	    getch_();
	}
    }
/*     first correction for the (future) exponent : if the first part */
/*     of the string have more then ndgmax digits we have to add */
/*     ndg - ndgrec (else we have expcor=0) */
    expcor = ndg - ndgrec;
    if (detdot) {
/*        2) got the "fractionnal" part of the "mantissa" */
	if (ndgrec == 0) {
/*           we have not passed throw the part 1) or only zeros have been met */
/*           and may be the number start with .000xxx : so clean up those 0 */
	    while(com_1.char1 == 0) {
		--expcor;
		getch_();
	    }
	}
/*        now we begin to record the digits */
	while(abs(com_1.char1) <= 9) {
	    if (ndgrec < 25) {
		++ndgrec;
		--expcor;
		digit[ndgrec - 1] = com_1.char1;
	    }
	    getch_();
	}
    }
/*     3) at this point the "mantissa" of the string decimal number */
/*        must be recorded, now detect the exponent */
    expo = 0;
    ndgexp = 0;
    sgnexp = plus;
    if (abs(com_1.char1) == d__ || abs(com_1.char1) == e) {
/*        the string have an exponent part (which, in Scilab, may be empty or */
/*        may had only a sign ! => expo = 0) */
	getch_();
	if (com_1.char1 == minus || com_1.char1 == plus) {
	    sgnexp = com_1.char1;
	    getch_();
	} else {
	    sgnexp = plus;
	}
/*        may be the exponent start by some 0 */
	while(com_1.char1 == 0) {
	    getch_();
	}
/*        now form the exponent : the var ndgexp is here */
/*        to treat spurious integer overflow ... */
	while(abs(com_1.char1) <= 9) {
	    expo = expo * 10 + com_1.char1;
	    ++ndgexp;
	    getch_();
	}
    }
/*     4) Now we can form the double float number s */
/*     4-1/ only zeros in the mantissa */
    if (ndgrec == 0) {
/*        no digits have been recorded : this is the case */
/*        when the mantissa part is of the form [000][.][000] */
/*        the number is 0 */
	*s = 0.;
	return 0;
    }
/*     4-2/ ndgexp is to large => the exponent expo is perhaps badly */
/*     computed (integer "overflow") or in all cases the */
/*     exponent is too large (positive or negative) such that it result */
/*     (for s) in a overflow or underflow depending the exponent sign */
    if (ndgexp >= 7) {
	if (sgnexp == minus) {
/*           underflow */
	    *s = 0.;
	} else {
/*           overflow : got an inf ... */
	    *s = 1. / (toto - toto);
	}
	return 0;
    }
/*     4-3/ now build the final exponent */
    if (sgnexp == plus) {
	expo += expcor;
    } else {
	expo = -expo + expcor;
    }
/*     4-4/ here some tests to avoid unnecessary call to the fortran "strtod" */
/*     Now we have a number s of the form  d_1 d_2 ... d_ndgrec 10^expo */
/*     which is equal to d_1 . d_2 ... d_ndgrec 10^(expo + ndgrec - 1) */
/*     with d_1 .ne. 0 */
/*     so it comes :  s >= 10^(expo + ndgrec - 1) */
/*                    s <= 10^(expo + ndgrec) */

/*     Suppose given EXPMAX such that  10^EXPMAX > max positive float number */
/*               and EXPMIN such that  10^EXPMIN < min positive float number */

/*     then if  expo + ndgrec - 1 >= EXPMAX then overflow occurs necessarily */
/*     and  if  expo + ndgrec <= EXPMIN then underflow occurs */

/*     On IEEE 754 we have : max positive float num = (approx) 1.8E+308 */
/*                           min positive float num = (approx) 4.9E-324 */
/*                           (if denormalised number are used) */

/*     So that EXPMAX = 309 */
/*        and  EXPMIN = -324  are OK (but larger limits are possible to take */
/*     into account others f.p. arithmetics) */
/*     Note that after the test (with these values) the exponent have a */
/*     maximum of 3 (decimals) digits */
    if (expo + ndgrec - 1 >= 309) {
/*        overflow : got an inf ... */
	*s = 1. / (toto - toto);
	return 0;
    }
    if (expo + ndgrec <= -324) {
/*        underflow : got an 0 */
	*s = 0.;
	return 0;
    }
/*     4-5/ Now the usual case where we can get the near floating point */
/*     without any problem */
    if (ndgrec <= 15 && abs(expo) <= 22) {
	*s = 0.;
	i__1 = ndgrec;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    *s = *s * 10. + digit[i__ - 1];
	}
	if (expo < 0) {
	    i__1 = -expo;
	    *s /= pow_di(&c_b2, &i__1);
	} else {
	    *s *= pow_di(&c_b2, &expo);
	}
	return 0;
    }
/*     4-6/ The other easy case where we can compute s : */
/*     if expo = EXPLIM + k  but [integer part]*10^k < max_int_coded_in_double */
/*     then it is OK (retrieve k in the exponent and multiply the integer */
/*     part by 10^k and do the same job as previus) */
    if (expo > 22 && expo - 22 + ndgrec <= 15) {
	*s = 0.;
	i__1 = ndgrec;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    *s = *s * 10. + digit[i__ - 1];
	}
/*        peut etre dangereux avec des options d'optimisation ? */
/*        (le compilo peut etre tente d'ecrire directement s = s*10**expo */
/*        ce qui detruit le truc ...) */
	i__1 = expo - 22;
	*s *= pow_di(&c_b2, &i__1);
	*s *= 1e22;
	return 0;
    }
/*     4-7/ else use langage routines (as strtod but in fortran) to do the job */
/*     the overhead is a retranslation into a string... */
    code0 = '0';
    i__1 = ndgrec;
    for (i__ = 1; i__ <= i__1; ++i__) {
	*(unsigned char *)&string[i__ - 1] = (char) (digit[i__ - 1] + code0);
    }
    if (expo < 0) {
	i__1 = ndgrec;
	s_copy(string + i__1, ".d-", ndgrec + 3 - i__1, 3L);
    } else {
	i__1 = ndgrec;
	s_copy(string + i__1, ".d+", ndgrec + 3 - i__1, 3L);
    }
    k = ndgrec + 4;
/*     the following because the Ix format is "right" justified ... */
    if (abs(expo) <= 9) {
	ici__1.icierr = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = 1;
	ici__1.iciunit = string + (k - 1);
	ici__1.icifmt = "(I1)";
	s_wsfi(&ici__1);
	i__1 = abs(expo);
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	e_wsfi();
	ici__1.icierr = 0;
	ici__1.iciend = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = k;
	ici__1.iciunit = string;
	ici__1.icifmt = 0;
	s_rsli(&ici__1);
	do_lio(&c__5, &c__1, (char *)&(*s), (ftnlen)sizeof(doublereal));
	e_rsli();
    } else if (abs(expo) <= 99) {
	ici__1.icierr = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = 2;
	ici__1.iciunit = string + (k - 1);
	ici__1.icifmt = "(I2)";
	s_wsfi(&ici__1);
	i__1 = abs(expo);
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	e_wsfi();
	ici__1.icierr = 0;
	ici__1.iciend = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = k + 1;
	ici__1.iciunit = string;
	ici__1.icifmt = 0;
	s_rsli(&ici__1);
	do_lio(&c__5, &c__1, (char *)&(*s), (ftnlen)sizeof(doublereal));
	e_rsli();
    } else {
	ici__1.icierr = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = 3;
	ici__1.iciunit = string + (k - 1);
	ici__1.icifmt = "(I3)";
	s_wsfi(&ici__1);
	i__1 = abs(expo);
	do_fio(&c__1, (char *)&i__1, (ftnlen)sizeof(integer));
	e_wsfi();
	ici__1.icierr = 0;
	ici__1.iciend = 0;
	ici__1.icirnum = 1;
	ici__1.icirlen = k + 2;
	ici__1.iciunit = string;
	ici__1.icifmt = 0;
	s_rsli(&ici__1);
	do_lio(&c__5, &c__1, (char *)&(*s), (ftnlen)sizeof(doublereal));
	e_rsli();
    }
} /* getval_ */

#undef zstk
#undef sstk
#undef istk
#undef cstk


