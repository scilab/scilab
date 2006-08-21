/*------------------------------------------------------------------------*/
/* file: GetCommandArg.h                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : tools to retrieve parameters within the command line for        */
/*        graphic routines.                                               */
/*------------------------------------------------------------------------*/

#ifndef _GET_COMMAND_ARG_H_
#define _GET_COMMAND_ARG_H_

#include "stack-c.h"
#include "machine.h"

/*------------------------------------------------------------------------*/
int get_style_arg(char *fname,int pos, int n1,rhs_opts opts[], int ** style );
int get_rect_arg(char *fname,int pos,rhs_opts opts[], double ** rect );
int get_strf_arg(char *fname,int pos,rhs_opts opts[], char ** strf );
int get_legend_arg(char *fname,int pos,rhs_opts opts[], char ** legend );
int get_labels_arg(char *fname,int pos,rhs_opts opts[], char ** labels );
int get_nax_arg(int pos,rhs_opts opts[], int ** nax, BOOL * flagNax );
int get_zminmax_arg(char *fname,int pos,rhs_opts opts[], double ** zminmax );
int get_colminmax_arg(char *fname,int pos,rhs_opts opts[], int ** colminmax );
int get_colout_arg(char *fname,int pos,rhs_opts opts[], int ** colout );
int get_with_mesh_arg(char *fname,int pos,rhs_opts opts[], BOOL * withMesh);
int get_logflags_arg(char *fname,int pos,rhs_opts opts[], char ** logFlags );
int get_optional_double_arg( char * fname, int pos, char * name, double ** value, int sz, rhs_opts opts[] ) ;
int get_optional_int_arg( char * fname, int pos, char * name, int ** value, int sz, rhs_opts opts[] ) ;
/*------------------------------------------------------------------------*/
#define sciGetStyle(fname,pos,n1,opts,style) { if ( get_style_arg(fname,pos,n1,opts,style) == 0) { return 0 ; } }
#define GetRect(fname,pos,opts,rect) { if ( get_rect_arg(fname,pos,opts,rect) == 0) { return 0 ; } }
#define GetStrf(fname,pos,opts,strf) { if ( get_strf_arg(fname,pos,opts,strf) == 0) { return 0 ; } }
#define GetLegend(fname,pos,opts,legend) { if ( get_legend_arg(fname,pos,opts,legend) == 0) { return 0 ; } }
#define GetLabels(fname,pos,opts,labels) { if ( get_labels_arg(fname,pos,opts,labels) == 0) { return 0 ; } }
#define GetNax(pos,opts,nax,flagNax) { if ( get_nax_arg(pos,opts,nax,flagNax)==0 ) { return 0 ; } }
#define GetZminmax(fname,pos,opts,zminmax) { if ( get_zminmax_arg(fname,pos,opts,zminmax) == 0) { return 0 ; } }
#define GetColminmax(fname,pos,opts,colminmax) { if ( get_colminmax_arg(fname,pos,opts,colminmax)==0 ) { return 0 ; } }
#define GetColOut(fname,pos,opts,colout) { if ( get_colout_arg(fname,pos,opts,colout)==0 ) { return 0 ; } }
#define GetWithMesh(fname,pos,opts,withMesh) { if ( get_with_mesh_arg(fname,pos,opts,withMesh)==0 ) { return 0 ; } }
#define GetLogflags(fname,pos,opts,logFlags) { if ( get_logflags_arg(fname,pos,opts,logFlags) == 0) { return 0 ; } }
#define GetOptionalDoubleArg(fname,pos,name,value,sz,opts) { if ( get_optional_double_arg(fname,pos,name,value,sz,opts) == 0) { return 0 ; } }
#define GetOptionalIntArg(fname,pos,name,value,sz,opts) { if ( get_optional_int_arg(fname,pos,name,value,sz,opts) == 0) { return 0 ; } }
/*------------------------------------------------------------------------*/

#endif /* _GET_COMMAND_ARG_H_ */