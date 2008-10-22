
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
 
#include "defs.h"
#include "hashtable_metanet.h"
#include "loadg.h"
#include "localization.h"

static int CompString(char **s1,char **s2)
{
  return strcmp((char*)*s1,(char*)*s2);
}

static char description[2*MAXNAM];

void C2F(loadg)(char *path, int *lpath, char **name, int *lname, int *directed, int *n, int **tail, int **head,
		char ***node_name,
		int **node_type, int **node_x, int **node_y, int **node_color, int **node_diam, int **node_border,
		int **node_font_size, double **node_demand,
		char ***edge_name, int **edge_color,int **edge_width, int **edge_hi_width, int **edge_font_size,
		double **edge_length, double **edge_cost, double **edge_min_cap, double **edge_max_cap,
		double **edge_q_weight, double **edge_q_orig, double **edge_weight,
		int *default_node_diam, int *default_node_border, int *default_edge_width,
		int *default_edge_hi_width, int *default_font_size,
		int *ndim, int *ma)
{
  FILE *fg;
#ifndef _MSC_VER
  DIR *dirp=NULL;
#endif
  char fname[2 * MAXNAM];
  char line[5 * MAXNAM];
  char strname[MAXNAM], head_name[MAXNAM], tail_name[MAXNAM];
  int isize,dsize;
  int i,s;
  ENTRY node;
  ENTRY *found=NULL;
  char dir[1024];
  char *pname=NULL;
  char **lar=NULL;

  path[*lpath] = '\0';
#ifndef _MSC_VER
  if ((dirp=opendir(path)) != NULL)
  {
    sprintf(description,_("\"%s\" is a directory"),path);
    cerro(description);
    closedir(dirp);
    return;
  }
#endif
  if (my_dirname(path) == NULL){ 
  getcwd(dir, (int) strlen(dir));}
  else {
    strcpy(dir,my_dirname(path));
  }
#ifndef _MSC_VER
  if ((dirp=opendir(dir)) == NULL)
  {
    sprintf(description,_("Directory \"%s\" does not exist"),dir);
    cerro(description);
    return;
  }
  closedir(dirp);
#endif
  pname = StripGraph(my_basename(path));
  *lname = (int)strlen(pname);

  if ((*name = (char *)MALLOC((unsigned)sizeof(char)*(*lname + 1))) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  strcpy(*name,pname);
  if (pname) { FREE(pname); pname=NULL;}
#ifndef _MSC_VER
  if(!CheckGraphName(*name,dir))
  {
    sprintf(description,_("Graph file \"%s/%s.graph\" does not exist"),dir,*name);
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
  if (fg == 0)
  {
    sprintf(description,_("Unable to open file \"%s/%s.graph\""),dir,*name);
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
  if ((*node_name = (char **)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  s = isize * *ndim;
  if ((*node_type = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_x = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_y = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_color = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_diam = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_border = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*node_font_size = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  s = dsize * *ndim;
  if ((*node_demand = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }

  /* alloc memory for edges */
  s = sizeof(char *) * *ma;
  if ((*edge_name = (char **)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  s = isize * *ma;
  if ((*head = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*tail = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_color = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_width = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_hi_width = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_font_size = (int *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  s = dsize * *ma;
  if ((*edge_length = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_cost = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_min_cap = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_max_cap = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_q_weight = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_q_orig = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }
  if ((*edge_weight = (double *)MALLOC(s)) == NULL)
  {
    cerro(_("Running out of memory"));
    return;
  }

  /* jump to node description */
  for (i = 0; i < 2 * *ma; i++) fgets(line,5 * MAXNAM,fg);

  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);
  fgets(line,5 * MAXNAM,fg);

  myhcreate(*ndim);
  for (i = 0; i < *ndim; i++)
  {
    fgets(line,5 * MAXNAM,fg);
    (*node_type)[i] = 0;
    sscanf(line,"%s %d",strname,&((*node_type)[i]));
    if (((*node_name)[i] = (char *)MALLOC(strlen(strname)+1)) == NULL)
	{
      cerro(_("Running out of memory"));
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

    if ((node.key = (char *)MALLOC(strlen(strname)+1)) == NULL)
	{
      cerro(_("Running out of memory"));
      return;
    }

    strcpy(node.key,strname);
    sprintf(strname,"%d",i+1);

    if ((node.data = (char *)MALLOC(strlen(strname)+1)) == NULL)
	{
      cerro(_("Running out of memory"));
      return;
    }
    strcpy(node.data,strname);
	myhsearch(node,SCIENTER);
	if (node.data) {FREE(node.data);node.data=NULL;}
	if (node.key) {FREE(node.key);node.key=NULL;}
  }

  /* check uniqueness of node names */
  if (*ndim != 1)
  {
    if ((lar = (char **)MALLOC(sizeof(char *) * *ndim)) == NULL)
	{
      cerro(_("Running out of memory"));
      return;
    }

    for (i = 0; i < *ndim; i++) lar[i] = (*node_name)[i];

    qsort((char*)lar,*ndim,sizeof(char*),(PF)CompString);

    for (i = 0; i < *ndim - 1; i++)
	{
      if (!strcmp(lar[i],lar[i+1]))
	  {
		sprintf(description,_("Bad graph file. Node \"%s\" is duplicated"),lar[i]);
		cerro(description);
		FREE(lar);
		lar=NULL;
		return;
      }
    }

    if (!strcmp(lar[*ndim - 2],lar[*ndim - 1]))
	{
      sprintf(description,_("Bad graph file. Node \"%s\" is duplicated"),lar[*ndim - 2]);
      cerro(description);
	  if (lar) {FREE(lar);lar=NULL;}
      return;
    }
    if (lar) {FREE(lar);lar=NULL;}
  }

   /* rewind and go to arc description */
  rewind(fg);
  for (i = 0; i < 11; i++) fgets(line,5 * MAXNAM,fg);

  for (i = 0; i < *ma; i++)
  {
	fgets(line,5 * MAXNAM,fg);
	(*edge_color)[i] = 0;
	(*edge_width)[i] = 0;
	(*edge_hi_width)[i] = 0;
	(*edge_font_size)[i] = 0;
	sscanf(line,"%s %s %s %d %d %d %d\n",strname,
		tail_name,head_name,
		&((*edge_color)[i]),&((*edge_width)[i]),&((*edge_hi_width)[i]),
		&((*edge_font_size)[i]));

	if (((*edge_name)[i] = (char *)MALLOC(strlen(strname)+1)) == NULL)
	{
		cerro(_("Running out of memory"));
		return;
    }
	strcpy((*edge_name)[i],strname);

	if ((node.key = (char *)MALLOC(strlen(head_name)+1)) == NULL)
	{
		cerro(_("Running out of memory"));
		return;
	}

	strcpy(node.key,head_name);
	found = myhsearch(node,SCIFIND);
	if (found == NULL)
	{
		sprintf(description,_("Bad graph file. Node \"%s\" referenced by arc \"%s\" not found"),head_name,(*edge_name)[i]);
		cerro(description);
		return;
	}
	(*head)[i] = atoi(found->data);

	if(found)
	{
		if (found->data) {FREE(found->data);found->data=NULL;}
		if (found->key) {FREE(found->key);found->key=NULL;}
		FREE(found);found=NULL;
	}


	if ((node.key = (char *)MALLOC(strlen(tail_name)+1)) == NULL)
	{
		cerro(_("Running out of memory"));
		return;
	}

	strcpy(node.key,tail_name);
	found = myhsearch(node,SCIFIND);
	if (found == NULL)
	{
		sprintf(description,
	    _("Bad graph file. Node \"%s\" referenced by arc \"%s\" not found"),
	    tail_name,(*edge_name)[i]);
		cerro(description);
		return;
	}
	(*tail)[i] = atoi(found->data);
	if(found)
	{
		if (found->data) {FREE(found->data);found->data=NULL;}
		if (found->key) {FREE(found->key);found->key=NULL;}
		FREE(found);found=NULL;
	}

	fgets(line,5 * MAXNAM,fg);
	sscanf(line,"%le %le %le %le %le %le %le",
		&((*edge_cost)[i]),&((*edge_min_cap)[i]),
		&((*edge_max_cap)[i]),&((*edge_length)[i]),
		&((*edge_q_weight)[i]),&((*edge_q_orig)[i]),
		&((*edge_weight)[i]));
	}

	myhdestroy();
	fclose(fg);
	if (node.key) {FREE(node.key);node.key=NULL;}
	if (node.data) {FREE(node.data);node.data=NULL;}
}
