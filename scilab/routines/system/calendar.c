/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "calendar.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
int IsAScalar(int RhsNumber);
static int isBissextile (unsigned annee);
static unsigned months_to_days (unsigned mois);
static long years_to_days (unsigned annee);
static long ymd_to_scalar (unsigned annee, unsigned mois, unsigned jour);
int *InversionMatrixInt(int W,int L,int *Matrix);
/*-----------------------------------------------------------------------------------*/
#define NBRDAY 7
#define NBRWEEK 6
/*-----------------------------------------------------------------------------------*/

int days[12]    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*-----------------------------------------------------------------------------------*/
int C2F(intcalendar) _PARAMS((char *fname))
{
	static int l1,n1,m1;

	int month=0;
	int year=0;
	int day, day_1, numdays, i;
    int a=0;

	int *CALMONTH=NULL;
	int *tmpMatrix=NULL;

	Rhs = Max(0, Rhs);
	CheckRhs(2,2);
	CheckLhs(1,1);
	
	if ( IsAScalar(Rhs-1) && IsAScalar(Rhs) )
	{
		GetRhsVar(1,"i",&m1,&n1,&l1);
		month=*istk(l1);

		GetRhsVar(2,"i",&m1,&n1,&l1);
		year=*istk(l1);

		if ( (year<1800) || (year>3000) )
		{
			Scierror(999,"Error : year (1800 - 3000).\n");
			return 0;
		}
				
		if ( (month<1) || (month>12) )
		{
			Scierror(999,"Error : month (1 - 12).\n");
			return 0;
		}
				
	}
	else
	{
		Scierror(999,"Invalid argument(s) type.\n");
		return 0;
	}
	CALMONTH=(int *)MALLOC( (NBRDAY*NBRWEEK)*sizeof(int) );
	for (i=0;i<NBRDAY*NBRWEEK;i++) CALMONTH[i]=0;

	/* verification si le mois de fevrier est a 28 jours ou 29 jours */
    numdays = days[month - 1];
    if (2 == month && isBissextile(year)) ++numdays;

	/* Cela sert a faire demarrer le calendrier par le Lundi */
    day_1 = (int)((ymd_to_scalar(year, month, 1) - (long)1) % 7L);

	for (day = 0; day < day_1; ++day) a++;

    /* ici on remplit les jours tous simplement */
    for (day = 1; day <= numdays; ++day, ++day_1, day_1 %= 7)
    {
        CALMONTH[a]= day;
		a++;
    }
	m1=NBRWEEK;
	n1=NBRDAY;
	tmpMatrix=CALMONTH;

	CALMONTH=InversionMatrixInt(NBRDAY,NBRWEEK,CALMONTH);
	if(tmpMatrix) {FREE(tmpMatrix);tmpMatrix=NULL;}
	
	CreateVarFromPtr(Rhs+1, "i", &m1, &n1 ,&CALMONTH);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();

	if (CALMONTH) {FREE(CALMONTH);CALMONTH=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int IsAScalar(int RhsNumber)
{
	int bOK=0;

	if ( GetType(RhsNumber) == sci_matrix )
	{
		static int l1,n1,m1;
		GetRhsVar(RhsNumber,"d",&m1,&n1,&l1);
		if ( (m1 == 1) && (n1 == 1) )
		{
			bOK=1;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
static int isBissextile (unsigned annee)
{
    return annee % 400 == 0 || (annee % 4 == 0 && annee % 100 != 0);
}
/*-----------------------------------------------------------------------------------*/
static unsigned months_to_days (unsigned mois)
{
    return (mois * 3057 - 3007) / 100;
}
/*-----------------------------------------------------------------------------------*/
static long years_to_days (unsigned annee)
{
    return annee * 365L + annee / 4 - annee / 100 + annee / 400;
}
/*-----------------------------------------------------------------------------------*/
static long ymd_to_scalar (unsigned annee, unsigned mois, unsigned jour)
{
    long scalaire;
    scalaire = jour + months_to_days(mois);
    if ( mois > 2 )                      
    scalaire -= isBissextile(annee) ? 1 : 2;
    annee--;
    scalaire += years_to_days(annee);
    return scalaire;
}
/*-----------------------------------------------------------------------------------*/
int *InversionMatrixInt(int W,int L,int *Matrix)
{
	int *buffer = (int *)MALLOC( (W*L)*sizeof(int) );

	int i=0;
	int j=0;

	for (i=0; i<W; i++) for (j=0; j<L; j++) buffer[i*L+j] = Matrix[j*W+i];

	return buffer;
}
/*-----------------------------------------------------------------------------------*/
