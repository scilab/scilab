# include "scicos_block4.h"
# include "../machine.h"
#include "../stack-c.h"
#include <stdio.h>
#include <string.h>

#if WIN32
#define NULL    0
#endif

extern int C2F(cvstr) __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern int C2F(mputnc)();
extern int C2F(namstr)();
extern void C2F(mopen)();
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,
                                long int nams_len));
extern void C2F(mclose) __PARAMS((integer *fd, double *res));
extern void sciprint __PARAMS((char *fmt,...));

static int id[nsiz];
static char fmtd[3]={'d','l','\000'};
static char fmti[3]={'i','l','\000'};
static char fmtl[3]={'l','l','\000'};
static char fmts[3]={'s','l','\000'};
static char fmtc[3]={'c','l','\000'};
static char fmtul[3]={'u','l','\000'};
static char fmtus[3]={'u','s','\000'};
static char fmtuc[3]={'u','c','\000'};

#ifdef hppa
#undef FILENAME_MAX
#define FILENAME_MAX 4096
#endif

/* work struct for that block */
typedef struct {
  int cnt;
  void *work;
  void *workt;
} towork_struct ;

void tows_c(scicos_block *block,int flag)
/* Copyright INRIA */
/* Put a typed vector in a scilab file.
 * Independant platform.
 *
 * Author A.Layec - initial rev. 18/09/07
 */
{
 /* for mopen */
 int fd;
 char *status;
 int swap = 1;
 double res;
 int out_n;
 long int lout;
 char filename[FILENAME_MAX];
 /* for name of file */
 char str[100];
 /* generic pointer */
 SCSREAL_COP *u_d,*u_cd,*ptr_d;
 SCSINT8_COP *u_c,*ptr_c;
 SCSUINT8_COP *u_uc, *ptr_uc;
 SCSINT16_COP *u_s,*ptr_s;
 SCSUINT16_COP *u_us,*ptr_us;
 SCSINT_COP *ptr_i;
 SCSINT32_COP *u_l,*ptr_l;
 SCSUINT32_COP *u_ul,*ptr_ul;
 /* the struct ptr of that block */
 towork_struct *ptr;
 /* */
 int nu,ut;
 int nz;
 double t;
 /* local */
 int i,j,k;
 int ierr;

 /* for path of TMPDIR/workspace */
 char env[256];
 char sep[2];
#ifdef _MSC_VER
  sep[0]='\\';
#else
  sep[0]='/';
#endif
  sep[1]='\0';

 /* retrieve param of that block */
 nu=GetInPortRows(block,1); /* number of rows of inputs*/
 ut=GetInType(block,1); /* input type */
 nz=block->ipar[0]; /* buffer size */

 if (flag==4) { /* init */

   /* begin campaign of allocations */
   if((*(block->work)=(towork_struct*) scicos_malloc(sizeof(towork_struct)))==NULL) {
    set_block_error(-16);
    return;
   }

   ptr = *(block->work);

   /*
    * t
    */
   if((ptr->workt=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                          nz*sizeof(double)))==NULL) {
    set_block_error(-16);
    scicos_free(ptr);
    *(block->work) = NULL;
    return;
   }
   ptr_i = (int*) ptr->workt;
   ptr_i[6] = 1;
   ptr_i[7] = nz;
   ptr_i[8] = 1;
   ptr_i[9] = 0;

   ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
   for (i=0;i<nz;i++) {
     ptr_d[i]=-1;
   }

   C2F(namstr)(id,(i=29,&i),(j=1,&j),(k=0,&k));

   ptr_i = (int*) ptr->workt;
   ptr_i[0]=id[0];
   ptr_i[1]=id[1];
   ptr_i[2]=id[2];
   ptr_i[3]=id[3];
   ptr_i[4]=id[4];
   ptr_i[5]=id[5];

   /*
    * x
    */
   switch (ut)
   {
    case SCSREAL_N    :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(double)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 1;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 0;
      ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_d[i]=0.;
      }
      break;

    case SCSCOMPLEX_N :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            2*nz*nu*sizeof(double)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 1;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 1;
      ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
      for (i=0;i<2*nu*nz;i++) {
       ptr_d[i]=0.;
      }
      break;

    case SCSINT8_N    :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(char)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 1;
      ptr_c = (SCSINT8_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_c[i]=0;
      }
      break;

    case SCSINT16_N   :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(short)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 2;
      ptr_s = (SCSINT16_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_s[i]=0;
      }
      break;

    case SCSINT32_N   :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(long)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 4;
      ptr_l = (SCSINT32_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_l[i]=0;
      }
      break;

    case SCSUINT8_N   :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(unsigned char)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 11;
      ptr_uc = (SCSUINT8_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_uc[i]=0;
      }
      break;

    case SCSUINT16_N  :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(unsigned short)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 12;
      ptr_us = (SCSUINT16_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_us[i]=0;
      }
      break;

    case SCSUINT32_N  :
      if((ptr->work=(void *) scicos_malloc(6*sizeof(int)+4*sizeof(int)+ \
                                            nz*nu*sizeof(unsigned long)))==NULL) {
       set_block_error(-16);
       scicos_free(ptr->workt);
       scicos_free(ptr);
       *(block->work) = NULL;
       return;
      }
      ptr_i = (int*) ptr->work;
      ptr_i[6] = 8;
      ptr_i[7] = nz;
      ptr_i[8] = nu;
      ptr_i[9] = 14;
      ptr_ul = (SCSUINT32_COP *) &(ptr_i[10]);
      for (i=0;i<nu*nz;i++) {
       ptr_ul[i]=0;
      }
      break;

    default  : /* Add a message here */
               break;
   }

   C2F(namstr)(id,(i=33,&i),(j=1,&j),(k=0,&k));

   ptr_i = (int*) ptr->work;
   ptr_i[0]=id[0];
   ptr_i[1]=id[1];
   ptr_i[2]=id[2];
   ptr_i[3]=id[3];
   ptr_i[4]=id[4];
   ptr_i[5]=id[5];

   /*
    * cnt
    */
   ptr->cnt=0;

 }

 else if (flag==5) { /* finish */

   ptr = *(block->work);

   if (ptr!=NULL) {
     /* */
     C2F(cvstr)(&(block->ipar[1]),&(block->ipar[2]),str,(j=1,&j), \
                 (unsigned long)strlen(str));
     str[block->ipar[1]] = '\0';

     /* retrieve path of TMPDIR/workspace */
     strcpy(env,getenv("TMPDIR"));
     strcat(env,sep);
     strcat(env,"Workspace");
     strcat(env,sep);
     strcat(env,str);

     /* open tmp file */
     status = "wb"; //** "w" : write
	                //** "b" : binary (required for Windows) 
     lout=FILENAME_MAX;
     C2F(cluni0)(env, filename, &out_n,1,lout);
     C2F(mopen)(&fd,env,status,&swap,&res,&ierr);
     /* a check must be done here on ierr */

     /* write x */
     ptr_i = (int*) ptr->work;
     C2F(mputnc) (&fd, &ptr_i[0], (j=nsiz,&j), fmti, &ierr);  /* write sci id */
     C2F(mputnc) (&fd, &ptr_i[6], (j=1,&j), fmti, &ierr);  /* write sci type */
     C2F(mputnc) (&fd, &ptr_i[7], (j=3,&j), fmti, &ierr);  /* write sci header */
     /* write data */
     switch (ut) {
      case SCSREAL_N    :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtd, &ierr);
        break;
      case SCSCOMPLEX_N :
         C2F(mputnc) (&fd, &ptr_i[10], (j=2*ptr_i[7]*ptr_i[8],&j), fmtd, &ierr);
        break;
      case SCSINT8_N    :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtc, &ierr);
        break;
      case SCSINT16_N   :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmts, &ierr);
        break;
      case SCSINT32_N   :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtl, &ierr);
        break;
      case SCSUINT8_N   :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtuc, &ierr);
        break;
      case SCSUINT16_N  :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtus, &ierr);
        break;
      case SCSUINT32_N  :
         C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7]*ptr_i[8],&j), fmtul, &ierr);
        break;
      default  : /* Add a message here */
                 break;
     }
     /* a check must be done here */

     /* write t */
     ptr_i = (int*) ptr->workt;
     C2F(mputnc) (&fd, &ptr_i[0], (j=nsiz,&j), fmti, &ierr);
     C2F(mputnc) (&fd, &ptr_i[6], (j=1,&j), fmti, &ierr);
     C2F(mputnc) (&fd, &ptr_i[7], (j=3,&j), fmti, &ierr);
     C2F(mputnc) (&fd, &ptr_i[10], (j=ptr_i[7],&j), fmtd, &ierr);
     /* a check must be done here on ierr */

     /* close tmp file */
     C2F(mclose)(&fd,&res);

     /* free */
     scicos_free(ptr->work);
     scicos_free(ptr->workt);
     scicos_free(ptr);
   }

 }

 else if (flag==2) { /* update state */

   ptr = *(block->work);

   ptr_i = (int*) ptr->work;

   /* check */
   if ((nz!=ptr_i[7]) || (nu!=ptr_i[8])) {
      sciprint("Size of buffer or input size have changed!\n");
      set_block_error(-1);
      return;
   }

   /*
    *store
    */

   /* x */
   switch (ut)
   {
    case SCSREAL_N    :
      u_d = GetRealInPortPtrs(block,1);
      ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_d[ptr->cnt+i*nz]=u_d[i];
      }
      break;

    case SCSCOMPLEX_N :
      u_d  = GetRealInPortPtrs(block,1);
      u_cd = GetImagInPortPtrs(block,1);
      ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_d[ptr->cnt+i*nz]=u_d[i];
       ptr_d[nz*nu+ptr->cnt+i*nz]=u_cd[i];
      }
      break;

    case SCSINT8_N    :
      u_c = Getint8InPortPtrs(block,1);
      ptr_c = (SCSINT8_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_c[ptr->cnt+i*nz]=u_c[i];
      }
      break;

    case SCSINT16_N   :
      u_s = Getint16InPortPtrs(block,1);
      ptr_s = (SCSINT16_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_s[ptr->cnt*nu+i*nz]=u_s[i];
      }
      break;

    case SCSINT32_N   :
      u_l = Getint32InPortPtrs(block,1);
      ptr_l = (SCSINT32_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_l[ptr->cnt+i*nz]=u_l[i];
      }
      break;

    case SCSUINT8_N   :
      u_uc = Getuint8InPortPtrs(block,1);
      ptr_uc = (SCSUINT8_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_uc[ptr->cnt+i*nz]=u_uc[i];
      }
      break;

    case SCSUINT16_N  :
      u_us = Getuint16InPortPtrs(block,1);
      ptr_us = (SCSUINT16_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_us[ptr->cnt+i*nz]=u_us[i];
      }
      break;

    case SCSUINT32_N  :
      u_ul = Getuint32InPortPtrs(block,1);
      ptr_ul = (SCSUINT32_COP *) &(ptr_i[10]);
      for (i=0;i<nu;i++) {
       ptr_ul[ptr->cnt+i*nz]=u_ul[i];
      }
      break;

    default  : /* Add a message here */
               break;
   }

   /* t */
   ptr_i = (int*) ptr->workt;
   t=get_scicos_time();

   ptr_d = (SCSREAL_COP *) &(ptr_i[10]);
   ptr_d[ptr->cnt]=t;

   /*
    * update cnt
    */
   ptr->cnt++;
   if (ptr->cnt==nz) ptr->cnt=0;
 }

}
