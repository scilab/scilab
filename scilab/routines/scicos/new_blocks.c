#include "scicos_block.h"
#include <math.h>
#include <stdlib.h>
#include "../machine.h"

void * scicos_malloc(size_t );
void scicos_free(void *p);

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

extern int s_copy();
extern int s_cmp();

void  mswitch(scicos_block *block,int flag)
{
  int i,j;
  i=block->ipar[1];
  if (i==0) {
    if (*block->inptr[0]>0) {
      j=floor(*block->inptr[0]);
    }else{
      j=ceil(*block->inptr[0]);
    }
  }else if (i==1) {
    if (*block->inptr[0]>0) {
      j=floor(*block->inptr[0]+.5);
    }else{
      j=ceil(*block->inptr[0]-.5);
    }
  }else if (i==2) {
    j=ceil(*block->inptr[0]);
  }else if (i==3) {
    j=floor(*block->inptr[0]);
  }
  j=j+1-block->ipar[0];
  j=max(j,1);
  j=min(j,block->nin-1);
  for (i=0;i<block->insz[j];i++) {
    block->outptr[0][i]=block->inptr[j][i];
  }
}


void logicalop(scicos_block *block,int flag)
{
  int i,j,k,l;
  i=block->ipar[0];
  switch (i)
    {
    case 0:
      if (block->nin==1)
	{
	  block->outptr[0][0]=1.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]<=0)
	      {
		block->outptr[0][0]=0.0;
		break;
	      }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=1.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]<=0) {
	      block->outptr[0][j]=0.0;
	      break;
	    }
	  }
	}
      }
      break;
      
    case 1:
      if (block->nin==1)
	{
	  block->outptr[0][0]=0.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      block->outptr[0][0]=1.0;
	      break;
	    }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=0.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      block->outptr[0][j]=1.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 2:
      if (block->nin==1)
	{
	  block->outptr[0][0]=0.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]<=0)
	      {
		block->outptr[0][0]=1.0;
		break;
	      }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=0.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]<=0) {
	      block->outptr[0][j]=1.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 3:
      if (block->nin==1)
	{
	  block->outptr[0][0]=1.0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      block->outptr[0][0]=0.0;
	      break;
	    }
	  }
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  block->outptr[0][j]=1.0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      block->outptr[0][j]=0.0;
	      break;
	    }
	  }
	}
      }
      break;

    case 4:
      if (block->nin==1)
	{
	  l=0;
	  for (j=0;j<block->insz[0];j++) {
	    if(block->inptr[0][j]>0) {
	      l=(l+1)%2;
	    }
	  }
	  block->outptr[0][0]=(double) l;
	}
      else {
	for (j=0;j<block->insz[0];j++) {
	  l=0;
	  for (k=0;k<block->nin;k++) {
	    if(block->inptr[k][j]>0) {
	      l=(l+1)%2;
	    }
	  }
	  block->outptr[0][j]=(double) l;
	}
      }
      break;

    case 5:
      for (j=0;j<block->insz[0];j++) {
	if(block->inptr[0][j]>0) {
	  block->outptr[0][j]=0.0;
	}
	else{
	  block->outptr[0][j]=1.0;
	}
      }
    }
}



void  switch2(scicos_block *block,int flag)
{
  int i,j,phase;
  if (flag == 1) {
    phase=get_phase_simulation();
    if (phase==1){
      i=2;
      if (*block->ipar==0){
	if (*block->inptr[1]>=*block->rpar) i=0;
      }else if (*block->ipar==1){
	if (*block->inptr[1]>*block->rpar) i=0;
      }else {
	if (*block->inptr[1]!=*block->rpar) i=0;			     
      }
    }else{
      if(block->mode[0]==1){
	i=0;
      }else if(block->mode[0]==2){
	i=2;
      }
    }
    for (j=0;j<block->insz[0];j++) {
      block->outptr[0][j]=block->inptr[i][j];
    }
  }else if(flag == 9){
    block->g[0]=*block->inptr[1]-(*block->rpar);
    if (phase==1){
      i=2;
      if (*block->ipar==0){
	if (block->g[0]>=0.0) i=0;
      }else if (*block->ipar==1){
	if (block->g[0]>0.0) i=0;
      }else {
	if (block->g[0]!=0.0) i=0;			     
      }
      if(i==0) {
	block->mode[0]=1;
      }else{
	block->mode[0]=2;
      }
    }
  }
}


void  hystheresis(scicos_block *block,int flag)
{
  if (flag==1){
    if (get_phase_simulation()==1) {
      if (*block->inptr[0]>=block->rpar[0]){
	block->outptr[0][0]=block->rpar[2];
      }else if (*block->inptr[0]<=block->rpar[1]){
	block->outptr[0][0]=block->rpar[3];
      }
    }else{
      if (block->mode[0]<2){
	block->outptr[0][0]=block->rpar[3];
      }else{
	block->outptr[0][0]=block->rpar[2];
      }
    } 
  } else if (flag==9){
    block->g[0]=*block->inptr[0]-(block->rpar[0]);
    block->g[1]=*block->inptr[0]-(block->rpar[1]);
    if (get_phase_simulation()==1) {
      if (block->g[0]>=0){
	block->mode[0]=2;
      }else if (block->g[1]<=0){
	block->mode[0]=1;
      }
    }
  }
}


void  thermo(scicos_block *block,int flag)
{
  double* pw;
  if (flag == 4){/* the workspace is used to store a discrete float */
    if ((*block->work=scicos_malloc(sizeof(double)))== NULL ) {
      set_block_error(-16);
      return;
    }
    pw=*block->work; 
    pw[0]=-1;
  }else  if (flag == 5){
    scicos_free(*block->work);
  } else if (flag==1){
    block->outptr[0][0]=block->x[0];
  }else if (flag==9){
    block->g[0]=block->x[0];
    block->g[1]=block->x[0]-1;
  }else if (flag==0){
    pw=*block->work;
    if( pw[0]<0){
      block->res[0]=block->xd[0]-1;
    }else{
      block->res[0]=block->xd[0]+1;
    }
  }else if (flag==2 && block->nevprt<0){
    pw=*block->work;
    if (block->jroot[0]<0){  /* this means the first
				zero-crossing has crossed
				from + to - */
      pw[0]=-1;
    }
    if (block->jroot[1]>0){ /* this means the second
				zero-crossing has crossed
				from - to + */
      pw[0]=1;
    }
  } 
}

