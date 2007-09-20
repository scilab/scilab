#include "scicos_block4.h"
/*    Masoud Najafi, Alan Layec September 2007 */
/*    Copyright INRIA
 *    Scicos block simulator
 *    From workspace block
 */
#include "../stack-c.h"
#include <stdio.h>
#include <string.h>
#include "../machine.h"

#if WIN32
#define NULL    0
#endif

#define Fnlength  block->ipar[0]
#define Order     block->ipar[1+Fnlength]
#define ZC        block->ipar[2+Fnlength]
#define OutEnd    block->ipar[3+Fnlength]
#define T0        ptr->workt[0]
#define TNm1      ptr->workt[nPoints-1]
#define TP        TNm1-T0

extern int C2F(cvstr) __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern int C2F(mgetnc)();
extern void C2F(mopen)();
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,
                                long int nams_len));
extern void C2F(mclose) __PARAMS((integer *fd, double *res));
extern void sciprint __PARAMS((char *fmt,...));

//int Myevalhermite(double *t, double *xa, double *xb, double *ya, double *yb, double *da, double *db, double *h, double *dh, double *ddh, double *dddh, int *i);

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
  int nPoints;
  int Yt;
  int Yst;
  int cnt1;
  int cnt2;
  int EVindex;
  int EVcnt;
  void *work;
  double *workt;
} fromwork_struct ;


