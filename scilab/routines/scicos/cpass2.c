#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include "nsp/machine.h"
#include "cc_pass2.h"
#include <memory.h>
#include <string.h>

/********************************** la fonctoin OR ********************************************/
int OR(int* Vect)
{
  int i=1;  
  int next=false;  

  while(!(next) && (i<Vect[0]+1))
    {
      next=(Vect[i]!=0)? true:false ;
      i++;
    }
  return next;
}
/************************************************* fin du OR *******************************/
/**************************** la fonction de Sign *****************************************/
int Sign(int x)
{
  int y;
  if  (x < 0) y=-1;
  else if (x > 0) y=1;
  else y=0;
  return y;
}
/******************************* fin de Sign  *************************************************/
/**************************** la fonction de Max1 *****************************************/
int Max1(int* vect)
{
  int i,max1;
  if (vect[0]<1){
    max1=0;
  }else{
    max1=vect[1];
    for(i=2;i<vect[0]+1;i++)
      {
	max1=(max1<vect[i])? vect[i]:max1;
      }
  }
  return max1;
}
/******************************* fin de Max1  *************************************************/
/**************************** la fonction de Min1 *****************************************/
int Min1(int* vect)
{
  int i,min1;
  if (vect[0]<1){
    min1=0;
  }else{
    min1=vect[1];
    for(i=2;i<vect[0]+1;i++)
      {
	min1=(min1>vect[i])? vect[i]:min1;
      }
  }
  return min1;
}
/******************************* fin de Min1  *************************************************/

/***************************** fonction AND ************************************************/
int AND(int* Vect)
{
  int i=1;  
  int next=false;  
  while((!(next)) && (i<Vect[0]+1))
    {
      next=(Vect[i]==0)? true :false;
      i++;
    }
  return !next;
}
/************************************** fin de AND *******************************************/

/******************************functio main *********************************************************/
int cpass2(bllst111,bllst112,bllst2,bllst3,bllst4,bllst5,bllst9,bllst10,
           bllst11,bllst12,bllst13,nmode,nzcross,bllst2ptr,bllst3ptr,bllst4ptr,bllst5ptr,bllst6ptr,bllst9ptr,typ_x,
           bllst11ptr,connectmat,clkconnect,corinvec,corinvptr,
           iz0,tevts,evtspt,pointi,outtb,zcptr,modptr,outlnk,inplnk,
           lnkptr,ordptr,execlk,ordclk,cord,oord,zord,critev,nb,ztyp,
           nblk,ndcblk,subscr,iord,ok)
     
     char ***bllst10,***bllst13,***bllst111;
     int **bllst2,**bllst3,**bllst4,**bllst5,**bllst2ptr,**bllst3ptr,**bllst4ptr;
     int **bllst5ptr,**typ_x,**bllst11ptr,*ok,**bllst112,**bllst9,**bllst9ptr;
     int **corinvec,**corinvptr,**evtspt,*pointi,**outtb,**zcptr,**modptr,**inplnk;
     int **outlnk,**lnkptr,**ordptr,**execlk,**ordclk,**cord,**oord;
     int **zord,**critev,**iz0,**subscr,**nzcross,**nmode,**bllst6ptr;
     int *nb,**ztyp,*nblk,*ndcblk,**iord,**bllst12,**connectmat,**clkconnect;
     double **bllst11,**tevts;  
{ 
 
  /*take care of the heritage*/
  int need_newblk=1,i,l,nombr,a,prime_pointer=1;
  int done,ncblk,nxblk,ndblk,nblk1; 
  int *typ_l,*typ_r,*typ_m,*tblock,*prime_act,*vec;
  int *typ_cons,*typ_z,*typ_s,*bexe,*boptr,*blnk,*blptr,*outoinptr,*typ_zx;
  int *outoin,*exe_cons,*evoutoin,*evoutoinptr,*ordptr1,*execlk0,*execlk_cons;
  double *initexe;
  nblk1=((int*) (*bllst10))[0];
  if (TEST) 
    {
      Message("No block can be activated");
      *ok=false;
      return 0;
    }
  
  mini_extract_info(*bllst2,bllst4,*bllst10,*bllst12,*bllst2ptr,*bllst3ptr,bllst4ptr,*typ_x,*connectmat,
		    *clkconnect,inplnk,outlnk,&typ_l,&typ_r,&typ_m,&tblock,&typ_cons,&typ_zx,*nzcross,ok);
 
  conn_mat(*inplnk,*outlnk,*bllst2ptr,*bllst3ptr,&outoin,&outoinptr,nblk);
  critical_events(*connectmat,*clkconnect,*bllst12,typ_r,typ_l,typ_zx,outoin,outoinptr,*bllst5ptr,critev);
  pak_ersi(clkconnect,typ_r,typ_l,outoin,outoinptr,tblock,typ_cons,*bllst5ptr,&exe_cons,*nblk);
  free(typ_r);
  typ_r=NULL;
  free(tblock);
  tblock=NULL;
  free(typ_zx);
  typ_zx=NULL;
  free(typ_cons);
  typ_cons=NULL;
  done=false; 
  if ((prime_act=malloc(sizeof(int)*2))== NULL ) return 0;
  prime_act[0]=1;
  prime_act[1]=0;
 
  for (i = 1; i <= nblk1; i++){
    if (!typ_l[i] && ((*bllst5ptr)[i+1]-(*bllst5ptr)[i] != 0)){
      if ((vec=malloc(sizeof(int)*((*bllst5ptr)[i+1]-(*bllst5ptr)[i]+1)))== NULL ) return 0;
      vec[0]=(*bllst5ptr)[i+1]-(*bllst5ptr)[i];
      if ((prime_act=(int*)realloc(prime_act,sizeof(int)*(prime_act[0]+vec[0]+1))) == NULL ) return 0;      
      for (l = 1; l <= vec[0]; l++){
	prime_act[prime_act[0]+l]=(*bllst5ptr)[i]+l-1;
      }
      prime_act[0]=prime_act[0]+vec[0];
      free(vec);
    }
  }
  while(!done)
    {
      make_ptr(*bllst10,&typ_l,&typ_m);
      cleanup(clkconnect);
      if (OR(typ_l))
	paksazi(bllst111,bllst112,bllst2,bllst3,bllst4,bllst5,bllst9,bllst10,bllst12,nzcross,bllst2ptr,bllst3ptr,
		bllst4ptr,bllst5ptr,bllst9ptr,connectmat,clkconnect,typ_l,typ_m,&done,ok,&need_newblk,
		corinvec,corinvptr,critev,&prime_pointer,&prime_act);
      else done=true;
      if(!(*ok)) return 0;
    }
  free(prime_act);  
  free(typ_l);
  typ_l=NULL;
  nombr=((int*) (*bllst10))[0];
  if (nblk1 < nombr) 
    {
      if (((*typ_x)=(int*)realloc((*typ_x),sizeof(int)*(nombr+1))) == NULL ) return 0;
      (*typ_x)[0]=nombr;
      if (((*bllst11ptr)=(int*)realloc((*bllst11ptr),sizeof(int)*((*bllst11ptr)[0]+nombr-nblk1+1))) == NULL ) return 0;
      (*bllst11ptr)[0]=(*bllst11ptr)[0]+nombr-nblk1;
      if (((*bllst11)=(double*)realloc((*bllst11),sizeof(double)*(((int*) (*bllst11))[0]+2*(nombr-nblk1)+1))) == NULL ) return 0;
      ((int*)(*bllst11))[0]=((int*)(*bllst11))[0]+2*(nombr-nblk1);
      if (((*nmode)=(int*)realloc((*nmode),sizeof(int)*(nombr+1))) == NULL ) return 0;
      (*nmode)[0]=nombr;      
      if (((*bllst13)=(char**)realloc((*bllst13),sizeof(char*)*(nombr+1))) == NULL )  return 0;
      ((int*) (*bllst13))[0]=nombr;
      for(i=nblk1+1;i<nombr+1;i++)
	{	  
	  (*typ_x)[i]=0;
	  (*nmode)[i]=1;	 	  
	  (*bllst11ptr)[i+1]=(*bllst11ptr)[i]+2;
	  for(l=(*bllst11ptr)[i];l<=(*bllst11ptr)[i+1]-1;l++)
	    {
	      (*bllst11)[l]=-1;
	    }
	  if (( (*bllst13)[i]=(char*) malloc(sizeof(char)*(2))) ==NULL )  return 0;
	  ((char*) (*bllst13)[i])[1]='\0';
	  *((*bllst13)[i])=' ';
	}      

    }
  free(typ_m);
  free(*inplnk);
  free(*outlnk);
  extract_info(*bllst2,*bllst3,*bllst5,*bllst10,*bllst11,*bllst12,*bllst2ptr,*bllst3ptr,*bllst4ptr,*bllst5ptr,
               *bllst11ptr,*connectmat,*clkconnect,lnkptr,inplnk,outlnk,*nzcross,*nmode,
	       &typ_s,typ_x,&typ_m,&initexe,&bexe,&boptr,&blnk,&blptr,ok,*corinvec,*corinvptr);
  free(typ_m);
  typ_m=NULL;
  if(!(*ok)) 
    {
      Message("Problem in port size.");
      return 0;
    }
  
  free(outoinptr);
  free(outoin);
  conn_mat(*inplnk,*outlnk,*bllst2ptr,*bllst3ptr,&outoin,&outoinptr,nblk);
  *nblk=nblk1;
  synch_clkconnect(typ_s,*clkconnect,&evoutoin,&evoutoinptr);
  discard(*bllst5ptr,*clkconnect,exe_cons,&ordptr1,execlk,&execlk0,&execlk_cons);
  free(*clkconnect);
  *clkconnect=NULL;
  free(exe_cons);
  exe_cons=NULL;
  
  scheduler(*bllst12,*bllst5ptr,*execlk,execlk0,execlk_cons,ordptr1,outoin,outoinptr,evoutoin,
	    evoutoinptr,&typ_z,typ_x,typ_s,bexe,boptr,blnk,blptr,ordptr,ordclk,cord,iord,oord,zord,ok,*nzcross);
  
  free(typ_s);
  typ_s=NULL;
  free(*typ_x);
  *typ_x=NULL;
  free(bexe);
  bexe=NULL;
  free(boptr);
  boptr=NULL;
  free(blnk);
  blnk=NULL;
  free(blptr);
  blptr=NULL;
  free(outoinptr);
  outoinptr=NULL;
  free(outoin);
  outoin=NULL;
  free(evoutoin);
  evoutoin=NULL;
  free(evoutoinptr);
  evoutoinptr=NULL;
  free(ordptr1);
  ordptr1=NULL;
  free(execlk0);
  execlk0=NULL;
  free(execlk_cons);
  execlk_cons=NULL;
  
  if(!(*ok)) return 0;
  if (((*zcptr)=malloc(sizeof(int)*((nombr+2))))== NULL ) return 0;
  (*zcptr)[0]=nombr+1;
  (*zcptr)[1]=1;
  if (((*modptr)=malloc(sizeof(int)*((nombr+2))))== NULL ) return 0;
  (*modptr)[0]=nombr+1;
  (*modptr)[1]=1;
  for (i=1; i<= nombr; i++)
    {
      (*zcptr)[i+1]=(*zcptr)[i] + typ_z[i];
      a=Sign(typ_z[i]);
      (*modptr)[i+1]=(*modptr)[i]+a*(*nmode)[i];
    }
  free(typ_z);
  /*completement inutile pour simulation, c'est pour la generation de code*/
  *ztyp=VecEg1(*nzcross); 
  for (i=1;i<=(*ztyp)[0];i++)
    {
      if ((*nzcross)[i] < 0) 
	{
	  (*ztyp)[i]=-1;
	}
      else if ((*nzcross)[i] > 0)
	{
	  (*ztyp)[i]=1;
	}
      else
	{
	  (*ztyp)[i]=0;  
	}
    }
  /* if xptr($)==1 & zcptr($)>1 then */
  if ((*bllst6ptr)[(*bllst6ptr)[0]]==1 && (*zcptr)[(*zcptr)[0]] > 1) {
    Message("No continuous-time state. Tresholds are ignored.");
  }
  ncblk=0;nxblk=0;ndblk=0;*ndcblk=0;

  *nb=nombr;
  
  init_agenda(initexe,*bllst5ptr,tevts,evtspt,pointi);
  free(initexe);
  initexe=NULL;
  if (((*outtb)=(int*)calloc((*lnkptr)[(*lnkptr)[0]],sizeof(int)))== NULL ) return 0;
  (*outtb)[0]=(*lnkptr)[(*lnkptr)[0]]-1;
  if (((*iz0)=(int*)calloc(*nb+1,sizeof(int)))== NULL ) return 0;
  (*iz0)[0]=*nb;
  
  /* *iz0=NULL; */
  *subscr=NULL;
  return 0;
} /*endfuncton cpass*/

