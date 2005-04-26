/*--------------------------------------------------------------
  Authors Pierrick Mode, Serge Steer INRIA 2005, Copyright INRIA
  -------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../machine.h"

#define  typ_short "s"
#define  typ_ushort "us"
#define  typ_char "c"
#define  typ_uchar "uc"
#define  typ_double "d"
#define  typ_int "i"

extern int ripole(char *inputfile, char *outputfile, int debug, int verbose);
extern void C2F(mgetnc) (integer *fd, void *res, integer *n, char *type, integer *ierr);
extern void C2F(mtell) (integer *fd, double *offset, integer *err);
extern void C2F(mseek) (integer *fd, integer *offset, char *flag, integer *err);
extern FILE *GetFile(integer *fd);
extern int GetSwap(integer *fd);
extern void sciprint __PARAMS ((char *fmt,...));

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*Prototype*/
void xls_read(int *fd, int *cur_pos,double **data, int **chainesind, int *N, int *M, int *err);
void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos,int *nsheets);
static double NumFromRk2(long rk);
static void getBoundsheets(int * fd,char ***Sheetnames, int** Abspos, int *nsheets,int *cur_pos,int *err);
static void getSST(int *fd,int BIFF,int *ns,char ***sst,int *err);
static void getBOF(int *fd ,int* Data, int *err);
static void getString(int *fd, int flag,char **str,int *err);
static int get_oleheader(int *fd);
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

static double return_a_nan()
{
  static int first = 1;
  static double nan = 1.0;

  if ( first )
    {
      nan = (nan - (double) first)/(nan - (double) first);
      first = 0;
    }
  return (nan);
}



