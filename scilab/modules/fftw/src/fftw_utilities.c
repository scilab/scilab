/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* ALAN LAYEC initial revision  : 30/09/06 fftw3 toolbox*/
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#include "fftw_utilities.h"
#include "MALLOC.h"
#include "callfftw.h"
/*-----------------------------------------------------------------------------------*/ 
/* definition of a structure to store parameters
* of FFTW plan - set here default value -
*/
FFTW_Plan_struct Sci_Plan =
{
	NULL,          /* fftw_plan *p              */
	NULL,          /* int *rank                 */
	NULL,          /* fftw_iodim **dims         */
	NULL,          /* int *howmany_rank         */
	NULL,          /* fftw_iodim **howmany_dims */
	NULL,          /* int *isn                  */
	NULL,          /* unsigned *sflags          */

	{NULL,NULL,    /* double * input/output     */
	NULL,NULL},
	FFTW_ESTIMATE, /* unsigned flags            */
	1,             /* int in                    */
	1,             /* int norm                  */

	0,             /* nb_CPlan                  */
};
/*-----------------------------------------------------------------------------------*/ 
/* Put fftw_plan p value in the static structure Sci_Plan
* and parameters of fftw_plan_guru_split_dft to reuse
* plan if parameters are the same between each call of
* fftw().
*
* Input : void
*
* Output : int, return 0 if failed, else 1.
*
*/
int PushFTTWPlan(void)
{
	/* declaration of local variable */
	int nbp,i;

	/* get number of plan */
	nbp = Sci_Plan.nb_CPlan+1;

	/* realloc Sci_Plan.p */
	if ((Sci_Plan.p=(fftw_plan *) REALLOC(Sci_Plan.p,sizeof(fftw_plan)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.p[nbp]=Sci_Plan.p[nbp-1];

	/* realloc Sci_Plan.rank */
	if ((Sci_Plan.rank=(int *) REALLOC(Sci_Plan.rank,sizeof(int)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.rank[nbp]=Sci_Plan.rank[nbp-1];

	/* realloc Sci_Plan.dims */
	if ((Sci_Plan.dims=(fftw_iodim **) REALLOC(Sci_Plan.dims,sizeof(fftw_iodim *)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.dims[nbp] = NULL;
	if (Sci_Plan.dims[nbp-1]!=NULL)
	{
		if ((Sci_Plan.dims[nbp]=(fftw_iodim *) MALLOC(sizeof(fftw_iodim)*Sci_Plan.rank[nbp])) == NULL)
			return(0);
		for(i=0;i<Sci_Plan.rank[nbp];i++)
		{
			Sci_Plan.dims[nbp][i].n  = Sci_Plan.dims[nbp-1][i].n;
			Sci_Plan.dims[nbp][i].is = Sci_Plan.dims[nbp-1][i].is;
			Sci_Plan.dims[nbp][i].os = Sci_Plan.dims[nbp-1][i].os;
		}
	}

	/* realloc Sci_Plan.howmany_rank */
	if ((Sci_Plan.howmany_rank=(int *) REALLOC(Sci_Plan.howmany_rank,sizeof(int)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.howmany_rank[nbp]=Sci_Plan.howmany_rank[nbp-1];

	/* realloc Sci_Plan.howmany_dims */
	if ((Sci_Plan.howmany_dims=(fftw_iodim **) REALLOC(Sci_Plan.howmany_dims,sizeof(fftw_iodim *)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.howmany_dims[nbp]=NULL;
	if (Sci_Plan.howmany_dims[nbp-1]!=NULL)
	{
		if ((Sci_Plan.howmany_dims[nbp]=(fftw_iodim *) MALLOC(sizeof(fftw_iodim)*Sci_Plan.howmany_rank[nbp])) == NULL)
			return(0);
		for(i=0;i<Sci_Plan.howmany_rank[nbp];i++)
		{
			Sci_Plan.howmany_dims[nbp][i].n  = Sci_Plan.howmany_dims[nbp-1][i].n;
			Sci_Plan.howmany_dims[nbp][i].is = Sci_Plan.howmany_dims[nbp-1][i].is;
			Sci_Plan.howmany_dims[nbp][i].os = Sci_Plan.howmany_dims[nbp-1][i].os;
		}
	}

	/* realloc Sci_Plan.sflags */
	if ((Sci_Plan.sflags=(unsigned *) REALLOC(Sci_Plan.sflags,sizeof(unsigned)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.sflags[nbp]=Sci_Plan.sflags[nbp-1];

	/* realloc Sci_Plan.isn */
	if ((Sci_Plan.isn=(int *) REALLOC(Sci_Plan.isn,sizeof(int)*(nbp+1))) == NULL)
		return(0);
	Sci_Plan.isn[nbp]=Sci_Plan.isn[nbp-1];

	/* increase number of plan */
	Sci_Plan.nb_CPlan++;

	/* */
	return(1);
}
/*-----------------------------------------------------------------------------------*/ 
/* Free plan the Sci_Plan structure
*
* Input : void
*
* Output : int, return 0 if failed, else 1.
*
*/
int FreeFTTWPlan(void)
{
	/* declaration of local variable */
	int i,nbp;

	/* get number of plan */
	nbp = Sci_Plan.nb_CPlan+1;

	if (Sci_Plan.p!=NULL)
	{
		/* free Sci_Plan.p */
		FREE(Sci_Plan.p);
		Sci_Plan.p=NULL;

		/* free Sci_Plan.dims/Sci_Plan.howmany_dims */
		for (i=0;i<nbp;i++)
		{
			if (Sci_Plan.dims[i]!=NULL)
			{
				FREE(Sci_Plan.dims[i]);
			}
			if (Sci_Plan.howmany_dims[i]!=NULL)
			{
				FREE(Sci_Plan.howmany_dims[i]);
			}
		}
		FREE(Sci_Plan.dims);
		FREE(Sci_Plan.howmany_dims);
		Sci_Plan.dims=NULL;
		Sci_Plan.howmany_dims=NULL;

		/* free Sci_Plan.sflags */
		FREE(Sci_Plan.sflags);
		Sci_Plan.sflags=NULL;

		/* free Sci_Plan.rank */
		FREE(Sci_Plan.rank);
		Sci_Plan.rank=NULL;

		/* free Sci_Plan.howmany_rank */
		FREE(Sci_Plan.howmany_rank);
		Sci_Plan.howmany_rank=NULL;

		/* free Sci_Plan.isn */
		FREE(Sci_Plan.isn);
		Sci_Plan.isn=NULL;

		/* number of plan = 0 */
		Sci_Plan.nb_CPlan=0;
	}
	/* */
	return(1);
}
/*-----------------------------------------------------------------------------------*/ 
/* Update the static structure Sci_Plan with new
* fftw_plan p value and parameters of
* fftw_plan_guru_split_dft to reuse
* plan if parameters are the same between each call of
* fftw().
*
* Input : fftw_plan p
*         int rank
*         const fftw_iodim *dims
*         int howmany_rank
*         const fftw_iodim *howmany_dims
*
* Output : int, return 0 if failed, else 1.
*
*/
int UpdateFTTWPlan(fftw_plan p, int rank, const fftw_iodim *dims,
				   int howmany_rank, const fftw_iodim *howmany_dims, int isn)
{
	/* declaration of local variable */
	int curp,i;

	/* get current plan number*/
	curp = Sci_Plan.nb_CPlan;

	/* Sci_Plan.p */
	if (Sci_Plan.p==NULL)
	{
		if ((Sci_Plan.p=(fftw_plan *) MALLOC(sizeof(fftw_plan))) == NULL)
			return(0);
	}
	Sci_Plan.p[curp]=p;

	/* Sci_Plan.rank */
	if (Sci_Plan.rank==NULL)
	{
		if ((Sci_Plan.rank=(int *) MALLOC(sizeof(int))) == NULL)
			return(0);
	}
	/* Sci_Plan.dims */
	if (Sci_Plan.dims==NULL)
	{
		if ((Sci_Plan.dims=(fftw_iodim **) MALLOC(sizeof(fftw_iodim *))) == NULL)
			return(0);
		Sci_Plan.dims[0]=NULL;
	}
	if (Sci_Plan.dims[curp]==NULL)
	{
		if ((Sci_Plan.dims[curp]=(fftw_iodim *) MALLOC(sizeof(fftw_iodim)*rank)) == NULL)
			return(0);
	}
	else
	{
		if (Sci_Plan.rank[curp] < rank)
		{
			if ((Sci_Plan.dims[curp]=(fftw_iodim *) REALLOC(Sci_Plan.dims[curp],sizeof(fftw_iodim)*rank)) == NULL)
				return(0);
		}
	}
	Sci_Plan.rank[curp]=rank;
	for(i=0;i<rank;i++)
	{
		if (dims!=NULL)
		{
			Sci_Plan.dims[curp][i].n  = dims[i].n;
			Sci_Plan.dims[curp][i].is = dims[i].is;
			Sci_Plan.dims[curp][i].os = dims[i].os;
		}
	}

	/* Sci_Plan.howmany_rank */
	if (Sci_Plan.howmany_rank==NULL)
	{
		if ((Sci_Plan.howmany_rank=(int *) MALLOC(sizeof(int))) == NULL)
			return(0);
	}
	/* Sci_Plan.howmany_dims */
	if (Sci_Plan.howmany_dims==NULL)
	{
		if ((Sci_Plan.howmany_dims=(fftw_iodim **) MALLOC(sizeof(fftw_iodim *))) == NULL)
			return(0);
		Sci_Plan.howmany_dims[0]=NULL;
	}
	if (Sci_Plan.howmany_dims[curp]==NULL)
	{
		if (howmany_rank!=0)
		{
			if ((Sci_Plan.howmany_dims[curp]=(fftw_iodim *) MALLOC(sizeof(fftw_iodim)*howmany_rank)) == NULL)
				return(0);
		}
	}
	else
	{
		if (Sci_Plan.howmany_rank[curp] < howmany_rank)
		{
			if ((Sci_Plan.howmany_dims[curp]=(fftw_iodim *) REALLOC(Sci_Plan.howmany_dims[curp],sizeof(fftw_iodim)*howmany_rank)) == NULL)
				return(0);
		}
	}
	Sci_Plan.howmany_rank[curp]=howmany_rank;
	for(i=0;i<howmany_rank;i++)
	{
		if (howmany_dims!=NULL)
		{
			Sci_Plan.howmany_dims[curp][i].n  = howmany_dims[i].n;
			Sci_Plan.howmany_dims[curp][i].is = howmany_dims[i].is;
			Sci_Plan.howmany_dims[curp][i].os = howmany_dims[i].os;
		}
	}

	/* Sci_Plan.sflags */
	if (Sci_Plan.sflags==NULL)
	{
		if ((Sci_Plan.sflags=(unsigned *) MALLOC(sizeof(unsigned))) == NULL)
			return(0);
	}
	Sci_Plan.sflags[curp]=Sci_Plan.flags;

	/* Sci_Plan.isn */
	if (Sci_Plan.isn==NULL)
	{
		if ((Sci_Plan.isn=(int *) MALLOC(sizeof(int))) == NULL)
			return(0);
	}
	Sci_Plan.isn[curp]=isn;

	/* */
	return(1);
}
/*-----------------------------------------------------------------------------------*/ 
/* Return a valid plan ptr.
* This function search in the static structure if
* the given input parameters follows an already stored
* set of parameters.
* If found then return an already stored plan ptr.
* If not found, then clean the last index and create
* a new set of parameters (and a new plan)
* with fftw_plan_guru_split_dft.
*
* Input : int rank
*         const fftw_iodim *dims
*         int howmany_rank
*         const fftw_iodim *howmany_dims
*         double *ri, double *ii
*         double *ro, double *io
*         unsigned flags, int isn
*
* Output : fftw_plan
*
*
*/
fftw_plan GetFFTWPlan(int rank, const fftw_iodim *dims,
					  int howmany_rank, const fftw_iodim *howmany_dims,
					  double *ri, double *ii, double *ro, double *io,
					  unsigned flags, int isn)
{
	/* declaration of local variable */
	fftw_plan p;
	int i,j,ok,ko;

	ok=0;
	ko=ok;

	/* check if for set of parameters there is plan stored
	* in Sci_Plan
	*/
	if (Sci_Plan.p!=NULL)
	{
		for (i=0;i<(Sci_Plan.nb_CPlan+1);i++)
		{/* set ok flag to false.
		 * ok becomes 1 if it exists a plan for given parameters
		 * in Sci_pplan struct.
		 */
			ok=0;

			if (i==Sci_Plan.nb_CPlan)
			{
				if (Sci_Plan.sflags[i]!=Sci_Plan.flags) /* ! must be removed later ! */
					break;
			}
			/* check if Sci_Plan.p[i] is already allocated */
			if (Sci_Plan.p[i]!=NULL)
			{
				/* check if Sci_Plan.rank[i] */
				if (Sci_Plan.rank[i]==rank)
				{
					/* check if Sci_Plan.howmany_rank[i] */
					if (Sci_Plan.howmany_rank[i]==howmany_rank)
					{
						/* check if Sci_Plan.isn[i] */
						if (Sci_Plan.isn[i]==isn)
						{
							/* check for NULL case of dims */
							if (dims==Sci_Plan.dims[i]) /*NULL case*/
							{
								/* check for NULL case of howmany_dims */
								if(howmany_dims==Sci_Plan.howmany_dims[i]) /*NULL case*/
									ok=1; /*it is stupid!*/
							}
							else
							{
								/* set ok to true as default */
								ok=1 ;
								/* check dims (of type fftw_iodim) structure */
								for (j=0;j<Sci_Plan.rank[i];j++)
								{
									if (Sci_Plan.dims[i][j].n != dims[j].n)   {ok=0;break;}
									if (Sci_Plan.dims[i][j].is != dims[j].is) {ok=0;break;}
									if (Sci_Plan.dims[i][j].os != dims[j].os) {ok=0;break;}
								}
								/* that ok, all seems to be good */
								if (ok)
								{
									/*NULL case*/
									if(howmany_dims==Sci_Plan.howmany_dims[i]) 
									{
										/* */
										ok=1;
										/* */
										p = Sci_Plan.p[i];
										/* get out from Sci_Plan loop */
										break;
									}
									else
									{
										/* check howmany_dims (of type fftw_iodim) structure */
										for (j=0;j<Sci_Plan.howmany_rank[i];j++)
										{
											if (Sci_Plan.howmany_dims[i][j].n != howmany_dims[j].n)   {ok=0;break;}
											if (Sci_Plan.howmany_dims[i][j].is != howmany_dims[j].is) {ok=0;break;}
											if (Sci_Plan.howmany_dims[i][j].os != howmany_dims[j].os) {ok=0;break;}
										}
										if (ok)
										{
											/* */
											p = Sci_Plan.p[i];
											/* get out from Sci_Plan loop */
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/* if !ok then destroy the last set of parameters
	* and create a new set at index Sci_Plan.nb_CPlan
	*/
	if (!ok)
	{
		if (Sci_Plan.p!=NULL)
		{
			if (Sci_Plan.p[Sci_Plan.nb_CPlan]!=NULL)
			{
				/* */
				call_fftw_destroy_plan(Sci_Plan.p[Sci_Plan.nb_CPlan]);
			}
		}

		/* create a new plan */
		p = call_fftw_plan_guru_split_dft(rank, dims,howmany_rank, howmany_dims,
			ri, ii, ro, io,flags);

		/* */
		Sci_Plan.io[0]=ri;
		Sci_Plan.io[1]=ii;
		Sci_Plan.io[2]=ro;
		Sci_Plan.io[3]=io;

		/* store new plan and its parameters in Sci_Plan */
		if ((UpdateFTTWPlan(p, rank, dims, howmany_rank, howmany_dims, isn))==0)
		{
			/* */
			call_fftw_destroy_plan(p);
			/* */
			p=NULL;
		}
	}

	/* return value of fftw_plan */
	return (p);
}
/*-----------------------------------------------------------------------------------*/ 
