#include "javasci_Matrix.h"
/********************************************************************************************************/
/* Copyright Cermics/Enpc : jean-philippe Chancelier */
/* Modifications Allan CORNET */
/* INRIA 2004 */
/********************************************************************************************************/
#include <math.h>
#include <stdio.h> 
#include <string.h>
#if WIN32
  #include <windows.h>
  #pragma comment(lib, "User32.lib")
#endif

#include "../machine.h"
#include "../stack-c.h"

/********************************************************************************************************/
#ifndef SCI 
  #define SCI ".."
#endif 
/********************************************************************************************************/
static int send_scilab_job(char *job) ;
static void Initialize() ;
/********************************************************************************************************/
extern void C2F(banier)(int *x) ;
extern integer C2F (ismenu) __PARAMS((void));
extern void initTCLTK();
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F(storeversion)(char *str,int n);
extern  int C2F(sxevents)();
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int version_flag();
/********************************************************************************************************/
#if WIN32
extern char *getenv();
extern void start_sci_gtk();
extern void add_sci_argv();
extern void add_sci_argv();
extern void set_sci_env();
extern void TextMessage1 (int ctrlflag);
extern void InitWindowGraphDll(void);
#endif
/********************************************************************************************************/
static int init = 0;
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Matrix_scilabJob(JNIEnv *env , jobject obj_this, jstring job)
{
  int i,cm,cn,lp;
  const char *cname,*cjob;
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);
  cjob = (*env)->GetStringUTFChars(env, job, NULL);

  if ( init == 0) { init++; Initialize();} 
  cm=jm;cn=jn;
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname))) 
    fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  else { 
    if ( send_scilab_job((char *)cjob) != 0) 
      {
	fprintf(stderr,"Bug in scilab \n");
      }
    else 
      {
	if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
	  fprintf(stderr,"erreur lors de la reception \n");
	else 
	  for (i=0 ; i < cm*cn ; i++) cx[i] = 0; /* *stk(lp+i); */
      }
  }
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseStringUTFChars(env, job , cjob);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
JNIEXPORT void JNICALL Java_javasci_Matrix_testFill(JNIEnv *env , jobject obj_this)
{
  int i,j;
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  
  for ( i = 0 ; i < jm ; i++) 
    for ( j = 0 ; j < jn ; j++) 
      cx[i + (jm)*j]= i+j;

  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*----------------------------------
 * Send a Scilab Matrix to scilab 
 *----------------------------------*/ 
JNIEXPORT void JNICALL Java_javasci_Matrix_scilabSend(JNIEnv *env , jobject obj_this)
{
  int cm,cn;
  const char *cname; 
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  /* get the field value */
  jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( init == 0) { init++; Initialize();} 
  cm=jm;cn=jn;
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,(unsigned long)strlen(cname))) 
    fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*----------------------------------
 *  scilabEvents
 *----------------------------------*/
JNIEXPORT void JNICALL Java_javasci_Matrix_scilabEvents(JNIEnv *env , jobject obj_this)
{
 #if WIN32
	TextMessage1 (1);
 #else 
	C2F(sxevents)();
 #endif

  while(C2F(ismenu)()==1 ) 
  {
    C2F(scirun)("quit;",(int)strlen("quit;"));
  }
}
/********************************************************************************************************/
JNIEXPORT jint JNICALL Java_javasci_Matrix_scilabHaveAGraph (JNIEnv *env , jobject obj_this)
{
	integer iflag =0,ids,num;
	jint vInt=0;

	if (version_flag() == 0)
	{
		sciGetIdFigure (&ids,&num,&iflag);
		if (num > 0) vInt=1;
		
	}/* NG end*/
	else
	{
		C2F(getwins)(&num,&ids ,&iflag);
		if (num > 0) vInt=1;
    } 
 
	return vInt;
}
/********************************************************************************************************/
/*----------------------------------
 * Get a Scilab Matrix from Scilab 
 *----------------------------------*/ 
