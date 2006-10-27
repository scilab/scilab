/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include "intgget.h"
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

#include "intcommongraphics.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef WITH_TK
extern int GetTclCurrentFigure(void);
#endif

/*-----------------------------------------------------------------------------------*/
extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;
extern int versionflag;
/*-----------------------------------------------------------------------------------*/
static char error_message[70];
/*-----------------------------------------------------------------------------------*/
int sciGet(sciPointObj *pobj,char *marker);
int getgrayplotdata(sciPointObj *pobj);
int getchampdata(sciPointObj *pobj);
int get3ddata(sciPointObj *pobj);
int getticks(char * xyztick, sciPointObj* psubwin);
int ChooseGoodFormat(char * c_format,char logflag, double *_grads,int n_grads);
int BuildTListForTicks(double * locations, char ** labels, int nbtics);
double * ReBuildTicksLog2Lin(char logflag, int nbtics, double *grads);
BOOL GetHandleVisibilityOnUimenu(sciPointObj * pobj);
/*-----------------------------------------------------------------------------------*/
int gget(char *fname,unsigned long fname_len)
{
  integer m1,n1,numrow2,numcol2,l2 ;
  int l1;
  unsigned long hdl;

  int lw;
  sciPointObj *pobj;

  CheckRhs(1,2);
  CheckLhs(0,1);
  
  /*  set or create a graphic window */

  /* le premier argument peut etre soit un Id, ou un marker(auquel cas, get renvoi la valeur de 
     l'objet courant ) */
  switch(VarType(1))
    {
    case 1: /* tclsci handle */
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"get",3);return 0;
      return 0;
      break;
    case 9: /* scalar argument (hdl + string) */
      CheckRhs(2,2);
      GetRhsVar(1,"h",&m1,&n1,&l1);
      if (m1!=1||n1!=1) { 
	lw = 1 + Top - Rhs;
	C2F(overload)(&lw,"get",3);return 0;
      }
      GetRhsVar(2,"c",&numrow2,&numcol2,&l2);
      if ( *hstk(l1) != sciGetHandle(pfiguremdl) && *hstk(l1) != sciGetHandle(paxesmdl)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
	{
	  if ((strcmp(cstk(l2),"old_style") !=0)
	      &&(strcmp(cstk(l2),"default_figure") !=0) 
	      && (strcmp(cstk(l2),"default_axes") !=0) ) SciWin();
	  if (version_flag() ==0)
	    hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle */
	  else
	    hdl = (unsigned long)0;
	}/* DJ.A 08/01/04 */
      else 
	hdl = (unsigned long)*hstk(l1);
      break;
    case 10:/* string argument (string) */
      CheckRhs(1,1);
      GetRhsVar(1,"c",&numrow2,&numcol2,&l2);
      if (strcmp(cstk(l2),"default_figure") !=0 && strcmp(cstk(l2),"default_axes") !=0 )
	{
	  if (strcmp(cstk(l2),"old_style") == 0 || strcmp(cstk(l2),"current_figure") ==0) {
	    hdl = (unsigned long)0;
	  }
	  else
	    {
	      SciWin();
	      if (version_flag() == 0)
		{
		  /* Test debug F.Leray 13.04.04 */
		  sciPointObj * ppobj;
		  if ((strcmp(cstk(l2),"children") != 0) &&  
		      (strcmp(cstk(l2),"zoom_") !=0) && 
		      (strcmp(cstk(l2),"clip_box") !=0) && 
		      (strcmp(cstk(l2),"auto_") !=0)) 
		    {
		      ppobj = sciGetCurrentObj();
		      hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ());}
		  else  
		    hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));/* on recupere le pointeur d'objet par le handle */
		}
	      else
		hdl = (unsigned long)0;
	    }/* DJ.A 08/01/04 */
	}
      else
	hdl = (unsigned long)0;
    
      break;
    default:
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"get",3);return 0;
      /*Scierror(999,"%s: Incorrect argument\r\n",fname);*/
      return 0;
      break;
    }
  /* cstk(l2) est la commande, l3 l'indice sur les parametres de la commande */
  CheckLhs(0,1);
  if (hdl == (unsigned long)0 ) {
    if (sciGet((sciPointObj *) NULL, cstk(l2))!=0) {
      Scierror(999,"%s: %s\r\n",fname,error_message);
      return 0;
    }
  }
  else {
    if ((pobj = sciGetPointerFromHandle(hdl))){

      if(sciGetEntityType(pobj) == SCI_SUBWIN && (strcmp(cstk(l2),"x_ticks")==0 ||
						  strcmp(cstk(l2),"y_ticks")==0 ||
						  strcmp(cstk(l2),"z_ticks")==0))
	{
	  if(getticks(cstk(l2),pobj)!=0)
	    return 0;
	}
      else if(strcmp(cstk(l2),"data") == 0){ /* distinction for "data" treatment for champ and surface objects */
	if(sciGetEntityType(pobj) == SCI_SURFACE)
	  {
	    if (get3ddata(pobj)!=0)
	      return 0;
	  }
	else if((sciGetEntityType(pobj) == SCI_SEGS) && (pSEGS_FEATURE(pobj)->ptype == 1))
	  {
	    if (getchampdata(pobj)!=0)
	      return 0;
	  }  
	else if((sciGetEntityType(pobj) == SCI_GRAYPLOT)  && (pGRAYPLOT_FEATURE(pobj)->type == 0)) /* case 0: real grayplot */
	  {
	    if (getgrayplotdata(pobj)!=0)
	      return 0;
	  } 
	else /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
	  {
	    if (sciGet(pobj, cstk(l2))!=0)
	      {
		Scierror(999,"%s: %s\r\n",fname,error_message);
		return 0;
	      }
	  }
      }
      else /* F.Leray 02.05.05 : main case (all but "data") (using sciGetPoint routine inside GetProperty.c) */
	{
	  if (sciGet(pobj, cstk(l2))!=0)
	    {
	      Scierror(999,"%s: %s\r\n",fname,error_message);
	      return 0;
	    }
	}
    }
    else
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
  }
  LhsVar(1)=Rhs+1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* @name sciGet(sciPointObj *pobj,) */
