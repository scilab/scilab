/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/

#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

#ifndef NULL
#define NULL 0
#endif 
typedef void (*voidg)();

typedef struct {   
	int nevprt;	   
	voidg funpt ;	   
	int type;	   
	int scsptr;	   
	int nz;	   
	double *z;	   
	int noz;	   
	int *ozsz;	   
	int *oztyp;	   
	void **ozptr;	   
	int nx;	   
	double *x;	   
	double *xd;	   
	double *res;	   
	int *xprop;	   
	int nin;	   
	int *insz;	   
	double **inptr;  
	int nout;	   
	int *outsz;	   
	double **outptr; 
	int nevout;	   
	double *evout;   
	int nrpar;	   
	double *rpar;	   
	int nipar;	   
	int *ipar;	   
	int nopar;	   
	int *oparsz;	   
	int *opartyp;	   
	void **oparptr;  
	int ng;	   
	double *g;	   
	int ztyp;	   
	int *jroot;	   
	char *label;	   
	void **work;	   
	int nmode;	   
	int *mode;	   
} scicos_block;    


void do_cold_restart();
int get_phase_simulation();
double get_scicos_time();
int get_block_number();
void set_block_error(int);
void set_pointer_xproperty(int* pointer);
// double Get_Jacobian_cj(void);
// double Get_Jacobian_ci(void);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

int Convert_number (char *, double *);
void homotopy(double *);
int hfjac_(double *, double *, int *);
int rhojac_(double *, double *,double *,double *, int *, double *,int *);
int rho_( double *, double *,double *,double *,double *, int *);
int fx_( double *, double *);
int read_xml_initial_states(int nvar,const char * xmlfile, char **ids, double *svars);
int write_xml_states(int,const char *, char **, double *);

#if _MSC_VER
#ifdef min
#undef min
#endif 
#ifdef max 
#undef max
#endif 
#endif 

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif 

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif


#endif /* __SCICOS_BLOCK_H__ */