/***************************************** fin de c_pass2 ************************************/
/***************************************** fonction scheduler*********************************/
int scheduler(bllst12,bllst5ptr,execlk,execlk0,execlk_cons,ordptr1,outoin,outoinptr,evoutoin,
               evoutoinptr,typ_z,typ_x,typ_s,bexe,boptr,blnk,blptr,ordptr2,ordclk,cord,iord,oord,
               zord,ok,nzcross)
     int *bllst5ptr,*outoinptr,*evoutoin,*evoutoinptr,**typ_z,**typ_x,*typ_s,*bexe,*boptr;
     int *blnk,*blptr,**ordptr2,*ok,*ordptr1,*bllst12,*nzcross;
     int *execlk,*execlk0,*execlk_cons,*outoin,**ordclk,**cord,**iord,**oord,**zord;
{
  int i,iii,k,l,j,jj,hh,o,maX,n,nblk=(*typ_x)[0],f,fin=0;
  int *vec,*wec,*ii,*ii1,*ii2,*r,*ext_cord1,*pp,*ppget,*ppget1,*ext_cord2,*typ_z_save;
  int *orddif,*cordX,*ext_cord,*ext_cord_old,*oordii,*ind,a,val=0;
  Mat2C ordclki,ext_cord1i;
  
  vec=NULL;
  wec=NULL;
  cordX=NULL;
  oordii=NULL;
  r=NULL;
  orddif=NULL;
  ii=NULL;
  ii1=NULL;
  ii2=NULL;
  ext_cord=NULL;
  ext_cord1=NULL;
  ind=NULL;
  *typ_z=VecEg1(nzcross);
  if (execlk0)
    {
      /* compute cord */
      ind=GetPartVect(execlk0,1,execlk0[0]/2);
      if ((vec=(int*)calloc(ind[0]+1,sizeof(int)))== NULL ) return 0;
      vec[0]=ind[0];
      wec=GetPartVect(execlk0,execlk0[0]/2+1,execlk0[0]/2);
      tree2(vec,vec[0],wec,ind,bllst12,outoin,outoinptr,cord,ok);
      if (ind) free(ind);
      if(vec) free(vec);
      vec=NULL;
      if(wec) free(wec);
      wec=NULL;
    } 
  else 
    {
      (*cord)=NULL;
    } 
      
  /* compute iord */
  if (execlk_cons)
    {
      ind=GetPartVect(execlk_cons,1,execlk_cons[0]/2);
      if ((vec=(int*)calloc(ind[0]+1,sizeof(int)))== NULL ) return 0;
      vec[0]=ind[0];
      wec=GetPartVect(execlk_cons,execlk_cons[0]/2+1,execlk_cons[0]/2);
      tree2(vec,vec[0],wec,ind,bllst12,outoin,outoinptr,iord,ok);
      if (ind) free(ind);
      if(vec) free(vec);
      vec=NULL;
      if(wec) free(wec);
      wec=NULL;
    } 
  else 
    {
      (*iord)=NULL;
    }

  if (!(*ok))
    {
      Message("Algebraic loop detected; cannot be compiled");
      (*cord)=NULL;
      (*iord)=NULL;
      return 0;
    }
 if (((*ordclk)=(int*)malloc(sizeof(int)))== NULL ) return 0;
 (*ordclk)[0]=0; 
 (*ordptr2)=VecEg1(ordptr1);
 for(o=1;o<bllst5ptr[nblk+1];o++)
   {
     ind=GetPartVect(execlk,ordptr1[o],ordptr1[o+1]-ordptr1[o]);
     if (ind)
       {
	 if ((vec=(int*)calloc(ind[0]+1,sizeof(int)))== NULL ) return 0;
	 vec[0]=ind[0];
	 wec=GetPartVect(execlk,ordptr1[o]+execlk[0]/2,ordptr1[o+1]-ordptr1[o]);
	 tree2(vec,vec[0],wec,ind,bllst12,outoin,outoinptr,&r,ok);
	 if (ind) free(ind);
	 if(vec) free(vec);
	 vec=NULL;
	 if(wec) free(wec);
	 wec=NULL;
	 if (!(*ok))
	   {
	     Message("Algebraic loop detected; cannot be compiled");
	     (*cord)=NULL;
	     (*iord)=NULL;
	     (*ordclk)=NULL;
	     (*ordptr2)=NULL;
	     return 0;
	   }
	 (*ordptr2)[o+1]=r[0]/2+(*ordptr2)[o];
	 if ((ordclki.col1=(int*)malloc(sizeof(int)*((*ordclk)[0]/2+r[0]/2+1))) == NULL) return 0;
	 ordclki.col1[0]=(*ordclk)[0]/2+r[0]/2;
	 if ((ordclki.col2=(int*)malloc(sizeof(int)*((*ordclk)[0]/2+r[0]/2+1))) == NULL) return 0;
	 ordclki.col2[0]=(*ordclk)[0]/2+r[0]/2;
	 pp=&(ordclki.col1)[1];
	 pp=memcpy(pp,&(*ordclk)[1],sizeof(int)*((*ordclk)[0]/2));
	 pp=&(ordclki.col2)[1];
	 pp=memcpy(pp,&(*ordclk)[(*ordclk)[0]/2+1],sizeof(int)*((*ordclk)[0]/2));
	 pp=&(ordclki.col1)[(*ordclk)[0]/2+1];
	 pp=memcpy(pp,&r[1],sizeof(int)*(r[0]/2));
	 pp=&(ordclki.col2)[(*ordclk)[0]/2+1];
	 pp=memcpy(pp,&r[1+r[0]/2],sizeof(int)*(r[0]/2));
	 if (((*ordclk)=(int*)realloc((*ordclk),sizeof(int)*((*ordclk)[0]+r[0]+1))) == NULL ) return 0;
	 (*ordclk)[0]=r[0]+(*ordclk)[0];
	 pp=&(*ordclk)[1];
	 pp=memcpy(pp,&(ordclki.col1)[1],sizeof(int)*(ordclki.col1[0]));
	 pp=&(*ordclk)[(*ordclk)[0]/2+1];
	 pp=memcpy(pp,&(ordclki.col2)[1],sizeof(int)*(ordclki.col1[0]));
	 if(ordclki.col1) free(ordclki.col1);
	 ordclki.col1=NULL;
	 if(ordclki.col2) free(ordclki.col2);
	 ordclki.col2=NULL;
	 
	 if(r) free(r);
	 r=NULL; 
       }
   }
 if ((orddif=(int*)malloc(sizeof(int)*(ordptr1[0]+1)))== NULL ) return 0;
 orddif[0]=ordptr1[0];
 for (k=1;k<orddif[0]+1;k++)
   {
     if (ordptr1[k]!=(*ordptr2)[k]) orddif[k]=1;
     else orddif[k]=0;
   }
 if (AND(orddif))
   {
     sciprint("serious bug,report0");
   }
 if(orddif) free(orddif);
 if (!(*cord))
   {
    if ((*cord=(int*)malloc(sizeof(int)))== NULL ) return 0;
     (*cord)[0]=0; 
   }
 
 if ((vec=(int*)malloc(sizeof(int)*(nblk+1)))== NULL ) return 0;
 vec[0]=nblk;
 Setmem(vec,-1);   
 for(i=1;i<(*cord)[0]/2+1;i++)
   {
     vec[(*cord)[i]]=0;
   }
 
 ext_cord1=VecEg1(*cord);
 if (ext_cord1[0] != 0){
   j=1;
   while (true) {
     if (typ_s[ext_cord1[j]]) {
       for (i=bllst5ptr[ext_cord1[j]]; i< bllst5ptr[ext_cord1[j]+1]; i++){
	 a=ext_cord1[0]/2+ordptr1[i+1]-ordptr1[i];
	 if ((ext_cord1i.col1=(int*)malloc(sizeof(int)*(a+1))) == NULL) return 0;
	 ext_cord1i.col1[0]=a;
	 if ((ext_cord1i.col2=(int*)malloc(sizeof(int)*(a+1))) == NULL) return 0;
	 ext_cord1i.col2[0]=a;
	 pp=&(ext_cord1i.col1)[1];
	 pp=memcpy(pp,&ext_cord1[1],sizeof(int)*(ext_cord1[0]/2));
	 pp=&(ext_cord1i.col2)[1];
	 pp=memcpy(pp,&ext_cord1[ext_cord1[0]/2+1],sizeof(int)*(ext_cord1[0]/2));
	 pp=&(ext_cord1i.col1)[ext_cord1[0]/2+1];
	 pp=memcpy(pp,&(*ordclk)[ordptr1[i]],sizeof(int)*(ordptr1[i+1]-ordptr1[i]));
	 pp=&(ext_cord1i.col2)[ext_cord1[0]/2+1];
	 pp=memcpy(pp,&(*ordclk)[ordptr1[i]+(*ordclk)[0]/2],sizeof(int)*(ordptr1[i+1]-ordptr1[i]));
	 if ((ext_cord1=(int*)realloc(ext_cord1,sizeof(int)*(2*a+1))) == NULL ) return 0;
	 ext_cord1[0]=2*a;
	 pp=&ext_cord1[1];
	 pp=memcpy(pp,&(ext_cord1i.col1)[1],sizeof(int)*(a));
	 pp=&ext_cord1[a+1];
	 pp=memcpy(pp,&(ext_cord1i.col2)[1],sizeof(int)*(a));
	 if(ext_cord1i.col1) free(ext_cord1i.col1);
	 ext_cord1i.col1=NULL;
	 if(ext_cord1i.col2) free(ext_cord1i.col2);
	 ext_cord1i.col2=NULL;
       }
     }
     j++;
     if (j > ext_cord1[0]/2) break;
   }
 }
 /* ext_cord=unique(ext_cord1(:,1)') */ 
 if ((ext_cord=(int*)malloc(sizeof(int)*(ext_cord1[0]/2+1))) == NULL ) return 0;
 ext_cord[0]=ext_cord1[0]/2;  
 for (i=1; i<ext_cord[0]+1; i++){
   f=0;
   for (j=1; j<=val; j++){
     if ((ext_cord[j]-ext_cord1[i])==0){
       f=1;
       break;
     }
   }
   if ( !f ){
     val++;
     ext_cord[val]=ext_cord1[i];
   }
 }
 ext_cord[0]=val;
 /* adding zero crossing surfaces to cont. time synchros */ 
 
 tree3(vec,vec[0],bllst12,typ_s,bexe,boptr,blnk,blptr,&ext_cord_old,ok);
 if(vec) free(vec);
 vec=NULL; 
 if ((ind=(int*)malloc(sizeof(int)*(ext_cord_old[0]+1))) == NULL ) return 0;
 ind[0]=ext_cord_old[0];
 C2F(isort)(&ext_cord_old[1],&ext_cord_old[0],&ind[1]);
 free(ind);
 if ((ind=(int*)malloc(sizeof(int)*(ext_cord[0]+1))) == NULL ) return 0;
 ind[0]=ext_cord[0];
 ext_cord2=VecEg1(ext_cord);
 C2F(isort)(&ext_cord2[1],&ext_cord2[0],&ind[1]);
 free(ind);
 if ((vec=(int*)calloc((ext_cord[0]+1),sizeof(int))) == NULL ) return 0;
 vec[0]=ext_cord[0];
 for (i=1; i<=ext_cord[0]; i++){
   if (ext_cord2[i] != ext_cord_old[i]) vec[i]=1;
 }
 free(ext_cord2);
 if (ext_cord_old) free(ext_cord_old);
 if ( OR(vec)){
   free(vec);
   return 0;
 }
 if (vec) free(vec);
 /*pour mettre a zero les typ_z qui ne sont pas dans ext_cord
   noter que typ_z contient les tailles des nzcross (peut etre >1)*/
     
 for (i=1; i<=ext_cord[0]; i++)
   {
     (*typ_z)[ext_cord[i]]=-(*typ_z)[ext_cord[i]];
   }
 for (i=1; i<=(*typ_z)[0]; i++)
   {
     if ((*typ_z)[i] < 0) 
       {
	 (*typ_z)[i]=-(*typ_z)[i];
       }
     else 
       {
	 (*typ_z)[i]=0;
       }
   }
 if (!(*ok)) 
   {
     sciprint("serious bug,report1");
   }
 typ_z_save=VecEg1(*typ_z);
 while (!fin){
   fin=1;
   for (i=ext_cord[0]; i>0; i--){
     iii=ext_cord[i];
     ii=GetPartVect(outoin,outoinptr[iii],outoinptr[iii+1]-outoinptr[iii]);
     if (!ii)
       {
	 if ((ii=(int*)malloc(sizeof(int)))== NULL ) return 0;
	 ii[0]=0;
       }      
     ppget=GetPartVect(evoutoin,evoutoinptr[iii],evoutoinptr[iii+1]-evoutoinptr[iii]);
     if(ppget)
       {
	 if ((ii=(int*)realloc(ii,sizeof(int)*(ii[0]+ppget[0]+1)))== NULL ) return 0;
	 pp=&ii[ii[0]+1];
	 pp=memcpy(pp,&(ppget)[1],sizeof(int)*(ppget[0]));
	 ii[0]=ii[0]+ppget[0];
	 free(ppget);
       }
     for (j=1;j<ii[0]+1;j++)
       {
	 l=ii[j];
	 if ((*typ_z)[l]) {
	   if ((*typ_z)[iii] == 0){
	     (*typ_z)[iii]=1;
	     fin=0;
	   }
	 }
	 if ((*typ_x)[l]) {
	   if (!(*typ_x)[iii]) {
	     (*typ_x)[iii]=1;
	     fin=0;
	   }
	 }
	 if ((*typ_z)[iii] && (*typ_x)[iii]) break;
       }
     if (ii) free(ii);
   }
 }
 
 free(ext_cord); ext_cord=NULL;
 
 if ((*cord)[0] != 0)
   {
     if ((ppget=(int*)calloc((*cord)[0]/2+1,sizeof(int))) == NULL ) return 0;
     ppget[0]=(*cord)[0]/2;
     ppget1=VecEg1(ppget);
     for (i=1; i <= (*cord)[0]/2; i++){
       if ((*typ_z)[(*cord)[i]]) ppget[i]=1;
       if ((*typ_x)[(*cord)[i]]) ppget1[i]=1;
     }
     ii=FindDif(ppget,0);
     free(ppget);
     if (ii)
       {
	 if (((*zord)=(int*)malloc(sizeof(int)*(2*ii[0]+1))) == NULL ) return 0;
	 (*zord)[0]=2*ii[0];	 
	 for(i=1;i<ii[0]+1;i++)
	   {
	     (*zord)[i]=(*cord)[ii[i]];
	     (*zord)[i+ii[0]]=(*cord)[ii[i]+(*cord)[0]/2];
	   }	 
	 free(ii);
       }
     ii=FindDif(ppget1,0);
     free(ppget1);
     if (ii)
       {
	 if (((*oord)=(int*)malloc(sizeof(int)*(2*ii[0]+1))) == NULL ) return 0;
	 (*oord)[0]=2*ii[0];	 
	 for(i=1;i<ii[0]+1;i++)
	   {
	     (*oord)[i]=(*cord)[ii[i]];
	     (*oord)[i+ii[0]]=(*cord)[ii[i]+(*cord)[0]/2];
	   }	 
	 free(ii);
       }
   } /* fin de if *cord[0] !=0 */
 else
   {
     *oord=NULL;
     *zord=NULL;
   }
     
 free(*typ_z);
 (*typ_z)=VecEg1(typ_z_save);
 free(typ_z_save);
 ii=GetPartVect(ext_cord1,1,ext_cord1[0]/2);
 if ((ii=(int*)realloc(ii,sizeof(int)*(ext_cord1[0]/2+(*ordclk)[0]/2+1))) == NULL ) return 0;
 ii[0]=ext_cord1[0]/2+(*ordclk)[0]/2;
 pp=&ii[ext_cord1[0]/2+1];
 pp=memcpy(pp,&(*ordclk)[1],sizeof(int)*((*ordclk)[0]/2));
 maX=Max1(ii)+1;
 free(ii);
 ii=NULL;
 if ((cordX=(int*)malloc(sizeof(int)*(ext_cord1[0]/2+1))) == NULL ) return 0;
 cordX[0]=ext_cord1[0]/2;
 ii1=GetPartVect(ext_cord1,1,ext_cord1[0]/2);
 ii2=GetPartVect(ext_cord1,ext_cord1[0]/2+1,ext_cord1[0]/2);
 if (ii1 && ii2)
   {
     for(i=1;i<ext_cord1[0]/2+1;i++)
       {
	 cordX[i]=maX*ii1[i]+ii2[i];
       }
     free(ii1);
     free(ii2);     
   }
 if (ext_cord1) free(ext_cord1);
 
 for (i=1;i<bllst5ptr[nblk+1];i++)
   {	 
     for (hh=ordptr1[i];hh<ordptr1[i+1];hh++)
       {
	 jj=(*ordclk)[hh];
	 if ((ii=(int*)malloc(sizeof(int)*(cordX[0]+1))) == NULL ) return 0;
	 ii[0]=cordX[0];
	 n=jj*maX+(*ordclk)[hh+(*ordclk)[0]/2];
	 for (j=1;j<cordX[0]+1;j++)
	   {
	     if (cordX[j]==n) ii[j]=1;
	     else ii[j]=0;
	   }
	 if (OR(ii))
	   {
	     (*ordclk)[hh+(*ordclk)[0]/2]=-(*ordclk)[hh+(*ordclk)[0]/2];
	   } /* fin de if OR()*/
	 if(ii) free(ii);
       } /* fin de for hh */
   } /* fin de for i */
 if(cordX) free(cordX);
 cordX=NULL;
 
 return 0;
} /* end function */
/***************************************** fin de scheduler**********************************/
/* =======================================function paksazi=============================================== */
int paksazi(char*** bllst111,int** bllst112,int** bllst2,int** bllst3,int** bllst4,int** bllst5,int** bllst9,
	    char*** bllst10,int** bllst12,int** nzcross,int** bllst2ptr,int** bllst3ptr,int** bllst4ptr,int** bllst5ptr,
	    int** bllst9ptr,int** connectmat,int** clkconnect,int* typ_l,int* typ_m,int* done,int* ok,
	    int* need_newblk,int** corinvec,int** corinvptr,int** critev,int* prime_pointer,int** prime_act)
{
  
  int i,j=1,k,l,o,leng,ki,sbb,m1,a;
  int *vectconnect,*iVect,*id,*dl,*ltmp,*idl,*lb,*indx,*tmpvect,*indxo,*indy,*bexe,*blnk;
  int *tmp,*clkconnecttmp,*clkconnect0,*texeclk0,*ind,*w2,*w1,*boptr,*blptr,*tclkconnect,*tcon,*ordptr1,*texeclki;
  int *b1,*typ_lm,*pointer,*r,*w3,*pointer1,*con,*vec,*clkconnectind,*connectmatind,*con1;
  int okk[1],nn=1,nblk=((int*) (*bllst10))[0];
  int nblkorg=nblk;
  int change=false;  
  Mat4C clkconnecti,connectmati;
  Mat2C bllst12i;
  int bl_out;  
  int *ptmp1,*ptmp2,*inter,*pp,*ppget;
  ptmp1=ptmp2=b1= typ_lm= pointer= r= w3= pointer1= con= vec= clkconnectind= connectmatind= con1=NULL;
  vectconnect=iVect= id= dl= ltmp= idl= lb= indx= tmpvect= indxo= indy= bexe= blnk=inter=NULL;
  tmp= clkconnecttmp= clkconnect0= texeclk0= ind= w2= w1= boptr= blptr= tclkconnect= tcon= ordptr1= texeclki=NULL;
  *ok=true;
  
  if(*need_newblk)
    {
      vectconnect=GetPartVect((*clkconnect),(*clkconnect)[0]/2+1,(*clkconnect)[0]/4);
      Inv(vectconnect);
      if ((iVect=(int*)malloc(sizeof(int)*(vectconnect[0]+1))) == NULL ) return 0;
      iVect[0]=vectconnect[0];
      C2F(isort)(&vectconnect[1],&vectconnect[0],&iVect[1]);
      free(vectconnect);
      vectconnect=NULL;
      clkconnectind=VecEg1(*clkconnect);
      for(i=1;i<iVect[0]+1;i++)
        {
          (*clkconnect)[i]=clkconnectind[iVect[i]];
          (*clkconnect)[i+iVect[0]]=clkconnectind[iVect[i]+iVect[0]];
          (*clkconnect)[i+2*iVect[0]]=clkconnectind[iVect[i]+2*iVect[0]];
          (*clkconnect)[i+3*iVect[0]]=clkconnectind[iVect[i]+3*iVect[0]];      
        }            
      free(iVect);
      iVect=NULL;
      if(clkconnectind) free(clkconnectind);
      clkconnectind=NULL;
      if ((id=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+1))) == NULL ) return 0;
      id[0]=(*clkconnect)[0]/4;
      a=(*clkconnect)[0]/2;
      for(i=1;i<(*clkconnect)[0]/4+1;i++)
        {
	  if(typ_l[(*clkconnect)[a+i]]==1) id[j++]=i;
	}
      id[0]=j-1;
      ltmp=GetCollVect(*clkconnect,id,3);
      if ((idl=(int*)malloc(sizeof(int)*(3+ltmp[0]))) == NULL ) return 0;
      idl[1]=1;
      j=2;
      for(i=1;i<ltmp[0];i++)
	{
	  if(ltmp[i+1]-ltmp[i] != 0) idl[j++]=i+1;
	}
      idl[0]=j;
      idl[j]=1+ltmp[0];
      if(ltmp) free(ltmp);
      ki=0;
      lb=FindEg(typ_l,1);
      for(i=1;i<lb[0]+1;i++)
	{
	  ki++;
	  if (id[0] != 0) {
	    indx=GetPartVect(id,idl[ki],idl[ki+1]-idl[ki]);
	  } else { indx=NULL;}
	  if (indx==NULL) 
	    {
	      Message("A synchro block is inactive; cannot be compile");
	      *ok=false;
	      return 0;
	    }
	  if ((tmpvect=(int*)malloc(sizeof(int)*(indx[0]+1))) == NULL ) return 0;
	  tmpvect[0]=indx[0];
	  ppget=GetCollVect(*clkconnect,indx,1);
	  for (l=1;l<ppget[0]+1;l++)
	    {
	      if (ppget[l]==lb[i]) tmpvect[l]=1;
	      else tmpvect[l]=0;
	    }
	  free (ppget);
	  if (OR(tmpvect))
	    {
	      Message("A algebric loop detected; on activation links");
	      *ok=false;
	      return 0;
	    }         
	  free(tmpvect);
	  tmpvect=NULL;
	  *need_newblk=false;
	  nn=indx[0];
	  if (nn >= 2)
	    {
	      *need_newblk=true;
	      ppget=GetPartVect(*clkconnect,1,(*clkconnect)[0]/4);
	      indxo=FindEg(ppget,lb[i]);
	      free(ppget);
	      ppget=GetPartVect(*connectmat,(*connectmat)[0]/2+1,(*connectmat)[0]/4);
	      indy=FindEg(ppget,lb[i]);
	      free(ppget);
	      if (indy[0] > 1) sciprint("Synchro block cannot have more than 1 input");
	      if (((*bllst2ptr)=(int*)realloc((*bllst2ptr),sizeof(int)*((*bllst2ptr)[0]+nn))) == NULL ) return 0;
	      if (((*bllst3ptr)=(int*)realloc((*bllst3ptr),sizeof(int)*((*bllst3ptr)[0]+nn))) == NULL ) return 0;
	      if (((*bllst4ptr)=(int*)realloc((*bllst4ptr),sizeof(int)*((*bllst4ptr)[0]+nn))) == NULL ) return 0;
	      if (((*bllst5ptr)=(int*)realloc((*bllst5ptr),sizeof(int)*((*bllst5ptr)[0]+nn))) == NULL ) return 0;	      
	      if (((*bllst9ptr)=(int*)realloc((*bllst9ptr),sizeof(int)*((*bllst9ptr)[0]+nn))) == NULL ) return 0;
	      if ((*bllst10=(char**)realloc(*bllst10,(nblk+nn)*sizeof(char*))) ==NULL )  return 0;
	      ((int*)(*bllst10))[0]=nblk+nn-1;
	      if ((*bllst111=(char**)realloc(*bllst111,sizeof(char*)*(nblk+nn))) == NULL )  return 0;
	      ((int*) (*bllst111))[0]=nblk+nn-1;
	      if (((*bllst112)=(int*)realloc((*bllst112),sizeof(int)*(nn+nblk))) == NULL ) return 0;
	      (*bllst112)[0]=nblk+nn-1;
	      if ((bllst12i.col1=(int*)malloc(sizeof(int)*(nblk+nn))) == NULL ) return 0;
	      bllst12i.col1[0]=nblk+nn-1;
	      if ((bllst12i.col2=(int*)malloc(sizeof(int)*(nblk+nn))) == NULL ) return 0;
	      bllst12i.col2[0]=nblk+nn-1;
	      if (((*bllst12)=(int*)realloc((*bllst12),sizeof(int)*(2*(nn+nblk)-1))) == NULL ) return 0;
	      (*bllst12)[0]=2*(nblk+nn-1);
	      if (((*corinvptr)=(int*)realloc((*corinvptr),sizeof(int)*((*corinvptr)[0]+nn))) == NULL ) return 0;
	      if (((*corinvec)=(int*)realloc((*corinvec),sizeof(int)*((*corinvec)[0]+4*nn+1))) == NULL ) return 0;
	      if (((*nzcross)=(int*)realloc((*nzcross),sizeof(int)*(nblk+nn))) == NULL ) return 0;
	      (*nzcross)[0]=nblk+nn-1;
	      
	      for(k=2;k<nn+1;k++)
		{
		  if (((*critev)=(int*)realloc((*critev),sizeof(int)*((*critev)[0]+(*bllst5ptr)[lb[i]+1]-(*bllst5ptr)[lb[i]]+1))) == NULL ) return 0;
		  a=(*critev)[0]-(*bllst5ptr)[lb[i]]+1;
		  for(l=(*bllst5ptr)[lb[i]]; l<(*bllst5ptr)[lb[i]+1]; l++)
		    (*critev)[l+a]=(*critev)[l];
		  (*critev)[0]=(*critev)[0]+(*bllst5ptr)[lb[i]+1]-(*bllst5ptr)[lb[i]];	
  
		  (*clkconnect)[indx[k]+(*clkconnect)[0]/2]=nblk+1;
		  
		  if (((*bllst111)[nblk+1]=(char*) malloc(sizeof(char)*(strlen((*bllst111)[lb[i]])+1))) ==NULL )  return 0;
		  ((char*) (*bllst111)[nblk+1])[strlen((*bllst111)[lb[i]])]='\0';
		  strcpy((*bllst111)[nblk+1],(*bllst111)[lb[i]]);
		  (*bllst112)[nblk+1]=(*bllst112)[lb[i]];
		  
		  (*bllst2ptr)[(*bllst2ptr)[0]+1]=(*bllst2ptr)[(*bllst2ptr)[0]]+(*bllst2ptr)[lb[i]+1]-(*bllst2ptr)[lb[i]];
		  (*bllst2ptr)[0]++;
		  if (((*bllst2)=(int*)realloc((*bllst2),sizeof(int)*((*bllst2)[0]+(*bllst2ptr)[nblk+2]-(*bllst2ptr)[nblk+1]+1))) == NULL ) return 0;
		  (*bllst2)[0]=(*bllst2)[0]+(*bllst2ptr)[nblk+2]-(*bllst2ptr)[nblk+1];
		  a=(*bllst2ptr)[lb[i]]-(*bllst2ptr)[nblk+1];            
		  for(l=(*bllst2ptr)[nblk+1];l<(*bllst2ptr)[nblk+2];l++)
		    {
		      (*bllst2)[l]=(*bllst2)[a+l];
		    }
		  (*bllst3ptr)[(*bllst3ptr)[0]+1]=(*bllst3ptr)[(*bllst3ptr)[0]]+(*bllst3ptr)[lb[i]+1]-(*bllst3ptr)[lb[i]];
		  (*bllst3ptr)[0]++;
		  if (((*bllst3)=(int*)realloc((*bllst3),sizeof(int)*((*bllst3)[0]+(*bllst3ptr)[nblk+2]-(*bllst3ptr)[nblk+1]+1))) == NULL ) return 0;
		  (*bllst3)[0]=(*bllst3)[0]+(*bllst3ptr)[nblk+2]-(*bllst3ptr)[nblk+1];
		  a=(*bllst3ptr)[lb[i]]-(*bllst3ptr)[nblk+1];
		  for(l=(*bllst3ptr)[nblk+1];l<=(*bllst3ptr)[nblk+2]-1;l++)
		    {
		      (*bllst3)[l]=(*bllst3)[a+l];
		    }
		  (*bllst4ptr)[(*bllst4ptr)[0]+1]=(*bllst4ptr)[(*bllst4ptr)[0]]+(*bllst4ptr)[lb[i]+1]-(*bllst4ptr)[lb[i]];
		  (*bllst4ptr)[0]++;
		  if (((*bllst4)=(int*)realloc((*bllst4),sizeof(int)*((*bllst4)[0]+(*bllst4ptr)[nblk+2]-(*bllst4ptr)[nblk+1]+1))) == NULL ) return 0;
		  (*bllst4)[0]=(*bllst4)[0]+(*bllst4ptr)[nblk+2]-(*bllst4ptr)[nblk+1];
		  a=(*bllst4ptr)[lb[i]]-(*bllst4ptr)[nblk+1];
		  for(l=(*bllst4ptr)[nblk+1];l<=(*bllst4ptr)[nblk+2]-1;l++)
		    {
		      (*bllst4)[l]=(*bllst4)[a+l];
		    }
		  (*bllst5ptr)[(*bllst5ptr)[0]+1]=(*bllst5ptr)[(*bllst5ptr)[0]]+(*bllst5ptr)[lb[i]+1]-(*bllst5ptr)[lb[i]];
		  (*bllst5ptr)[0]++;
		  if (((*bllst5)=(int*)realloc((*bllst5),sizeof(int)*((*bllst5)[0]+(*bllst5ptr)[nblk+2]-(*bllst5ptr)[nblk+1]+1))) == NULL ) return 0;
		  (*bllst5)[0]=(*bllst5)[0]+(*bllst5ptr)[nblk+2]-(*bllst5ptr)[nblk+1];
		  a=(*bllst5ptr)[lb[i]]-(*bllst5ptr)[nblk+1];
		  for(l=(*bllst5ptr)[nblk+1];l<=(*bllst5ptr)[nblk+2]-1;l++)
		    {
		      (*bllst5)[l]=(*bllst5)[a+l];
		    }
		  (*bllst9ptr)[(*bllst9ptr)[0]+1]=(*bllst9ptr)[(*bllst9ptr)[0]]+(*bllst9ptr)[lb[i]+1]-(*bllst9ptr)[lb[i]];
		  (*bllst9ptr)[0]++;
		  if (((*bllst9)=(int*)realloc((*bllst9),sizeof(int)*((*bllst9)[0]+(*bllst9ptr)[nblk+2]-(*bllst9ptr)[nblk+1]+1))) == NULL ) return 0;
		  (*bllst9)[0]=(*bllst9)[0]+(*bllst9ptr)[nblk+2]-(*bllst9ptr)[nblk+1];
		  a=(*bllst9ptr)[lb[i]]-(*bllst9ptr)[nblk+1];
		  for(l=(*bllst9ptr)[nblk+1];l<=(*bllst9ptr)[nblk+2]-1;l++)
		    {
		      (*bllst9)[l]=(*bllst9)[a+l];
		    }
		  
		  if (((*bllst10)[nblk+1]=(char*)malloc(sizeof(char)*2)) ==NULL )  return 0;
		  *(*bllst10)[nblk+1]=*(*bllst10)[lb[i]];
		  (*bllst10)[nblk+1][1]='\0';
		  
		  for (l=1;l<nblk+1;l++)
		    {
		      bllst12i.col1[l]=(*bllst12)[l];
		      bllst12i.col2[l]=(*bllst12)[l+nblk];
		    }
		  bllst12i.col1[nblk+1]=(*bllst12)[lb[i]];
		  bllst12i.col2[nblk+1]=(*bllst12)[lb[i]+nblk];
		  for (l=1;l<nblk+2;l++)
		    {
		      (*bllst12)[l]=bllst12i.col1[l];
		      (*bllst12)[l+nblk+1]=bllst12i.col2[l];
		    }
		  (*nzcross)[nblk+1]=(*nzcross)[lb[i]];
		  
		  (*corinvptr)[(*corinvptr)[0]+1]=(*corinvptr)[(*corinvptr)[0]]+(*corinvptr)[lb[i]+1]-(*corinvptr)[lb[i]];
		  (*corinvptr)[0]++;
		  (*corinvec)[0]=(*corinvec)[0]+(*corinvptr)[nblk+2]-(*corinvptr)[nblk+1];
		  a=(*corinvptr)[lb[i]]-(*corinvptr)[nblk+1];
		  for(l=(*corinvptr)[nblk+1];l<(*corinvptr)[nblk+2];l++)
		    {
		      (*corinvec)[l]=(*corinvec)[a+l];
		    }
		  
		  
		  if ((tmp=(int*)malloc(sizeof(int)*(1+4*indxo[0]))) == NULL ) return 0;;
		  tmp[0]=4*indxo[0];
		  for(l=1;l<indxo[0]+1;l++)
		    {
		      tmp[l]=(*clkconnect)[indxo[l]];
		      tmp[l+indxo[0]]=(*clkconnect)[indxo[l]+(*clkconnect)[0]/4];
		      tmp[l+2*indxo[0]]=(*clkconnect)[indxo[l]+(*clkconnect)[0]/2];
		      tmp[l+3*indxo[0]]=(*clkconnect)[indxo[l]+3*((*clkconnect)[0]/4)];
		    }
		  
		  leng=(*clkconnect)[0]/4;
		  if ((clkconnecti.col1=(int*)malloc(sizeof(int)*(leng+tmp[0]/4+1))) == NULL ) return 0;
		  clkconnecti.col1[0]=leng+tmp[0]/4;  
		  if ((clkconnecti.col2=(int*)malloc(sizeof(int)*(leng+1+tmp[0]/4))) == NULL ) return 0;
		  clkconnecti.col2[0]=leng+tmp[0]/4;  
		  if ((clkconnecti.col3=(int*)malloc(sizeof(int)*(leng+1+tmp[0]/4))) == NULL ) return 0;
		  clkconnecti.col3[0]=leng+tmp[0]/4;
		  if ((clkconnecti.col4=(int*)malloc(sizeof(int)*(leng+1+tmp[0]/4))) == NULL ) return 0;
		  clkconnecti.col4[0]=leng+tmp[0]/4;  
		  for(l=1;l<leng+1;l++)
		    {
		      clkconnecti.col1[l]=(*clkconnect)[l];
		      clkconnecti.col2[l]=(*clkconnect)[l+leng];
		      clkconnecti.col3[l]=(*clkconnect)[l+leng*2];
		      clkconnecti.col4[l]=(*clkconnect)[l+leng*3];
		    }
		  for(l=1;l<tmp[0]/4+1;l++)
		    {
		      clkconnecti.col1[l+leng]=nblk+1;
		      clkconnecti.col2[l+leng]=tmp[l+tmp[0]/4];
		      clkconnecti.col3[l+leng]=tmp[l+tmp[0]/2];
		      clkconnecti.col4[l+leng]=tmp[l+3*tmp[0]/4];
		    }
		  free(*clkconnect);
		  if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*leng+tmp[0]+1))) == NULL ) return 0;
		  (*clkconnect)[0]=4*leng+tmp[0];
		  if(tmp) free(tmp);
		  for(l=1;l<(*clkconnect)[0]/4+1;l++)
		    {
		      (*clkconnect)[l]=clkconnecti.col1[l];
		      (*clkconnect)[l+(*clkconnect)[0]/4]=clkconnecti.col2[l];
		      (*clkconnect)[l+(*clkconnect)[0]/2]=clkconnecti.col3[l];
		      (*clkconnect)[l+3*(*clkconnect)[0]/4]=clkconnecti.col4[l];
		    }
		  nblk++;
		  free(clkconnecti.col1);
		  clkconnecti.col1=NULL;
		  free(clkconnecti.col2);
		  clkconnecti.col2=NULL;
		  free(clkconnecti.col3);
		  clkconnecti.col3=NULL;
		  free(clkconnecti.col4);
		  clkconnecti.col4=NULL;
		} /* end for k */
	      free(bllst12i.col1);
	      free(bllst12i.col2);
	      leng=(*connectmat)[0]/4;
	      if ((connectmati.col1=(int*)malloc(sizeof(int)*(leng+nn))) == NULL ) return 0;
	      connectmati.col1[0]=leng+nn-1;  
	      if ((connectmati.col2=(int*)malloc(sizeof(int)*(leng+nn))) == NULL ) return 0;
	      connectmati.col2[0]=leng+nn-1;  
	      if ((connectmati.col3=(int*)malloc(sizeof(int)*(leng+nn))) == NULL ) return 0;
	      connectmati.col3[0]=leng+nn-1;
	      if ((connectmati.col4=(int*)malloc(sizeof(int)*(leng+nn))) == NULL ) return 0;
	      connectmati.col4[0]=leng+nn-1;  
	      for(l=1;l<leng+1;l++)
		{              
		  connectmati.col1[l]=(*connectmat)[l];
		  connectmati.col2[l]=(*connectmat)[l+leng];
		  connectmati.col3[l]=(*connectmat)[l+2*leng];
		  connectmati.col4[l]=(*connectmat)[l+3*leng];
		}   
	      for (l=1;l<nn;l++)
		{		      
		  connectmati.col1[l+leng]=connectmati.col1[indy[1]];
		  connectmati.col2[l+leng]=connectmati.col2[indy[1]];
		  connectmati.col3[l+leng]=nblkorg+l;
		  connectmati.col4[l+leng]=1;
		}
	      free(*connectmat);
	      leng=4*connectmati.col1[0];
	      if (((*connectmat)=(int*)malloc(sizeof(int)*(leng+1))) == NULL ) return 0;
	      (*connectmat)[0]=leng;
	      for(l=1;l<(*connectmat)[0]/4+1;l++)
		{
		  (*connectmat)[l]=connectmati.col1[l];
		  (*connectmat)[l+(*connectmat)[0]/4]=connectmati.col2[l];
		  (*connectmat)[l+(*connectmat)[0]/2]=connectmati.col3[l];
		  (*connectmat)[l+3*(*connectmat)[0]/4]=connectmati.col4[l];
		}
	      change=true;
	      nblkorg=nblk;
	      free(connectmati.col1);
	      connectmati.col1=NULL;
	      free(connectmati.col2);
	      connectmati.col2=NULL;
	      free(connectmati.col3);
	      connectmati.col3=NULL;
	      free(connectmati.col4);
	      connectmati.col4=NULL;
	      if (indy) {free(indy);
	      indy=NULL;}
	      if (indxo) {free(indxo);
	      indxo=NULL;}
	    }
	  free(indx);
	  indx=NULL;
	} /*fin de for i*/
      if (change) 
	{
	  *done=false;
	  free(lb);
	  free(idl);
	  free(id);  
	  return 0;
	}
      if(lb) free(lb);
      if (idl) {free(idl);
      idl=NULL;}
      free(id);  
    }
  
  clkconnecttmp=VecEg1(*clkconnect);
  a=clkconnecttmp[0]/4;
  if ((ppget=(int*)malloc(sizeof(int)*(a+1))) == NULL ) return 0;
  if ((idl=(int*)malloc(sizeof(int)*(a+1))) == NULL ) return 0;   
  j=k=1;
  for(l=1;l<a+1;l++)
    {
      if(clkconnecttmp[l] != 0) ppget[j++]=l;
      else idl[k++]=l;
    }
  ppget[0]=j-1;
  idl[0]=k-1;
  free(*clkconnect);
  if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*ppget[0]+1))) == NULL ) return 0;
  (*clkconnect)[0]=4*ppget[0];
  for(l=1;l<ppget[0]+1;l++)
    {
      (*clkconnect)[l]=clkconnecttmp[ppget[l]];
      (*clkconnect)[l+ppget[0]]=clkconnecttmp[ppget[l]+a];
      (*clkconnect)[l+2*ppget[0]]=clkconnecttmp[ppget[l]+2*a];
      (*clkconnect)[l+3*ppget[0]]=clkconnecttmp[ppget[l]+3*a];
    }
  free(ppget);
  if(!idl)
    {
      if ((idl=(int*)malloc(sizeof(int))) == NULL ) return 0;
      idl[0]=0;
    }
  if ((clkconnect0=(int*)malloc(sizeof(int)*(4*idl[0]+1))) == NULL ) return 0;
  clkconnect0[0]=4*idl[0];
  for(l=1;l<idl[0]+1;l++)
    {
      clkconnect0[l]=clkconnecttmp[idl[l]];
      clkconnect0[l+idl[0]]=clkconnecttmp[idl[l]+a];
      clkconnect0[l+2*idl[0]]=clkconnecttmp[idl[l]+2*a];
      clkconnect0[l+3*idl[0]]=clkconnecttmp[idl[l]+3*a];
    }
  if(clkconnecttmp) free(clkconnecttmp);
  free(idl);
  inter=GetPartVect(clkconnect0,1,clkconnect0[0]/4);
  if(!inter)
    {
      if ((inter=(int*)malloc(sizeof(int))) == NULL ) return 0;
      inter[0]=0;
    }
  idl=FindEg(inter,0);
  free(inter);
  inter=NULL;
  if(!idl)
    {
      if ((idl=(int*)malloc(sizeof(int))) == NULL ) return 0;
      idl[0]=0;
    }
  if ((texeclk0=(int*)malloc(sizeof(int)*(idl[0]+1))) == NULL ) return 0;
  texeclk0[0]=idl[0];
  a=clkconnect0[0]/2;
  for (l=1;l<idl[0]+1;l++)
   {
     texeclk0[l]=clkconnect0[idl[l]+a];
   } 
  free(idl);
  if ((con=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+1))) == NULL ) return 0;
  con[0]=(*clkconnect)[0]/4;
  for(l=1;l<con[0]+1;l++)
    {
      con[l]=-((*bllst5ptr)[(*clkconnect)[l]]+(*clkconnect)[l+(*clkconnect)[0]/4]-1);
    } 
  if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
  ind[0]=con[0];
  C2F(isort)(&con[1],&con[0],&ind[1]);
  Inv(con);
  clkconnectind=VecEg1(*clkconnect);
  for(l=1;l<ind[0]+1;l++)
    {
      (*clkconnect)[l]=clkconnectind[ind[l]];
      (*clkconnect)[l+ind[0]]=clkconnectind[ind[l]+ind[0]];
      (*clkconnect)[l+2*ind[0]]=clkconnectind[ind[l]+2*ind[0]];
      (*clkconnect)[l+3*ind[0]]=clkconnectind[ind[l]+3*ind[0]];      
    } 
  free(clkconnectind);
  clkconnectind=NULL;  
  free(ind);
  bexe=GetPartVect(*clkconnect,(*clkconnect)[0]/2+1,(*clkconnect)[0]/4);
  sbb=(*clkconnect)[0]/4; 
  if ((idl=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  idl[0]=sbb;
  for(l=1;l<sbb+1;l++)
    {
      idl[l]=l;
    }
  if ((w2=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  w2[0]=sbb;
  w2[1]=(*clkconnect)[1];
  for(l=1;l<sbb;l++)
    {
      w2[l+1]=(*clkconnect)[l+1]-(*clkconnect)[l];
    }
  boptr=Duplicataa(idl,w2);
  free(w2);
  w2=NULL;
  if(idl) free(idl);
  if ((boptr=(int*)realloc(boptr,sizeof(int)*(nblk+2))) == NULL ) return 0;
  a=sbb+1;  
  for (l=1;l<nblk-boptr[0]+2;l++)
    {
      boptr[l+boptr[0]]=a;
    }
  boptr[0]=nblk+1;
  ptmp1=GetPartVect(*connectmat,(*connectmat)[0]/4+1,(*connectmat)[0]/4);
  m1=Max1(ptmp1);
  if ((w1=(int*)malloc(sizeof(int)*((*connectmat)[0]/4+1))) == NULL ) return 0;
  w1[0]=(*connectmat)[0]/4;
  for (l=1;l<w1[0]+1;l++)
    {
      w1[l]=-m1*(*connectmat)[l]-(*connectmat)[l+(*connectmat)[0]/4]+1;
    }
  free (ptmp1);
  if ((ind=(int*)malloc(sizeof(int)*(w1[0]+1))) == NULL ) return 0;
  ind[0]=w1[0];
  C2F(isort)(&w1[1],&w1[0],&ind[1]);
  free(w1);
  w1=NULL;
  connectmatind=VecEg1(*connectmat);
  for(l=1;l<ind[0]+1;l++)
    {
      (*connectmat)[l]=connectmatind[ind[l]];
      (*connectmat)[l+ind[0]]=connectmatind[ind[l]+ind[0]];
      (*connectmat)[l+2*ind[0]]=connectmatind[ind[l]+2*ind[0]];
      (*connectmat)[l+3*ind[0]]=connectmatind[ind[l]+3*ind[0]];      
    } 
  free(connectmatind);
  connectmatind=NULL;
  free(ind);     
  blnk=GetPartVect(*connectmat,(*connectmat)[0]/2+1,(*connectmat)[0]/4);
  sbb=(*connectmat)[0]/4; 
  if ((idl=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  idl[0]=sbb;
  for(l=1;l<sbb+1;l++)
    {
      idl[l]=l;
    }
  if ((w2=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  w2[0]=sbb;
  w2[1]=(*connectmat)[1];
  for(l=1;l<sbb;l++)
    {
      w2[l+1]=(*connectmat)[l+1]-(*connectmat)[l];
    }
  blptr=Duplicataa(idl,w2);
  free(w2);
  free(idl);
  idl=NULL;
  if ((blptr=(int*)realloc(blptr,sizeof(int)*(nblk+2))) == NULL ) return 0;
  a=sbb+1;
  for (l=1;l<nblk-blptr[0]+2;l++)
    {
      blptr[l+blptr[0]]=a;
    }
  blptr[0]=nblk+1;  
  if ((w1=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+1))) == NULL ) return 0;
  w1[0]=(*clkconnect)[0]/4;  
  w2=GetPartVect(*clkconnect,1,(*clkconnect)[0]/4);
  for (l=1;l<w1[0]+1;l++)
    {
      w1[l]=typ_l[w2[l]];
    }
  free(w2);
  w2=FindEg(w1,0);
  free(w1);
  w1=NULL;
  if ((tclkconnect=(int*)malloc(sizeof(int)*(4*w2[0]+1))) == NULL ) return 0;
  tclkconnect[0]=4*w2[0];
  for(l=1;l<w2[0]+1;l++)
    {    
      tclkconnect[l]=(*clkconnect)[w2[l]];
      tclkconnect[l+w2[0]]=(*clkconnect)[w2[l]+(*clkconnect)[0]/4];
      tclkconnect[l+2*w2[0]]=(*clkconnect)[w2[l]+(*clkconnect)[0]/2];
      tclkconnect[l+3*w2[0]]=(*clkconnect)[w2[l]+3*(*clkconnect)[0]/4];
    }  
  if(w2) free(w2);
  w2=NULL;
  if ((tcon=(int*)malloc(sizeof(int)*(tclkconnect[0]/4+1))) == NULL ) return 0;
  tcon[0]=tclkconnect[0]/4;
  w1=GetPartVect(tclkconnect,1,tclkconnect[0]/4);
  w2=GetPartVect(tclkconnect,tclkconnect[0]/4+1,tclkconnect[0]/4);
  for(l=1;l<tcon[0]+1;l++)
    {
      tcon[l]=(*bllst5ptr)[w1[l]]+w2[l]-1;
    }
  free(w1);
  w1=NULL;
  free(w2);
  w2=NULL;
  sbb=tcon[0];
  if ((idl=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  idl[0]=sbb;
  for(l=1;l<sbb+1;l++)
    {
      idl[l]=l;
    }
  if ((w2=(int*)malloc(sizeof(int)*(sbb+1))) == NULL ) return 0;
  w2[0]=sbb;
  w2[1]=tcon[1];
  for(l=1;l<sbb;l++)
    {
      w2[l+1]=tcon[l+1]-tcon[l];
    }
  free(tcon);
  ordptr1=Duplicataa(idl,w2);
  free(w2);
  w2=NULL;
  free(idl);
  if ((ordptr1=(int*)realloc(ordptr1,sizeof(int)*((*bllst5ptr)[(*bllst5ptr)[0]-nn+1]+1))) == NULL ) return 0;
  a=sbb+1;
  for(l=1;l<(*bllst5ptr)[(*bllst5ptr)[0]-nn+1]-ordptr1[0]+1;l++)
    {
      ordptr1[l+ordptr1[0]]=a;
    }
  ordptr1[0]=(*bllst5ptr)[(*bllst5ptr)[0]-nn+1];
  leng=clkconnect0[0]/4;
  if ((clkconnecti.col1=(int*)malloc(sizeof(int)*(leng+(*clkconnect)[0]/4+1))) == NULL ) return 0;
  clkconnecti.col1[0]=leng+(*clkconnect)[0]/4;  
  if ((clkconnecti.col2=(int*)malloc(sizeof(int)*(leng+(*clkconnect)[0]/4+1))) == NULL ) return 0;
  clkconnecti.col2[0]=leng+(*clkconnect)[0]/4;  
  if ((clkconnecti.col3=(int*)malloc(sizeof(int)*(leng+(*clkconnect)[0]/4+1))) == NULL ) return 0;
  clkconnecti.col3[0]=leng+(*clkconnect)[0]/4;
  if ((clkconnecti.col4=(int*)malloc(sizeof(int)*(leng+(*clkconnect)[0]/4+1))) == NULL ) return 0;
  clkconnecti.col4[0]=leng+(*clkconnect)[0]/4;  

  for(l=1;l<clkconnect0[0]/4+1;l++)
    {     
      clkconnecti.col1[l]=clkconnect0[l];
      clkconnecti.col2[l]=clkconnect0[l+clkconnect0[0]/4];
      clkconnecti.col3[l]=clkconnect0[l+clkconnect0[0]/2];
      clkconnecti.col4[l]=clkconnect0[l+3*clkconnect0[0]/4];
    }
  for(l=1;l<(*clkconnect)[0]/4+1;l++)
    {
      clkconnecti.col1[l+clkconnect0[0]/4]=(*clkconnect)[l];
      clkconnecti.col2[l+clkconnect0[0]/4]=(*clkconnect)[l+(*clkconnect)[0]/4];
      clkconnecti.col3[l+clkconnect0[0]/4]=(*clkconnect)[l+(*clkconnect)[0]/2];
      clkconnecti.col4[l+clkconnect0[0]/4]=(*clkconnect)[l+3*(*clkconnect)[0]/4];
    }
  free(*clkconnect);
  if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*clkconnecti.col4[0]+1))) == NULL ) return 0;
  (*clkconnect)[0]=4*clkconnecti.col4[0];
  for(l=1;l<clkconnecti.col4[0]+1;l++)
    {
      (*clkconnect)[l]=clkconnecti.col1[l];
      (*clkconnect)[l+clkconnecti.col4[0]]=clkconnecti.col2[l];
      (*clkconnect)[l+(*clkconnect)[0]/2]=clkconnecti.col3[l];
      (*clkconnect)[l+3*(*clkconnect)[0]/4]=clkconnecti.col4[l];
    }
  free(clkconnecti.col1);
  free(clkconnecti.col2);
  free(clkconnecti.col3);
  free(clkconnecti.col4);
  con1=VecEg1(con);
  free(con);
  if ((con=(int*)malloc(sizeof(int)*(con1[0]+clkconnect0[0]/4+1))) == NULL ) return 0;
  con[0]=con1[0]+clkconnect0[0]/4;
  pp=&con[clkconnect0[0]/4+1];
  pp=memcpy(pp,&con1[1],sizeof(int)*(con1[0]));
  free(con1);
  for(l=1;l<clkconnect0[0]/4+1;l++)
    {
      con[l]=0;     
    }
  free(clkconnect0);  
  /* ---------------- */
  while ( *prime_pointer <= (*prime_act)[0]){
    o=(*prime_act)[*prime_pointer];
    (*prime_pointer)++;
    if (o==0)
      {        
	texeclki=VecEg1(texeclk0);
	free(texeclk0);
      }
    else
      {
	w1=GetPartVect(tclkconnect,tclkconnect[0]/2+1,tclkconnect[0]/4);
	texeclki=GetPartVect(w1,ordptr1[o],ordptr1[o+1]-ordptr1[o]);
	if(w1) free(w1);
      }
    if (!texeclki)
      {
	if ((texeclki=(int*)malloc(sizeof(int))) == NULL ) return 0;
	texeclki[0]=0;
      }
    if ((idl=(int*)malloc(sizeof(int)*(texeclki[0]+1))) == NULL ) return 0;
    idl[0]=texeclki[0];
    for(l=1;l<texeclki[0]+1;l++)
      {
	idl[l]=typ_l[texeclki[l]];
      }
    if ( OR(idl) )
      {
	if ((vec=(int*)malloc(sizeof(int)*(nblk+1))) == NULL ) return 0;
	vec[0]=nblk;
	Setmem(vec,-1);
	for (l=1;l<texeclki[0]+1;l++)
	  {
	    vec[texeclki[l]]=0;
	  }
	if ((typ_lm=(int*)malloc(sizeof(int)*(typ_l[0]+1))) == NULL ) return 0;
	typ_lm[0]=typ_l[0];
	for(l=1;l<typ_l[0]+1;l++)
	  {
	    if (typ_l[l]) typ_lm[l]=1;
	    else 
	      {
		if (typ_m[l]) typ_lm[l]=-1;
		else typ_lm[l]=0;
	      }
	  }
	tree3(vec,vec[0],*bllst12,typ_lm,bexe,boptr,blnk,blptr,&r,okk);
	free(typ_lm);
	free(vec);
	if (!*okk)
	  {
	    Message("Algebraic loop detected; cannot be compiled.");
	    *bllst2=NULL;*bllst3=NULL;**bllst10=NULL;
	    *ok=false;
	    *done=true;	    
	    return 0;
	  }
	pointer=FindEg(con,o);
	for(j=1;j<r[0]+1;j++)
	  {
	    if (typ_l[r[j]])
	      {
		
		*((*bllst10)[r[j]])='s';
		typ_l[r[j]]=false;
		
		if ((vec=malloc(sizeof(int)*((*bllst5ptr)[r[j]+1]-(*bllst5ptr)[r[j]]+1)))== NULL ) return 0;
		vec[0]=(*bllst5ptr)[r[j]+1]-(*bllst5ptr)[r[j]];
		if ((*prime_act=(int*)realloc(*prime_act,sizeof(int)*((*prime_act)[0]+vec[0]+1))) == NULL ) return 0;      
		for (l = 1; l <= vec[0]; l++){
		  (*prime_act)[(*prime_act)[0]+l]=(*bllst5ptr)[r[j]]+l-1;
		}
		(*prime_act)[0]=(*prime_act)[0]+vec[0];
		free(vec);		
		
		w1=GetCollVect(*clkconnect,pointer,3);                            
		w2=FindDif(w1,r[j]);
		pointer1=VecEg1(pointer);
		if(pointer) free(pointer);
		pointer=NULL;
		if ((pointer=(int*)malloc(sizeof(int)*(w2[0]+1))) == NULL ) return 0;
		pointer[0]=w2[0];
		for(l=1;l<w2[0]+1;l++)
		  {
		    pointer[l]=pointer1[w2[l]];
		  }
		if(w1) free(w1);
		w1=NULL;
		if(w2) free(w2);
		free(pointer1);
		w1=GetCollVect(*clkconnect,pointer,3);
		w3=GetCollVect(*clkconnect,pointer,4);        
		for (l=1;l<pointer[0]+1;l++)
		  {
		    (*clkconnect)[pointer[l]]=r[j];
		    (*clkconnect)[pointer[l]+(*clkconnect)[0]/4]=1;
		    (*clkconnect)[pointer[l]+(*clkconnect)[0]/2]=w1[l];
		    (*clkconnect)[pointer[l]+3*(*clkconnect)[0]/4]=w3[l];
		  }
		if(w1) free(w1);
		if(w3) free(w3);
		for (bl_out=2;bl_out<(*bllst5ptr)[r[j]+1]-(*bllst5ptr)[r[j]]+1;bl_out++)
		  {
		    leng=(*clkconnect)[0]/4;
		    if ((clkconnecti.col1=(int*)malloc(sizeof(int)*(leng+pointer[0]+1))) == NULL ) return 0;
		    clkconnecti.col1[0]=leng+pointer[0];  
		    if ((clkconnecti.col2=(int*)malloc(sizeof(int)*(leng+pointer[0]+1))) == NULL ) return 0;
		    clkconnecti.col2[0]=leng+pointer[0];  
		    if ((clkconnecti.col3=(int*)malloc(sizeof(int)*(leng+pointer[0]+1))) == NULL ) return 0;
		    clkconnecti.col3[0]=leng+pointer[0];
		    if ((clkconnecti.col4=(int*)malloc(sizeof(int)*(leng+pointer[0]+1))) == NULL ) return 0;
		    clkconnecti.col4[0]=leng+pointer[0];  
		    for(l=1;l<(*clkconnect)[0]/4+1;l++)
		      {     
			clkconnecti.col1[l]=(*clkconnect)[l];
			clkconnecti.col2[l]=(*clkconnect)[l+(*clkconnect)[0]/4];
			clkconnecti.col3[l]=(*clkconnect)[l+(*clkconnect)[0]/2];
			clkconnecti.col4[l]=(*clkconnect)[l+3*(*clkconnect)[0]/4];
		      }
		    w1=GetCollVect(*clkconnect,pointer,3);
		    w3=GetCollVect(*clkconnect,pointer,4);    
		    for(l=1;l<pointer[0]+1;l++)
		      {
			clkconnecti.col1[l+(*clkconnect)[0]/4]=r[j];
			clkconnecti.col2[l+(*clkconnect)[0]/4]=bl_out;
			clkconnecti.col3[l+(*clkconnect)[0]/4]=w1[l];
			clkconnecti.col4[l+(*clkconnect)[0]/4]=w3[l];
		      }
		    if(w1) free(w1);
		    if(w3) free(w3);
		    free(*clkconnect);
		    if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*clkconnecti.col1[0]+1))) == NULL ) return 0;
		    (*clkconnect)[0]=4*clkconnecti.col1[0];
		    for(l=1;l<(*clkconnect)[0]/4+1;l++)
		      {
			(*clkconnect)[l]=clkconnecti.col1[l];
			(*clkconnect)[l+(*clkconnect)[0]/4]=clkconnecti.col2[l];
			(*clkconnect)[l+(*clkconnect)[0]/2]=clkconnecti.col3[l];
			(*clkconnect)[l+3*(*clkconnect)[0]/4]=clkconnecti.col4[l];
		      }
		    free(clkconnecti.col1);
		    clkconnecti.col1=NULL;
		    free(clkconnecti.col2);
		    clkconnecti.col2=NULL;
		    free(clkconnecti.col3);
		    clkconnecti.col3=NULL;
		    free(clkconnecti.col4);
		    clkconnecti.col4=NULL;             
		  }
		*need_newblk=true;
		*ok=true;
		*done=false;
		free(r);
		free(pointer);
		free(idl);
		free (texeclki);
		free(bexe);
		free(blnk);  
		free(boptr);
		free(blptr);
		free(tclkconnect);  
		free(ordptr1);  
		free(con);
		return 0;
	      }
	    else
	      {
		w1=GetCollVect(*clkconnect,pointer,3);
		w2=FindDif(w1,r[j]);
		if (w1) free(w1);
		pointer1=VecEg1(pointer);
		free(pointer);
		if ((pointer=(int*)malloc(sizeof(int)*(w2[0]+1))) == NULL ) return 0;
		pointer[0]=w2[0];
		for(l=1;l<w2[0]+1;l++)
		  {
		    pointer[l]=pointer1[w2[l]];
		  }   
		free(pointer1);
		if (w2) free(w2);
	      }
	  } /*fin de for bl=r*/
	free(r);
      }  /* fin de test */
    if (pointer && pointer[0] != 0) 
      {
	free(pointer);
	sciprint("warning(problem1)");
	return 0;
      }
    free(idl);
    free (texeclki);
  } /* fin for o */
  if ( OR(typ_l) ) 
    {
      sciprint("warning(problem2)");
    }
  
  *ok=true;
  *done=true;
  free(bexe);
  free(blnk);  
  free(boptr);
  free(blptr);
  free(tclkconnect);  
  free(ordptr1);  
  free(con);
  
  return 0;
} /* end function */
/* ======================================= endfunction paksazi =============================================== */
/* **************************************** function discard ************************************************* */
void *discard(int* bllst5ptr,int* clkconnect,int* exe_cons,int** ordptr1,int** execlk,int** clkconnectj0,
  int** clkconnectj_cons )
{
  /****** function discard1 a faire ********** */ 
  int mma,j,m,l,a,b,k,sum;
  int *clkconnectj,*con,*ind,*con1,*iini,*clkconnectj1,*clkconnecttmp,*clkconnect0;
  int *clkconnectjj,*new_ordptr1,*clkconnectjind,*clkconnectj3,*clkconnectj4;
  int *clkconnectjind3,*clkconnectjind4;
  double *clkconnectjd;
  int *ppget;
  Mat2C execlki;
  *clkconnectj0=NULL;
  *clkconnectj_cons=NULL;
  clkconnectjj=NULL;
  execlki.col1=NULL;
  execlki.col2=NULL;
  *execlk=NULL;
  clkconnectjd=NULL;
  con=NULL;
  ind=NULL;
  con1=NULL;
  iini=NULL;
  clkconnectj1=NULL;
  clkconnecttmp=NULL;
  clkconnect0=NULL;
  clkconnectj=NULL;
  clkconnectjind=NULL;
  execlki.col1=NULL;
  execlki.col2=NULL;
  clkconnectj3=NULL;
  clkconnectj4=NULL;
  clkconnectjind3=NULL;
  clkconnectjind4=NULL;
  if (exe_cons && exe_cons[0] !=0 )
    {
      a=exe_cons[0]/2;
      ppget=GetPartVect(exe_cons,a+1,a);
      mma=Max1(ppget)+1;
      free(ppget);
      if ((con=(int*)malloc(sizeof(int)*(a+1))) == NULL) return 0;
      con[0]=a;
      for(j=1;j<a+1;j++)
        {
          con[j]=-mma*exe_cons[j]-exe_cons[j+a];
        } 
      if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
      ind[0]=con[0];
      C2F(isort)(&con[1],&con[0],&ind[1]); 
      Inv(con);
      clkconnectjind=VecEg1(exe_cons);
      for(j=1;j<ind[0]+1;j++)
        {
          exe_cons[j]=clkconnectjind[ind[j]];
          exe_cons[j+ind[0]]=clkconnectjind[ind[j]+ind[0]];
        }

      free(clkconnectjind);
      clkconnectjind=NULL;
      free(ind);
      ind=NULL;
      if (con[0]>=2)
        {
          if ((con1=(int*)malloc(sizeof(int)*(con[0]))) == NULL) return 0;
	  k=1;
          for(j=1; j < con[0]; j++)
           {
             if ((con[j+1] - con[j]) == 0) con1[k++]=j;
           }
	  con1[0]=k-1;
          if (con1)
            {
             for(j=con1[0];j>0;j--)
               {
                 Incr2(exe_cons,con1[j]);
               }
	     free(con1);con1=NULL;
	    }      
        }
      free(con);con=NULL;
      a=exe_cons[0]/2;
      if ((clkconnectjd=(double*)malloc(sizeof(double)*(a+1))) == NULL) return 0;
      clkconnectjd[0]=a;
      for(j=1;j<a+1;j++)
        {

          clkconnectjd[j]=powerd(2,exe_cons[j+a]-1);
          exe_cons[j+a]=(int) clkconnectjd[j];
        }
      free(clkconnectjd);
      clkconnectjd=NULL;
      con=GetPartVect(exe_cons,1,a);  
      if (con[0]>=2)
        {
          if ((iini=(int*)malloc(sizeof(int)*(con[0]))) == NULL) return 0;
	  k=1;
          for(j=1; j < con[0]; j++)
            {
              if((con[j+1]-con[j]) != 0) iini[k++]=j;
            }
	  iini[0]=k-1;
          if ((iini=(int*)realloc(iini,sizeof(int)*(iini[0]+2))) == NULL) return 0;
          iini[iini[0]+1]=a;
          iini[0]++;
	}
      else
        {
	  if ((iini=(int*)malloc(sizeof(int)*(2))) == NULL) return 0;
          iini[0]=1;
          iini[1]=1;                          
        }
      if (((*clkconnectj_cons)=(int*)malloc(sizeof(int)*(2*iini[0]+1))) == NULL) return 0;
      (*clkconnectj_cons)[0]=2*iini[0];
      
      for (j=1;j<iini[0]+1;j++)
        {
	  ind=FindEg(con,con[iini[j]]);
          (*clkconnectj_cons)[j]=con[iini[j]];
	  sum=0;
	  for(l=1; l < ind[0]+1; l++)
	    {
	      sum+=exe_cons[ind[l]+a];
	    }   
          
	  (*clkconnectj_cons)[j+iini[0]]=sum;
	  free(ind);ind=NULL;  
        }
      free(con); con=NULL;
      free(iini); iini=NULL;
    } /* fin de if (exe_cons) */
    else
      {
        if ((*clkconnectj_cons))
          {
            free((*clkconnectj_cons));
            (*clkconnectj_cons)=NULL;
          }
      }
  /* *** fin de function discard1 ***** */
  if(clkconnect[0] !=0)
    {
      clkconnecttmp=VecEg1(clkconnect);
      a=clkconnecttmp[0]/4;
      ppget=GetPartVect(clkconnecttmp,1,a);
      ind=FindDif(ppget,0);
      free(ppget);
      if ((clkconnect=(int*)malloc(sizeof(int)*(4*ind[0]+1))) == NULL) return 0;
      clkconnect[0]=4*ind[0];
      for(l=1;l<ind[0]+1;l++)
	{
	  clkconnect[l]=clkconnecttmp[ind[l]];
	  clkconnect[l+ind[0]]=clkconnecttmp[ind[l]+a];
	  clkconnect[l+2*ind[0]]=clkconnecttmp[ind[l]+clkconnecttmp[0]/2];
	  clkconnect[l+3*ind[0]]=clkconnecttmp[ind[l]+3*a];
	}
      if(ind) free(ind);
      ind=NULL;
      ppget=GetPartVect(clkconnecttmp,1,a);
      ind=FindEg(ppget,0); 
      free(ppget);
      if (ind)
	{
	  if ((clkconnect0=(int*)malloc(sizeof(int)*(4*ind[0]+1))) == NULL) return 0;
	  clkconnect0[0]=4*ind[0];
	  for(l=1;l<ind[0]+1;l++)
	    {
	      clkconnect0[l]=clkconnecttmp[ind[l]];
	      clkconnect0[l+ind[0]]=clkconnecttmp[ind[l]+a];
	      clkconnect0[l+2*ind[0]]=clkconnecttmp[ind[l]+clkconnecttmp[0]/2];
	      clkconnect0[l+3*ind[0]]=clkconnecttmp[ind[l]+3*a];
	    }
	  free(ind);
	  ind=NULL;
	}    
      free(clkconnecttmp);clkconnecttmp=NULL;
      if (clkconnect0 && clkconnect0[0] != 0)
	{
	  a=clkconnect0[0]/4;
	  clkconnectj3=GetPartVect(clkconnect0,a+a+1,a);                      
	  clkconnectj4=GetPartVect(clkconnect0,3*a+1,a);
	  mma=Max1(clkconnectj4)+1;
	  if ((con=(int*)malloc(sizeof(int)*(a+1))) == NULL) return 0;
	  con[0]=a;
	  for(j=1;j<a+1;j++)
	    {
	      con[j]=-mma*clkconnectj3[j]-clkconnectj4[j];
	    } 
	  if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
	  ind[0]=con[0];
	  C2F(isort)(&con[1],&con[0],&ind[1]); 
	  Inv(con);
	  clkconnectjind3=VecEg1(clkconnectj3);
	  clkconnectjind4=VecEg1(clkconnectj4);
	  for(j=1;j<ind[0]+1;j++)
	    {
	      clkconnectj3[j]=clkconnectjind3[ind[j]];
	      clkconnectj4[j]=clkconnectjind4[ind[j]];
	    }
	  free(clkconnectjind3); clkconnectjind3=NULL;
	  free(clkconnectjind4); clkconnectjind4=NULL;
	  free(ind); ind=NULL;
	  if (con[0]>=2)
	    {
	      if ((con1=(int*)malloc(sizeof(int)*(con[0]))) == NULL) return 0;
	      k=1;
	      for (j = 1; j< con[0]; j++)
		{
		  if ((con[j+1] - con[j]) == 0) con1[k++]=j;
		}
	      con1[0]=k-1;
	      if (con1)
		{
		  for(j=con1[0];j>0;j--)
		    {
		      Incr1(clkconnectj3,con1[j]);
		      Incr1(clkconnectj4,con1[j]);
		    }
		  free(con1);con1=NULL;
		}     
	    }
	  a=clkconnectj3[0];
	  free(con);con=NULL;
	  if ((clkconnectjd=(double*)malloc(sizeof(double)*(a+1))) == NULL) return 0;
	  clkconnectjd[0]=a;
	  for(j=1;j<a+1;j++)
	    {
	      clkconnectjd[j]=powerd(2,clkconnectj4[j]-1);
	      clkconnectj4[j]=(int) clkconnectjd[j];  
	    }
	  
	  free(clkconnectjd); clkconnectjd=NULL;
	  if (clkconnectj3[0]>=2)
	    {
	      if ((iini=(int*)malloc(sizeof(int)*(a))) == NULL) return 0;
	      k=1;
	      for(j=1;j<a;j++)
		{
		  if ((clkconnectj3[j+1]-clkconnectj3[j]) != 0) iini[k++]=j;
		}
	      iini[0]=k-1;
	      if ((iini=(int*)realloc(iini,sizeof(int)*(iini[0]+2))) == NULL) return 0;
	      iini[iini[0]+1]=a;
	      iini[0]++;
	    }
	  else
	    {
	      if ((iini=(int*)malloc(sizeof(int)*(2))) == NULL) return 0;
	      iini[0]=1;
	      iini[1]=1;                           
	    }
	  if (((*clkconnectj0)=(int*)malloc(sizeof(int)*(2*iini[0]+1))) == NULL) return 0;
	  (*clkconnectj0)[0]=2*iini[0];
	  for (j=1;j<iini[0]+1;j++)
	    {
	      ind=FindEg(clkconnectj3,clkconnectj3[iini[j]]);
	      (*clkconnectj0)[j]=clkconnectj3[iini[j]];
	      sum=0;
	      for (l=1; l <= ind[0]; l++)
		{
		  sum+=clkconnectj4[ind[l]];
		}
	      (*clkconnectj0)[j+iini[0]]=sum;
	      free(ind);ind=NULL; 
	    }
	  
	  free(con);con=NULL;
	  free(iini);iini=NULL;
	  free(clkconnectj3);clkconnectj3=NULL;
	  free(clkconnectj4);clkconnectj4=NULL;
	} /* fin de if (clkconnect0) */
      else
	{
	  if ((*clkconnectj0))
	    {
	      free((*clkconnectj0));
	      (*clkconnectj0)=NULL;
	    }
	}
      free(clkconnect0);clkconnect0=NULL;
      a=clkconnect[0]/4;
      if ((con=(int*)malloc(sizeof(int)*(a+1))) == NULL) return 0;
      con[0]=a;
      for(l=1;l<a+1;l++)
	{
	  con[l]=-bllst5ptr[clkconnect[l]]-clkconnect[l+a]+1;
	}
      if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
      ind[0]=con[0];
      C2F(isort)(&con[1],&con[0],&ind[1]); 
      Inv(con);
      clkconnectjind=VecEg1(clkconnect);
      for(j=1;j<ind[0]+1;j++)
	{
	  clkconnect[j]=clkconnectjind[ind[j]];
	  clkconnect[j+ind[0]]=clkconnectjind[ind[j]+ind[0]];
	  clkconnect[j+2*ind[0]]=clkconnectjind[ind[j]+2*ind[0]];
	  clkconnect[j+3*ind[0]]=clkconnectjind[ind[j]+3*ind[0]];
	}
      
      free(clkconnectjind);
      clkconnectjind=NULL;
      free(ind);ind=NULL;
    } /*fin de if (clkconnect[0]!=0) */
  else
    {
      if ((con=(int*)malloc(sizeof(int))) == NULL) return 0;
      con[0]=0;
    }
  if (((*ordptr1)=(int*)malloc(sizeof(int)*(bllst5ptr[bllst5ptr[0]]+1))) == NULL) return 0;
  (*ordptr1)[0]=bllst5ptr[bllst5ptr[0]];
  (*ordptr1)[1]=1;
  for (j=1;j<bllst5ptr[bllst5ptr[0]];j++)
    {
      con1=FindInfEg(con,j);
      if ((con1 == NULL) || (con1[0] == 0 ))
	{         
	  (*ordptr1)[j+1]=(*ordptr1)[j];
	}
      else
	{
	  (*ordptr1)[j+1]=Max1(con1)+1;
	  free(con1);con1=NULL;
	}
      
    }
  
  if ((new_ordptr1=(int*)malloc(sizeof(int)*(bllst5ptr[bllst5ptr[0]]+1))) == NULL) return 0;
  new_ordptr1[0]=bllst5ptr[bllst5ptr[0]];
  new_ordptr1[1]=1;
  free(con); con=NULL;
  if (((*execlk)=(int*)malloc(sizeof(int))) == NULL) return 0;
  (*execlk)[0]=0;
  for (m=1;m<bllst5ptr[bllst5ptr[0]];m++)
    {
      if ((*ordptr1)[m]!=(*ordptr1)[m+1])
	{
	  clkconnectj3=GetPartVect(clkconnect,(*ordptr1)[m]+a+a,(*ordptr1)[m+1]-(*ordptr1)[m]);
	  clkconnectj4=GetPartVect(clkconnect,(*ordptr1)[m]+3*a,(*ordptr1)[m+1]-(*ordptr1)[m]);
	  
       /* **************** debut de funct descard1 ************ */
	  mma=Max1(clkconnectj4)+1;
	  if ((con=(int*)malloc(sizeof(int)*(clkconnectj4[0]+1))) == NULL) return 0;
	  con[0]=clkconnectj4[0];
	  
	  for(l=1;l<con[0]+1;l++)
	    {
	      con[l]=-mma*clkconnectj3[l]-clkconnectj4[l];
	    } 
	 if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
	 ind[0]=con[0];
	 C2F(isort)(&con[1],&con[0],&ind[1]); 
	 Inv(con);
         clkconnectjind3=VecEg1(clkconnectj3);
	 clkconnectjind4=VecEg1(clkconnectj4);
         for(j=1;j<ind[0]+1;j++)
           {
             clkconnectj3[j]=clkconnectjind3[ind[j]];
             clkconnectj4[j]=clkconnectjind4[ind[j]];
           }
         free(clkconnectjind3); clkconnectjind3=NULL;
	 free(clkconnectjind4); clkconnectjind4=NULL;
         free(ind); ind=NULL;
         if (con[0]>=2)
           {
             if ((con1=(int*)malloc(sizeof(int)*(con[0]))) == NULL) return 0;
	     k=1;
             for(j = 1;j < con[0]; j++)
               {
                 if ((con[j+1] - con[j]) == 0) con1[k++]=j;
               }
	     con1[0]=k-1;
             if (con1)
               {
                 for(j=con1[0];j>0;j--)
                   {
                     Incr1(clkconnectj3,con1[j]);
		     Incr1(clkconnectj4,con1[j]);
                   }
		 free(con1);con1=NULL;
               }        
	   }
	 free(con);con=NULL;
         b=clkconnectj3[0];
         if ((clkconnectjd=(double*)malloc(sizeof(double)*(b+1))) == NULL) return 0;
	 clkconnectjd[0]=b;
         for(j=1;j<b+1;j++)
           {
             clkconnectjd[j]=powerd(2,clkconnectj4[j]-1);
             clkconnectj4[j]=(int) clkconnectjd[j];      
           }
         free(clkconnectjd); clkconnectjd=NULL;
         if (clkconnectj3[0]>=2)
           {
             if ((iini=(int*)malloc(sizeof(int)*(clkconnectj3[0]))) == NULL) return 0;
             k=1;
             for(j=1; j < b; j++)
               {
                 if((clkconnectj3[j+1]-clkconnectj3[j]) != 0) iini[k++]=j;
               }
	     iini[0]=k-1;
             if ((iini=(int*)realloc(iini,sizeof(int)*(iini[0]+2))) == NULL) return 0;
             iini[iini[0]+1]=b;
             iini[0]++;
           }
         else
           {
             if ((iini=(int*)malloc(sizeof(int)*(2))) == NULL) return 0;
             iini[0]=1;
             iini[1]=1;                       
           }
	 if ((clkconnectjj=(int*)malloc(sizeof(int)*(2*iini[0]+1))) == NULL) return 0;
         clkconnectjj[0]=2*iini[0];
         for (j=1;j<iini[0]+1;j++)
           {
	     ind=FindEg(clkconnectj3,clkconnectj3[iini[j]]);
	     clkconnectjj[j]=clkconnectj3[iini[j]];
	     sum=0;
	     for (l=1; l <= ind[0]; l++)
	       {
		 sum+=clkconnectj4[ind[l]];
	       }
             clkconnectjj[j+iini[0]]=sum;
	     free(ind); ind=NULL;       
           }
	 
	 free(con);con=NULL;
	 free(iini);iini=NULL;
	 free(clkconnectj3);clkconnectj3=NULL;
	 free(clkconnectj4);clkconnectj4=NULL;
       } /* fin de if */
     else
       {
         if (clkconnectjj)
           {
             free(clkconnectjj);
             clkconnectjj=NULL;
           }  
       }     

     /* *** fin de function discard1 ***** */
     if(clkconnectjj)
       {
         if (((*execlk)=(int*)realloc((*execlk),sizeof(int)*((*execlk)[0]+clkconnectjj[0]+1))) == NULL ) return 0;
         if ((execlki.col1=(int*)malloc(sizeof(int)*((*execlk)[0]/2+clkconnectjj[0]/2+1))) == NULL) return 0;
         execlki.col1[0]=(*execlk)[0]/2+clkconnectjj[0]/2;
         if ((execlki.col2=(int*)malloc(sizeof(int)*((*execlk)[0]/2+clkconnectjj[0]/2+1))) == NULL) return 0;
         execlki.col2[0]=(*execlk)[0]/2+clkconnectjj[0]/2;
	 for(l=1;l<(*execlk)[0]/2+1;l++)
           {
             execlki.col1[l]=(*execlk)[l];
             execlki.col2[l]=(*execlk)[l+(*execlk)[0]/2];
           }
	 for(l=1;l<clkconnectjj[0]/2+1;l++)
           {
             execlki.col1[l+(*execlk)[0]/2]=clkconnectjj[l];
             execlki.col2[l+(*execlk)[0]/2]=clkconnectjj[l+clkconnectjj[0]/2];
           }

         (*execlk)[0]=clkconnectjj[0]+(*execlk)[0];
         for(l=1;l<(*execlk)[0]/2+1;l++)
           {
             (*execlk)[l]=execlki.col1[l];
             (*execlk)[l+(*execlk)[0]/2]=execlki.col2[l];
           }
         if(execlki.col1) free(execlki.col1);
         if(execlki.col2) free(execlki.col2);
         
         new_ordptr1[m+1]=new_ordptr1[m]+clkconnectjj[0]/2;
         free(clkconnectjj);
         clkconnectjj=NULL;
       }
     else
       {
         new_ordptr1[m+1]=new_ordptr1[m];
       }
     
    } /* for m */
  free( *ordptr1);
  *ordptr1=new_ordptr1;
  if (clkconnect[0] != 0) free(clkconnect);
  return NULL;
}/* fin de funct */