/* Sets the value to the object */
int sciGet(sciPointObj *pobj,char *marker)
{
  int numrow, numcol, outindex, i,j,k;
  integer x[2], itmp = 0, na ;
  sciSons *toto;
  double *tab;
  char **str;
  sciPointObj *psubwin;
  int Etype,ids,iflag=0;

  if (pobj != (sciPointObj *)NULL && pobj  != pfiguremdl  && pobj  != paxesmdl
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ){ /* Addings F.Leray 10.06.04 */
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    Etype=sciGetEntityType (pobj);}

  if ((pobj == (sciPointObj *)NULL) && 
      (strcmp(marker,"old_style") !=0 ) && 
      (strcmp(marker,"figure_style") != 0) && 
      (strcmp(marker,"current_figure") != 0) &&
      (strcmp(marker,"default_figure") !=0) && 
      (strcmp(marker,"default_axes") !=0))
    {
      if (version_flag() == 0)
	{strcpy(error_message,"handle is not valid");return -1;}
      else
	{strcpy(error_message,"function not valid under old graphics style");return -1;}
    }
  else if (strcmp(marker,"figures_id") == 0){
    sciGetIdFigure (&ids,&numcol,&iflag);
    numrow   = 1;
    CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
    iflag = 1; 
    sciGetIdFigure (istk(outindex),&numcol,&iflag);

  }
  /***************** graphics mode *******************************/ 
  else if (strcmp(marker,"visible") == 0) {
    if (sciGetVisibility((sciPointObj *)pobj)){
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),"on", numrow*numcol);
    }
    else {
      numrow   = 1;
      numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),"off", numrow*numcol);
    }
  }
  else if (strcmp(marker,"pixel_drawing_mode") == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_FIGURE) {
	numrow = 1;
	i=pFIGURE_FEATURE (pobj)->gmode.xormode;
	numcol = strlen(pmodes[i]);
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),pmodes[i], numrow*numcol);
      }
      else
	{strcpy(error_message,"pixel_drawing_mode do not exist for this handle");return -1;}

    }  
  else if (strcmp(marker,"old_style") == 0)
    {
      if (versionflag != 0){
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"figure_style") == 0)
    {
      numrow   = 1;
      if (pobj != pfiguremdl)
	{
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  if (version_flag()!=0)
	    strncpy(cstk(outindex),"old", numrow*numcol); 
	  else 
	    strncpy(cstk(outindex),"new", numrow*numcol);      
	}
      else /* what is it usefull for ? F.Leray 20.04.05 */
	{
	  numcol   = 10;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"customised", numrow*numcol);
	}
    }
  else if (strcmp(marker,"auto_resize") == 0)
    {
      if (sciGetResize((sciPointObj *) pobj)){
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol); 
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  /************************  figure Properties *****************************/ 
  else if (strcmp(marker,"figure_position") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      numrow   = 1;numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
      if ((sciPointObj *) pobj != pfiguremdl)
	{
#if WIN32 /* Correction pour figure_position (Windows) Allan CORNET Mai 2004 */
	  stk(outindex)[0] = sciGetFigurePosX ((sciPointObj *) pobj); 
	  stk(outindex)[1] = sciGetFigurePosY ((sciPointObj *) pobj);
#else
	  stk(outindex)[0] = sciGetFigurePosX ((sciPointObj *) pobj)-4; 
	  stk(outindex)[1] = sciGetFigurePosY ((sciPointObj *) pobj)-20;
#endif
	}
      else
	{
	  stk(outindex)[0] = pFIGURE_FEATURE (pobj)->inrootposx; 
	  stk(outindex)[1] = pFIGURE_FEATURE (pobj)->inrootposy;
	}
    }  
  else if (strcmp(marker,"axes_size") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if ((sciPointObj *) pobj != pfiguremdl)
	{ 
	  stk(outindex)[0] = sciGetWidth ((sciPointObj *) pobj); 
	  stk(outindex)[1] = sciGetHeight ((sciPointObj *) pobj); 
	}
      else
	{
	  stk(outindex)[0] = pFIGURE_FEATURE (pobj)->windowdimwidth;
	  stk(outindex)[1] = pFIGURE_FEATURE (pobj)->windowdimheight; 
	}
    } 
  else if (strcmp(marker,"figure_size") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if ((sciPointObj *) pobj != pfiguremdl)
	{
	  int cur,num=pFIGURE_FEATURE(pobj)->number;
	  C2F(dr)("xget","window",&itmp,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	  C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

	  C2F(dr)("xget","wpdim",&itmp,x,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

	  pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth=x[0];  
	  pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight=x[1]; 
	}
      stk(outindex)[0] = pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth;  
      stk(outindex)[1] = pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight;  
    }
  else if (strcmp(marker,"figure_name") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      numrow = 1;
      numcol = sciGetNameLength((sciPointObj *) pobj);
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetName((sciPointObj *) pobj), numrow*numcol);
    }
  else if (strcmp(marker,"figure_id") == 0)
    {
      numrow = 1;
      numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetNum((sciPointObj *)pobj);
    }
  else if (strcmp(marker,"rotation_style") == 0) 
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      if (pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle == 0){
	numrow   = 1;
	numcol   = 5;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"unary", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 8;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"multiple",numrow*numcol);
      }
    }
  else if (strcmp(marker,"immediate_drawing") == 0) 
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      if (pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw == TRUE){
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off",numrow*numcol);
      }
    }
  /*Ajout A.Djalel*/
  else if (strcmp(marker,"pixmap") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      if (pFIGURE_FEATURE(pobj)->pixmap==1){
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }

  /********** Handles Properties *********************************************/       
  else if (strcmp(marker,"type") == 0)
    {
      numrow = 1;  
      numcol = strlen(sciGetCharEntityType((sciPointObj *) pobj));
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetCharEntityType((sciPointObj *) pobj), numrow*numcol);
    }
  else if (strcmp(marker,"parent") == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *hstk(outindex) = sciGetHandle(sciGetParent((sciPointObj *)pobj));
    }
  else if (strcmp(marker,"current_axes") == 0)
    {
      /*       sciPointObj * psubwin = sciGetSelectedSubWin(sciGetCurrentFigure()); */
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *hstk(outindex) = sciGetHandle(sciGetSelectedSubWin(sciGetCurrentFigure()));
    }

  else if (strcmp(marker,"current_figure") == 0)
    {

      if (cf_type==1) {
	C2F(sciwin)();/*SciWin();*/
	numrow   = 1;
	numcol   = 1;
	if(version_flag() == 0)
	  { /* return handle on the current figure */
	    CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	    *hstk(outindex) = sciGetHandle(sciGetCurrentFigure());
	  }
	else
	  { /* return id of the current figure */
	    double *XGC,dv=0;
	    int v=0;
	    struct BCG *CurrentScilabXgc = (struct BCG *) NULL;
	    C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
	    CurrentScilabXgc=(struct BCG *)XGC;


	    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	    *stk(outindex) = (double) CurrentScilabXgc->CurWindow;
	  }
      }
      else {
	numrow   = 1;
	numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
#ifdef WITH_TK
	*stk(outindex) = (double )GetTclCurrentFigure();
#endif
      }
    }
  else if((strcmp(marker,"current_obj") == 0) || (strcmp(marker,"current_entity") == 0))
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *hstk(outindex) = sciGetHandle(sciGetCurrentObj());
    }
  else if (strcmp(marker,"children") == 0)
    { 
      i = 0;
      toto = sciGetSons((sciPointObj *) pobj);
      while ((toto != (sciSons *)NULL) && (toto->pointobj != (sciPointObj *)NULL))
	{
	  /* DJ.A 30/12 */
	  if( sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_MERGE
	     && sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_LABEL
		 && GetHandleVisibilityOnUimenu((sciPointObj *)toto->pointobj) ) /* F.Leray 28.05.04 */
	    i++;
	  toto = toto->pnext;
	}
      numrow   = i;
      numcol   = 1;
      if(numrow==0) {
	CreateVar(Rhs+1,"d",&numrow,&numrow,&outindex);
      }
      else {
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	toto = sciGetSons((sciPointObj *) pobj);
	i = 0;
	while ((toto != (sciSons *)NULL) && (toto->pointobj != (sciPointObj *)NULL))
	  { /* DJ.A 30/12 */
	    if(sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_MERGE
	       && sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_LABEL
		   && GetHandleVisibilityOnUimenu((sciPointObj *)toto->pointobj) ) /* F.Leray 28.05.04 */
	      {
		hstk(outindex)[i] = sciGetHandle((sciPointObj *)toto->pointobj);
		i++;
	      }
	    toto = toto->pnext;/* toto is pointer to one son */
	  }
      }
    }
  else if (strcmp(marker,"hdl") == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *hstk(outindex) = sciGetHandle(sciGetCurrentObj());
    }
  /* DJ.A 08/01/04 */
  else if (strcmp(marker,"default_figure") == 0)
    {
      if (pfiguremdl != (sciPointObj *) NULL) 
	{
	  numrow   = 1;
	  numcol   = 1;	
	  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	  *hstk(outindex) =  sciGetHandle(pfiguremdl);
	}
      else
	{
	  strcpy(error_message,"Default figure do not existed ");
	  return -1;
	}
    }
  else if (strcmp(marker,"default_axes") == 0)
    { 
      if (paxesmdl != (sciPointObj *) NULL) 
	{
	  numrow   = 1;
	  numcol   = 1;
	  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	  *hstk(outindex) = sciGetHandle(paxesmdl);	
	}
      else
	{
	  strcpy(error_message,"Default axes do not existed ");
	  return -1;
	}
    }
  /******************************** context graphique  *****************************************/

  else if (strcmp(marker,"color_map") == 0)
  {
    if ( sciGetEntityType( pobj ) != SCI_FIGURE )
    {
      strcpy(error_message,"color_map property does not exist for this handle.");
      return -1;
    }
    if ((sciPointObj *) pobj != pfiguremdl)
    {
      numcol = 3;
      numrow = sciGetNumColors (pobj);
      if ( numrow == 0) numcol=0;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for  ( i = 0 ; i < numcol*numrow ; i++ )
      {
        stk(outindex)[i] = pFIGURE_FEATURE(pobj)->pcolormap[i];
      }
    }
    else
    {
      numcol = 3;
      numrow = sciGetNumColors (pobj);
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for  (i = 0; i < numcol*numrow; i++)
      {
        stk(outindex)[i] = pFIGURE_FEATURE(pfiguremdl)->pcolormap[i];
      }
    }
  }
  else if (strcmp(marker,"interp_color_vector") == 0)
    {
      int * vectmp = sciGetInterpVector((sciPointObj *) pobj);

      if(sciGetEntityType(pobj) != SCI_POLYLINE)
	{ strcpy(error_message,"interp_color_vector property does not exist for this handle"); return -1;}

      if(vectmp != NULL){
	numrow = 1;
	numcol = pPOLYLINE_FEATURE(pobj)->dim_icv;

	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex); 
	for (i=0;i<numcol;i++)
	  stk(outindex)[i] =  vectmp[i];
      }
      else{
	numrow = 0;
	numcol = 0;

	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex); 
      }
    }
  else if (strcmp(marker,"interp_color_mode") == 0)
    {
      if(pPOLYLINE_FEATURE(pobj)->isinterpshaded == TRUE){
	numrow = 1;
	numcol = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else{
	numrow = 1;
	numcol = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"background") == 0) /**DJ.Abdemouche 2003**/
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);

      *stk(outindex) = sciGetBackgroundToDisplay((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"foreground") == 0) 
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);

      *stk(outindex) = sciGetForegroundToDisplay((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"fill_mode") == 0) 
    {
      if (sciGetIsFilled((sciPointObj *) pobj)==1) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"thickness") == 0) 
    {
      numrow   = 1;numcol   = 1;
 	 
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetLineWidth((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"arrow_size_factor") == 0) 
    {
      numrow   = 1;numcol   = 1;
      if(sciGetEntityType(pobj) == SCI_POLYLINE){
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pPOLYLINE_FEATURE(pobj)->arsize_factor;
      }
    }
  else if (strcmp(marker,"line_style") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetLineStyle((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"line_mode") == 0)
    {
      if (sciGetIsLine((sciPointObj *)pobj) == 1) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {	
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"surface_mode") == 0)
    {
      if((sciGetEntityType(pobj) == SCI_PLOT3D) ||
	 (sciGetEntityType(pobj) == SCI_FAC3D)  ||
	 (sciGetEntityType(pobj) == SCI_SURFACE)){
	if (sciGetIsLine((sciPointObj *)pobj) == 1) {
	  numrow   = 1;
	  numcol   = 2;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"on", numrow*numcol);
	}
	else {
	  numrow   = 1;
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"off", numrow*numcol);
	}
      }
      else {strcpy(error_message,"Surface_mode value can not be accessed with this object, use line_mode"); return -1;}
    }
  else if (strcmp(marker,"mark_style") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetMarkStyle((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"mark_mode") == 0)
    {
      if (sciGetIsMark((sciPointObj *)pobj) == 1) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"mark_size_unit") == 0)
    {
      numrow   = 1;
      if (sciGetMarkSizeUnit((sciPointObj *)pobj) == 1){
	numcol = 5;
	CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	strncpy(cstk(outindex),"point", numrow*numcol);
      }
      else if(sciGetMarkSizeUnit((sciPointObj *)pobj) == 2){
	numcol = 9;
	CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	strncpy(cstk(outindex),"tabulated", numrow*numcol);
      }
    }
  else if (strcmp(marker,"mark_size") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetMarkSize((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"mark_foreground") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetMarkForegroundToDisplay((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"mark_background") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetMarkBackgroundToDisplay((sciPointObj *) pobj);
    }
  else if (strcmp(marker,"bar_layout") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	{   
	  numrow   = 1;
	  numcol   = 7;

	  if(pPOLYLINE_FEATURE (pobj)->bar_layout == 0)
	    { /* 0 grouped; 1 stacked */
	      
	      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	      strncpy(cstk(outindex),"grouped", numrow*numcol);
	    }
	  else
	    {
	      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	      strncpy(cstk(outindex),"stacked", numrow*numcol);
	    }
	}
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    }
  else if (strcmp(marker,"bar_width") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	*stk(outindex) = pPOLYLINE_FEATURE (pobj)->bar_width;
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    }
  else if (strcmp(marker,"x_shift") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	{
	  sciPolyline *  ppolyline = pPOLYLINE_FEATURE (pobj);
	  
	  if(ppolyline->x_shift == (double *) NULL)
	    {
	      numrow = numcol = 0;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	    }
	  else
	    {
	      numrow   = 1;numcol   = ppolyline->n1;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	      
	      for (i=0;i<numcol;i++)
		stk(outindex)[i] = ppolyline->x_shift[i];
	    }
	}
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    } 
  else if (strcmp(marker,"y_shift") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	{
	  sciPolyline *  ppolyline = pPOLYLINE_FEATURE (pobj);
	  
	  if(ppolyline->y_shift == (double *) NULL)
	    {
	      numrow = numcol = 0;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	    }
	  else
	    {
	      numrow   = 1;numcol   = ppolyline->n1;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	      
	      for (i=0;i<numcol;i++)
		stk(outindex)[i] = ppolyline->y_shift[i];
	    }
	}
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    } 
  else if (strcmp(marker,"z_shift") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	{
	  sciPolyline *  ppolyline = pPOLYLINE_FEATURE (pobj);
	  
	  if(ppolyline->z_shift == (double *) NULL)
	    {
	      numrow = numcol = 0;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	    }
	  else
	    {
	      numrow   = 1;numcol   = ppolyline->n1;
	      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
	      
	      for (i=0;i<numcol;i++)
		stk(outindex)[i] = ppolyline->z_shift[i];
	    }
	}
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    } 
  else if (strcmp(marker,"polyline_style") == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
  if (sciGetEntityType (pobj) == SCI_POLYLINE)
    *stk(outindex) = pPOLYLINE_FEATURE (pobj)->plot;
  else
    { strcpy(error_message,"Unknown polyline property"); return -1;}
    } 
  
  /****************************************************/
  else if (strcmp(marker,"font_size") == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontDeciWidth((sciPointObj *)pobj)/100;
    }
  else if (strcmp(marker,"font_angle") == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = (sciGetFontOrientation((sciPointObj *)pobj))/10;
    }
  else if (strcmp(marker,"font_foreground") == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontForegroundToDisplay((sciPointObj *)pobj);
    }
  else if (strcmp(marker,"font_color") == 0)	{ /* F.Leray 09.04.04 : Added for FIGURE and SUBWIN objects */
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->textcolor;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      *stk(outindex) = sciGetFontForegroundToDisplay(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"font_color property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"font_style") == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontStyle((sciPointObj *)pobj);
    }
  else if (strcmp(marker,"font_name") == 0)
    {
      numrow = 1;
      numcol = sciGetFontNameLength((sciPointObj *)pobj);
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetFontName((sciPointObj *)pobj), numrow*numcol);
    }
  else if (strcmp(marker,"text_box_mode") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT) {
	numrow = 1;
	if (pTEXT_FEATURE (pobj)->fill == -1) {
	  numcol = 3;
	  CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	  strncpy(cstk(outindex),"off", numcol);}
	else if (pTEXT_FEATURE (pobj)->fill == 0) {
	  numcol = 8;
	  CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	  strncpy(cstk(outindex),"centered", numcol);}
	else {
	  numcol = 6;
	  CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	  strncpy(cstk(outindex),"filled", numcol);}
      }
      else
	{strcpy(error_message,"text_box_mode property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"text_box") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT) {
	numrow = 1;
	numcol = 2;
	CreateVar(Rhs+1,"d", &numrow, &numcol, &outindex);
	*stk(outindex)= pTEXT_FEATURE (pobj)->wh[0];
	*stk(outindex+1)= pTEXT_FEATURE (pobj)->wh[1];
      }
      else
	{strcpy(error_message,"text_box property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"text") == 0)
    {
      numrow = 1;
      numcol = sciGetTextLength((sciPointObj *)pobj);
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      strncpy(cstk(outindex), sciGetText((sciPointObj *)pobj), numrow*numcol);
    }

  else if (strcmp(marker,"auto_clear") == 0)
    {
      if(   sciGetEntityType(pobj) != SCI_SUBWIN 
         && sciGetEntityType(pobj) != SCI_FIGURE )
       {strcpy(error_message,"Entity type not yet taken into account for this property");return -1;}
	
      if (!sciGetAddPlot((sciPointObj *)pobj)) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {	
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off",numrow*numcol);
      }
    }
  else if (strcmp(marker,"auto_scale") == 0)
    {
      if ( sciGetAutoScale((sciPointObj *)pobj)) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off",numrow*numcol);
      }
    }
  else if ((strcmp(marker,"zoom_box") == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN))
    {
      if (!sciGetZooming((sciPointObj *)pobj))
	{
	  numrow=0; numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	}
      else
	{
	  numrow=1;numcol=4;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++)
	    stk(outindex)[i] =  pSUBWIN_FEATURE(pobj)->ZRect[i];			
	} 
    }
  else if ((strcmp(marker,"zoom_state") == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN))
    {
      if (sciGetZooming((sciPointObj *)pobj)) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol); 
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"clip_box") == 0) 
    {
      if (sciGetIsClipping ((sciPointObj *) pobj) > 0)
	{
	  double *cliparea = sciGetClipping(pobj);
	  numrow=1; numcol=4;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++)
	    stk(outindex)[i] =  cliparea[i];
	}
      else
	{ 
	  numrow=0;numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	}
    }
  else if (strcmp(marker,"clip_state") == 0) 
    {
      if (sciGetIsClipping (pobj) == 0) {
	numrow   = 1;
	numcol   = 7;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"clipgrf", numrow*numcol); 
      }
      else if (sciGetIsClipping (pobj) > 0) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);	
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);    
      }
    }
  else if (strcmp(marker,"data") == 0)
    {
      tab = sciGetPoint ( pobj, &numrow, &numcol ) ;

      /*  if(sciGetEntityType (pobj) != SCI_SURFACE){ */
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      k=0;
      for (j=0;j < numcol*numrow;j++)
      {
	stk(outindex)[j] = tab[j];
      }

      if ( tab != NULL )
      {
        FREE(tab)  ;
        tab = NULL ;
      }

    }

  /**************** callback *********************/
  else if (strcmp(marker,"callbackmevent") == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i", &numrow, &numcol, &outindex);
      istk(outindex)[0] = sciGetCallbackMouseEvent((sciPointObj *)pobj);
    }
  else if (strcmp(marker,"callback") == 0)
    {
      numrow = 1;
      numcol = sciGetCallbackLen((sciPointObj *)pobj);
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      strncpy(cstk(outindex), sciGetCallback((sciPointObj *)pobj), numrow*numcol);
    }



  /**************************** AXES *************/
  else if (strcmp(marker,"x_label") == 0) /* we send back a handle on the x_label object */
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"x_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	hstk(outindex)[0] = sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_x_label);

      }
    }
  else if (strcmp(marker,"y_label") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"y_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	hstk(outindex)[0] = sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_y_label);
      }
    }
  else if (strcmp(marker,"z_label") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"z_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	hstk(outindex)[0] = sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_z_label);
      }
    }
  else if (strcmp(marker,"title") == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"title property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	hstk(outindex)[0] = sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_title);
      }
    }
  else if (strcmp(marker,"log_flags") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow = 1;numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	*cstk(outindex)  = pSUBWIN_FEATURE (pobj)->logflags[0];
	*cstk(outindex+1)= pSUBWIN_FEATURE (pobj)->logflags[1];
	*cstk(outindex+2)= pSUBWIN_FEATURE (pobj)->logflags[2];

      }
      else 	
	{strcpy(error_message,"log_flag property undefined for this object");return -1;}

    }
  else if (strcmp(marker,"tics_direction") == 0)
    {
      numrow = 1;
      switch (pAXES_FEATURE (pobj)->dir)
	{
	case 'u': 
	  numcol =  3;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "top" , numrow*numcol);
	  break;
	case 'd': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "bottom" , numrow*numcol);
	  break;
	case 'r': 
	  numcol =  5;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "right" , numrow*numcol); 
	  break;
	case 'l': 
	  numcol =  4;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "left", numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error");return -1;
	  break;
	}
    }
  else if (strcmp(marker,"x_location") == 0) 
    {
      char loc;
      numrow = 1;
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	loc = pSUBWIN_FEATURE (pobj)->axes.xdir;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}

      switch (loc)
	{
	case 'u': 
	  numcol =  3;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "top" , numrow*numcol);
	  break;
	case 'd': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "bottom" , numrow*numcol);
	  break;
	case 'c': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "middle" , numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error"); return -1;
	  break;
	}
    }  
  else if (strcmp(marker,"y_location") == 0)
    {
      char loc;
      numrow = 1;
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	loc = pSUBWIN_FEATURE (pobj)->axes.ydir;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}

      switch (loc)
	{
	case 'l': 
	  numcol =  4;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "left" , numrow*numcol);
	  break;
	case 'r': 
	  numcol =  5;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "right" , numrow*numcol);
	  break;
	case 'c': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "middle" , numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error"); return -1;
	  break;
	}
    } 
  else if (strcmp(marker,"tight_limits") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if (pSUBWIN_FEATURE (pobj)->tight_limits == 1) {
	  numrow   = 1;
	  numcol   = 2;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"on", numrow*numcol);
	}
	else {
	  numrow   = 1;
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"off", numrow*numcol);      
	}
      }
      else
	{strcpy(error_message,"tight_limits property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"closed") == 0) 
    { 
      if(sciGetEntityType(pobj) != SCI_POLYLINE)
	{strcpy(error_message,"closed property does not exist for this handle");return -1;}

      if (pPOLYLINE_FEATURE(pobj)->closed == 1) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"auto_position") == 0) 
    { 
      if(sciGetEntityType(pobj) != SCI_LABEL)
	{strcpy(error_message,"auto_position does not exist for this handle");return -1;}

      if (pLABEL_FEATURE(pobj)->auto_position == TRUE) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"auto_rotation") == 0) 
    { 
      if(sciGetEntityType(pobj) != SCI_LABEL)
	{strcpy(error_message,"auto_rotation does not exist for this handle");return -1;}

      if (pLABEL_FEATURE(pobj)->auto_rotation == TRUE) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol);
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);
      }
    }
  else if (strcmp(marker,"position") == 0) 
    {
      	  if (sciGetEntityType(pobj) == SCI_UIMENU)
		  {
			  numrow = 1;
			  numcol = 1;
			  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
			  stk(outindex)[0]=pUIMENU_FEATURE(pobj)->MenuPosition;
		  }
		  else if (sciGetEntityType(pobj) == SCI_LABEL)
		  {
			  numrow = 1;
			  numcol = 2;
			  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
			  sciGetPosition(pobj,&stk(outindex)[0],&stk(outindex)[1]);
		  }
		  else
		  {
			  strcpy(error_message,"position does not exist for this handle");
			  return -1;
		  }
	  
		  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);

		  

      
      

      /*       /\* switch to manual mode for label position *\/ */
      /*       pLABEL_FEATURE(pobj)->auto_position = FALSE; */
    }
  else if (strcmp(marker,"auto_ticks") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	char ** foo = (char **) NULL;
	int i;

	numrow   = 1;numcol   = 3;
	if((foo=malloc(numcol*(sizeof(char *))))==NULL){
	  strcpy(error_message,"No memory left for allocating temporary auto_ticks");return -1;}

	for(i=0;i<numcol;i++)
	  if( pSUBWIN_FEATURE (pobj)->axes.auto_ticks[i] == TRUE)
	    {
	      if((foo[i]=malloc(3*(sizeof(char))))==NULL){
		strcpy(error_message,"No memory left for allocating temporary auto_ticks");return -1;}
	      strcpy(foo[i],"on");
	    }
	  else
	    {
	      if((foo[i]=malloc(4*(sizeof(char))))==NULL){
		strcpy(error_message,"No memory left for allocating temporary auto_ticks");return -1;}
	      strcpy(foo[i],"off");
	    }    

	CreateVarFromPtr(Rhs+1,"S",&numrow,&numcol,foo);

        /* free the foo */
        for ( i = 0 ; i < numcol ; i++ )
        {
          free( foo[i] ) ;
        }
        free( foo ) ;

      }
      else
	{strcpy(error_message,"auto_ticks property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"axes_reverse") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	char ** foo = (char **) NULL;
	int i;

	numrow   = 1;numcol   = 3;
	if((foo=malloc(numcol*(sizeof(char *))))==NULL){
	  strcpy(error_message,"No memory left for allocating temporary reverse");return -1;}

	for(i=0;i<numcol;i++)
	  if( pSUBWIN_FEATURE (pobj)->axes.reverse[i] == TRUE)
	    {
	      if((foo[i]=malloc(3*(sizeof(char))))==NULL){
		strcpy(error_message,"No memory left for allocating temporary reverse");return -1;}
	      strcpy(foo[i],"on");
	    }
	  else
	    {
	      if((foo[i]=malloc(4*(sizeof(char))))==NULL){
		strcpy(error_message,"No memory left for allocating temporary reverse");return -1;}
	      strcpy(foo[i],"off");
	    }    

	CreateVarFromPtr(Rhs+1,"S",&numrow,&numcol,foo);

        /* free the foo */
        for ( i = 0 ; i < numcol ; i++ )
        {
          free( foo[i] ) ;
        }
        free( foo ) ;

      }
      else
	{strcpy(error_message,"reverse property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"view") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow   = 1;numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	if (pSUBWIN_FEATURE (pobj)->is3d)
	  strncpy(cstk(outindex),"3d", numrow*numcol); 
	else 
	  strncpy(cstk(outindex),"2d", numrow*numcol);      
      }
      else
	{strcpy(error_message,"view property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"axes_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow   = 1;numcol   = 4;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for (i=0;i<4;i++) {
	stk(outindex)[i] = pSUBWIN_FEATURE (pobj)->WRect[i];
      }
    }
    else
      {strcpy(error_message,"axes_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"data_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      /**DJ.Abdemouche 2003**/
      numrow   = 2;
      numcol=(pSUBWIN_FEATURE (pobj)->is3d)? 3 : 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      stk(outindex)[0] = pSUBWIN_FEATURE (pobj)->SRect[0];
      stk(outindex)[1] = pSUBWIN_FEATURE (pobj)->SRect[1];
      stk(outindex)[2] = pSUBWIN_FEATURE (pobj)->SRect[2];
      stk(outindex)[3] = pSUBWIN_FEATURE (pobj)->SRect[3];
      if (pSUBWIN_FEATURE (pobj)->is3d)
	{
	  stk(outindex)[4] = pSUBWIN_FEATURE (pobj)->SRect[4];
	  stk(outindex)[5] = pSUBWIN_FEATURE (pobj)->SRect[5];	
	}
    }
    else if (sciGetEntityType (pobj) == SCI_SURFACE) { /* used for what ? F.Leray 20.04.05 */
      numrow   = 3;numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for (i=0;i<numcol*numrow;i++) {
	stk(outindex)[i] = pSURFACE_FEATURE (pobj)->ebox[i];
      }
    }
    else
      {strcpy(error_message,"data_bounds property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"margins") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      /**SS  2004**/
      numrow   = 1;
      numcol=4;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      stk(outindex)[0] = pSUBWIN_FEATURE (pobj)->ARect[0];
      stk(outindex)[1] = pSUBWIN_FEATURE (pobj)->ARect[1];
      stk(outindex)[2] = pSUBWIN_FEATURE (pobj)->ARect[2];
      stk(outindex)[3] = pSUBWIN_FEATURE (pobj)->ARect[3];
    }
    else
      {strcpy(error_message,"margins property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"tics_color") == 0) 
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if (sciGetEntityType (pobj) == SCI_AXES)
	*stk(outindex) = pAXES_FEATURE (pobj)->ticscolor;
      else if (sciGetEntityType (pobj) == SCI_SUBWIN){
	sciprint("Warning: tics_color use is deprecated and no more taken into account, use foreground property to edit Axes color\n");
	*stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.ticscolor;
      }
      else
	{strcpy(error_message,"tics_color property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"tics_style") == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), &pAXES_FEATURE (pobj)->tics , numrow*numcol);
    }
  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */
  else if ((strcmp(marker,"sub_tics") == 0) || (strcmp(marker,"sub_ticks") == 0))
    {
      numrow   = 1;
      /*numcol   = (sciGetEntityType (pobj) == SCI_AXES) ? 1 : 2 ;*/
      
      if ( sciGetEntityType (pobj) == SCI_AXES )
      {
        numcol = 1 ;
        CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pAXES_FEATURE (pobj)->subint;
      }
      else if ( sciGetEntityType (pobj) == SCI_SUBWIN )
      {
        sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
        numcol = (ppSubWin->is3d)? 3 : 2;
        CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
        for ( i = 0 ; i < numcol ; i++ )
        {
          stk(outindex)[i] = ppSubWin->axes.nbsubtics[i] - 1 ;
        }
      }
      else
      {
        strcpy(error_message,"sub_ticks property does not exist for this handle");
        return -1 ;
      }
    }
  else if (strcmp(marker,"tics_segment") == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	if (pAXES_FEATURE (pobj)->seg==1) {
	  numrow   = 1;
	  numcol   = 2;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"on", numrow*numcol);
	}
	else {	
	  numrow   = 1;
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"off", numrow*numcol);
	}
      }
      else
	{strcpy(error_message,"tics_segment property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"labels_font_size") == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->fontsize;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.fontsize;*/
      *stk(outindex) = sciGetFontDeciWidth(pobj)/100;} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_size property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_color") == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->textcolor;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.textcolor;*/
      *stk(outindex) = sciGetFontForegroundToDisplay(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_color property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_style") == 0)	{ /* Adding F.Leray 09.04.04 */
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){ /* F.Leray 09.04.04: For the moment sciAxes have no font_style property*/
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.textcolor;*/
      *stk(outindex) = sciGetFontStyle(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_style property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"format_n") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES) {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),pAXES_FEATURE (pobj)->format, numrow*numcol);
    }
    else
      {strcpy(error_message,"format_n property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"xtics_coord") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow=1;
	numcol= pAXES_FEATURE (pobj)->nx;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol;i++)
	  stk(outindex)[i] = pAXES_FEATURE (pobj)->vx[i];
      }
      else
	{strcpy(error_message,"xtics_coord property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"ytics_coord") == 0)
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow=1;
	numcol= pAXES_FEATURE (pobj)->ny;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol;i++)
	  stk(outindex)[i] = pAXES_FEATURE (pobj)->vy[i];
      }
      else
	{strcpy(error_message,"ytics_coord property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"tics_labels") == 0)
    {
      char **foo = (char **) NULL;
      int i;
      int N;
      if (sciGetEntityType (pobj) == SCI_AXES) 
	{
	  numrow=1;
	  numcol= Max(pAXES_FEATURE (pobj)->nx,pAXES_FEATURE (pobj)->ny);
	  str = pAXES_FEATURE (pobj)->str;
	  if (str==NULL){

	    /* 	    for (i=0;i<numcol;i++){ */
	    if(pAXES_FEATURE (pobj)->format==NULL)
	      {
		/* we need to compute a c_format */
		char c_format[5];
		double *vector = NULL;

		if(ComputeXIntervals(pobj,pAXES_FEATURE (pobj)->tics,&vector,&N,1) != 0){
		  Scierror(999,"Error: Bad size in tics_coord ; you must first increase the size of the tics_coord\n");
		  return 0;
		}

		ComputeC_format(pobj,c_format);

		if((foo=malloc(N*(sizeof(char *))))==NULL){
		  strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
		for(i=0;i<N;i++){
		  if((foo[i]=malloc(256*(sizeof(char)+1)))==NULL){
		    strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
		}

		for(i=0;i<N;i++){
		  if(pAXES_FEATURE (pobj)->nx<numcol) 
		    sprintf(foo[i],c_format,vector[i]);
		  else
		    sprintf(foo[i],c_format,vector[i]);
		}
		FREE(vector); vector = (double *) NULL;
	      }
	    else
	      {
		double *vector = NULL;

		if(ComputeXIntervals(pobj,pAXES_FEATURE (pobj)->tics,&vector,&N,1) != 0){
		  Scierror(999,"Error: Bad size in tics_coord ; you must first increase the size of the tics_coord\n");
		  return 0;
		}

		if((foo=malloc(N*(sizeof(char *))))==NULL){
		  strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
		for(i=0;i<N;i++){
		  if((foo[i]=malloc(256*(sizeof(char)+1)))==NULL){
		    strcpy(error_message,"No memory left for allocating temporary tics_labels");return -1;}
		}

		for(i=0;i<N;i++){
		  if(pAXES_FEATURE (pobj)->nx<numcol) 
		    sprintf(foo[i],pAXES_FEATURE (pobj)->format,vector[i]);
		  else
		    sprintf(foo[i],pAXES_FEATURE (pobj)->format,vector[i]);
		}
		FREE(vector); vector = (double *) NULL;
	      }
	    /* 	    } */

	    /* I recompute the nb_tics_labels */
	    pAXES_FEATURE (pobj)->nb_tics_labels = N;

	    CreateVarFromPtr(Rhs+1,"S",&numrow,&N,foo);

	    if(foo != NULL)
	      for(i=0;i<N;i++) { FREE(foo[i]); foo[i] = NULL;}
	    FREE(foo); foo = NULL;
	  }
	  else{ /* str has been previously set once */
	    N = pAXES_FEATURE (pobj)->nb_tics_labels;
	    CreateVarFromPtr(Rhs+1,"S",&numrow,&N,str);
	  }
	}
      else
	{strcpy(error_message,"tics_labels property does not exist for this handle");return -1;}
    }
  else if ((strcmp(marker,"box") == 0)) {
    if (sciGetIsBoxed(pobj)) {
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),"on", numrow*numcol); 
    }
    else {
      numrow   = 1;
      numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),"off", numrow*numcol);  
    }
  }
  /**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"grid") == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) 
	{
	  numrow   = 1; 
	  numcol   = (pSUBWIN_FEATURE (pobj)->is3d)? 3 : 2;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++) 
	    {stk(outindex)[i] = pSUBWIN_FEATURE (pobj)->grid[i];}
	}
      else
	{strcpy(error_message,"grid property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"axes_visible") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      char ** foo = (char **) NULL;
      int i;

      numrow   = 1;numcol   = 3;
      if((foo=malloc(numcol*(sizeof(char *))))==NULL){
	strcpy(error_message,"No memory left for allocating temporary axes_visible");return -1;}
      for(i=0;i<numcol;i++)
	if( pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] == TRUE)
	  {
	    if((foo[i]=malloc(3*(sizeof(char))))==NULL){
	      strcpy(error_message,"No memory left for allocating temporary axes_visible");return -1;}
	    strcpy(foo[i],"on");
	  }
	else
	  {
	    if((foo[i]=malloc(4*(sizeof(char))))==NULL){
	      strcpy(error_message,"No memory left for allocating temporary axes_visible");return -1;}
	    strcpy(foo[i],"off");
	  }    

      CreateVarFromPtr(Rhs+1,"S",&numrow,&numcol,foo);

      /* free the foo */
      for ( i = 0 ; i < numcol ; i++ )
      {
        free( foo[i] ) ;
      }
      free( foo ) ;

    }
    else
      {strcpy(error_message,"axes_visible property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"hiddencolor") == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow   = 1;numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSUBWIN_FEATURE (pobj)->hiddencolor;
      }
      else if (sciGetEntityType (pobj) == SCI_SURFACE) {
	numrow   = 1;numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSURFACE_FEATURE (pobj)->hiddencolor;
      }
      else{
	strcpy(error_message,"hiddencolor property does not exist for this handle");
	return -1;
      }
    }
  /**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"isoview") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      if (pSUBWIN_FEATURE (pobj)->isoview) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol); 
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);  
      }
    }
    else
      {strcpy(error_message,"isoview property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"cube_scaling") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      if (pSUBWIN_FEATURE (pobj)->cube_scaling) {
	numrow   = 1;
	numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"on", numrow*numcol); 
      }
      else {
	numrow   = 1;
	numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	strncpy(cstk(outindex),"off", numrow*numcol);  
      }
    }
    else
      {strcpy(error_message,"cube_scaling property does not exist for this handle");return -1;}
  }
  /**************** SEGS  *********************/
  else if (strcmp(marker,"arrow_size") == 0)
    {    
      if (sciGetEntityType (pobj) == SCI_SEGS) {
	numrow = 1;numcol = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSEGS_FEATURE (pobj)->arrowsize ;
      }
      else
	{strcpy(error_message,"arrow_size property does not exist for this handle");return -1;}
    } 
  else if ((strcmp(marker,"segs_color") == 0) && (sciGetEntityType (pobj) == SCI_SEGS))
    {
      if (pSEGS_FEATURE (pobj)->ptype == 0)
	{
	  numrow = 1;numcol = (pSEGS_FEATURE (pobj)->Nbr1)/2;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++) {
	    stk(outindex)[i] = pSEGS_FEATURE (pobj)->pstyle[i] ;
	  }
	}
    }  
  else if ((strcmp(marker,"colored") == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){
    if (pSEGS_FEATURE (pobj)->ptype != 0)
      { 
	if (pSEGS_FEATURE (pobj)->typeofchamp == 1) {
	  numrow   = 1;
	  numcol   = 2;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"on", numrow*numcol); 
	}
	else {	
	  numrow   = 1;
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"off", numrow*numcol); 
	}
      }
    else
      {strcpy(error_message,"colored property does not exist for Segs");return -1;} 
  }
  /**************** Matplot Grayplot *********************/
  else if (strcmp(marker,"data_mapping") == 0) {
    if (sciGetEntityType (pobj) == SCI_GRAYPLOT) {
      numrow = 1;numcol = 6;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strcpy(cstk(outindex),pGRAYPLOT_FEATURE (pobj)->datamapping);
    }
    else
      {strcpy(error_message,"data_mapping property does not exist for this handle");return -1;}
  } 
  /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0) {
    /* DJ.A 2003 */
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow = 1;numcol = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex)=pSUBWIN_FEATURE (pobj)->alpha;
      *stk(outindex+1)=pSUBWIN_FEATURE (pobj)->theta;
    }
    else
      {strcpy(error_message,"rotation_angle property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"color_mode") == 0) {/*DJ.A merge*/
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = pSURFACE_FEATURE (pobj)->flag[0];
    }
    else
      {strcpy(error_message,"color_mode property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"color_flag") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      stk(outindex)[0] = pSURFACE_FEATURE (pobj)->flagcolor;
    }
    else
      {strcpy(error_message,"color_flag property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"cdata_mapping") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_FAC3D) {
	numrow   = 1;numcol   = 6;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	if(pSURFACE_FEATURE (pobj)->cdatamapping == 0) /* scaled mode */
	  strncpy(cstk(outindex),"scaled", numrow*numcol); 
	else if(pSURFACE_FEATURE (pobj)->cdatamapping == 1) /* direct mode */
	  strncpy(cstk(outindex),"direct", numrow*numcol); 
      }
      else
      {
        strcpy(error_message,"cdata_mapping property only exists for Fac3d surfaces.");
        return -1;
      } 
    }
    else
      {strcpy(error_message,"cdata_mapping property does not exist for this handle");return -1;}
  }

  else if (strcmp(marker,"surface_color") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) { 
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PARAM3D1) {
	numrow=1;
	numcol=pSURFACE_FEATURE (pobj)->dimzy;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol*numrow;i++) 
	  stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D) {
	if (pSURFACE_FEATURE (pobj)->flagcolor==2 || pSURFACE_FEATURE (pobj)->flagcolor==4) {
	  numrow=1;
	  numcol=pSURFACE_FEATURE (pobj)->dimzy;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol*numrow;i++) 
	    stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
	}
	else if  (pSURFACE_FEATURE (pobj)->flagcolor==3) {
	  numrow=pSURFACE_FEATURE (pobj)->dimzx;
	  numcol=pSURFACE_FEATURE (pobj)->dimzy;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol*numrow;i++) 
	    stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
	}
	else {
	  numrow=0; numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	}

      }
    }
    else
      {strcpy(error_message,"surface_color property does not exist for this handle");return -1;}
  } 
  /*  ===================================== FEC ======================================== */
  else if (strcmp(marker,"triangles") == 0) {
    if (sciGetEntityType (pobj) == SCI_FEC) { 
      numrow=pFEC_FEATURE (pobj)->Ntr;
      numcol=5;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for (i=0;i<numcol*numrow;i++) 
	stk(outindex)[i] = (double)pFEC_FEATURE (pobj)->pnoeud[i];
    }
    else
      {strcpy(error_message,"triangles property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"z_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_FEC) { 
      numrow=1;
      numcol=2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for (i=0;i<2;i++) 
	stk(outindex)[i] = (double)pFEC_FEATURE (pobj)->zminmax[i];
    }
    else
      {strcpy(error_message,"z_bounds property does not exist for this handle");return -1;}

  }
  else if (strcmp(marker,"user_data") == 0) {
    /* user_data */
    int *size_ptr, data_size;
    int **user_data_ptr,*data_ptr;
    sciGetPointerToUserData (pobj,&user_data_ptr, &size_ptr);

    data_ptr=*user_data_ptr;
    data_size=0;
    if (*user_data_ptr==NULL||*size_ptr==0) {
      numrow=0;
      numcol=0;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    }
    else {
      data_size=(int)((*size_ptr+1)/2);
      SetWorkSize(Rhs+1,&data_size);
      data_ptr=GetData(Rhs+1);
      memcpy(data_ptr,*user_data_ptr,(*size_ptr)*sizeof (int));
    }
  }
  else if (strcmp(marker,"handle_visible") == 0)
  {
	  if (sciGetEntityType (pobj) == SCI_UIMENU)
	  {
		if (GetHandleVisibilityOnUimenu((sciPointObj *)pobj))
		{
		  numrow   = 1;
		  numcol   = 2;
		  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
		  strncpy(cstk(outindex),"on", numrow*numcol);
		}
		else 
		{
		  numrow   = 1;
		  numcol   = 3;
		  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
		  strncpy(cstk(outindex),"off", numrow*numcol);
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
		  char CallbackType[16];
		  switch (pUIMENU_FEATURE(pobj)->CallbackType)
		  {
			case -1:
				strcpy(CallbackType,"disable");
			break;
			case 0:
				strcpy(CallbackType,"string");
			break;
			case 1:
				strcpy(CallbackType,"C");
			break;
			case 2:
				strcpy(CallbackType,"internal");
			break;
			case 3:
				strcpy(CallbackType,"addmenu");
			break;
			default:
				strcpy(error_message,"unknow callbak type");
				return -1;
			break;
		  }

		  numrow   = 1;
		  numcol   = strlen(CallbackType);
		  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
		  strncpy(cstk(outindex),CallbackType, numrow*numcol);

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
		if (pUIMENU_FEATURE(pobj)->Enable == TRUE)
		 {
			 numrow   = 1;
			 numcol   = 2;
			 CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
			 strncpy(cstk(outindex),"on", numrow*numcol);
		 }
		 else 
		 {
			 numrow   = 1;
			 numcol   = 3;
			 CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
			 strncpy(cstk(outindex),"off", numrow*numcol);
		 }
	  }
	  else
	  {
		  strcpy(error_message,"menu_enable property does not exist for this handle");
		  return -1;
	  }
  }
  else 
    {sprintf(error_message,"Unknown  property %s",marker);return -1;}

  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now given as a tlist (like for surface and champ objects) */
int getgrayplotdata(sciPointObj *pobj)
{
  char *variable_tlist[] = {"grayplotdata","x","y","z"};
  int m_variable_tlist = 0;
  int n_variable_tlist = 0;
  /*int n_variable_tlist = 1; */

  int  numrow, numcol,l;
  /*   int nx,ny; */

  /* F.Leray debug*/
  sciGrayplot * ppgrayplot = pGRAYPLOT_FEATURE (pobj);

  m_variable_tlist = 1;
  n_variable_tlist = 4;

  /* Add 'variable' tlist items to stack */
  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);

  numrow = ppgrayplot->nx;
  numcol = 1;
  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &ppgrayplot->pvecx);

  numrow = ppgrayplot->ny;
  numcol = 1;
  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &ppgrayplot->pvecy);

  numrow = ppgrayplot->nx;
  numcol = ppgrayplot->ny;

  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &ppgrayplot->pvecz);

  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now given as a tlist (like for surface objects) */