void * scicos_malloc(size_t size)
{
  return malloc(size);
}


void scicos_free(void *p)
{
  return free(p);
}

void variable_delay(scicos_block *block,int flag)
{/*  rpar[0]=max delay, rpar[1]=init value, ipar[0]=buffer length */
  double* pw,del,t,td;
  int* iw;
  int i,j,k;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=
	 scicos_malloc(sizeof(int)+sizeof(double)* 
		       block->ipar[0]*(1+block->insz[0])))== NULL ) {
      set_block_error(-16);
      return;
    }
    pw=*block->work; 
    pw[0]=-block->rpar[0]*block->ipar[0];
    for(i=1;i< block->ipar[0];i++){
      pw[i]=pw[i-1]+block->rpar[0];
      for(j=1;j< block->insz[0]+1;j++){
	pw[i+block->ipar[0]*j]=block->rpar[1];
      }
    }
    iw=pw+block->ipar[0]*(1+block->insz[0]);
    *iw=0;
  }else  if (flag == 5){
    scicos_free(*block->work);
  } else if (flag==1) {
    if (get_phase_simulation()==1) do_cold_restart();
    pw=*block->work; 
    iw=pw+block->ipar[0]*(1+block->insz[0]);
    t=get_scicos_time();
    del=min(max(0,block->inptr[1][0]),block->rpar[0]);
    td=t-del;
    if(td<pw[*iw]){
      sciprint("delayed time=%f but last stored time=%f \r\n", td, pw[*iw]);
      sciprint("Conisder increasing the length of buffer in variable delay block\r\n");
    }
    if (t>pw[(block->ipar[0]+*iw-1)%block->ipar[0]]){
      for(j=1;j< block->insz[0]+1;j++){
	pw[*iw +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[*iw]=t;
      *iw=(*iw+1)%block->ipar[0];
    }else{
      for(j=1;j< block->insz[0]+1;j++){
	pw[(block->ipar[0]+*iw-1)%block->ipar[0] +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[(block->ipar[0]+*iw-1)%block->ipar[0]]=t;
    }

    i=0;j= block->ipar[0]-1;

    while (j-i>1) {
      k=(i+j)/2;
      if (td<pw[(k+*iw)%block->ipar[0]]) {
	j=k;
      }else if (td>pw[(k+*iw)%block->ipar[0]]) {
	i=k;
      }else{
	i=k;
	j=k;
	break;
      }
    }
    i=(i+*iw)%block->ipar[0];
    j=(j+*iw)%block->ipar[0];
    del=pw[j]-pw[i];
    if(del!=0.0){
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=((pw[j]-td)*pw[i+block->ipar[0]*k] +
			       (td-pw[i])*pw[j+block->ipar[0]*k])/del;
      }
    }else{
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=pw[i+block->ipar[0]*k];
      }
    }
  }
}

void time_delay(scicos_block *block,int flag)
{/*  rpar[0]=delay, rpar[1]=init value, ipar[0]=buffer length */
  double* pw,del,t,td;
  int* iw;
  int i,j,k;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=
	 scicos_malloc(sizeof(int)+sizeof(double)* 
		       block->ipar[0]*(1+block->insz[0])))== NULL ) {
      set_block_error(-16);
      return;
    }
    pw=*block->work; 
    pw[0]=-block->rpar[0]*block->ipar[0];
    for(i=1;i< block->ipar[0];i++){
      pw[i]=pw[i-1]+block->rpar[0];
      for(j=1;j< block->insz[0]+1;j++){
	pw[i+block->ipar[0]*j]=block->rpar[1];
      }
    }
    iw=pw+block->ipar[0]*(1+block->insz[0]);
    *iw=0;
  }else  if (flag == 5){
    scicos_free(*block->work);
  } else if (flag==0||flag==2) {
    if (flag==2) do_cold_restart();
    pw=*block->work; 
    iw=pw+block->ipar[0]*(1+block->insz[0]);
    t=get_scicos_time();
    td=t-block->rpar[0];
    if(td<pw[*iw]){
      sciprint("delayed time=%f but last stored time=%f \r\n", td, pw[*iw]);
      sciprint("Conisder increasing the length of buffer in delay block \r\n");
    }

    if (t>pw[(block->ipar[0]+*iw-1)%block->ipar[0]]){
      for(j=1;j< block->insz[0]+1;j++){
	pw[*iw +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[*iw]=t;
      /*sciprint("**time is %f. I put %f, in %d \r\n", t,block->inptr[0][0],*iw);*/
      *iw=(*iw+1)%block->ipar[0];
 
    }else{
      for(j=1;j< block->insz[0]+1;j++){
	pw[(block->ipar[0]+*iw-1)%block->ipar[0] +block->ipar[0]*j]=block->inptr[0][j-1];
      }
      pw[(block->ipar[0]+*iw-1)%block->ipar[0]]=t;
      /*sciprint("**time is %f. I put %f, in %d \r\n", t,block->inptr[0][0],*iw);*/

    }

  } else if (flag==1) {
    pw=*block->work; 
    iw=pw+block->ipar[0]*(1+block->insz[0]);
    t=get_scicos_time();
    td=t-block->rpar[0];

    i=0;j= block->ipar[0]-1;

    while (j-i>1) {
      k=(i+j)/2;
      if (td<pw[(k+*iw)%block->ipar[0]]) {
	j=k;
      }else if (td>pw[(k+*iw)%block->ipar[0]]) {
	i=k;
      }else{
	i=k;
	j=k;
	break;
      }
    }
    i=(i+*iw)%block->ipar[0];
    j=(j+*iw)%block->ipar[0];
    del=pw[j]-pw[i];
    /*sciprint("time is %f. interpolating %d and %d, i.e. %f, %f\r\n", t,i,j,pw[i],pw[j]);*/
    if(del!=0.0){
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=((pw[j]-td)*pw[i+block->ipar[0]*k] +
			       (td-pw[i])*pw[j+block->ipar[0]*k])/del;
      }
    }else{
      for (k=1;k<block->insz[0]+1;k++){
	block->outptr[0][k-1]=pw[i+block->ipar[0]*k];
      }
    }
  }
}


