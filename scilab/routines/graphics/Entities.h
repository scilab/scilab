/*------------------------------------------------------------------------
 *    Graphic library 
 *    Copyright INRIA
 *    newGraph Library header
 *    Matthieu PHILIPPE, INRIA 2001-2002
 *    Djalel ABDEMOUCHE, INRIA 2002-2004
 --------------------------------------------------------------------------*/



#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#ifndef __GNUC__XXX
#include <commctrl.h>
#endif
#include <winuser.h>
#include "../wsci/wgnuplib.h"
#endif

/*en fait il n'y a essentiellement besion que de Math.h dans stack-c.h
  sauf pour les callback (il faudrait creer une fonction et l'appeler) */
#include "../stack-c.h" 


#ifndef WIN32
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE 
#define TRUE 1
#endif
#define PS_SOLID 0
#define HS_HORIZONTAL 0
#define PLANES 0
#define RASTERCAPS 0
#define RC_PALETTE 0
#define BITSPIXEL 0
#define FLOAT 0
#define TEXTMETRIC 0
#define GM_ADVANCED 0
#define MWT_IDENTITY 0
#define HORZRES 0
#define VERTRES 0
#define VERTSIZE 0
#define DEFAULT_CHARSET 0
#define HORZSIZE 0                                                     
#define MF_SEPARATOR 0
#define SW_MINIMIZE 0
#define SW_SHOWNORMAL 0
#define SB_VERT 0
#define SB_HORZ 0
#endif 
#define NUMCOLORS_SCI 32
/* Renvoi le pointeur sur la structure */

#define pFIGURE_FEATURE(pointobj)      ((sciFigure        *)pointobj->pfeatures)/** */
#define pSTATUSB_FEATURE(pointobj)     ((sciStatusBar     *)pointobj->pfeatures)/** */
#define pSUBWIN_FEATURE(pointobj)      ((sciSubWindow     *)pointobj->pfeatures)/** */
#define pTEXT_FEATURE(pointobj)        ((sciText          *)pointobj->pfeatures)/** */
#define pTITLE_FEATURE(pointobj)       ((sciTitle         *)pointobj->pfeatures)/** */
#define pLEGEND_FEATURE(pointobj)      ((sciLegend        *)pointobj->pfeatures)/** */
#define pPOLYLINE_FEATURE(pointobj)    ((sciPolyline      *)pointobj->pfeatures)/** */
#define pARC_FEATURE(pointobj)         ((sciArc           *)pointobj->pfeatures)/** */
#define pRECTANGLE_FEATURE(pointobj)   ((sciRectangle     *)pointobj->pfeatures)/** */
#define pMERGE_FEATURE(pointobj)       ((sciMerge         *)pointobj->pfeatures)/* DJ.A 30/12 */
#define pSURFACE_FEATURE(pointobj)     ((sciSurface       *)pointobj->pfeatures)/** */
#define pLIGHT_FEATURE(pointobj)       ((sciLightSource   *)pointobj->pfeatures)/** */ 
/*#define pAXIS_FEATURE(pointobj)        ((sciAxis          *)pointobj->pfeatures)*/ /** */
#define pAXES_FEATURE(pointobj)        ((sciAxes          *)pointobj->pfeatures)/** */
#define pGRAYPLOT_FEATURE(pointobj)    ((sciGrayplot      *)pointobj->pfeatures)/** */
#define pFEC_FEATURE(pointobj)         ((sciFec           *)pointobj->pfeatures)/** */
#define pPANNER_FEATURE(pointobj)      ((sciPanner        *)pointobj->pfeatures)/** */
#define pSBH_FEATURE(pointobj)         ((sciScrollBarHorz *)pointobj->pfeatures)/** */
#define pSBV_FEATURE(pointobj)         ((sciScrollBarVert *)pointobj->pfeatures)/** */
#define pLABELMENU_FEATURE(pointobj)   ((sciLabelMenu     *)pointobj->pfeatures)/** */
#define pMENUCONTEXT_FEATURE(pointobj) ((sciMenuContext   *)pointobj->pfeatures)/** */
#define pMENU_FEATURE(pointobj)        ((sciMenu          *)pointobj->pfeatures)/** */
#define pAGREG_FEATURE(pointobj)       ((sciAgreg         *)pointobj->pfeatures)/** */
#define pSEGS_FEATURE(pointobj)        ((sciSegs          *)pointobj->pfeatures)/** */

#define pLABEL_FEATURE(pointobj)       ((sciLabel         *)pointobj->pfeatures)/** */

#ifndef WIN32
typedef unsigned short HMENU;
typedef void *HFONT;                                         
typedef int BOOL;                                       
typedef unsigned long DWORD;
#endif

typedef struct tagPOINT2D
{
  double x;
  double y;
}
POINT2D;



typedef struct tagPOINT3D
{/** */
  double x;/** */
  double y;/** */
  double z;/** */
}/**used to specifie a 3D coordinates */
POINT3D;  

/**@name sciEntityType
 * Used to determine the type of the entity
 * @memo SCI_FIGURE,
 * @memo SCI_SUBWIN,
 * @memo SCI_TEXT,
 * @memo SCI_TITLE,
 * @memo SCI_LEGEND,
 * @memo SCI_ARC,
 * @memo SCI_POLYLINE,
 * @memo SCI_RECTANGLE,
 * @memo SCI_SURFACE,
 * @memo SCI_LIGHT,
 * @memo SCI_AXIS, 
 * @memo SCI_AXES, 
 * @memo SCI_SEGS 
 * @memo SCI_GRAYPLOT, 
 * @memo SCI_FEC,
 * @memo SCI_PANNER,
 * @memo SCI_SBH,
 * @memo SCI_SBV,
 * @memo SCI_MENU,
 * @memo SCI_MENUCONTEXT,
 * @memo SCI_STATUSB,
 */