/* ======================================= endfunction discard =============================================== */
/* *************************************** function extract_info ******************************************** */
int extract_info(int* bllst2,int* bllst3,int* bllst5,char **bllst10,double* bllst11,int* bllst12,int* bllst2ptr,
		 int* bllst3ptr,int* bllst4ptr,int* bllst5ptr,int* bllst11ptr,int* connectmat,int* clkconnect,
		 int** lnkptr,int** inplnk,int** outlnk,int* nzcross,int* nmode,int** typ_s,int** typ_x,
		 int** typ_m,double** initexe,int** bexe,int** boptr,int** blnk,int** blptr,int* ok,
		 int* corinvec,int* corinvptr)
{
  int j,l,ko,ki,nlnk,ptlnk,siz_unco,m1,n,jj,a,nbl=((int*)bllst10)[0];
  int *prt,*clkconnecttmp,*fff,*clkconnectind;
  int *idl,*ind,*con,*unco,*lnkbsz,*ppget,*mm;
  double *ll11;
  Mat3C initexei;
  ind=NULL;
  idl=NULL;
  ll11=NULL;
  clkconnecttmp=NULL;
  *ok=true;
  prt=NULL;
  if ((fff=(int*)malloc(sizeof(int)*(nbl+1))) == NULL) return 0;
  fff[0]=nbl;
  Setmem(fff,1);

  *typ_s=VecEg1(fff);
  *typ_m=VecEg1(fff);
  free(fff); fff=NULL;
  adjust_inout(bllst2,bllst3,bllst2ptr,bllst3ptr,nzcross,nmode,connectmat,ok,corinvec,corinvptr,nbl);
  if(!*ok) return 0;

  if (((*initexe)=(double*)malloc(sizeof(double))) == NULL) return 0;
  (*initexe)[0]=0;                
  
  for (j=1;j<nbl+1;j++)
    {
      if (nmode[j] < 0){
	Message("Number of modes in block j cannot be determined");	
	*ok=false;
      }
      if (*(bllst10[j]) == 's' ) (*typ_s)[j]=1;
      else (*typ_s)[j]=0;
      if (*(bllst10[j]) == 'x' ) (*typ_x)[j]=1;
      if (*(bllst10[j])=='m') (*typ_m)[j]=1;
      else (*typ_m)[j]=0;
      fff=GetPartVect(bllst5,bllst5ptr[j],bllst5ptr[j+1]-bllst5ptr[j]);
      if (fff) 
        {
          ll11=GetPartVectd(bllst11,bllst11ptr[j],bllst11ptr[j+1]-bllst11ptr[j]);
          if(ll11)
            {
	      prt=FindSupEgd(ll11,0);
	  
              if ((initexei.col1=(double*)malloc(sizeof(double)*((int) ((*initexe)[0])/3+prt[0]+1))) == NULL) return 0;
	      initexei.col1[0]=(int) ((*initexe)[0])/3+prt[0];
              if ((initexei.col2=(double*)malloc(sizeof(double)*((int) ((*initexe)[0])/3+prt[0]+1))) == NULL) return 0;
	      initexei.col2[0]=(int) ((*initexe)[0])/3+prt[0];
              if ((initexei.col3=(double*)malloc(sizeof(double)*((int) ((*initexe)[0])/3+prt[0]+1))) == NULL) return 0;
	      initexei.col3[0]=(int) ((*initexe)[0])/3+prt[0];
              for(l=1;l<(int) ((*initexe)[0])/3+1;l++)
                {                                  
                  initexei.col1[l]=(*initexe)[l];
                  initexei.col2[l]=(*initexe)[l+(int) ((*initexe)[0])/3];
                  initexei.col3[l]=(*initexe)[l+2* (int) ((*initexe)[0])/3];
                }
              for(l=1;l<prt[0]+1;l++)
                {
                  initexei.col1[l+(int) ((*initexe)[0])/3]=j;
                  initexei.col2[l+(int) ((*initexe)[0])/3]=prt[l];
                  initexei.col3[l+(int) ((*initexe)[0])/3]=ll11[prt[l]];
                }
              if (((*initexe)=(double*)realloc((*initexe),(size_t) (sizeof(double)*(3*initexei.col1[0]+1)))) == NULL ) return 0;
	      ((*initexe)[0])=3*((int) initexei.col1[0]);
              for(l=1;l<(int) (*initexe)[0]/3+1;l++)
                {
                  (*initexe)[l]=initexei.col1[l];
                  (*initexe)[l+(int) ((*initexe)[0])/3]=initexei.col2[l];
                  (*initexe)[l+2*(int) ((*initexe)[0])/3]=initexei.col3[l];
                }
	      free(initexei.col1);
	      free(initexei.col2);
	      free(initexei.col3);
	      free(prt);prt=NULL;
	      free(ll11); ll11=NULL;
            }
	  free(fff);
        }
    } /* end for j */
  if (clkconnect[0] != 0)
    { 
  clkconnecttmp=VecEg1(clkconnect);
  ppget=GetPartVect(clkconnecttmp,1,clkconnecttmp[0]/4);
  idl=FindDif(ppget,0);
  free(ppget);
  if ((clkconnect=(int*)malloc(sizeof(int)*(4*idl[0]+1))) == NULL ) return 0;
  clkconnect[0]=4*idl[0];
  for(l=1;l<idl[0]+1;l++)
    {
      clkconnect[l]=clkconnecttmp[idl[l]];
      clkconnect[l+idl[0]]=clkconnecttmp[clkconnecttmp[0]/4+idl[l]];
      clkconnect[l+2*idl[0]]=clkconnecttmp[idl[l]+clkconnecttmp[0]/2];
      clkconnect[l+3*idl[0]]=clkconnecttmp[idl[l]+3*clkconnecttmp[0]/4];
    }
  free(clkconnecttmp);clkconnecttmp=NULL;
  free(idl);idl=NULL;
  clkconnecttmp=GetPartVect(clkconnect,1,clkconnect[0]/4);
  idl=GetPartVect(clkconnect,clkconnect[0]/4+1,clkconnect[0]/4);
  if (!idl)
    {
      if ((idl=(int*)malloc(sizeof(int))) == NULL ) return 0;
      idl[0]=0;
    }
  if ((con=(int*)malloc(sizeof(int)*(idl[0]+1))) == NULL ) return 0;
  con[0]=idl[0];
  for(l=1;l<clkconnect[0]/4+1;l++)
    {
      con[l]=-bllst5ptr[clkconnecttmp[l]]-idl[l]+1;
    }
  free(clkconnecttmp);clkconnecttmp=NULL;
  free(idl);idl=NULL;
  if ((ind=(int*)malloc(sizeof(int)*(con[0]+1))) == NULL ) return 0;
  ind[0]=con[0];
  C2F(isort)(&con[1],&con[0],&ind[1]);
  free(con);con=NULL;
  clkconnectind=VecEg1(clkconnect);
  for(l=1;l<ind[0]+1;l++)
    {
      clkconnect[l]=clkconnectind[ind[l]];
      clkconnect[l+ind[0]]=clkconnectind[ind[l]+ind[0]];
      clkconnect[l+2*ind[0]]=clkconnectind[ind[l]+2*ind[0]];
      clkconnect[l+3*ind[0]]=clkconnectind[ind[l]+3*ind[0]];      
    } 
  free(clkconnectind);
  clkconnectind=NULL;
  free(ind);ind=NULL;
    }
  if (((*boptr)=(int*)malloc(sizeof(int)*(nbl+2))) == NULL ) return 0;
  (*boptr)[0]=nbl+1;
  (*boptr)[1]=1;
  if (((*bexe)=(int*)malloc(sizeof(int))) == NULL ) return 0;
  (*bexe)[0]=0;
  ppget=GetPartVect(clkconnect,1,clkconnect[0]/4);
  if (!ppget)
    {
      if ((ppget=(int*)malloc(sizeof(int))) == NULL ) return 0;
      ppget[0]=0;
    }
  for (j=1;j<nbl+1;j++)
    {
      idl=FindEg(ppget,j);
      if(idl)
        {
          if ((ind=(int*)malloc(sizeof(int)*(idl[0]+1))) == NULL ) return 0;
          ind[0]=idl[0];
          for (l=1;l<ind[0]+1;l++)
            {
              ind[l]=clkconnect[idl[l]+clkconnect[0]/2];
            }      
          if (((*bexe)=(int*)realloc((*bexe),sizeof(int)*((*bexe)[0]+ind[0]+1))) == NULL ) return 0;
          for (l=1;l<ind[0]+1;l++)
            {
              (*bexe)[l+(*bexe)[0]]=ind[l];
            }
          (*bexe)[0]=(*bexe)[0]+ind[0];
          (*boptr)[j+1]=(*boptr)[j]+ind[0];
	  free(idl);idl=NULL;
	  free(ind);ind=NULL;
        }
      else (*boptr)[j+1]=(*boptr)[j];
    }
  free(ppget); ppget=NULL;
  if (clkconnect[0] != 0) {free(clkconnect);clkconnect=NULL;}
  if (((*blptr)=(int*)malloc(sizeof(int)*(nbl+2))) == NULL ) return 0;
  (*blptr)[0]=nbl+1;
  (*blptr)[1]=1;
  if (((*blnk)=(int*)malloc(sizeof(int))) == NULL ) return 0;
  (*blnk)[0]=0;
  ppget=GetPartVect(connectmat,1,connectmat[0]/4);
  if(!ppget)
    {
      if ((ppget=(int*)malloc(sizeof(int))) == NULL ) return 0;
      ppget[0]=0;
    }
  for (j=1;j<nbl+1;j++)
    {      
      idl=FindEg(ppget,j);
      if(idl && idl[0] != 0)
        {
          if ((ind=(int*)malloc(sizeof(int)*(idl[0]+1))) == NULL ) return 0;
          ind[0]=idl[0];
          for (l=1;l<ind[0]+1;l++)
            {
              ind[l]=connectmat[idl[l]+connectmat[0]/2];
            }      
          if (((*blnk)=(int*)realloc((*blnk),sizeof(int)*((*blnk)[0]+ind[0]+1))) == NULL ) return 0;
          for (l=1;l<ind[0]+1;l++)
            {
              (*blnk)[l+(*blnk)[0]]=ind[l];
            }
          (*blnk)[0]=(*blnk)[0]+ind[0];
          (*blptr)[j+1]=(*blptr)[j]+ind[0];
	  free(ind);ind=NULL;
        }
      else (*blptr)[j+1]=(*blptr)[j];
      free(idl);idl=NULL;
    }
  free(ppget);ppget=NULL;  
  
  nlnk=connectmat[0]/4;
  if (((*inplnk)=(int*)calloc(bllst2ptr[bllst2ptr[0]],sizeof(int))) == NULL ) return 0;
  (*inplnk)[0]=bllst2ptr[bllst2ptr[0]]-1;
  if (((*outlnk)=(int*)calloc(bllst3ptr[bllst3ptr[0]],sizeof(int))) == NULL ) return 0;
  (*outlnk)[0]=bllst3ptr[bllst3ptr[0]]-1;
  ptlnk=1; 
  if ((lnkbsz=(int*)malloc(sizeof(int))) == NULL ) return 0;
  lnkbsz[0]=0;

  for (jj=1;jj<nlnk+1;jj++)
    {
      ko=(*outlnk)[bllst3ptr[connectmat[jj]]+connectmat[jj+nlnk]-1];
      ki=(*inplnk)[bllst2ptr[connectmat[jj+2*nlnk]]+connectmat[jj+3*nlnk]-1];
      if (ko != 0 && ki !=0)
        {
          if (ko > ki )
            {
	      ind=FindSup(*outlnk,ko);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=(*outlnk)[ind[l]]-1;
                }
              free(ind);ind=NULL;
              ind=FindEg(*outlnk,ko);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=ki;
                }
              free(ind); ind=NULL;
              ind=FindSup(*inplnk,ko);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=(*inplnk)[ind[l]]-1;
                }
              free(ind);ind=NULL;
              ind=FindEg(*inplnk,ko);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=ki;
                }
              ptlnk--;
	    }
          else if (ki > ko)
            {
	      ind=FindSup(*outlnk,ki);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=(*outlnk)[ind[l]]-1;
                }
              free(ind); ind=NULL;
              ind=FindEg(*outlnk,ki);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=ko;
                }
              free(ind); ind=NULL;
              ind=FindSup(*inplnk,ki);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=(*inplnk)[ind[l]]-1;
                }
              free(ind); ind=NULL;
              ind=FindEg(*inplnk,ki);
              for(l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=ko;
                }
              ptlnk--;
	    }
        }
      else if (ko != 0)
        {
          (*inplnk)[bllst2ptr[connectmat[jj+2*nlnk]]+connectmat[jj+3*nlnk]-1]=ko;
        }
      else if (ki != 0)
        {
          (*outlnk)[bllst3ptr[connectmat[jj]]+connectmat[jj+nlnk]-1]=ki;
        }
      else
        {
          (*outlnk)[bllst3ptr[connectmat[jj]]+connectmat[jj+nlnk]-1]=ptlnk;
          (*inplnk)[bllst2ptr[connectmat[jj+2*nlnk]]+connectmat[jj+3*nlnk]-1]=ptlnk;
	  ind=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
          if ((lnkbsz=(int*)realloc(lnkbsz,sizeof(int)*(lnkbsz[0]+2))) == NULL ) return 0;
          lnkbsz[lnkbsz[0]+1]=ind[connectmat[jj+nlnk]];
          lnkbsz[0]++;                      
          ptlnk++;
	  free(ind);ind=NULL;
        }
    } /* fin de for jj */

  if ((ind=(int*)malloc(sizeof(int)*(lnkbsz[0]+2))) == NULL ) return 0;
  ind[0]=lnkbsz[0]+1;
  ind[1]=1;
  for (l=2;l<ind[0]+1;l++)
    {
      ind[l]=lnkbsz[l-1];
    }
  free(lnkbsz);lnkbsz=NULL;
  CumSum(ind);
  if (((*lnkptr)=(int*)malloc(sizeof(int)*(ind[0]+1))) == NULL ) return 0;
  (*lnkptr)[0]=ind[0];
  for (l = 1; l < ind[0]+1; l++)
    {
      (*lnkptr)[l]=ind[l];
    } 
  free(ind);ind=NULL;
  unco=FindEg(*outlnk,0);
  if (unco && unco[0] !=0)
    {
      siz_unco=0;
      for (l=1;l<unco[0]+1;l++)
        {
          j=unco[l]; 
	  ppget=FindInfEg(bllst3ptr,j);
	  m1=Max1(ppget);
          n=j-bllst3ptr[m1]+1;
	  free(ppget);ppget=NULL;
          ind=GetPartVect(bllst3,bllst3ptr[m1],bllst3ptr[m1+1]-bllst3ptr[m1]);
          if (ind)
            {
              if ((idl=(int*)malloc(sizeof(int)*(3))) == NULL ) return 0;
              idl[0]=2;
              idl[1]=siz_unco;
              idl[2]=ind[n];
	      if (ind[n] < 1)
		{
		  ppget=GetPartVect(corinvec,corinvptr[m1],corinvptr[m1+1]-corinvptr[m1]);
		  if ((mm=(int*)malloc(sizeof(int)*(2))) == NULL ) return 0;
		  mm[0]=1;
		  mm[1]=-1;
		  *ok=connection(ppget,mm);
		  free(ppget);
		  free(mm);
		  return 0;
		}
              siz_unco=Max1(idl);
	      free(idl);idl=NULL;
	      free(ind);ind=NULL;
            }
        }
      if (((*lnkptr)=(int*)realloc((*lnkptr),sizeof(int)*((*lnkptr)[0]+2))) == NULL ) return 0; 
      (*lnkptr)[(*lnkptr)[0]+1]=(*lnkptr)[(*lnkptr)[0]]+siz_unco;
      (*lnkptr)[0]=(*lnkptr)[0]+1;
      a=Max1(*outlnk)+1;
      for (l=1;l<unco[0]+1;l++)
        {
          (*outlnk)[unco[l]]=a;                           
        }
    }
  free(unco);unco=NULL; 
  return 0;
} /* end funct*/
     
