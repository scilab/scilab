/*------------------------------------------------------------------------
 *    PVM functions interfaces
 *    Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved 
 *
 *    file generated with intersci-n from intpvm.desc 
 *    then some routines where added or removed 
 *    Jean-Philippe Chancelier July 2002 
 *    WARNING: The primitive in this file works 
 *             with variables transmited as references 
 *             Thus we must take care not to change a transmited 
 *             argument. 
 *    pvm_sci2f77 and pvm_f772sci do not respect this convention 
 *-----------------------------------------------------------------------*/

#include <string.h> 
#include "pvm3.h"
#include "../mex.h"
#include "../stack-c.h"
#include "sci_pvm.h"

/******************************************
 * SCILAB function : pvm_error_mode
 ******************************************/

static int pvm_error = 0;

int intspvm_error_mode(fname) 
     char *fname;
{ 
  char *loc;
  static char stop[]="stop"; 
  static char cont[]="continue"; 
  int m1,n1,l1,un=1; 
  CheckRhs(0,1);
  CheckLhs(1,1);
  if ( Rhs == 1 ) 
    { 
      GetRhsVar(1,"c",&m1,&n1,&l1);
      if ( strcmp(cstk(l1),stop) == 0) 
	pvm_error = 1; 
      else if ( strcmp(cstk(l1),cont) == 0)
	pvm_error = 0; 
      else {
	Scierror(999,"%s: first argument should be \"stop\" or \"continue\"\r\n",fname);
	return 0;
      }
      LhsVar(1)=0;
    }
  else 
    {
      int m;
      if ( pvm_error == 1 ) { 
	m = strlen(stop);
	CreateVarFromPtr(1,"c",&m,&un,(loc=stop,&loc));
      } else { 
	m = strlen(cont);
	CreateVarFromPtr(1,"c",&m,&un,(loc=cont,&loc));
      }
      LhsVar(1)=1;
    }
  return 0;
}

static void error_check(char *fname,int err) {
  if ( pvm_error == 1 && err != 0 ) 
    {
      Scierror(999,"%s: %s\r\n",fname,
	       scipvm_error_msg(err));
    }
}

/******************************************
 * SCILAB function : pvm_joingroup, fin = 1
 ******************************************/

int intspvm_joingroup(fname)
     char *fname;
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: inum */
  *istk(l2) = pvm_joingroup(cstk(l1));
  LhsVar(1)= 2;
  error_check(fname,*istk(l2));
  return 0;
}

/******************************************
 * SCILAB function : pvm_lvgroup, fin = 2
 ******************************************/

int intspvm_lvgroup(fname)
     char *fname;
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: size */
  *istk(l2) = pvm_lvgroup(cstk(l1));
  LhsVar(1)= 2;
  error_check(fname,*istk(l2));
  return 0;
}


/******************************************
 * SCILAB function : pvm_gsize, fin = 3
 ******************************************/

int intspvm_gsize(fname)
     char *fname;
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: size */
  *istk(l2) = pvm_gsize(cstk(l1));
  LhsVar(1)= 2;
  error_check(fname,*istk(l2));
  return 0;
}

/******************************************
 * SCILAB function : pvm_gettid, fin = 4
 ******************************************/

int intspvm_gettid(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable inum */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(3,"i",&un,&un,&l3);/* named: tid */
  *istk(l3) = pvm_gettid(cstk(l1),*istk(l2));
  LhsVar(1)= 3;
  return 0;
}
/******************************************
 * SCILAB function : pvm_getinst, fin = 5
 ******************************************/

int intspvm_getinst(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable tid */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(3,"i",&un,&un,&l3);/* named: inum */
  *istk(l3) = pvm_getinst(cstk(l1),*istk(l2));
  LhsVar(1)= 3;
  return 0;
}
/******************************************
 * SCILAB function : pvm_barrier, fin = 6
 ******************************************/

int intspvm_barrier(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable count */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(3,"i",&un,&un,&l3);/* named: res */
  *istk(l3) = pvm_barrier(cstk(l1),*istk(l2));
  LhsVar(1)= 3;
  return 0;
}
/******************************************
 * SCILAB function : pvm_bcast, fin = 7
 ******************************************/