typedef enum
{/**Entity type FIGURE*/
  SCI_FIGURE,		
  /**Entity type SUBWINDOW*/
  SCI_SUBWIN,		
  /**Entity type TEXT*/
  SCI_TEXT,			
  /**Entity type TITLE */
  SCI_TITLE,		
  /**Entity type LEGEND */
  SCI_LEGEND,		
  /**Entity type ARC */
  SCI_ARC,			
  /**Entity type POLYLINE*/
  SCI_POLYLINE,	    
  /**Entity type RECTANGLE*/
  SCI_RECTANGLE,    
  /**Entity type SURFACE*/
  SCI_SURFACE,
  /**Entity type MERGE*/  /* DJ.A 30/12 */
  SCI_MERGE,
  /**Entity type LIGHT*/
  SCI_LIGHT,		
  /**Entity type AXIS*/
  SCI_AXIS,	
  /**Entity type AXES*/
  SCI_AXES,	  
  /**Entity type SEGS*/
  SCI_SEGS,	
  /**Entity type GRAYPLOT*/
  SCI_GRAYPLOT, 
  /**Entity type FEC*/
  SCI_FEC,				
  /**Entity type PANNER*/
  SCI_PANNER,		
  /**Entity type HORIZONTALL SCROLL BAR */
  SCI_SBH,			
  /**Entity type VERTICALL SCROLL BAR*/
  SCI_SBV,			
  /**Entity type MENU*/
  SCI_MENU,			
  /**Entity type CONTEXT MENU*/
  SCI_MENUCONTEXT,	
  /**Entity type STATUS BAR*/
  SCI_STATUSB,	    
  /**Entity type AGREGATION */
  SCI_AGREG,			
  /**Entity type LABEL created by F.Leray 26.05.04 */
  SCI_LABEL
}
/**Struct of Entity type*/
sciEntityType;	


/**@name sciPointObj
 * Used to determine the feature and the type of the entity
 */
typedef struct
{/** */
  sciEntityType entitytype;
  /**  points to the characteristic of the structure (figure, axes...) */
  void *pfeatures;		
}/** */
sciPointObj;  


/**@name sciHandleTab
 * Used to determine handles associated with the entity
 */
typedef struct tagHandleTab
{/** point to the structure that uses this handle */
  sciPointObj *pointobj;	
  /** */
  long index;
  /** */
  struct tagHandleTab *pprev;
  /** */
  struct tagHandleTab *pnext;
}/** */
sciHandleTab; 

/**@name sciClipTab
 * Used to determine store clipping associated to the entity
 */
typedef struct 
{/** */
  int index;
  /** */
  double clip[4];
}/** */
sciClipTab;  


/**@name Sons
 * Used to determine the hierarchy
  sciPointObj *pointobj;
  sciSons     *pnext;
 */
typedef struct tagSons
{
	/** */
  struct tagSons *pprev;	  
  /**  is the pointer of next the son's structure      */
  sciPointObj *pointobj;	
  /** */
  struct tagSons *pnext;
}
/** */
sciSons;  


/**@name RelationShip
 * Used to determine the hierarchy
 */
typedef struct
{/** is the scilab handle of THIS */
  sciHandleTab *phandle;       
  /** points to the parent structures    */
  sciPointObj *pparent;	       
  /** points to the sons structures      */
  sciSons *psons;	       
  /** the last sciSons of the list (the first created!) */
  sciSons *plastsons;			
  /** points to the current son          */
  sciPointObj *pcurrentson;    
}/** */
sciRelationShip;  



/**@name GraphicContext
 * Used to know what are the contexte value attached with the Graphic area  
 * valeur de hachure (fillstyle)
 * HS_BDIAGONAL 45-degree downward left-to-right hatch 
 * HS_CROSS Horizontal and vertical crosshatch 
 * HS_DIAGCROSS 45-degree crosshatch 
 * HS_FDIAGONAL 45-degree upward left-to-right hatch 
 * HS_HORIZONTAL Horizontal hatch 
 * HS_VERTICAL Vertical hatch 
 * 
 * LineStyle:
 * PS_SOLID The pen is solid. 
 * PS_DASH The pen is dashed. This style is valid only when the pen width is one 
 *         or less in device units. 
 * PS_DOT The pen is dotted. This style is valid only when the pen width is one 
 *         or less in device units. 
 * PS_DASHDOT The pen has alternating dashes and dots. This style is valid only 
 *         when the pen width is one or less in device units. 
 * PS_DASHDOTDOT The pen has alternating dashes and double dots. This style is 
 *         valid only when the pen width is one or less in device units. 
 * PS_NULL The pen is invisible. 
 * PS_INSIDEFRAME The pen is solid. When this pen is used in any GDI drawing 
 *         function that takes a bounding rectangle, the dimensions of the 
 *         figure are shrunk so that it fits entirely in the bounding rectangle, 
 *         taking into account the width of the pen. This applies only to geometric pens. 
 */
typedef struct
{
  /** currently not used in this version */
  /** pointe sur un ID de la table des couleur Scilab */
  int backgroundcolor;	    
  /** pointe sur un ID de la table des couleur Scilab */
  int foregroundcolor;
  /** */
  int fillcolor;
  /** */
  int fillstyle;
  /** associeted to PEN or mark size */
  int linewidth;
  /** */
  int linestyle;
  /** */
  BOOL ismark;
  /** */
  int markstyle;
}/** */
sciGraphicContext;  


/*----------------------
  SCIFONT.H header file no more used
  ----------------------*/

/* Changing those lines, be carreful that
 the new attribut is update in the sciset and sciget routines
 */
/** */
#define SCI_DONT_CARE          0
/** */
#define SCI_ATTR_BOLD          1
/** */
#define SCI_ATTR_ITALIC        2
/** */
#define SCI_ATTR_UNDERLINE     4
/** */
#define SCI_ATTR_STRIKEOUT     8


/**@name sciFont
 * Used to know what are the contexte value attached with the Graphic area  
 */
typedef struct
{
  /** currently not used in this version */
  /* il faudrait prendre la font windows */
  /** pointe sur un ID de la table des couleur Scilab */
  int backgroundcolor;		
  /** pointe sur un ID de la table des couleur Scilab */
  int foregroundcolor;		
  /** pointe sur le nom de la fonte  */
  char *pfontname;		 
  /** renvoi la longueur de fontname */
  unsigned int fontnamelen;	
  /** */
  int fonttype; /* scilab font index 0 to 9 */
  /** Gets the width of the character in tenth of point */
  int fontdeciwidth;		
  /** this is coded in tenth of a degree */
  int textorientation;		
}/** */
sciFont;  


/**@name scigMode
 * Used to determine all the mode that can be used. Only some entity can sets this itself
 */
