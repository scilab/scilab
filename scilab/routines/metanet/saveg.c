#include <stdio.h>
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__)
#if defined(netbsd) || defined(freebsd) 
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

#ifdef WIN32
#include <stdlib.h> /*qsort*/
#include <direct.h> /*_getcwd _chdir*/
#endif
#if (defined __MSC__) || (defined __ABSC__) || (defined __MINGW32__) 
/** only used for x=dir[1024] **/
#ifndef __ABSC__
#include <io.h>
#define  getwd(x) _getcwd(x,1024) 
#define chdir(x) _chdir(x)
#else
#define  getwd(x) getcwd_(x,1024) 
#endif
#endif

#if defined(netbsd) || defined(freebsd)
#include <unistd.h>
#endif



#include "../machine.h"

#define MAXNAM 80

extern double atof();
extern char* my_basename();
extern void cerro();
extern char* dirname();
extern int CheckGraphName();
extern char *StripGraph();

typedef int (*PF)();

static int CompString(s1, s2)
char **s1, **s2;
{
  return strcmp((char*)*s1,(char*)*s2);
}

void C2F(saveg)(path,lpath,name,lname,directed,node_number,tail,head,
  node_name,node_type,node_x,node_y,node_color,node_diam,node_border,
  node_font_size,node_demand,
  edge_name,edge_color,edge_width,edge_hi_width,edge_font_size,
  edge_length,edge_cost,edge_min_cap,edge_max_cap,edge_q_weight,edge_q_orig,
  edge_weight,
  default_node_diam,default_node_border,default_edge_width,
  default_edge_hi_width,default_font_size,ma)
char *path; int *lpath;
char *name; int *lname,*directed,*node_number,*tail,*head;
char ***node_name; 
int *node_type,*node_x,*node_y,*node_color,*node_diam,*node_border;
int *node_font_size;
double *node_demand;
char ***edge_name;
int *edge_color,*edge_width,*edge_hi_width,*edge_font_size;
double *edge_length,*edge_cost,*edge_min_cap,*edge_max_cap;
double *edge_q_weight,*edge_q_orig,*edge_weight;
int *default_node_diam,*default_node_border,*default_edge_width;
int *default_edge_hi_width,*default_font_size;
int *ma;
{
  FILE *f;
#if !(defined __MSC__) && !(defined __ABSC__) &&! (defined __MINGW32__)
  DIR *dirp;
#else
  int it;
#endif
  char fname[2*MAXNAM];
  char description[2*MAXNAM];
  char dir[1024], nname[2*MAXNAM];
  int i;
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
 
  path[*lpath] = '\0';
  name[*lname] = '\0';
  
  if (!strcmp(path," ")) {
    getwd(dir);
    strcpy(nname,name);
  }
  else {
#if (defined __MSC__) || (defined __ABSC__) || (defined __MINGW32__)
    getwd(dir);
#if (defined __MSC__)   || (defined __MINGW32__)
    it= chdir(path);
    chdir(dir);
#else
    it= chdir_(path,strlen(path));
    chdir_(dir,strlen(dir));
#endif
    if (it == 0) {
      strcpy(dir,path);
    }
#else
    if ((dirp=opendir(path)) != NULL) {
      strcpy(dir,path);
      closedir(dirp);
    }
#endif 
    else {
      strcpy(nname,StripGraph(my_basename(path)));
      if (dirname(path) == NULL) getwd(dir);
      else strcpy(dir,dirname(path));     
    }
  }
#if !(defined __MSC__) && !(defined __ABSC__) && ! (defined __MINGW32__)
  if ((dirp=opendir(dir)) == NULL) {
    sprintf(description,"Directory \"%s\" does not exist",dir);
    cerro(description);
    return;
  }
  closedir(dirp);

  /*  if(CheckGraphName(nname,dir)) {
    sprintf(description,"Graph \"%s\" already exists in directory \"%s\"",
	    nname,dir);
    cerro(description);
    return;
    }*/
#endif

  strcpy(fname,dir);
  strcat(fname,"/");
  strcat(fname,nname);
  strcat(fname,".graph");
  f = fopen(fname,"w");
  if (f == NULL) {
    sprintf(description,
	    "Unable to write file in directory %s, check access",dir);
    cerro(description);
    return;
  }
  /* Write graph to file */
  
  fprintf(f,"GRAPH TYPE (0 = UNDIRECTED, 1 = DIRECTED), DEFAULTS (NODE DIAMETER, NODE BORDER, ARC WIDTH, HILITED ARC WIDTH, FONTSIZE):\n");
  fprintf(f,"%d %d %d %d %d %d\n",*directed,*default_node_diam,
	  *default_node_border,*default_edge_width,*default_edge_hi_width,
	  *default_font_size);
  if (*directed) {
    fprintf(f,"NUMBER OF ARCS:\n");
  }
  else {
    fprintf(f,"NUMBER OF EDGES:\n");
  }
  fprintf(f,"%d\n",*ma);
  fprintf(f,"NUMBER OF NODES:\n");
  fprintf(f,"%d\n",*node_number);
  fprintf(f,"****************************************\n");

  /* Write arcs to files */

  if (*directed) {
    fprintf(f,"DESCRIPTION OF ARCS:\n");
    fprintf(f,"ARC NAME, TAIL NODE NAME, HEAD NODE NAME, COLOR, WIDTH, HIWIDTH, FONTSIZE\n");
  }
  else {
    fprintf(f,"DESCRIPTION OF EDGES:\n");
    fprintf(f,"EDGE NAME, NODE NAME, NODE NAME, COLOR, WIDTH, HIWIDTH, FONTSIZE\n");
  }
  fprintf
    (f,"COST, MIN CAP, MAX CAP, LENGTH, Q WEIGHT, Q ORIGIN, WEIGHT\n");
  fprintf(f,"\n");

  for (i = 0; i < *ma; i++) {
    fprintf(f,"%s %s %s %d %d %d %d\n",(*edge_name)[i],(*node_name)[tail[i]-1],
	    (*node_name)[head[i]-1],edge_color[i],edge_width[i],
	    edge_hi_width[i],edge_font_size[i]);
    fprintf(f,"%e %e %e %e %e %e %e\n",
	    edge_cost[i],edge_min_cap[i],edge_max_cap[i],edge_length[i],
	    edge_q_weight[i],edge_q_orig[i],edge_weight[i]);
  }
  
  /* Write nodes to files */

  fprintf(f,"****************************************\n");
  fprintf(f,"DESCRIPTION OF NODES:\n");
  fprintf(f,"NODE NAME, POSSIBLE TYPE (1 = SINK, 2 = SOURCE)\n");
  fprintf(f,"X, Y, COLOR, DIAMETER, BORDER, FONTSIZE\n");
  fprintf(f,"DEMAND\n");
  fprintf(f,"\n");

  for (i = 0; i < *node_number; i++) {
    if (node_type[i] == 0)
      fprintf(f,"%s\n",(*node_name)[i]);
    else fprintf(f,"%s %d\n",(*node_name)[i],node_type[i]);
    fprintf(f,"%d %d %d %d %d %d \n",
	    node_x[i],node_y[i],node_color[i],node_diam[i],
	    node_border[i],node_font_size[i]);
    fprintf(f,"%e\n",node_demand[i]);
  }

  fclose(f);
}