static int c__1 = 1;
static int c__0 = 0;
static int c_n1 = -1;
static int c__4 = 4;
static int c__21 = 21;
static int c__3 = 3;
static double c_b84 = 0.;

void cscope(scicos_block *block,int flag)
{
  double t;
  double *z__;
  double *rpar;
  int *ipar, nipar,nu,kfun;
  
 
  /* Initialized data */
  
  static double frect[4] = { 0.,0.,1.,1. };
  static int cur = 0;
  static int verb = 0;
  
  int i__1,i;
  
  static double rect[4];
  extern int C2F(getlabel)(), C2F(dset)();
  static double ymin, ymax;
  static char strf[40];
  static int i__, k, n, v;
  static double tsave;
  static int n1, n2;
  extern int C2F(plot2d)(), C2F(setscale2d)();
  static int na;
  extern int C2F(dr)();
  static double dt, dv;
  static int nxname;
  extern int C2F(sciwin)(), C2F(dr1)(), C2F(scicosclip)();
  static char buf[40];
  static int wid, iwd;
  static double per;
  static int nax[4], iwp;
  
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = 0/1 color flag */
  /*     ipar(3) = buffer size */
  /*     ipar(4:11) = line type for ith curve */
  
  /*     ipar(12:13) : window position */
  /*     ipar(14:15) : window dimension */
  
  /*     rpar(1)=dt */
  /*     rpar(2)=ymin */
  /*     rpar(3)=ymax */
  /*     rpar(4)=periode */
  
   nu=block->insz[0];
  if (nu>8) {nu=8;}
  rpar=block->rpar;
  ipar=block->ipar;
  nipar=block->nipar;
  t=get_scicos_time();
  
  /*      character*(4) logf */
  /* Parameter adjustments */
  --ipar;
  --rpar;
  
  iwp = nipar - 3;
  iwd = nipar - 1;

  wid = ipar[1];
  if(wid==-1){
    wid=20000+get_block_number();
  }
  
  if (flag == 2) {
    z__=*block->work; 
    --z__;
    dt = rpar[1];
    ymin = rpar[2];
    ymax = rpar[3];
    per = rpar[4];
    n = ipar[3];
    k = (int) z__[1];
    if (k > 0) {
      n1 = (int) (z__[k + 1] / per);
      if (z__[k + 1] < 0.) {
	--n1;
      }
    } else {
      n1 = 0;
    }
    
    tsave = t;
    if (dt > 0.) {
      t = z__[k + 1] + dt;
    }
    
    n2 = (int) (t / per);
    if (t < 0.) {
      --n2;
    }
    
    /*     add new point to the buffer */
    ++k;
    z__[k + 1] = t;
    for (i = 0; i < nu; ++i) {
      z__[n + 1 + i * n + k] = block->inptr[0][i];
      /* L1: */
    }
    z__[1] = (double) k;
    if (n1 == n2 && k < n) {
      t = tsave;
      return ;
    }
    
    /*     plot 1:K points of the buffer */
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    if (k > 0) {
      C2F(scicosclip)(&c__1);
      for (i__ = 0; i__ < nu; ++i__) {
	C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[i__ + 4], &c__1, &k,
		 &v, &z__[2], &z__[n + 2 + i__  * n], &dv, &dv);
      }
      C2F(scicosclip)(&c__0);
    }
    /*     shift buffer left */
    z__[2] = z__[k + 1];
    for (i__ = 0; i__ < nu; ++i__) {
      z__[n + 1 + i__ * n + 1] = z__[n + 1 + i__ * n + k];
    }
    z__[1] = 1.;
	if (n1 != n2) {
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	    nax[2] = 2;
	    nax[3] = 10;
	    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv);
	    C2F(dr1)("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0, &
		     c__0, &v, &dv, &dv, &dv, &dv);
	    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
		    dv, &dv);
	    s_copy(buf, "t@ @input and output\000",40,21);
	    s_copy(strf, "011\000",40,4);
	    rect[0] = per * (n1 + 1);
	    rect[1] = ymin;
	    rect[2] = per * (n1 + 2);
	    rect[3] = ymax;
	    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, 
		     &v, &dv, &dv, &dv, &dv);
	    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect, nax,
			&c__4, &c__21);
	}
	t = tsave;
	
  } else if (flag == 4) {/* the workspace is used to store buffer 
			     */
    if ((*block->work=
	 scicos_malloc(sizeof(double)*(1+ipar[3]*(1+nu))))== NULL ) {
      set_block_error(-16);
      return;
    }
    z__=*block->work; 
    --z__;
    z__[1]=-1.0;


    n = ipar[3];
    ymin = rpar[2];
    ymax = rpar[3];
    per = rpar[4];
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 2;
    nax[3] = 10;
    n1 = (int) (t / per);
    if (t <= 0.) {
      --n1;
    }
    C2F(sciwin)();
    C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
	     dv, &dv);
    C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    if (ipar[iwp] >= 0) {
	    C2F(dr1)("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v, &v, &
		     v, &v, &dv, &dv, &dv, &dv);
    }
    if (ipar[iwd] >= 0) {
      C2F(dr1)("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
      /*     to force dimensions update */
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    rect[0] = per * (n1 + 1);
    rect[1] = ymin;
    rect[2] = per * (n1 + 2);
    rect[3] = ymax;
    C2F(setscale2d)(frect, rect, "nn\000");
    C2F(dr1)("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0, &
	     c__0, &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    s_copy(buf, "t@ @input and output\000",40,21);
    s_copy(strf, "011\000",40,4);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect, nax, &
		c__4, &c__21);
    C2F(scicosclip)(&c__1);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    s_copy(buf + i__1, "\000", nxname + 1 - i__1,1);
    if ((nxname == 1 && *(unsigned char *)buf == ' ') || (nxname == 0)) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    z__[1] = 0.;
    z__[2] = t;
    i__1 = nu * n;
    C2F(dset)(&i__1, &c_b84, &z__[3], &c__1);
  } else if (flag == 5) {
    z__=*block->work; 
    --z__;
    n = ipar[3];
    k = (int) z__[1];
    if (k <= 1) {
      return ;
    }
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(scicosclip)(&c__1);
    for (i__ = 0; i__ < nu; ++i__) {
      C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[i__ + 4], &c__1, &k, &v,
	       &z__[2], &z__[n + 2 + i__ * n], &dv, &dv);
    }
    C2F(scicosclip)(&c__0);
    scicos_free(*block->work);
  }
}