typedef struct
{ 
  /** Are new Plot added to the old plot on the graphics window        */
  BOOL addplot;	       
  /** The min max ranges of the the graphics window is given by users  */
  BOOL autoscaling;    
  /** Is zooming allowed                                               */
  BOOL zooming; 
  /** Is the graphic automatically resized to fill the graphics window */
  BOOL wresize;	    
  /** Is old style                                                     */
  BOOL oldstyle;
  /** drawing xor mode                                                         */
  int xormode;
  	       	       
  
}/** */
scigMode;  


/**@name Range
 * Structure used to specifie Range
 */
typedef struct
{/** */
  double xmin;
  /** */
  double xmax;
  /** */
  double ymin;
  /** */
  double ymax;
  /** */
  double zmin;
  /** */
  double zmax;
}/** */
sciRange;  


/**@name Figure
 * Structure used to specify Figure (different to XGC) 
 * @see 
 */
typedef struct
{/** will be integrated in the new structure      */
  struct BCG *pScilabXgc;	
  sciRelationShip relationship;
  scigMode gmode;
  sciGraphicContext graphiccontext; /* the only property used here is background */
  sciPointObj * originalsubwin0011;
  /** specifies the title for this window  */
  char name[80];	 
  /** specifies le length of the string name */
  int namelen;
  /** specifies the number of this window            */
  int number;			       
  /** specifies the dimension of this graphic        */
  int figuredimwidth;		       
  int figuredimheight;		       
  /* specifies the dimension of this figure         */
  int windowdimwidth;		       
  int windowdimheight;	
  /* specifies the dimension of this window         */
  /** */
  int inrootposx;
  int inrootposy;
  /** specifies the colr map                         */
  double *pcolormap;
  /** specifies if this window is iconified*/
  int numcolors;
  BOOL isiconified;		     
  /** specifies if this window is selected             */
  BOOL isselected; 
  int rotstyle;
  /** specifies if this object is visble             */
  BOOL visible;
  /** specifies the number of the selected son         */
  int numsubwinselected;
  /** specifies the current pixmap status         */
  int pixmap; /* DJ.A 30/12 */
  int wshow ;
  
  sciFont fontcontext; /* F.Leray 08.04.04 */
  
}/** */
sciFigure;  


/**@name Text
 * Structure used to specify Texte 
 */
typedef struct
{
  /** */
  sciRelationShip relationship;
  /** */
  sciFont fontcontext;
  /** */
  char *ptextstring;
  /** position in scilab window (not pixel) */
  double x;			    
  /** position in scilab window (not pixel) */
  double y;
  /***/
  double wh[2];
  BOOL fill;
  /** */
  double z; /**DJ.Abdemouche 2003**/
  unsigned int textlen;
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen; 
  int callbackevent; 
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
}/** */
sciText;  




/**@name sciLegendPlace
 * Enumeration used to specify the title place 
 */
typedef enum
{
  /** */
  SCI_LEGEND_OUTSIDE = -1,
  /** */
  SCI_LEGEND_IN_INSIDE = 0,
  /** */
  SCI_LEGEND_IN_UPPER_LEFT = 1,
  /** */
  SCI_LEGEND_IN_UPPER_RIGHT = 2,
  /** */
  SCI_LEGEND_IN_LOWER_LEFT = 3,
  /** */
  SCI_LEGEND_IN_LOWER_RIGHT = 4,
  /** */
  SCI_LEGEND_IN_SPECIFIED = 5
}  /** */
sciLegendPlace;



/**@name Legend
 * Structure used to specify Legend 
 */
typedef struct
{
  /** */
  int nblegends;
  /** */
  int *pstyle;
  /** */
  sciText text;
  /** */
  POINT2D pos;
  /** specifies the frame's width */
  int width;		     
  /** specifies the frame's height */
  int height;		     
   /** */
  BOOL isselected;
  /** specifies if the legend is surrounded */
  BOOL issurround;	     
  /* void    *associetedentity;  get the associeted entity by asking parents.relation.sons */
  /* the subwindow dimension is get by asking relation     */
  sciLegendPlace place;
  /** */
  sciSons *associatedentity;
  /** */
  sciPointObj **pptabofpointobj;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
}/** */
sciLegend;  


/**@name sciTitlePlace
 * Enumeration used to specify the title place 
 */
typedef enum
{
  /** */
  SCI_TITLE_IN_TOP = 0,
  /** */
  SCI_TITLE_IN_BOTTOM = 1,
}/** */
sciTitlePlace;	


/**@name Titre
 * Structure used to specify Texte 
 */
typedef struct
{
  /** */
  sciText text;
  /** absolut position in subindow*/
  POINT2D pos;	
  int ptype;
  /** up, down */
  sciTitlePlace titleplace; 
  /** */
  BOOL isselected;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
}/** */
sciTitle;  

/**@name Titre
 * Structure used to specify Labels like Title or classic labels
 */
typedef struct
{
  /* sciRelationShip relationship; */
  /** */
  sciText text;
  /** absolut position in subindow*/
  POINT2D pos;	
  int ptype;
  /** up, down */
  /*sciTitlePlace titleplace; */
  /** */
  BOOL isselected;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
}/** */
sciLabel;  


typedef struct 
{  
  int  ticscolor;
  /*  int  fontsize;
      int  textcolor;*/
  sciFont fontcontext;
  
  int  subint[3]; /* Dj.A 17/12/03 */
  int  rect;
  char xdir;   /**  xdir  = 'u' | 'd'  : gives the xy-axes positions **/ 
  char ydir;   /**  ydir  = 'r' | 'l' : gives the xy-axes positions **/ 
  double  xlim[4];  /* [xmin,xmax,ar,nint]           */
  double  ylim[4];  /* [ymin,ymax,ar,nint]           */ 
  double  zlim[4];  /* [zmin,zmax,ar,nint]         */  /**DJ.Abdemouche 2003**/
  double    limits[7]; /* = 1 set tight limits = 0 set axes auto shape */
  integer flag[3]; /* 3d options */
  
  /* F.Leray To keep the logic on aaint:*/
  integer aaint[4];
}
AXES; 

/**@name SubWindow
 * Structure used to specify SubWindow wich is a subplot
 */
