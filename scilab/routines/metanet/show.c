#include <stdio.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef __ABSC__
#define getpid() getpid_()
#endif

#include "../machine.h"

#include "../libcomm/libCalCom.h"
#include "../libcomm/libCom.h"

#include "netcomm.h"

extern Message attendre_reponse();
extern void cerro();
extern int checkNetconnect();
extern int checkTheNetwindow();
extern void SendMsg();

extern int metanetSync;
extern char *Netwindows[];
extern int theNetwindow;

typedef int (*PF)();

static int CompString(s1, s2)
char **s1, **s2;
{
  return strcmp((char*)*s1,(char*)*s2);
}

#define mystrcat(s1,s2) istr=0;while(s2[istr]!='\0'){s1[ib++]=s2[istr++];}s1[ib++]=CHARSEP;

#define MAXNAM 80
#define MAXISIZE 16 /* estimated maximum number of digits in an int */

static char description[2*MAXNAM];

void C2F(showp)(p,psize,sup)
int *p;
int *psize;
int *sup;
{
  char *g;
  char str[MAXNAM];
  int i,istr;
  int ib = 0;

  if (!checkNetconnect() || !checkTheNetwindow()) return;

  if (*psize == 0) return;

  if ((g = (char *)malloc((unsigned)((MAXISIZE + 1) * *psize + 1))) == NULL) {
    cerro("Running out of memory");
    return;
  }

  for (i = 0; i < *psize; i++) {
    sprintf(str,"%d",p[i]);
    mystrcat(g,str);
  }
  g[ib] = '\0';

  if (metanetSync) {
    if (*sup) SendMsg(SHOWP1_S,g); else SendMsg(SHOWP_S,g);
  }
  else {
    if (*sup) SendMsg(SHOWP1,g); else SendMsg(SHOWP,g);
  }

  free(g);

  if (metanetSync) {
    attendre_reponse(Netwindows[theNetwindow-1],
		     MSG_DISTRIB_LISTE_ELMNT,
		     NBP_DISTRIB_LISTE_ELMNT);
  }
}

void C2F(showns)(ns,nsize,sup)
int *ns;
int *nsize;

int *sup;
{
  char *g;
  char str[MAXNAM];
  int i,istr;
  int ib = 0;

  if (!checkNetconnect() || !checkTheNetwindow()) return;

  if (*nsize == 0) return;

  if ((g = (char *)malloc((unsigned)((MAXISIZE + 1) * *nsize + 1))) == NULL) {
    cerro("Running out of memory");
    return;
  }

  for (i = 0; i < *nsize; i++) {
    sprintf(str,"%d",ns[i]);
    mystrcat(g,str);
  }
  g[ib] = '\0';

  if (metanetSync) {
    if (*sup) SendMsg(SHOWNS1_S,g); else SendMsg(SHOWNS_S,g);
  }
  else {
    if (*sup) SendMsg(SHOWNS1,g); else SendMsg(SHOWNS,g);
  }

  free(g);

  if (metanetSync) {
    attendre_reponse(Netwindows[theNetwindow-1],
		     MSG_DISTRIB_LISTE_ELMNT,
		     NBP_DISTRIB_LISTE_ELMNT);
  }
}

void C2F(showg)(
  name,lname,directed,node_number,tail,head,
  node_name,node_type,node_x,node_y,node_color,node_diam,node_border,
  node_font_size,node_demand,
  edge_name,edge_color,edge_width,edge_hi_width,edge_font_size,
  edge_length,edge_cost,edge_min_cap,edge_max_cap,edge_q_weight,edge_q_orig,
  edge_weight,
  default_node_diam,default_node_border,default_edge_width,
  default_edge_hi_width,default_font_size,
  is_nlabel,node_label,is_elabel,edge_label,
  ma,window,sup,scale)