void xls_read(int *fd, int *cur_pos,double **data, int **chainesind, int *N, int *M, int *err)
{
  /*---------------Déclaration Des Variables*--------------------*/
  unsigned short Opcode, Len;   /*Code Operationnel et Longueur du tag a lire*/
  double *valeur;    /*Tableau Recapitulatif (Final) des valeurs de la feuille Excel*/
  double pos;

  int one=1;
  int three=3;

  int i;  /*Variables de boucle*/
  int hauteur, longueur, capacite;   /*Hauteur, longueur de la feuille,  */
  int taille; /*Nombre de types de caractèers a enregistrer*/
  char *sheetname;   /*Nom de la feuille*/
  int rkvalue; /*RK value*/
  /*for RK */
  short row, col, xf;/*Index to row, to column, and to XF record*/
  /*for MULRK */
  unsigned short ixfe;
  short colFirst,colLast,ncol;/*Index to rox, to first column (fc)*/
  /* for LABELSST */
  short labelsst1[3];
  int indsst;/*Index to SST record*/
  /* for DIMENSIONS */
  int f_row,l_row;
  short f_col, l_col, notused;
  /* for FORMULA */
  short formula_row, formula_col, formula_xf;
  double resultat;/*Result of the formula*/
  short optionflag;/*Option flags*/
  int formula_notused; /*Not used*/
  double NaN=return_a_nan();

  int BOFData[7]; /*[BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]*/
  /* initialization of pointers corresponding to malloc's */
  valeur=(double *)NULL;
  sheetname=(char *)NULL;
  *chainesind= (int *) NULL;
  *err=0;

  *cur_pos=*cur_pos;
  C2F(mseek) (fd, cur_pos, "set", err);
  if (*err > 0) goto ErrL;

  /* first record should be a BOF */
  getBOF(fd ,BOFData, err);

  if (*err > 0) return;
  if (BOFData[0]<0) { /* not a BOF */
    *err=2;
    return;
  }
  if (BOFData[0] != 8) { /* not a BIFF8 */
    *err=3;
    return;
  }

  C2F(mtell) (fd, &pos, err);
  if (*err > 0) goto ErrL;
  *cur_pos=(int)pos;

  while(1)
    {
      C2F(mseek) (fd, cur_pos, "set", err);
      if (*err > 0) goto ErrL;
      /*Enregistrement de l'Opcode et de la Len du tag*/
      C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
      if (*err > 0) goto ErrL;
      C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
      if (*err > 0) goto ErrL;
     switch(Opcode) {
     case 10:/*EOF */
       *N=hauteur;
       *M=longueur;
       *data=valeur;
       *cur_pos=*cur_pos+4+Len;
       return;
     case 638: /*RK*/
       C2F(mgetnc) (fd, (void*)&row, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&col, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&xf , &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &rkvalue , &one, typ_int, err);
       if (*err > 0) goto ErrL;
       valeur[col*(hauteur)+row]= NumFromRk2(rkvalue);
       break;
     case 515: /*Number*/
       C2F(mgetnc) (fd, (void*)&row, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&col, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&xf , &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &resultat , &one, typ_double, err);
       if (*err > 0) goto ErrL;
       valeur[col*(hauteur)+row]=resultat ;
       break;

     case 189: /*MULRK*/
       C2F(mgetnc) (fd, (void*)&row, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd,  (void*)&colFirst, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       /*List of nc=lc-fc+1  XF/RK structures*/
       ncol=(Len-6)/6;	
       for (i=0;i<ncol;i++) {
	 C2F(mgetnc) (fd, (void*) &ixfe, &one, typ_short, err);
	 if (*err > 0) goto ErrL;
	 C2F(mgetnc) (fd, (void*) &rkvalue, &one, typ_int, err);
	 if (*err > 0) goto ErrL;
	 valeur[row+(colFirst+i)*hauteur]=NumFromRk2(rkvalue);
       }

       /*Index of last column*/
       C2F(mgetnc) (fd, (void*) &colLast, &one, typ_ushort, err);
       if (*err > 0) goto ErrL;
       break;

     case 253:/*LABELSST*/
       C2F(mgetnc) (fd, (void*) labelsst1, &three, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &indsst , &one, typ_int, err);
       if (*err > 0) goto ErrL;
       /*Allocation dans le tableau final*/
       (*chainesind)[(labelsst1[1])*(hauteur)+labelsst1[0]]=indsst+1;
       break;
     case 512:/* DIMENSIONS*/
       C2F(mgetnc) (fd, (void*) &f_row, &one, typ_int, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &l_row, &one, typ_int, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &f_col, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &l_col, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &notused, &one, typ_short, err);
       if (*err > 0) goto ErrL;
	
       /*Calcul de longueur, hauteur et capacite dela feuille*/
       hauteur=l_row-f_row;
       longueur=l_col-f_col;
       capacite=hauteur*longueur;
	
       /*Déclaration des tableaux de synthèse*/
       if ((valeur=(void*) malloc((capacite+1)*sizeof(double)))==NULL)  goto ErrL;
       if ((*chainesind=(int *) malloc((capacite+1)*sizeof(int)))==NULL)  goto ErrL;
       for (i=0;i<=capacite;i++) {
	 (*chainesind)[i]=0;
	 valeur[i]=NaN;
       }
       break;
     case 6:/* FORMULA*/
       C2F(mgetnc) (fd, (void*) &formula_row, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &formula_col, &one, typ_short, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*) &formula_xf, &one, typ_short, err);
       if (*err > 0) goto ErrL;
	
       C2F(mgetnc) (fd, (void*) &resultat, &one, typ_double, err);
       if (*err > 0) goto ErrL;
       valeur[(formula_col*hauteur+formula_row)]=resultat;

       C2F(mgetnc) (fd, (void*)&optionflag, &one, typ_short, err);
       if (*err > 0) goto ErrL;
	
       C2F(mgetnc) (fd, (void*) &formula_notused, &one, typ_int, err);
       if (*err > 0) goto ErrL;
	
       /*Formuled data*/
       taille=Len-2-2-2-8-2-4;
       /*char formuladata[taille];
	 C2F(mgetnc) (fd, (void*) formuladata, &taille, typ_char, err);
	 if (*err > 0) goto ErrL;*/

       break;		
     }
     *cur_pos=*cur_pos+4+Len;
    }
  return;
 ErrL:
  {
    free(sheetname);
    free(valeur);
    free(*chainesind);
    if (*err==0)
      *err=1; /* malloc problem */
    else
      *err=2; /* read problem */
    return;
  }
}