typedef struct
{
  /** */
  sciRelationShip relationship;
  /** */
  scigMode gmode;
  /** */
  sciGraphicContext graphiccontext; 
  /** */
  /** specifies the title for this window  */
  char name[sizeof ("ScilabGraphic") + 4];	    
    /** */
  int namelen;
  /** specifies the number for this window                   */
  int number;			
  /** specifies if this subwindow is selected                */
  BOOL isselected;			 
  /** specifies the position in the parent figure            */
  int infigureposx;			 
  /** specifies the position in the parent figure            */
  int infigureposy;			 
  /** specifies the width of the subplot                     */
  int windimwidth;		       
  /** specifies the width of the subplot                     */
  int windimheight;		      
  /** specifies the limite of the plot                       */
  sciRange range;			 
  /** specifies the factor of the x zoom                     */
  int zoomx;				 
  /** specifies the factor of the y zoom                     */
  int zoomy;
  double SRect[6]; /* [xmin xmax ymin ymax zmin zmax] : Strict rect. coming from update_specification_bounds function or from a set(a,"data_bounds",[...]) */
  double FRect[6];
  double WRect[4]; /* subwin */
  double ARect[4]; /* margins*/
  int zoomy_kp;
  double FRect_kp[4];
  char logflags[2]; 
  int grid[3];
  BOOL isaxes;
  AXES axes;
  BOOL is3d;
  BOOL tight_limits;
  double theta_kp;
  double alpha_kp;	
  double theta;
  double alpha;	
  /** specifie the associated entity popup menu */
  sciPointObj *pPopMenu;    
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen;  
  int callbackevent; 
  /** specifies if this object is visble             */
  BOOL visible;  
  int isclip;
  /**DJ.Abdemouche 2003**/
  integer project[3];
  BOOL isoview; 
  int hiddencolor;
  int hiddenstate;
  BOOL facetmerge; 
  int with_leg; /* Adding F.Leray 07.05.04 */ /* for strflag[0] support : not needed today */
  BOOL cube_scaling; /* Matlab like view in 3D when one or two range is/are preferential */
  BOOL FirstPlot; /* An internal state used to indicated that high level functions must not use SRect*/

  /* F.Leray 25.04.05 Labels in sciSubWin*/
  sciPointObj * mon_title;
  sciPointObj * mon_x_label;
  sciPointObj * mon_y_label;
  sciPointObj * mon_z_label;


}/** */
sciSubWindow;  


/**@name Arc
 * Structure used to specify 
 * @author Matthieu PHILIPPE /MAJ D.ABDEMOUCHE
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
typedef struct
{
  /** */
  sciRelationShip relationship;
  /** */
  sciGraphicContext graphiccontext;
  /** */
  double x;
  /** */
  double y;
  /** */
  double width;
  /** */
  double height;
  /** begin at alpha1 */
  double alphabegin;		
  /** end at alpha2   */
  double alphaend;
  int fill;		
  /** */
  BOOL isselected;
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen;   
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;  
  int isclip; 
  double z; /**DJ.Abdemouche 2003**/
}  /** */
sciArc;


/**@name listPoints
 * Structure used to specify 
 */
typedef struct taglistPoints
{
  /** */
  double x;
  /** */
  double y;
  /** */
  struct taglistPoints *pnextpoints;
}  /** */
sciListPoints;


/**@name Polyline
 * Structure used to specify 
 * @author Matthieu PHILIPPE /MAJ D.ABDEMOUCHE
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  /* listPoints     points; */
  POINT2D *pvector;		/* vecteur de points redondant, for future developpement*/
  double *pvx;			/* vecteur des points x doublon avec pvector je les garde pour compatiblite*/
  double *pvy;			/* vecteur des points y doublon avec pvector*/
  double *pvz; /**DJ.Abdemouche 2003**/
  int n1;                               /** number of point */
  int n2;                             /** numbre of curve if Plot **/ 
  int closed;                      /** is it a closed polyline */
  int plot;                        /** is it simple poly or a plot (Plot2d /Plot2d1/.../Plot2d4) */
  double xmin;				/** xmin */
  double ymin;				/** ymin */
  double zmin;				/** ymin */
  double xmax;				/** xmax Adding  F.Leray  02.04.04 */
  double ymax;	                        /** ymax Adding  F.Leray  02.04.04 */
  double zmax;	                        /** ymax Adding  F.Leray  02.04.04 */
  double width;			/** width */
  double height;		/** height */
  BOOL isselected;
  char *callback;		/** specifies the text scilab code for the callback associated with this entity */
  int callbacklen;		/** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
}
sciPolyline;  /** */


/**@name Rectangle
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  double x;			   /** original */
  double y;			   /** original */
  double width;
  double height;
  int fillflag;
  int fillcolor;
  int str; 
  integer strwidth;
  integer strheight;
  double horzcurvature;		  /** to do rectangle with round corner */ /*F.Leray not implemented till now... 19.03.04 to see...*/
  double vertcurvature;		  /** to do rectangle with round corner */ /*F.Leray not implemented till now... 19.03.04 to see...*/
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
  double z;  /** rectangle */
}
sciRectangle;  /** */


typedef enum
{
	SCI_FAC3D,
	SCI_CONTOUR,
	SCI_PARAM3D,
	SCI_PARAM3D1,
	SCI_PLOT3D,
}
sciTypeOf3D;


/**@name Surface
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  double * pvecx;
  double * pvecy;
  double * pvecz;
  integer *zcol;
  integer *inputCMoV; /* Adding here in order to always have the input data*/
  /* We will determinate if it is a vector or matrix data with m3n and n3n values*/

  integer izcol;  
  integer dimzx;
  integer dimzy;

  /*F.Leray 12.03.04 Adding here to know the length of arrays pvecx, pvecy and pvecz*/

  int nc;
  int nx;
  int ny;
  int nz;
  int isfac;
  int m1,n1;
  int m2,n2;
  int m3,n3;
  int m3n,n3n;

  POINT2D *pproj;			/* projections on 2d */
  integer flag[3];
  double ebox[6];
  int flagcolor; /* this flag indicates the type of the color of how the facet have to be colored 
                        0: uniformed color
			1: facet's color are computed with z*/ /* in case of a simple plot...!!! F.Leray 19.03.04 */
  sciTypeOf3D typeof3d;
  int hiddencolor;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 

}
sciSurface;  /** */


/**@name merge DJ.A 30/12
 * Structure used to specify 
 */

typedef struct
{
  sciRelationShip relationship;
  long *from_entity; /* vector of handles on the facet or segment entity*/
  int *index_in_entity;/*index of facet or segment in its entity*/
  int N; /* number of facets or segments */
}
sciMerge;  /** */


/**@name LightSource
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  POINT3D org;
  BOOL switchon;		/* on or off */
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
}
sciLightSource;	 /** */


