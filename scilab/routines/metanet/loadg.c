#include <stdio.h>
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
#ifdef __FreeBSD__
#include <sys/types.h>
#endif
#include <dirent.h>
#endif
#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#if (defined __MSC__) || (defined __ABSC__) || (defined __MINGW32__)
/** only used for x=dir[1024] **/
#ifndef __ABSC__
#define  getwd(x) _getcwd(x,1024)
#else
#define  getwd(x) getcwd_(x,1024)
#endif
#endif

#include "mysearch.h"
#include "../machine.h"
#include "defs.h"

#define MAXNAM 80

extern double atof();
extern char* my_basename();
extern int CheckGraphName();
extern void cerro();
extern char* dirname();
extern char *StripGraph();

typedef int (*PF)();

static int CompString(s1, s2)
char **s1, **s2;
{
  return strcmp((char*)*s1,(char*)*s2);
}

static char description[2*MAXNAM];

void C2F(loadg)(path,lpath,name,lname,directed,n,tail,head,
  node_name,node_type,node_x,node_y,node_color,node_diam,node_border,
  node_font_size,node_demand,
  edge_name,edge_color,edge_width,edge_hi_width,edge_font_size,
  edge_length,edge_cost,edge_min_cap,edge_max_cap,edge_q_weight,edge_q_orig,
  edge_weight,
  default_node_diam,default_node_border,default_edge_width,
  default_edge_hi_width,default_font_size,
  ndim,ma)