/* ======================================= endfunction extract_info ========================================== */

/* ************************************* function conn_mat ************************************************** */
int conn_mat(int* inplnk,int* outlnk,int* bllst2ptr,int* bllst3ptr,int** outoin,int** outoinptr,int* nblk)
{
  int l,ll,j,jj,jjj,m1,n;
  int *outoini,*ind,*k,*ppget,a;
  int *ii;
  Mat2C outoinii;
  ii=NULL;  
  outoini=NULL;
  *nblk=bllst2ptr[0]-1;
  if (((*outoinptr)=(int*)malloc(sizeof(int)*(*nblk+2))) == NULL ) return 0;
  (*outoinptr)[0]=*nblk+1;
  if (((*outoin)=(int*)malloc(sizeof(int))) == NULL ) return 0;
  (*outoin)[0]=0;
  (*outoinptr)[1]=1;
  for (j=1;j<*nblk+1;j++)
    {
      if ((k=(int*)malloc(sizeof(int)*(bllst3ptr[j+1]-bllst3ptr[j]+1))) == NULL ) return 0;
       k[0]=bllst3ptr[j+1]-bllst3ptr[j];
       a=-bllst3ptr[j]+1;
      for (l=bllst3ptr[j];l<bllst3ptr[j+1];l++)
        {
          k[l+a]=l;
        }
      if ((ii=(int*)malloc(sizeof(int))) == NULL ) return 0;
      ii[0]=0;
      for (l=1;l<k[0]+1;l++)
        {
          jjj=outlnk[k[l]];
          ind=FindEg(inplnk,jjj);
          if (ind)
            {
              if ((ii=(int*)realloc(ii,sizeof(int)*(ii[0]+ind[0]+1))) == NULL ) return 0;
	      for (ll=1;ll<ind[0]+1;ll++)
                {
                  ii[ii[0]+ll]=ind[ll];
                }
	      
              ii[0]=ii[0]+ind[0];
	      free(ind); ind=NULL;
            }
	}
      jj=0;
      if (ii && ii[0] != 0)
	{
	  if ((outoini=(int*)malloc(sizeof(int)*(2*ii[0]+1))) == NULL ) return 0;
	  outoini[0]=2*ii[0];
	  for (ll=1;ll<ii[0]+1;ll++)
	    {
	      l=ii[ll];
	      ppget=FindInfEg(bllst2ptr,l);
	      m1=Max1(ppget);
	      free(ppget);ppget=NULL;
	      n=l-bllst2ptr[m1]+1;
	      outoini[ll]=m1;
	      outoini[ll+ii[0]]=n;
	      jj++;
	    }
	  if ((outoinii.col1=(int*)malloc(sizeof(int)*(outoini[0]/2+(*outoin)[0]/2+1))) == NULL) return 0;
	  outoinii.col1[0]=outoini[0]/2+(*outoin)[0]/2;
	  if ((outoinii.col2=(int*)malloc(sizeof(int)*(outoini[0]/2+(*outoin)[0]/2+1))) == NULL) return 0;
	  outoinii.col2[0]=outoini[0]/2+(*outoin)[0]/2;
	  for(l=1;l<(*outoin)[0]/2+1;l++)
	    {
	      outoinii.col1[l]=(*outoin)[l];
	      outoinii.col2[l]=(*outoin)[l+(*outoin)[0]/2];
	    }
	  for(l=1;l<outoini[0]/2+1;l++)
	    {
	      outoinii.col1[l+(*outoin)[0]/2]=outoini[l];
	      outoinii.col2[l+(*outoin)[0]/2]=outoini[l+outoini[0]/2];
	    }
	  
	  if (((*outoin)=(int*)realloc((*outoin),sizeof(int)*((*outoin)[0]+outoini[0]+1))) == NULL ) return 0;
	  (*outoin)[0]=(*outoin)[0]+outoini[0];
	  
	  for(l=1;l<(*outoin)[0]/2+1;l++)
	    {
	      (*outoin)[l]=outoinii.col1[l];
	      (*outoin)[l+(*outoin)[0]/2]=outoinii.col2[l];
	    }
	  free(outoinii.col1); outoinii.col1=NULL;
	  free(outoinii.col2); outoinii.col2=NULL;
	  free(outoini);outoini=NULL;
	}
      (*outoinptr)[j+1]=(*outoinptr)[j]+jj;
      free(k);k=NULL;
      free(ii);ii=NULL;
    } /* end for */
  return 0;
} /* end function */
/* ======================================= endfunction conn_mat ========================================== */