/**@name Axis
 * Structure used to specify axis feature 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  sciText text;
  char strflag[4];
  int strflaglen;
  double aaint[4];
  double minx;
  double miny;
  double minz;
  double maxx;
  double maxy;
  double maxz;
  int styledimension;		/* 2=2d 3=3d */
  int stylecrossing;
  double orgcrossing;
  char *plabelx;
  char *plabely;
  char *plabelz;
  BOOL manualscale;
  BOOL plotit;
  BOOL isselected;
  int grid;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip; /* Adding F.Leray 10.03.04*/
}
sciAxis;  /** */



/**@name Axes
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  char dir;   /** dir = 'r' | 'l' | 'u' | 'd' : gives the tics directions **/  
  char tics;  /** tics = 'v' (for vector) or 'r' (for range) or i **/
  POINT2D *vector;		/* vecteur de points redondant, for future developpement*/
  double *vx;  /** vx vector of size nx **/
  double *vy;  /** vy vector of size ny **/ 
  /**DJ.Abdemouche 2003**/
  double *vz; 
  int nx;
  int ny;
  /***/
  int nz;
  char **str ;  /** string vector **/
  int subint;  /** subints : number of sub intervals **/ 
  char *format; /** format for tick marks **/
  int fontsize;
  int textcolor;
  int ticscolor;
  char logscale;
  int seg;      /**0 or 1, flag which control the drawing of the segment associated to the axes **/
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;


}
sciAxes;  

/**@name Segs
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
  
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  double *vx;  /** vx vector of size Nbr **/ /*F.Leray 18.02.04 ...of size Nbr1 ? No depending on the type ptype*/
  double *vy;  /** vy vector of size Nbr **/ /*F.Leray 18.02.04 ...of size Nbr2 ? No depending on the type ptype*/
  double *vfx;
  double *vfy;
  integer Nbr1;   
  integer Nbr2;
  integer *pstyle;
  integer iflag;      /**0 or 1, flag which control the drawing of the segment  **/
  double arrowsize;  /*F.Leray units : hundreds (i.e. 100, 150,...)*/
  integer pcolored;
  double parfact;
  integer ptype;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
  double *vz; /**DJ.Abdemouche 2003**/
  double *vfz;

}
sciSegs; 
 



/**@name Grayplot
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  
  double *pvecx;  /** vx vector of size nx **/
  double *pvecy;  /** vy vector of size ny **/ 
  double *pvecz;  /** vz vector of size nx*ny **/  
  int nx; 
  int ny;
  int type;   /** 0 if a grayplot, 1if a matplot **/  
  char datamapping[7];
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */   
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible; 
}
sciGrayplot;  
/** */

/**@name Fec
 * Structure used to specify fec feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  
  double *pvecx;  
  double *pvecy;   
  double *pnoeud;
  double *pfun;    
  int Nnode; 
  int Ntr;  
  double *zminmax; 
  integer *colminmax;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */ 
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

}
sciFec;  /** */


/**@name Panner
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int posx;
  int posy;
  int width;
  int height;
  int totalheigth;		/* the height in the menu bar */
}
sciPanner;  /** */


/**@name ScrollBarHorz
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int poshorz;
  int totalheight;
}
sciScrollBarHorz;  /** */


/**@name ScrollBarVert
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int posvert;
  int totalwidth;
}
sciScrollBarVert;  /** */


/**@name LabelMenu
 * Structure used to specify a menu 
 */
typedef struct tagLabelMenu
{
  char *plabel;
  struct tagLabelMenu *pnextlabelmenu;
}
sciLabelMenu;  /** */



/**@name Menu
 * Structure used to specify a menu 
  */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  sciText text;
  sciLabelMenu *plabelmenu;
  HMENU  hPopMenu;			/** specifie the handle of the popup menu */
}
sciMenu /** */ , sciMenuContext;/** */


/**@name MenuContext
 * Structure used to specify popupmenu called with the right mouse button
 *
typedef struct {
  sciRelationShip    relationship;
  sciGraphicContext graphiccontext;
  sciText           text;
  sciLabelMenu      *plabelmenu;
} sciMenuContext ;
ATTENTION DECLARE SI DESSUS */

/**@name StatusBar
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  sciText text;
  char *ptext;
}
sciStatusBar;  /** */


/**@name Panner
 * Structure used to specify 
  */
typedef struct
{
  sciRelationShip relationship;
  double xmin;			   /** original xmin */
  double ymin;			   /** original ymin */
  double xmax;
  double ymax;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */ 
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

}
sciAgreg;  /** */





/** functions **/
extern void *sciGetPointerToFeature (sciPointObj * pobj);
/* Entity Type functions */
extern void sciSetEntityType (sciPointObj * pobj, sciEntityType value);
extern sciEntityType sciGetEntityType (sciPointObj * pobj);
extern char *sciGetCharEntityType (sciPointObj * pobj);
/* Handle functions */
extern void sciSetHandle (sciPointObj * pobj, sciHandleTab * pvalue);
extern int sciAddNewHandle (sciPointObj * pobj);
extern int sciDelHandle (sciPointObj * pobj);
extern long sciGetHandle (sciPointObj * pobj);
extern sciHandleTab *sciGetHandleTabPointer (sciPointObj * pobj);
extern sciPointObj *sciGetPointerFromHandle (long handle);

/* Graphic Context Functions */
extern sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj);
extern int sciInitGraphicContext (sciPointObj * pobj);
extern int sciInitFontContext (sciPointObj * pobj);
extern int sciSetColormap (sciPointObj * pobj, double *rgbmat,
			   integer m, integer n);
extern int sciSetNumColors (sciPointObj * pobj, int numcolors);
extern int sciGetNumColors (sciPointObj * pobj);
extern double *sciCloneColormap (sciPointObj * pobj);
extern int sciGetColormap (sciPointObj * pobj, double *rgbmat);
/*Adding F.Leray 31.03.04*/
extern int sciSetGoodIndex(sciPointObj * pobj, int colorindex);
extern int sciGetGoodIndex(sciPointObj * pobj, int colorindex);
extern void sciRecursiveUpdateBaW(sciPointObj *pobj, int old_m, int m);
extern int sciUpdateBaW (sciPointObj * pobj, int flag, int value);
extern int sciGetForegroundToDisplay (sciPointObj * pobj);
extern int sciGetBackgroundToDisplay (sciPointObj * pobj);
/*End Adding 31.03.04*/