static double c_b103 = 0.;

void cmscope(scicos_block *block,int flag)
{
  double t;
  double *z__;
  double *rpar;
  int *ipar, nipar,nu;
  


  static int cur = 0;
  static int verb = 0;
  
  int i__1, i__2, i__3,nwid,kk,i,j,sum;
  
  int kfun;  
  
  static char name__[4];
  static double rect[4];
  static int kwid;
  extern int C2F(getlabel)();
  static int nfwid;
  extern int C2F(dset)();
  static char strf[40];
  static int i__, k, n, v;
  static double frect[4], tsave;
  static int n1, n2;
  extern int C2F(plot2d)(), C2F(setscale2d)();
  static int na;
  extern int C2F(dr)();
  static double dt, dv;
  static int it, nxname;
  extern int C2F(sciwin)(), C2F(dr1)(), C2F(scicosclip)();
  static char buf[40];
  static int wid, iwd;
  static double per;
  static int nax[4], ilt, iwp;

  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = number of subwindows (input ports) */
  /*     ipar(3) = buffer size */
  /*     ipar(4:5) : window position */
  /*     ipar(6:7) : window dimension */
  /*     ipar(8:7+ipar(2)) = input port sizes */
  /*     ipar(8+ipar(2):7+ipar(2)+nu) = line type for ith curve */
  /*     rpar(1)=dt */
  /*     rpar(2)=periode */
  /*     rpar(3)=ymin_1 */
  /*     rpar(4)=ymax_1 */
  /*     ... */
  /*     rpar(2*k+1)=ymin_k */
  /*     rpar(2*k+2)=ymax_k */

  nu=block->insz[0];
  rpar=block->rpar;
  ipar=block->ipar;
  nipar=block->nipar;
  t=get_scicos_time();

  --ipar;
  --rpar;

  wid = ipar[1];
  if(wid==-1){
    wid=20000+get_block_number();
  }
  nwid = ipar[2];
  n = ipar[3];
  per = rpar[2];
  dt = rpar[1];

  if (flag == 2) {
    z__=*block->work; 
    --z__;
    k = (int) z__[1];
    if (k > 0) {
      n1 = (int) (z__[k + 1] / per);
      if (z__[k + 1] < 0.) {
	--n1;
      }
    } else {
      n1 = 0;
    }

    tsave = t;
    if (dt > 0.) {
      t = z__[k + 1] + dt;
    }

    n2 = (int) (t / per);
    if (t < 0.) {
      --n2;
    }

    /*     add new point to the buffer */
    ++k;
    z__[k + 1] = t;
    kk=0;
    for (i=0;i<block->nin;++i){
      for (j = 0; j <block->insz[i] ; ++j) {
	z__[n + 1 + kk * n + k] =block->inptr[i][j] ;
	++kk;
      }
    }
    z__[1] = (double) k;
    if (n1 == n2 && k < n) {
      t = tsave;
      return ;
    }

    /*     plot 1:K points of the buffer */
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    ilt = ipar[2] + 8;
    it = 0;
    /*     loop on input ports */
    if (k > 0) {
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid) {
	s_copy(buf, "xlines\000", 40, 7);
	rect[0] = per * n1;
	rect[1] = rpar[(kwid << 1) + 1];
	rect[2] = per * (n1 + 1);
	rect[3] = rpar[(kwid << 1) + 2];
	frect[0] = 0.;
	frect[1] = (kwid - 1) * (1. / nwid);
	frect[2] = 1.;
	frect[3] = 1. / nwid;
	C2F(setscale2d)(frect, rect, "nn\000");
	C2F(scicosclip)(&c__1);
	/*     loop on input port elements */
	i__2 = ipar[kwid + 7];
	for (i__ = 1; i__ <= i__2; ++i__) {
	  C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[ilt + it], &
		   c__1, &k, &v, &z__[2], &z__[n + 2 + it * n], &dv, 
		   &dv);
	  ++it;
	}
	C2F(scicosclip)(&c__0);
      }
    }
    /*     shift buffer left */
    z__[2] = z__[k + 1];
    sum=0;
    for (i=0;i<block->nin;++i){
      sum=sum+block->insz[i];
    }    i__1 = sum;
    for (i__ = 1; i__ <= i__1; ++i__) {
      z__[n + 1 + (i__ - 1) * n + 1] = z__[n + 1 + (i__ - 1) * n + k];
    }
    z__[1] = 1.;
    if (n1 != n2) {
      /*     clear window */
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv);
      C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &
	       c__0, &v, &dv, &dv, &dv, &dv);
      C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
	      dv, &dv);
      s_copy(strf, "011\000", 40, 4);
      C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, 
	       &v, &dv, &dv, &dv, &dv);
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid) {
	rect[0] = per * (n1 + 1);
	rect[1] = rpar[(kwid << 1) + 1];
	rect[2] = per * (n1 + 2);
	rect[3] = rpar[(kwid << 1) + 2];
	frect[0] = 0.;
	frect[1] = (kwid - 1) * (1. / nwid);
	frect[2] = 1.;
	frect[3] = 1. / nwid;
	C2F(setscale2d)(frect, rect, "nn\000");
	C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect, 
		    nax);
      }
    }
    t = tsave;

  } else if (flag == 4) {
    sum=0;
    for (i=0;i<block->nin;++i){
      sum=sum+block->insz[i];
    }
    if ((*block->work=
	 scicos_malloc(sizeof(double)*(1+ipar[3]*(1+sum))))== NULL ) {
      set_block_error(-16);
      return;
    }
    z__=*block->work; 
    --z__;
    z__[1]=-1.0;
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 2;
    nax[3] = 10;
    n1 = (int) (t / per);
    if (t <= 0.) {
      --n1;
    }
    C2F(sciwin)();
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    iwp = 4;
    if (ipar[iwp] >= 0) {
      C2F(dr1)("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
    }
    iwd = 6;
    if (ipar[iwd] >= 0) {
      C2F(dr1)("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
    }
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    s_copy(strf, "011\000", 40, 4);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    s_copy(buf + i__1, "\000", nxname + 1 - i__1, 1);
    if ((nxname == 1 && *(unsigned char *)buf == ' ') || nxname == 0) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    i__1 = nwid;
    for (kwid = 1; kwid <= i__1; ++kwid) {
      rect[0] = per * (n1 + 1);
      rect[1] = rpar[(kwid << 1) + 1];
      rect[2] = per * (n1 + 2);
      rect[3] = rpar[(kwid << 1) + 2];
      frect[0] = 0.;
      frect[1] = (kwid - 1) * (1. / nwid);
      frect[2] = 1.;
      frect[3] = 1. / nwid;
      C2F(setscale2d)(frect, rect, "nn\000");
      C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect, nax);
    }
    
    z__[1] = 0.;
    z__[2] = t;
    i__1 = sum * n;
    C2F(dset)(&i__1, &c_b103, &z__[3], &c__1);
  } else if (flag == 5) {
    z__=*block->work; 
    --z__;
    k = (int) z__[1];
    if (k <= 1) {
      return ;
    }
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    
    ilt = ipar[2] + 8;
    it = 0;
    n1 = (int) (t / per);
    if (t <= 0.) {
      --n1;
    }
    /*     loop on input ports */
    i__1 = nwid;
    for (kwid = 1; kwid <= i__1; ++kwid) {
      rect[0] = per * (n1 + 1);
      rect[1] = rpar[(kwid << 1) + 1];
      rect[2] = per * (n1 + 2);
      rect[3] = rpar[(kwid << 1) + 2];
      frect[0] = 0.;
      frect[1] = (kwid - 1) * (1. / nwid);
      frect[2] = 1.;
      frect[3] = 1. / nwid;
      F2C(setscale2d)(frect, rect, "nn\000");
      F2C(scicosclip)(&c__1);
      /*     loop on input port elements */
      i__2 = ipar[kwid + 7];
      for (i__ = 1; i__ <= i__2; ++i__) {
	i__3 = k - 1;
	C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[ilt + it], &c__1, &
		 i__3, &v, &z__[2], &z__[n + 2 + it * n], &dv, &dv);
	++it;
      }
      C2F(scicosclip)(&c__0);
    }
  scicos_free(*block->work);
  }
} 