/* ************************************* function  critical_events ************************************************** */
int critical_events(int* connectmat,int* clkconnect,int *bllst12,int *typ_r,int *typ_l,int *typ_zx,int *outoin,
		    int *outoinptr,int *bllst5ptr,int **critev)
{
  int i,j,k,*typ_c,done1,mm,a,*cll,val=0,*cllind,*typr;
  int *clkconnecttmp,*clkconnectind,*ind,*ii,*oo,*vec,nblk,*r,*nd,max1,nnd,done,*jj;
  Mat4C clkconnecti;
  
  typr=VecEg1(typ_r);
  nblk=bllst12[0]/2;
  if ((typ_c=(int*)malloc(sizeof(int)*(typ_l[0]+1))) == NULL ) return 0;
  typ_c[0]=typ_l[0];
  Setmem(typ_c,0);
  for (i=1; i<=typr[0]; i++){
    if (bllst12[i+bllst12[0]/2]){
      typr[i]=1;
    }
  }
  clkconnecttmp=VecEg1(clkconnect);
  done1=0;
  while (!done1){
    done1=1;
    mm=Max1(clkconnecttmp)+1;
    a=clkconnecttmp[0]/4;
    if ((cll=(int*)malloc(sizeof(int)*(a+1))) == NULL ) return 0;
    cll[0]=a;
    for (i=1; i<=a; i++)
      {
	cll[i]=-clkconnecttmp[i]*mm-clkconnecttmp[i+a];
      }
    if ((ind=(int*)malloc(sizeof(int)*(a+2))) == NULL ) return 0;
    C2F(isort)(&cll[1],&a,&ind[0]);
    clkconnectind=VecEg1(clkconnecttmp);
    for(i=1; i<=a; i++)
      {
	clkconnecttmp[i]=clkconnectind[ind[i-1]];
	clkconnecttmp[i+a]=clkconnectind[ind[i-1]+a];
	clkconnecttmp[i+2*a]=clkconnectind[ind[i-1]+2*a];
	clkconnecttmp[i+3*a]=clkconnectind[ind[i-1]+3*a];      
      }
    if (ind) free(ind);
    free(clkconnectind); clkconnectind=NULL;
    if ((cll=(int*)realloc(cll,sizeof(int)*(a+3))) == NULL ) return 0;
    cll[0]=a+2;
    cllind=VecEg1(cll);
    for (j=2;j<cll[0]+1;j++)
      {
	cll[j]=-cllind[j-1];
      }   
    if(cllind) free(cllind);
    cllind=NULL;
    cll[1]=-1;
    cll[cll[0]]=mm;
    
    if ((ii=(int*)malloc(sizeof(int)*(a+2))) == NULL ) return 0;
    ii[0]=a+1;
    for (i=1; i<=ii[0]; i++)
      ii[i]=cll[i+1]-cll[i];
    if (cll) free(cll);
    
    ind=FindDif(ii,0);
    if (ii) free(ii);
    if (ind){
      for (i=1; i<ind[0]; i++){
	if ((vec=(int*)malloc(sizeof(int)*(nblk+1))) == NULL ) return 0;
	vec[0]=nblk;
	Setmem(vec,-1);
	a=clkconnecttmp[0]/4;
	for (j=ind[i]; j<ind[i+1]; j++)
	  vec[clkconnecttmp[j+2*a]]=0;
	 	
	nd=GetPartVect(outoin,outoin[0]/2+1,outoin[0]/2);
	if (!nd)
	  {
	    if ((nd=(int*)malloc(sizeof(int))) == NULL ) return 0;
	    nd[0]=0;
	  }
	max1=Max1(nd);
	free(nd);
	nnd=max1+1;
	if ((nd=(int*)calloc((nblk*nnd+1),sizeof(int))) == NULL ) return 0;
	nd[0]=nblk*nnd;
	
	tree4(vec,nd,nnd,outoin,outoinptr,typr,&r);
	if(nd) free(nd);
	if(vec) free(vec); vec=NULL;
	
	if (r) {
	  if ((clkconnecti.col1=(int*)malloc(sizeof(int)*(a+r[0]/2+1))) == NULL ) return 0;
	  clkconnecti.col1[0]=a+r[0]/2;
	  if ((clkconnecti.col2=(int*)malloc(sizeof(int)*(a+r[0]/2+1))) == NULL ) return 0;
	  clkconnecti.col2[0]=a+r[0]/2;
	  if ((clkconnecti.col3=(int*)malloc(sizeof(int)*(a+r[0]/2+1))) == NULL ) return 0;
	  clkconnecti.col3[0]=a+r[0]/2;
	  if ((clkconnecti.col4=(int*)malloc(sizeof(int)*(a+r[0]/2+1))) == NULL ) return 0;
	  clkconnecti.col4[0]=a+r[0]/2;
	  for(j=1; j<a+1; j++)
	    {
	      clkconnecti.col1[j]=clkconnecttmp[j];
	      clkconnecti.col2[j]=clkconnecttmp[j+a];
	      clkconnecti.col3[j]=clkconnecttmp[j+2*a];
	      clkconnecti.col4[j]=clkconnecttmp[j+3*a];
	    }
	  for (j=1; j<r[0]/2+1; j++)
	    {     
	      clkconnecti.col1[j+a]=clkconnecttmp[ind[i]];
	      clkconnecti.col2[j+a]=clkconnecttmp[ind[i]+a];
	      clkconnecti.col3[j+a]=r[j];
	      clkconnecti.col4[j+a]=r[j+r[0]/2];
	    }
	  free(clkconnecttmp);
	  if ((clkconnecttmp=(int*)malloc(sizeof(int)*(4*clkconnecti.col1[0]+1))) == NULL ) return 0;
	  clkconnecttmp[0]=4*clkconnecti.col1[0];
	  for (j=1; j<clkconnecttmp[0]/4+1; j++)
	    {
	      clkconnecttmp[j]=clkconnecti.col1[j];
	      clkconnecttmp[j+clkconnecttmp[0]/4]=clkconnecti.col2[j];
	      clkconnecttmp[j+clkconnecttmp[0]/2]=clkconnecti.col3[j];
	      clkconnecttmp[j+3*clkconnecttmp[0]/4]=clkconnecti.col4[j];
	    }
	  free(clkconnecti.col1);
	  clkconnecti.col1=NULL;
	  free(clkconnecti.col2);
	  clkconnecti.col2=NULL;
	  free(clkconnecti.col3);
	  clkconnecti.col3=NULL;
	  free(clkconnecti.col4);
	  clkconnecti.col4=NULL;
	  free(r); r=NULL;
	}
      }
      free(ind);
    }
    done=0;
    while (!done){
      done=1;
      if ((vec=(int*)calloc(typ_c[0]+1,sizeof(int))) == NULL ) return 0;
      vec[0]=typ_c[0];
      for (i=1; i<=typ_c[0]; i++){
	if ( typ_l[i] && !(typ_c[i]) )
	  vec[i]=1;	
      }
      jj=FindEg(vec,1);
      if (vec) free(vec);
      if (jj[0] != 0){
	if ((vec=(int*)calloc(clkconnecttmp[0]/4+1,sizeof(int))) == NULL ) return 0;
	vec[0]=clkconnecttmp[0]/4;
	for (i=1; i<=jj[0]; i++){
	  for (j=1; j<=clkconnecttmp[0]/4; j++){
	    if (clkconnecttmp[j+clkconnecttmp[0]/2] == jj[i]) vec[j]=1;	      
	  }
	  if (!OR(vec)){
	    oo=GetPartVect(clkconnecttmp,1,clkconnecttmp[0]/4);
	    ind=FindEg(oo,jj[i]);
	    if (oo) free(oo);
	    if (ind){
	      for (j=1; j<=ind[0]; j++){
		typr[clkconnecttmp[ind[j]+clkconnecttmp[0]/2]]=1;
		clkconnecttmp[ind[j]]=-4321;
		clkconnecttmp[ind[j]+clkconnecttmp[0]/4]=-4321;
		clkconnecttmp[ind[j]+clkconnecttmp[0]/2]=-4321;
		clkconnecttmp[ind[j]+3*clkconnecttmp[0]/4]=-4321;
	      }
	      for (j=1; j<clkconnecttmp[0]+1; j++)
		{
		  if ( clkconnecttmp[j] != -4321 )  clkconnecttmp[j-val]=clkconnecttmp[j];
		  else
		    val++;
		}
	      clkconnecttmp[0]=clkconnecttmp[0]-4*ind[0];
	      free(ind); ind=NULL;
	    }
	    typ_c[jj[i]]=1;
	    done1=0;
	    done=0;
	  }
	}	
	if (vec) free(vec);
      }
      free(jj);
    }
  }
  free(typ_c);
  free(typr);
  if (((*critev)=(int*)calloc(bllst5ptr[bllst5ptr[0]],sizeof(int))) == NULL ) return 0;
  (*critev)[0]=bllst5ptr[bllst5ptr[0]]-1;
  for (i=1; i<bllst5ptr[0]; i++){
    for (j=bllst5ptr[i]; j<bllst5ptr[i+1]; j++){
      if ((vec=(int*)calloc(clkconnecttmp[0]/4+1,sizeof(int))) == NULL ) return 0;
      vec[0]=clkconnecttmp[0]/4;
      for (k=1; k<=clkconnecttmp[0]/4; k++){
	if ((clkconnecttmp[k] == i) && (clkconnecttmp[k+clkconnecttmp[0]/4] == j-bllst5ptr[i]+1)) vec[k]=1;
      }
      ind=FindEg(vec,1);
      if (ind){
	if (vec) free(vec);
	if ((vec=(int*)malloc(sizeof(int)*(ind[0]+1))) == NULL ) return 0;
	vec[0]=ind[0];
	for (k=1; k<=ind[0]; k++)
	  vec[k]=typ_zx[clkconnecttmp[ind[k]+clkconnecttmp[0]/2]];
	if (OR(vec)) (*critev)[j]=1;
	if (vec) free(vec);
	free(ind);
      }
    }
  }
  free(clkconnecttmp);
  return 0;
} /* end function */
/* ======================================= endfunction critical_events ========================================== */