extern int sciSetBackground (sciPointObj * pobj, int colorindex);
extern int sciGetBackground (sciPointObj * pobj);
extern int sciSetForeground (sciPointObj * pobj, int colorindex);
extern int sciGetForeground (sciPointObj * pobj);
extern int sciGetLineWidth (sciPointObj * pobj);
extern int sciSetLineWidth (sciPointObj * pobj, int linewidth);
extern int sciGetLineStyle (sciPointObj * pobj);
extern int sciSetLineStyle (sciPointObj * pobj, int linestyle);
extern BOOL sciGetIsMark (sciPointObj * pobj);
int extern sciSetIsMark (sciPointObj * pobj, BOOL ismark);
extern int sciGetMarkStyle (sciPointObj * pobj);
extern int sciSetMarkStyle (sciPointObj * pobj, int markstyle);
extern int sciGetFillStyle (sciPointObj * pobj);
extern int sciSetFillStyle (sciPointObj * pobj, int fillstyle);
extern int sciGetFillColor (sciPointObj * pobj);
extern int sciSetFillColor (sciPointObj * pobj, int fillcolor);
extern int sciGetFillFlag (sciPointObj * pobj);
extern int sciSetFillFlag (sciPointObj * pobj, int fillflag);

/* Text Functions */
extern sciFont *sciGetFontContext (sciPointObj * pobj);
extern int sciGetFontDeciWidth (sciPointObj * pobj);
extern int sciSetFontDeciWidth (sciPointObj * pobj, int fontdeciwidth);
extern int sciGetFontOrientation (sciPointObj * pobj);
extern int sciSetFontOrientation (sciPointObj * pobj, int textorientation);
extern int sciSetText (sciPointObj * pobj, char text[], int n);
extern char *sciGetText (sciPointObj * pobj);
extern unsigned int sciGetTextLength (sciPointObj * pobj);
extern int sciGetFontBackground (sciPointObj * pobj);
extern int sciSetFontBackground (sciPointObj * pobj, int color);
extern int sciGetFontForeground (sciPointObj * pobj);
/* F.Leray 09.04.04*/
extern int sciGetFontForegroundToDisplay (sciPointObj * pobj);
extern int sciGetFontBackgroundToDisplay (sciPointObj * pobj);

extern int sciSetFontForeground (sciPointObj * pobj, int color);
extern int sciGetFontStyle (sciPointObj * pobj);
extern int sciSetFontStyle (sciPointObj * pobj, int fontdeciwidth);
extern char *sciGetFontName (sciPointObj * pobj);
extern int sciSetFontName (sciPointObj * pobj, char pfontname[], int n);
extern int sciSetTextPosX (sciPointObj * pobj, double x);
extern int sciSetTextPosY (sciPointObj * pobj, double x);
extern double sciGetTextPosX (sciPointObj * pobj);
extern double sciGetTextPosY (sciPointObj * pobj);
extern double sciGetTextPosWidth (sciPointObj * pobj);
extern double sciGetTextPosHeight (sciPointObj * pobj);

/*Title Functions */
extern int sciSetTitlePos (sciPointObj * pobj, int x, int y);
extern POINT2D sciGetTitlePos (sciPointObj * pobj);
extern int sciSetTitlePlace (sciPointObj * pobj, sciTitlePlace place);
extern sciTitlePlace sciGetTitlePlace (sciPointObj * pobj);
extern unsigned int sciGetSizeofText (sciPointObj * pobj);

/* RelationShip Function */
extern sciRelationShip *sciGetRelationship (sciPointObj * pobj);
extern int sciSetParent (sciPointObj * pson, sciPointObj * pparent);
extern sciPointObj *sciGetParent (sciPointObj * pobj);
extern sciPointObj *sciGetParentFigure (sciPointObj * pobj);
extern sciPointObj *sciGetParentSubwin (sciPointObj * pobj);
extern int sciGetNumFigure (sciPointObj * pobj);
extern struct BCG *sciGetScilabXgc (sciPointObj * pobj);
extern void sciSetCurrentSon (sciPointObj * pparent, sciPointObj * pson);
extern sciPointObj *sciGetCurrentSon (sciPointObj * pobj);
extern BOOL sciAddThisToItsParent (sciPointObj * pthis,  sciPointObj * pparent);
extern BOOL sciDelThisToItsParent (sciPointObj * pthis, sciPointObj * pparent);
extern sciSons *sciGetSons (sciPointObj * pobj);
extern sciSons *sciGetLastSons (sciPointObj * pobj);

/* GMODE */
extern scigMode *sciGetGraphicMode (sciPointObj * pobj);
extern void sciSetIsClipping (sciPointObj * pobj, int value);
extern int sciGetIsClipping (sciPointObj * pobj);
extern void sciSetClipping (sciPointObj * pobj, double *pclip);
extern double *sciGetClipping (sciPointObj * pobj);
extern void sciClip (int value);
extern void sciUnClip (int value);
extern void sciSetHighLight (sciPointObj * pobj, BOOL value);
extern BOOL sciGetHighLight (sciPointObj * pobj);
extern void sciSetAddPlot (sciPointObj * pobj, BOOL value);
extern BOOL sciGetAddPlot (sciPointObj * pobj);
extern void sciSetAutoScale (sciPointObj * pobj, BOOL value);
extern BOOL sciGetAutoScale (sciPointObj * pobj);
extern void sciSetZooming (sciPointObj * pobj, BOOL value);
extern BOOL sciGetZooming (sciPointObj * pobj);
extern void sciSetGraphicsStyle (sciPointObj * pobj, BOOL value);
extern BOOL sciGetGraphicsStyle (sciPointObj * pobj);
extern void sciSetXorMode (sciPointObj * pobj, int value);
extern int sciGetXorMode (sciPointObj * pobj);
extern void sciSetVisibility (sciPointObj * pobj, BOOL value);
extern BOOL sciGetVisibility (sciPointObj * pobj);
extern void sciSetResize (sciPointObj * pobj, BOOL value);
extern BOOL sciGetResize (sciPointObj * pobj);
extern void sciSetDefaultValues ();

/* Window Functions */
extern void sciSetName (sciPointObj * pobj, char *pvalue, int length);
extern char *sciGetName (sciPointObj * pobj);
extern int sciGetNameLength (sciPointObj * pobj);

