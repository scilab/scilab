/* Copyright INRIA */
/* Allan CORNET 2006 */

#ifndef SCICOS_DEF
#define SCICOS_DEF


#if _LCC_ & FORDLL
#define IMPORT __declspec (dllimport)
#else
#ifdef SCICOS_EXPORTS
#define IMPORT_SCICOS
#else
#ifdef FORDLL
#define IMPORT_SCICOS extern  __declspec (dllimport)
#else
#define IMPORT_SCICOS
#endif
#endif
#endif



typedef struct
{
    int counter;
} COSDEBUGCOUNTER_struct;


typedef struct
{
    int solver;
} SOLVER_struct;


typedef struct
{
    int kfun;
} CURBLK_struct;


typedef struct
{
    double scale;
}  RTFACTOR_struct;



typedef struct
{
    int ptr;
}  SCSPTR_struct;


typedef struct
{
    int idb;
} DBCOS_struct;


typedef struct
{
    double atol, rtol, ttol, deltat;
} COSTOL_struct;

/*!
 * \brief Storage for the halt flag.
 */
typedef struct
{
    /*!
     * \brief halt the simulation flag
     * \a 0 is used to continue the simulation
     * \a 1 is used to halt the simulation
     * \a 2 is used to switch to the final time then halt the simulation
     */
    int halt;
}  COSHLT_struct;


typedef struct
{
    int cosd;
} COSDEBUG_struct;

#define COSERR_len 4096
typedef struct
{
    char buf[COSERR_len];
} COSERR_struct;

typedef struct
{
    int isrun;
} COSIM_struct;


#endif /*SCICOS_DEF*/