/* *************************************** function make_ptr ******************************************* */
int make_ptr(char** bllst10,int** typ_l,int** typ_m)
{
  int ll,j=1;
  int *typ_l1,*pp,*typ_m1;
  if (((int *)bllst10)[0]<(*typ_l)[0]+1) return 0;
  ll=((int*)bllst10)[0]-(*typ_l)[0];
  if ((typ_l1=(int*)calloc((ll+1),sizeof(int))) == NULL ) return 0;
  typ_l1[0]=ll;
  if ((typ_m1=(int*)calloc((ll+1),sizeof(int))) == NULL ) return 0;
  typ_m1[0]=ll;
  if (*(bllst10[(*typ_l)[0]+1])=='l') typ_l1[1]=1;
  j=1;
  for (ll=(*typ_l)[0]+2;ll<((int*)bllst10)[0]+1;ll++)
    {
      j++;      
      if (*(bllst10[ll]) == 'l') typ_l1[j]=1;
      else typ_l1[j]=0;
    }
  if (((*typ_l)=(int*)realloc((*typ_l),sizeof(int)*((*typ_l)[0]+typ_l1[0]+1))) == NULL ) return 0;
  pp=&(*typ_l)[1+(*typ_l)[0]];
  pp=memcpy(pp,&typ_l1[1],sizeof(int)*(typ_l1[0]));
  (*typ_l)[0]+=typ_l1[0];
  
  if (((*typ_m)=(int*)realloc((*typ_m),sizeof(int)*((*typ_m)[0]+typ_m1[0]+1))) == NULL ) return 0;
  pp=&(*typ_m)[1+(*typ_m)[0]];
  pp=memcpy(pp,&typ_m1[1],sizeof(int)*(typ_m1[0]));
  (*typ_m)[0]+=typ_m1[0];
  free(typ_l1);typ_l1=NULL;
  free(typ_m1);typ_m1=NULL;
  return 0;
} /* end function */
/* ======================================= endfunction make_ptr ========================================== */
/* *************************************** function adjust_inout ****************************************** */
int adjust_inout(int* bllst2,int* bllst3,int* bllst2ptr,int* bllst3ptr,int* nzcross,int* nmode,int* connectmat,int* ok,int* corinvec,int* corinvptr,int nblk1)
{
  int hhjj,j,hh,jj,nout,nin,findflag,mini1,mini2;
  int *wwi,*ww,*nww,*ind,*ind1,*wwi1,a,ninnout,*nww1;
  double k,k1;
  int kint,prt_out,prt_in;
  wwi=ww=nww=ind=ind1=wwi1=nww1=NULL;
  
  for (hhjj=1;hhjj<nblk1+2;hhjj++)
    {
      for (hh=1;hh<nblk1+2;hh++)
        {
          *ok=true;
          for (jj=1;jj<connectmat[0]/4+1;jj++)
            {
              ww=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
	      nout=ww[connectmat[jj+connectmat[0]/4]];
	      free(ww);
              ww=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
	      nin=ww[connectmat[jj+3*connectmat[0]/4]];
	      free(ww);
              if (nout>0 && nin>0)
		{
                  if (nin != nout)
                    {
		      ind1=GetPartVect(corinvec,corinvptr[connectmat[jj]],corinvptr[connectmat[jj]+1]-corinvptr[connectmat[jj]]);
		      ind=GetPartVect(corinvec,corinvptr[connectmat[jj+connectmat[0]/2]],corinvptr[connectmat[jj+connectmat[0]/2]+1]-corinvptr[connectmat[jj+connectmat[0]/2]]);
		      prt_out=connectmat[jj+connectmat[0]/4];
		      prt_in=connectmat[jj+3*connectmat[0]/4];
		      badconnection(ind1,prt_out,nout,ind,prt_in,nin);
		      *ok=false;
		      if(ind1) free (ind1); 
		      if(ind) free (ind); 
		      return 0;
                    }
                } /* fin de if 1 */
              else if (nout>0 && nin<0)
		{
                  wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
                  ww=FindEg(wwi,nin);
                  for (j=1;j<ww[0]+1;j++)
                    {
                      wwi[ww[j]]=nout;
                    }
                  for (j=bllst2ptr[connectmat[jj+connectmat[0]/2]];j<bllst2ptr[connectmat[jj+connectmat[0]/2]+1];j++)
                    {
                      bllst2[j]=wwi[j-bllst2ptr[connectmat[jj+connectmat[0]/2]]+1];
                    }
                  if(wwi) free(wwi);
                  if(ww) free(ww);
                  wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj+connectmat[0]/2]],bllst3ptr[connectmat[jj+connectmat[0]/2]+1]-bllst3ptr[connectmat[jj+connectmat[0]/2]]);
                  if (wwi) 
                    {
		      ww=FindEg(wwi,nin);
                      if (ww)
                        {
                          for (j=1;j<ww[0]+1;j++)
                            {
                              wwi[ww[j]]=nout;
                            }
			  free(ww);
                          for (j=bllst3ptr[connectmat[jj+connectmat[0]/2]];j<bllst3ptr[connectmat[jj+connectmat[0]/2]+1];j++)
                            {
                              bllst3[j]=wwi[j-bllst3ptr[connectmat[jj+connectmat[0]/2]]+1];
                            }
			}
                      
                      free(wwi);
                    }
		  wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj+connectmat[0]/2]],bllst3ptr[connectmat[jj+connectmat[0]/2]+1]-bllst3ptr[connectmat[jj+connectmat[0]/2]]);
                  if (wwi)
                    {
                      ww=FindEg(wwi,0);
		      wwi1=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
                      mini1=Min1(wwi1);
                      if (ww && (mini1 > 0))
                        {
			  a=Sum(wwi1);
			  for (j=1;j<ww[0]+1;j++)
                            {
                              wwi[ww[j]]=a;
                            }
                          for (j=bllst3ptr[connectmat[jj+connectmat[0]/2]];j<bllst3ptr[connectmat[jj+connectmat[0]/2]+1];j++)
                            {
                              bllst3[j]=wwi[j-bllst3ptr[connectmat[jj+connectmat[0]/2]]+1];
                            }
			}
		      free(ww);ww=NULL;
		      free(wwi);wwi=NULL;
		      if (wwi1) free(wwi1);
		      wwi1=NULL;
                    }
		  if (nzcross[connectmat[jj+connectmat[0]/2]] == nin){
		    nzcross[connectmat[jj+connectmat[0]/2]] = nout;
		  }
		  if (nmode[connectmat[jj+connectmat[0]/2]] == nin){
		    nmode[connectmat[jj+connectmat[0]/2]] = nout;
		  }
		}
	      else if (nin>0 && nout<0)
		{
		  wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
		  ww=FindEg(wwi,nout);
		  for (j=1;j<ww[0]+1;j++)
		    {
		      wwi[ww[j]]=nin;
		    }
		  for (j=bllst3ptr[connectmat[jj]];j<bllst3ptr[connectmat[jj]+1];j++)
		    {
		      bllst3[j]=wwi[j-bllst3ptr[connectmat[jj]]+1];
		    }
		  if(wwi) free(wwi);
		  if(ww) free(ww);
		  
		  wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj]],bllst2ptr[connectmat[jj]+1]-bllst2ptr[connectmat[jj]]);
		  if (wwi)
		    {
		      ww=FindEg(wwi,nout);
		      if (ww)
			{
			  for (j=1;j<ww[0]+1;j++)
			    {
			      wwi[ww[j]]=nin;
			    }
			  if(ww) free(ww);
			}
		      for (j=bllst2ptr[connectmat[jj]];j<bllst2ptr[connectmat[jj]+1];j++)
			{
			  bllst2[j]=wwi[j-bllst2ptr[connectmat[jj]]+1];
			}                 
		      if(wwi) free(wwi);
		    }
                  wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj]],bllst2ptr[connectmat[jj]+1]-bllst2ptr[connectmat[jj]]);
		  if (!wwi)
		    {
		      if ((wwi=(int*)malloc(sizeof(int))) == NULL ) return 0;
		      wwi[0]=0;
		    }
                  ww=FindEg(wwi,0);
		  wwi1=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
                  mini2=Min1(wwi1);
		  
                  if ((ww != NULL) && (mini2 > 0))
                    {
		      a=Sum(wwi1);
		      for (j=1;j<ww[0]+1;j++)
                        {
                          wwi[ww[j]]=a;
                        }
                      for (j=bllst2ptr[connectmat[jj]];j<bllst2ptr[connectmat[jj]+1];j++)
                        {
                          bllst2[j]=wwi[j-bllst2ptr[connectmat[jj]]+1];
                        }
		    }
		  free(ww);ww=NULL;
		  if(wwi1) free(wwi1);
		  wwi1=NULL;
		  if(wwi) free(wwi);
		  wwi=NULL;
		  if (nzcross[connectmat[jj]] == nout){
		    nzcross[connectmat[jj]] = nin;
		  }
		  if (nmode[connectmat[jj]] == nout){
		    nmode[connectmat[jj]] = nin;
		  }
		}
	      else if (nin==0)
		{
		  ww=GetPartVect(bllst3,bllst3ptr[connectmat[jj+connectmat[0]/2]],bllst3ptr[connectmat[jj+connectmat[0]/2]+1]-bllst3ptr[connectmat[jj+connectmat[0]/2]]);
		  wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
		  if (Min1(ww)>0)
		    {                                       
		      if (nout>0)
			{
			  if (Sum(ww)==nout)
			    {                                               
			      wwi[connectmat[jj+3*connectmat[0]/4]]=nout;
			      for (j=bllst2ptr[connectmat[jj+connectmat[0]/2]];j<bllst2ptr[connectmat[jj+connectmat[0]/2]+1];j++)
				{
				  bllst2[j]=wwi[j-bllst2ptr[connectmat[jj+connectmat[0]/2]]+1];
				}
			    }
			  else
			    {
			      ind=GetPartVect(corinvec,corinvptr[connectmat[jj+connectmat[0]/2]],corinvptr[connectmat[jj+connectmat[0]/2]+1]-corinvptr[connectmat[jj+connectmat[0]/2]]);
			      if ((ind1=(int*)malloc(sizeof(int)*2)) == NULL ) return 0;
			      ind1[0]=1;
			      ind1[1]=-1;
			      badconnection(ind,0,0,ind1,0,0);
			      *ok=false;
			      if(ind) free (ind); 
			      if(ind1) free (ind1);
			      return 0;
			      }
			}
			else
			  {
			    wwi[connectmat[jj+3*connectmat[0]/4]]=Sum(ww);
			    for (j=bllst2ptr[connectmat[jj+connectmat[0]/2]];j<bllst2ptr[connectmat[jj+connectmat[0]/2]+1];j++)
			      {
				bllst2[j]=wwi[j-bllst2ptr[connectmat[jj+connectmat[0]/2]]+1];
			      }
			    *ok=false;
			  }
		    }
		  else
		    {
		      ind=FindInf(ww,0);
		      if ((nww=(int*)malloc(sizeof(int)*(ind[0]+1))) == NULL ) return 0;
		      nww[0]=ind[0];
		      for (j=1;j<ind[0]+1;j++)
			{
			  nww[j]=ww[ind[j]]-ww[ind[1]];
			  }
		      if (Norm(nww)==0 && nout>0)
			{
			  wwi[connectmat[jj+3*connectmat[0]/4]]=nout;
			  for (j=bllst2ptr[connectmat[jj+connectmat[0]/2]];j<bllst2ptr[connectmat[jj+connectmat[0]/2]+1];j++)
			      {
				bllst2[j]=wwi[j-bllst2ptr[connectmat[jj+connectmat[0]/2]]+1];
			      }                                           
			  ind1=FindSup(ww,0);
			  if ((nww1=(int*)malloc(sizeof(int)*(ind1[0]+1))) == NULL ) return 0;
			  nww1[0]=ind1[0];
			  for (j=1;j<ind1[0]+1;j++)
			    {
			      nww1[j]=ww[ind1[j]];
			    }
			  k1=nout-Sum(nww1);
			  k=k1/nww[0];
			  kint=(int) k;
			  free(nww1);
			  free(ind1);
			  if (k == (double) kint && k>0)
			    {
			      for (j=1;j<ind[0]+1;j++)
				{
				  ww[ind[j]]=(int) k;
				}
			      for (j=bllst3ptr[connectmat[jj+connectmat[0]/2]];j<bllst3ptr[connectmat[jj+connectmat[0]/2]+1];j++)
				{
				  bllst3[j]=ww[j-bllst3ptr[connectmat[jj+connectmat[0]/2]]+1];
				}
			    }
			  else
			    {
			      ind=GetPartVect(corinvec,corinvptr[connectmat[jj+connectmat[0]/2]],corinvptr[connectmat[jj+connectmat[0]/2]+1]-corinvptr[connectmat[jj+connectmat[0]/2]]);
			      if ((ind1=(int*)malloc(sizeof(int)*2)) == NULL ) return 0;
			      ind1[0]=1;
			      ind1[1]=-1;
			      badconnection(ind,0,0,ind1,0,0);
			      *ok=false;
			      if(ind) free (ind); 
			      if(ind1) free (ind1);
			      return 0; 
			    }
			}
		      else
			{
			  *ok=false;
			}
		      free(ind);ind=NULL;
		      free(nww);
		    }
		    free(ww);ww=NULL;
		    free(wwi);wwi=NULL;
		}                               
	      else if (nout==0)
		{
		  ww=GetPartVect(bllst2,bllst2ptr[connectmat[jj]],bllst2ptr[connectmat[jj]+1]-bllst2ptr[connectmat[jj]]);
		  wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
		  if (Min1(ww)>0)
		    {                                       
		      if (nin>0) 
			{
			  if (Sum(ww)==nin)
			    {                                               
			      wwi[connectmat[jj+connectmat[0]/4]]=nin;
			      for (j=bllst3ptr[connectmat[jj]];j<bllst3ptr[connectmat[jj]+1];j++)
				{
				  bllst3[j]=wwi[j-bllst3ptr[connectmat[jj]]+1];
				}
			    }
			  else
			      {
				ind=GetPartVect(corinvec,corinvptr[connectmat[jj]],corinvptr[connectmat[jj]+1]-corinvptr[connectmat[jj]]);
				if ((ind1=(int*)malloc(sizeof(int)*2)) == NULL ) return 0;
				ind1[0]=1;
				ind1[1]=-1;
				badconnection(ind,0,0,ind1,0,0);
				*ok=false;
				if(ind) free (ind); 
				if(ind1) free (ind1);
				return 0;
			      }
			}
		      else
			{
			  wwi[connectmat[jj+connectmat[0]/4]]=Sum(ww);
			  for (j=bllst3ptr[connectmat[jj]];j<bllst3ptr[connectmat[jj]+1];j++)
			    {
			      bllst3[j]=wwi[j-bllst3ptr[connectmat[jj]]+1];
			    }
			  *ok=false;
			}
		    }
		  else
		    {
		      ind=FindInf(ww,0);
		      if ((nww=(int*)malloc(sizeof(int)*(ind[0]+1))) == NULL ) return 0;
		      nww[0]=ind[0];
		      for (j=1;j<ind[0]+1;j++)
			{
			  nww[j]=ww[ind[j]]-ww[ind[1]];
			}
		      if (Norm(nww)==0 && nin>0)
			{
			  wwi[connectmat[jj+connectmat[0]/4]]=nin;
			  for (j=bllst3ptr[connectmat[jj]];j<bllst3ptr[connectmat[jj]+1];j++)
			    {
			      bllst3[j]=wwi[j-bllst3ptr[connectmat[jj]]+1];
			    }
			  ind1=FindSup(ww,0);
			  if ((nww1=(int*)malloc(sizeof(int)*(ind1[0]+1))) == NULL ) return 0;
			  nww1[0]=ind1[0];
			  for (j=1;j<ind1[0]+1;j++)
			    {
			      nww1[j]=ww[ind1[j]];
			    }
			  free(ind1);
			  k1=nin-Sum(nww1);
			  k=k1/nww[0];
			  free(nww1);
			  kint=(int)k;
			  if (k==(double) kint && k>0)
			    {
			      for (j=1;j<ind[0]+1;j++)
				{
				  ww[ind[j]]= (int) k;
				}
			      for (j=bllst2ptr[connectmat[jj]];j<bllst2ptr[connectmat[jj]+1];j++)
				{
				  bllst2[j]=ww[j-bllst2ptr[connectmat[jj]]+1];
				}
			    }
			  else
			    {
			      ind=GetPartVect(corinvec,corinvptr[connectmat[jj]],corinvptr[connectmat[jj]+1]-corinvptr[connectmat[jj]]);
			      if ((ind1=(int*)malloc(sizeof(int)*2)) == NULL ) return 0;
			      ind1[0]=1;
			      ind1[1]=-1;
			      badconnection(ind,0,0,ind1,0,0);
			      *ok=false;
			      if(ind) free (ind); 
			      if(ind1) free (ind1);
			      return 0;
			    }
			}
		      else
			{
			  *ok=false;
			} 
		      if(ind) free(ind); ind=NULL;
		      if(nww) free(nww); nww=NULL;
		    }
		  
		  if(ww) free(ww); ww=NULL;
		  if(wwi) free(wwi); wwi=NULL;
		}
	      else
		{
		  *ok=false;
		}
            } /*fin de for jj */
          if (*ok) return 0;
        } /*  fin de for hh */
      Message("Not enough information to find port sizes, I try to find the problem");
      findflag=false;
      for (jj=1;jj<connectmat[0]/4+1;jj++)
        {
	  ww=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
	  nout=ww[connectmat[jj+connectmat[0]/4]];
	  free(ww);
	  ww=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
	  nin=ww[connectmat[jj+3*connectmat[0]/4]];
	  free(ww);
	  if (nout<=0 && nin<=0)
	    {
	      findflag=true;
	      ind1=GetPartVect(corinvec,corinvptr[connectmat[jj]],corinvptr[connectmat[jj]+1]-corinvptr[connectmat[jj]]);
	      ind=GetPartVect(corinvec,corinvptr[connectmat[jj+connectmat[0]/2]],corinvptr[connectmat[jj+connectmat[0]/2]+1]-corinvptr[connectmat[jj+connectmat[0]/2]]);
	      
	      ninnout=connection(ind1,ind);
	      if(ind1) free (ind1); 
	      if(ind) free (ind); 
	      if (!(ninnout)) 
		{
		  *ok=false;
		  return 0;
		}
	      if (ninnout<=0) 
		{
		  *ok=false;
		  return 0;
		}
	      wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
	      ww=FindEg(wwi,nout);
	      for (j=1;j<ww[0]+1;j++)
		{
		  wwi[ww[j]]=ninnout;
		}
	      for (j=bllst3ptr[connectmat[jj]];j<bllst3ptr[connectmat[jj]+1];j++)
		{
		  bllst3[j]=wwi[j-bllst3ptr[connectmat[jj]]+1];
		}
	      if(wwi) free(wwi);
	      if(ww) free(ww);
	      if (nzcross[connectmat[jj]] == nout){
		nzcross[connectmat[jj]] = ninnout;
	      }
	      if (nmode[connectmat[jj]] == nout){
		nmode[connectmat[jj]] = ninnout;
	      }
	      
	      wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj]],bllst2ptr[connectmat[jj]+1]-bllst2ptr[connectmat[jj]]);
	      if (wwi)
		{
		  ww=FindEg(wwi,nout);
		  if (ww)
		    {
		      for (j=1;j<ww[0]+1;j++)
			{
			  wwi[ww[j]]=ninnout;
			}
		      if(ww) free(ww);
		    }
		  for (j=bllst2ptr[connectmat[jj]];j<bllst2ptr[connectmat[jj]+1];j++)
		    {
		      bllst2[j]=wwi[j-bllst2ptr[connectmat[jj]]+1];
		    }                 
		  if(wwi) free(wwi);
		}
	      wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj]],bllst2ptr[connectmat[jj]+1]-bllst2ptr[connectmat[jj]]);
	      if (!wwi)
		{
		  if ((wwi=(int*)malloc(sizeof(int))) == NULL ) return 0;
		  wwi[0]=0;
		}
	      ww=FindEg(wwi,0);
	      wwi1=GetPartVect(bllst3,bllst3ptr[connectmat[jj]],bllst3ptr[connectmat[jj]+1]-bllst3ptr[connectmat[jj]]);
	      mini2=Min1(wwi1);
	      
	      if ((ww != NULL) && (mini2 > 0))
		{
		  a=Sum(wwi1);
		  for (j=1;j<ww[0]+1;j++)
		    {
		      wwi[ww[j]]=a;
		    }
		  for (j=bllst2ptr[connectmat[jj]];j<bllst2ptr[connectmat[jj]+1];j++)
		    {
		      bllst2[j]=wwi[j-bllst2ptr[connectmat[jj]]+1];
		    }
		}
	      free(ww);ww=NULL;
	      if(wwi1) free(wwi1);
	      wwi1=NULL;
	      if(wwi) free(wwi);
	      wwi=NULL;
	      
	      wwi=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
	      ww=FindEg(wwi,nin);
	      for (j=1;j<ww[0]+1;j++)
		{
		  wwi[ww[j]]=ninnout;
		}
	      for (j=bllst2ptr[connectmat[jj+connectmat[0]/2]];j<bllst2ptr[connectmat[jj+connectmat[0]/2]+1];j++)
		{
		  bllst2[j]=wwi[j-bllst2ptr[connectmat[jj+connectmat[0]/2]]+1];
		}
	      if(wwi) free(wwi);
	      if(ww) free(ww);

	      if (nzcross[connectmat[jj+connectmat[0]/2]] == nin){
		nzcross[connectmat[jj+connectmat[0]/2]] = ninnout;
	      }
	      if (nmode[connectmat[jj+connectmat[0]/2]] == nin){
		nmode[connectmat[jj+connectmat[0]/2]] = ninnout;
	      }
	      wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj+connectmat[0]/2]],bllst3ptr[connectmat[jj+connectmat[0]/2]+1]-bllst3ptr[connectmat[jj+connectmat[0]/2]]);
	      if (wwi) 
		{
		  ww=FindEg(wwi,nin);
		  if (ww)
		    {
		      for (j=1;j<ww[0]+1;j++)
			{
			  wwi[ww[j]]=ninnout;
			}
		      free(ww);
		      for (j=bllst3ptr[connectmat[jj+connectmat[0]/2]];j<bllst3ptr[connectmat[jj+connectmat[0]/2]+1];j++)
			{
			  bllst3[j]=wwi[j-bllst3ptr[connectmat[jj+connectmat[0]/2]]+1];
			}
		    }
		  
		  free(wwi);
		}
	      wwi=GetPartVect(bllst3,bllst3ptr[connectmat[jj+connectmat[0]/2]],bllst3ptr[connectmat[jj+connectmat[0]/2]+1]-bllst3ptr[connectmat[jj+connectmat[0]/2]]);
	      if (wwi)
		{
		  ww=FindEg(wwi,0);
		  wwi1=GetPartVect(bllst2,bllst2ptr[connectmat[jj+connectmat[0]/2]],bllst2ptr[connectmat[jj+connectmat[0]/2]+1]-bllst2ptr[connectmat[jj+connectmat[0]/2]]);
		  mini1=Min1(wwi1);
		  if (ww && (mini1 > 0))
		    {
		      a=Sum(wwi1);
		      for (j=1;j<ww[0]+1;j++)
			{
			  wwi[ww[j]]=a;
			}
		      for (j=bllst3ptr[connectmat[jj+connectmat[0]/2]];j<bllst3ptr[connectmat[jj+connectmat[0]/2]+1];j++)
			{
			  bllst3[j]=wwi[j-bllst3ptr[connectmat[jj+connectmat[0]/2]]+1];
			}
		    }
		  free(ww);ww=NULL;
		  free(wwi);wwi=NULL;
		  if (wwi1) free(wwi1);
		  wwi1=NULL;
		}
	    }
	}/* fin de for 2emme jj */
      if (!findflag )
	{ 
	  Message("I cannot find a link with undetermined size; My guess is that you have a block with unconnected; undetermined  output ports");
	  *ok=false;
	  return 0;
	}
    } /* fin de for hhjj */
  return 0;
} /* end function */
/* ======================================= endfunction adjust_inout ========================================== */