extern void sciSetNum (sciPointObj * pobj, int *pvalue);
extern int sciGetNum (sciPointObj * pobj);
extern void sciSetDim (sciPointObj * pobj, int *pwidth, int *pheight);
extern double sciGetWidth (sciPointObj * pobj); /* ???? */
extern double sciGetHeight (sciPointObj * pobj);/* ???? */
extern int sciSetFigurePos (sciPointObj * pobj, int pposx, int pposy);
extern int sciGetFigurePosX (sciPointObj * pobj);
extern int sciGetFigurePosY (sciPointObj * pobj);
extern void sciSetFigureIconify (sciPointObj * pobj, BOOL value);
extern BOOL sciGetIsFigureIconified (sciPointObj * pobj);

extern int sciSetSubWindowPos (sciPointObj * pobj, int *x, int *y);
extern int sciGetSubwindowPosX (sciPointObj * pobj);
extern int sciGetSubwindowPosY (sciPointObj * pobj);
extern BOOL sciGetIsSelected (sciPointObj * pobj);
extern sciPointObj *sciGetSelectedSon (sciPointObj * pobj);
extern sciPointObj *sciGetSelectedSubWin (sciPointObj * pparent);
extern int sciSetSelectedSubWin (sciPointObj * psubwinobj);
extern sciPointObj *sciIsExistingSubWin (double *WRect);
extern int sciSetOriginalSubWin (sciPointObj * pfigure, sciPointObj * psubwin);
extern int sciGetScrollPosV (sciPointObj * pobj);
extern int sciGetScrollPosH (sciPointObj * pobj);


/* Object Manipulation Function */
extern int sciDrawObj (sciPointObj * pobj);
extern int DestroyAllGraphicsSons (sciPointObj * pthis);
extern sciPointObj *ConstructFigure ();
extern int DestroyFigure (sciPointObj * pthis);
extern sciPointObj *ConstructStatusBar (sciPointObj * pparentfigure);
extern int DestroyStatusBar (sciPointObj * pthis);
extern sciPointObj *ConstructSubWin (sciPointObj * pparentfigure,
				     int pwinnum);
extern int DestroySubWin (sciPointObj * pthis);
extern sciPointObj *ConstructScrollV (sciPointObj * pparentfigure);
extern sciPointObj *ConstructScrollH (sciPointObj * pparentfigure);
extern int DestroyScrollV (sciPointObj * pthis);
extern int DestroyScrollH (sciPointObj * pthis);
extern sciPointObj *ConstructText (sciPointObj * pparentfigure, char text[],
				   int n, double x, double y,double *wh);
extern sciPointObj *CloneText (sciPointObj * pthis);
extern int DestroyText (sciPointObj * pthis);
extern sciPointObj *ConstructTitle (sciPointObj * pparentfigure, char text[],
				    int type);
extern int DestroyTitle (sciPointObj * pthis);
extern sciPointObj *ConstructLegend (sciPointObj * pparentfigure, char text[],
				     int n, int nblegends, int *pstyle, sciPointObj **pptabofpointobj);
extern int DestroyLegend (sciPointObj * pthis);
extern sciPointObj *ConstructPolyline (sciPointObj * pparentfigure,
				       double *pvecx, double *pvecy, double *pvecz,int closed, int n1, int n2, int plot);/*3D constructor*/
extern int DestroyPolyline (sciPointObj * pthis);
extern sciPointObj *ConstructArc (sciPointObj * pparentfigure, double x,
				  double y, double height, double width,
				  double alphabegin, double alphaend, int color, int fill);

extern int DestroyArc (sciPointObj * pthis);
extern sciPointObj *ConstructRectangle (sciPointObj * pparentfigure, double x,
					double y, double height, double width,
					double horzcurvature,
					double vertcurvature, int fillflag, int fillcolor, int str);
extern int DestroyRectangle (sciPointObj * pthis);
extern sciPointObj *ConstructSurface (sciPointObj * pparentsubwin, sciTypeOf3D typeof3d, 
				      double * pvecx, double * pvecy, double * pvecz,
				      integer *zcol, integer izcol, integer dimzx, integer dimzy, 
				      integer *flag, double *ebox, integer flagcolor, integer *isfac,
				      integer *m1, integer *n1, integer *m2, integer *n2, 
				      integer *m3, integer *n3, integer *m3n, integer *n3n);
extern int DestroySurface (sciPointObj * pthis);

extern sciPointObj *ConstructMerge (sciPointObj * pparentsubwin,int N,int *index_in_entity,long *from_entity);


extern int DestroyMerge (sciPointObj * pthis);
/*extern sciPointObj *ConstructAxis (sciPointObj * pparentfigure,char *strflag,int style,
				   double minx, double miny, double minz,
				   double maxx, double maxy, double maxz); 
extern int DestroyAxis (sciPointObj * pthis);
*/
extern sciPointObj *ConstructGrayplot (sciPointObj * pparentfigure,double *vx,double *vy, 
                                   double *vz,int nx,int ny, int type);
extern int DestroyGrayplot (sciPointObj * pthis);

extern sciPointObj *ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx,
                                   int nx, double *vy, int ny, char *str[], int subint, char *format, 
                                   int fontsize, int textcolor, int ticscolor, char logscale, int seg);

extern int DestroyAxes (sciPointObj * pthis);

extern sciPointObj *ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud, 
				  double *pfun, int Nnode, int Ntr, double *zminmax, integer *colminmax);
extern int DestroyFec (sciPointObj * pthis);
extern sciPointObj *ConstructSegs (sciPointObj * pparentsubwin, integer type,double *vx, double *vy, integer Nbr1, 
               integer Nbr2, double *vfx, double *vfy, integer flag, 
              integer *style, double arsize1,  integer colored, double arfact); 
extern int DestroySegs (sciPointObj * pthis);
/**/
extern int sciAddLabelMenu (sciPointObj * pthis, char plabel[], int n);
extern int sciDelLabelsMenu (sciPointObj * pthis);
extern sciPointObj *ConstructMenuContext (sciPointObj * pparentfigure);

extern int DestroyMenuContext (sciPointObj * pthis);
extern sciPointObj *ConstructMenu (sciPointObj * pparentfigure,
				      char plabel[], int n);
extern int DestroySciMenu (sciPointObj * pthis);
extern sciPointObj *ConstructAgregation (long *tabpointobj, int number);

extern sciPointObj *sciCloneObj (sciPointObj * pobj);
extern sciPointObj *sciCopyObj (sciPointObj * pobj, sciPointObj * psubwinparenttarget );