char *path; int *lpath,*directed,*n,**tail,**head;
char **name; int *lname;
char ***node_name; 
int **node_type,**node_x,**node_y,**node_color,**node_diam,**node_border;
int **node_font_size;
double **node_demand;
char ***edge_name;
int **edge_color,**edge_width,**edge_hi_width,**edge_font_size;
double **edge_length,**edge_cost,**edge_min_cap,**edge_max_cap;
double **edge_q_weight,**edge_q_orig,**edge_weight;
int *default_node_diam,*default_node_border,*default_edge_width;
int *default_edge_hi_width,*default_font_size;
int *ndim,*ma;
{
  /** #ifdef __MSC__
  return;
  #else  **/
  FILE *fg;
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
  DIR *dirp;
#endif
  char fname[2 * MAXNAM];
  char line[5 * MAXNAM];
  char strname[MAXNAM], head_name[MAXNAM], tail_name[MAXNAM];
  int isize,dsize;
  int i,s;
  ENTRY node,*found;
  char dir[1024];
  char *pname;
  char **lar;

  path[*lpath] = '\0';
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
  if ((dirp=opendir(path)) != NULL) {
    sprintf(description,"\"%s\" is a directory",path);
    cerro(description);
    closedir(dirp);
    return;
  }
#endif
  if (dirname(path) == NULL) getwd(dir);
  else strcpy(dir,dirname(path));
#if !(defined __MSC__) && !(defined __ABSC__) &&! (defined __MINGW32__)
  if ((dirp=opendir(dir)) == NULL) {
    sprintf(description,"Directory \"%s\" does not exist",dir);
    cerro(description);
    return;
  }
  closedir(dirp);
#endif
  pname = StripGraph(my_basename(path));

  *lname = strlen(pname);

  if ((*name = (char *)malloc((unsigned)sizeof(char)*(*lname + 1)))
      == NULL) {
    cerro("Running out of memory");
    return;
  }
  strcpy(*name,pname);
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
  if(!CheckGraphName(*name,dir)) {
    sprintf(description,"Graph file \"%s/%s.graph\" does not exist",dir,*name);
    cerro(description);
    return;
  }
#endif  
  isize = sizeof(int);
  dsize = sizeof(double);

  strcpy(fname,dir);
  strcat(fname,"/");
  strcat(fname,*name);
  strcat(fname,".graph");
  fg = fopen(fname,"r");
  if (fg == 0) {
    sprintf(description,"Unable to open file \"%s/%s.graph\"",dir,*name);
    cerro(description);
    return;
  }
  /* read graph from graph file */
  
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  *default_node_diam = NODEDIAM;
  *default_node_border = NODEW;
  *default_edge_width = ARCW;
  *default_edge_hi_width = ARCH;
  *default_font_size = DRAWFONTSIZE;
  sscanf(line,"%d %d %d %d %d %d",directed,default_node_diam,
	 default_node_border,default_edge_width,
	 default_edge_hi_width,default_font_size);
  if (*default_node_diam == 0) *default_node_diam = NODEDIAM;
  if (*default_node_border == 0) *default_node_border = NODEW;
  if (*default_edge_width == 0) *default_edge_width = ARCW;
  if (*default_edge_hi_width == 0) *default_edge_hi_width = ARCH;
  if (*default_font_size == 0) *default_font_size = DRAWFONTSIZE;
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  sscanf(line,"%d",ma);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  sscanf(line,"%d",ndim);
  *n = *ndim;
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  
  /* alloc memory for nodes */
  s = sizeof(char *) * *ndim;
  if ((*node_name = (char **)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  s = isize * *ndim;
  if ((*node_type = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
    if ((*node_x = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*node_y = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*node_color = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*node_diam = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*node_border = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*node_font_size = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  s = dsize * *ndim;
  if ((*node_demand = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  /* alloc memory for edges */
  s = sizeof(char *) * *ma;
  if ((*edge_name = (char **)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  s = isize * *ma;
  if ((*head = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*tail = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  } 
  if ((*edge_color = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_width = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_hi_width = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_font_size = (int *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  s = dsize * *ma;
  if ((*edge_length = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_cost = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_min_cap = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_max_cap = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_q_weight = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_q_orig = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if ((*edge_weight = (double *)malloc(s)) == NULL) {
    cerro("Running out of memory");
    return;
  }

  /* jump to node description */
  for (i = 0; i < 2 * *ma; i++)
    fgets(line,5 * MAXNAM,fg);

  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);

  myhcreate(*ndim);
  for (i = 0; i < *ndim; i++) {
    fgets(line,5 * MAXNAM,fg);
    (*node_type)[i] = 0;
    sscanf(line,"%s %d",strname,&((*node_type)[i]));
    if (((*node_name)[i] = (char *)malloc(strlen(strname)+1)) == NULL) {
      cerro("Running out of memory");
      return;
    }
    strcpy((*node_name)[i],strname);
    fgets(line,5 * MAXNAM,fg);
    (*node_x)[i] = 0;
    (*node_y)[i] = 0;
    (*node_color)[i] = 0;
    (*node_diam)[i] = 0;
    (*node_border)[i] = 0;
    (*node_font_size)[i] = 0;
    sscanf(line,"%d %d %d %d %d %d",
	   &((*node_x)[i]),&((*node_y)[i]),&((*node_color)[i]),
	   &((*node_diam)[i]),&((*node_border)[i]),&((*node_font_size)[i]));
    fgets(line,5 * MAXNAM,fg);
    sscanf(line,"%le",&((*node_demand)[i]));
    if ((node.key = (char *)malloc(strlen(strname)+1)) == NULL) {
      cerro("Running out of memory");
      return;
    }
    strcpy(node.key,strname);
    sprintf(strname,"%d",i+1);
    if ((node.data = (char *)malloc(strlen(strname)+1)) == NULL) {
      cerro("Running out of memory");
      return;
    }   
    strcpy(node.data,strname);
    myhsearch(node,ENTER);
  }

  /* check uniqueness of node names */
  if (*ndim != 1) {
    if ((lar = (char **)malloc(sizeof(char *) * *ndim)) == NULL) {
      cerro("Running out of memory");
      return;
    }
    for (i = 0; i < *ndim; i++) lar[i] = (*node_name)[i];
    qsort((char*)lar,*ndim,sizeof(char*),(PF)CompString);
    for (i = 0; i < *ndim - 1; i++) {
      if (!strcmp(lar[i],lar[i+1])) {
	sprintf(description,
		"Bad graph file. Node \"%s\" is duplicated",lar[i]);
	cerro(description);
	free(lar);
	return;
      }
    }
    if (!strcmp(lar[*ndim - 2],lar[*ndim - 1])) {
      sprintf(description,
	      "Bad graph file. Node \"%s\" is duplicated",lar[*ndim - 2]);
      cerro(description);
      free(lar);
      return;
    }
    free(lar);
  }
 
   /* rewind and go to arc description */
  rewind(fg);
  for (i = 0; i < 11; i++)
    fgets(line,5 * MAXNAM,fg);
  for (i = 0; i < *ma; i++) {
  fgets(line,5 * MAXNAM,fg);
  (*edge_color)[i] = 0;
  (*edge_width)[i] = 0;
  (*edge_hi_width)[i] = 0;
  (*edge_font_size)[i] = 0;
  sscanf(line,"%s %s %s %d %d %d %d\n",strname,
	 tail_name,head_name,
	 &((*edge_color)[i]),&((*edge_width)[i]),&((*edge_hi_width)[i]),
	 &((*edge_font_size)[i]));
    if (((*edge_name)[i] = (char *)malloc(strlen(strname)+1)) == NULL) {
      cerro("Running out of memory");
      return;
    }
  strcpy((*edge_name)[i],strname);

  if ((node.key = (char *)malloc(strlen(head_name)+1)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  strcpy(node.key,head_name);
  found = myhsearch(node,FIND);
  if (found == NULL) {
    sprintf(description,
	    "Bad graph file. Node \"%s\" referenced by arc \"%s\" not found",
	    head_name,(*edge_name)[i]);
    cerro(description);
    return;
  }
  (*head)[i] = atoi(found->data);
  if ((node.key = (char *)malloc(strlen(tail_name)+1)) == NULL) {
    cerro("Running out of memory");
    return;
  }
  strcpy(node.key,tail_name);
  found = myhsearch(node,FIND);
  if (found == NULL) {
    sprintf(description,
	    "Bad graph file. Node \"%s\" referenced by arc \"%s\" not found",
	    tail_name,(*edge_name)[i]);
    cerro(description);
    return;
  }
  (*tail)[i] = atoi(found->data);
 
  fgets(line,5 * MAXNAM,fg);
  sscanf(line,"%le %le %le %le %le %le %le",
	 &((*edge_cost)[i]),&((*edge_min_cap)[i]),
	 &((*edge_max_cap)[i]),&((*edge_length)[i]),
	 &((*edge_q_weight)[i]),&((*edge_q_orig)[i]),
	 &((*edge_weight)[i]));
  }
  myhdestroy();
  fclose(fg);
  /** #endif**/ /**  __MSC__ **/
}