int intspvm_bcast(fname)
     char *fname;
{
  int m1,n1,l1,m3,n3,l3,un=1,l4,size,l5,used,ierr;
  int *Ipack,address,*header;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable buff */
  header = GetData(2);
  /*  checking variable msgtag */
  GetRhsVar(3,"i",&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(4,"i",&un,&un,&l4);/* named: res */
  /* Use the rest of the stack as working area */ 
  CreateWork(5,&size,&l5); 
  Ipack= (int *) stk(l5);
  /* Build a pack vector for buff: size is the max size 
   * allowed for packing 
   */ 
  address = Top-Rhs+2;
  C2F(varpak)(&address,Ipack,&used,&size,&ierr); 
  switch (ierr ) { 
  case 1: Scierror(999,"%s: work space (stacksize) is too small d\r\n",fname);
    return 0;
  case 2: Scierror(999,"%s: Unknow type or not yet implemented\r\n",fname);
    return 0; 
  }
  C2F(scipvmbcast)(cstk(l1),&m1,Ipack,&used,(double *)header,istk(l3),istk(l4));
  LhsVar(1)= 4;
  error_check(fname,*istk(l4));
  return 0;
}

/******************************************
 * SCILAB function : pvm_tasks, fin = 8
 ******************************************/

int intspvm_tasks(fname)
     char *fname;
{
  int un=1,m1,n1,l1,where=0; 
  char  **pname;
  int ntask, *ptid,*pptid,*pdtid,*pflag,info,ne3,*work; 

  CheckRhs(0,1); 
  CheckLhs(1,1);
  if ( Rhs == 1) { 
    GetRhsVar(1,"i",&m1,&n1,&l1); 
    CheckScalar(1,m1,n1);
    where = *istk(l1);
  }
  
  C2F(scipvmtasks)(&where,&ntask,&ptid,&pptid,&pdtid,&pflag,&pname,&ne3,&info); 
  if (info != 0 ) 
    {
      Scierror(999,"%s: Error \r\n",fname);
      return 0;
    }
  /* Creation of output list of length 7*/
  Createlist(Rhs+1,7);
  CreateListVarFromPtr(Rhs+1,1,"i",&un,&ne3,&ptid);
  FREE(ptid);
  CreateListVarFromPtr(Rhs+1,2,"i",&un,&ne3,&pptid);
  FREE(pptid);
  CreateListVarFromPtr(Rhs+1,3,"i",&un,&ne3,&pdtid);
  FREE(pdtid);
  CreateListVarFromPtr(Rhs+1,4,"i",&un,&ne3,&pflag);
  FREE(pflag);
  CreateListVarFromPtr(Rhs+1,5,"S",&ne3,&un,pname);
  FreeRhsSVar(pname); 
  CreateListVarFromPtr(Rhs+1,6,"i",&un,&un,(work=&ntask,&work));
  CreateListVarFromPtr(Rhs+1,7,"i",&un,&un,(work=&info,&work));
  LhsVar(1)= Rhs+1;
  error_check(fname,info);
  return 0;
}
/******************************************
 * SCILAB function : pvm_config, fin = 9
 ******************************************/

int intspvm_config(fname)
     char *fname;
{
  int un=1;
  int nhost,narch,*pdtid,*pspeed,ne3,info,*work;
  char  **pname,**parch;
  CheckRhs(0,0);
  CheckLhs(1,1);
  C2F(scipvmconfig)(&nhost,&narch,&pdtid,&pname,&parch,&pspeed,&ne3,&info);
  /* Creation of output list of length 7*/
  if (info != 0 ) 
    {
      Scierror(999,"%s: Error \r\n",fname);
      return 0;
    }
  Createlist(1,7);
  CreateListVarFromPtr(1,1,"i",&un,&un,(work=&nhost,&work));
  CreateListVarFromPtr(1,2,"i",&un,&un,(work=&narch,&work));
  CreateListVarFromPtr(1,3,"i",&un,&ne3,&pdtid);
  FREE(pdtid);
  CreateListVarFromPtr(1,4,"S",&ne3,&un,pname);
  FreeRhsSVar(pname); 
  CreateListVarFromPtr(1,5,"S",&ne3,&un,parch);
  FreeRhsSVar(parch); 
  CreateListVarFromPtr(1,6,"i",&un,&ne3,&pspeed);
  FREE(pspeed);
  CreateListVarFromPtr(1,7,"i",&un,&un,(work=&info,&work));
  LhsVar(1)= 1;
  error_check(fname,info);
  return 0;
}

/******************************************
 * SCILAB function : pvm_addhosts, fin = 10
 ******************************************/

int intspvm_addhosts(fname)
     char *fname;
{
  int m1,n1,un=1,mn2,l2;
  char  **Str1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable hosts */
  GetRhsVar(1,"S",&m1,&n1,&Str1);
  CheckOneDim(1,1,m1,1);
  /* cross variable size checking */
  CreateVar(2,"i",(un=1,&un),(mn2=n1,&mn2),&l2);/* named: infos */
  pvm_addhosts(Str1,n1 ,istk(l2));
  LhsVar(1)= 2;
  return 0;
}

/******************************************
 * SCILAB function : pvm_delhosts, fin = 11
 ******************************************/

int intspvm_delhosts(fname)
     char *fname;
{
  int m1,n1,un=1,mn2,l2;
  char  **Str1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable hosts */
  GetRhsVar(1,"S",&m1,&n1,&Str1);
  CheckOneDim(1,1,m1,1);
  /* cross variable size checking */
  CreateVar(2,"i",(un=1,&un),(mn2=n1,&mn2),&l2);/* named: infos */
  pvm_delhosts(Str1, n1,istk(l2));
  LhsVar(1)= 2;
  return 0;
}

/******************************************
 * SCILAB function : pvm_parent, fin = 12
 ******************************************/

int intspvm_parent(fname)
     char *fname;
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  *istk(l1) = pvm_parent();
  LhsVar(1)= 1;
  return 0;
}
/******************************************
 * SCILAB function : pvm_tidtohost, fin = 13
 ******************************************/