extern int sciSet (sciPointObj * pobj, char *marker, int *value, int *numrow,
		   int *numcol);
extern int sciGet (sciPointObj * pobj, char *marker);
extern long sciGetCurrentHandle ();
extern sciPointObj *sciGetCurrentObj ();
extern sciPointObj *sciGetCurrentFigure ();
extern void sciSetCurrentFigure (sciPointObj * mafigure);

struct BCG *sciGetCurrentScilabXgc ();
extern int sciSetCurrentObj (sciPointObj * pobj);
extern int sciSetReplay (BOOL value);
extern BOOL sciGetReplay ();
extern int sciSetHDC (sciPointObj * pobj);

extern int sciSetPosX (sciPointObj * pthis, double x);
extern int sciSetPosY (sciPointObj * pthis, double y);
extern double sciGetPosX (sciPointObj * pthis);
extern double sciGetPosY (sciPointObj * pthis);
extern double sciGetPosZ (sciPointObj * pthis);/**DJ.Abdemouche 2003**/
extern double sciGetPosWidth (sciPointObj * pthis);
extern double sciGetPosHeight (sciPointObj * pthis);
extern double *sciGetPoint (sciPointObj * pthis, int *num, int *numco);
extern int sciSetPoint(sciPointObj * pthis, double *tab, int *numrow, int *numcol);
extern sciPointObj *sciGetObjClicked (sciPointObj *pthis,int x, int y);
extern BOOL sciIsClicked(sciPointObj *pthis,int x, int y);

extern int sciAddCallback (sciPointObj * pthis, char *code, int len, int mevent);
extern int sciExecCallback (sciPointObj * pthis);
extern int sciGetCallbackMouseEvent (sciPointObj * pthis);
extern int sciSetCallbackMouseEvent (sciPointObj * pthis, int mevent);
extern int sciGetCallbackLen (sciPointObj * pthis);
extern char *sciGetCallback (sciPointObj * pthis);
extern int sciDelCallback (sciPointObj * pthis);
extern void sciRedrawFigure();
extern void sciRedrawF(int *num);
extern void sciXbasc();
extern void sciXclear();
extern void sciXdraw();
extern BOOL sciGetdrawmode (sciPointObj *pobj);
extern int sciSetdrawmode(BOOL mode);
extern int Objmove (long *hdl, double x,double y, BOOL opt);
extern int sciUndelete (long *hdl);
extern BOOL sciIsAreaZoom(integer *box, integer *box1 ,integer *section);
extern void DeleteObjs(int win);
extern void scizoom(double bbox[]);
extern void Zoom_Subwin(double bbox[]);
extern void Unzoom_Subwin();
extern sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin);
extern void sciSwitchWindow(int *winnum);
extern int sciType (char *marker, sciPointObj *pobj);  /* F.Leray MODIFICATION ICI*/
extern int sciUnAgregation (sciPointObj * pobj);
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int version_flag(); 
void set_version_flag(int flag) ;
extern int sciDelGraphicObj (sciPointObj * pthis);
extern unsigned int sciGetFontNameLength (sciPointObj * pobj);
extern struct BCG *sciGetCurrentScilabXgc ();
/* Specific function for Windows */
#ifdef WIN32
extern HFONT sciCreateFont (HDC hdc, char *szFaceName, int iDeciPtHeight, int iDeciPtWidth, int iAttributes, BOOL fLogRes);
extern HDC sciGetDC (sciPointObj * pobj);                                  
extern HDC sciGetHDC (sciPointObj * pobj);   
#endif
extern sciPointObj *sciGetSurface(sciPointObj *psubwin);
extern void Obj_RedrawNewAngle(sciPointObj *psubwin,double theta,double alpha);
extern BOOL Check3DObjs(sciPointObj *pobj);
extern sciPointObj *CheckClickedSubwin(integer x, integer y);
/**DJ.Abdemouche 2003**/
extern void SetEch3d1(double *xbox, double *ybox, double *zbox, double *bbox, double *teta, double *alpha, integer flag);
extern void Convex_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox);
extern void DrawAxis(double *xbox, double *ybox, integer *Indices, integer style);
extern void axis_3ddraw(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD);
extern void triedre(sciPointObj *pobj, double *xbox, double *ybox, double *zbox, integer *InsideU, integer *InsideD);
extern void Nextind(integer ind1, integer *ind2, integer *ind3);
/* extern int Axes3dStrings(integer *ixbox, integer *iybox, integer *xind, char *legend); */
extern int Axes3dStrings(integer *ixbox, integer *iybox, integer *xind);
extern int trans3d(sciPointObj *pobj,integer n,integer *xm,integer *ym,double *x, double *y,double *z);
extern BOOL Ishidden(sciPointObj *pobj);
extern BOOL IsDownAxes(sciPointObj *pobj);
extern void Plo2dTo3d(integer type, integer *n1, integer *n2, double *x, double *y, double *z, double *x1, double *y1, double *z1);
extern void update_3dbounds(sciPointObj *pobj);
double Fill_XYdec01_TLO_and_ISO_case(int Xdec3, double val);
extern void  sci_update_frame_bounds(int cflag);
extern double graphic_search(double *id, double *tab1, double *tab2, integer *n);
extern void update_graduation(sciPointObj *pobj);
/***/
extern void initsubwin();
extern int Gen3DPoints (integer type,integer *polyx,integer *polyy,integer *fill,integer whiteid,double zmin,double zmax,double *,double *,double *,integer i,integer j,integer jj1,integer *p,integer dc,integer fg); 
extern void Merge3d(sciPointObj *psubwin);/*DJ.A merge*/ 
extern void DrawMerge3d(sciPointObj *psubwin, sciPointObj *pmerge);
extern sciPointObj *sciGetMerge(sciPointObj *psubwin);
/* DJ.A 08/01/04 */
extern int C2F(CreateModels) ();
extern int InitFigureModel();
extern int InitAxesModel();

extern sciHandleTab * sciGetpendofhandletab();
extern void rebuild_strflag( sciPointObj * psubwin, char * STRFLAG);

/* F.Leray 25.05.04 */
extern int labels2D_draw(sciPointObj * psubwin);
extern sciPointObj * ConstructLabel (sciPointObj * pparentsubwin, char *text, int type);
extern int DestroyLabel (sciPointObj * pthis);
