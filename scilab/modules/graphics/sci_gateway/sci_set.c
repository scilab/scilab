/*------------------------------------------------------------------------*/
/* file: sci_set.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_set.h"
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>

#include "bcg.h"
#include "Graphics.h" 

#include "stack-c.h"

#include "CloneObjects.h"
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"

/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "BuildObjects.h"
#include "DestroyObjects.h"

#include "Xcall1.h"
#include "Format.h"
#include "pixel_mode.h"
#include "getHandleProperty/setHandleProperty.h"
#include "ColorMapManagement.h"


#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern int versionflag;
/*-----------------------------------------------------------------------------------*/
static char error_message[256];
/*-----------------------------------------------------------------------------------*/
int setticks(char * xyztick, sciPointObj* psubwin, int * ptrindex, int * numrow, int * numcol);
int sciSet(sciPointObj *pobj, char *marker, int *value, int *numrow, int *numcol);
char ** ReBuildUserTicks( char old_logflag, char new_logflag, double * u_xgrads, int *u_nxgrads, char ** u_xlabels);
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels);
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/
int sci_set(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,numrow3,numcol3,l3,num,cur,na,verb=0;
  unsigned long hdl; 
  int lw,t2;
  BOOL vis_save = FALSE ;
  sciPointObj *pobj;

  /* F.Leray Adding some tmp variable for SCI_SURFACE / data case*/
  integer m3tl, n3tl, l3tl;
  int numrow[4],i;
  int numcol[4], lxyzcol[4];
  int ptrindex[2];
  int flagc = -1;
  int setStatus = 0 ; /* after the call to sciSet get the status : 0 <=> OK,          */ 
                      /*                                          -1 <=> Error,       */
                      /*                                           1 <=> nothing done */

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
      if ( *hstk(l1) != sciGetHandle(getFigureModel()) && *hstk(l1) != sciGetHandle(getAxesModel())
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_z_label))
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
      if ( *hstk(l1) != sciGetHandle(getFigureModel()) && *hstk(l1) != sciGetHandle(getAxesModel())
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_y_label) 
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_z_label))
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
      if (VarType(3) != t2)
      { 
	Scierror(999,"%s: uncompatible values for property type  '%s' \r\n",fname,cstk(l2));
	return 0;
      } 
      if ( VarType(3) == 1 )
      {
        GetRhsVar(3,"d",&numrow3,&numcol3,&l3) ;
      }
      else if ( VarType(3) == 9 )
      {
        GetRhsVar(3,"h",&numrow3,&numcol3,&l3);
      }
      else if ( VarType(3) == 10 ) 
      { 
        if (    strcmp( cstk(l2), "tics_labels"  ) != 0
             && strcmp( cstk(l2), "auto_ticks"   ) != 0
             && strcmp( cstk(l2), "axes_visible" ) != 0
             && strcmp( cstk(l2), "axes_reverse" ) != 0
             && strcmp( cstk(l2), "text"         ) != 0 )
        {
          GetRhsVar(3,"c",&numrow3,&numcol3,&l3);
        } 
	else
        {
	  GetRhsVar(3,"S",&numrow3,&numcol3,&l3);
        }
      }
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

      if ( VarType(2) == 1 )
      {
        GetRhsVar(2,"d",&numrow3,&numcol3,&l3);
      }
      else if ( VarType(2) == 9 )
      {
        GetRhsVar(2,"h",&numrow3,&numcol3,&l3);
      }
      else if ( VarType(2) == 10 )
      {
	if (   strcmp( cstk(l2), "tics_labels"  ) != 0
	    && strcmp( cstk(l2), "auto_ticks"   ) != 0
	    && strcmp( cstk(l2), "axes_visible" ) != 0
	    && strcmp( cstk(l2), "axes_reverse" ) != 0
               && strcmp( cstk(l2), "text"      ) != 0 )
        {
          GetRhsVar(2,"c",&numrow3,&numcol3,&l3);
        } 
	else
        {
	  GetRhsVar(2,"S",&numrow3,&numcol3,&l3);
        }
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
	
        if(VarType(3) != 16)
        {
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
    else /* F.Leray 02.05.05 : main case (using sciGetPoint routine inside GetProperty.c) */
    {
      if ( (setStatus = sciSet(pobj, cstk(l2), &l3, &numrow3, &numcol3)) < 0 ) {
	Scierror(999,"%s: %s\r\n",fname,error_message);
	return 0;
      }
    }

    if ( !( vis_save == 0 && sciGetVisibility(pobj)==0 ) && setStatus == 0 ) 
    {
      /* do not redraw figure if object remains invisible */
      if ((strcmp(cstk(l2),"figure_style") !=0) &&
	  (strcmp(cstk(l2),"old_style") !=0 ) && 
	  (strcmp(cstk(l2),"current_axes") !=0) &&
	  (strcmp(cstk(l2),"default_figure") !=0) && 
	  (strcmp(cstk(l2),"default_axes") !=0) &&
	  (pobj != getFigureModel()) && (pobj != getAxesModel())
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_title
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_x_label
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_y_label
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_z_label )
      { 
        /* Addings F.Leray 10.06.04 */
	num= sciGetNumFigure (pobj);    
	C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

	sciDrawObj(sciGetParentFigure(pobj)); /* F.Leray we redraw here*/
	/* 	EraseAndOrRedraw(pobj); */  /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */


	C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  else if ( ( setStatus = sciSet((sciPointObj *) NULL, cstk(l2), &l3, &numrow3, &numcol3) ) < 0 )
  {
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
        ppsubwin->axes.u_xgrads = MALLOC( prod * sizeof(double) ) ;
        if( ppsubwin->axes.u_xgrads == NULL )
        {
          return -1 ;
        }
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
/**@name int sciset(sciPointObj *pobj,char *marker, long *x, long *y, long *w, long *h)
 * Sets the value to the object
 */

extern void set_cf_type(int val);

int sciSet(sciPointObj *pobj, char *marker, int *value, int *numrow, int *numcol)
{
  int xtmp;
  int  i,v=1,verb=0;
  double dv=0.0; 
  char   **ptr;
  sciPointObj *psubwin ;

  /* debug F.Leray 28.04.04 */
  /* sciSubWindow * ppsubwin = NULL;*/

  if ((strcmp(marker,"figure_model") !=0) && (strcmp(marker,"axes_model") !=0)
      && (pobj  != getFigureModel())  && (pobj  != getAxesModel())
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_title
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_x_label
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_y_label
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_z_label ) /* Addings F.Leray 10.06.04 */
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
  {
    return set_color_map_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"old_style") == 0)
  {
    return set_old_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_style") == 0)
  {
    return set_figure_style_property( pobj, *value, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"pixel_drawing_mode") == 0)
  {
    return set_pixel_drawing_mode_property( pobj, *value, *numrow, *numcol ) ;
  }  
  else if ( strcmp(marker,"default_values") == 0 )
  {
    return set_default_values_property( pobj, *value, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"visible") == 0) 
  {
    return set_visible_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if ( strcmp(marker,"auto_resize") == 0 )
  {
    return set_auto_resize_property( pobj, *value, *numrow, *numcol ) ;
  }
  /*************************** Handles Properties ********/
  else if ((strcmp(marker,"current_obj") == 0) || (strcmp(marker,"current_entity") == 0))
  {
    return set_current_entity_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"current_axes") == 0) 
  {
    return set_current_axes_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"current_figure") == 0) 
  {
    return set_current_figure_property( pobj, *value, *numrow, *numcol ) ;
  }
  /************************  figure Properties *****************************/ 
  else if (strcmp(marker,"figure_position") == 0)
  {
    return set_figure_position_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"axes_size") == 0)
  {
    return set_axes_size_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"figure_size") == 0)
  {
    return set_figure_size_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_name") == 0)
  {
    return set_figure_name_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_id") == 0)
  {
    return set_figure_id_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"rotation_style") == 0)
  {
    return set_rotation_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"immediate_drawing") == 0)
  {
    return set_immediate_drawing_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"pixmap") == 0)
  {
    return set_pixmap_property( pobj, *value, *numrow, *numcol ) ;
  }
  /********************** context graphique ******************************/
  else if (strcmp(marker,"background") == 0)
  {
    return set_background_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"interp_color_vector") == 0)
  {
    return set_interp_color_vector_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"interp_color_mode") == 0)
  {
    return set_interp_color_mode_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"foreground") == 0)
  {
    return set_foreground_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"fill_mode") == 0)
  {
    return set_fill_mode_property( pobj, *value, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"thickness") == 0) 
  {
    return set_thickness_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"arrow_size_factor") == 0)
  {
    return set_arrow_size_factor_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"line_style") == 0)
  {
    return set_line_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"line_mode") == 0)
  {
    return set_line_mode_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"surface_mode") == 0)
  {
    return set_surface_mode_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_style") == 0)
  {
    return set_mark_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_mode") == 0)
  {
    return set_mark_mode_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_size_unit") == 0)
  {
    return set_mark_size_unit_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_size") == 0)
  {
    return set_mark_size_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_foreground") == 0)
  {
    return set_mark_foreground_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_background") == 0)
  {
    return set_mark_background_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"bar_width") == 0)
  {
    return set_bar_width_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"bar_layout") == 0)
  {
    return set_bar_layout_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"x_shift") == 0)
  {
    return set_x_shift_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_shift") == 0)
  {
    return set_y_shift_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"z_shift") == 0)
  {
    return set_z_shift_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"polyline_style") == 0)
  {
    return set_polyline_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  /************* font properties *********/
  else if (strcmp(marker,"font_size") == 0)
  {
    return set_font_size_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_angle") == 0)
  {
    return set_font_angle_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_foreground") == 0)
  {
    return set_font_foreground_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_color") == 0)
  {
    return set_font_color_property( pobj, *value, *numrow, *numcol ) ;
  }	
  else if (strcmp(marker,"font_style") == 0)
  {
    return set_font_style_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_name") == 0)
  {
    return set_font_name_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text_box_mode") == 0)
  {
    return set_text_box_mode_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"auto_dimensionning") == 0 )
  {
    return set_auto_dimensionning_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"alignment") == 0 )
  {
    return set_alignment_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text_box") == 0)
  {
    return set_text_box_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text") == 0)
  {
    return set_text_property( pobj, *value, *numrow, *numcol ) ;
  }
  /******************/
  else if (strcmp(marker,"auto_clear") == 0)
  {
    return set_auto_clear_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"auto_scale") == 0)
  {
    return set_auto_scale_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"zoom_box") == 0 )
  {
    return set_zoom_box_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if ( strcmp(marker,"zoom_state") == 0 )
  {
    return set_zoom_state_property( pobj, *value, *numrow, *numcol ) ;
  }
  /***********************************************/
  else if (strcmp(marker,"clip_box") == 0)
  {
    return set_clip_box_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"clip_state") == 0)
  {
    return set_clip_state_property( pobj, *value, *numrow, *numcol ) ;
  }		
  else if (strcmp(marker,"data") == 0)
  {
    return set_data_property( pobj, *value, *numrow, *numcol ) ;
  }

  /**************** callback *********************/
  else if (strcmp(marker,"callbackmevent") == 0)
  {
    return set_callbackmevent_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"callback") == 0)
  {
    return set_callback_property( pobj, *value, *numrow, *numcol ) ;
  }
  /******** AXES *******************************/
  else if (strcmp(marker,"x_label") == 0)
  {
    return set_x_label_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_label") == 0)
  {
    return set_y_label_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"z_label") == 0)
  {
    return set_z_label_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"title") == 0)
  {
    return set_title_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tics_direction") == 0)
  {
    return set_tics_direction_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"x_location") == 0)
  {
    return set_x_location_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_location") == 0)
  {
    return set_y_location_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tight_limits") == 0) 
  {
    return set_tight_limits_property( pobj, *value, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"closed") == 0)
  {
    return set_closed_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"auto_position") == 0)
  {
    return set_auto_position_property( pobj, *value, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"auto_rotation") == 0)
  {
    return set_auto_rotation_property( pobj, *value, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"position") == 0)
    {
      if ( sciGetAutoPosition( pobj ) )
      {
        sciSetAutoPosition( pobj, FALSE ) ;
      }
      if (sciGetEntityType(pobj)== SCI_UIMENU)
      {
        pUIMENU_FEATURE(pobj)->MenuPosition = (int)stk(*value)[0];
        return 0 ;
      }
      else if(sciGetEntityType(pobj) == SCI_LABEL)
      {
        return sciSetPosition(pobj,stk(*value)[0],stk(*value)[1]);
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
      if ( strcmp(cstk(*value),"2d") == 0 )
      { 
        return sciSetIs3d( pobj, FALSE ) ;
      }
      else if ( strcmp(cstk(*value),"3d") == 0 )
      {
        return sciSetIs3d( pobj, TRUE ) ;
      }            
	else
        {
          strcpy(error_message,"Second argument must be '2d' or '3d'");
          return -1;
        }
      }
    else
    {
      strcpy(error_message,"view property does not exist for this handle");
      return -1;
    }
    
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

      /* check if the bounds are corrects */
      /* allows equality with bounds since it is working */
      if ( xMin > xMax || yMin > yMax || zMin > zMax )
      {
        strcpy(error_message,"Error : Min and Max values for one axis do not verify Min <= Max.");
        return -1 ;
      }

      /* check for logflags that values are greater than 0 */
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
      return sciSetFontDeciWidth(pobj,(int) (100*xtmp));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_size property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_color") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->textcolor=(int)*stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* pSUBWIN_FEATURE (pobj)->axes.textcolor=(int)*stk(*value);*/
      return sciSetFontForeground(pobj,(int)*stk(*value));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_color property does not exist for this handle");return -1;}
  }	
  else if (strcmp(marker,"labels_font_style") == 0)	{ /* Adding F.Leray 09.04.04 : For the moment sciAxes have no style property*/
    if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE)
      return sciSetFontStyle(pobj,(int)*stk(*value));
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
      if ( sciGetEntityType( pobj ) == SCI_SUBWIN )
      {
        if ( (strcmp(cstk(*value),"off") == 0) )
        {
          return sciSetBoxType( pobj, BT_OFF ) ;
        }
        else if ( (strcmp(cstk(*value),"on") == 0) )
        {
          return sciSetBoxType( pobj, BT_ON ) ;
        }
        else if ( (strcmp(cstk(*value),"hidden_axis") == 0) )
        {
          return sciSetBoxType( pobj, BT_HIDDEN_AXIS ) ;
        }
        else if ( (strcmp(cstk(*value),"back_half") == 0) )
        {
          return sciSetBoxType( pobj, BT_BACK_HALF ) ;
        }
        else
        {
          strcpy(error_message,"Second argument must be 'on', 'off', 'hidden_axis' or 'back_half'. ") ;
          return -1;
        }
      }
      else
      {
        if ((strcmp(cstk(*value),"on") == 0))
          return sciSetIsBoxed(pobj,TRUE);
        else if ((strcmp(cstk(*value),"off") == 0))  
          return sciSetIsBoxed(pobj,FALSE);
        else
	{
          strcpy(error_message,"Second argument must be 'on' or 'off'") ;
          return -1;
        }
      }
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
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
      {
        sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
	if (((*numrow * *numcol == 2) || (*numrow * *numcol == 3)) && 
	    (flags[0]=='n'||flags[0]=='l')&&
	    (flags[1]=='n'||flags[1]=='l')) {

	  /* Update the data_bounds values ?? NO we will see if data_bounds contains
	     negative boundaries and send an error message to the user */

	  if( ( ppSubWin->SRect[0] <= 0. || ppSubWin->SRect[1] <= 0.) 
	      && flags[0] == 'l')
          {
	    strcpy(error_message,
		   "Error: data_bounds on x axis must be strictly positive to switch to logarithmic mode");
	    return -1;
	  }
	  else
          {
            ppSubWin->axes.u_xlabels = ReBuildUserTicks( ppSubWin->logflags[0], flags[0],
                                                         ppSubWin->axes.u_xgrads, 
                                                         &ppSubWin->axes.u_nxgrads, 
                                                         ppSubWin->axes.u_xlabels);
            
            /*  ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[0], flags[0],'x'); */
            ppSubWin->logflags[0]=flags[0];
          }

	  if((ppSubWin->SRect[2] <= 0. || ppSubWin->SRect[3] <= 0.) 
	     && flags[1] == 'l'){ 
	    strcpy(error_message,
		   "Error: data_bounds on y axis must be strictly positive to switch to logarithmic mode");
	    return -1;
	  }
	  else
          {
            ppSubWin->axes.u_ylabels = ReBuildUserTicks( ppSubWin->logflags[1], flags[1],  
                                                         ppSubWin->axes.u_ygrads, 
                                                         &ppSubWin->axes.u_nygrads, 
                                                         ppSubWin->axes.u_ylabels);
            
            /* 	      ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[1], flags[1],'y'); */
            ppSubWin->logflags[1]=flags[1];
          }

	  /* third new case (for z) F.Leray 03.11.04 */
	  if((*numrow * *numcol == 3) &&
	     (flags[2]=='n'||flags[2]=='l')){
	    if( (ppSubWin->SRect[4] <= 0. || ppSubWin->SRect[5] <= 0.) 
		&& flags[2] == 'l')
	      {strcpy(error_message,"Error: data_bounds on z axis must be strictly positive to switch to logarithmic mode");return -1;}
	    else
	      {
		ppSubWin->axes.u_zlabels = ReBuildUserTicks( ppSubWin->logflags[2], flags[2],  
                                                             ppSubWin->axes.u_zgrads, 
                                                             &ppSubWin->axes.u_nzgrads, 
                                                             ppSubWin->axes.u_zlabels);

		/*  ReBuildUserTicks( pobj, pSUBWIN_FEATURE (pobj)->logflags[0], flags[0],'x'); */
		ppSubWin->logflags[2]=flags[2];
	      }

	  }

	}
	else 
        {
          strcpy(error_message,"incorrect log_flags value");
          return -1;
        }
      }
      else
      {
        strcpy(error_message,"log_flags property does not exist for this handle");
        return -1;
      }
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
	strcpy(pGRAYPLOT_FEATURE (pobj)->datamapping,cstk(*value));
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

      sciSurface * ppSurf = pSURFACE_FEATURE ( pobj ) ;

      if ( ppSurf->typeof3d == SCI_FAC3D )
      {
	if ( (strcmp( cstk(*value), "scaled" ) == 0 ) )
        {
	  if( ppSurf->cdatamapping != 0 )
          { /* not already scaled */
	    LinearScaling2Colormap(pobj);
	    ppSurf->cdatamapping = 0;
	  }
	} 
	else if ((strcmp(cstk(*value),"direct") == 0))
        {
	  if(pSURFACE_FEATURE (pobj)->cdatamapping != 1)
          { 
            /* not already direct */
	    int nc = ppSurf->nc;

	    FREE( ppSurf->color ) ;
            ppSurf->color = NULL ;

	    /* 	    printf("pSURFACE_FEATURE (pobj)->color = %d\n",pSURFACE_FEATURE (pobj)->color); */
	    /* 	    printf("nc = %d\n",nc); */
	    /* 	    fflush(NULL); */

	    if(nc>0)
            {
	      if ((ppSurf->color = MALLOC (nc * sizeof (double))) == NULL)
              {
		return -1;
              }
	    }

	    for( i = 0 ; i < nc ; i++ )
            {
	      ppSurf->color[i] = ppSurf->zcol[i] ;
            }

	    ppSurf->cdatamapping = 1 ;
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
  else if( strcmp(marker,"hidden_axis_color") == 0 )
  {
    if ( sciGetEntityType (pobj) == SCI_SUBWIN )
    {
      int haColor = (int) *stk(*value) ;
      int colormapSize = sciGetNumColors(pobj) ;
      if ( haColor >= -2 && haColor <= colormapSize + 1 )
      {
        pSUBWIN_FEATURE(pobj)->axes.hiddenAxisColor = haColor ;
      }
    }
    else
    {
      strcpy(error_message,"hidden_axis_color property does not exist for this handle");
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
/*-----------------------------------------------------------------------------------*/