void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos,int *nsheets)
{
  /* if opt==1 it is supposed that the current file position is at the beginning of oleheader
   * if opt==0 it is supposed that the current file position is at the  beginning of workbook stream
   */

  /* we suppose that the ole file as a simple structure: 
   * Workbook stream should follows immediately the header 
   * and is strored in sequential sections 
   */

  /*return *err:
    0 = OK
    1 = not an OLE file
    2 = no Workbook included
    3 = memory allocation problem
    4 = incorrect file
    5 = not a BIFF8 xls file
   */
  /*---------------Déclaration Des Variables*--------------------*/
  int k,one=1; 
  int cur_pos, init_pos;
  double pos;
  unsigned short Opcode, Len; 
  /*BOF data*/
  int BOFData[7]; /*[BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]*/
  *nsheets=0;
  *err=0;

  /*---------------Déclaration Des Variables*--------------------*/
  cur_pos=0;

  /*  if (get_oleheader(fd)) {
    *err=1;
    return;
    }*/
  C2F(mtell) (fd, &pos, err);
  cur_pos=(int)pos;
  init_pos=cur_pos;

  /* first record should be a BOF */
  getBOF(fd ,BOFData, err);
  if (*err > 0) return;

  if (BOFData[0]<0) { /* not a BOF */
    *err=4;
    return;
  }
  if (BOFData[0] != 8) { /* not a BIFF8 */
    *err=5;
    return;
  }

  C2F(mtell) (fd, &pos, err);
  if (*err > 0) goto Err2;
  cur_pos=(int)pos;

  /* loops on records till an EOF is found */
  while(1) {
    C2F(mseek) (fd, &cur_pos, "set", err);
    if (*err > 0) goto Err2;
    /*Enregistrement de l'Opcode et de la Len du tag*/
    C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
    if (*err > 0) goto Err2;
    C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
    if (*err > 0) goto Err2;
    switch(Opcode) {
    case 10: /*EOF*/
      cur_pos=cur_pos+4+Len;
      return ;
    case 133: /* Boundsheets */
      getBoundsheets(fd,Sheetnames, Abspos,nsheets,&cur_pos,err);
      for (k=0;k<*nsheets;k++) (*Abspos)[k]+=init_pos;
      if (*err > 0) return;
      break;
    case 252: /* SST= Shared String table*/
      getSST(fd,BOFData[0],ns,sst,err);
      if (*err > 0) return;
      cur_pos=cur_pos+4+Len;
      break;
    default:
      cur_pos=cur_pos+4+Len;
    }
  }
  return;
 Err2:
  *err=4; /* read problem */
  return;

}

static double NumFromRk2(long rk)
{
  double num;
  if(rk & 0x02)
    {
      /* int*/
      num = (double) (rk >> 2);
    }
  else
    {
      /* hi words of IEEE num*/
      *((int *)&num+1) = rk & 0xfffffffc;
      *((int *)&num) = 0;
    }
  if(rk & 0x01)
    /* divide by 100*/
    num /= 100;
  return num;
}

static void getBOF(int *fd ,int* Data, int *err)
{
  /* return Data a vector [BIFF  Version DataType Identifier Year HistoryFlags LowestXlsVersion]
   * works for BIFF2 to BIFF8 records */
  int BIFF;
  short Version;
  short DataType;
  short Identifier=0;
  short Year=0;
  int HistoryFlags=0;
  int LowestXlsVersion=0;

  unsigned short Opcode;
  unsigned short Len;
  int one=1;

  C2F(mgetnc) (fd, (void*)&Opcode, &one, typ_ushort, err);
  if (*err > 0) return;
  C2F(mgetnc) (fd, (void*)&Len, &one, typ_ushort, err);
  if (*err > 0) return;

  switch(Opcode) {
  case 2057:     /*Begin of file, BOF for BIFF5 BIFF7 BIFF8 BIFF8X*/
    C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&Identifier, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&Year, &one, typ_short, err);
    if (*err > 0) return;
    if (Len==16) {
      C2F(mgetnc) (fd, (void*)&HistoryFlags, &one, typ_int, err);
      if (*err > 0) return;
      C2F(mgetnc) (fd, (void*)&LowestXlsVersion, &one, typ_int, err);
      if (*err > 0) return;
      BIFF=8;
      if (Version != 1536) return;
    }
    else
      BIFF=7;
    break;
  case 1033 : /*Interprétation du BIFF4  0409 H*/
    C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
    if (*err > 0) return;
    BIFF=4;
    break;
  case 521 : /*Interprétation du BIFF3  0209 H*/
    C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
    if (*err > 0) return;
    BIFF=3;
    break;
  case 9 : /*Interprétation du BIFF2  0009 H*/
    C2F(mgetnc) (fd, (void*)&Version, &one, typ_short, err);
    if (*err > 0) return;
    C2F(mgetnc) (fd, (void*)&DataType, &one, typ_short, err);
    if (*err > 0) return;
    BIFF=2;
    break;
  default:
    BIFF=-1; /* not a BOF record */
    Version=0;
    DataType=0;
  }
  Data[0]=BIFF;
  Data[1]=Version;
  Data[2]=DataType;
  Data[3]=Identifier;
  Data[4]=Year;
  Data[5]=HistoryFlags;
  Data[6]=LowestXlsVersion;

}