JNIEXPORT void JNICALL Java_javasci_Matrix_scilabGetN(JNIEnv *env , jobject obj_this)
{
  int cm,cn,lp;
  const char *cname; 
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  jfieldID id_t = (*env)->GetFieldID(env, class_Mine, "t", "I");
   /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  jint jt = (*env)->GetIntField(env, obj_this, id_t);
	jfieldID id_x = (*env)->GetFieldID(env, class_Mine, "x", "[D");
	jdoubleArray jx = (*env)->GetObjectField(env, obj_this, id_x);
	double *cx = (*env)->GetDoubleArrayElements(env,jx,NULL);

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( init == 0) { init++; Initialize();} 
  cm=jm;cn=jn;
  if ( jt == 0 )
	{
	  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp, (unsigned long)strlen(cname))) 
		  fprintf(stderr,"erreur lors de la reception \n");
	  else
	  {
		  C2F(creadmat)((char *)cname, &cm, &cn, cx,(unsigned long)strlen(cname));
	  }
	}
	else 
	{
	  if ( ! C2F(cmatcptr)((char *)cname, &cm, &cn, &lp,(unsigned long)strlen(cname))) 
		 fprintf(stderr,"erreur lors de la reception \n");
	  else
	  {
		  C2F(creadcmat)((char *)cname, &cm, &cn, cx,(unsigned long)strlen(cname));
	  }
	}
	(*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/********************************************************************************************************/
/*----------------------------------
 * Get a Scilab Matrix from Scilab 
 *----------------------------------*/ 
JNIEXPORT jstring JNICALL Java_javasci_Matrix_scilabGetSN(JNIEnv *env , jobject obj_this, jint ixarg, jint jarg)
{
  int cm,cn,lp;
  const char *cname; 
  int ix, j, nlr;
  char tmpStr[100];
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  /* get the fields i.e x,m,n,name  */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name", 
					 "Ljava/lang/String;");
  jfieldID id_m = (*env)->GetFieldID(env, class_Mine, "m", "I");
  jfieldID id_n = (*env)->GetFieldID(env, class_Mine, "n", "I");
  jfieldID id_t = (*env)->GetFieldID(env, class_Mine, "t", "I");
   /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jint jm = (*env)->GetIntField(env, obj_this, id_m);
  jint jn = (*env)->GetIntField(env, obj_this, id_n);
  /*local variable is initialized but not referenced*/
/*  jint jt = (*env)->GetIntField(env, obj_this, id_t);*/

  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  if ( init == 0) { init++; Initialize();} 
  cm=jm;cn=jn;
  ix= ixarg + 1;
  j = jarg + 1;

  if ( ! C2F(cmatsptr)((char *)cname, &cm, &cn, &ix, &j, &lp, &nlr, (unsigned long)strlen(cname))) 
	  fprintf(stderr,"erreur lors de la reception \n");
  else
  {
	  nlr = 100;
	  C2F(creadchains)((char *)cname, &ix, &j, &nlr, tmpStr, (unsigned long)strlen(cname), (unsigned long)strlen(tmpStr));
  }

  /* (*env)->ReleaseStringArrayElements(env,js,cs,0); */
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  return (*env)->NewStringUTF(env, tmpStr);
}
/********************************************************************************************************/
/*----------------------------------
 * execute a scilab instruction 
 *----------------------------------*/ 
JNIEXPORT void JNICALL Java_javasci_Matrix_scilabExec(JNIEnv *env , jclass cl, jstring job)
{
  const char *cjob;
  cjob = (*env)->GetStringUTFChars(env, job, NULL);
  if ( init == 0) { init++; Initialize();} 
  if ( send_scilab_job((char *)cjob) != 0) 
    {
      fprintf(stderr,"Bug in scilab \n");
    }
  (*env)->ReleaseStringUTFChars(env, job , cjob);
}
/********************************************************************************************************/
/* 
 * Initialisation de Scilab 
 */
/********************************************************************************************************/
static void Initialize() 
{
  static char env[1024];
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;

  #if WIN32
    static char nw[]="-nw";
    static char nb[]="-nb";
  #endif

  #if WIN32
    char *p1 = (char*)getenv ("SCI");
  #endif
  
  #if WIN32
  /* Supprime le mode windows et la baniere */
    add_sci_argv(nb);
    add_sci_argv(nw);
  #endif
  
  #if WIN32 
    if ( p1== NULL )
    {
		/* Detection Scilab path */
		char modname[MAX_PATH+1];
		if (!GetModuleFileName (GetModuleHandle("javasci.dll"), modname, MAX_PATH))
		{
			MessageBox(NULL,"javasci.dll not found","Warning",MB_ICONWARNING);
		}
		else
		{
			char *p;
			if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \javasci.dll from modname */
			else
			{
				*p='\0';
				if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \bin from modname */
				else
				{
					*p='\0';
					set_sci_env(modname);
				}
			}
		}
    }
    else 
	{
		set_sci_env(p1);
	}
  #else
   sprintf (env, "%s=%s", "SCI",SCI);
   putenv (env);
  #endif

  /* Scilab Initialization */
  initTCLTK(); /* TK Init. */
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }
  /* running the startup */ 
  C2F(settmpdir)();

  /* Initialisation fenetre graphique */
  #if WIN32
    InitWindowGraphDll();
    start_sci_gtk() ;
  #endif

  /* Chargement de Scilab.star */
  C2F(scirun)(initstr,(int)strlen(initstr));
  
  fprintf(stderr,"Fin de Init\n");
 
}
/********************************************************************************************************/
static int send_scilab_job(char *job) 
{
  static char buf[1024],
  format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  C2F(scirun)(buf,(int)strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return (int) 0; /* *stk(lp); */
}
/********************************************************************************************************/
int MAIN__() 
{
	return 0;
}
/********************************************************************************************************/
