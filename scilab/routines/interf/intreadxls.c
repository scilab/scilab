#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../machine.h"


#include "../stack-c.h"


/*-----------------------------------------
  Author Pierrick Mode INRIA, Copyright INRIA
  -----------------------------------------*/

extern void C2F(mgetnc) (integer *fd, double *res, integer *n, char *type, integer *ierr);
extern void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);
extern void C2F(mtell) (integer *fd, double *offset, integer *err);
extern void C2F(mseek) (integer *fd, integer *offset, char *flag, integer *err);


/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*Prototype*/
int xls_read(double **data, int **chainesind,int *N,int *M, int *err,int *Pos);
void xls_open(int *err, int *Pos, char ***sst, int *ns);
int C2F(numfromrk)(integer *RKP,double* NUMP);
double NumFromRk2(long rk);
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/


int C2F(intreadxls)(char *fname)
{
  int m1,n1,l1,lpos,zero=0,ierr;
  double *data;
  int *ind;
  int M,N,MN;
  
  CheckLhs(2,3);
  CheckRhs(1,1);

  /*  checking variable Pos */
  GetRhsVar(1,"i",&m1,&n1,&lpos);
  
 
  xls_read(&data, &ind, &N, &M,  &ierr, istk(lpos)); 
  if (ierr == 1) 
    {
      Scierror(999,"%s :Not enough memory to allocate results \r\n",fname);
      return 0;
    }
  else if(ierr == 2)
    {
      Scierror(999,"%s :Failed to read expected data, may be invalid xls file \r\n",fname);
      return 0;
    }
  else if(ierr == 3)
    {
      Scierror(999,"%s :End of File \r\n",fname);
      return 0;
    }

  sciprint("readxls %d %d %d\n",M,N,ierr);
  
  MN=M*N;
  if (MN==0) 
    {
      CreateVar(Rhs+1, "d", &zero,&zero, &l1);
      CreateVar(Rhs+2, "d", &zero,&zero, &l1);
    }
  
  else 
    {
      CreateVarFromPtr(Rhs+1, "d", &M,&N, &data);
      CreateVarFromPtr(Rhs+2, "i", &M,&N, &ind);
      free(data);
      free(ind);
    }
  
  
  LhsVar(1)=1;
  LhsVar(2)= Rhs+1;
  LhsVar(3)= Rhs+2;
  C2F(putlhsvar)();

  return 0;
}

int C2F(intopenxls)(char *fname)
{
  int k,m1,n1,l1,l2,one=1,five=5,fd,f_swap=0;
  int ierr,ns;
  double res;
  char **sst;
  
  CheckLhs(2,2);

  CheckRhs(1,1);

  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  C2F(mopen)(&fd, cstk(l1),"rb", &f_swap, &res, &ierr);
  if (ierr != 0) 
    {
      Scierror(999,"%s :Cannot open file %s \r\n",fname,cstk(l1));
      return 0;
    }
  
  CreateVar(2,"i",&one,&five,&l2);
  *istk(l2)=fd; /* logical unit */
  *istk(l2+1)=0; /* cur_pos    */
  *istk(l2+2)=0;  /* eofcount */
  *istk(l2+3)=0;   /* fcount */
  *istk(l2+4)=0;    /* BIFF */

  xls_open(&ierr, istk(l2),&sst,&ns);

  if (ierr == 1) 
    {
      Scierror(999,"%s :Not enough memory to allocate sst \r\n",fname);
      return 0;
    }
  else  if (ierr == 2) 
    {
      Scierror(999,"%s :Failed to read expected data, may be invalid xls file \r\n",fname);
      return 0;
    }
  
  /* Create a typed list to return the properties */
  CreateVarFromPtr(3,"S", &one, &ns, sst); 
  for (k=0;k<ns;k++) free(sst[k]);
  free(sst);
  
  LhsVar(1)= 2;
  LhsVar(2)= 3;
  C2F(putlhsvar)();
  return 0;
}