static void getSST(int *fd,int BIFF,int *ns,char ***sst,int *err)
{
  int i,one=1;
  /* SST data */
  int ntot; /*total number of strings */
  int nm;/*Number of following strings*/

  char *transfert;  /*temp*/
  transfert=(char *)NULL;
  *ns=0;

  *sst=NULL;

  if(BIFF==8) {
    /*Total number of strings in the workbook*/
    C2F(mgetnc) (fd, (void*)&ntot, &one, typ_int, err);
    if (*err > 0) goto ErrL;
    C2F(mgetnc) (fd, (void*)&nm, &one, typ_int, err);
    if (*err > 0) goto ErrL;
    *ns=nm;
    if (nm !=0) {
      if( (*sst=(char **)malloc(nm*sizeof(char*)))==NULL)  goto ErrL;
      for (i=0;i<nm;i++) (*sst)[i]=NULL;
      for(i=0;i<nm;i++) {/* LOOP ON STRINGS */
	getString(fd,1,&((*sst)[i]),err);
	if (*err > 0) goto ErrL;
      }
    }
  }
  return;
 ErrL:
  if (*sst != NULL) {
    for (i=0;i<nm;i++)
      if ( (*sst)[i]!= NULL ) free((*sst)[i]);
    free(*sst);
  }

  if (*err==0)
    *err=3; /* malloc problem */
  else
    *err=4; /* read problem */
}