int getchampdata(sciPointObj *pobj)
{
  char *variable_tlist[] = {"champdata","x","y","fx","fy"};
  int m_variable_tlist = 0;
  int n_variable_tlist = 0;
  /*int n_variable_tlist = 1; */

  int  numrow, numcol,l;

  /* F.Leray debug*/
  sciSegs * ppsegs = pSEGS_FEATURE (pobj);

  m_variable_tlist = 1;
  n_variable_tlist = 5;

  /* Add 'variable' tlist items to stack */
  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);

  numrow = ppsegs->Nbr1;
  numcol = 1;
  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &ppsegs->vx);

  numrow = ppsegs->Nbr2;
  numcol = 1;
  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &ppsegs->vy);

  numrow = ppsegs->Nbr1;
  numcol = ppsegs->Nbr2;
  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &ppsegs->vfx);

  /* same numrow and numcol */
  CreateListVarFromPtr(Rhs+1, 5, "d", &numrow,&numcol, &ppsegs->vfy);

  return 0;
}
/*-----------------------------------------------------------------------------------*/
int get3ddata(sciPointObj *pobj)
{
  char *variable_tlist_color[] = {"3d","x","y","z","color"};
  char *variable_tlist[] = {"3d","x","y","z"};
  int m_variable_tlist = 0;
  int n_variable_tlist = 0;
  /*int n_variable_tlist = 1; */

  int flag_c;
  /*int one = 1;
    int four = 4;*/
  int  numrow, numcol,l;

  /* F.Leray debug*/
  /*sciSurface * psurf = pSURFACE_FEATURE (pobj); */

  /* tests a faire sur presence et taille color */
  flag_c = pSURFACE_FEATURE (pobj)->m3n;
  if(flag_c !=0)
    {
      m_variable_tlist = 1;
      n_variable_tlist = 5;

      if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)
	{
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3*/
	  /*
	    CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	    CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	    CreateVarFromPtr(Rhs+1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
	  */

	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist_color);


	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3*/
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);

	  numrow = pSURFACE_FEATURE (pobj)->m3n;
	  numcol = pSURFACE_FEATURE (pobj)->n3n;

	  /* F.Leray 24.03.04 Replaced by: */
	  CreateListVarFromPtr(Rhs+1, 5, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->inputCMoV);

	}
      else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
	{
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist_color);

	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* but here m1!=m2!=m3 and n1!=n2!=n3*/
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);

	  numrow = pSURFACE_FEATURE (pobj)->m2;
	  numcol = pSURFACE_FEATURE (pobj)->n2;
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);

	  numrow = pSURFACE_FEATURE (pobj)->m3;
	  numcol = pSURFACE_FEATURE (pobj)->n3;
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);

	  numrow = pSURFACE_FEATURE (pobj)->m3n;
	  numcol = pSURFACE_FEATURE (pobj)->n3n;


	  /* F.Leray 24.03.04 Replaced by: */
	  CreateListVarFromPtr(Rhs+1, 5, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->inputCMoV);

	}
    }
  else /* no color provided in input*/
    {
      m_variable_tlist = 1;
      n_variable_tlist = 4;

      if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)
	{
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3 */
	  /*
	    CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	    CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	    CreateVarFromPtr(Rhs+1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
	  */

	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);

	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3 */
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	}
      else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
	{
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);

	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* but here m1!=m2!=m3 and n1!=n2!=n3 */
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);

	  numrow = pSURFACE_FEATURE (pobj)->m2;
	  numcol = pSURFACE_FEATURE (pobj)->n2;
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);

	  numrow = pSURFACE_FEATURE (pobj)->m3;
	  numcol = pSURFACE_FEATURE (pobj)->n3;
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	}
    }


  return 0;
}
/*-----------------------------------------------------------------------------------*/