/* **************************************** function pak_ersi **************************************************** */
int pak_ersi(int** clkconnect,int* typ_r,int* typ_l,int* outoin,int* outoinptr,
              int* tblock,int* typ_cons,int* bllst5ptr,int** exe_cons,int nblk)
{
  int *all_out,*ind,*ind1,*vec,*cll,*ii,*oo,*r,*clkconnectind,*cllind,*nd,*pp,*ppget;
  int mm,j,l,k,a,nnd,max1;
  Mat4C clkconnecti;
  Mat2C exe_consi;
  Mat2C all_outi;
  nd=ind=ind1=all_out=vec=cll=ii=oo=r=clkconnectind=cllind=ppget=NULL;
  *exe_cons=NULL;
  if ((all_out=(int*)malloc(sizeof(int))) == NULL ) return 0;
  all_out[0]=0;                       
  for (k=1;k<bllst5ptr[0];k++)
    {
      if (typ_l[k]!=1)
        {
          a=bllst5ptr[k+1]-bllst5ptr[k];
	  if (a >= 1)
	    {
	      if ((all_out=(int*)realloc(all_out,sizeof(int)*(all_out[0]+2*a+1))) == NULL ) return 0;
	      if ((all_outi.col1=(int*)malloc(sizeof(int)*(all_out[0]/2+a+1))) == NULL ) return 0;
	      all_outi.col1[0]=all_out[0]/2+a;
	      if ((all_outi.col2=(int*)malloc(sizeof(int)*(all_out[0]/2+a+1))) == NULL ) return 0;
	      all_outi.col2[0]=all_out[0]/2+a;
	      for(l=1;l<all_out[0]/2+1;l++)
		{
		  all_outi.col1[l]=all_out[l];
		  all_outi.col2[l]=all_out[l+all_out[0]/2];
		}
	      for (j=1;j<a+1;j++)
		{
		  all_outi.col1[all_out[0]/2+j]=k;
		  all_outi.col2[all_out[0]/2+j]=j;
		}
	      all_out[0]=2*all_outi.col1[0];
	      for(l=1;l<all_out[0]/2+1;l++)
		{
		  all_out[l]=all_outi.col1[l];
		  all_out[l+all_out[0]/2]=all_outi.col2[l];
		}
	      free(all_outi.col1); all_outi.col1=NULL;
	      free(all_outi.col2); all_outi.col2=NULL;
	    }
	}
    } 
  if ((all_outi.col1=(int*)malloc(sizeof(int)*(all_out[0]/2+2))) == NULL ) return 0;
  all_outi.col1[0]=all_out[0]/2+1;
  if ((all_outi.col2=(int*)malloc(sizeof(int)*(all_out[0]/2+2))) == NULL ) return 0;
  all_outi.col2[0]=all_out[0]/2+1;
  for(l=1;l<all_out[0]/2+1;l++)
    {
      all_outi.col1[l]=all_out[l];
      all_outi.col2[l]=all_out[l+all_out[0]/2];
    }
  all_outi.col1[all_out[0]/2+1]=0;
  all_outi.col2[all_out[0]/2+1]=0;
  if ((all_out=(int*)realloc(all_out,sizeof(int)*(2*all_outi.col1[0]+1))) == NULL ) return 0;
  all_out[0]=2*all_outi.col1[0];
  for(l=1;l<all_out[0]/2+1;l++)
    {
      all_out[l]=all_outi.col1[l];
      all_out[l+all_out[0]/2]=all_outi.col2[l];
    }
  free(all_outi.col1);
  all_outi.col1=NULL;
  free(all_outi.col2);
  all_outi.col2=NULL;
  
  ind=FindEg(tblock,1);
  if (!ind)
    {
      if ((ind=(int*)malloc(sizeof(int))) == NULL ) return 0;
      ind[0]=0;
    }
  for (j=1;j<ind[0]+1;j++)
    {
      k=ind[j];
      if ((clkconnecti.col1=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+all_out[0]/2+1))) == NULL ) return 0;
      clkconnecti.col1[0]=(*clkconnect)[0]/4+all_out[0]/2;
      if ((clkconnecti.col2=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+all_out[0]/2+1))) == NULL ) return 0;
      clkconnecti.col2[0]=(*clkconnect)[0]/4+all_out[0]/2;
      if ((clkconnecti.col3=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+all_out[0]/2+1))) == NULL ) return 0;
      clkconnecti.col3[0]=(*clkconnect)[0]/4+all_out[0]/2;
      if ((clkconnecti.col4=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+all_out[0]/2+1))) == NULL ) return 0;
      clkconnecti.col4[0]=(*clkconnect)[0]/4+all_out[0]/2;
      for(l=1;l<(*clkconnect)[0]/4+1;l++)
	{
	  clkconnecti.col1[l]=(*clkconnect)[l];
	  clkconnecti.col2[l]=(*clkconnect)[l+(*clkconnect)[0]/4];
	  clkconnecti.col3[l]=(*clkconnect)[l+(*clkconnect)[0]/2];
	  clkconnecti.col4[l]=(*clkconnect)[l+3*(*clkconnect)[0]/4];
	}
      for(l=1;l<all_out[0]/2+1;l++)
	{
	  clkconnecti.col1[l+(*clkconnect)[0]/4]=all_out[l];
	  clkconnecti.col2[l+(*clkconnect)[0]/4]=all_out[l+all_out[0]/2];
	  clkconnecti.col3[l+(*clkconnect)[0]/4]=k;
	  clkconnecti.col4[l+(*clkconnect)[0]/4]=0;
	}
      free(*clkconnect);
      if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*clkconnecti.col1[0]+1))) == NULL ) return 0;
      (*clkconnect)[0]=4*clkconnecti.col1[0];
      for(l=1;l<(*clkconnect)[0]/4+1;l++)
	{
	  (*clkconnect)[l]=clkconnecti.col1[l];
	  (*clkconnect)[l+(*clkconnect)[0]/4]=clkconnecti.col2[l];
	  (*clkconnect)[l+(*clkconnect)[0]/2]=clkconnecti.col3[l];
	  (*clkconnect)[l+3*(*clkconnect)[0]/4]=clkconnecti.col4[l];
	}
      free(clkconnecti.col1);
      clkconnecti.col1=NULL;
      free(clkconnecti.col2);
      clkconnecti.col2=NULL;
      free(clkconnecti.col3);
      clkconnecti.col3=NULL;
      free(clkconnecti.col4);
      clkconnecti.col4=NULL;
    }/* fin de for k */
  free(all_out);all_out=NULL;
  ind1=FindEg(typ_cons,1);
  if (ind1)
    {
      if ((ind=(int*)realloc(ind,sizeof(int)*(ind[0]+ind1[0]+1))) == NULL ) return 0;
      pp=&ind[1+ind[0]];
      pp=memcpy(pp,&ind1[1],ind1[0]*sizeof(int));
      ind[0]=ind[0]+ind1[0];
      free(ind1); ind1=NULL;
    }
  if (((*exe_cons)=(int*)malloc(sizeof(int)*(2*ind[0]+1))) == NULL ) return 0;
  (*exe_cons)[0]=2*ind[0];
  for (j=1;j<ind[0]+1;j++)
    {
      (*exe_cons)[j]=ind[j];
      (*exe_cons)[j+ind[0]]=0;
    }
  if ((vec=(int*)malloc(sizeof(int)*(nblk+1))) == NULL ) return 0;
  vec[0]=nblk;
  Setmem(vec,-1);
  for (j=1;j<ind[0]+1;j++)
    {
      vec[ind[j]]=0;
    }
  nd=GetPartVect(outoin,outoin[0]/2+1,outoin[0]/2);
  if (!nd)
    {
      if ((nd=(int*)malloc(sizeof(int))) == NULL ) return 0;
      nd[0]=0;
    }
  max1=Max1(nd);
  free(nd);
  nnd=max1+1;
  if ((nd=(int*)calloc((nblk*nnd+1),sizeof(int))) == NULL ) return 0;
  nd[0]=nblk*nnd;
  
  tree4(vec,nd,nnd,outoin,outoinptr,typ_r,&r);
  if(nd) free(nd);
  if(vec) free(vec); vec=NULL;
  if (r)
    {  
      if ((exe_consi.col1=(int*)malloc(sizeof(int)*((*exe_cons)[0]/2+r[0]/2+1))) == NULL) return 0;
      
      if ((exe_consi.col2=(int*)malloc(sizeof(int)*(r[0]/2+(*exe_cons)[0]/2+1))) == NULL) return 0;
      exe_consi.col2[0]=(*exe_cons)[0]/2+r[0]/2;
      pp=&(exe_consi.col1)[1];
      pp=memcpy(pp,&(*exe_cons)[1],sizeof(int)*((*exe_cons)[0]/2));
      exe_consi.col1[0]=(*exe_cons)[0]/2+r[0]/2;
      pp=&(exe_consi.col2)[1];
      pp=memcpy(pp,&(*exe_cons)[(*exe_cons)[0]/2+1],sizeof(int)*((*exe_cons)[0]/2));
      pp=&(exe_consi.col1)[(*exe_cons)[0]/2+1];
      pp=memcpy(pp,&r[1],sizeof(int)*(r[0]/2));
      pp=&(exe_consi.col2)[(*exe_cons)[0]/2+1];
      pp=memcpy(pp,&r[1+r[0]/2],sizeof(int)*(r[0]/2));
      
      if (((*exe_cons)=(int*)realloc((*exe_cons),sizeof(int)*((*exe_cons)[0]+r[0]+1))) == NULL ) return 0;
      (*exe_cons)[0]=(*exe_cons)[0]+r[0];
      for(l=1;l<(*exe_cons)[0]/2+1;l++)
	{
	  (*exe_cons)[l]=exe_consi.col1[l];
	  (*exe_cons)[l+(*exe_cons)[0]/2]=exe_consi.col2[l];
	}
      free(exe_consi.col1);
      exe_consi.col1=NULL;
      exe_consi.col1=NULL;
      free(exe_consi.col2);
      exe_consi.col2=NULL;
      free(r); r=NULL;
    }
  if(ind) free(ind);
  
  mm=Max1(*clkconnect)+1;
  if ((cll=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+1))) == NULL ) return 0;
  cll[0]=(*clkconnect)[0]/4;
  for(j=1;j<cll[0]+1;j++)
    {
      cll[j]=-(*clkconnect)[j]*mm-(*clkconnect)[j+(*clkconnect)[0]/4];
    }
  if ((ind=(int*)malloc(sizeof(int)*(cll[0]+1))) == NULL ) return 0;
  ind[0]=cll[0];
  C2F(isort)(&cll[1],&cll[0],&ind[1]); 
  clkconnectind=VecEg1(*clkconnect);
  for(j=1;j<ind[0]+1;j++)
    {
      (*clkconnect)[j]=clkconnectind[ind[j]];
      (*clkconnect)[j+ind[0]]=clkconnectind[ind[j]+ind[0]];
      (*clkconnect)[j+2*ind[0]]=clkconnectind[ind[j]+2*ind[0]];
      (*clkconnect)[j+3*ind[0]]=clkconnectind[ind[j]+3*ind[0]];      
    }
  if(ind) free(ind);
  ind=NULL;
  if(clkconnectind) free(clkconnectind);
  clkconnectind=NULL;
  if ((cll=(int*)realloc(cll,sizeof(int)*((*clkconnect)[0]/4+3))) == NULL ) return 0;
  cll[0]=(*clkconnect)[0]/4+2;
  cllind=VecEg1(cll);
  for (j=2;j<cll[0]+1;j++)
    {
      cll[j]=-cllind[j-1];
    }   
  if(cllind) free(cllind);
  cllind=NULL;
  cll[1]=-1;
  cll[cll[0]]=mm;
  if ((vec=(int*)malloc(sizeof(int)*(cll[0]))) == NULL ) return 0;
  vec[0]=cll[0]-1;
  for (j=1;j<vec[0]+1;j++)
    {
      vec[j]=cll[j+1]-cll[j];
    }
  if(cll) free(cll);
  cll=NULL;
  ii=FindDif(vec,0);
  if(vec) free(vec);
  for (k=1;k<ii[0];k++)
    {
      if ((oo=(int*)malloc(sizeof(int)*(ii[k+1]-ii[k]+1))) == NULL ) return 0;
      oo[0]=ii[k+1]-ii[k];
      for (l=ii[k];l<ii[k+1];l++) 
        {
          oo[l-ii[k]+1]=l;
        }
      if ((vec=(int*)malloc(sizeof(int)*(nblk+1))) == NULL ) return 0;
      vec[0]=nblk;
      Setmem(vec,-1);
      for(j=1;j<oo[0]+1;j++)
        {
          vec[(*clkconnect)[oo[j]+(*clkconnect)[0]/2]]=0;
        }
      if (oo) free(oo); oo=NULL;
      ppget=GetPartVect(outoin,outoin[0]/2+1,outoin[0]/2);
      if(!ppget)
	{
	  if ((ppget=(int*)malloc(sizeof(int))) == NULL ) return 0;
	  ppget[0]=0;
	}
      max1=Max1(ppget);
      if (ppget) free(ppget);
      ppget=NULL;
      nnd=max1+1;
      if ((nd=(int*)calloc((nblk*nnd+1),sizeof(int))) == NULL ) return 0;
      nd[0]=nblk*nnd;
      
      tree4(vec,nd,nnd,outoin,outoinptr,typ_r,&r);
      
      if (nd) free(nd); nd=NULL;
      if(vec) free(vec); vec=NULL;
      
      if (r)
        {
          if ((clkconnecti.col1=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+r[0]/2+1))) == NULL ) return 0;
          clkconnecti.col1[0]=(*clkconnect)[0]/4+r[0]/2;
          if ((clkconnecti.col2=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+r[0]/2+1))) == NULL ) return 0;
          clkconnecti.col2[0]=(*clkconnect)[0]/4+r[0]/2;
          if ((clkconnecti.col3=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+r[0]/2+1))) == NULL ) return 0;
          clkconnecti.col3[0]=(*clkconnect)[0]/4+r[0]/2;
          if ((clkconnecti.col4=(int*)malloc(sizeof(int)*((*clkconnect)[0]/4+r[0]/2+1))) == NULL ) return 0;
          clkconnecti.col4[0]=(*clkconnect)[0]/4+r[0]/2;
	  for(l=1;l<(*clkconnect)[0]/4+1;l++)
            {
              clkconnecti.col1[l]=(*clkconnect)[l];
              clkconnecti.col2[l]=(*clkconnect)[l+(*clkconnect)[0]/4];
              clkconnecti.col3[l]=(*clkconnect)[l+(*clkconnect)[0]/2];
              clkconnecti.col4[l]=(*clkconnect)[l+3*(*clkconnect)[0]/4];
            }
          for(l=1;l<r[0]/2+1;l++)
            {     
              clkconnecti.col1[l+(*clkconnect)[0]/4]=(*clkconnect)[ii[k]];
              clkconnecti.col2[l+(*clkconnect)[0]/4]=(*clkconnect)[ii[k]+(*clkconnect)[0]/4];
              clkconnecti.col3[l+(*clkconnect)[0]/4]=r[l];
              clkconnecti.col4[l+(*clkconnect)[0]/4]=r[l+r[0]/2];
            }
	  free(*clkconnect);
          if (((*clkconnect)=(int*)malloc(sizeof(int)*(4*clkconnecti.col1[0]+1))) == NULL ) return 0;
          (*clkconnect)[0]=4*clkconnecti.col1[0];
	  for(l=1;l<(*clkconnect)[0]/4+1;l++)
            {
              (*clkconnect)[l]=clkconnecti.col1[l];
              (*clkconnect)[l+(*clkconnect)[0]/4]=clkconnecti.col2[l];
              (*clkconnect)[l+(*clkconnect)[0]/2]=clkconnecti.col3[l];
              (*clkconnect)[l+3*(*clkconnect)[0]/4]=clkconnecti.col4[l];
            }
          free(clkconnecti.col1);
          clkconnecti.col1=NULL;
          free(clkconnecti.col2);
          clkconnecti.col2=NULL;
          free(clkconnecti.col3);
          clkconnecti.col3=NULL;
          free(clkconnecti.col4);
          clkconnecti.col4=NULL;
	  free(r); r=NULL;
	}
    }/* fin de for k */
  if(ii) free(ii);
  ii=NULL;
  
  return 0;
}/* end function */
/* ======================================= endfunction pak_ersi =================================================== */
/* ************************************** function mini_extract_info ********************************************** */
int mini_extract_info(int* bllst2,int** bllst4,char **bllst10,int* bllst12,int* bllst2ptr,int* bllst3ptr,int** bllst4ptr,
		      int* typ_x,int* connectmat,int* clkconnect,int** inplnk,int** outlnk,int** typ_l,int** typ_r,int** typ_m,
		      int** tblock,int** typ_cons,int** typ_zx,int* nzcross,int* ok)