void  satur(scicos_block *block,int flag)
{/* rpar[0]:upper limit,  rpar[1]:lower limit */
  if (flag==1){
    if (get_phase_simulation()==1) {
      if (*block->inptr[0]>=block->rpar[0]){
	block->outptr[0][0]=block->rpar[0];
      }else if (*block->inptr[0]<=block->rpar[1]){
	block->outptr[0][0]=block->rpar[1];
      }else {
	block->outptr[0][0]=block->inptr[0][0];
      }
    }else{
      if (block->mode[0]==1){
	block->outptr[0][0]=block->rpar[0];
      }else if(block->mode[0]==2) {
	block->outptr[0][0]=block->rpar[1];
      }else{
	block->outptr[0][0]=block->inptr[0][0];
      } 
    }
  } else if (flag==9){
    block->g[0]=*block->inptr[0]-(block->rpar[0]);
    block->g[1]=*block->inptr[0]-(block->rpar[1]);
    if (get_phase_simulation()==1) {
      if (block->g[0]>=0){
	block->mode[0]=1;
      }else if (block->g[1]<=0){
	block->mode[0]=2;
      }else {
	block->mode[0]=3;
      }
    }
  }
}


void  step_func(scicos_block *block,int flag)
{
  int i;
  if (flag==1&&block->nevprt==1) {
    for (i=0;i<block->outsz[0];++i) {
      block->outptr[0][i]=block->rpar[block->outsz[0]+i];
    } 
  }else if (flag==4) {
    for(i=0;i<block->outsz[0];++i) {
      block->outptr[0][i]=block->rpar[i];
    }
  }
}


void  integral_func(scicos_block *block,int flag)
{int i;
  if (flag==0){
    if(block->ng>0){
      for(i=0;i<block->nx;++i) {
	if(block->mode[i]==3){
	  block->xd[i]=block->inptr[0][i];
	}else{
	  block->xd[i]=0.0;
	}
      }
    }else{
      for(i=0;i<block->nx;++i) {
	block->xd[i]=block->inptr[0][i];
      }
    }
  }else if (flag==1){
    for(i=0;i<block->nx;++i) {
      block->outptr[0][i]=block->x[i];
    }
  }else if (flag==2&&block->nevprt==1){
    for(i=0;i<block->nx;++i) {
      block->x[i]=block->inptr[1][i];
    }
  } else if (flag==9){
    for(i=0;i<block->nx;++i) {
      if (block->mode[i]==3){
	block->g[i]=(block->x[i]-(block->rpar[0]))*(block->x[i]-(block->rpar[1]));
      } else {
	block->g[i]=block->inptr[0][i];
      } 
      if (get_phase_simulation()==1) {
	if (block->inptr[0][i]>=0&&block->x[i]>=block->rpar[0]){
	  block->mode[i]=1;
	}else if (block->inptr[0][i]<=0&&block->x[i]<=block->rpar[1]){
	  block->mode[i]=2;
	}else {
	  block->mode[i]=3;
	}
      }
    }
  }
}

void  absolute_value(scicos_block *block,int flag)
{
  int i,j;
  if (flag==1){
    if( block->ng>0){
      for(i=0;i<block->insz[0];++i){
	if (get_phase_simulation()==1) {
	  if (block->inptr[0][i]<0){
	    j=2;
	  } else{
	    j=1;
	  }
	}else {
	  j=block->mode[i];
	}
	if (j==1){
	  block->outptr[0][i]=block->inptr[0][i];
	} else{
	  block->outptr[0][i]=-block->inptr[0][i];
	}
      }
    }else{
      for(i=0;i<block->insz[0];++i){
	if (block->inptr[0][i]<0){
	  block->outptr[0][i]=-block->inptr[0][i];
	}else{
	  block->outptr[0][i]=block->inptr[0][i];
	}
      }
    }
  }else if (flag==9){
    for(i=0;i<block->insz[0];++i){
      block->g[i]=block->inptr[0][i];
      if (get_phase_simulation()==1) {
	if(block->g[i]<0){
	  block->mode[i]=2;
	}else{
	  block->mode[i]=1;
	}
      }
    }
  }
}