int xls_read(double **data, int **chainesind, int *N, int *M, int *err,int *Pos)
{
  /*---------------Déclaration Des Variables*--------------------*/
  unsigned short Opcode, Len;   /*Code Operationnel et Longueur du tag a lire*/
  char *sst2;
  char head[513];   /*Entete de 512 caracteres*/
  double pos;    /* Variable "egale" a cur_pos*/
  double *valeur;    /*Tableau Recapitulatif (Final) des valeurs de la feuille Excel*/
  char *transfert;  /*Chaine temporaire servant a l'enregistrement des string dans le tableau finale*/
  
  /*Type des bits a copier*/
  char typ_short[]="s";
  char typ_ushort[]="us";
  char typ_char[]="c";
  char typ_double[]="d";
  char typ_int[]="i";
  
  int one=1;
  int two=2;
  int three=3;
  int four=4;

  int i,j, n;  /*Variables de boucle*/
  int loop=1;    /*Variables de boucle*/
  int fcount,eofcount;    /*Compteur du nombre de feuille du fichier Excel  et compteur de chaque fin de feuille*/  
  int nbchar, nbstring;   /*Nombre de string et de caractères contenues dans le SST*/
  int hauteur, longueur, capacite;   /*Hauteur de la feuille,    longueur de la feuille,    et capacite de celle ci (=longueur*hauteur)*/
  int *fd, *cur_pos;

  fd=&(Pos[0]);
  cur_pos=&(Pos[1]);
  
  if(Pos[2]>Pos[3])
    {
      *err=3;
      return 0;
    }
 
  /*----------Parcours du fichier & Déclaration des variables--------*/
  
  int taille; /*Nombre de types de caractèers a enregistrer*/
  unsigned short infos[two];   /*Tableaux contenant Opcode et Len = Informations du tag lu*/
  char *chaine3;   /*Sert a stocker les tags non lus*/
  int BIFF8;  /*BIFF8*/
  
  /*----------Parcours du fichier & Déclaration des variables--------*/


  while(loop!=0)
    {
      C2F(mseek) (fd, cur_pos, "set", err);
      /*Enregistrement de l'Opcode et de la Len du tag*/
      C2F(mgetnc) (fd, (double*) infos, &two, typ_short, err);
      if (*err > 0) goto ErrL;
      Opcode=infos[0]; Len=infos[1];
      /*------------------------------------------------------------------*/
      /*------------------------------------------------------------------*/


      /*------------------------------------------------------------------*/
      /*------------------------------------------------------------------*/
      switch(Opcode)
	{
	case 10:
	  {	     
	    Pos[2]=Pos[2]+1;
	    *N=hauteur;
	    *M=longueur;
	    *data=valeur;
	    
	    if (Pos[2]!=Pos[3])
	      {
		Pos[1]=Pos[1]+4+Len;
		return 1;
	      }
	    else 
	      {
		Pos[1]=-3;
		return 1;
	      }
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 638:
	  {	    /*Interpretation des RK*/
	    /*Index to row, to column, and to XF record*/
	    short row, col, xf;
	    C2F(mgetnc) (fd, (void *)&row, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (void *)&col, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (void *)&xf , &one, typ_short, err);
	    if (*err > 0) goto ErrL;

	    /*RK value*/
	    int rkvalue;
	    C2F(mgetnc) (fd, (double*) &rkvalue , &one, typ_int, err);
	    if (*err > 0) goto ErrL;
	    valeur[col*(hauteur)+row]= NumFromRk2(rkvalue);
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 189:
	  {  /*Interpretation des MULRK*/
	    int temp;
	    unsigned short ixfe;
	    
	    /*Index to rox, to first column (fc)*/
	    short row,colFirst,colLast,ncol;
	    C2F(mgetnc) (fd, (void *)&row, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd,  (void *)&colFirst, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    
	    /*List of nc=lc-fc+1  XF/RK structures*/
	    ncol=(Len-6)/6;	    
	    for (i=0;i<ncol;i++) 
	      {
		C2F(mgetnc) (fd, (void *) &ixfe, &one, typ_short, err);
		if (*err > 0) goto ErrL;
		C2F(mgetnc) (fd, (void *) &temp, &one, typ_int, err);
		if (*err > 0) goto ErrL;
		valeur[row+(colFirst+i)*hauteur]=NumFromRk2(temp);
	      } 

	    /*Index of last column*/
	    C2F(mgetnc) (fd, (void *) &colLast, &one, typ_ushort, err);
	    if (*err > 0) goto ErrL;
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 133:
	  {/*Interprétation des feuilles de travail, Boundsheet*/
	    
	    /* Absolute stream position of BoF*/
	    int abspos;
	    C2F(mgetnc) (fd, (void*) &abspos, &one, typ_int, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Visiblity , Sheet type*/
	    char visibility;
	    char sheettype;
	    C2F(mgetnc) (fd, (void*) &visibility, &one, typ_char, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (void*) &sheettype, &one, typ_char, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Length-Absolute strem position of BoF - visibility - sheet type = Sheet name*/
	    taille=Len-4-2; 
	    char sheetname[taille];
	    C2F(mgetnc) (fd, (void*) sheetname, &taille, typ_char, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Incrementation du fcount*/
	    Pos[3]=Pos[3]+1;
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 253:
	  {   /*Interpretation du LABELSST*/
	    /*Tableau récapitulatif des textes
	      Index to row, to column and to XF record*/
	    
	    short labelsst1[three];
	    C2F(mgetnc) (fd, (double*) labelsst1, &three, typ_short, err);
	    if (*err > 0) goto ErrL;

	    /*Index to SST record*/
	    int indsst;
	    C2F(mgetnc) (fd, (double*) &indsst , &one, typ_int, err);
	    if (*err > 0) goto ErrL;

	    /*Allocation dans le tableau final*/
	    int I=indsst;
	    (*chainesind)[(labelsst1[1])*(hauteur)+labelsst1[0]]=I;
	  }
	  break;
	  /*------------------------------------------------------------------*/ 
	  /*------------------------------------------------------------------*/	   
	      
	      
	  /*------------------------------------------------------------------*/	
	  /*------------------------------------------------------------------*/	   
	case 512:
	  {	  /*Interpretation des DIMENSIONS*/
	    /*Index 
	      to first used row, 
	      to last used row, increased by 1,
	      to first used column,
	      to last used column, increased by 1,
	      not used*/
	    
	    int f_row,l_row;
	    short f_col, l_col, notused;
	    C2F(mgetnc) (fd, (double*) &f_row, &one, typ_int, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &l_row, &one, typ_int, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &f_col, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &l_col, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &notused, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Calcul de longueur, hauteur et capacite dela feuille*/
	    hauteur=l_row-f_row;
	    longueur=l_col-f_col;
	    capacite=hauteur*longueur;
	    
	    /*Déclaration des tableaux de synthèse*/
	    if ((valeur=(double*) malloc((capacite+1)*sizeof(double)))==NULL)  goto ErrL;
	    /* test malloc a faire */
	    if ((*chainesind=(int *) malloc((capacite+1)*sizeof(int)))==NULL)  goto ErrL;
	    for (i=0;i<=capacite;i++) 
	      {
		(*chainesind)[i]=-1; 
		valeur[i]=0;
	      }
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 6:
	  {
	    /*Interpretatio des FORMULA*/

	    /*Index to row, Index to column, Index to XF record*/
	    short formula_row, formula_col, formula_xf;
	    C2F(mgetnc) (fd, (double*) &formula_row, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &formula_col, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (double*) &formula_xf, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Result of the formula*/
	    double resultat;
	    C2F(mgetnc) (fd, (double*) &resultat, &one, typ_double, err);
	    if (*err > 0) goto ErrL;
	    valeur[(formula_col*hauteur+formula_row)]=resultat;
	    
	    /*Option flags*/ 
	    short optionflag;
	    C2F(mgetnc) (fd, (double*)&optionflag, &one, typ_short, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Not used*/
	    int formula_notused;
	    C2F(mgetnc) (fd, (double*) &formula_notused, &one, typ_int, err);
	    if (*err > 0) goto ErrL;
	    
	    /*Formuled data*/
	    taille=Len-2-2-2-8-2-4;/*Index to row, to column, to XF record, Result of formula, NOt used*/
	    char formuladata[taille];
	    C2F(mgetnc) (fd, (double*) formuladata, &taille, typ_char, err);
	    if (*err > 0) goto ErrL;
	  }
	  break;		    
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	  
	}
      *cur_pos=*cur_pos+4+Len;
      Pos[1]= *cur_pos;
    } 
 
 FAIL:
  return;
 ErrL:
  {
    if (*err==0) 
      *err=1; /* malloc problem */
    else
      *err=2; /* read problem */
  }
}


void xls_open(int *err, int *Pos, char ***sst, int *ns)
{
  /*---------------Déclaration Des Variables*--------------------*/
  
  int hauteur, longueur, capacite;   /*Hauteur de la feuille,    longueur de la feuille,    et capacite de celle ci (=longueur*hauteur)*/
  int i,j;   /*Variables de boucle*/
  int loop=1;    /*Variables de boucle*/
  int fcount=0 ,eofcount=0;    /*Compteur du nombre de feuille du fichier Excel  et compteur de chaque fin de feuille*/
  int nbchar, nbstring;   /*Nombre de string et de caractères contenues dans le SST*/
  int *position;
  int cur_pos;
  unsigned short Opcode, Len;   /*Code Operationnel et Longueur du tag a lire*/
  
  char head[513];   /*Entete de 512 caracteres*/
  char typ [3]; /*Type des caractères que l'on va lire, s=short, i=int, c=char...*/
  char *sst2;    /*Tableau des strings du workbook*/
  
  double pos;    /* Variable "egale" a cur_pos*/
  
  char *transfert;  /*Chaine temporaire servant a l'enregistrement des string dans le tableau finale*/
  char typ_short[]="s";
  char typ_ushort[]="us";
  char typ_char[]="c";
  char typ_double[]="d";
  char typ_int[]="i";

  int  one=1;
  int two=2;
  int three=3;
  int four=4;

  *err=0;
  *sst=NULL;
  /*---------------Déclaration Des Variables*--------------------*/
  int *fd=&(Pos[0]);
  cur_pos=Pos[1];
  
  /*Enregistrement de l'entete*/
  if(cur_pos==0)
    {
      C2F(mseek) (fd, &cur_pos, "set", err);
      int n=512;
      strcpy(typ,"c");
      C2F(mgetnc) (fd, (double*) head, &n, typ, err);
      C2F(mtell) (fd, &pos, err);
      cur_pos=(int)pos;
    }


  /*----------Parcours du fichier & Déclaration des variables--------*/
  
  int taille; /*Nombre de types de caractèers a enregistrer*/
  unsigned short infos[two];   /*Tableaux contenant Opcode et Len = Informations du tag lu*/
  char *chaine3;   /*Sert a stocker les tags non lus*/
  int BIFF;  /*BIFF*/
  
  /*----------Parcours du fichier & Déclaration des variables--------*/


  while(loop!=0)
    {
      C2F(mseek) (fd, &cur_pos, "set", err);
      /*Enregistrement de l'Opcode et de la Len du tag*/
      C2F(mgetnc) (fd, (double*) infos, &two, typ_short, err);
		    if (*err > 0) goto ErrL;
      Opcode=infos[0]; Len=infos[1];
      /*------------------------------------------------------------------*/
      /*------------------------------------------------------------------*/
      
      
      /*------------------------------------------------------------------*/
      /*------------------------------------------------------------------*/
      switch(Opcode)
	{
	  /*Begin of file, BOF  0809-->2057*/ 
	case 2057:    
	  {  /*Interpretation du Begion Of File*/
	    /*Interprétation du BIFF*/
	    unsigned short chaine[four];
	    C2F(mgetnc) (fd, (double*) chaine, &four, typ_short, err);
		    if (*err > 0) goto ErrL;
	    int chaine2[two];
	    C2F(mgetnc) (fd, (double*) chaine2, &two, typ_int, err);
		    if (*err > 0) goto ErrL;
	    if(chaine[0]==1536) // (0600)hexa=1536
	      BIFF=8;
	    else if(chaine[0]==1280)
	      BIFF=7;
	    Pos[4]=BIFF;
	  }
	  break; 
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 1033:
	  {
	    /*Interprétation du BIFF4  0409 H*/
	    BIFF=4;
	    Pos[4]=BIFF;
	  }
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 521:
	  {
	    /*Interprétation du BIFF4  0409 H*/
	    BIFF=3;
	    Pos[4]=BIFF;
	  }
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 10:
	  {	     
	    eofcount=eofcount+1;	
	    Pos[1]=cur_pos+4+Len;
	    Pos[2]=eofcount;
	    Pos[3]=fcount+1;
	    
	    return ;
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/

	  
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 133:
	  {	
	    /*Interprétation des feuilles de travail, Boundsheet*/
	    
	    /* Absolute stream position of BoF*/
	    int abspos;
	    C2F(mgetnc) (fd, (void*) &abspos, &one, typ_int, err);
		    if (*err > 0) goto ErrL;

	    /*Visiblity , Sheet type*/
	    char visibility;
	    char sheettype;
	    C2F(mgetnc) (fd, (void*) &visibility, &one, typ_char, err);
		    if (*err > 0) goto ErrL;
	    C2F(mgetnc) (fd, (void*) &sheettype, &one, typ_char, err);
		    if (*err > 0) goto ErrL;
	    
	    /*Length-Absolute strem position of BoF - visibility - sheet type = Sheet name*/
	    taille=Len-4-2; 
	    char sheetname[taille];
	    C2F(mgetnc) (fd, (void*) sheetname, &taille, typ_char, err);
		    if (*err > 0) goto ErrL;

	    /*Incrementation du fcount*/
	    fcount=fcount+1;
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/
	case 252:
	  { 
	    /*Interprétation de la SST= Shared String table*/
	    if(BIFF==8)
	      {
		/*Total number of strings in the workbook*/
		C2F(mgetnc) (fd, (double*) &nbstring, &two, typ_int, err);

		if( (*sst=(char **)malloc((nbstring+1)*sizeof(char*)))==NULL)  goto ErrL;
		if (*err > 0) goto ErrL;
		for (i=0;i<nbstring;i++) 
		  (*sst)[i]=NULL;

		int longueur;
		*ns=nbstring;
		
		for(i=0;i<nbstring;i++)
		  {
		    /*Number of folowing strings,  (nm)*/
		    short nm;
		    C2F(mgetnc) (fd, (double*) &nm, &one, typ_short, err);
		    if (*err > 0) goto ErrL;
		    
		    /*Lecture de l'octet et analyse de celui-ci*/
		    char ustring;
	            int compressed,fareast,rich;
		    C2F(mgetnc) (fd, (double*) &ustring, &one, typ_char, err);
		    if (*err > 0) goto ErrL;

		    compressed=(int)(ustring&0x01);
		    fareast=(int)(ustring&0x04);
		    rich=(int)(ustring&0x08);
		    sciprint("i=%d,compressed=%d,fareast=%d,rich=%d,nm=%d\n",i,compressed,fareast,rich,nm);
		    if(compressed==0)
		      longueur=nm;
		    else if(compressed==1)
		      longueur=2*nm;
		    		    
		    if(rich==0 && fareast==0)
		       {
			 /*Enregistrement du character array*/
			 if ((transfert= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
			 C2F(mgetnc) (fd, (double*) transfert, &longueur, typ_char, err);
			 printf("err=%i",*err);
			 if (*err > 0) goto ErrL;
			 transfert[longueur]='\0';
			 
			 /*Enregistrement de la vauleur dans le tableau récapitulatif*/
			 (*sst)[i]=transfert;
		       }
		    if(rich!=0 && fareast==0)
		      {
			/*number of rich Text formatting runs*/
			short rt;
			C2F(mgetnc) (fd, (double*) &rt, &one, typ_short, err);
			if (*err > 0) goto ErrL;
			
			/*Enregistrement du character array*/
			if ((transfert= (char*) malloc((longueur+1)*sizeof(char)))==NULL)  goto ErrL;
			C2F(mgetnc) (fd, (double*) transfert, &longueur, typ_char, err);
			printf("err=%i",*err);
			if (*err > 0) goto ErrL;
			
			transfert[longueur]='\0';
			/*Enregistrement de la vauleur dans le tableau r-bécapitulatif*/
			(*sst)[i]=transfert;
			
			/*list of rt formatting runs*/
			int list;
			int listlength=4*rt;
			C2F(mgetnc) (fd, (double*) list, &listlength, typ_int, err);
			if (*err > 0) goto ErrL;
		      }
		    sciprint("%s\n",transfert);
		  }
	      }
	  }
	  break;
	  /*------------------------------------------------------------------*/
	  /*------------------------------------------------------------------*/


	  /*------------------------------------------------------------------*/	
	  /*------------------------------------------------------------------*/	   
	}
      cur_pos=cur_pos+4+Len;
      Pos[1]=cur_pos;
    } 
  return;
 ErrL:
  {
    if (*sst != NULL) 
      {
	for (i=0;i<nbstring;i++) 
	  {
	    if ( (*sst)[i]!= NULL ) free((*sst)[i]);
	  }
	free(*sst);
      }
    if (*err==0) 
      *err=1; /* malloc problem */
    else
      *err=2; /* read problem */
  }
}

double NumFromRk2(long rk)
{
  double num;
  if(rk & 0x02)
    {
      // int
      num = (double) (rk >> 2);
    }
  else
    {
      // hi words of IEEE num
      *((int *)&num+1) = rk & 0xfffffffc;
      *((int *)&num) = 0;
    }
  if(rk & 0x01)
    // divide by 100
    num /= 100;
  return num;
}
