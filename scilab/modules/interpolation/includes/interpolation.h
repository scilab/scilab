/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
enum {NOT_A_KNOT, NATURAL, CLAMPED, PERIODIC, FAST, FAST_PERIODIC, 
MONOTONE, BY_ZERO, C0, LINEAR, BY_NAN, UNDEFINED};
/*-----------------------------------------------------------------------------------*/
typedef struct { char *str_type; int type; } TableType;
/*-----------------------------------------------------------------------------------*/
#define NB_SPLINE_TYPE 7
static TableType SplineTable[NB_SPLINE_TYPE] = { 
	{ "not_a_knot"   , NOT_A_KNOT    },
	{ "natural"      , NATURAL       },
	{ "clamped"      , CLAMPED       },
	{ "periodic"     , PERIODIC      },
	{ "monotone"     , MONOTONE      },
	{ "fast"         , FAST          },
	{ "fast_periodic", FAST_PERIODIC }};
/*-----------------------------------------------------------------------------------*/
#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] = {
	{ "C0"        , C0         },
	{ "by_zero"   , BY_ZERO    },
	{ "natural"   , NATURAL    },
	{ "periodic"  , PERIODIC   },
	{ "by_nan"    , BY_NAN     },
	{ "linear"    , LINEAR     }};
/*-----------------------------------------------------------------------------------*/
typedef struct realhypermat 
{
	int dimsize;        /* number of dimensions of the hyper matrix */
	int size;           /* total number of elements : size = dims[0]x... x dims[dimsize-1] */
	int *dims;          /* number of elements in each dimension */
	double *R;          /* points to the elements  */
} RealHyperMat;
/*-----------------------------------------------------------------------------------*/
extern int get_rhs_scalar_string(int num, int *length, int **tabchar);
#define GetRhsScalarString(num,n,t) if (!get_rhs_scalar_string(num,n,t)) { return 0;}
/*-----------------------------------------------------------------------------------*/
extern int get_rhs_real_hmat(int num, RealHyperMat *H);
#define GetRhsRealHMat(pos,H) if (!get_rhs_real_hmat(pos,H)) { return 0;}
/*-----------------------------------------------------------------------------------*/
#ifndef max 
#define max(x,y)	(((x)>(y))?(x):(y))
#endif 

#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif
