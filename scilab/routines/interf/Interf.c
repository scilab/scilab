/*------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#include "../stack-c.h"

#define MAX_ENV 256 

#ifdef NORANDLIB 

C2F(randlib)()
{
  int ilib=0;
  static char name[MAX_ENV];
  static char *names[]={ "randlib",(char *) 0};
  static char *files[] = {
    name,
    (char *) 0
  };
  int nums[1];
  static int Buildname=0;
  static int randpos = -1;
  if ( Buildname == 0) { BuildName(name,"rand.so"); Buildname++;}
  CallDynInterf(&randpos,1,0,names,nums,files);
}  

#endif 

#ifdef NODCD 

C2F(dcd)()
{
  int ilib=0;
  static char name[MAX_ENV];
  static char *files[] = {
    name,
    (char *) 0
  };
  int nums[1];
  static char *names[] = {"dcd",(char *)0};
  static int Buildname=0;
  static int randpos = -1;
  if ( Buildname == 0) { BuildName(name,"dcd.so"); Buildname++;}
  CallDynInterf(&randpos,1,0,names,nums,files);
}  

#endif 

#ifdef NOSCICOS 

static char scicos_name[MAX_ENV];
static char *scicos_names[]={ "coselm","intcos",(char *) 0};
static char *scicos_files[] = {
  scicos_name,
  (char *) 0
};
int scicos_nums[2];
static int coselmpos=-1;
static int intcospos=-1;

void C2F(coselm) ()
{
  static int Buildname=0;
  if ( Buildname == 0) { BuildName(scicos_name,"scicos.so"); Buildname++;}
  CallDynInterf(&coselmpos,2,0,scicos_names,scicos_nums,scicos_files);
}

void C2F(intcos) ()
{
  static int Buildname=0;
  if ( Buildname == 0) { BuildName(scicos_name,"scicos.so"); Buildname++;}
  CallDynInterf(&intcospos,2,1,scicos_names,scicos_nums,scicos_files);
}

#endif 

#ifdef NOSIGNAL

void C2F(sigelm) ()
{
  int Error=9999;
  sciprint("Scicos interface not loaded \n");
  Error(9999);
  return;
}

void C2F(datatf) ()
{
  int Error=9999;
  sciprint("Scicos interface not loaded \n");
  Error(9999);
  return;
}

#endif 


#ifdef NOSOUND 

C2F(soundi)()
{
  int Error=9999;
  sciprint("Sound and <<binary read>> interface not loaded \n");
  Error(9999);
  return;
}  

#endif 


#ifdef NOSPARSE 

void C2F(spelm) ()
{
  int Error=9999;
  sciprint("Sparse interface not loaded \n");
  Error(9999);
  return;
}

void C2F(lspops) ()
{
  int Error=9999;
  sciprint("Sparse interface not loaded \n");
  Error(9999);
  return;
}

void C2F(spops) ()
{
  int Error=9999;
  sciprint("Sparse interface not loaded \n");
  Error(9999);
  return;
}

#endif 