int intspvm_tidtohost(fname)
     char *fname;
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: res */
  *istk(l2) = pvm_tidtohost(*istk(l1));
  LhsVar(1)= 2;
  return 0;
}
/******************************************
 * SCILAB function : pvm_set_timer, fin = 14
 ******************************************/

int intspvm_set_timer(fname)
     char *fname;
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  C2F(scipvmsettimer)(istk(l1));
  LhsVar(1)= 1;
  return 0;
}
/******************************************
 * SCILAB function : pvm_get_timer, fin = 15
 ******************************************/

int intspvm_get_timer(fname)
     char *fname;
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"d",&un,&un,&l1);/* named: res */
  C2F(scipvmgettimer)(stk(l1));
  LhsVar(1)= 1;
  return 0;
}
/******************************************
 * SCILAB function : pvm_mytid, fin = 16
 ******************************************/

int intspvm_mytid(fname)
     char *fname;
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  *istk(l1) = pvm_mytid();
  LhsVar(1)= 1;
  return 0;
}


/******************************************
 * SCILAB function : pvm_exit, fin = 17
 ******************************************/

int intspvm_exit(fname)
     char *fname;
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  *istk(l1) = pvm_exit();
  LhsVar(1)= 1;
  error_check(fname,*istk(l1));
  return 0;
}
/******************************************
 * SCILAB function : pvm_kill, fin = 18
 ******************************************/

int intspvm_kill(fname)
     char *fname;
{
  int m1,n1,l1,mn1,un=1,mn2,l2,i;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  CreateVar(2,"i",(un=1,&un),(mn2=n1,&mn2),&l2);/* named: res */
  for (i = 0; i < n1; ++i) {
    istk(l2)[i] = pvm_kill( istk(l1)[i]);
    error_check(fname,istk(l2)[i]);
  }
  LhsVar(1)= 2;
  return 0;
}

/******************************************
 * SCILAB function : pvm_spawn, fin = 19
 ******************************************/

int intspvm_spawn(fname)
     char *fname;
{
  char *nw, *where ; 
  static char def_nw[]=""; 
  static char def_where[]="null"; 
  int m1,n1,l1,mn1,m2,n2,l2,un=1,lwhere,lnw; 
  int m3,n3,l3,m4,n4,l4,tids,res;
  CheckRhs(2,4);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  mn1=m1*n1; 
  /*  checking variable ntask */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  nw = def_nw;
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,"c",&m3,&n3,&l3); 
    if ( m3*n3 != 0 && strcmp(cstk(l3),"nw")==0) 
      nw = cstk(l3); 
  }
  lnw= strlen(nw);
  where = def_where; 
  if ( Rhs >= 4 ) { 
    GetRhsVar(4,"c",&m4,&n4,&l4); 
    if ( m4*n4 != 0 ) 
      where = cstk(l4); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,"i",&un,istk(l2),&tids);/* named: tids */
  CreateVar(Rhs+2,"i",&un,&un,&res);/* named: res */
  C2F(scipvmspawn)(cstk(l1),&mn1,nw,&lnw,where,&lwhere,istk(l2),istk(tids),istk(res));
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  error_check(fname,*istk(res));
  return 0;
}

