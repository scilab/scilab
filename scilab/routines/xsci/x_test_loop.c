/* Copyright (C) 1998 Chancelier Jean-Philippe */
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "../machine.h"
#include "../menusX/men_scilab.h" 
#include "All-extern-x1.h" 
#include "All-extern.h" 
     
extern int main  __PARAMS((int argc, char **argv));  
extern int C2F (dsort) __PARAMS((void));  
extern void test_plot  __PARAMS((void));  
extern void test_apropos  __PARAMS((void));  
extern void test_menu  __PARAMS((void));  
extern void test_quit  __PARAMS((void));  
extern void test_loop  __PARAMS((void));  
extern void test_message  __PARAMS((void));  
extern void test_click  __PARAMS((void));  
extern void test_events  __PARAMS((void));  
extern void test_xinfo  __PARAMS((void));  
extern void test_xgc  __PARAMS((void));  
extern void LTest  __PARAMS((char *x0));  
extern void C2F (sigbas) __PARAMS((int *i));  
extern void check_win  __PARAMS((void));  
extern void C2F (setfbutn) __PARAMS((char *name, int *rep));  
extern int C2F (scilines) __PARAMS((int *nl, int *nc));  
extern int C2F (sciquit) __PARAMS((void));  
extern void plot  __PARAMS((void));  
extern void cerro  __PARAMS((char *str));  
extern void cout  __PARAMS((char *str));  
extern void C2F (cvstr) __PARAMS((int *n, int *line, char *str, int *job, long int lstr));  

#define PI0 (integer *) 0
#define PD0 (double *)  0

extern int demo_menu_activate;

#include <string.h> /* in case of dmalloc */ 
#include <malloc.h>


int main(argc, argv)
     int argc;
     char **argv;
{
  int i,nowindow=0,nostartup=0;
  demo_menu_activate=1;
  for (i=argc-1 ; i >=0  ; i-- )
    {
      if ( strcmp(argv[i],"-ns")==0) nostartup=1;
      if ( strcmp(argv[i],"-nw")==0) nowindow=1;
    };
  if ( nowindow) 
    C2F(scilab)(&nostartup);
  else
    main_sci(argc,argv);
  return(0);
};

void C2F(fbutn)() {};
void C2F(sciwin)() {};

#define PROMPT "[loop test]-->"

typedef  struct  {
  char *name;
  void (*fonc)(); } TestOpTab ;

static void vide_() {}
     
static char buf[1000];


void test_plot()
{
  integer win=0;
  check_win();
  plot();
  C2F(xsaveplots)(&win,"pipo.sav",0L);
  C2F(xloadplots)("pipo.sav",0L);
}

jmp_buf env;

void inter(int an_int)
{
  fprintf(stderr,"Signal Reached");
  longjmp(env, 1);		 /* return to prompt  */
}


void test_interupt()
{
  int i=0;
  (void) signal(SIGINT, inter);
  (void) signal(SIGKILL, inter);
  if (!setjmp(env)) 
    {
      /* first time */
    } 
  else 
    {	
      fprintf(stderr,"OOOOOps");
      return;
    }
  while (1) { printf("%d\r\n",i++);};
}


void test_menu()
{
  integer win_num=0,ne=3,ierr=0,typ=0;
  static char * entries[]={
    "Un ","Deux","Trois",NULL};
  AddMenu(&win_num,"test button",entries,&ne,&typ,"poo",&ierr);
}

void test_quit() {
 sci_clear_and_exit(0);
};

void test_loop() {
  while (1) {
    C2F(sxevents)();
  };
} 

void test_message() 
{
  TestMatrixDialogWindow();
  TestChoose();
  TestMessage(1);
  TestDialog() ;
  TestmDialogWindow();
  TestChoice();
}

void test_click() {
  integer i,iw=0,istr=0;
  double x,y;
  check_win();
  C2F(dr1)("xclick","void",&i,&iw,&istr,PI0,PI0, PI0,&x,&y,PD0,PD0,0L,0L);
  sprintf(buf,"-->[%d,%f,%f]",i,x,y);
  Xputstring(buf,strlen(buf));
}

void test_events() 
{
  int i;
  for ( i=0 ; i < 1000; i++) 
    {
#ifdef sun
      unsigned usec=20;
#endif
      xevents1();
#ifdef sun
      usleep(usec);
#endif
    };
  Scistring("Quittting enevent loop");
};

