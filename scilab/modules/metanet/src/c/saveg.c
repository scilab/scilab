
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "saveg.h"
#include "localization.h"
#include "machine.h"
static int CompString(char **s1, char **s2)
{
  return strcmp((char*)*s1,(char*)*s2);
}

void C2F(saveg) (char *path, int *lpath,
		 char *name, int *lname,int *directed,int *node_number,int *tail,int *head,
		 char ***node_name,
		 int *node_type,int *node_x,int *node_y,int *node_color,int *node_diam,int *node_border,
		 int *node_font_size,
		 double *node_demand,
		 char ***edge_name,
		 int *edge_color,int *edge_width,int *edge_hi_width,int *edge_font_size,
		 double *edge_length,double *edge_cost,double *edge_min_cap,double *edge_max_cap,
		 double *edge_q_weight,double *edge_q_orig,double *edge_weight,
		 int *default_node_diam,int *default_node_border,int *default_edge_width,
		 int *default_edge_hi_width,int *default_font_size,
		 int *ma)
{
  FILE *f;
#if !(defined _MSC_VER)
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
    if ((lar = (char **)MALLOC(sizeof(char *) * *node_number)) == NULL) {
      cerro(_("Running out of memory"));
      return;
    }
    for (i = 0; i < *node_number; i++) lar[i] = (*node_name)[i];
    qsort((char*)lar,*node_number,sizeof(char*),(PF)CompString);
    for (i = 0; i < *node_number - 1; i++) {
      if (!strcmp(lar[i],lar[i+1])) {
	sprintf(description,
		_("Bad graph file. Node \"%s\" is duplicated"),lar[i]);
	cerro(description);
	FREE(lar);
	return;
      }
    }
    if (!strcmp(lar[*node_number - 2],lar[*node_number - 1])) {
      sprintf(description,
	      _("Bad graph file. Node \"%s\" is duplicated"),lar[*node_number - 2]);
      cerro(description);
      FREE(lar);
      return;
    }
    FREE(lar);
  }

  path[*lpath] = '\0';
  name[*lname] = '\0';

  if (!strcmp(path," ")) {
    getcwd(dir,(int)strlen(dir));
    strcpy(nname,name);
  }
  else {
#if (defined _MSC_VER)
    getcwd(dir,(int)strlen(dir));
#if (defined _MSC_VER)
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
      if (my_dirname(path) == NULL) getcwd(dir,(int)strlen(dir));
      else strcpy(dir,my_dirname(path));
    }
  }
#if !(defined _MSC_VER)
  if ((dirp=opendir(dir)) == NULL) {
    sprintf(description,_("Directory \"%s\" does not exist"),dir);
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
	    _("Unable to write file in directory %s, check access"),dir);
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
    fprintf(f,"%d %d %d %d %d %d\n",
	    node_x[i],node_y[i],node_color[i],node_diam[i],
	    node_border[i],node_font_size[i]);
    fprintf(f,"%e\n",node_demand[i]);
  }

  fclose(f);
}