char *name; int *lname;int *directed,*node_number,*tail,*head;
char ***node_name; 
int *node_type,*node_x,*node_y,*node_color,*node_diam,*node_border;
int *node_font_size; double *node_demand;
char ***edge_name;
int *edge_color,*edge_width,*edge_hi_width,*edge_font_size;
double *edge_length,*edge_cost,*edge_min_cap,*edge_max_cap;
double *edge_q_weight,*edge_q_orig,*edge_weight;
int *default_node_diam,*default_node_border,*default_edge_width;
int *default_edge_hi_width,*default_font_size;
char ***node_label, ***edge_label;
int *is_nlabel, *is_elabel;
int *ma, *window, *sup;
double *scale;
{
  int i,istr,ln;
  char fname[2 * MAXNAM];
  char str[MAXNAM],str1[MAXNAM];
  char *b;
  int ib = 0;
  FILE *f;
  int isize = sizeof(int);
  int dsize = sizeof(double);
  int csize = sizeof(char);
  int pid;
  char **lar;

 /* check uniqueness of node names */
  if (*node_number != 1) {
    if ((lar = (char **)malloc(sizeof(char *) * *node_number)) == NULL) {
      cerro("Running out of memory");
      return;
    }
    for (i = 0; i < *node_number; i++) lar[i] = (*node_name)[i];
    qsort((char*)lar,*node_number,sizeof(char*),(PF)CompString);
    for (i = 0; i < *node_number - 1; i++) {
      if (!strcmp(lar[i],lar[i+1])) {
	sprintf(description,
		"Bad graph file. Node \"%s\" is duplicated",lar[i]);
	cerro(description);
	free(lar);
	return;
      }
    }
    if (!strcmp(lar[*node_number - 2],lar[*node_number - 1])) {
      sprintf(description,
	      "Bad graph file. Node \"%s\" is duplicated",lar[*node_number - 2]);
      cerro(description);
      free(lar);
      return;
    }
    free(lar);
  }

  pid = getpid();

  sprintf(fname,"/tmp/%d.metanet.%d",pid,*window);
  f = fopen(fname,"w+");
  if (f == NULL) {
    cerro("Unable to write in directory \"/tmp\"");
    return;
  }
  
  /* name */
  name[*lname] = '\0';

  /* directed */
  fwrite((char*)directed,isize,1,f);

  /* scale */
  fwrite((char*)scale,dsize,1,f);

  /* n */
  fwrite((char*)node_number,isize,1,f);

  /* ma */
  fwrite((char*)ma,isize,1,f);

  /* tail */
  fwrite((char*)tail,isize,*ma,f);

  /* head */
  fwrite((char*)head,isize,*ma,f);

  /* node_name */
  for (i = 0; i < *node_number; i++) {
    ln = strlen((*node_name)[i]);
    fwrite((char*)&ln,isize,1,f);
    fwrite((*node_name)[i],csize,ln+1,f);
    free((*node_name)[i]);
  }
  free((char *)*node_name);

  /* node_type */
  fwrite((char*)node_type,isize,*node_number,f);

  /* node_x */
  fwrite((char*)node_x,isize,*node_number,f);

  /* node_y */
  fwrite((char*)node_y,isize,*node_number,f);

  /* node_color */
  fwrite((char*)node_color,isize,*node_number,f);

  /* node_diam */
  fwrite((char*)node_diam,isize,*node_number,f);

  /* node_border */
  fwrite((char*)node_border,isize,*node_number,f);

  /* node_font_size */
  fwrite((char*)node_font_size,isize,*node_number,f);

  /* node_demand */
  fwrite((char*)node_demand,dsize,*node_number,f);

  /* edge_name */
  for (i = 0; i < *ma; i++) {
    ln = strlen((*edge_name)[i]);
    fwrite((char*)&ln,isize,1,f);
    fwrite((*edge_name)[i],csize,ln+1,f);
    free((*edge_name)[i]);
  }
  free((char *)*edge_name);

  /* edge_color */
  fwrite((char*)edge_color,isize,*ma,f);

  /* edge_width */
  fwrite((char*)edge_width,isize,*ma,f);

  /* edge_hi_width */
  fwrite((char*)edge_hi_width,isize,*ma,f);

  /* edge_font_size */
  fwrite((char*)edge_font_size,isize,*ma,f);

  /* edge_length */
  fwrite((char*)edge_length,dsize,*ma,f);

  /* edge_cost */
  fwrite((char*)edge_cost,dsize,*ma,f);

  /* edge_min_cap */
  fwrite((char*)edge_min_cap,dsize,*ma,f);

  /* edge_max_cap */
  fwrite((char*)edge_max_cap,dsize,*ma,f);

  /* edge_q_weight */
  fwrite((char*)edge_q_weight,dsize,*ma,f);

  /* edge_q_orig */
  fwrite((char*)edge_q_orig,dsize,*ma,f);

  /* edge_weight */
  fwrite((char*)edge_weight,dsize,*ma,f);

  /* default_node_diam */
  fwrite((char*)default_node_diam,isize,1,f);

  /* default_node_border */
  fwrite((char*)default_node_border,isize,1,f);

  /* default_edge_width */
  fwrite((char*)default_edge_width,isize,1,f);

  /* default_edge_hi_width */
  fwrite((char*)default_edge_hi_width,isize,1,f);

  /* default_font_size */
  fwrite((char*)default_font_size,isize,1,f);
  
  /* node_label */
  fwrite((char*)is_nlabel,isize,1,f);
  if (*is_nlabel) {
    for (i = 0; i < *node_number; i++) {
      ln = strlen((*node_label)[i]);
      fwrite((char*)&ln,isize,1,f);
      fwrite((*node_label)[i],csize,ln+1,f);
      free((*node_label)[i]);
    }
    free((char *)*node_label);
  }

  /* edge_label */
  fwrite((char*)is_elabel,isize,1,f);
  if (*is_elabel) {
    for (i = 0; i < *ma; i++) {
      ln = strlen((*edge_label)[i]);
      fwrite((char*)&ln,isize,1,f);
      fwrite((*edge_label)[i],csize,ln+1,f);
      free((*edge_label)[i]);
    }
    free((char *)*edge_label);
  }

  fclose(f);

  sprintf(str,"%d",pid);
  sprintf(str1,"%d",*window);

  if ((b = (char *)
       malloc((unsigned)(strlen(str)+strlen(str1)+strlen(name)+4))) == NULL) {
    cerro("Running out of memory");
    return;
  }

  mystrcat(b,name);
  mystrcat(b,str);
  mystrcat(b,str1);
  b[ib] = '\0';

  if (metanetSync) {
    if (*sup) SendMsg(LOAD1_S,b); else SendMsg(LOAD_S,b);
  }
  else {
    if (*sup) SendMsg(LOAD1,b); else SendMsg(LOAD,b);
  }
  
  free(b);

  if (metanetSync) {
    attendre_reponse(Netwindows[theNetwindow-1],
		     MSG_DISTRIB_LISTE_ELMNT,
		     NBP_DISTRIB_LISTE_ELMNT);
  }
}