void 
bounce_ball(scicos_block *block,int flag)
{

     int nevprt,nx,*ipar;
     int *outsz;
     double *x,*xd,*rpar;
     double *g;
     int ng;
     int *jroot;
     
     
  int i1;
  double d1, d2, d3;
  
  static double a, b, c;
  static int i, j, k, n;
  static double v, s1, s2, s3, s4, xsi,*y1,*y2;
  
  /*     Scicos block simulator */
  /*     bouncing ball */
  /*     rpar(i): mass of ball i */
  /*     rpar(i+n): radius of ball i */
  /*     rpar(2n+1:2n+4); [xmin,xmax,ymin,ymax] */
  /*     x: [x1,x1',y1,y1',x2,x2',y2,y2',...,yn'] */
  /*     n:number of ball=ny1=ny2 */
  /*     y1: x-coord des balles */
  /*     y2: y-coord des balles */
  /*     ipar: storage de taille [nx(n-1)/2=ng]*2 */
  nevprt=block->nevprt;
  nx=block->nx;
  ipar=block->ipar;
  outsz=block->outsz;
  x=block->x;
  xd=block->xd;
  rpar=block->rpar;

  g=block->g;
  ng=block->ng;
  jroot=block->jroot;
  /* Parameter adjustments to index vectors as in Scilab (fortran)*/
  --g;
  --ipar;
  --rpar;
  --x;
  --xd;
  y1=block->outptr[0];
  y2=block->outptr[1];
  --y2;
  --y1;
  --jroot;
  
  n = outsz[0];
  if (flag == 0) {
    c = rpar[(n << 1) + 6];
    i1 = n;
    for (i = 1; i <= i1; ++i) {
      xd[((i - 1) << 2) + 1] = x[((i - 1) << 2) + 2];
      xd[((i - 1) << 2) + 3] = x[((i - 1) << 2) + 4];
      xd[((i - 1) << 2) + 2] = -c * x[((i - 1) << 2) + 2];
      xd[((i - 1) << 2) + 4] = -rpar[(n << 1) + 5] ;
    }
    
  } else if (flag == 1) {
    i1 = n;
    for (i = 1; i <= i1; ++i) {
      y1[i] = x[(i - 1 << 2) + 1];
      y2[i] = x[(i - 1 << 2) + 3];
    }
  } else if (flag == 9) {
    i1 = ng - (n << 2);
    for (k = 1; k <= i1; ++k) {
      i = ipar[(k - 1 << 1) + 1];
      j = ipar[(k - 1 << 1) + 2];
      d1 = x[(i - 1 << 2) + 1] - x[(j - 1 << 2) + 1];
      d2 = x[(i - 1 << 2) + 3] - x[(j - 1 << 2) + 3];
      d3 = rpar[i + n] + rpar[j + n];
      g[k] = d1 * d1 + d2 * d2 - d3 * d3;
    }
    k = ng - (n << 2) + 1;
    i1 = n;
    for (i = 1; i <= i1; ++i) {
      g[k] = x[(i - 1 << 2) + 3] - rpar[i + n] - rpar[(n << 1) + 3];
      ++k;
      g[k] = rpar[(n << 1) + 4] - x[(i - 1 << 2) + 3] - rpar[i + n];
      ++k;
      g[k] = x[(i - 1 << 2) + 1] - rpar[(n << 1) + 1] - rpar[i + n];
      ++k;
      g[k] = rpar[(n << 1) + 2] - rpar[i + n] - x[(i - 1 << 2) + 1];
      ++k;
    }
    
  } else if (flag == 2 && nevprt < 0) {
    i1 = ng - (n << 2);
    for (k = 1; k <= i1; ++k) {
      if (jroot[k] < 0) {
	i = ipar[(k - 1 << 1) + 1];
	j = ipar[(k - 1 << 1) + 2];
	s1 = x[(j - 1 << 2) + 1] - x[(i - 1 << 2) + 1];
	s2 = -rpar[i] * s1 / rpar[j];
	s3 = x[(j - 1 << 2) + 3] - x[(i - 1 << 2) + 3];
	s4 = -rpar[i] * s3 / rpar[j];
	a = rpar[i] * (s1 * s1 + s3 * s3) + rpar[j] * (s2 * s2 + s4 
						       * s4);
	b = rpar[i] * (s1 * x[(i - 1 << 2) + 2] + s3 * x[(i - 1 
							  << 2) + 4]) + rpar[j] * (s2 * x[(j - 1 << 2) + 2] + 
										   s4 * x[(j - 1 << 2) + 4]);
	xsi = -(b * 2. / a);
	x[(i - 1 << 2) + 2] += s1 * xsi;
	x[(j - 1 << 2) + 2] += s2 * xsi;
	x[(i - 1 << 2) + 4] += s3 * xsi;
	x[(j - 1 << 2) + 4] += s4 * xsi;
      }
    }
    k = ng - (n << 2) + 1;
    i1 = n;
    for (i = 1; i <= i1; ++i) {
      if (jroot[k] < 0) {
	x[(i - 1 << 2) + 4] = -x[(i - 1 << 2) + 4];
      }
      ++k;
      if (jroot[k] < 0) {
	x[(i - 1 << 2) + 4] = -x[(i - 1 << 2) + 4];
      }
      ++k;
      if (jroot[k] < 0) {
	x[(i - 1 << 2) + 2] = -x[(i - 1 << 2) + 2];
      }
      ++k;
      if (jroot[k] < 0) {
	x[(i - 1 << 2) + 2] = -x[(i - 1 << 2) + 2];
      }
      ++k;
    }
  }
} 

static integer c__5 = 5;
static integer c__6 = 6;