/******************************************
 * SCILAB function : pvm_spawn_independent, fin = 20
 ******************************************/

int intspvm_spawn_independent(fname)
     char *fname;
{
  int m1,n1,l1,mn1,m2,n2,l2,un=1,m3,n3,l3,lwhere,tids,res;
  static char def_where[]="null"; 
  char *where; 
  CheckRhs(2,3);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  mn1=m1*n1;
  /*  checking variable ntask */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  where = def_where; 
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,"c",&m3,&n3,&l3); 
    if ( m3*n3 != 0 ) 
      where = cstk(l3); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,"i",&un,istk(l2),&tids);/* named: tids */
  CreateVar(Rhs+2,"i",&un,&un,&res);/* named: res */
  C2F(scipvmspawnindependent)(cstk(l1),&mn1,istk(l2),where,&lwhere,istk(tids),istk(res));
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  error_check(fname,*istk(res));
  return 0;
}

/******************************************
 * SCILAB function : pvm_recv, fin = 21
 ******************************************/

int intspvm_recv(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,un=1,l3,used,size,l5,count=5,l,tid,tag;
  CheckRhs(2,2);
  CheckLhs(1,4);
  /*  checking variable tid */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  tid=*istk(l1);
  /*  checking variable tag */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  tag=*istk(l2);
  /* space for info */ 
  CreateVar(3,"i",&un,&un,&l3);/* named: res */
  /* space for paking info */ 
  CreateWork(4,&size,&l5); 
  /* variable at position 4 is created as a scalar matrix 
   * but we transmit stk(l5-2) to scipvmrecv which 
   * then can change the header of object at position 4 
   */ 
  C2F(scipvmrecv)(stk(l5-2),&size,&used,&tid,&tag,istk(l3));
  /* now we know the exact size used */
  SetWorkSize(4,&used);
  LhsVar(1)=4;
  LhsVar(2)=3;
  LhsVar(3)=1;
  LhsVar(4)=2;
  /* since 1 and 2 could be ref */ 
  if ( IsRef(1) ) { 
    CreateVar(count,"i",&un,&un,&l);
    *istk(l)=tid;
    LhsVar(3)=count;
    count++;
  }
  if ( IsRef(2) ) { 
    CreateVar(count,"i",&un,&un,&l);
    *istk(l)=tag;
    LhsVar(4)=count;
  }
  error_check(fname,*istk(l3));
  return 0;
}

/******************************************
 * SCILAB function : pvm_send, fin = 22
 ******************************************/