void fromws_c(scicos_block *block,int flag)
{
  double t,a,b,c,y1,y2,t1,t2;
  int i,inow;
  double *y;
  double  d1,d2,h, dh, ddh, dddh;
  int fd;
  char *status;
  int swap = 1;
  double res;
  int out_n;
  long int lout;
  char filename[FILENAME_MAX];
  char str[100];
  int ierr;
  int Ytype, nPoints, mY, YsubType, my, ytype,j,jfirst;
  int Ydim[10];
  int cnt1, cnt2, EVindex, EVcnt;
  /* generic pointer */
  SCSREAL_COP *y_d,*y_cd,*ptr_d;
  SCSINT8_COP *y_c,*ptr_c;
  SCSUINT8_COP *y_uc, *ptr_uc;
  SCSINT16_COP *y_s,*ptr_s;
  SCSUINT16_COP *y_us,*ptr_us;
  SCSINT32_COP *y_l,*ptr_l;
  SCSUINT32_COP *y_ul,*ptr_ul;
 /* the struct ptr of that block */
  fromwork_struct *ptr;

 /* for path of TMPDIR/workspace */
  char env[256];
  char sep[2];
#ifdef _MSC_VER
  sep[0]='\\';
#else
  sep[0]='/';
#endif
  sep[1]='\0';

 my=GetOutPortRows(block,1); /* number of rows of Outputs*/

 if (flag==4){
   C2F(cvstr)(&(block->ipar[0]),&(block->ipar[1]),str,(j=1,&j), \
               (unsigned long)strlen(str));
   str[block->ipar[1]] = '\0';

   /* retrieve path of TMPDIR/workspace */
   strcpy(env,getenv("TMPDIR"));
   strcat(env,sep);
   strcat(env,"Workspace");
   strcat(env,sep);
   strcat(env,str);

   /* open tmp file */
   status = "r";
   lout=FILENAME_MAX;
   C2F(cluni0)(env, filename, &out_n,1,lout);
   C2F(mopen)(&fd,env,status,&swap,&res,&ierr);
   sciprint("ierr=%d",ierr);
   if (ierr!=0) {sciprint("The indicated variable does not exst"); set_block_error(-16);return;};

   /* read x */
   C2F(mgetnc) (&fd, &Ydim[0], (j=nsiz,&j), fmti, &ierr);  /* read sci id */
   C2F(mgetnc) (&fd, &Ydim[6], (j=1,&j), fmti, &ierr);  /* read sci type */
   C2F(mgetnc) (&fd, &Ydim[7], (j=3,&j), fmti, &ierr);  /* read sci header */

   Ytype=Ydim[6];
   nPoints=Ydim[7];
   mY=Ydim[8]; 
   YsubType=Ydim[9];
   ytype=GetOutType(block,1);     /* output type */
   sciprint("Ytype=%d ytype=%d ",Ytype,ytype);

   if (mY!=my) {sciprint("Data dimentions are incoherent"); set_block_error(-16);return;};

   if (Ytype==1) { /*real/complex cases*/
     switch (YsubType) {
     case 0: if (ytype!=10) {sciprint("Output should be of Real type"); set_block_error(-16);return;};break;
     case 1: if (ytype!=11) {sciprint("Output should be of copmlex type"); set_block_error(-16);return;};break;
     }
   }else if(Ytype==8) { /*integer cases*/
     switch (YsubType) {
     case 1: if (ytype!=81) {sciprint("Output should be of int8 type"); set_block_error(-16);return;};break;
     case 2: if (ytype!=82) {sciprint("Output should be of int16 type"); set_block_error(-16);return;};break;
     case 4: if (ytype!=84) {sciprint("Output should be of int32 type"); set_block_error(-16);return;};break;
     case 11:if (ytype!=811) {sciprint("Output should be of uint8 type"); set_block_error(-16);return;};break;
     case 12:if (ytype!=812) {sciprint("Output should be of uint16 type"); set_block_error(-16);return;};break;
     case 14:if (ytype!=814) {sciprint("Output should be of uint32 type"); set_block_error(-16);return;};break;
     }
   }
   if((*(block->work)=(fromwork_struct*) scicos_malloc(sizeof(fromwork_struct)))==NULL) {
     set_block_error(-16);
     return;
   }
   ptr = *(block->work);
  

   if (Ytype==1) { /*real/complex case*/
     switch (YsubType) {
     case 0 : // Real
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(double)))==NULL) {set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return; }
       ptr_d = (SCSREAL_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_d, (j=nPoints*mY,&j), fmtd, &ierr);  /* read double data */
       break;       
     case 1:  // complex
       if((ptr->work=(void *) scicos_malloc(2*nPoints*mY*sizeof(double)))==NULL) {set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return;  }
       ptr_d = (SCSREAL_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_d, (j=2*nPoints*mY,&j), fmtd, &ierr);  /* read double data */
       break;
     }       
   }else if(Ytype==8) { /*integer case*/
     switch (YsubType) {
     case 1 ://int8
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(char)))==NULL) {set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return; }
       ptr_c = (SCSINT8_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_c, (j=nPoints*mY,&j), fmtc, &ierr);  /* read char data */
       break;
     case 2 :  // int16
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(short)))==NULL) { set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return; }
       ptr_s = (SCSINT16_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_s, (j=nPoints*mY,&j), fmts, &ierr);  /* read short data */
       break;
     case 4 :   // int32
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(long)))==NULL) {set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return; }
       ptr_l = (SCSINT32_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_l, (j=nPoints*mY,&j), fmtl, &ierr);  /* read short data */
       break;
     case 11 :   // uint8
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(unsigned char)))==NULL) { set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return;   }
       ptr_uc = (SCSUINT8_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_uc, (j=nPoints*mY,&j), fmtuc, &ierr);  /* read short data */
       break;
     case 12 : // uint16
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(unsigned short)))==NULL) {set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return;  }
       ptr_us = (SCSUINT16_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_us, (j=nPoints*mY,&j), fmtus, &ierr);  /* read short data */
       break;
     case 14 :  // uint32
       if((ptr->work=(void *) scicos_malloc(nPoints*mY*sizeof(unsigned long)))==NULL) { set_block_error(-16);scicos_free(ptr);*(block->work)=NULL;return;    }
       ptr_ul = (SCSUINT32_COP *) ptr->work;
       C2F(mgetnc) (&fd, ptr_ul, (j=nPoints*mY,&j), fmtul, &ierr);  /* read short data */
       break;
     }
   }
   
   /* read t */
   C2F(mgetnc) (&fd, &Ydim[0], (j=nsiz,&j), fmti, &ierr);  /* read sci id */
   C2F(mgetnc) (&fd, &Ydim[6], (j=1,&j), fmti, &ierr);  /* read sci type */
   C2F(mgetnc) (&fd, &Ydim[7], (j=3,&j), fmti, &ierr);  /* read sci header */
   
   if (nPoints!=Ydim[7]) {
     sciprint("The size of the Time and Data vectors  are incoherent");
     set_block_error(-16);*(block->work)=NULL;
     scicos_free(ptr->work);
     scicos_free(ptr);
     return;
   };
   if ((Ydim[6]!=1)| (Ydim[9]|=0)) {
     sciprint("The Time vector is not double"); 
     set_block_error(-16);*(block->work)=NULL;
     scicos_free(ptr->work);
     scicos_free(ptr);
     return;
   };
   
   if((ptr->workt=(double *) scicos_malloc(nPoints*sizeof(double)))==NULL) {
     set_block_error(-16);*(block->work)=NULL;
     scicos_free(ptr->work);
     scicos_free(ptr);
     return;
   }
   ptr_d = (SCSREAL_COP *) ptr->workt;
   C2F(mgetnc) (&fd, ptr_d, (j=nPoints,&j), fmtd, &ierr);  /* read data of t */
   //================================
   cnt1=nPoints-1;
   cnt2=nPoints;
   for (i=0;i<nPoints;i++){ // finding the first positive time instant
     if (ptr->workt[i]>=0 ) {
       cnt1=i-1;
       cnt2=i;
       break;
     }
   }

   jfirst=nPoints; // finding first positive tie instant
   for (j=0;j<nPoints;j++){		
     if (ptr->workt[j]>0) {
       jfirst=j;
       break;
     }
   }
   
   ptr->nPoints=nPoints;
   ptr->Yt=Ytype;
   ptr->Yst=YsubType;
   ptr->cnt1=cnt1;
   ptr->cnt2=cnt2;
   ptr->EVindex=jfirst;
   ptr->EVcnt=0;
   return;   
   /******************** **********************************/
 }else if (flag==1){   /* event date computation */
   ptr = *(block->work);
   nPoints=ptr->nPoints;
   t=get_scicos_time();
   cnt1=ptr->cnt1;
   cnt2=ptr->cnt2;
   EVindex= ptr->EVindex;
   EVcnt=ptr->EVcnt;
   if (OutEnd==2) t-=(EVcnt-1)*TP;

   inow=nPoints-1;
   for (i=cnt1;i<nPoints;i++){		
     if (i==-1) continue;
     if (t<ptr->workt[i]) {
       inow=i-1;
       if (inow<cnt2){
	 cnt2=inow;
       }else{
	 cnt1=cnt2;
	 cnt2=inow;
       }
       break;
     }
   }
   sciprint("\n\r flag=2 inow=%d, nP=%d typ=%d, stype=%d",inow,nPoints,ptr->Yt,ptr->Yst);

   if (Order==0) {
     if (ptr->Yt==1){
       switch (ptr->Yst){
       case 0: // -------------double----------------------------
	 y_d = GetRealOutPortPtrs(block,1);
	 ptr_d=(double*) ptr->work;

	 for (j=0;j<my;j++){
	   if (inow<0){
	     y_d[j]=0.0;
	   }else if (inow>=nPoints-1){
	     if (OutEnd==0){
	       y_d[j]=0.0;// outputs set to zero
	     }else if (OutEnd==1){
	       y_d[j]=ptr_d[nPoints-1+(j)*nPoints]; // hold outputs at the end
	     }	     
	   }else{
	     y_d[j]=ptr_d[inow+(j)*nPoints];
	     sciprint(" yyy=%g ",y_d[j]);
	   }
	 }
	 break;
       case 1: // --------------Complex----------------------
	 y_d  = GetRealInPortPtrs(block,1);
	 y_cd = GetImagInPortPtrs(block,1);
	 ptr_d=(double*) ptr->work;
	 for (j=0;j<my;j++){
	   y_d[j]=ptr_d[inow+(j)*nPoints];
	   y_cd[j]=ptr_d[nPoints*my+inow+(j)*nPoints];
	 }
	 break;       
       }
     }else if (ptr->Yt==8){
       switch (ptr->Yst){
       case 0: // int8 char  
	 y_c = Getint8OutPortPtrs(block,1);
	 ptr_c=(char*) ptr->work;
	 for (j=0;j<my;j++){
	   y_c[j]=ptr_c[inow+(j)*nPoints];
	 }
	 break;
       case 2: // int16 short
	 y_s = Getint16OutPortPtrs(block,1);
	 ptr_s=(short*) ptr->work;
	 for (j=0;j<my;j++){
	   y_s[j]=ptr_s[inow+(j)*nPoints];
	 }
	 break;
       case 4: // int32 long
	 y_l = Getint32OutPortPtrs(block,1);
	 ptr_l=(long*) ptr->work;
	 for (j=0;j<my;j++){
	   y_l[j]=ptr_l[inow+(j)*nPoints];
	 }
	 break;
       case 11: // uint8 uchar
	 y_uc = Getuint8OutPortPtrs(block,1);
	 ptr_uc=(unsigned char*) ptr->work;
	 for (j=0;j<my;j++){
	   y_uc[j]=ptr_uc[inow+(j)*nPoints];
	 }
	 break;
       case 12: // uint16 ushort
	 y_us = Getuint16OutPortPtrs(block,1);
	 ptr_us=(unsigned short*) ptr->work;
	 for (j=0;j<my;j++){
	   y_us[j]=ptr_us[inow+(j)*nPoints];
	 }
	 break;
       case 14: // uint32 ulong
 	 y_ul = Getuint32OutPortPtrs(block,1);
	 ptr_ul=(unsigned long*) ptr->work;
	 for (j=0;j<my;j++){
	   y_ul[j]=ptr_ul[inow+(j)*nPoints];
	 }
	 break;
       }
     }
   }
     /*  if(Order==1) {
	 t1=ptr->workt[inow];
	 t2=ptr->workt[inow+1];
	 y1=ptr->work[inow];
	 y2=ptr->work[inow+1];
	 y[0]=(y2-y1)*(t-t1)/(t2-t1)+y1;
	 break;
	 }*/
   /********************************************************************/
 }else if(flag==3){   /* event date computation */
   t=get_scicos_time();
   ptr = *(block->work);
   nPoints=ptr->nPoints;
   cnt1=ptr->cnt1;
   cnt2=ptr->cnt2;
   EVindex= ptr->EVindex;
   EVcnt=ptr->EVcnt;

   if (ZC==1) {// generate Events only if ZC is active
     if ((Order==1)||(Order==0)){
       sciprint("\n\r flag=3 t=%g  EVindex=%d",t,EVindex);

       i=EVindex;
       if (i==nPoints-1){ 
	 if (OutEnd==2) {
	   i=0;
	   cnt1=-1;
	   cnt2=0;	   
	 }
       }
       if (i<nPoints-1 & i>0) {
	 block->evout[0]=ptr->workt[i+1]-ptr->workt[i];	 
	 EVindex=i+1;
       }

       if (i==0) {
	 block->evout[0]=ptr->workt[i];	 
	 EVindex=i+1;
       }
 
       if (EVindex==1)  EVcnt++;/* When OutEnd==2 (perodic output)*/

     }

     ptr->cnt1=cnt1;
     ptr->cnt2=cnt2;
     ptr->EVindex=EVindex;
     ptr->EVcnt=EVcnt;
   }
   /***********************************************************************/
 }else if (flag==5){   /* finish */
   ptr = *(block->work);
   if (ptr!=NULL) {
     if (ptr->work!=NULL) {
       scicos_free(ptr->work);
     }
     if (ptr->workt!=NULL) {
       scicos_free(ptr->workt);
     }
     scicos_free(ptr);
   }
 }
 /*************************************************************************/
}