void test_xinfo() 
{
  xinfo_("Xinfo Tester",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
};


void test_xgc() {
  integer num=0;
  /** need ../xgc dir to work **/
  /** xgc(); **/
  DeleteWindowToList(num);
}

void test_help() {
  Sci_Help("cos");
}

void test_apropos() {
  Sci_Apropos("cos");
}


extern int C2F(stimer)(void);

void test_timer(){
  sciprint("stimer %d\r\n",(int)C2F(stimer)());
} 



static TestOpTab testTab[] ={
  {"add menu",test_menu},
  {"apropos",test_apropos},
  {"click", test_click},
  {"events",test_events},
  {"help",test_help},
  {"interupt",test_interupt},
  {"loop",test_loop},
  {"menus",test_message},
  {"plot",test_plot},
  {"quit",test_quit},
  {"stimer",test_timer},
  {"xgc",test_xgc},
  {"xinfo",test_xinfo},
  {(char *) NULL,vide_}
  };

void LTest(x0) 
     char * x0;
{
  int i=0;
  if ( strcmp("quit",x0) == 0) exit(0);
  while ( testTab[i].name != (char *) NULL)
     {
       int j;
       j = strcmp(x0,testTab[i].name);
       if ( j == 0 ) 
	 { 
	   (*(testTab[i].fonc))();
	   return;}
       else 
	 { 
	   if ( j <= 0)
	     {
	       sciprint("\nUnknow X operator <%s>\r\n",x0);
	       break;
	     }
	   else i++;
	 }
     }
  sciprint("\n Unknow X operator <%s>\r\n",x0);
  i=0;
  sciprint("%s","List of known operators \r\n");
  while ( testTab[i].name != (char *) NULL)
    {
      sciprint("\t%s\r\n",testTab[i].name);
      i++;
    }
}

int F2C(scilab)(nostartup)
     int *nostartup;
{
  int siz=1000,len_line,eof,i;
  C2F(xscion)(&i);
  if ( i == 1) 
    for ( ; ; ) {
      Xputstring(PROMPT,strlen(PROMPT));
      C2F(zzledt1)(buf,&siz,&len_line,&eof,0L);
      Xputstring(buf,len_line);
      Xputstring("\r\n",2);
      LTest(buf);
    }
  else 
    for ( ; ; ) {
      fprintf(stdout,PROMPT);
      C2F(zzledt)(buf,&siz,&len_line,&eof,0L);
      fprintf(stdout,buf);
      fprintf(stdout,"\r\n");
      LTest(buf) ;
    };
  return(0);
};


void C2F(sigbas)(i)
     int *i;
{
  fprintf(stderr,"CTRL_C activated \n");
};


void check_win()
{
  integer verb=0,win,na;
  C2F(dr1)("xget","window",&verb,&win,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
};

void C2F(setfbutn)(name,rep)
     char *name;
     int *rep;
{
  
}

int C2F(dsort)(){};

int C2F(scilines)(nl,nc)
     int *nl, *nc;
{return(0);};

int C2F(sciquit)()
{
  return(1);
}
     
#include <math.h>

#define XN2DD 2
#define NCURVES2DD  1

void plot()
{
  integer style[NCURVES2DD],aaint[4],n1,n2;
  double x[NCURVES2DD*XN2DD],y[NCURVES2DD*XN2DD],brect[4];
  int i,j;
  for ( j =0 ; j < NCURVES2DD ; j++)
    {
      i=0;
      x[i+ XN2DD*j]= ((double) i+1)/10.0;
      y[i+ XN2DD*j]= 1.234;
      i=1;
      x[i+ XN2DD*j]= ((double) i+2)/10.0;
      y[i+ XN2DD*j]= 2,64;
      }
  for ( i=0 ; i < NCURVES2DD ; i++)
    style[i]= -NCURVES2DD+i;
  n1=NCURVES2DD;n2=XN2DD;
  aaint[0]=aaint[2]=2;aaint[1]=aaint[3]=10;
  C2F(plot2d)(x,y,&n1,&n2,style,"021"," ",brect,aaint,0L,0L);
};


void cerro(str)
char *str;
{
  fprintf(stderr,"%s",str);
}

void cout(str)
char *str;
{
  fprintf(stdout,"%s",str);
}


void C2F(cvstr)(n,line,str,job,lstr)
     int *n,*line;
     char str[];
     int  *job;
     long int lstr;
{};