int getticks(char * xyztick, sciPointObj* psubwin)
{
  int i,nbtics;
  char c_format[5]; 
  char **  ticklabel = (char**) NULL;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  double *tmp= NULL;

  /* x */
  if(strcmp(xyztick,"x_ticks")==0)
    {
      /* compute the c_format used for convert double to char (for labels) */
      ChooseGoodFormat(c_format,ppsubwin->logflags[0],ppsubwin->axes.xgrads,ppsubwin->axes.nxgrads);

      if(ppsubwin->axes.auto_ticks[0] == TRUE)
	{
	  nbtics = ppsubwin->axes.nxgrads;

	  if((ticklabel=(char **)MALLOC(nbtics*sizeof(char *)))==NULL){
	    sciprint("No more place for allocating ticklabel");
	    return -1;
	  }

	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[0],nbtics,ppsubwin->axes.xgrads);

	  for(i=0;i<nbtics;i++)
	    {  
	      char foo[100];

	      sprintf(foo,c_format,tmp[i]);

	      if((ticklabel[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
		sciprint("No more place for allocating ticklabel");
		return -1;
	      }

	      strcpy(ticklabel[i],foo);
	    }

	  /* construction de la tlist */

	  BuildTListForTicks(tmp,ticklabel, nbtics);
          
          /* free ticklabel */
          for ( i = 0 ; i < nbtics ; i++ )
          {
            FREE( ticklabel[i] ) ;
          }
	  FREE(ticklabel); ticklabel = (char **) NULL;
	  FREE(tmp); tmp = (double *) NULL;
	}
      else /* we display the x tics specified by the user*/
	{
	  nbtics = ppsubwin->axes.u_nxgrads;

	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[0],nbtics,ppsubwin->axes.u_xgrads);


	  BuildTListForTicks(tmp,ppsubwin->axes.u_xlabels, nbtics);

	  FREE(tmp); tmp = (double *) NULL;
	}
    }

  /* y */
  else if(strcmp(xyztick,"y_ticks")==0)
    {
      /* compute the c_format used for convert double to char (for labels) */
      ChooseGoodFormat(c_format,ppsubwin->logflags[1],ppsubwin->axes.ygrads,ppsubwin->axes.nygrads);

      if(ppsubwin->axes.auto_ticks[1] == TRUE)
	{
	  nbtics = ppsubwin->axes.nygrads;

	  if((ticklabel=(char **)MALLOC(nbtics*sizeof(char *)))==NULL){
	    sciprint("No more place for allocating ticklabel");
	    return -1;
	  }

	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[1],nbtics,ppsubwin->axes.ygrads);

	  for(i=0;i<nbtics;i++)
	    {  
	      char foo[100];

	      sprintf(foo,c_format,tmp[i]);

	      if((ticklabel[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
		sciprint("No more place for allocating ticklabel");
		return -1;
	      }

	      strcpy(ticklabel[i],foo);
	    }

	  /* construction de la tlist */
	  BuildTListForTicks(tmp,ticklabel, nbtics);

          /* free ticklabel */
          for ( i = 0 ; i < nbtics ; i++ )
          {
            FREE( ticklabel[i] ) ;
          }
	  FREE(ticklabel); ticklabel = (char **) NULL;
	  FREE(tmp); tmp = (double *) NULL;
	}
      else /* we display the y tics specified by the user*/
	{
	  nbtics = ppsubwin->axes.u_nygrads;

	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[1],nbtics,ppsubwin->axes.u_ygrads);

	  BuildTListForTicks(tmp,ppsubwin->axes.u_ylabels, nbtics);

	  FREE(tmp); tmp = (double *) NULL;
	}
    }

  /* z */
  else if(strcmp(xyztick,"z_ticks")==0)
    {
      /*     int test_tmp = (ppsubwin->axes.auto_ticks[2] == TRUE)?ppsubwin->axes.nzgrads:ppsubwin->axes.u_nzgrads; */

      /*       if(test_tmp == 0)  */
      /* 	{ */
      /* 	  int zero = 0; */
      /* 	  BuildTListForTicks(NULL,NULL, zero); */


      /* 	  return 0; */
      /* 	} */
      /* compute the c_format used for convert double to char (for labels) */

      ChooseGoodFormat(c_format,ppsubwin->logflags[2],ppsubwin->axes.zgrads,ppsubwin->axes.nzgrads);

      if(ppsubwin->axes.auto_ticks[2] == TRUE)
	{
	  nbtics = ppsubwin->axes.nzgrads;

	  if((ticklabel=(char **)MALLOC(nbtics*sizeof(char *)))==NULL){
	    sciprint("No more place for allocating ticklabel");
	    return -1;
	  }


	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[2],nbtics,ppsubwin->axes.zgrads);


	  for(i=0;i<nbtics;i++)
	    {  
	      char foo[100];

	      sprintf(foo,c_format,tmp[i]);

	      if((ticklabel[i]=(char *)MALLOC((strlen(foo)+1)*sizeof(char )))==NULL){
		sciprint("No more place for allocating ticklabel");
		return -1;
	      }

	      strcpy(ticklabel[i],foo);
	    }

	  /* construction de la tlist */
	  BuildTListForTicks(tmp,ticklabel, nbtics);
          
          /* free ticklabel */
          for ( i = 0 ; i < nbtics ; i++ )
          {
            FREE( ticklabel[i] ) ;
          }
	  FREE(ticklabel); ticklabel = (char **) NULL;
	  FREE(tmp); tmp = (double *) NULL;
	}
      else /* we display the z tics specified by the user*/
	{
	  nbtics = ppsubwin->axes.u_nzgrads;

	  tmp = ReBuildTicksLog2Lin(ppsubwin->logflags[2],nbtics,ppsubwin->axes.u_zgrads);


	  BuildTListForTicks(tmp,ppsubwin->axes.u_zlabels, nbtics);

	  FREE(tmp); tmp = (double *) NULL;
	}
    }
  else
    {
      sciprint("Impossible case xyztick must be equal to x_, y_ or z_tick");
      return -1;
    }

  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* compute the c_format used for convert double to char (for labels) */