int intspvm_send(fname)
     char *fname;
{
  int *Ipack,*header,address;
  int m1,n1,l1,mn1,m3,n3,l3,un=1,l4,size,l5,used,ierr;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  /*  the data at position 2 will be packed by varpack */
  header = GetData(2);
  /*  checking variable tag */
  GetRhsVar(3,"i",&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(4,"i",&un,&un,&l4);/* named: res */
  CreateWork(5,&size,&l5); 
  Ipack= (int *) stk(l5);
  /* Build a pack vector for buff: size is the max size 
   * allowed for packing 
   */ 
  address = Top-Rhs+2;
  C2F(varpak)(&address,Ipack,&used,&size,&ierr); 
  switch (ierr ) { 
  case 1: Scierror(999,"%s: work space (stacksize) is too small d\r\n",fname);
    return 0;
  case 2: Scierror(999,"%s: Unknow type or not yet implemented\r\n",fname);
    return 0; 
  }

  C2F(scipvmsend)(istk(l1),&n1,Ipack,&used,(double *)header,istk(l3),istk(l4));
  LhsVar(1)= 4;
  error_check(fname,*istk(l4));
  return 0;
}

/******************************************
 * SCILAB function : pvm_recv_var, fin = 24
 ******************************************/

int intspvm_recv_var(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /*  checking variable tag */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /*  checking variable buff */
  GetRhsVar(3,"c",&m3,&n3,&l3);
  /* cross variable size checking */
  CreateVar(4,"i",&un,&un,&l4);/* named: res */
  C2F(scipvmrecvvar)(istk(l1),istk(l2),cstk(l3),istk(l4));
  LhsVar(1)= 4;
  error_check(fname,*istk(l4));
  return 0;
}
/******************************************
 * SCILAB function : pvm_send_var, fin = 25
 ******************************************/

int intspvm_send_var(fname)
     char *fname;
{
  int m1,n1,l1,mn1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  /*  checking variable buff */
  GetRhsVar(2,"c",&m2,&n2,&l2);
  /*  checking variable tag */
  GetRhsVar(3,"i",&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(4,"i",&un,&un,&l4);/* named: res */
  C2F(scipvmsendvar)(istk(l1),&n1,cstk(l2),istk(l3),istk(l4));
  LhsVar(1)= 4;
  error_check(fname,*istk(l4));
  return 0;
}

/******************************************
 * SCILAB function : pvm_reduce 
 ******************************************/

int intspvm_reduce(fname)
     char *fname;
{
  int m1,n1,l1,mn1,m2,n2,l2,m3,n3,l3,un=1,m4,n4,l4,mn4,res;
  int m5,n5,l5;
  CheckRhs(5,5);
  CheckLhs(1,2);
  /*  checking variable func */
  GetRhsVar(1,"c",&m1,&n1,&l1);mn1=m1*n1;
  /*  checking variable data */
  GetRhsVar(2,"d",&m2,&n2,&l2);
  /*  checking variable msgtag */
  GetRhsVar(3,"i",&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /*  checking variable group */
  GetRhsVar(4,"c",&m4,&n4,&l4);mn4=m4*n4;
  /*  checking variable rootginst */
  GetRhsVar(5,"i",&m5,&n5,&l5);
  CheckScalar(5,m5,n5);
  CreateVar(6,"i",&un,&un,&res);
  C2F(scipvmreduce)(cstk(l1),&mn1,stk(l2),&m2,&n2,istk(l3),cstk(l4),&mn4,istk(l5),istk(res));
  LhsVar(1)= 6;
  error_check(fname,*istk(res));
  return 0;
}

/******************************************
 * SCILAB function : pvm_start 
 ******************************************/

int intspvm_start(fname)
     char *fname;
{
  int m1,n1,l1,mn1,un=1,res;
  char *host; 
  static char hdefault[]="null";
  CheckRhs(0,1);
  CheckLhs(1,1);
  if (Rhs ==1 ) {
    GetRhsVar(1,"c",&m1,&n1,&l1);mn1=m1*n1;
    host = cstk(l1);
  } else {
    host = hdefault;
    mn1= strlen(hdefault);
  }
  CreateVar(Rhs+1,"i",&un,&un,&res);
  C2F(scipvmstart)(istk(res),host,&mn1);
  LhsVar(1)=Rhs+1;
  error_check(fname,*istk(res));
  return 0;
}

/******************************************
 * SCILAB function : pvm_halt 
 ******************************************/

int intspvm_halt(fname)
     char *fname;
{
  int res,un=1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVar(1,"i",&un,&un,&res);
  C2F(scipvmhalt)(istk(res));
  LhsVar(1)=1;
  error_check(fname,*istk(res));
  return 0;
}

/******************************************
 * SCILAB function : pvm_error
 ******************************************/

int intspvm_error(fname)
     char *fname;
{
  int m1,n1,l1,mres,nres=1;
  char *res; 
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable func */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  res = scipvm_error_msg(*istk(l1));
  CreateVarFromPtr(2, "c",(mres=strlen(res), &mres), &nres, &res);
  LhsVar(1)=2; 
  return 0;
}

/******************************************
 * SCILAB function : pvm_sci2f77
 ******************************************/

int intspvm_sci2f77(fname)
     char *fname;
{
  int *header;
  CheckRhs(1,1);
  CheckLhs(0,1);
  /*  checking variable func */
  header = GetData(1);
  C2F(scipvmscitof77)(&Top);
  if ( IsRef(1) ) 
    { 
      /* arg is a reference it is changed */
      LhsVar(1)=0;
    }
  else 
    {
      /* arg is not a reference we must return 
       * the changed argument 
       */
      LhsVar(1)=1;
    }
  return 0;
}

/******************************************
 * SCILAB function : pvm_f772sci
 ******************************************/

int intspvm_f772sci(fname)
     char *fname;
{
  int *header;
  CheckRhs(1,1);
  CheckLhs(1,1);
  header = GetData(1);
  C2F(scipvmf77tosci)(&Top);
  if ( IsRef(1) ) 
    { 
      /* arg is a reference it is changed */
      LhsVar(1)=0;
    }
  else 
    {
      /* arg is not a reference we must return 
       * the changed argument 
       */
      LhsVar(1)=1;
    }
  return 0;
}

/******************************************
 * SCILAB function : pvm_probe 
 ******************************************/

int intspvm_probe(fname)
     char *fname;
{
  int m1,n1,l1,m2,n2,l2,un=1;
  int res;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,"i",&m1,&n1,&l1);/*  tid */ 
  CheckScalar(1,m1,n1);
  GetRhsVar(2,"i",&m2,&n2,&l2);/* msgtag */ 
  CheckScalar(2,m2,n2);
  CreateVar(3,"i",&un,&un,&res);
  *istk(res) = pvm_probe(*istk(l1),*istk(l2));
  LhsVar(1)= 3; 
  return 0;
}

/******************************************
 * SCILAB function : pvm_bufinfo
 ******************************************/

int intspvm_bufinfo(fname)
     char *fname;
{
  int m1,n1,l1,un=1;
  int bytes,msgtag,tid,res,i;
  CheckRhs(1,1);
  CheckLhs(1,4);
  GetRhsVar(1,"i",&m1,&n1,&l1);/* bufid */ 
  CheckScalar(1,m1,n1);
  CreateVar(2,"i",&un,&un,&bytes);
  CreateVar(3,"i",&un,&un,&msgtag);
  CreateVar(4,"i",&un,&un,&tid);
  CreateVar(5,"i",&un,&un,&res);
  C2F(scipvmbufinfo)(istk(l1), istk(bytes), istk(msgtag), istk(tid), istk(res)); 
  for ( i = 1 ; i <= Lhs ; i++) 
    LhsVar(i)= i+1; 
  error_check(fname,*istk(res));
  return 0;  
}

 

static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intspvm_joingroup,"pvm_joingroup"},
  {(Myinterfun)sci_gateway,intspvm_lvgroup,"pvm_lvgroup"},
  {(Myinterfun)sci_gateway,intspvm_gsize,"pvm_gsize"},
  {(Myinterfun)sci_gateway,intspvm_gettid,"pvm_gettid"},
  {(Myinterfun)sci_gateway,intspvm_getinst,"pvm_getinst"},
  {(Myinterfun)sci_gateway,intspvm_barrier,"pvm_barrier"},
  {(Myinterfun)sci_gateway,intspvm_bcast,"pvm_bcast"},
  {(Myinterfun)sci_gateway,intspvm_tasks,"pvm_tasks"},
  {(Myinterfun)sci_gateway,intspvm_config,"pvm_config"},
  {(Myinterfun)sci_gateway,intspvm_addhosts,"pvm_addhosts"},
  {(Myinterfun)sci_gateway,intspvm_delhosts,"pvm_delhosts"},
  {(Myinterfun)sci_gateway,intspvm_parent,"pvm_parent"},
  {(Myinterfun)sci_gateway,intspvm_tidtohost,"pvm_tidtohost"},
  {(Myinterfun)sci_gateway,intspvm_set_timer,"pvm_set_timer"},
  {(Myinterfun)sci_gateway,intspvm_get_timer,"pvm_get_timer"},
  {(Myinterfun)sci_gateway,intspvm_mytid,"pvm_mytid"},
  {(Myinterfun)sci_gateway,intspvm_exit,"pvm_exit"},
  {(Myinterfun)sci_gateway,intspvm_kill,"pvm_kill"},
  {(Myinterfun)sci_gateway,intspvm_spawn,"pvm_spawn"},
  {(Myinterfun)sci_gateway,intspvm_spawn_independent,"pvm_spawn_independent"},
  {(Myinterfun)sci_gateway,intspvm_recv,"pvm_recv"},
  {(Myinterfun)sci_gateway,intspvm_send,"pvm_send"},
  {(Myinterfun)sci_gateway,intspvm_recv_var,"pvm_recv_var"},
  {(Myinterfun)sci_gateway,intspvm_send_var,"pvm_send_var"},
  {(Myinterfun)sci_gateway,intspvm_reduce,"pvm_reduce"},
  {(Myinterfun)sci_gateway,intspvm_start,"pvm_start"},
  {(Myinterfun)sci_gateway,intspvm_halt,"pvm_halt"},
  {(Myinterfun)sci_gateway,intspvm_error,"pvm_error"},
  {(Myinterfun)sci_gateway,intspvm_sci2f77,"pvm_sci2f77"},
  {(Myinterfun)sci_gateway,intspvm_f772sci,"pvm_f772sci"},
  {(Myinterfun)sci_gateway,intspvm_probe,"pvm_probe"},
  {(Myinterfun)sci_gateway,intspvm_bufinfo,"pvm_bufinfo"},
  {(Myinterfun)sci_gateway,intspvm_error_mode,"pvm_error_mode"},
};




int C2F(intpvm)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