{
  int j,ptlnk,l,ko,ki,nbl=((int*)bllst10)[0];
  int *inpnum,*cinpnum,*bllst4i,*ind,*fff,*bllst4ptri;
  inpnum=cinpnum=bllst4i=ind=fff=bllst4ptri=NULL;
  *ok=true;     
  if ((fff=(int*)malloc(sizeof(int)*(nbl+1)))==NULL) return 0;
  fff[0]=nbl;   
  Setmem(fff,1);
  (*typ_l)=VecEg1(fff);
  (*typ_r)=VecEg1(fff);
  (*typ_cons)=VecEg1(fff);
  (*typ_m)=VecEg1(fff);
  (*typ_zx)=VecEg1(fff);
  (*tblock)=VecEg1(fff);
  if (fff) {free(fff);
  fff=NULL;}
  
  for(j=1;j<nbl+1;j++)
    {
      if ( !(typ_x[j]) && (nzcross[j] == 0)) (*typ_zx)[j]=0;
      bllst4ptri=VecEg1(*bllst4ptr);
      inpnum=GetPartVect(bllst2,bllst2ptr[j],bllst2ptr[j+1]-bllst2ptr[j]);
      cinpnum=GetPartVect(*bllst4,(*bllst4ptr)[j],(*bllst4ptr)[j+1]-(*bllst4ptr)[j]);
      if (cinpnum == NULL)
        {
          ok= false;
          ((int*)(*typ_r))[j]=!(bllst12[j+bllst12[0]/2]);
          ((int*)(*tblock))[j]=bllst12[j+bllst12[0]/2];
          if(!(bllst12[j+bllst12[0]/2]))
            {
              if (inpnum)
                {
                  if ((cinpnum=(int*)malloc(sizeof(int)*(inpnum[0]+1))) == NULL) return 0;
                  cinpnum[0]=inpnum[0]; 
                  Setmem(cinpnum,1);
                  bllst4i=VecEg1(*bllst4);
                  if (((*bllst4)=(int*)realloc((*bllst4),sizeof(int)*((*bllst4)[0]+cinpnum[0]+1))) == NULL ) return 0;
                  (*bllst4)[0]=(*bllst4)[0]+cinpnum[0];
                  for(l=j+1;l<(*bllst4ptr)[0]+1;l++) (*bllst4ptr)[l]=bllst4ptri[l]+cinpnum[0];
                  
                  for (l=(*bllst4ptr)[1];l<(*bllst4ptr)[j];l++)
                    {
                      (*bllst4)[l]=bllst4i[l];
                    }
                  for (l=1;l<cinpnum[0]+1;l++)
                    {
                      (*bllst4)[l+(*bllst4ptr)[j]-1]=cinpnum[l];
                    }
                  for (l=(*bllst4ptr)[j+1];l<(*bllst4ptr)[(*bllst4ptr)[0]];l++)
                    {
                      (*bllst4)[l]=bllst4i[l-cinpnum[0]];
                    }
                  if (bllst4i) free(bllst4i);
		}
            }
        } /* if cin */
      else
        {
          (*tblock)[j]=bllst12[j+bllst12[0]/2];
          (*typ_r)[j]=false;
        }       
      (*typ_l)[j]=*(bllst10[j]) == 'l';
      (*typ_m)[j]=*(bllst10[j]) == 'm';
      (*typ_cons)[j]=((cinpnum==NULL)&&(inpnum==NULL)&&(!bllst12[j+bllst12[0]/2]));
      if(bllst4ptri) free(bllst4ptri);
      if (inpnum) free(inpnum);
      if (cinpnum) free(cinpnum);
    } /* fin de for j */
  
  if (((*inplnk)=(int*)calloc(bllst2ptr[bllst2ptr[0]],sizeof(int))) == NULL) return 0;
  (*inplnk)[0]=bllst2ptr[bllst2ptr[0]]-1;       
  if (((*outlnk)=(int*)calloc(bllst3ptr[bllst3ptr[0]],sizeof(int))) == NULL) return 0;
  (*outlnk)[0]=bllst3ptr[bllst3ptr[0]]-1;       
  ptlnk=1;
  for (j=1;j<connectmat[0]/4+1;j++)
    {
      ko=(*outlnk)[bllst3ptr[connectmat[j]]+connectmat[j+connectmat[0]/4]-1];
      ki=(*inplnk)[bllst2ptr[connectmat[j+connectmat[0]/2]]+connectmat[j+3*connectmat[0]/4]-1];
      if (ko!=0 && ki!=0)
        {
          if (ko>ki)
            {
	      ind=FindSup(*outlnk,ko);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=(*outlnk)[ind[l]]-1;
                }
              if(ind) free(ind); ind=NULL;
              ind=FindEg(*outlnk,ko);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=ki;
                }

              if(ind) free(ind);ind=NULL;
              ind=FindSup(*inplnk,ko);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=(*inplnk)[ind[l]]-1;
                }
              if (ind) free(ind);ind=NULL;
              ind=FindEg(*inplnk,ko);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*inplnk)[ind[l]]=ki;
                }
              ptlnk--;
	      if(ind) free(ind); ind=NULL;
            } /* fin du 2i f */
          else if (ki>ko)
            {
	      ind=FindSup(*outlnk,ki);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=(*outlnk)[ind[l]]-1;
                }
              if(ind) free(ind); ind=NULL;
              ind=FindEg(*outlnk,ki);
              for (l=1;l<ind[0]+1;l++)
                {
                  (*outlnk)[ind[l]]=ko;
                }
             if(ind) free(ind); ind=NULL;
             ind=FindSup(*inplnk,ki);
             for (l=1;l<ind[0]+1;l++)
               {
                 (*inplnk)[ind[l]]=(*inplnk)[ind[l]]-1;
               }
             if (ind) free(ind); ind=NULL;
             ind=FindEg(*inplnk,ki);
             for (l=1;l<ind[0]+1;l++)
               {
                 (*inplnk)[ind[l]]=ko;
               }
             ptlnk--;
	     if (ind) free(ind); ind=NULL;
            }
        } /*fin du 1 if */
      else if (ko!=0)
        {
          (*inplnk)[bllst2ptr[connectmat[j+connectmat[0]/2]]+connectmat[j+3*connectmat[0]/4]-1]=ko;
        }
      else if (ki!=0)
        {
          (*outlnk)[bllst3ptr[connectmat[j]]+connectmat[j+connectmat[0]/4]-1]=ki;
        }
      else 
        {
          (*outlnk)[bllst3ptr[connectmat[j]]+connectmat[j+connectmat[0]/4]-1]=ptlnk;
          (*inplnk)[bllst2ptr[connectmat[j+connectmat[0]/2]]+connectmat[j+3*connectmat[0]/4]-1]=ptlnk;
          ptlnk++;
        }
    } /* fin du 2 emme for j */
  ind=FindEg(*outlnk,0);
  if (ind)
    { 
      ko=Max1(*outlnk);
      for (j=1;j<ind[0]+1;j++)
        {
          (*outlnk)[ind[j]]=ko+1;
        }   
      free(ind);ind=NULL;
    }   
  
  return 0;
} /* fin de function */
/* ======================================= endfunction mini_extract_info =================================================== */
/* *************************************** function synch_clkconnect ******************************************************* */
int synch_clkconnect(int* typ_s,int* clkconnect,int** evoutoin,int** evoutoinptr)
{
  int *dd,*ind,*ppget;
  register  int j,l;
  ind=NULL;
  dd=NULL;
  ppget=NULL;
  if (((*evoutoin)=(int*)malloc(sizeof(int))) == NULL ) return 0;
  (*evoutoin)[0]=0;
  if (((*evoutoinptr)=(int*)malloc(sizeof(int)*2)) == NULL ) return 0;
  (*evoutoinptr)[0]=1;
  (*evoutoinptr)[1]=1;
  for (j=1;j<typ_s[0]+1;j++)
    {
      if (typ_s[j])
        {
          
	  ppget=GetPartVect(clkconnect,1,clkconnect[0]/4);
          ind=FindEg(ppget,j);
	  if (ppget)free(ppget); ppget=NULL;
          if (ind) 
            {
              if ((dd=(int*)malloc(sizeof(int)*(ind[0]+1))) == NULL ) return 0;
              dd[0]=ind[0];
              for(l=1;l<ind[0]+1;l++)
                {
                  dd[l]=clkconnect[ind[l]+clkconnect[0]/2];
                }
	      free(ind); ind=NULL;
	      if (!dd) 
		{
		  if ((dd=(int*)malloc(sizeof(int))) == NULL ) return 0;
		  dd[0]=0;
		}
            }	 
        }
      else 
        {
          if ((dd=(int*)malloc(sizeof(int))) == NULL ) return 0;
          dd[0]=0;
        }
              
      if (((*evoutoin)=(int*)realloc((*evoutoin),sizeof(int)*((*evoutoin)[0]+dd[0]+1))) == NULL ) return 0;
      if (((*evoutoinptr)=(int*)realloc((*evoutoinptr),sizeof(int)*((*evoutoinptr)[0]+2))) == NULL ) return 0;
      for(l=1;l<dd[0]+1;l++)
        {
          (*evoutoin)[l+(*evoutoin)[0]]=dd[l];
        }
      
      (*evoutoinptr)[(*evoutoinptr)[0]+1]=(*evoutoinptr)[(*evoutoinptr)[0]]+dd[0];
      (*evoutoin)[0]=(*evoutoin)[0]+dd[0];
      (*evoutoinptr)[0]++;
      free(dd); dd=NULL;      
    }
  
  return 0;
} /* fin de function */
/* ======================================= endfunction synch_clkconnect ===================================================== */
/* *************************************** function cleanup ******************************************************* */
int cleanup(int** clkconnect)
{
  int a,i,mm,j,*ind,*vec,*clkconnectind,val=0;
  double *cc;
  ind=vec=NULL;
  cc=NULL;
  mm=Max1(*clkconnect)+1;
  if ((cc=(double*)malloc(sizeof(double)*((*clkconnect)[0]/4+1))) == NULL ) return 0;
  a=(*clkconnect)[0]/4;
  cc[0]=a;
  for (j=1;j<(*clkconnect)[0]/4+1;j++)
    {
      cc[j]=-(*clkconnect)[j+3*(*clkconnect)[0]/4]-mm*(*clkconnect)[j+(*clkconnect)[0]/2]-power(mm,2)*(*clkconnect)[j+(*clkconnect)[0]/4]-(double) power(mm,3)*(*clkconnect)[j];
    }
  if ((ind=(int*)malloc(sizeof(int)*(a+1))) == NULL ) return 0;
  ind[0]=a;
  C2F(dsort)(&cc[1],&a,&ind[1]);
  clkconnectind=VecEg1(*clkconnect);
  for(j=1;j<ind[0]+1;j++)
    {
      (*clkconnect)[j]=clkconnectind[ind[j]];
      (*clkconnect)[j+ind[0]]=clkconnectind[ind[j]+ind[0]];
      (*clkconnect)[j+2*ind[0]]=clkconnectind[ind[j]+2*ind[0]];
      (*clkconnect)[j+3*ind[0]]=clkconnectind[ind[j]+3*ind[0]];      
    }
  free(clkconnectind); clkconnectind=NULL;
  if(ind) free(ind);ind=NULL;
  if ((vec=(int*)malloc((size_t) (sizeof(int)*(cc[0])))) == NULL ) return 0;
  vec[0]=(int) cc[0]-1;
  for (j=1;j<vec[0]+1;j++)
    {
      vec[j]=(int) (cc[j+1]-cc[j]);
    }
  if(cc) free(cc); cc=NULL;
  ind=FindEg(vec,0);
  if(vec) free(vec); vec=NULL;
  if (ind)
    {
      for(j=1;j<ind[0]+1;j++)
        {
          (*clkconnect)[ind[j]]=-4321;
          (*clkconnect)[ind[j]+(*clkconnect)[0]/4]=-4321;
          (*clkconnect)[ind[j]+(*clkconnect)[0]/2]=-4321;
          (*clkconnect)[ind[j]+3*(*clkconnect)[0]/4]=-4321;
        }
      for(i=1;i<(*clkconnect)[0]+1;i++)
        {
          if( (*clkconnect)[i]!=-4321 )  (*clkconnect)[i-val]=(*clkconnect)[i];
          else
            val++;
	}
      (*clkconnect)[0]=(*clkconnect)[0]-4*ind[0];
      free(ind); ind=NULL;
    }
  
  return 0;
} /* end function */
/* ======================================= endfunction cleanup ===================================================== */
/* *************************************** function init_agenda ******************************************************* */
int init_agenda(double* initexe,int* bllst5ptr,double** tevts,int** evtspt,int* pointi)
{
  int nblk,j,ninit,nevts;
  int *indtime,a;
  double *timevec,*initexeind,*pp;
  initexeind=NULL;
  timevec=NULL;
  indtime=NULL;
  nblk=bllst5ptr[0]-1;
  timevec=GetPartVectd(initexe,2*((int) initexe[0])/3+1,(int) initexe[0]/3);
  if ( timevec )
    {
      Invd(timevec);
      a=(int) timevec[0];
      if ((indtime=(int*)malloc(sizeof(int)*(a+1))) == NULL ) return 0;
      indtime[0]=a;
      C2F(dsort)(&timevec[1],&a,&indtime[1]);
      if ((initexeind=(double*)malloc(sizeof(double)*((int) initexe[0]+1))) == NULL ) return 0; 
      initexeind[0]=(int) initexe[0];
      pp=&initexeind[1];
      pp=memcpy(pp,&initexe[1],sizeof(double)*((int) initexe[0]));
      for(j=1;j<indtime[0]+1;j++)
        {
          initexe[j]=initexeind[indtime[j]];
          initexe[j+(int) initexeind[0]/3]=initexeind[indtime[j]+(int) initexeind[0]/3];
          initexe[j+2* (int) initexeind[0]/3]=initexeind[indtime[j]+2* (int) initexeind[0]/3];
        }
      if(initexeind) free(initexeind);
      if(indtime) free(indtime);
      free(timevec);
      ninit=(int) initexe[0]/3;
    }
  else {initexe=NULL;ninit=0;}
  
  *pointi=0;
  nevts=bllst5ptr[nblk+1]-1;
  if (((*tevts)=(double*)calloc((nevts+1),sizeof(double))) == NULL) return 0 ;
  ((*tevts)[0])=nevts;
  if ( initexe != NULL)
    {
      for (j=1;j<ninit+1;j++)
        {
          (*tevts)[bllst5ptr[(int) initexe[j]]+(int) initexe[j+ninit]-1]=initexe[j+2*ninit];
        }
    }
  if (((*evtspt)=(int*)malloc(sizeof(int)*(nevts+1))) == NULL) return 0 ;
  (*evtspt)[0]=nevts;
  Setmem(*evtspt,-1);
  if ( ninit > 0 )
    {
      *pointi=bllst5ptr[(int) initexe[1]]+(int) initexe[1+ninit]-1;
      (*evtspt)[*pointi]=0;
    }
  if ( ninit > 1 )
    {
      for (j=1;j<ninit;j++)
        {
          (*evtspt)[bllst5ptr[(int) initexe[j]]+(int) initexe[j+ninit]-1]=bllst5ptr[(int) initexe[j+1]]+(int) initexe[j+1+ninit]-1;
        }
      (*evtspt)[bllst5ptr[(int) initexe[ninit]]+(int) initexe[ninit+ninit]-1]=0;
    }
  return 0;
} /* end function */
/* ======================================= endfunction init_agenda ===================================================== */
/********************** function tree4 *********************************************/
int tree4(int* vec,int* nd,int nnd,int* outoin,int* outoinptr,int* typ_r,int** r)
{
  int i,j,fini=0,k,ii,*ind,*ind1,l,nb=vec[0];
  Mat2C ri;
  if ((*r=(int*)malloc(sizeof(int))) == NULL) return 0 ;
  (*r)[0]=0;
  ind=GetPartVect(outoin,1,outoin[0]/2);
  ind1=GetPartVect(outoin,outoin[0]/2+1,outoin[0]/2);
  for (j = 1 ; (j < nb) &&  (! fini); j++)
    {
      fini=1;
      for (i=1;i<nb+1;i++)
        {
          if (vec[i] > -1)
            {
              if (outoinptr[i+1]-outoinptr[i] !=0){
              for (k=outoinptr[i];k<outoinptr[i+1];k++)
                {                 
                  ii=ind[k];
		  if(typ_r[ii] == 1)
                    {
                      if(nd[ind1[k]+1+(ii-1)*nnd] == 0){
                      if ((ri.col1=(int*)malloc(sizeof(int)*((*r)[0]/2+2))) == NULL ) return 0;
                      ri.col1[0]=(*r)[0]/2+1;
                      if ((ri.col2=(int*)malloc(sizeof(int)*((*r)[0]/2+2))) == NULL ) return 0;
                      ri.col2[0]=(*r)[0]/2+1;
                      for(l=1;l<(*r)[0]/2+1;l++)
                        {
                          ri.col1[l]=(*r)[l];
                          ri.col2[l]=(*r)[l+(*r)[0]/2];
                        }
                      ri.col1[(*r)[0]/2+1]=ind[k];
                      ri.col2[(*r)[0]/2+1]=ind1[k];
                      free(*r);
                      if ((*r=(int*)malloc(sizeof(int)*(2*ri.col1[0]+1))) == NULL ) return 0;
                      (*r)[0]=2*ri.col1[0];
                      for(l=1;l<(*r)[0]/2+1;l++)
                        {
                          (*r)[l]=ri.col1[l];
                          (*r)[l+(*r)[0]/2]=ri.col2[l];
                        }
                      free(ri.col1);
                      ri.col1=NULL;
                      free(ri.col2);
                      ri.col2=NULL;
                      vec[ii]=0;
                      fini=0;
                      nd[ind1[k]+1+(ii-1)*nnd]=1;
                      }
		    } /* fin de if(typ_r)                 */
                } /* fin de for k*/
              } /*fin de if*/
            } /* fin de if vec(i)*/
        } /* fin de for i*/
    } /* fin de for j*/
  if ((*r)[0]==0) 
    {
      free(*r);
      *r=NULL;
    }
  free(ind1);
  ind1=NULL;
  free(ind);
  ind=NULL;
  
  return 0;
} /* end function*/

/* =================== endfunction tree4 ========================================== */
/************************* function tree2 *********************************/
int tree2(vect, nb, wec, ind, deput, outoin, outoinptr, ord, ok)
int *vect,nb,*wec,*ind,*deput,*outoin,*outoinptr,**ord,*ok;
{
  int fini=0,i,j,k,m,ii,*ord1,n;
  ord1=NULL;
  *ok = 1;
  for (j = 1; (j <= nb+2) && (! fini); ++j) 
    {
      fini = 1;
      for (i = 1; i <= nb; ++i) 
        {
          if (vect[i] == j - 1) 
            {
              if (j == nb + 2) 
                {
                  *ok = 0;
                  *ord=NULL;
                  return 0;
                }
              if (outoinptr[ind[i] + 1] - outoinptr[ind[i]] != 0) 
                {                 
                  for (k = outoinptr[ind[i]]; k <= outoinptr[ind[i] + 1] - 1; ++k) 
		    {
		      ii = outoin[k];
		      for (n = 1; n <= nb; ++n)
			{
			  if (ind[n] == ii   && vect[n] > -1 && deput[ii] == 1) 
			    {
			      fini = 0;
			      vect[n] = j;
			    }
			}
                    }
		}
	    }
	}
    }
  Inv(vect);  
  if ((ord1=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return 0;
  ord1[0]=vect[0];
  C2F(isort)(&vect[1],&vect[0],&ord1[1]);
  if ((*ord=(int*)malloc(sizeof(int)*(2*nb+1))) == NULL ) return 0;
  (*ord)[0]=2*nb;
  for(m=1 ; m <= nb; ++m)
    {
      (*ord)[m]=ind[ord1[m]];
      (*ord)[m+nb]=wec[ord1[m]];
    }
  if(ord1) free(ord1);
  ord1=NULL;
  return 0;
} 
/* ===========================================   fin de tree2 ======================================== */ 
/* ********************************************** function tree3 ***************************************** */
int tree3(vec, nb, deput, typl, bexe, boptr, blnk, blptr,ord, ok)
     int *vec,nb,*deput,*typl,*bexe,*boptr,*blnk,*blptr,**ord,*ok;
{
  int fini,i,l,j,m,nkk,*w,kk,*ind;
  
  w=NULL;
  *ok = 1;
  for( i= 1; i <= nb; ++i)
    {
      if ((vec[i] == 0) && (typl[i] == 1)) vec[i]=1; 
    }
  fini=0;
  for (j = 1; (j <= nb+2) &&(! fini); ++j) 
    {
      fini = 1;
      if (j == nb + 2) 
	{
	  *ok = 0;
	  *ord = NULL;
	  return 0;
	}
      
      for (i = 1; i <= nb; ++i) 
	{
          if (vec[i] > -1 && typl[i] != -1) 
            {
	      if (typl[i] == 1) 
                {
                  nkk = boptr[i + 1] - boptr[i];
                  if (nkk != 0) 
                    {
                      for (m = 1; m <= nkk; ++m) 
                        {
                          kk = bexe[m + boptr[i] - 1];
			  if (typl[kk] == 1) 
			    {
			      if (vec[kk] < vec[i]+2)
				{
				  fini=0;
				  vec[kk]=vec[i]+2;
				}
			    }
			  else
			    {
			      if (vec[kk] < vec[i]+1)
				{
				  fini=0;
				  vec[kk]=vec[i]+1;
				} 
			    }
			}
		    }
                } 
              else 
                {
                  nkk = blptr[i+1]-blptr[i];
                  if (nkk != 0) 
                    {
                      for (m = 1; m <= nkk; ++m) 
                        {
                          kk = blnk[m+blptr[i]-1];
                          if (vec[kk] > -1 && (deput[kk] == 1 || typl[kk] == 1)) 
                            {
			      if (vec[kk] < vec[i])
				{
				  fini=0;
				  vec[kk]=vec[i];
				}
			    }
                        }
                    }
                }
	    }
          
	}
    }
  Inv(vec);
  if ((ind=(int*)malloc(sizeof(int)*(vec[0]+1))) == NULL ) return 0;
  ind[0]=vec[0];
  C2F(isort)(&vec[1],&vec[0],&ind[1]);
  w=FindDif(vec,1);
  if (((*ord)=(int*)malloc(sizeof(int)*(w[0]+1))) == NULL) return 0;
  (*ord)[0]=w[0];
  for (l= 1; l<w[0]+1;l++)
    {
      (*ord)[l]=ind[w[l]];
    }
  free(w); w=NULL;
  free(ind); ind=NULL;
  return 0;
}
/*===========================================   fin de tree3 ======================================== */


/***************************************************************/
/*    fonctions de gestions des matrices vecteurs             */ 
/*************************************************************/

int* GetCollVect(int* vect,int* vectid,int numcoll)
{
  int *vectr;
  int i;
  if ((vectr=(int*)malloc(sizeof(int)*(vectid[0]+1))) == NULL ) return (int*) NULL;
  vectr[0]=vectid[0];

  for(i=1;i<vectid[0]+1;i++)
    {
      vectr[i]=vect[vectid[i]+vect[0]*(numcoll-1)/4];
    }
  return (int*) vectr;
}

int* FindDif(int* vect,int val)
{
  int *vectr,i,j=1;
  if (vect[0]==0) return NULL;
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
       if(vect[i]!=val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr;
}

int* FindEg(int* vect,int val)
{     
  int *vectr,i,j=1;
  if (vect[0]==0) return NULL;
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
      if(vect[i]==val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr; 
}

int* FindSupEgd(double* vect,int val)
{     
  int *vectr;
  register int i,j=1;
  if (vect[0]==0) return NULL;
  if ((vectr=(int*)malloc(sizeof(int)*((int) vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
      if((int) vect[i]>=val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr; 

}

int* GetPartVect(int* vect,int idebut,int taille)
{
  int *vectr,*p;
  if (taille < 1) return (int*) NULL;
  if ((vectr=(int*)malloc(sizeof(int)*(taille+1))) == NULL ) return (int*) NULL;
  vectr[0]=taille;
  p=&vectr[1];
  p=memcpy(p,&vect[idebut],sizeof(int)*taille);
  return (int*) vectr;
}

double* GetPartVectd(double* vect,int idebut,int taille)
{
  double *vectr,*p;
  if (taille < 1) return (double*) NULL;
  if ((vectr=(double*)malloc(sizeof(double)*(taille+1))) == NULL ) return (double*) NULL;
  vectr[0]=taille;
  p=&vectr[1];
  p=memcpy(p,&vect[idebut],sizeof(double)*taille);
  return (double*) vectr;
}

int* Duplicataa(v,w)
     int *v,*w;
{
  register int i,j,k;
  int *ww;
  register int pp;
  k=1;
  if ((ww=(int*)malloc(sizeof(int))) == NULL ) return (int*) NULL;
  ww[0]=0;
  for (i=1;i<v[0]+1;i++) 
    {
      if (w[i]!=0)
        {
          if ((ww=(int*)realloc(ww,sizeof(int)*(w[i]+ww[0]+1))) == NULL ) return (int*) NULL;
          ww[0]=ww[0]+w[i];
          pp=v[i];
          for (j=1;j<w[i]+1;j++) 
            {
              ww[k]=pp;
              k++;
            }
        }
    }
  return (int*) ww;
}

int power (int base, int n) 
{
  register int i,p;        
  p = 1;
  for (i = 1; i <= n; i++)
    p *= base;
  return p;
}

double powerd (double base, int n) 
{
  int i;
  double p;        
  p = 1;
 
  if ( n < 0 ) 
    {
      base = 1/base;
      n=-n;
    }
  for (i = 1; i <= n; i++)
    p *= base;
  return (double) p;
}

int* FindInfEg(int* vect,int val)
{  
  int *vectr;
  register int i,j=1;
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
      if(vect[i]<=val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr;
}

int* FindInf(int* vect,int val)
{  
  int *vectr;
  register int i,j=1;
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
      if(vect[i]<val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr;
}

int* FindSup(int* vect,int val)
{
  int *vectr;
  register int i,j=1;
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  for(i=1; i<vect[0]+1;i++)
    {
      if(vect[i]>val) vectr[j++]=i;
    }
  vectr[0]=j-1;
  return (int*) vectr;
}

int Sum (int *vect) 
{
  int i,sum=0; 
  for (i = 1; i <= vect[0]; i++)
    {
      sum+=vect[i];
    }
  return sum;
}

int Norm (int *vect) 
{
  int i,sum=0; 
  for (i = 1; i <= vect[0]; i++)
    {
      if(vect[i]>=0) {
	sum+=vect[i];
      }else{
	sum+=-vect[i];
      }
    }
  return sum;
}

void CumSum (int* vect) 
{
  int i;        
  for (i = 2; i <= vect[0]; i++)
    {
      vect[i]+=vect[i-1];
    }
}



int* VecEg1 (int* vect) 
{  
  int *vectr;        
  if ((vectr=(int*)malloc(sizeof(int)*(vect[0]+1))) == NULL ) return (int*) NULL;
  vectr=memcpy(vectr,vect,(vect[0]+1)*sizeof(int));
  return (int*) vectr;
}

void Setmem (int* vect,int val) 
{   
  int j;        
  for (j = 1; j <= vect[0]; j++)
    {
      vect[j]=val;
    }
}

void Inv(int* vect) 
{
  int i;   
  for(i=1;i<vect[0]+1;i++)
    {
      vect[i]=-vect[i];
    }
}

void Incr2(int* vect,int j) 
{
  int i;        
  for (i = j; i <= vect[0]/2-1; i++)
    {
      vect[i]=vect[i+1];
      vect[i+vect[0]/2]=vect[i+1+vect[0]/2];      
    }
  vect[0]=vect[0]-2;
}
void Invd(double* vect) 
{
  int i;   
  for(i=1;i<(int) vect[0]+1;i++)
    {
      vect[i]=-vect[i];
    }
}
void Incr1(int* vect,int j) 
{
  int i;        
  for (i = j; i <= vect[0]-1; i++)
    {
      vect[i]=vect[i+1];
    }
  vect[0]--;
}



