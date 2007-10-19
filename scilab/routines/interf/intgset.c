/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "intgset.h"
/*-----------------------------------------------------------------------------------*/
#include "../graphics/bcg.h"
#include "../stack-c.h"
#include "../graphics/CloneObjects.h"
#include "../graphics/Interaction.h"
#include "../graphics/SetProperty.h"
#include "../graphics/GetProperty.h"
#include "../graphics/InitObjects.h"
#include "../graphics/DrawObjects.h"

/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "../graphics/BuildObjects.h"
#include "../graphics/DestroyObjects.h"

#ifdef WITH_TK
#include "../tclsci/GedManagement.h"
#endif

#include "intcommongraphics.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;
extern int versionflag;
/*-----------------------------------------------------------------------------------*/
static char error_message[256];
/*-----------------------------------------------------------------------------------*/
int setticks(char * xyztick, sciPointObj* psubwin, int * ptrindex, int * numrow, int * numcol);
int setchampdata(sciPointObj *pobj, int *value, int *numrow, int *numcol, char *fname);
int setgrayplotdata(sciPointObj *pobj, int *value, int *numrow, int *numcol, char *fname);
int set3ddata(sciPointObj *pobj, int *value, int *numrow, int *numcol, int flagc, char *fname);
int sciSet(sciPointObj *pobj, char *marker, int *value, int *numrow, int *numcol);
int LinearScaling2Colormap(sciPointObj* pobj);
char ** ReBuildUserTicks( char old_logflag, char new_logflag, double * u_xgrads, int *u_nxgrads, char ** u_xlabels);
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels);
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/
int gset(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,numrow3,numcol3,l3,num,cur,na,verb=0;
  unsigned long hdl; 
  int lw,t2;
  BOOL vis_save;
  sciPointObj *pobj;

  /* F.Leray Adding some tmp variable for SCI_SURFACE / data case*/
  integer m3tl, n3tl, l3tl;
  int numrow[4],i;
  int numcol[4], lxyzcol[4];
  int ptrindex[2];
  int flagc = -1;

  /* F.Leray Init. to 0*/
  for(i=0;i<4;i++){
    numrow[i] = 0;
    numcol[i] = 0;
    lxyzcol[i] = 0;
  }
  ptrindex[0] = 0;
  ptrindex[1] = 0;


  CheckRhs(2,3);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  switch(VarType(1)) 
    {
    case 1: /* tclsci handle */
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"set",3);return 0;
      return 0;
      break;
    case 9: /* first is a scalar argument so it's a gset(hdl,"command",[param]) */
      /* F.Leray; INFO: case 9 is considered for a matrix of graphic handles*/
      CheckRhs(3,3);
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
      if ( *hstk(l1) != sciGetHandle(pfiguremdl) && *hstk(l1) != sciGetHandle(paxesmdl)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
	{
	  if (m1!=1||n1!=1) { 
	    lw = 1 + Top - Rhs;
	    C2F(overload)(&lw,"set",3);return 0;
	  }
	  if (version_flag() ==0)
	    hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */ 
	  else
	    hdl = (unsigned long)0;
	  if (hdl == (unsigned long)0 )
	    pobj = (sciPointObj *) NULL;
	  else
	    pobj = sciGetPointerFromHandle(hdl);
	} 
      else 
	{
	  hdl = (unsigned long)*hstk(l1);
	  pobj = sciGetPointerFromHandle(hdl);
	}

      GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */  
      if ( *hstk(l1) != sciGetHandle(pfiguremdl) && *hstk(l1) != sciGetHandle(paxesmdl)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) 
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
	if ((strcmp(cstk(l2),"old_style") !=0) 
	    &&(strcmp(cstk(l2),"default_figure") !=0) 
	    && (strcmp(cstk(l2),"default_axes") !=0) ) SciWin();
      if (strcmp(cstk(l2),"user_data")==0) {
	/* set pobj->user_data*/
	int *size_ptr, data_size;
	int **user_data_ptr,*data_ptr;
	int delete_user_data=0;

	if (VarType(3) == 1) { 
	  GetRhsVar(3,"d",&numrow3,&numcol3,&l3);
	  if (numrow3==0||numcol3==0) delete_user_data=1;
	}
	else if (VarType(3) == 0) delete_user_data=1;

	sciGetPointerToUserData (pobj,&user_data_ptr, &size_ptr);

	if (delete_user_data) {
	  FREE(*user_data_ptr);
	  *user_data_ptr=NULL;
	  *size_ptr=0;
	}
	else {
	  data_ptr=GetData(3);
	  data_size=GetDataSize(3)*2;

	  if (*user_data_ptr==NULL) {
	    if ( (*user_data_ptr=(int *) MALLOC (data_size*sizeof (int))) == NULL) {
	      strcpy(error_message,"No memory left for allocating user_data");return -1;}
	    *size_ptr=data_size;
	    memcpy(*user_data_ptr,data_ptr,data_size*sizeof (int));
	  }
	  else if (*size_ptr==data_size) {
	    memcpy(*user_data_ptr,data_ptr,data_size*sizeof (int));
	  }
	  else {
	    FREE(*user_data_ptr);
	    if ( (*user_data_ptr=(int *) MALLOC (data_size * sizeof (int))) == NULL) {
	      strcpy(error_message,"No memory left for allocating user_data");return -1;}
	    *size_ptr=data_size;
	    memcpy(*user_data_ptr,data_ptr,data_size*sizeof(int));

	  }
	}
	return 0;
      }
      t2=sciType(cstk(l2),pobj);
      if (t2<0) {
	Scierror(999,"%s: unknown property name '%s' \r\n",fname,cstk(l2));
	return 0;} 
      if (VarType(3) != t2) { 
	Scierror(999,"%s: uncompatible values for property type  '%s' \r\n",fname,cstk(l2));
	return 0;} 
      if (VarType(3) == 1)  GetRhsVar(3,"d",&numrow3,&numcol3,&l3);
      if (VarType(3) == 9)  GetRhsVar(3,"h",&numrow3,&numcol3,&l3);
      if (VarType(3) == 10) 
	{ if ((strcmp(cstk(l2),"tics_labels") !=0)
	      && ((strcmp(cstk(l2),"auto_ticks")) !=0)
	      && ((strcmp(cstk(l2),"axes_visible")) !=0)
	      && ((strcmp(cstk(l2),"axes_reverse")) !=0))
	  {GetRhsVar(3,"c",&numrow3,&numcol3,&l3);} 
	else
	  GetRhsVar(3,"S",&numrow3,&numcol3,&l3); }
      break;
    case 10:/* first is a string argument so it's a gset("command",[param]) */ 
      CheckRhs(2,2);
      GetRhsVar(1,"c",&m2,&n2,&l2);
      if (strcmp(cstk(l2),"default_figure") !=0 && strcmp(cstk(l2),"default_axes") !=0 )
	{
	  if ((strcmp(cstk(l2),"old_style") ==0) || 
	      (strcmp(cstk(l2),"current_figure") ==0)) {
	    hdl = (unsigned long)0;
	    pobj = (sciPointObj *) NULL;
	  }
	  else
	    {
	      SciWin();
	      if (version_flag() == 0 )
		if ((strcmp(cstk(l2),"zoom_") !=0) && 
		    (strcmp(cstk(l2),"auto_") !=0) && 
		    (strcmp(cstk(l2),"clip_box") !=0) )   
		  hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ()) ; 
		else  
		  hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));
	      else
		hdl = (unsigned long)0;
	      if (hdl == (unsigned long)0 )
		pobj = (sciPointObj *) NULL;
	      else
		pobj = sciGetPointerFromHandle(hdl);
	    }
	}
      else
	{
	  hdl = (unsigned long)0;
	  pobj = (sciPointObj *) NULL;
	}
      t2=sciType(cstk(l2),pobj);
      if (t2<0) {
	Scierror(999,"%s: unknown property name '%s' \r\n",fname,cstk(l2));
	return 0;} 
      if (VarType(2) != t2)
	if(strcmp(cstk(l2),"current_figure") != 0 && VarType(2) !=1 ){  
	  /* F.Leray : special unique case here set("current_figure", HANDLE);*/
	  /* HANDLE type is 9 */
	  Scierror(999,"%s: uncompatible values for property type  '%s' \r\n",fname,cstk(l2));
	  return 0;
	}

      if ( (VarType(2) == 1) )   {GetRhsVar(2,"d",&numrow3,&numcol3,&l3); }
      if ( (VarType(2) == 9) )   {GetRhsVar(2,"h",&numrow3,&numcol3,&l3); }
      if ( (VarType(2) == 10) ) {
	if ((strcmp(cstk(l2),"tics_labels") !=0)
	    && ((strcmp(cstk(l2),"auto_ticks")) !=0)
	    && ((strcmp(cstk(l2),"axes_visible")) !=0)
	    && ((strcmp(cstk(l2),"axes_reverse")) !=0))
	  {GetRhsVar(2,"c",&numrow3,&numcol3,&l3);} 
	else 
	  GetRhsVar(2,"S",&numrow3,&numcol3,&l3);
      }
      break;
    default:
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"set",3);return 0;
      break;
    }

  if ( (hdl != (unsigned long)0) ) { /* F.Leray 16.03.04*/
    pobj = sciGetPointerFromHandle(hdl);
    if (pobj == (sciPointObj *)NULL) {
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
      return 0;
    }
    vis_save=sciGetVisibility(pobj); /*used not to redraw the figure is object remains invisible SS 20.04.04*/
    if(sciGetEntityType(pobj) == SCI_SUBWIN && (strcmp(cstk(l2),"x_ticks")==0 ||
						strcmp(cstk(l2),"y_ticks")==0 ||
						strcmp(cstk(l2),"z_ticks")==0))
      {
	char * sizeNotEqual = "Ticks location and label vectors must have the same size.\r\n" ;
        
        if(VarType(3) != 16){
	  Scierror(999,"%s: Incorrect argument, must be a Tlist!\r\n",fname);
	  return -1;
	}

	GetRhsVar(3,"t",&m3tl,&n3tl,&l3tl);
	GetListRhsVar(3,2,"d",&numrow[0],&numcol[0],&ptrindex[0]);
       
        if ( numrow[0] == 0 && numcol[0] == 0 )
        {
          if ( ElementType( 3, 3 ) == 1 )
          {
            /* labels should be an empty vector */
            GetListRhsVar(3,3,"d",&numrow[1],&numcol[1],&ptrindex[1]);
            if ( numrow[1] != 0 || numcol[1] != 0 )
            {
              Scierror(999,sizeNotEqual) ;
              return -1 ;
            }
          }
          else
          {
            Scierror(999,sizeNotEqual) ;
            return -1 ;
          }
        }
        else
        {
          /* we should have the same number of ticks and locations */
          GetListRhsVar(3,3,"S",&numrow[1],&numcol[1],&ptrindex[1]);
          if ( numrow[1] != numrow[0] || numcol[1] != numcol[0] )
          {
            Scierror(999,sizeNotEqual) ;
            return -1 ;
          }
        }
        	
        if(setticks(cstk(l2),pobj, ptrindex, numrow, numcol) != 0) return 0;

      }
    else if(strcmp(cstk(l2),"data") == 0){ /* distinction for "data" treatment for champ and surface objects */
      if((sciGetEntityType(pobj) == SCI_SEGS) && (pSEGS_FEATURE(pobj)->ptype == 1))
	{	/* F.Leray Work here*/
	  int address[4],i;
	  for(i=0;i<4;i++) address[i] = 0;

	  if(VarType(3) != 16){
	    Scierror(999,"%s: Incorrect argument, must be a Tlist!\r\n",fname);
	    return -1;
	  }

	  GetRhsVar(3,"t",&m3tl,&n3tl,&l3tl);

	  if(m3tl != 5 || n3tl != 1){
	    sciprint("Tlist size must be 1x5\r\n");
	    return -1;
	  }

	  GetListRhsVar(3,2,"d",&numrow[0],&numcol[0],&address[0]);
	  GetListRhsVar(3,3,"d",&numrow[1],&numcol[1],&address[1]);
	  GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&address[2]);
	  GetListRhsVar(3,5,"d",&numrow[3],&numcol[3],&address[3]);

	  if (setchampdata(pobj, address, numrow, numcol,fname)!=0)  return 0;
	}
      else if((sciGetEntityType(pobj) == SCI_GRAYPLOT) && (pGRAYPLOT_FEATURE(pobj)->type == 0)) /* case 0: real grayplot */
	{	/* F.Leray Work here*/
	  int address[4],i;
	  for(i=0;i<4;i++) address[i] = 0;

	  if(VarType(3) != 16){
	    Scierror(999,"%s: Incorrect argument, must be a Tlist!\r\n",fname);
	    return -1;
	  }

	  GetRhsVar(3,"t",&m3tl,&n3tl,&l3tl);

	  if(m3tl != 4 || n3tl != 1){
	    sciprint("Tlist size must be 1x4\r\n");
	    return -1;
	  }

	  GetListRhsVar(3,2,"d",&numrow[0],&numcol[0],&address[0]);
	  GetListRhsVar(3,3,"d",&numrow[1],&numcol[1],&address[1]);
	  GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&address[2]);

	  if (setgrayplotdata(pobj, address, numrow, numcol,fname)!=0)  return 0;
	}
      else if(sciGetEntityType(pobj) == SCI_SURFACE)
	{	/* F.Leray Work here*/
	  if(VarType(3) != 16){
	    Scierror(999,"%s: Incorrect argument, must be a Tlist!\r\n",fname);
	    return -1;
	  }
	  GetRhsVar(3,"t",&m3tl,&n3tl,&l3tl);

	  /* GetListRhsVar(3,1,"d",&numrow[0],&numcol[0],&lxyzcol[0]); 
	     Not good because 3,1 is the string character "3d x y z [en option col]"*/
	  GetListRhsVar(3,2,"d",&numrow[0],&numcol[0],&lxyzcol[0]);
	  GetListRhsVar(3,3,"d",&numrow[1],&numcol[1],&lxyzcol[1]);

	  if(m3tl == 4)
	    {
	      GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&lxyzcol[2]);
	      flagc = 0;
	    }
	  else if( m3tl == 5)
	    {
	      GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&lxyzcol[2]);
	      GetListRhsVar(3,5,"d",&numrow[3],&numcol[3],&lxyzcol[3]);
	      flagc = 1;
	    }
	  else
	    {
	      sciprint("Error m3tl must be equal to 4 or 5\r\n");
	      return -1;
	    }

	  if (set3ddata(pobj, lxyzcol, numrow, numcol,flagc,fname)!=0)  return 0;
	}
      else /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
	{
	  if (sciSet(pobj, cstk(l2), &l3, &numrow3, &numcol3)!=0) {
	    Scierror(999,"%s: %s\r\n",fname,error_message);
	    return 0;
	  }
	}
    }
    else{ /* F.Leray 02.05.05 : main case (using sciGetPoint routine inside GetProperty.c) */
      if (sciSet(pobj, cstk(l2), &l3, &numrow3, &numcol3)!=0) {
	Scierror(999,"%s: %s\r\n",fname,error_message);
	return 0;
      }
    }

    if (!(vis_save==0&&sciGetVisibility(pobj)==0)) {/* do not redraw figure if object remains invisible */
      if ((strcmp(cstk(l2),"figure_style") !=0) &&
	  (strcmp(cstk(l2),"old_style") !=0 ) && 
	  (strcmp(cstk(l2),"current_axes") !=0) &&
	  (strcmp(cstk(l2),"default_figure") !=0) && 
	  (strcmp(cstk(l2),"default_axes") !=0) &&
	  (pobj != pfiguremdl) && (pobj != paxesmdl)
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ){ /* Addings F.Leray 10.06.04 */
	num= sciGetNumFigure (pobj);    
	C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

        /* The merge object need to be rebuild */
        if ( sciIsMergeable( pobj ) && pSUBWIN_FEATURE(sciGetParentSubwin( pobj ))->facetmerge )
        {
          Merge3d( sciGetParentSubwin( pobj ) ) ;
        }
        
	sciDrawObj(sciGetParentFigure(pobj)); /* F.Leray we redraw here*/

	C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  else if (sciSet((sciPointObj *) NULL, cstk(l2), &l3, &numrow3, &numcol3)!=0) {
    Scierror(999,"%s: %s\r\n",fname,error_message);
    return 0;
  }

  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int setticks(char * xyztick, sciPointObj* psubwin, int * ptrindex, int * numrow, int * numcol)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  int i,row,col,prod,old_prod;

  row = *numrow;
  col = *numcol; 
  prod = row * col;

  if(strcmp(xyztick,"x_ticks")==0)
    {

      old_prod = ppsubwin->axes.u_nxgrads; /* old size of the locations/labels vectors*/

      ppsubwin->axes.auto_ticks[0] = FALSE;
      ppsubwin->axes.u_nxgrads = prod;

      FREE(ppsubwin->axes.u_xgrads); ppsubwin->axes.u_xgrads = NULL;
      if ( prod > 0 )
      {
        if((ppsubwin->axes.u_xgrads=(double *) MALLOC(prod*sizeof(double)))==NULL) return -1;
      }

      if(ppsubwin->logflags[0]=='l')
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_xgrads[i] = log10(stk(ptrindex[0])[i]);
	}
      else
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_xgrads[i] = stk(ptrindex[0])[i];
	  ppsubwin->axes.nbsubtics[0] = ComputeNbSubTics(psubwin,ppsubwin->axes.u_nxgrads,'n',NULL,ppsubwin->axes.nbsubtics[0]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
	}

      if(ppsubwin->axes.u_xlabels != NULL)
	for(i=0;i<old_prod;i++) { /* we free the old vector components (size == old_prod) */
	  FREE(ppsubwin->axes.u_xlabels[i]); 
	  ppsubwin->axes.u_xlabels[i] = NULL;}

      FREE(ppsubwin->axes.u_xlabels); ppsubwin->axes.u_xlabels = NULL;

      if ( prod > 0 )
      {
        ppsubwin->axes.u_xlabels = *(char ***) &ptrindex[1];
      }
    }
  else if(strcmp(xyztick,"y_ticks")==0)
    {

      old_prod = ppsubwin->axes.u_nygrads; /* old size of the locations/labels vectors*/

      ppsubwin->axes.auto_ticks[1] = FALSE;
      ppsubwin->axes.u_nygrads = prod;

      FREE(ppsubwin->axes.u_ygrads); ppsubwin->axes.u_ygrads = NULL;
      if ( prod > 0 )
      {
        if((ppsubwin->axes.u_ygrads=(double *) MALLOC(prod*sizeof(double)))==NULL) return -1;
      }


      if(ppsubwin->logflags[1]=='l')
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_ygrads[i] = log10(stk(ptrindex[0])[i]);
	}
      else
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_ygrads[i] = stk(ptrindex[0])[i];
	  ppsubwin->axes.nbsubtics[1] = ComputeNbSubTics(psubwin,ppsubwin->axes.u_nygrads,'n',NULL,ppsubwin->axes.nbsubtics[1]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
	}

      if(ppsubwin->axes.u_ylabels != NULL)
	for(i=0;i<old_prod;i++) {  /* we free the old vector components (size == old_prod) */
	  FREE(ppsubwin->axes.u_ylabels[i]);
	  ppsubwin->axes.u_ylabels[i] = NULL;}

      FREE(ppsubwin->axes.u_ylabels); ppsubwin->axes.u_ylabels = NULL;
      if ( prod > 0 )
      {
        ppsubwin->axes.u_ylabels = *(char ***) &ptrindex[1];
      }
    }
  else if(strcmp(xyztick,"z_ticks")==0)
    {

      old_prod = ppsubwin->axes.u_nzgrads; /* old size of the locations/labels vectors*/

      ppsubwin->axes.auto_ticks[2] = FALSE;
      ppsubwin->axes.u_nzgrads = prod;

      FREE(ppsubwin->axes.u_zgrads); ppsubwin->axes.u_zgrads = NULL;
      if ( prod > 0 )
      {
        if((ppsubwin->axes.u_zgrads=(double *) MALLOC(prod*sizeof(double)))==NULL) return -1;
      }


      if(ppsubwin->logflags[2]=='l')
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_zgrads[i] = log10(stk(ptrindex[0])[i]);
	}
      else
	{
	  for(i=0;i< prod;i++)
	    ppsubwin->axes.u_zgrads[i] = stk(ptrindex[0])[i];
	  ppsubwin->axes.nbsubtics[2] = ComputeNbSubTics(psubwin,ppsubwin->axes.u_nzgrads,'n',NULL,ppsubwin->axes.nbsubtics[2]); /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
	}

      if(ppsubwin->axes.u_zlabels != NULL)
	for(i=0;i<old_prod;i++) { /* we free the old vector components (size == old_prod) */
	  FREE(ppsubwin->axes.u_zlabels[i]);
	  ppsubwin->axes.u_zlabels[i] = NULL;}

      FREE(ppsubwin->axes.u_zlabels); ppsubwin->axes.u_zlabels = NULL;

      if ( prod > 0 )
      {
        ppsubwin->axes.u_zlabels = *(char ***) &ptrindex[1];
      }
    }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now set as a tlist (like for surface objects) */