static void getString(int *fd, int flag,char **str,int *err)
{
  short ln;
  int longueur,one=1;
  char OptionFlag;
  int compressed,fareast,rich;
  int *list; /*formatting runs */

  *str=(char *)NULL;
  list= (int *)NULL;
  *err=0;
  ln=0;
  if (flag)
    C2F(mgetnc) (fd, (void*)&ln, &one, typ_short, err);
  else
    C2F(mgetnc) (fd, (void*)&ln, &one, typ_char, err);

  if (*err > 0) goto ErrL;
  C2F(mgetnc) (fd, (void*)&OptionFlag, &one, typ_char, err);
  if (*err > 0) goto ErrL;
  compressed=(int)(OptionFlag&0x01);
  fareast=(int)(OptionFlag&0x04);
  rich=(int)(OptionFlag&0x08);

  if(compressed==0)
    longueur=ln;
  else if(compressed==1)
    longueur=2*ln;
		
  if(rich==0 && fareast==0) {
    /*Enregistrement du character array*/
    if ((*str= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)*str, &longueur, typ_char, err);
    if (*err > 0) goto ErrL;
    (*str)[longueur]='\0';

  }
  else if(rich!=0 && fareast==0)  {
    short rt;/*number of rich Text formatting runs*/
    int listlength;
    C2F(mgetnc) (fd, (void*)&rt, &one, typ_short, err);
    if (*err > 0) goto ErrL;
    listlength=4*rt;
    /*Enregistrement du character array*/
    if ((*str= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)*str, &longueur, typ_char, err);
    if (*err > 0) goto ErrL;
    (*str)[longueur]='\0';
    if ((list= (int *) malloc(listlength*sizeof(int)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)list, &listlength, typ_int, err);
    if (*err > 0) goto ErrL;
    free(list);list=(int *)NULL;
  }
  else if(rich==0 && fareast!=0) {
    int sz; /* fareast data size */
    char *asian;
    C2F(mgetnc) (fd, (void*)&sz, &one, typ_int, err);
    if (*err > 0) goto ErrL;
    /*Enregistrement du character array*/
    if ((*str= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)*str, &longueur, typ_char, err);
    if (*err > 0) goto ErrL;
    (*str)[longueur]='\0';
    /*asian phonetics*/
    if ((asian = (char *) malloc(sz*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)asian, &sz, typ_char, err);
    if (*err > 0) goto ErrL;
    free(asian);asian=(char *)NULL;
  }
  else if(rich!=0 && fareast!=0) {
    short rt;
    int sz; /* fareast data size */
    char *asian;
    int listlength;/*list of rt formatting runs*/
    C2F(mgetnc) (fd, (void*)&rt, &one, typ_short, err);
    if (*err > 0) goto ErrL;
    listlength=4*rt;
    C2F(mgetnc) (fd, (void*)&sz, &one, typ_int, err);
    if (*err > 0) goto ErrL;
    if ((*str= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)*str, &longueur, typ_char, err);
    if (*err > 0) goto ErrL;
    (*str)[longueur]='\0';
    if ((list= (int *) malloc(listlength*sizeof(int)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)list, &listlength, typ_int, err);
    if (*err > 0) goto ErrL;
    free(list);list=(int *)NULL;
    /*asian phonetics*/
    if ((asian= (char *) malloc(sz*sizeof(char)))==NULL)  goto ErrL;
    C2F(mgetnc) (fd, (void*)asian, &sz, typ_char, err);
    if (*err > 0) goto ErrL;
    free(asian);asian=(char *)NULL;
  }
  else{
    sciprint("Unhandled case");
    goto ErrL;
  }
  return;
 ErrL:
  if (*err==0) {
    free(*str);
    free(list);
    *err=3; /* malloc problem */
  }
  else
    *err=4; /* read problem */
}	

static void getBoundsheets(int * fd,char ***Sheetnames, int** Abspos, int *nsheets,int *cur_pos,int *err)
{/* the global workbook contains a sequence of boudsheets this procedure reads all
 * the sequence and returns a vector o sheetnames, a vector of absolute sheet positions*/
  int abspos; /* Absolute stream position of BoF*/
  char visibility,sheettype;/*Visiblity , Sheet type*/
  int pos;
  unsigned short Opcode;
  unsigned short Len;
  int one=1;
  int ns,i;

  *Sheetnames=(char **)NULL;
  *Abspos=(int *)NULL;
  *err=0;

  /* memorize the first boundsheet beginning */
  pos=*cur_pos;
  /* Count number of boundsheets */
  ns=0;
  while(1) {
    C2F(mseek) (fd, cur_pos, "set", err);
    if (*err > 0) goto ErrL;
    C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
    if (*err > 0) goto ErrL;
    C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
    if (*err > 0) goto ErrL;
    if (Opcode==133) {
      C2F(mgetnc) (fd, (void*)&abspos, &one, typ_int, err);
      if (*err > 0) goto ErrL;
      C2F(mgetnc) (fd, (void*)&visibility, &one, typ_char, err);
      if (*err > 0) goto ErrL;
      C2F(mgetnc) (fd, (void*)&sheettype, &one, typ_char, err);
      if (sheettype==0) {/* worksheet */
	ns++;
      }
      *cur_pos=*cur_pos+4+Len;
    }
    else
      break;

    }

  *nsheets=ns;
  /*alloc the Sheetnames ans Abspos arrays */
   if( (*Sheetnames=(char **)malloc(ns*sizeof(char*)))==NULL)  goto ErrL;
   if( (*Abspos=(int *)malloc(ns*sizeof(int)))==NULL)  goto ErrL;

   /* rescan boundsheet sequence to get the data */
   *cur_pos=pos;
   i=-1;
   while(1) {
     C2F(mseek) (fd, cur_pos, "set", err);
     if (*err > 0) goto ErrL;
     C2F(mgetnc) (fd, &Opcode, &one, typ_ushort, err);
     C2F(mgetnc) (fd, &Len, &one, typ_ushort, err);
     if (Opcode==133) {
       C2F(mgetnc) (fd, (void*)&abspos, &one, typ_int, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&visibility, &one, typ_char, err);
       if (*err > 0) goto ErrL;
       C2F(mgetnc) (fd, (void*)&sheettype, &one, typ_char, err);
       if (sheettype==0) {/* worksheet */
	 i++;
	 (*Abspos)[i]=abspos;
	 getString(fd, 0,&((*Sheetnames)[i]),err);
	 if (*err > 0) goto ErrL;
       }
       *cur_pos=*cur_pos+4+Len;
     }
     else
       break;

   }
   return;
 ErrL:
   if (*Sheetnames != NULL) {
     for (i=0;i<ns;i++)
       if ( (*Sheetnames)[i]!= NULL ) free((*Sheetnames)[i]);
     free(*Sheetnames);
   }
   free(*Abspos);
   if (*err==0)
     *err=3; /* malloc problem */
   else
     *err=4; /* read problem */
}
static int get_oleheader(int *fd)
{
  unsigned char MAGIC[8] = { 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };
  unsigned char header[512];
  int c,ierr;

  C2F(mgetnc) (fd, (void *)header,(c=512,&c), typ_uchar, &ierr);
  if (ierr !=0)  return 1;
  if (memcmp (header, MAGIC, sizeof (MAGIC)) != 0) return 1;
  return 0;
}