void bouncexy(scicos_block *block,int flag)
{
  int nevprt=block->nevprt;
  double t;
  double *z__;
  int nz;
  double *rpar;
  int nrpar, *ipar, nipar;
  double *u,*y;
  int nu,kfun;

 
  static int cur = 0;
  static int verb = 0;
  
  int i__1;
  
  static double rect[4];
  extern /* Subroutine */ int C2F(getlabel)();
  static double xmin, ymin, xmax, ymax;
  static char strf[40];
  extern /* Subroutine */ int C2F(sxevents)();
  static int i__, n, v;
  extern /* Subroutine */ int C2F(plot2d)();
  static int na;
  extern /* Subroutine */ int C2F(dr)();
  static double dv;
  static int on;
  static double zz[10];
  static int nxname;
  extern /* Subroutine */ int C2F(sciwin)(), C2F(dr1)();
  static char buf[40];
  static int wid, nax[4];
  
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = mode : animated =0 fixed=1 */
  /*     ipar(3:3+nu-1) = colors of balls */
  
  /*     rpar(1)=xmin */
  /*     rpar(2)=xmax */
  /*     rpar(3)=ymin */
  /*     rpar(4)=ymax */
  nevprt=block->nevprt;
  nz=block->nz;
  nrpar=block->nrpar; 
  nipar=block->nipar;
  nu=block->insz[0];
  z__=block->z;
  rpar=block->rpar;
  ipar=block->ipar;
  u=block->inptr[0];
  y=block->inptr[1];
  t=get_scicos_time();

  
  /* Parameter adjustments */
  --u;
  --y;
  --ipar;
  --rpar;
  --z__;
  
  /* Function Body */
  
  if (flag == 2) {
    wid = ipar[1];
    n = nu;
    
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    /*     erase first point */
    if (ipar[2] == 0) {
      C2F(dr1)("xfarcs\000", "v\000", &v, &ipar[3], &n, &v, &v, &v, &z__[1],
	       &dv, &dv, &dv);
    }
    /*     draw new point */
    i__1 = nu;
    for (i__ = 1; i__ <= i__1; ++i__) {
      z__[(i__ - 1) * 6 + 1] = u[i__] - z__[(i__ - 1) * 6 + 3] / 2;
      z__[(i__ - 1) * 6 + 2] = y[i__] + z__[(i__ - 1) * 6 + 4] / 2;
    }
    C2F(dr1)("xfarcs\000", "v\000", &v, &ipar[3], &n, &v, &v, &v, &z__[1], &
	     dv, &dv, &dv);
    C2F(dr1)("xset\000", "wshow\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
  } else if (flag == 4) {
    wid = ipar[1];
    n = nu;
    xmin = rpar[1];
    xmax = rpar[2];
    ymin = rpar[3];
    ymax = rpar[4];
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 2;
    nax[3] = 10;
    C2F(sciwin)();
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(dr1)("xsetdr\000", "X11\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    on = 1;
    C2F(dr1)("xset\000", "pixmap\000", &on, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	     &dv);
    rect[0] = xmin;
    rect[1] = ymin;
    rect[2] = xmax;
    rect[3] = ymax;
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    s_copy(strf, "030\000", 40, 4);
    C2F(dr1)("xset\000", "thickness\000", &c__1, &v, &v, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect, nax);
    C2F(dr1)("xset\000", "wshow\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    zz[0] = xmin;
    zz[1] = xmin;
    zz[2] = xmax;
    zz[3] = xmax;
    zz[4] = xmin;
    zz[5] = ymax;
    zz[6] = ymin;
    zz[7] = ymin;
    zz[8] = ymax;
    zz[9] = ymax;
    C2F(dr1)("xpolys\000", "v\000", &v, &v, &c__1, &c__1, &c__5, &v, zz, &zz[5], &dv, &dv);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    s_copy(buf + i__1, "\000", nxname + 1 - i__1, 1);
    if (nxname == 1 && *(unsigned char *)buf == ' ' || nxname == 0) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    if (ipar[2] == 0) {
      C2F(dr1)("xset\000", "alufunction\000", &c__6, &v, &v, &v, &v, &v, &
	       dv, &dv, &dv, &dv);
    }
    C2F(sxevents)();
    /*     first point drawing */
    if (ipar[2] == 0) {
      C2F(dr1)("xfarcs\000", "v\000", &v, &ipar[3], &n, &v, &v, &v, &z__[1],
	       &dv, &dv, &dv);
    }
  }
} 

void extractor(scicos_block *block,int flag)
{
  int i,j;
  if(flag==1){
    for(i=0;i<block->nipar;++i){
      j=block->ipar[i]-1;
      if(j<0) j=0;
      if(j>=block->insz[0]) j=block->insz[0]-1;
      block->outptr[0][i]=block->inptr[0][j];
    }
  }
}

void scalar2vector(scicos_block *block,int flag)
{
  int i;
  if(flag==1){
    for(i=0;i<block->outsz[0];++i){
      block->outptr[0][i]=block->inptr[0][0];
    }
  }
}

void minmax(scicos_block *block,int flag)
{ /*ipar[0]=1 -> min,  ipar[0]=2 -> max */
  
  int i,phase;
  double maxmin;
  phase=get_phase_simulation();
  if (flag == 1) {
    if(block->nin==1){
      if(block->ng==0|phase==1){
	maxmin=block->inptr[0][0];
	for (i=1;i<block->insz[0];++i){
	  if(block->ipar[0]==1){
	    if(block->inptr[0][i]<maxmin)  maxmin=block->inptr[0][i];
	  } else {
	    if(block->inptr[0][i]>maxmin)  maxmin=block->inptr[0][i];
	  }
	}
      }else{
	maxmin=block->inptr[0][block->mode[0]-1];
      }
      block->outptr[0][0]=maxmin;

    }else if (block->nin==2){
      for (i=0;i<block->insz[0];++i){
	if(block->ng==0|phase==1){
	  if(block->ipar[0]==1){
	    block->outptr[0][i]=min(block->inptr[0][i],block->inptr[1][i]);
	  } else {
	    block->outptr[0][i]=max(block->inptr[0][i],block->inptr[1][i]);
	  }
	}else{
	  block->outptr[0][i]=block->inptr[block->mode[0]-1][i];
	}
      }
    }
  } else if(flag == 9){
    if(block->nin==1){
      if(block->nin==1){
	if (phase==2){
	  for (i=0;i<block->insz[0];++i){
	    if(i!=block->mode[0]-1){
	      block->g[i]=block->inptr[0][i]-block->inptr[0][block->mode[0]-1];
	    }else{
	      block->g[i]=1.0;
	    }
	  }
	} else if (phase==1){
	  maxmin=block->inptr[0][0];
	  for (i=1;i<block->insz[0];++i){
	    block->mode[0]=1;
	    if(block->ipar[0]==1){
	      if(block->inptr[0][i]<maxmin) {
		maxmin=block->inptr[0][i];
		block->mode[0]=i+1;
	      }
	    } else {
	      if(block->inptr[0][i]>maxmin) {
		maxmin=block->inptr[0][i];
		block->mode[0]=i+1;
	      }
	    }
	  }
	}
      }
    }else if(block->nin==2){
      for (i=0;i<block->insz[0];++i){
	block->g[i]=block->inptr[0][i]-block->inptr[1][i];
	if(phase==1){
	  if(block->ipar[0]==1){
	    if(block->g[i]>0) {
	      block->mode[i]=2;
	    }else{
	      block->mode[i]=1;
	    }
	  }else{
	    if(block->g[i]<0) {
	      block->mode[i]=2;
	    }else{
	      block->mode[i]=1;
	    }
	  }
	}
      }
    }
  }
}


void  signum(scicos_block *block,int flag)
{
  int i,j;
  if (flag==1){
    for(i=0;i<block->insz[0];++i){
      if (get_phase_simulation()==1||block->ng==0) {
	if (block->inptr[0][i]<0){
	  j=2;
	} else if (block->inptr[0][i]>0){
	  j=1;
	}else{
	  j=0;
	}
      }else {
	j=block->mode[i];
      }
      if (j==1){
	block->outptr[0][i]=1.0;
      } else if(j==2){
	block->outptr[0][i]=-1.0;
      }else{
	block->outptr[0][i]=0.0;
      }
    }
  }else if (flag==9){
    for(i=0;i<block->insz[0];++i){
      block->g[i]=block->inptr[0][i];
      if (get_phase_simulation()==1) {
	if(block->g[i]<0){
	  block->mode[i]=2;
	}else{
	  block->mode[i]=1;
	}
      }
    }
  }
}


void product(scicos_block *block,int flag)
{
  int i,j,k,l;
  if(flag==1){
    if (block->nin==1){
      block->outptr[0][0]=1.0;
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][0]=block->outptr[0][0]*block->inptr[0][j];
      }
    }
    else {
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][j]=1.0;
	for (k=0;k<block->nin;k++) {
	  if(block->ipar[k]>0){
	    block->outptr[0][j]=block->outptr[0][j]*block->inptr[k][j];
	  }else{
	    if(block->inptr[k][j]==0){
	      set_block_error(-2);
	      return;
	    }else{
	      block->outptr[0][j]=block->outptr[0][j]/block->inptr[k][j];
	    }
	  }
	}
      }
    }
  }
}

  
void summation(scicos_block *block,int flag)
{
  int i,j,k,l;
  if(flag==1){
    if (block->nin==1){
      block->outptr[0][0]=0.0;
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][0]=block->outptr[0][0]+block->inptr[0][j];
      }
    }
    else {
      for (j=0;j<block->insz[0];j++) {
	block->outptr[0][j]=0.0;
	for (k=0;k<block->nin;k++) {
	  if(block->ipar[k]>0){
	    block->outptr[0][j]=block->outptr[0][j]+block->inptr[k][j];
	  }else{
	    block->outptr[0][j]=block->outptr[0][j]-block->inptr[k][j];
	  }
	}
      }
    }
  }
}

  
void multiplex(scicos_block *block,int flag)
{
  int i,j,k;
  if (block->nin==1){
    k=0;
    for (i=0;i<block->nout;++i){
      for (j=0;j<block->outsz[i];++j){
	block->outptr[i][j]=block->inptr[0][k];
	++k;
      }
    }
  }else {
    k=0;
    for (i=0;i<block->nin;++i){
      for (j=0;j<block->insz[i];++j){
	block->outptr[0][k]=block->inptr[i][j];
	++k;
      }
    }
  }
}