/* setchampdata(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setchampdata(sciPointObj *pobj, int *value, int *numrow, int *numcol, char *fname)
{
  int i=0;
  sciSegs * ppsegs = pSEGS_FEATURE (pobj);

  integer m1, n1, l1, m2, n2, l2, m3, n3, l3, m4, n4, l4;

  double * vx = NULL, * vy = NULL;
  double * vfx = NULL, * vfy = NULL;

  m1 = numrow[0];
  m2 = numrow[1];
  m3 = numrow[2];
  m4 = numrow[3];

  n1 = numcol[0];
  n2 = numcol[1];
  n3 = numcol[2];
  n4 = numcol[3];

  l1 = value[0];
  l2 = value[1];
  l3 = value[2];
  l4 = value[3];

  if (n1 != 1 || n2 != 1){
    Scierror(999,"%s:  Inside the Tlist : the first argument must be columns vectors\r\n",fname);
    return 0;
  }

  if (m3 != m1 || n3 != m2 || m4 != m3 || n4 != n3) {
    Scierror(999,"%s:  Inside the Tlist : incompatible length in the third and/or fourth argument(s)\r\n",fname);
    return 0;
  }

  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0 || m4 * n4 == 0) { LhsVar(1)=0; return 0;} 

  /* Update the dimensions Nbr1 and Nbr2 */
  ppsegs->Nbr1 = m1;
  ppsegs->Nbr2 = m2;

  /* Free the old values... */
  FREE(ppsegs->vx); ppsegs->vx = NULL;
  FREE(ppsegs->vy); ppsegs->vy = NULL;
  FREE(ppsegs->vfx); ppsegs->vfx = NULL;
  FREE(ppsegs->vfy); ppsegs->vfy = NULL;

  /* allocations:*/
  if ((vx = MALLOC (m1 * sizeof (double))) == NULL) return -1;
  if ((vy = MALLOC (m2 * sizeof (double))) == NULL) {
    FREE(vx); vx = (double *) NULL;
    return -1;
  }

  if ((vfx = MALLOC (m3*n3 * sizeof (double))) == NULL) return -1;
  if ((vfy = MALLOC (m4*n4 * sizeof (double))) == NULL) {
    FREE(vfx); vfx = (double *) NULL;
    return -1;
  }

  /* Copy the new values F.Leray */
  for(i=0;i< m1;i++)
    vx[i] = stk(l1)[i];

  for(i=0;i< m2;i++)
    vy[i] = stk(l2)[i];

  for(i=0;i< m3*n3;i++){ /* vfx and vfy must have the same dimensions */
    vfx[i] = stk(l3)[i];
    vfy[i] = stk(l4)[i];
  }

  ppsegs->vx = vx;
  ppsegs->vy = vy;
  ppsegs->vfx = vfx;
  ppsegs->vfy = vfy;

  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now set as a tlist (like for surface and champ objects) */
/* setgrayplot(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setgrayplotdata(sciPointObj *pobj, int *value, int *numrow, int *numcol, char *fname)
{
  int i=0;
  sciGrayplot * ppgrayplot = pGRAYPLOT_FEATURE (pobj);

  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;

  double * pvecx = NULL, * pvecy = NULL;
  double * pvecz = NULL;

  m1 = numrow[0];
  m2 = numrow[1];
  m3 = numrow[2];

  n1 = numcol[0];
  n2 = numcol[1];
  n3 = numcol[2];

  l1 = value[0];
  l2 = value[1];
  l3 = value[2];

  if (n1 != 1 || n2 != 1){
    Scierror(999,"%s:  Inside the Tlist : the first argument must be columns vectors\r\n",fname);
    return 0;
  }

  if (m3 != m1 || n3 != m2) {
    Scierror(999,"%s:  Inside the Tlist : incompatible length in the third argument\r\n",fname);
    return 0;
  }

  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0 ) { LhsVar(1)=0; return 0;} 

  /* Update the dimensions nx and ny */
  ppgrayplot->nx = m1;
  ppgrayplot->ny = m2;

  /* Free the old values... */
  FREE(ppgrayplot->pvecx); ppgrayplot->pvecx = NULL;
  FREE(ppgrayplot->pvecy); ppgrayplot->pvecy = NULL;
  FREE(ppgrayplot->pvecz); ppgrayplot->pvecz = NULL;

  /* allocations:*/
  if ((pvecx = MALLOC (m1 * sizeof (double))) == NULL) return -1;
  if ((pvecy = MALLOC (m2 * sizeof (double))) == NULL) {
    FREE(pvecx); pvecx = (double *) NULL;
    return -1;
  }

  if ((pvecz = MALLOC (m3*n3 * sizeof (double))) == NULL)
    return -1;


  /* Copy the new values F.Leray */
  for(i=0;i< m1;i++)
    pvecx[i] = stk(l1)[i];

  for(i=0;i< m2;i++)
    pvecy[i] = stk(l2)[i];

  for(i=0;i< m3*n3;i++){ /* vfx and vfy must have the same dimensions */
    pvecz[i] = stk(l3)[i];
  }

  ppgrayplot->pvecx = pvecx;
  ppgrayplot->pvecy = pvecy;
  ppgrayplot->pvecz = pvecz;

  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