int ChooseGoodFormat(char * c_format,char logflag, double *_grads,int n_grads)
{
  int last_index = n_grads - 1;

  if(logflag == 'l')
    {
      ChoixFormatE(c_format,
		   exp10(_grads[0]),
		   exp10(_grads[last_index]),
		   (( exp10(_grads[last_index]))-( exp10(_grads[0])))/(last_index));
    }
  else
    {
      ChoixFormatE(c_format,
		   _grads[0],
		   _grads[last_index],
		   ((_grads[last_index])-(_grads[0]))/(last_index)); /* Adding F.Leray 06.05.04 */
    }

  return 0;

}
/*-----------------------------------------------------------------------------------*/
double * ReBuildTicksLog2Lin(char logflag, int nbtics, double *grads)
{
  int flag_limit = 0,i;
  double * tmp = NULL;

  if ( nbtics <= 0 || ( tmp = MALLOC( nbtics * sizeof(double) ) ) == NULL )
  {
    return NULL ;
  }

  if(logflag=='l')
    {
      for(i=0;i<nbtics;i++)
	{
	  flag_limit = 0;

	  /* 10^(-307) == -Inf */
	  flag_limit = flag_limit + ((grads[i])<-307)?-1:0;
	  /* 10^(+307) == +Inf */
	  flag_limit = flag_limit + ((grads[i])>307)?1:0;

	  if( flag_limit == -1)
	    {
	      tmp[i] = 0.;
	    }
	  else if ( flag_limit == 1)
	    {
	      tmp[i] = exp10(307);
	    }
	  else  if ( flag_limit == 0) /* general case */
	    {
	      tmp[i]=exp10(grads[i]);
	    }
	}
    }
  else
    {
      for(i=0;i<nbtics;i++)
	tmp[i] = grads[i];
    }

  return tmp;
}
/*-----------------------------------------------------------------------------------*/
int BuildTListForTicks(double * locations, char ** labels, int nbtics)
{
  int un=1, trois=3,l;
  char *variable_tlist[] = {"ticks","locations","labels"};

  CreateVar(Rhs+1,"t",&trois,&un,&l);
  CreateListVarFromPtr(Rhs+1, 1, "S", &un, &trois, variable_tlist);

  CreateListVarFromPtr(Rhs+1, 2, "d", &nbtics, &un, &locations);
  CreateListVarFromPtr(Rhs+1, 3, "S", &nbtics, &un, labels);

  return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL GetHandleVisibilityOnUimenu(sciPointObj * pobj)
{
	if (sciGetEntityType(pobj)!=SCI_UIMENU) return TRUE;

	return pUIMENU_FEATURE(pobj)->handle_visible;
}
/*-----------------------------------------------------------------------------------*/