void gainblk(scicos_block *block,int flag)
{
  int i,un=1;
  if (block->nrpar==1){
    for (i=0;i<block->insz[0];++i){
      block->outptr[0][i]=block->rpar[0]*block->inptr[0][i];
    }
  }else{
    C2F(dmmul)(block->rpar,block->outsz[0],block->inptr[0],
	       block->insz[0],block->outptr[0],block->outsz[0],
	       block->outsz[0],block->insz[0],&un);
  }
}

void backlash(scicos_block *block,int flag)
{ 
  double* rw,t;
  int i,j,k;
  if (flag == 4){/* the workspace is used to store previous values */
    if ((*block->work=	 scicos_malloc(sizeof(double)* 4))== NULL ) {
      set_block_error(-16);
      return;
    }
    rw=*block->work; 
    t=get_scicos_time();
    rw[0]=t;
    rw[1]=t;
    rw[2]=block->rpar[0];
    rw[3]=block->rpar[0];
  }else  if (flag == 5){
    scicos_free(*block->work);
  }else  if (flag == 1) {
    rw=*block->work;
    t=get_scicos_time();
    if(t>rw[1]) {
      rw[0]=rw[1];
      rw[2]=rw[3];
    }
    rw[1]=t;
    if(block->inptr[0][0]>rw[2]+block->rpar[1]/2){
      rw[3]=block->inptr[0][0]-block->rpar[1]/2;
    } else if (block->inptr[0][0]<rw[2]-block->rpar[1]/2){
      rw[3]=block->inptr[0][0]+block->rpar[1]/2;
    } else {
      rw[3]=rw[2];
    }
    block->outptr[0][0]=rw[3];
  }  else if (flag == 9) {
    rw=*block->work;
    t=get_scicos_time();
    if(t>rw[1]){
      block->g[0] = block->inptr[0][0]-block->rpar[1]/2-rw[3];
      block->g[1] = block->inptr[0][0]+block->rpar[1]/2-rw[3];
    }else{
      block->g[0] = block->inptr[0][0]-block->rpar[1]/2-rw[2];
      block->g[1] = block->inptr[0][0]+block->rpar[1]/2-rw[2];
    }
  } 
}