int set3ddata(sciPointObj *pobj, int *value, int *numrow, int *numcol, int flagc, char *fname)
{
  int i=0,j,nc;
  sciSurface * psurf = pSURFACE_FEATURE (pobj);


  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n, n3n, l3n, ii;

  double * pvecx = NULL, * pvecy = NULL, * pvecz = NULL;
  integer /* * zcol = NULL,*/ izcol = 0 ;
  int dimvectx = 0;
  int dimvecty = 0;

  m1 = numrow[0];
  m2 = numrow[1];
  m3 = numrow[2];
  m3n = numrow[3]; /* F.Leray for color */
  n1 = numcol[0];
  n2 = numcol[1];
  n3 = numcol[2];
  n3n = numcol[3]; /* F.Leray for color */
  l1 = value[0];
  l2 = value[1];
  l3 = value[2];
  l3n = value[3]; /* F.Leray for color */

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) {
    if (! (m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3)) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The three first arguments have incompatible length \r\n",fname);
      return 0;
    }
  } else {
    if (m2 * n2 != n3) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The second and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if (m1 * n1 != m3) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The first and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if ( m1*n1 <= 1 || m2*n2 <= 1 ) 
      {
	Scierror(999,"%s: Inside the Tlist (third argument):The first and second arguments should be of size >= 2\r\n",fname);
	return 0;
      }
  }

  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0) { LhsVar(1)=0; return 0;} 
  /* SciWin();
     C2F(scigerase)();*/

  if(flagc == 1)
    {
      /*psurf->nc = 1; */  /* Wrong!!  */
      /* psurf->nc = m3n*n3n;*/
      izcol = 1;
      if (   m3n*n3n == m3*n3 ) izcol=2  ;
      psurf->izcol = izcol;
    }
  else
    {
      /*psurf->nc = 0;*/
      psurf->izcol = 0;
    }

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    { /* case isfac=1;*/
      if(psurf->isfac != 1)
	{
	  sciprint("Can not change the typeof3d of graphic object: its type is SCI_PLOT3D\r\n");
	  return 0;
	}
    }
  else
    { 
      /* case isfac=0;*/
      if(psurf->isfac != 0)
	{
	  sciprint("Can not change the typeof3d of graphic object: its type is SCI_FAC3D\r\n");
	  return 0;
	}
    }

  
  /* check the monotony on x and y */
  
  if ( psurf->isfac == 1 )
  {
    /* x is considered as a matrix */
    dimvectx = -1 ;
  }
  else if(m1 == 1) /* x is a row vector */
  {
    dimvectx = n1;
  }
  else if(n1 == 1) /* x is a column vector */
  {
    dimvectx = m1;
  }
  else /* x is a matrix */
  {
    dimvectx = -1;
  }
  
  if(dimvectx>1){
    /* test the monotony on x*/
    if(stk(l1)[0] >= stk(l1)[1]) /* decreasing */
      {
	int i;
	for(i=1;i<dimvectx-1;i++)
	  {
	    if(stk(l1)[i] < stk(l1)[i+1])
	      {
		Scierror(999,"Objplot3d: x vector is not monotonous \t\n");
		return 0;
	      }
	  }
	psurf->flag_x = -1;
      }
    else /* x[0] < x[1]*/
      {
	for(i=1;i<dimvectx-1;i++)
	  {
	    if(stk(l1)[i] > stk(l1)[i+1])
	      {
		Scierror(999,"Objplot3d: x vector is not monotonous \t\n");
		return 0;
	      }
	  }
	psurf->flag_x = 1;
      }
  }

  if ( psurf->isfac == 1 )
  {
    /* y is considered as a matrix */
    dimvecty = -1 ;
  }
  else if( m2 == 1) /* y is a row vector */
  {
    dimvecty = n2;
  }
  else if( n2 == 1) /* y is a column vector */
  {
    dimvecty = m2;
  }
  else /* y is a matrix */
  {
    dimvecty = -1;
  }

  if(dimvecty>1){
    /* test the monotony on y*/
    if(stk(l2)[0] >= stk(l2)[1]) /* decreasing */
      {
	int i;
	for(i=1;i<dimvecty-1;i++)
	  {
	    if(stk(l2)[i] < stk(l2)[i+1])
	      {
		Scierror(999,"Objplot3d: y vector is not monotonous \t\n");
		return 0;
	      }
	  }
	psurf->flag_y = -1;
      }
    else /* y[0] < y[1]*/
      {
	for(i=1;i<dimvecty-1;i++)
	  {
	    if(stk(l2)[i] > stk(l2)[i+1])
	      {
		Scierror(999,"Objplot3d: y vector is not monotonous \t\n");
		return 0;
	      }
	  }
	psurf->flag_y = 1;
      }
  }
  
  /* Update of the dimzx, dimzy depends on  m3, n3: */
  psurf->dimzx = m3;
  psurf->dimzy = n3;
  

  /* Free the old values... */
  FREE(psurf->pvecx); psurf->pvecx = NULL;
  FREE(psurf->pvecy); psurf->pvecy = NULL;
  FREE(psurf->pvecz); psurf->pvecz = NULL;
  /* ...even on zcol wich must have been initialized like others or set to NULL in case there was no color before
     The FREE macro tests the NULL pointer existence... */
  FREE(psurf->zcol); psurf->zcol = NULL;
  /* If we had a previous color matrix/vector and we do not specify a new one, I consider we are losing it.*/
  /* That's why we make a FREE as follows:*/
  FREE(psurf->inputCMoV);psurf->inputCMoV = NULL; /* F.Leray 23.03.04*/

  /* allocations:*/
  if ((pvecx = MALLOC (m1*n1 * sizeof (double))) == NULL) return -1;
  if ((pvecy = MALLOC (m2*n2 * sizeof (double))) == NULL) {
    FREE(pvecx); pvecx = (double *) NULL;
    return -1;
  } 
  if ((pvecz = MALLOC (m3*n3 * sizeof (double))) == NULL) {
    FREE(pvecx); pvecx = (double *) NULL;
    FREE(pvecy); pvecy = (double *) NULL;
    return -1;
  }

  /* Copy the new values F.Leray */
  for(i=0;i< m1*n1;i++)
    pvecx[i] = stk(l1)[i];

  for(i=0;i< m2*n2;i++)
    pvecy[i] = stk(l2)[i];

  for(i=0;i< m3*n3;i++)
    pvecz[i] = stk(l3)[i];
  
  if(flagc ==1) /* F.Leray There is a color matrix */
    {
      if(m3n * n3n != 0) /* Normally useless test here: means we have a color vector or matrix */
	{
	  if (((psurf->inputCMoV = MALLOC (( (m3n)*(n3n) * sizeof (double)))) == NULL))
	    {
	      FREE(pvecx); pvecx = (double *) NULL;
	      FREE(pvecy); pvecy = (double *) NULL;
	      FREE(pvecz); pvecz = (double *) NULL;
	      return -1;
	    }

	  for (j = 0;j < (m3n)*(n3n); j++)  
	    psurf->inputCMoV[j] = stk(l3n)[j];

	}

      if(psurf->flagcolor == 2 || psurf->flagcolor == 4)
	{ /* case of SCI_PLOT3D avoid */
	  nc = psurf->dimzy;
	}
      else if(psurf->flagcolor == 3)
	{
	  nc = psurf->dimzx * psurf->dimzy;
	}  
      else
	nc=0;

      if(nc>0){
	if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL) {
	  FREE(pvecx); pvecx = (double *) NULL;
	  FREE(pvecy); pvecy = (double *) NULL;
	  FREE(pvecz); pvecz = (double *) NULL;
	  return -1;
	}
      }

      /* case flagcolor == 2*/
      if(psurf->flagcolor==2 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	{
	  /* We have just enough information to fill the psurf->zcol array*/
	  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
	    psurf->zcol[j] = psurf->inputCMoV[j];  /* DJ.A 2003 */
	}
      else if(psurf->flagcolor==2 && !( m3n==1 || n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	{
	  /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
	  /* NO !! Let's do better; F.Leray 08.05.04 : */
	  /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
	  /* in our example: m3n=4 and n3n=400 */
	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    {
	      double tmp = 0;
	      for(ii=0;ii<m3n;ii++)
		tmp = tmp +  psurf->inputCMoV[j*m3n + ii];
	      tmp = tmp / m3n;
	      psurf->zcol[j] = tmp;
	    }
	}
      /* case flagcolor == 3*/
      else if(psurf->flagcolor==3 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	{
	  /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
	  /* We repeat the data:*/
	  for(i = 0; i< psurf->dimzy; i++){
	    for (j = 0;j < psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
	      psurf->zcol[psurf->dimzx*i+j] = psurf->inputCMoV[i];  /* DJ.A 2003 */
	  }
	}
      else if(psurf->flagcolor==3 && !( m3n==1 || n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	{
	  /* We have just enough information to fill the psurf->zcol array*/
	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    psurf->zcol[j]= psurf->inputCMoV[j];
	}  
      /* case flagcolor == 4*/
      else if(psurf->flagcolor==4 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	{
	  /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
	  /* We repeat the data:*/
	  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
	    psurf->zcol[j] = psurf->inputCMoV[j];

	}
      else if(psurf->flagcolor==4 && !( m3n==1 || n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	{
	  /* input : color matrix , we use 1 color per facet with Matlab selection mode (no average computed) */
	  /* HERE is the difference with case 2 */
	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    psurf->zcol[j] = psurf->inputCMoV[j*m3n];
	}

    }
  else /* else we put the value of the color_mode flag[0]*/
    {

      if(psurf->flagcolor == 2 || psurf->flagcolor == 4)
	{ /* case of SCI_PLOT3D avoid */
	  nc = psurf->dimzy;
	}
      else if(psurf->flagcolor == 3)
	{
	  nc = psurf->dimzx *  psurf->dimzy;
	} 
      else
	nc=0;

      if(nc>0){
	if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL) {
	  FREE(pvecx); pvecx = (double *) NULL;
	  FREE(pvecy); pvecy = (double *) NULL;
	  FREE(pvecz); pvecz = (double *) NULL;
	  return -1;
	}
      }

      /* case flagcolor == 2*/
      if(psurf->flagcolor==2 || psurf->flagcolor==4) /* we have to fill a Color vector */
	{
	  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
	    psurf->zcol[j] = psurf->flag[0];
	}
      else if(psurf->flagcolor==3) /* we have to fill a color matrix */
	{
	  for(i = 0; i< psurf->dimzy; i++){
	    for (j = 0;j < psurf->dimzx; j++)
	      psurf->zcol[psurf->dimzx*i+j] = psurf->flag[0];
	  }
	}
      else
	{
	  /* case flagcolor == 0 or 1 */
	  psurf->zcol = NULL;
	  psurf->izcol = 0;
	}
    }

  psurf->pvecx = pvecx;
  psurf->pvecy = pvecy;
  psurf->pvecz = pvecz;

  psurf->nc = nc; /*Adding F.Leray 23.03.04*/

  psurf->m1 = m1;
  psurf->m2 = m2;
  psurf->m3 = m3;
  psurf->n1 = n1;
  psurf->n2 = n2;
  psurf->n3 = n3;
  psurf->m3n = m3n; /* If m3n and n3n are 0, then it means that no color matrix/vector was in input*/
  psurf->n3n = n3n;

  /* We need to rebuild ...->color matrix */
  if(psurf->flagcolor != 0 && psurf->flagcolor !=1){ 
    if(psurf->cdatamapping == 0){ /* scaled */
      FREE(psurf->color);
      LinearScaling2Colormap(pobj);
    }
    else{
      int nc = psurf->nc;

      FREE(psurf->color);

      if(nc>0){
	if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
	  return -1;
      }

      for(i=0;i<nc;i++)
	psurf->color[i] = psurf->zcol[i];
      /* copy zcol that has just been freed and re-alloc + filled in */
    }
  }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* removeNewStyleMenu                                                                */
/* remove the menu and toolbar which can not be used in old style                    */
/*-----------------------------------------------------------------------------------*/
void updateMenus( struct BCG * XGC )
{
#if  WIN32
  {
    extern void RefreshGraphToolBar(struct BCG * ScilabGC);
    extern void RefreshMenus(struct BCG * ScilabGC);
    
    RefreshMenus(XGC);
    RefreshGraphToolBar(XGC);
  }
#else
  {
      extern void refreshMenus( struct BCG * ScilabGC ) ;

      refreshMenus( XGC ) ;
      /* no toolbar under linux */
  }
#endif

}

/*-----------------------------------------------------------------------------------*/
/**@name int sciset(sciPointObj *pobj,char *marker, long *x, long *y, long *w, long *h)
 * Sets the value to the object
 */
int sciSet(sciPointObj *pobj, char *marker, int *value, int *numrow, int *numcol)
{
  int xtmp;
  int  i,num,v=1,na,id,cur,verb=0;
  double dv=0.0; 
  char   **ptr;
  sciPointObj *psubwin, *figure, *tmpobj;
  struct BCG *XGC;

  /* debug F.Leray 28.04.04 */
  /* sciSubWindow * ppsubwin = NULL;*/

  if ((strcmp(marker,"figure_model") !=0) && (strcmp(marker,"axes_model") !=0)
      && (pobj  != pfiguremdl)  && (pobj  != paxesmdl)
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      if (pobj != (sciPointObj *)NULL) 
			{
				psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
      }

      if ((pobj == (sciPointObj *)NULL) && 
	  (strcmp(marker,"old_style") !=0 ) && 
	  (strcmp(marker,"figure_style") != 0) && 
	  (strcmp(marker,"current_figure") != 0)) 
	{
	  if (version_flag() == 0)
	    strcpy(error_message,"handle not valid");
	  else
	    strcpy(error_message,"function not valid under old graphics style");
	  return -1;
	}
    }

  /***************** graphics mode *******************************/ 
  if (strcmp(marker,"color_map") == 0)
    sciSetColormap((sciPointObj *)pobj, stk(*value), *numrow, *numcol);
  else if (strcmp(marker,"old_style") == 0) {
    if ((strcmp(cstk(*value),"on") == 0)) 
      versionflag = 1;
    else if ((strcmp(cstk(*value),"off") == 0))
      versionflag = 0; 
    else {
      strcpy(error_message,"old_style must be 'on' or 'off'");
      return -1;
    }
  }
  else if (strcmp(marker,"figure_style") == 0) {
    if (pobj != pfiguremdl)
      {
	if ((strcmp(cstk(*value),"old") == 0)) {
	  if (version_flag() == 0)  {
	      /* 	    versionflag = 1; */

	    sciXClearFigure();

            
#ifdef WITH_TK
            /* close ged to prevent errors when using it */
            sciDestroyGed() ;
#endif
	    C2F(dr)("xget","gc",&verb,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L);
	
	    if (XGC->mafigure != (sciPointObj *)NULL) {
		DestroyAllGraphicsSons(XGC->mafigure);
		DestroyFigure (XGC->mafigure);
		XGC->mafigure = (sciPointObj *)NULL; 
	    }
	    
	    XGC->graphicsversion = 1; /* Adding F.Leray 23.07.04 : we switch to old graphic mode */
            
            /* remove the Insert menu and purge the Edit menu in old style */
            /* A.Cornet, JB Silvy 12/2005 */
            updateMenus( XGC ) ;
            /*#if  WIN32
		{
			extern void RefreshGraphToolBar(struct BCG * ScilabGC);
			extern void RefreshMenus(struct BCG * ScilabGC);

			RefreshMenus(XGC);
			RefreshGraphToolBar(XGC);
		}
		#endif*/
	    C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
	    
	    /* Add xclear to refresh toolbar for Windows */
	    C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, 
		      PD0, 0L, 0L);
	  }
	}
	else if ((strcmp(cstk(*value),"new") == 0)) {   
	  if (version_flag() == 1)  {
	    C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);      
	    
	    C2F(dr)("xget","window",&verb,&num,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L); 
	    C2F(dr)("xstart","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
	    XGC=(struct BCG *) sciGetCurrentScilabXgc ();
	    if ((figure = ConstructFigure (XGC)) != NULL) {
	      /* Adding F.Leray 25.03.04*/
	      sciSetCurrentObj(figure);
	      XGC->mafigure = (sciPointObj *) figure;
	      XGC->graphicsversion = 0;   /* new graphic mode */
	      cf_type=1;
	      /* Add xclear to refresh toolbar for Windows */
	      C2F(dr1)("xclear","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
	      if ((psubwin = ConstructSubWin (figure, XGC->CurWindow)) != NULL){
		sciSetCurrentObj(psubwin);
		sciSetOriginalSubWin (figure, psubwin);
              }
				/* Refresh toolbar and Menus */
        updateMenus( XGC ) ;
	    }
	  }
	}
	else {
	  strcpy(error_message,"Figure style must be 'old' or 'new'");    
	  return -1;
	}
      }
    else{
      strcpy(error_message,"Cannot set the style of a model");    
      return -1;
    }  
  }  
  else if (strcmp(marker,"pixel_drawing_mode") == 0) {
    if (sciGetEntityType (pobj) == SCI_FIGURE) {
      v=-1;
      for (i=0;i<16;i++) {
	if (strcmp(cstk(*value),pmodes[i])==0) {v=i;break;}
      }
      if (v>=0) 
	sciSetXorMode((sciPointObj *) pobj, v);
      else {
	strcpy(error_message,"Invalid value");  
	return -1;
      }  
    }
    else
      {strcpy(error_message,"pixel_drawing_mode: unknown property for this handle");return -1;}
  }  
  else if (strcmp(marker,"default_values") == 0) {
    if (*stk(*value) == 1)
      {
	if (pobj == pfiguremdl)
	  InitFigureModel();
	else if (pobj == paxesmdl)
	  InitAxesModel();
	else
	  sciSetDefaultValues();
      }
    else {
      strcpy(error_message,"Value must be 1 to set default values");
      return -1;
    }
  }  
  else if (strcmp(marker,"visible") == 0) {
    if ((strcmp(cstk(*value),"on") == 0)) 
      sciSetVisibility((sciPointObj *)pobj, TRUE); 
    else if ((strcmp(cstk(*value),"off") == 0))  
      sciSetVisibility((sciPointObj *)pobj, FALSE); 
    else
      {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
  } 
  else if (strcmp(marker,"auto_resize") == 0)  { 
    if ((strcmp(cstk(*value),"on") == 0)) 
      sciSetResize((sciPointObj *) pobj, TRUE); 
    else if ((strcmp(cstk(*value),"off") == 0))  
      sciSetResize((sciPointObj *) pobj, FALSE);
    else
      {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}

  }
  /*************************** Handles Properties ********/
  else if ((strcmp(marker,"current_obj") == 0) || (strcmp(marker,"current_entity") == 0))
    {
      tmpobj=(sciPointObj *)sciGetPointerFromHandle((unsigned long)hstk(*value)[0]);
      if (tmpobj == (sciPointObj *)NULL) 
	{strcpy(error_message,"Object is not valid");return -1;}
      sciSetCurrentObj(tmpobj);
    }
  else if (strcmp(marker,"current_axes") == 0) 
    {
      tmpobj =(sciPointObj *)sciGetPointerFromHandle((unsigned long)hstk(*value)[0]);
      if (tmpobj == (sciPointObj *) NULL)
	{strcpy(error_message,"Object is not valid");return -1;}
      if (sciGetEntityType (tmpobj) == SCI_SUBWIN){
	sciPointObj * pfigure = NULL;
	sciSetSelectedSubWin(tmpobj);
	/* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
	/* we must also set the current figure to subwin->parent */
	pfigure = sciGetParentFigure(tmpobj);

	num=pFIGURE_FEATURE(pfigure)->number;
	C2F(dr1)("xset","window",&num,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
	if (sciSwitchWindow(&num) != 0){
	  strcpy(error_message,"It was not possible to create the requested figure");return -1;
	}
	/* End modif. on the 11.02.05 */
      }
      else
	{strcpy(error_message,"Object is not an Axes Entity");return -1;}
    }
  else if (strcmp(marker,"current_figure") == 0) 
    {
      if (VarType(2) == 9) {
	tmpobj =(sciPointObj *)sciGetPointerFromHandle((unsigned long)hstk(*value)[0]);
	if (tmpobj == (sciPointObj *) NULL)
	  {strcpy(error_message,"Object is not valid");return -1;}
	if (sciGetEntityType (tmpobj) != SCI_FIGURE)
	  {strcpy(error_message,"Object is not a handle on a figure");return -1;}
	num=pFIGURE_FEATURE(tmpobj)->number;
      }
      else if (VarType(2) == 1){
	num=(int)stk(*value)[0];
      }
      else{
	strcpy(error_message,"Bad argument to determine the current figure: should be a window number or a handle (available under new graphics mode only)");return -1;
      }
      
      C2F(dr1)("xset","window",&num,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);

      if(version_flag() == 0){
	if (sciSwitchWindow(&num) != 0){
	  strcpy(error_message,"It was not possible to create the requested figure");return -1;
	}
      }

    }

  /************************  figure Properties *****************************/ 
  else if (strcmp(marker,"figure_position") == 0)
    {
      sciSetFigurePos ((sciPointObj *)pobj, (int)stk(*value)[0], (int)stk(*value)[1]);
    } 
  else if (strcmp(marker,"axes_size") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimwidth=(int)stk(*value)[0];  
      pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimheight=(int)stk(*value)[1];
      if ((sciPointObj *)pobj != pfiguremdl) {
	num=pFIGURE_FEATURE(pobj)->number;
	C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F(dr)("xset","wdim",
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimwidth),
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimheight),
		PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    } 
  else if (strcmp(marker,"figure_size") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth=(int)stk(*value)[0];  
      pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight=(int)stk(*value)[1];
      if ((sciPointObj *)pobj != pfiguremdl) {
	num=pFIGURE_FEATURE(pobj)->number;
	C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F(dr)("xset","wpdim",
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth),
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight),
		PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }

    }
  else if (strcmp(marker,"figure_name") == 0) {
    sciSetName((sciPointObj *) pobj, cstk(*value), (*numcol)*(*numrow));
  }
  else if (strcmp(marker,"figure_id") == 0){
    if (sciGetEntityType (pobj) != SCI_FIGURE) {
      sprintf(error_message,"%s property undefined for this object",marker);
      return -1;
    }
    id = (int)stk(*value)[0];
    if ((sciPointObj *)pobj != pfiguremdl)
      {
	C2F(dr)("xset","window",&id,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L);
	if (sciSwitchWindow(&id) != 0){
	  strcpy(error_message,"It was not possible to create the requested figure");return -1;
	}
      }
    else
      sciSetNum(pfiguremdl, &id);
  }
  else if (strcmp(marker,"rotation_style") == 0)
    { 
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      if (strcmp(cstk(*value),"unary")==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle = 0 ;
      else if (strcmp(cstk(*value),"multiple")==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle = 1 ;
      else  {strcpy(error_message,"Nothing to do (value must be 'unary/multiple')"); return -1;}
    }
  else if (strcmp(marker,"immediate_drawing") == 0)
    { 
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      if (strcmp(cstk(*value),"on")==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = TRUE ;
      else if (strcmp(cstk(*value),"off")==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = FALSE ;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    }
  else if (strcmp(marker,"pixmap") == 0){
    if (sciGetEntityType (pobj) != SCI_FIGURE) {
      sprintf(error_message,"%s property undefined for this object",marker);
      return -1;
    }
    if (strcmp(cstk(*value),"on")==0 )
      pFIGURE_FEATURE(pobj)->pixmap =1;
    else if (strcmp(cstk(*value),"off")==0 )
      pFIGURE_FEATURE(pobj)->pixmap =0;
    else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
  }
  /********************** context graphique ******************************/
  else if (strcmp(marker,"background") == 0)
    {
      /* I add this line under:*/
      sciSetBackground((sciPointObj *)pobj, (int)stk(*value)[0]);

    }
  else if (strcmp(marker,"interp_color_vector") == 0)
    {
      if(sciGetEntityType(pobj) != SCI_POLYLINE)
	{strcpy(error_message,"interp_color_vector can only be set on Polyline objects"); return -1;}

      if(((*numcol) == 3 && pPOLYLINE_FEATURE(pobj)->dim_icv == 3) || 
	 ((*numcol) == 4 && pPOLYLINE_FEATURE(pobj)->dim_icv == 4))
	{
	  int tmp[4];
	  for(i=0;i<(*numcol);i++) tmp[i] = (int) stk(*value)[i];

	  sciSetInterpVector((sciPointObj *)pobj, (*numcol),tmp);
	}
      else
	{strcpy(error_message,"Under interpolated color moden the column dimension of the color vector must match the number of points defining the line (which must be 3 or 4)"); return -1;}
    }
  else if (strcmp(marker,"interp_color_mode") == 0)
    {
      if((sciGetEntityType(pobj) != SCI_POLYLINE))
	{strcpy(error_message,"interp_color_mode can only be set on Polyline objects"); return -1;}

      if(strcmp(cstk(*value),"on")==0 ){
	if(sciGetInterpVector(pobj) == NULL)
	  {strcpy(error_message,"You must first specify an interp_color_vector for this object"); return -1;}
	else
	  pPOLYLINE_FEATURE (pobj)->isinterpshaded = TRUE;
      }
      else
	pPOLYLINE_FEATURE (pobj)->isinterpshaded = FALSE;
    } 
  else if (strcmp(marker,"foreground") == 0)
    {
      sciSetForeground((sciPointObj *)pobj, (int) stk(*value)[0]);
    }
  else if (strcmp(marker,"fill_mode") == 0)
    { 
      if (strcmp(cstk(*value),"on")==0 )
	sciSetIsFilled((sciPointObj *)pobj,TRUE);
      else if (strcmp(cstk(*value),"off")==0 )
	sciSetIsFilled((sciPointObj *)pobj,FALSE);
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    }  
  else if (strcmp(marker,"thickness") == 0)  {
    sciSetLineWidth((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strcmp(marker,"arrow_size_factor") == 0)  {
    if(sciGetEntityType(pobj) == SCI_POLYLINE)
      pPOLYLINE_FEATURE(pobj)->arsize_factor = *stk(*value);
  }
  else if (strcmp(marker,"line_style") == 0) {
    sciSetLineStyle((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strcmp(marker,"line_mode") == 0) {
    if (strcmp(cstk(*value),"on")==0 )
      sciSetIsLine((sciPointObj *) pobj,1);
    else if (strcmp(cstk(*value),"off")==0 )
      sciSetIsLine((sciPointObj *) pobj,0);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if (strcmp(marker,"surface_mode") == 0) {
    if((sciGetEntityType(pobj) == SCI_PLOT3D) ||
       (sciGetEntityType(pobj) == SCI_FAC3D)  ||
       (sciGetEntityType(pobj) == SCI_SURFACE)){
      if (strcmp(cstk(*value),"on")==0 )
	sciSetIsLine((sciPointObj *) pobj,1);
      else if (strcmp(cstk(*value),"off")==0 )
	sciSetIsLine((sciPointObj *) pobj,0);
      else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
    }
    else {strcpy(error_message,"Surface_mode can not be set with this object, use line_mode"); return -1;}
  }
  else if (strcmp(marker,"mark_style") == 0) {
    sciSetIsMark((sciPointObj *) pobj, TRUE);
    sciSetMarkStyle((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strcmp(marker,"mark_mode") == 0) {
    if (strcmp(cstk(*value),"on")==0 )
      sciSetIsMark((sciPointObj *) pobj,1);
    else if (strcmp(cstk(*value),"off")==0 )
      sciSetIsMark((sciPointObj *) pobj,0);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if (strcmp(marker,"mark_size_unit") == 0) {
    if (strcmp(cstk(*value),"point")==0 )
      sciSetMarkSizeUnit((sciPointObj *) pobj, 1); /* 1 : points, 2 : tabulated */
    else if (strcmp(cstk(*value),"tabulated")==0 )
      sciSetMarkSizeUnit((sciPointObj *) pobj, 2);
    else  {strcpy(error_message,"Value must be 'point/tabulated'"); return -1;}
  }
  else if (strcmp(marker,"mark_size") == 0) {
    /* sciSetIsMark((sciPointObj *) pobj, TRUE); */ 
    /* F.Leray 27.01.05 commented because mark_size is automatically launched */
    /* in tcl/tk editor (which causes marks appearance even when unwanted). */
    sciSetMarkSize((sciPointObj *) pobj, (int)*stk(*value));
  }
  else if (strcmp(marker,"mark_foreground") == 0) {
    /*    sciSetIsMark((sciPointObj *) pobj, TRUE); */
    /* F.Leray 27.01.05 commented because mark_size is automatically launched */
    /* in tcl/tk editor (which causes marks appearance even when unwanted). */
    sciSetMarkForeground((sciPointObj *) pobj, (int)*stk(*value));
  }
  else if (strcmp(marker,"mark_background") == 0) {
    /*   sciSetIsMark((sciPointObj *) pobj, TRUE); */
    /* F.Leray 27.01.05 commented because mark_size is automatically launched */
    /* in tcl/tk editor (which causes marks appearance even when unwanted). */
    sciSetMarkBackground((sciPointObj *) pobj, (int)*stk(*value));
  }
  else if (strcmp(marker,"bar_width") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	double valeur = stk(*value)[0];
	pPOLYLINE_FEATURE (pobj)->bar_width = valeur;
      }
      else
	{strcpy(error_message,"Object has no bar shift");return -1;}
    }
  else if (strcmp(marker,"bar_layout") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	if(strcmp(cstk(*value),"grouped") == 0)
	  pPOLYLINE_FEATURE (pobj)->bar_layout = 0;
	else if(strcmp(cstk(*value),"stacked") == 0)
	  pPOLYLINE_FEATURE (pobj)->bar_layout = 1;
	else
	  {strcpy(error_message,"Bad property specified for bar_layout");return -1;}
      }
      else
	{strcpy(error_message,"Object has no bar style");return -1;}
    }
  else if (strcmp(marker,"x_shift") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	int num = 0;
	if(*numcol > 1 && *numrow > 1){
	  strcpy(error_message,"Bad input, x_shift should be a row or column vector.");
	  return -1;
	}
	num = (*numrow)*(*numcol);

	if (num != 0 && num!=pPOLYLINE_FEATURE (pobj)->n1) /* we can specify [] (null vector) to reset to default */
	  {
	    strcpy(error_message,"Wrong size for input vector.");
	    return -1;
	  }
      
	FREE(pPOLYLINE_FEATURE (pobj)->x_shift);
	pPOLYLINE_FEATURE (pobj)->x_shift = (double *) NULL;
	
	if(num != 0){
	  if ((pPOLYLINE_FEATURE (pobj)->x_shift = (double *) MALLOC (num * sizeof (double))) == NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_coord");return -1;}
	  
	  for (i=0;i<num;i++)
	    pPOLYLINE_FEATURE (pobj)->x_shift[i] = *stk(*value+i);
	}
      }
      else
	{strcpy(error_message,"Object has no x_shift");return -1;}
    }
  else if (strcmp(marker,"y_shift") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	int num = 0;
	if(*numcol > 1 && *numrow > 1){
	  strcpy(error_message,"Bad input, y_shift should be a row or column vector.");
	  return -1;
	}
	num = (*numrow)*(*numcol);

	if (num != 0 && num!=pPOLYLINE_FEATURE (pobj)->n1) /* we can specify [] (null vector) to reset to default */
	  {
	    strcpy(error_message,"Wrong size for input vector.");
	    return -1;
	  }
      
	FREE(pPOLYLINE_FEATURE (pobj)->y_shift);
	pPOLYLINE_FEATURE (pobj)->y_shift = (double *) NULL;

	if(num != 0){
	  if ((pPOLYLINE_FEATURE (pobj)->y_shift = (double *) MALLOC (num * sizeof (double))) == NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_coord");return -1;}
	  
	  for (i=0;i<num;i++)
	    pPOLYLINE_FEATURE (pobj)->y_shift[i] = *stk(*value+i);
	}
      }
      else
	{strcpy(error_message,"Object has no y_shift");return -1;}
    }
  else if (strcmp(marker,"z_shift") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	int num = 0;
	if(*numcol > 1 && *numrow > 1){
	  strcpy(error_message,"Bad input, z_shift should be a row or column vector.");
	  return -1;
	}
	num = (*numrow)*(*numcol);

	if (num != 0 && num!=pPOLYLINE_FEATURE (pobj)->n1) /* we can specify [] (null vector) to reset to default */
	  {
	    strcpy(error_message,"Wrong size for input vector.");
	    return -1;
	  }
      
	FREE(pPOLYLINE_FEATURE (pobj)->z_shift);
	pPOLYLINE_FEATURE (pobj)->z_shift = (double *) NULL;
	
	if(num != 0){
	  if ((pPOLYLINE_FEATURE (pobj)->z_shift = (double *) MALLOC (num * sizeof (double))) == NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_coord");return -1;}
	  
	  for (i=0;i<num;i++)
	    pPOLYLINE_FEATURE (pobj)->z_shift[i] = *stk(*value+i);
	}
      }
      else
	{strcpy(error_message,"Object has no z_shift");return -1;}
    }

  else if (strcmp(marker,"polyline_style") == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE){
	int valeur = (int)stk(*value)[0];
	if ((valeur==1) || (valeur==2) ||
	    (valeur==3) || (valeur==4) ||
	    (valeur==5) || (valeur==6) ||
	    (valeur==7)){
	  pPOLYLINE_FEATURE (pobj)->plot = valeur;
	}
	else
	  {strcpy(error_message,"Style must be 1,2,3,4,5,6 or 7");return -1;}
      }
      else
	{strcpy(error_message,"Object is not a Polyline");return -1;}
    }

  /************* font properties *********/
  else if (strcmp(marker,"font_size") == 0)
    {
      xtmp = (int)stk(*value)[0];
      sciSetFontDeciWidth((sciPointObj *) pobj, xtmp*100);
    }
  else if (strcmp(marker,"font_angle") == 0)
    {
      xtmp = (int)stk(*value)[0];
      if ( sciGetAutoRotation( pobj ) )
      {
        sciSetAutoRotation( pobj, FALSE ) ;
      }
      sciSetFontOrientation((sciPointObj *) pobj,(int) (*stk(*value)*10));
    }
  else if (strcmp(marker,"font_foreground") == 0)
    {
      xtmp = (int) *stk(*value);
      sciSetFontForeground((sciPointObj *) pobj, xtmp);
    }
  else if (strcmp(marker,"font_color") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->textcolor=(int)*stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      sciSetFontForeground(pobj,(int)*stk(*value));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"font_color property does not exist for this handle");return -1;}
  }	
  else if (strcmp(marker,"font_style") == 0)
    {
      xtmp = (int) *stk(*value);
      if ( (xtmp > 10) || xtmp < 0)
	{strcpy(error_message,"The value must be in [0 10]");return -1;}
      else
	sciSetFontStyle((sciPointObj *) pobj, xtmp);
    }
  else if (strcmp(marker,"font_name") == 0) {
    sciSetFontName((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow));
  }

  else if (strcmp(marker,"text_box_mode") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT) {
	if (strcmp(cstk(*value),"off") == 0)
	  pTEXT_FEATURE (pobj)->fill =  -1;
	else if  (strcmp(cstk(*value),"centered") == 0)
	  pTEXT_FEATURE (pobj)->fill =  0;
	else if  (strcmp(cstk(*value),"filled") ==  0)
	  pTEXT_FEATURE (pobj)->fill =  1;
	else  
	  {strcpy(error_message,"Value must be 'off', 'centered' or 'filled'");
	  return -1;}
      }
      else
	{strcpy(error_message,"text_box_mode property does not exist for this handle");
	return -1;}
    }
  else if (strcmp(marker,"text_box") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT)
      {
        if ( *numcol * *numrow != 2 )
        {
          strcpy(error_message,"text_box must be a 2D vector.");
          return -1 ;
        }
	pTEXT_FEATURE (pobj)->wh[0]=*stk(*value);
	pTEXT_FEATURE (pobj)->wh[1]=*stk(*value+1);
      }
      else
	{
          strcpy(error_message,"text_box property does not exist for this handle");
          return -1;
        }
    }
  else if (strcmp(marker,"text") == 0) {
    sciSetText((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow));
  }
  /******************/
  else if (strcmp(marker,"auto_clear") == 0) {
    if (strcmp(cstk(*value),"on")==0 )
      sciSetAddPlot((sciPointObj *) pobj,FALSE);
    else if (strcmp(cstk(*value),"off")==0 )
      sciSetAddPlot((sciPointObj *) pobj,TRUE);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if (strcmp(marker,"auto_scale") == 0) {
    if (strcmp(cstk(*value),"on")==0 )
      sciSetAutoScale((sciPointObj *) pobj, TRUE);
    else if (strcmp(cstk(*value),"off")==0 )
      sciSetAutoScale((sciPointObj *) pobj, FALSE);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if ((strcmp(marker,"zoom_box") == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN)) { 
    /* On doit avoir avoir une matrice 4x1 */
    if (*numcol * *numrow == 4)
      scizoom(stk(*value),pobj);  
    else if (*numcol * *numrow == 0)
      unzoom();
    else
      {strcpy(error_message,"Argument must be a vector of size 4");return -1;}

  } 
  else if ((strcmp(marker,"zoom_state") == 0)  && (sciGetEntityType (pobj) == SCI_SUBWIN)){
    if ((strcmp(cstk(*value),"on") == 0))
      { 
	if (!sciGetZooming((sciPointObj *)pobj))
	  {strcpy(error_message,"set zoom box ( set('zoom_box',[xmin ymin xmax ymax]))");return -1;}
	else
	  {strcpy(error_message,"Object is already zoomed");return -1;}
      }
    else if ((strcmp(cstk(*value),"off") == 0)) 
      { unzoom();
      sciSetZooming((sciPointObj *)pobj,FALSE);
      } 
    else
      {strcpy(error_message,"Value must be 'on/off'");       return -1;}
  }  
  /***********************************************/
  else if (strcmp(marker,"clip_box") == 0)  { 
    /* On doit avoir avoir une matrice 4x1 */
    if (*numcol * *numrow == 4){
      sciSetClipping((sciPointObj *)pobj, stk(*value));
      sciSetIsClipping(pobj, 1);
    }
    else {strcpy(error_message,"Argument must be a vector of size 4");return -1;}

  } 
  else if (strcmp(marker,"clip_state") == 0) {
    if ((strcmp(cstk(*value),"clipgrf") == 0))
      sciSetIsClipping( (sciPointObj *)pobj,0);
    else if ((strcmp(cstk(*value),"off") == 0)) 
      sciSetIsClipping( (sciPointObj *)pobj,-1);
    else if ((strcmp(cstk(*value),"on") == 0))
      /*       if(sciGetClipping(pobj) != NULL){ */
      if(sciGetIsClipRegionValuated(pobj) == 1){
	sciSetIsClipping( (sciPointObj *)pobj,1);
      }
      else{
	sciSetIsClipping( (sciPointObj *)pobj,0);
      }
    else
      {strcpy(error_message,"Value must be 'clipgrf', 'on' or 'off'"); return -1;}
  }		
  else if (strcmp(marker,"data") == 0){
    CheckAndUpdate_x_shift(pobj,*numrow); /* used only on Polyline */
    CheckAndUpdate_y_shift(pobj,*numrow); /* used only on Polyline */
    CheckAndUpdate_z_shift(pobj,*numrow); /* used only on Polyline */
    
    sciSetPoint((sciPointObj *)pobj, stk(*value), numrow, numcol);
    
  }

  /**************** callback *********************/
  else if (strcmp(marker,"callbackmevent") == 0) {
    sciSetCallbackMouseEvent((sciPointObj *)pobj,(int) *stk(*value));
  }
  else if (strcmp(marker,"callback") == 0) {
    sciAddCallback((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow),1);
  }




  /******** AXES *******************************/
  else if (strcmp(marker,"x_label") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"x_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
  else if (strcmp(marker,"y_label") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"y_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
  else if (strcmp(marker,"z_label") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"z_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
  else if (strcmp(marker,"title") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"title property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }

  else if (strcmp(marker,"tics_direction") == 0)
    {   
      if (pAXES_FEATURE (pobj)->ny == 1)
	{ 
	  if(strcmp(cstk(*value),"top") == 0) 
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"u",1);
	  else if (strcmp(cstk(*value),"bottom") == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"d",1);
	  else
	    {strcpy(error_message,"Second argument must be 'top' or 'bottom'");return -1;}
	} 
      else
	{
	  if(strcmp(cstk(*value),"right") == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"r",1);
	  else if  (strcmp(cstk(*value),"left") == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"l",1);
	  else
	    {strcpy(error_message,"Second argument must be 'right' or 'left' ");return -1;}
	}
    }
  else if (strcmp(marker,"x_location") == 0)
    {  
      char loc;
      if(strcmp(cstk(*value),"top") == 0)
	loc='u';
      else if  (strcmp(cstk(*value),"bottom") == 0)
	loc='d';
      else if (strcmp(cstk(*value),"middle") == 0)
	loc='c';
      else  
	{strcpy(error_message,"Second argument must be 'top', 'bottom' or 'middle'");return -1;}
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->axes.xdir = loc;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"y_location") == 0)
    {   
      char loc;
      if(strcmp(cstk(*value),"left") == 0)
	loc='l';
      else if  (strcmp(cstk(*value),"right") == 0)
	loc='r';
      else if (strcmp(cstk(*value),"middle") == 0)
	loc='c';
      else  
	{strcpy(error_message,"Second argument must be 'left', 'right' or 'middle'");return -1;}
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->axes.ydir = loc;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"tight_limits") == 0) 
    {                   
      if ((strcmp(cstk(*value),"off") == 0)) 
	pSUBWIN_FEATURE (pobj)->tight_limits=FALSE;
      else if ((strcmp(cstk(*value),"on") == 0))
	pSUBWIN_FEATURE (pobj)->tight_limits=TRUE;
      else
	{strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
    } 
  else if (strcmp(marker,"closed") == 0)
    { 
      if(sciGetEntityType(pobj) != SCI_POLYLINE)
	{strcpy(error_message,"closed property does not exist for this handle");return -1;}

      if (strcmp(cstk(*value),"on")==0 )
	pPOLYLINE_FEATURE(pobj)->closed = 1;
      else if (strcmp(cstk(*value),"off")==0 )
	pPOLYLINE_FEATURE(pobj)->closed = 0;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    } 
  else if (strcmp(marker,"auto_position") == 0)
    { 
      if(sciGetEntityType(pobj) != SCI_LABEL)
	{strcpy(error_message,"auto_position does not exist for this handle");return -1;}

      if (strcmp(cstk(*value),"on")==0 )
	pLABEL_FEATURE(pobj)->auto_position = TRUE;
      else if (strcmp(cstk(*value),"off")==0 )
	pLABEL_FEATURE(pobj)->auto_position = FALSE;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    }  
  else if (strcmp(marker,"auto_rotation") == 0)
    { 
      if(sciGetEntityType(pobj) != SCI_LABEL)
	{strcpy(error_message,"auto_rotation does not exist for this handle");return -1;}

      if (strcmp(cstk(*value),"on")==0 )
	pLABEL_FEATURE(pobj)->auto_rotation = TRUE;
      else if (strcmp(cstk(*value),"off")==0 )
	pLABEL_FEATURE(pobj)->auto_rotation = FALSE;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    } 
  else if (strcmp(marker,"position") == 0)
    {
      if ( sciGetAutoPosition( pobj ) )
      {
        sciSetAutoPosition( pobj, FALSE ) ;
      }
      if (sciGetEntityType(pobj)== SCI_UIMENU)
      {
        pUIMENU_FEATURE(pobj)->MenuPosition=(int)stk(*value)[0];
      }
      else if(sciGetEntityType(pobj) == SCI_LABEL)
      {
        sciSetPosition(pobj,stk(*value)[0],stk(*value)[1]);
      }
      else
      {
        strcpy(error_message,"position does not exist for this handle");
        return -1;
      }
    }
  /* F.Leray adding auto_ticks flags */
  else if (strcmp(marker,"auto_ticks") == 0) 
    {
      if(*numcol == 1){

	ptr= *(char ***)value;

	if ((strcmp(ptr[0],"off") == 0)) 
	  {
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[0]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[1]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[2]=FALSE;
	  }
	else if ((strcmp(ptr[0],"on") == 0))
	  {
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[0]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[1]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.auto_ticks[2]=TRUE;
	  }
	else
	  {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
      }
      else if (*numcol == 2 || *numcol == 3){

	ptr= *(char ***)value;

	for (i = 0; i < *numcol; i++ )
	  {
	    if ((strcmp(ptr[i],"off") == 0)) 
	      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[i]=FALSE;
	    else if ((strcmp(ptr[i],"on") == 0))
	      pSUBWIN_FEATURE (pobj)->axes.auto_ticks[i]=TRUE;
	    else
	      {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
	  }	
      }
      else
	{strcpy(error_message,"Number of the second argument must be taken between 1 to 3");return -1;}
    }
  else if (strcmp(marker,"axes_reverse") == 0) 
    {
      if(*numcol == 1){

	ptr= *(char ***)value;

	if ((strcmp(ptr[0],"off") == 0)) 
	  {
	    pSUBWIN_FEATURE (pobj)->axes.reverse[0]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.reverse[1]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.reverse[2]=FALSE;
	  }
	else if ((strcmp(ptr[0],"on") == 0))
	  {
	    pSUBWIN_FEATURE (pobj)->axes.reverse[0]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.reverse[1]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.reverse[2]=TRUE;
	  }
	else
	  {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
      }
      else if (*numcol == 2 || *numcol == 3){

	ptr= *(char ***)value;

	for (i = 0; i < *numcol; i++ )
	  {
	    if ((strcmp(ptr[i],"off") == 0)) 
	      pSUBWIN_FEATURE (pobj)->axes.reverse[i]=FALSE;
	    else if ((strcmp(ptr[i],"on") == 0))
	      pSUBWIN_FEATURE (pobj)->axes.reverse[i]=TRUE;
	    else
	      {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
	  }	
      }
      else
	{strcpy(error_message,"Number of the second argument must be taken between 1 to 3");return -1;}
    }

  else if (strcmp(marker,"view") == 0) 
    { 
      /* DJ.A 2003 */
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {                  
	if ((strcmp(cstk(*value),"2d") == 0)) 
	  {  
	    if(pSUBWIN_FEATURE (pobj)->is3d == FALSE) return 0;  /* Adding F.Leray 18.0604 */
	    if (sciGetSurface(pobj) == (sciPointObj *) NULL)
	      {
		pSUBWIN_FEATURE (pobj)->is3d = FALSE;
		pSUBWIN_FEATURE (pobj)->project[2]= 0;
	      }
	    pSUBWIN_FEATURE (pobj)->theta_kp=pSUBWIN_FEATURE (pobj)->theta;
	    pSUBWIN_FEATURE (pobj)->alpha_kp=pSUBWIN_FEATURE (pobj)->alpha;  
	    pSUBWIN_FEATURE (pobj)->alpha  = 0.0;
	    pSUBWIN_FEATURE (pobj)->theta  = 270.0;
	    if(sciGetCurrentScilabXgc () != (struct BCG *) NULL)
	      UpdateSubwinScale(pobj);
	    /* 	    sciRedrawFigure(); /\* F.Leray 10.06.04 Adding 2 lines here... *\/ */
	    pSUBWIN_FEATURE (pobj)->is3d = FALSE; /*...and here */
	  } 
	else if ((strcmp(cstk(*value),"3d") == 0)){
	  if(pSUBWIN_FEATURE (pobj)->is3d == TRUE) return 0; /* Adding F.Leray 18.0604 */
	  pSUBWIN_FEATURE (pobj)->is3d = TRUE;
	  Obj_RedrawNewAngle(pobj,pSUBWIN_FEATURE (pobj)->theta_kp,pSUBWIN_FEATURE (pobj)->alpha_kp);
	  wininfo("alpha=%.1f,theta=%.1f",pSUBWIN_FEATURE (pobj)->alpha_kp,pSUBWIN_FEATURE (pobj)->theta_kp);
	}            
	else
	  {strcpy(error_message,"Second argument must be '2d' or '3d'");return -1;}
      }
      else
	{strcpy(error_message,"view property does not exist for this handle");return -1;}

    } 
  else if (strcmp(marker,"axes_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      if (*numrow * *numcol != 4) 
	{strcpy(error_message,"Second argument must have 4 elements r");return -1;}
      for (i=0;i<4;i++) {
	pSUBWIN_FEATURE (pobj)->WRect[i]=stk(*value)[i];

	set_scale ("ttffff", pSUBWIN_FEATURE (pobj)->WRect,
		   NULL, NULL,
		   NULL, NULL);
      }
    }
    else
      {strcpy(error_message,"axes_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"data_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      
      /* JB Silvy 09/11/05 */
      sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
      double * bounds = stk(*value) ;
      double   xMin ;
      double   xMax ;
      double   yMin ;
      double   yMax ;
      double   zMin = 1.0 ; /* initialize to avoid checking between 2D and 3D */
      double   zMax = 2.0 ;

      /* get the bounds of the matrix */
      /* only allowed 2x2, 2x3, 1x4, 4x1, 1x6 and 6x1 matrices */
      char * errorMsg = "Second argument should be a 2x2, 2x3, 1x4, 4x1, 1x6 or 6x1 matrix." ;
      switch ( *numrow )
      {
      case 1 : /* row vector */
        if ( *numcol == 4 )
        {
          xMin = bounds[0] ;
          xMax = bounds[1] ;
          yMin = bounds[2] ;
          yMax = bounds[3] ;
        }
        else if( *numcol == 6 )
        {
          xMin = bounds[0] ;
          xMax = bounds[1] ;
          yMin = bounds[2] ;
          yMax = bounds[3] ;
          zMin = bounds[4] ;
          zMax = bounds[5] ;
        }
        else
        {
          strcpy( error_message, errorMsg ) ;
          return -1 ;
        }
        break ;

      case 2 : /* 2x2 or 2x3 matrix */
        if ( *numcol == 2 )
        {
          xMin = bounds[0] ;
          yMin = bounds[2] ;
          xMax = bounds[1] ;
          yMax = bounds[3] ;
        }
        else if ( *numcol == 3 )
        {
          xMin = bounds[0] ;
          yMin = bounds[2] ;
          zMin = bounds[4] ;
          xMax = bounds[1] ;
          yMax = bounds[3] ;
          zMax = bounds[5] ;
        }
        else
        {
          strcpy( error_message, errorMsg ) ;
          return -1 ;
        }
        break ;

      case 4 : /* column vector for 2D */
        if ( *numcol == 1 )
        {
          xMin = bounds[0] ;
          xMax = bounds[1] ;
          yMin = bounds[2] ;
          yMax = bounds[3] ;
        }
        else
        {
          strcpy( error_message, errorMsg ) ;
          return -1 ;
        }
        break ;
      case 6 : /* column vector for 3D */
        if ( *numcol == 1 )
        {
          xMin = bounds[0] ;
          xMax = bounds[1] ;
          yMin = bounds[2] ;
          yMax = bounds[3] ;
          zMin = bounds[4] ;
          zMax = bounds[5] ;
        }
        else
        {
          strcpy( error_message, errorMsg ) ;
          return -1 ;
        }
        break ;
      default:
        strcpy( error_message, errorMsg ) ;
        return -1 ;
      }

      /* check if there is not an inf within the values */
      /* since this has not any meaning */
      if (    !finite(xMin) || !finite(xMax)
           || !finite(yMin) || !finite(yMax)
           || !finite(zMin) || !finite(zMax) )
      {
        strcpy(error_message,"Error : data_bounds values must be finite.");
        return -1 ;
      }

      /* check if the bounds are corrects */
      /* allows equality with bounds since it is working */
      if ( xMin > xMax || yMin > yMax || zMin > zMax )
      {
        strcpy(error_message,"Error : Min and Max values for one axis do not verify Min <= Max.");
        return -1 ;
      }

      /* check for logflags that values are greater then 0 */
      if (   ( ppSubWin->logflags[0] == 'l' && xMin <= 0.0 )
          || ( ppSubWin->logflags[1] == 'l' && yMin <= 0.0 )
          || ( ppSubWin->logflags[2] == 'l' && zMin <= 0.0 ) )
      {
        strcpy(error_message,"Error: bounds on axis must be strictly positive to use logarithmic mode\n") ;
        return -1 ;
      }

      /* copy the values in the axis */
      if ( *numrow * *numcol == 4 )
      { 
        /* 2D */
        ppSubWin->SRect[0] = xMin ;
        ppSubWin->SRect[1] = xMax ;
        ppSubWin->SRect[2] = yMin ;
        ppSubWin->SRect[3] = yMax ;
      }
      else
      {
        /* 3D */
        ppSubWin->SRect[0] = xMin ;
        ppSubWin->SRect[1] = xMax ;
        ppSubWin->SRect[2] = yMin ;
        ppSubWin->SRect[3] = yMax ;
        ppSubWin->SRect[4] = zMin ;
        ppSubWin->SRect[5] = zMax ;
      }

      ppSubWin->FirstPlot = FALSE;


    }
    else if (sciGetEntityType (pobj) == SCI_SURFACE) {
      if (*numrow * *numcol != 6) 
	{strcpy(error_message,"Second argument must have 6 elements ");return -1;}
      for (i=0;i<6;i++) {
	pSURFACE_FEATURE (pobj)->ebox[i]=stk(*value)[i];
      }
    }
    else
      {strcpy(error_message,"data_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"margins") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {   
      /**DJ.Abdemouche 2003**/
      if (*numrow * *numcol != 4) 
	{strcpy(error_message,"Second argument must have 4 elements ");return -1;}
      pSUBWIN_FEATURE (pobj)->ARect[0]=stk(*value)[0];
      pSUBWIN_FEATURE (pobj)->ARect[1]=stk(*value)[1];
      pSUBWIN_FEATURE (pobj)->ARect[2]=stk(*value)[2];
      pSUBWIN_FEATURE (pobj)->ARect[3]=stk(*value)[3];}
    else
      {strcpy(error_message,"margins property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"tics_color") == 0) {   
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->ticscolor = (int)stk(*value)[0];
    else if (sciGetEntityType (pobj) == SCI_SUBWIN){
      sciprint("Warning: tics_color use is deprecated and no more taken into account, use foreground property to edit Axes color\n");
      pSUBWIN_FEATURE (pobj)->axes.ticscolor = (int)stk(*value)[0];
    }
    else
      {strcpy(error_message,"tics_color property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"tics_style") == 0) { 
    if((strcmp(cstk(*value),"v") != 0)&&
       (strcmp(cstk(*value),"r") != 0)&&
       (strcmp(cstk(*value),"i") != 0)) { 
      strcpy(error_message,"tics must be 'v' or 'r' or 'i'");return -1;
    }

    if (sciGetEntityType (pobj) == SCI_AXES){
      double *vector = NULL;
      int N = 0;
      char xy_type = *cstk(*value);

      if(pAXES_FEATURE (pobj)->str != NULL){
	if(ComputeXIntervals(pobj,xy_type,&vector,&N,1) != 0){
	  Scierror(999,"Error: Bad size in tics_coord ; you must first increase the size of the tics_coord\n");
	  return 0;
	}
	if( pAXES_FEATURE (pobj)->nb_tics_labels < N){
	  Scierror(999,"Warning: tics_labels has been set by user ; you must first increase the size of the tics_labels string vector before switching to '%c' tics_style mode\n",xy_type);
	  return 0;
	}
      }

      pAXES_FEATURE (pobj)->tics = xy_type;
    }
    else {
      strcpy(error_message,"tics_style property does not exist for this handle");return -1;
    }
  }
  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */
  else if ((strcmp(marker,"sub_tics") == 0) || (strcmp(marker,"sub_ticks") == 0))
  {
    if (sciGetEntityType (pobj) == SCI_AXES)
    {
      pAXES_FEATURE (pobj)->subint= (int) *stk(*value);
    }
    else if (sciGetEntityType (pobj) == SCI_SUBWIN) 
    {
      sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
      if ((*numcol != 3 )&& (*numcol != 2))
      {
	strcpy(error_message,"Value must have two elements (three if 3D universe) ");
        return -1;
      }
      ppSubWin->flagNax = TRUE;
      for ( i = 0; i < *numcol ; i++ )
      {
	char logflag = ppSubWin->logflags[i] ;
        int  nbTicks ;

	if(logflag == 'l')
        {
	  /* 	  sciprint("Subtics number can not be set while using logarithmic scaling\n"); */
	  continue;
	}
        
        nbTicks = (int) *stk(*value+i);
	if( nbTicks >= 0 )
        {
	  ppSubWin->axes.nbsubtics[i] = nbTicks + 1 ;
        } 
	else
        {
	  ppSubWin->axes.nbsubtics[i] = 1 ;
        }
      }
    }
    else
    {
      strcpy(error_message,"sub_ticks property does not exist for this handle");
      return -1 ;
    }

  }
  else if (strcmp(marker,"format_n") == 0)
    {
      strncpy(pAXES_FEATURE (pobj)->format,cstk(*value),1);   
    }
  else if (strcmp(marker,"tics_segment") == 0) 
    {
      if (strcmp(cstk(*value),"on")==0 )
	pAXES_FEATURE (pobj)->seg=1;
      else if (strcmp(cstk(*value),"off")==0 )
	pAXES_FEATURE (pobj)->seg=0;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')");
      return -1;}
    }

  else if (strcmp(marker,"labels_font_size") == 0)	{
    xtmp =  (int) *stk(*value);
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->fontsize = xtmp;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* pSUBWIN_FEATURE (pobj)->axes.fontsize = (int) *stk(*value);*/
      sciSetFontDeciWidth(pobj,(int) (100*xtmp));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_size property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_color") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->textcolor=(int)*stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* pSUBWIN_FEATURE (pobj)->axes.textcolor=(int)*stk(*value);*/
      sciSetFontForeground(pobj,(int)*stk(*value));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_color property does not exist for this handle");return -1;}
  }	
  else if (strcmp(marker,"labels_font_style") == 0)	{ /* Adding F.Leray 09.04.04 : For the moment sciAxes have no style property*/
    if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE)
      sciSetFontStyle(pobj,(int)*stk(*value));
    else
      {strcpy(error_message,"labels_font_style property does not exist for this handle");return -1;}
  }	
  else if (strcmp(marker,"tics_labels") == 0) 
    {
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a vector"); return -1;}
      else
	{
	  ptr= *(char ***)value; /**DJ.Abdemouche 2003**/
	  if (pAXES_FEATURE(pobj)->nb_tics_labels > *numcol)
	    {sprintf(error_message,"Value must have at least %d elements",pAXES_FEATURE(pobj)->nb_tics_labels);
	    return -1;}
	  else
	    {
	      if(pAXES_FEATURE(pobj)->str != NULL)
		for(i=0;i<pAXES_FEATURE(pobj)->nb_tics_labels;i++) 
		  { FREE(pAXES_FEATURE(pobj)->str[i]); pAXES_FEATURE(pobj)->str[i] = NULL;}
	      FREE(pAXES_FEATURE(pobj)->str);
	      pAXES_FEATURE(pobj)->str =ptr;
	      pAXES_FEATURE(pobj)->nb_tics_labels = *numcol; /* could be increased to support xy_type switching (i.e. xy_type='v' -> xy_type='r') */
	    }
	}
    }
  else if (strcmp(marker,"xtics_coord") == 0) 
    { 
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a row vector");return -1;}
      else if (( pAXES_FEATURE(pobj)->nx == 1) &&(*numcol!=1))
	{strcpy(error_message,"Second argument must be a scalar");return -1;}
      else if (( pAXES_FEATURE(pobj)->nx != 1) &&(*numcol==1))
	{strcpy(error_message,"Second argument  must be a vector");return -1;}
      else
	{
	  int N = 0;
	  double *vector = NULL;
	  char c_format[5];
	  char **foo = (char **) NULL;

	  /* what follows remains here as it was */

	  FREE(pAXES_FEATURE(pobj)->vx); pAXES_FEATURE(pobj)->vx = NULL;

	  if ((pAXES_FEATURE(pobj)->vx = (double *) MALLOC (*numcol * sizeof (double))) == NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_coord");return -1;}

	  pAXES_FEATURE(pobj)->nx= *numcol;
	  for (i = 0; i < *numcol;i++)
	    pAXES_FEATURE(pobj)->vx[i]=*stk(*value+i); 

	  ComputeXIntervals(pobj,pAXES_FEATURE (pobj)->tics,&vector,&N,0);
	  ComputeC_format(pobj,c_format);

	  if(pAXES_FEATURE(pobj)->str != NULL)
	    for(i=0;i<pAXES_FEATURE(pobj)->nb_tics_labels;i++) 
	      { FREE(pAXES_FEATURE(pobj)->str[i]); pAXES_FEATURE(pobj)->str[i] = NULL;}

	  FREE(pAXES_FEATURE(pobj)->str); pAXES_FEATURE (pobj)->str = NULL;


	  if((foo=MALLOC(N*(sizeof(char *))))==NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
	  for(i=0;i<N;i++){
	    if((foo[i]=MALLOC(256*(sizeof(char)+1)))==NULL){
	      strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
	  }


	  for(i=0;i<N;i++){
	    if(pAXES_FEATURE (pobj)->nx < (*numcol))
	      sprintf(foo[i],c_format,vector[i]);
	    else
	      sprintf(foo[i],c_format,vector[i]);
	  }

	  FREE(vector); vector = (double *) NULL;

	  /* I recompute the nb_tics_labels and save the new strings */
	  pAXES_FEATURE (pobj)->nb_tics_labels = N;
	  pAXES_FEATURE(pobj)->str=foo;


	}
    }
  else if (strcmp(marker,"ytics_coord") == 0) 
    {
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a row vector");return -1;}
      else if (( pAXES_FEATURE(pobj)->ny ==1) &&( *numcol != 1))
	{strcpy(error_message,"Second argument must be a scalar");return -1;}
      else if (( pAXES_FEATURE(pobj)->ny !=1) && (*numcol == 1))
	{strcpy(error_message,"Second argument must be a vector");return -1;}
      else
	{                       
	  int N = 0;
	  double *vector = NULL;
	  char c_format[5];
	  char **foo = (char **) NULL;

	  /* what follows remains here as it was */

	  FREE(pAXES_FEATURE(pobj)->vy); pAXES_FEATURE(pobj)->vy = NULL;

	  if ((pAXES_FEATURE(pobj)->vy = MALLOC (*numcol * sizeof (double))) == NULL)
	    return 0;
	  pAXES_FEATURE(pobj)->ny= *numcol; 
	  for (i = 0; i < *numcol;i++)
	    pAXES_FEATURE(pobj)->vy[i]=*stk(*value+i);  

	  ComputeXIntervals(pobj,pAXES_FEATURE (pobj)->tics,&vector,&N,0);
	  ComputeC_format(pobj,c_format);

	  if(pAXES_FEATURE(pobj)->str != NULL)
	    for(i=0;i<pAXES_FEATURE(pobj)->nb_tics_labels;i++) 
	      { FREE(pAXES_FEATURE(pobj)->str[i]); pAXES_FEATURE(pobj)->str[i] = NULL;}

	  FREE(pAXES_FEATURE(pobj)->str); pAXES_FEATURE (pobj)->str = NULL;

	  if((foo=MALLOC(N*(sizeof(char *))))==NULL){
	    strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
	  for(i=0;i<N;i++){
	    if((foo[i]=MALLOC(256*(sizeof(char)+1)))==NULL){
	      strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
	  }

	  for(i=0;i<N;i++){
	    if(pAXES_FEATURE (pobj)->ny < (*numcol))
	      sprintf(foo[i],c_format,vector[i]);
	    else
	      sprintf(foo[i],c_format,vector[i]);
	  }

	  FREE(vector); vector = (double *) NULL;

	  /* I recompute the nb_tics_labels and save the new strings */
	  pAXES_FEATURE (pobj)->nb_tics_labels = N;
	  pAXES_FEATURE(pobj)->str=foo;

	}
    } 
  else if  (strcmp(marker,"box") == 0) 
    {
      if ((strcmp(cstk(*value),"on") == 0))
	sciSetIsBoxed(pobj,TRUE);
      else if ((strcmp(cstk(*value),"off") == 0))  
	sciSetIsBoxed(pobj,FALSE);
      else
	{strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
    }
  else if (strcmp(marker,"grid") == 0) {/**DJ.Abdemouche 2003**/
    for (i = 0; i < *numcol; i++ )
      {
	if (stk(*value)[i] < -1)
	  {strcpy(error_message,"Argument must be -1 (no grid)  or number of color");return -1;}
	else
	  {pSUBWIN_FEATURE (pobj)->grid[i]=(int) stk(*value)[i];}
      }   
  }
  else if  (strcmp(marker,"axes_visible") == 0) 
    {
      if(*numcol == 1){

	ptr= *(char ***)value;

	if ((strcmp(ptr[0],"off") == 0)) 
	  {
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[0]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[1]=FALSE;
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[2]=FALSE;
	    /* 	    pSUBWIN_FEATURE (pobj)->isaxes= FALSE; */
	  }
	else if ((strcmp(ptr[0],"on") == 0))
	  {
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[0]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[1]=TRUE;
	    pSUBWIN_FEATURE (pobj)->axes.axes_visible[2]=TRUE;
	    /* 	    pSUBWIN_FEATURE (pobj)->isaxes= TRUE; */
	  }
	else
	  {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
      }
      else if (*numcol == 2 || *numcol == 3){

	ptr= *(char ***)value;

	for (i = 0; i < *numcol; i++ )
	  {
	    if ((strcmp(ptr[i],"off") == 0)) {
	      pSUBWIN_FEATURE (pobj)->axes.axes_visible[i]=FALSE;
	      /*   pSUBWIN_FEATURE (pobj)->isaxes= FALSE; */ /* DO NOT PUT at all !*/
	    }
	    else if ((strcmp(ptr[i],"on") == 0)){
	      pSUBWIN_FEATURE (pobj)->axes.axes_visible[i]=TRUE;
	      /* 	      pSUBWIN_FEATURE (pobj)->isaxes= TRUE; */
	    }
	    else
	      {strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
	  }	
      }
      else
	{strcpy(error_message,"Number of the second argument must be taken between 1 to 3");return -1;}
    }
  else if (strcmp(marker,"hiddencolor") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->hiddencolor=(int) stk(*value)[0];
      else if (sciGetEntityType (pobj) == SCI_SURFACE)
	pSURFACE_FEATURE (pobj)->hiddencolor=(int) stk(*value)[0];
      else
	{strcpy(error_message,"hiddencolor property does not exist for this handle");return -1;}

    }
  /**DJ.Abdemouche 2003**/
  else if  (strcmp(marker,"isoview") == 0) 
    {  /* DJ.A 2003 */
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if ((strcmp(cstk(*value),"on") == 0)) 
	  pSUBWIN_FEATURE (pobj)->isoview= TRUE; 
	else if ((strcmp(cstk(*value),"off") == 0))  
	  pSUBWIN_FEATURE (pobj)->isoview= FALSE;
	else
	  {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
      }
      else
	{strcpy(error_message,"isoview property does not exist for this handle");return -1;}

    }  /**/
  else if  (strcmp(marker,"cube_scaling") == 0)  /* F.Leray 22.04.04 */
    {  
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if(pSUBWIN_FEATURE (pobj)->is3d == FALSE)
	  sciprint("Warning: cube_scaling property is only used in 3D mode\n");

	if ((strcmp(cstk(*value),"on") == 0)) 
	  pSUBWIN_FEATURE (pobj)->cube_scaling= TRUE; 
	else if ((strcmp(cstk(*value),"off") == 0))
	  pSUBWIN_FEATURE (pobj)->cube_scaling= FALSE;
	else
	  {strcpy(error_message,"Value must be 'on' or 'off' / Only use for 3D mode");return -1;}
      }
      else
	{strcpy(error_message,"cube_scaling property does not exist for this handle");return -1;}

    }  /**/
  else if (strcmp(marker,"log_flags") == 0)
    {
      char *flags;
      flags=cstk(*value);
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if (((*numrow * *numcol == 2) || (*numrow * *numcol == 3)) && 
	    (flags[0]=='n'||flags[0]=='l')&&
	    (flags[1]=='n'||flags[1]=='l')) {

	  /* Update the data_bounds values ?? NO we will see if data_bounds contains
	     negative boundaries and send an error message to the user */

	  if( (pSUBWIN_FEATURE (pobj)->SRect[0] <= 0. || pSUBWIN_FEATURE (pobj)->SRect[1] <= 0.) 
	      && flags[0] == 'l') {
	    strcpy(error_message,
		   "Error: data_bounds on x axis must be strictly positive to switch to logarithmic mode");
	    return -1;
	  }
	  else
	    {
	      pSUBWIN_FEATURE (pobj)->axes.u_xlabels = ReBuildUserTicks( pSUBWIN_FEATURE (pobj)->logflags[0], flags[0],
									 pSUBWIN_FEATURE (pobj)->axes.u_xgrads, 
									 &pSUBWIN_FEATURE (pobj)->axes.u_nxgrads, 
									 pSUBWIN_FEATURE (pobj)->axes.u_xlabels);

	      /*  ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[0], flags[0],'x'); */
	      pSUBWIN_FEATURE (pobj)->logflags[0]=flags[0];
	    }

	  if((pSUBWIN_FEATURE (pobj)->SRect[2] <= 0. || pSUBWIN_FEATURE (pobj)->SRect[3] <= 0.) 
	     && flags[1] == 'l'){ 
	    strcpy(error_message,
		   "Error: data_bounds on y axis must be strictly positive to switch to logarithmic mode");
	    return -1;
	  }
	  else
	    {
	      pSUBWIN_FEATURE (pobj)->axes.u_ylabels = ReBuildUserTicks( pSUBWIN_FEATURE (pobj)->logflags[1], flags[1],  
									 pSUBWIN_FEATURE (pobj)->axes.u_ygrads, 
									 &pSUBWIN_FEATURE (pobj)->axes.u_nygrads, 
									 pSUBWIN_FEATURE (pobj)->axes.u_ylabels);

	      /* 	      ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[1], flags[1],'y'); */
	      pSUBWIN_FEATURE (pobj)->logflags[1]=flags[1];
	    }

	  /* third new case (for z) F.Leray 03.11.04 */
	  if((*numrow * *numcol == 3) &&
	     (flags[2]=='n'||flags[2]=='l')){
	    if( (pSUBWIN_FEATURE (pobj)->SRect[4] <= 0. || pSUBWIN_FEATURE (pobj)->SRect[5] <= 0.) 
		&& flags[2] == 'l')
	      {strcpy(error_message,"Error: data_bounds on z axis must be strictly positive to switch to logarithmic mode");return -1;}
	    else
	      {
		pSUBWIN_FEATURE (pobj)->axes.u_zlabels = ReBuildUserTicks( pSUBWIN_FEATURE (pobj)->logflags[2], flags[2],  
									   pSUBWIN_FEATURE (pobj)->axes.u_zgrads, 
									   &pSUBWIN_FEATURE (pobj)->axes.u_nzgrads, 
									   pSUBWIN_FEATURE (pobj)->axes.u_zlabels);

		/*  ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[0], flags[0],'x'); */
		pSUBWIN_FEATURE (pobj)->logflags[2]=flags[2];
	      }


	  }


	}
	else 
	  {strcpy(error_message,"incorrect log_flags value");return -1;}
      }
      else
	{strcpy(error_message,"log_flags property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"arrow_size") == 0) {
    if(sciGetEntityType (pobj) == SCI_SEGS)
      pSEGS_FEATURE (pobj)->arrowsize = *stk(*value);      
    else
      {strcpy(error_message,"arrow_size property does not exist for this handle");return -1;}
  }  
  else if ((strcmp(marker,"segs_color") == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){  
    if (pSEGS_FEATURE (pobj)->ptype == 0){
      if ((*numrow)* (*numcol)==1) {
	pSEGS_FEATURE (pobj)->iflag=0;
	for (i = 0; i < (pSEGS_FEATURE (pobj)->Nbr1)/2 ;i++)
	  pSEGS_FEATURE (pobj)->pstyle[i]=(int)*stk(*value);
      } 
      else if (((*numrow)* (*numcol)== (pSEGS_FEATURE (pobj)->Nbr1)/2)) {
	pSEGS_FEATURE (pobj)->iflag=1;
	for (i = 0; i < (pSEGS_FEATURE (pobj)->Nbr1)/2 ;i++)
	  pSEGS_FEATURE (pobj)->pstyle[i]=(integer) *stk(*value+i);
      }
      else
	{ 
	  sprintf(error_message,"segs color has a wrong size (%d), expecting 1 or (%d )",((*numrow)* (*numcol)) ,(pSEGS_FEATURE (pobj)->Nbr1)/2 );
	  return -1;
	}
    } 
    else
      {strcpy(error_message,"segs_color property does not exist for this handle");return -1;}
  }
  else if ((strcmp(marker,"colored") == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){
    if (pSEGS_FEATURE (pobj)->ptype != 0){
      if ((strcmp(cstk(*value),"on") == 0))
	pSEGS_FEATURE (pobj)->typeofchamp = 1;
      else if ((strcmp(cstk(*value),"off") == 0))  
	pSEGS_FEATURE (pobj)->typeofchamp = 0;
      else
	{strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
    }
    else
      {strcpy(error_message,"colored property does not exist for Segs");return -1;}
  }
  /**************** Matplot Grayplot *********************/
  else if (strcmp(marker,"data_mapping") == 0) {
    if (sciGetEntityType (pobj) == SCI_GRAYPLOT) {
      if ((strcmp(cstk(*value),"scaled") == 0)||(strcmp(cstk(*value),"direct") == 0)) 
	strncpy(pGRAYPLOT_FEATURE (pobj)->datamapping,cstk(*value),6);
      else
	{strcpy(error_message,"Value must be 'direct' or 'scaled'");return -1;}
    }
    else
      {strcpy(error_message,"data_mapping property does not exist for this handle");return -1;}
  } 
  /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0) {
    /* DJ.A 2003 */
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      Obj_RedrawNewAngle(pobj,*stk(*value+1),*stk(*value));
      wininfo("alpha=%.1f,theta=%.1f",pSUBWIN_FEATURE (pobj)->alpha,pSUBWIN_FEATURE (pobj)->theta);
    }
    else
      {strcpy(error_message,"rotation_angles property does not exist for this handle");return -1;}
  }
  /*DJ.A merge*/
  else if (strcmp(marker,"color_mode") == 0) {    
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      /*    int m3n,n3n,nc,j; */
      int flagcolor;
      sciSurface * psurf = pSURFACE_FEATURE (pobj);

      flagcolor = psurf->flagcolor;

      psurf->flag[0]= (integer) stk(*value)[0];

      if(flagcolor != 0 && flagcolor !=1){ 
	if(psurf->m3n * psurf->n3n == 0) {/* There is no color matrix/vect. in input : update the fake color one */
	  int j,nc;

	  if(flagcolor == 2 || flagcolor == 4)
	    nc = psurf->dimzy; /* rappel: dimzy always equal n3*/
	  else if(flagcolor == 3)
	    nc = psurf->dimzx * psurf->dimzy;

	  FREE(psurf->zcol);

	  if ((psurf->zcol = MALLOC ( nc * sizeof (double))) == NULL)
	    return -1;

	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    psurf->zcol[j]= psurf->flag[0];


	  if(flagcolor != 0 && flagcolor !=1){ 
	    /* We need to rebuild ...->color matrix */
	    if(psurf->cdatamapping == 0){ /* scaled */
	      FREE(psurf->color);
	      LinearScaling2Colormap(pobj);
	    }
	    else{
	      int nc = psurf->nc;

	      FREE(psurf->color);

	      if(nc>0){
		if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
		  return -1;
	      }

	      for(i=0;i<nc;i++)
		psurf->color[i] = psurf->zcol[i];
	      /* copy zcol that has just been freed and re-alloc + filled in */
	    }
	  }
	}
      }

      if(psurf->typeof3d==SCI_FAC3D)  /* we have to deal with colors... and may be update because we just changed  psurf->flag[0]*/
	if(psurf->flagcolor == 0)
	  {
	    pSURFACE_FEATURE (pobj)->izcol = 0;
	  }
    }
    else
      {strcpy(error_message,"color_mode property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"color_flag") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      if (*numrow * *numcol != 1)
	{strcpy(error_message,"Second argument must have 1 elements ");return -1;}
      if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_PLOT3D) {
	if ((*stk(*value)<0)||(*stk(*value)>1))
	  {strcpy(error_message,"Second argument must be equal to 0 or 1");return -1;}
	pSURFACE_FEATURE (pobj)->flagcolor= (int)stk(*value)[0];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_FAC3D) 
	{
	  int m3n,n3n,nc,j,ii,flagcolor=(int)stk(*value)[0];
	  /*int *zcol;*/
	  /*F.Leray psurf for debug*/
	  sciSurface * psurf = pSURFACE_FEATURE (pobj);

	  if ((*stk(*value)<0)||(*stk(*value)>4))
	    {strcpy(error_message,"Second argument must be 0 1 2 3 or 4");return -1;}


	  /* F.Leray 18.03.04 Something goes wrong here: No need to re-build/re-alloc zcol normally!!*/
	  /* YES in fact but use of another fixed array named inputCMoV (inputColorMatrixorVector)*/



	  if (pSURFACE_FEATURE (pobj)->flagcolor == stk(*value)[0])
	    return 0;

	  if(flagcolor == 0)
	    {
	      pSURFACE_FEATURE (pobj)->izcol = 0;
	    }
	  else if(flagcolor == 1)
	    {
	      pSURFACE_FEATURE (pobj)->izcol = 0;
	    }
	  else if(flagcolor == 2) /* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  */
	    {
	      m3n = pSURFACE_FEATURE (pobj)->m3n;
	      n3n = pSURFACE_FEATURE (pobj)->n3n;
	      nc = psurf->dimzy; /* rappel: dimzy always equal n3*/

	      psurf->nc = nc;
	      FREE(psurf->zcol); psurf->zcol = NULL;

	      if(nc>0){
		if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
		  return -1;
	      }

	      if(m3n * n3n != 0 ){ /* There is either a color matrix or vector*/
		if( m3n==1 || n3n ==1)
		  {
		    /* We have just enough information to fill the psurf->zcol array*/
		    for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		      psurf->zcol[j]= psurf->inputCMoV[j];                 	
		  }
		else if (!( m3n==1 || n3n ==1))
		  {
		    /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
		    /* NO !! Let's do better; F.Leray 08.05.04 : */
		    /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
		    /* in our example: m3n=4 and n3n=400 */
		    for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		      {
			double tmp = 0;
			for(ii=0;ii<m3n;ii++)
			  tmp = tmp +  psurf->inputCMoV[j*m3n + ii];
			tmp = tmp / m3n;
			psurf->zcol[j]= tmp;
		      }
		  }
	      }
	      else if (m3n * n3n == 0) {/* There is no color matrix/vect. in input */
		for (j = 0;j < psurf->dimzy; j++)   /* nc value is dimzy*/
		  psurf->zcol[j]= psurf->flag[0];
	      }

	    }
	  else if (flagcolor == 3) /* interpolated (shading) case */
	    {
	      m3n = pSURFACE_FEATURE (pobj)->m3n;
	      n3n = pSURFACE_FEATURE (pobj)->n3n;
	      nc = psurf->dimzx * psurf->dimzy;

	      psurf->nc = nc;
	      FREE(psurf->zcol); psurf->zcol = NULL;


	      if ((psurf->zcol = MALLOC ( nc * sizeof (double))) == NULL)
		return -1;


	      if(m3n * n3n != 0 ){ /* There is either a color matrix or vector*/
		if( m3n==1 || n3n ==1) /* COLOR VECTOR */
		  {
		    /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
		    /* We repeat the data:*/
		    for(i = 0; i< psurf->dimzy; i++){
		      for (j = 0;j < psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
			psurf->zcol[(psurf->dimzx)*i+j]= psurf->inputCMoV[i];
		    }               	
		  }
		else if (!( m3n==1 || n3n ==1)) /* COLOR MATRIX */
		  {
		    /* We have just enough information to fill the psurf->zcol array*/
		    for (j = 0;j < (psurf->dimzx) * (psurf->dimzy); j++)   /* nc value is dimzy*/
		      psurf->zcol[j]=  psurf->inputCMoV[j];
		  }
	      }
	      else if (m3n * n3n == 0) {/* There is no color matrix/vect. in input */

		for(i = 0; i<  psurf->dimzy; i++){
		  for (j = 0;j <  psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		    psurf->zcol[( psurf->dimzx)*i+j]= psurf->flag[0];
		}  
	      }
	    }
	  /* New : case == 4 introduced on the 10.03.05 (F.Leray) to support faceted (flat) Matlab case */
	  else if(flagcolor == 4) /* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  */
	    {
	      m3n = pSURFACE_FEATURE (pobj)->m3n;
	      n3n = pSURFACE_FEATURE (pobj)->n3n;
	      nc = psurf->dimzy; /* rappel: dimzy always equal n3*/

	      psurf->nc = nc;
	      FREE(psurf->zcol); psurf->zcol = NULL;

	      if(nc>0){
		if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
		  return -1;
	      }

	      if(m3n * n3n != 0 ){ /* There is either a color matrix or vector */
		if( m3n==1 || n3n ==1)
		  {
		    /* We have just enough information to fill the psurf->zcol array*/
		    for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		      psurf->zcol[j]= psurf->inputCMoV[j];                 	
		  }
		else if (!( m3n==1 || n3n ==1))
		  {
		    /* input : color matrix , we use 1 color per facet with Matlab selection mode (no average computed) */
		    /* HERE is the difference with case 2 */
		    for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		      psurf->zcol[j] = psurf->inputCMoV[j*m3n];
		  }
	      }
	      else if (m3n * n3n == 0) {/* There is no color matrix/vect. in input */
		for (j = 0;j < psurf->dimzy; j++)   /* nc value is dimzy*/
		  psurf->zcol[j]= psurf->flag[0];
	      }
	    }

	  if(flagcolor != 0 && flagcolor !=1){ 
	    /* We need to rebuild ...->color matrix */
	    if(psurf->cdatamapping == 0){ /* scaled */
	      FREE(psurf->color);
	      LinearScaling2Colormap(pobj);
	    }
	    else{
	      int nc = psurf->nc;

	      FREE(psurf->color);

	      if(nc>0){
		if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
		  return -1;
	      }

	      for(i=0;i<nc;i++)
		psurf->color[i] = psurf->zcol[i];
	      /* copy zcol that has just been freed and re-alloc + filled in */
	    }
	  }

	  /* Finally, update the flagcolor */
	  pSURFACE_FEATURE (pobj)->flagcolor = flagcolor;

	}
    }
  }
  else if (strcmp(marker,"cdata_mapping") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_FAC3D) {
	if ((strcmp(cstk(*value),"scaled") == 0)){
	  if(pSURFACE_FEATURE (pobj)->cdatamapping != 0){ /* not already scaled */
	    LinearScaling2Colormap(pobj);
	    pSURFACE_FEATURE (pobj)->cdatamapping = 0;
	  }
	} 
	else if ((strcmp(cstk(*value),"direct") == 0)){
	  if(pSURFACE_FEATURE (pobj)->cdatamapping != 1){ /* not already direct */
	    int nc = pSURFACE_FEATURE (pobj)->nc;

	    FREE(pSURFACE_FEATURE (pobj)->color); pSURFACE_FEATURE (pobj)->color = NULL;

	    /* 	    printf("pSURFACE_FEATURE (pobj)->color = %d\n",pSURFACE_FEATURE (pobj)->color); */
	    /* 	    printf("nc = %d\n",nc); */
	    /* 	    fflush(NULL); */

	    if(nc>0){
	      if ((pSURFACE_FEATURE (pobj)->color = MALLOC (nc * sizeof (double))) == NULL)
		return -1;
	    }

	    for(i=0;i<nc;i++)
	      pSURFACE_FEATURE (pobj)->color[i] = pSURFACE_FEATURE (pobj)->zcol[i];

	    pSURFACE_FEATURE (pobj)->cdatamapping = 1;
	  }
	}
	else
	  {strcpy(error_message,"cdata_mapping value must be 'scaled' or 'direct'");return -1;}
      }
      else
	{strcpy(error_message,"cdata_mapping property only exists for Fac3d surfaces");return -1;}
    }
    else
      {strcpy(error_message,"cdata_mapping property does not exist for this handle");return -1;}
  }

  else if (strcmp(marker,"surface_color") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) { 
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PARAM3D1)  {
	if (pSURFACE_FEATURE (pobj)->dimzy != *numrow * *numcol) 
	  {sprintf(error_message,"Second argument must have %d elements ",
		   pSURFACE_FEATURE (pobj)->dimzy);return -1;}
	for (i=0;i<pSURFACE_FEATURE (pobj)->dimzy;i++) 
	  pSURFACE_FEATURE (pobj)->zcol[i]= (integer)stk(*value)[i];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)  {
	strcpy(error_message,"surface_color cannot be set in this case");
	return -1;
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)  {
	if (pSURFACE_FEATURE (pobj)->flagcolor<2){
	  strcpy(error_message,"surface_color cannot be set in this case");
	  return -1;
	} 
	else {
	  int N;
	  if (pSURFACE_FEATURE (pobj)->flagcolor==2)
	    N=pSURFACE_FEATURE (pobj)->dimzy;
	  else
	    N=pSURFACE_FEATURE (pobj)->dimzy * pSURFACE_FEATURE (pobj)->dimzx;
	  if (*numrow * *numcol != N)
	    {sprintf(error_message,"Second argument must have %d elements ",N);return -1;}
	  for (i=0;i<N;i++) 
	    pSURFACE_FEATURE (pobj)->zcol[i]= (integer)stk(*value)[i];
	}
      }
    }
    else {
      strcpy(error_message,"surface_color property does not exist for this handle");
      return -1;
    } 
  }
  else if (strcmp(marker,"triangles") == 0) {
    if (sciGetEntityType (pobj) == SCI_FEC) { 
      double *pnoeud;
      if (*numcol != 5)
	{strcpy(error_message,"Second argument must have 5 columns ");return -1;}
      if (*numrow !=pFEC_FEATURE (pobj)->Ntr) {
	pnoeud=pFEC_FEATURE(pobj)->pnoeud;
	if ((pFEC_FEATURE(pobj)->pnoeud = MALLOC (*numrow * 5* sizeof (int))) == NULL){
	  strcpy(error_message,"Not enough memory");
	  pFEC_FEATURE(pobj)->pnoeud=pnoeud;
	  return -1;
	}
      }
      for (i=0;i<*numrow*5;i++) 
	pFEC_FEATURE (pobj)->pnoeud[i]=stk(*value)[i];
    }
    else
      {strcpy(error_message,"triangles property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"z_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_FEC) { 
      if (*numcol *  *numrow!= 2)
	{strcpy(error_message,"Second argument must have 2 elements ");return -1;}

      for (i=0;i<2;i++) 
	pFEC_FEATURE (pobj)->zminmax[i]=stk(*value)[i];
    }
    else
      {strcpy(error_message,"z_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"handle_visible") == 0)
  {
	if (sciGetEntityType (pobj) == SCI_UIMENU)
	{
		if ((strcmp(cstk(*value),"on") == 0)) 
			pUIMENU_FEATURE(pobj)->handle_visible=TRUE;
		else if ((strcmp(cstk(*value),"off") == 0))  
			pUIMENU_FEATURE(pobj)->handle_visible=FALSE;
		else
		{
			strcpy(error_message,"Value must be 'on' or 'off'");
			return -1;
		}
	}
	else
	{
		strcpy(error_message,"handle_visible property does not exist for this handle");
		return -1;
	}
  }
  else if (strcmp(marker,"callback_type") == 0)
  {
	  if (sciGetEntityType (pobj) == SCI_UIMENU)
	  {
		if ((strcmp(cstk(*value),"string") == 0))
		{
			pUIMENU_FEATURE(pobj)->CallbackType=0;
		}
		else
		if ((strcmp(cstk(*value),"C") == 0))
		{
			pUIMENU_FEATURE(pobj)->CallbackType=1;
		}
		else
		if ((strcmp(cstk(*value),"internal") == 0))
		{
			pUIMENU_FEATURE(pobj)->CallbackType=2;
		}
		else
		if ((strcmp(cstk(*value),"addmenu") == 0))
		{
			pUIMENU_FEATURE(pobj)->CallbackType=3;
		}
		else
		{
			strcpy(error_message,"Value must be 'string','C','internal','addmenu'");
			return -1;
		}
	  }
	  else
	  {
		  strcpy(error_message,"callback_type property does not exist for this handle");
		  return -1;
	  }
  }
  else if (strcmp(marker,"menu_enable") == 0)
  {
	  if (sciGetEntityType (pobj) == SCI_UIMENU)
	  {
		  if ((strcmp(cstk(*value),"on") == 0))
		  {
			  if (pUIMENU_FEATURE(pobj)->Enable != TRUE)
			  {
				  pUIMENU_FEATURE(pobj)->Enable=TRUE;
			  }
			  
		  }
		  else
		  if ((strcmp(cstk(*value),"off") == 0))
		  {
			  if (pUIMENU_FEATURE(pobj)->Enable != FALSE)
			  {
				  pUIMENU_FEATURE(pobj)->Enable=FALSE;
			  }
		  }
		  else
		  {
			  strcpy(error_message,"Value must be 'on' or 'off'");
			  return -1;
		  }
	  }
	  else
	  {
		  strcpy(error_message,"menu_enable property does not exist for this handle");
		  return -1;
	  }
  }
  else 
    {
		sprintf(error_message,"Unknown  property %s",marker);
		return -1;
	}
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int LinearScaling2Colormap(sciPointObj* pobj)
{
  int i;
  int nbcol =  sciGetNumColors (pobj);	/* the number of the colors inside the current colormap */
  sciSurface * psurf = pSURFACE_FEATURE (pobj);

  double min,max;

  double indexmin = 1.;
  double indexmax = (double) nbcol;

  int nc = psurf->nc; /* the number of colors contained inside zcol matrix */


  if(psurf->zcol == NULL){
    sciprint("Color matrix is NULL ; can not build color scaled linearly into the current colormap");
    return -1;
  }

  if (((psurf->color = MALLOC (nc * sizeof (double))) == NULL)){
    sciprint("Allocation failed for color in LinearScaling2Colormap");
    return -1;
  }

  /* get the min inside zcol */
  min = psurf->zcol[0];
  for(i=0;i<nc;i++)
    if(min > psurf->zcol[i]) min = psurf->zcol[i];

  /* get the max inside zcol */
  max = psurf->zcol[0];
  for(i=0;i<nc;i++)
    if(max < psurf->zcol[i]) max = psurf->zcol[i];

  if(min != max)
    {
      /* linear interpolation */
      double A = (indexmin-indexmax)/(min-max); 
      double B = (min*indexmax-indexmin*max)/(min-max);
      for(i=0;i<nc;i++)
	psurf->color[i] = A*psurf->zcol[i] + B + 0.1;
    }
  else
    {
      double C = (indexmin+indexmax)/2;
      for(i=0;i<nc;i++)
	psurf->color[i] = C;
    }

  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Called by a.log_flags='nn','ln','nl', or 'll'*/
/* For the moment, z has no logflag F.Leray 05.10.04 */
char ** ReBuildUserTicks( char old_logflag, char new_logflag, double * u_xgrads, int *u_nxgrads, char ** u_xlabels)
{

  if(old_logflag==new_logflag) return u_xlabels; /* nothing to do l->l or n->n */

  if(u_xgrads!=NULL)
    {
      if(old_logflag=='n' && new_logflag=='l') /* n->l */ /* 10-> 1, 100->2 ...*/
	{

	  u_xlabels=CaseLogflagN2L(u_nxgrads,u_xgrads,u_xlabels);

	}
      else if(old_logflag=='l' && new_logflag=='n')
	{
	  int nbtics = *u_nxgrads;
	  int i;

	  for(i=0;i<nbtics;i++) u_xgrads[i] = exp10(u_xgrads[i]);

	}
    }

  return  u_xlabels;
}
/*-----------------------------------------------------------------------------------*/
/* Remove negative graduations when switching from N (linear) to L (logarithmic) scale */
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels)
{
  int nbtics = *u_nxgrads;
  int i;
  char ** ticklabel = (char **) NULL;
  int cmpteur = 0, cmpteur2 = 0, offset = 0;


  for(i=0;i<nbtics;i++) 
    {
      if(u_xgrads[i]<=0){
	sciprint("Warning: graduation number %d is ignored : when switching to logarithmic scale, we must have strictly positive graduations!\n",i);
      }
      else
	{
	  u_xgrads[cmpteur] = log10(u_xgrads[i]);
	  cmpteur++;
	}
    }

  if(cmpteur != nbtics)
    {
      if((ticklabel=(char **)MALLOC(cmpteur*sizeof(char *)))==NULL){
	sciprint("No more place for allocating ticklabel");
      }

      cmpteur2 = 0;
      offset = nbtics - cmpteur;
      for(i=0;i<cmpteur;i++){
	if((ticklabel[cmpteur2]=(char *)MALLOC((strlen(u_xlabels[i+offset])+1)*sizeof(char )))==NULL){
	  sciprint("No more place for allocating ticklabel");
	}
	strcpy(ticklabel[cmpteur2],u_xlabels[i+offset]);
	cmpteur2++;
      }

      for(i=0;i<nbtics;i++){ FREE(u_xlabels[i]); u_xlabels[i] = NULL;}

      FREE(u_xlabels); u_xlabels = NULL;
      u_xlabels = ticklabel;
    }

  *u_nxgrads = cmpteur;
  cmpteur = 0;
  cmpteur2 = 0;

  return u_xlabels;
}


int CheckAndUpdate_x_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = NULL;
  int i;

  if(sciGetEntityType(pobj) != SCI_POLYLINE)
    return 0;
  
  ppolyline = pPOLYLINE_FEATURE(pobj);


  if(ppolyline->x_shift == (double *) NULL)
    return 0;
  else
    {
      int size_x_old = ppolyline->n1; /* number of x data */
      if(size_x_old == numrow)
	return 0;
      else if(size_x_old > numrow)
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<numrow;i++)
	    new_bar[i] = ppolyline->x_shift[i];
	  
	  FREE(ppolyline->x_shift);
	  
	  ppolyline->x_shift = new_bar;
	  
	}
      else /* case where size_x_old < numrow */
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<size_x_old;i++)
	    new_bar[i] = ppolyline->x_shift[i];
	  
	  for(i=size_x_old;i<numrow;i++)
	    new_bar[i] = 0.;
	  
	  FREE(ppolyline->x_shift);
	  
	  ppolyline->x_shift = new_bar;
	}
    }
  
  return 0;
}

int CheckAndUpdate_y_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = NULL;
  int i;

  if(sciGetEntityType(pobj) != SCI_POLYLINE)
    return 0;
  
  ppolyline = pPOLYLINE_FEATURE(pobj);


  if(ppolyline->y_shift == (double *) NULL)
    return 0;
  else
    {
      int size_x_old = ppolyline->n1; /* number of x data */
      if(size_x_old == numrow)
	return 0;
      else if(size_x_old > numrow)
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<numrow;i++)
	    new_bar[i] = ppolyline->y_shift[i];
	  
	  FREE(ppolyline->y_shift);
	  
	  ppolyline->y_shift = new_bar;
	  
	}
      else /* case where size_x_old < numrow */
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<size_x_old;i++)
	    new_bar[i] = ppolyline->y_shift[i];
	  
	  for(i=size_x_old;i<numrow;i++)
	    new_bar[i] = 0.;
	  
	  FREE(ppolyline->y_shift);
	  
	  ppolyline->x_shift = new_bar;
	}
    }
  
  return 0;
}

int CheckAndUpdate_z_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = NULL;
  int i;

  if(sciGetEntityType(pobj) != SCI_POLYLINE)
    return 0;
  
  ppolyline = pPOLYLINE_FEATURE(pobj);


  if(ppolyline->z_shift == (double *) NULL)
    return 0;
  else
    {
      int size_x_old = ppolyline->n1; /* number of x data */
      if(size_x_old == numrow)
	return 0;
      else if(size_x_old > numrow)
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<numrow;i++)
	    new_bar[i] = ppolyline->z_shift[i];
	  
	  FREE(ppolyline->z_shift);
	  
	  ppolyline->z_shift = new_bar;
	  
	}
      else /* case where size_x_old < numrow */
	{
	  double * new_bar = NULL;
	  
	  if((new_bar = (double *) MALLOC(numrow*sizeof(double))) == NULL)
	    {
	      strcpy(error_message,"No more place to allocate new_bar");
	      return -1;
	    }
	  
	  for(i=0;i<size_x_old;i++)
	    new_bar[i] = ppolyline->z_shift[i];
	  
	  for(i=size_x_old;i<numrow;i++)
	    new_bar[i] = 0.;
	  
	  FREE(ppolyline->z_shift);
	  
	  ppolyline->x_shift = new_bar;
	}
    }
  
  return 0;
}
/*-----------------------------------------------------------------------------------*/

