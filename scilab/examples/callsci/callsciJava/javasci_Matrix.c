#include "javasci_Matrix.h"

#include <math.h>
#include <stdio.h> 
#include <string.h>
#include "machine.h"
#include "stack-c.h"


static int send_scilab_job(char *job) ;
static void Initialize() ;

extern void C2F(banier)(int *x) ;
extern int C2F(inisci)(int *,int *,int *);
extern int C2F (sciquit) (void);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F(storeversion)(char *str,int n);

static int init = 0;


JNIEXPORT void JNICALL Java_javasci_Matrix_scilabJob
  (JNIEnv *env , jobject obj_this, jstring job)
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
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,strlen(cname))) 
    fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  else { 
    if ( send_scilab_job((char *)cjob) != 0) 
      {
	fprintf(stderr,"Bug in scilab \n");
      }
    else 
      {
	if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,strlen(cname))) 
	  fprintf(stderr,"erreur lors de la reception \n");
	else 
	  for (i=0 ; i < cm*cn ; i++) cx[i] = *stk(lp+i);
      }
  }
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseStringUTFChars(env, job , cjob);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}

/*----------------------------------
 * just a test XXXXX 
 *----------------------------------*/ 

JNIEXPORT void JNICALL Java_javasci_Matrix_testFill
  (JNIEnv *env , jobject obj_this)
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
      cx[i+ (jm)*j]= i+j;

  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}


/*----------------------------------
 * Send a Scilab Matrix to scilab 
 *----------------------------------*/ 

JNIEXPORT void JNICALL Java_javasci_Matrix_scilabSend
  (JNIEnv *env , jobject obj_this)
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
  if (!  C2F(cwritemat)((char *)cname,&cm,&cn,cx,strlen(cname))) 
    fprintf(stderr,"erreur lors de l'envoi a scilab \n");
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}
/*----------------------------------
 * Get a Scilab Matrix from Scilab 
 *----------------------------------*/ 

JNIEXPORT void JNICALL Java_javasci_Matrix_scilabGet
  (JNIEnv *env , jobject obj_this)
{
  int i,cm,cn,lp;
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
  if ( ! C2F(cmatptr)((char *)cname, &cm, &cn, &lp,strlen(cname))) 
    fprintf(stderr,"erreur lors de la reception \n");
  else 
    for (i=0 ; i < cm*cn ; i++) cx[i] = *stk(lp+i);
  (*env)->ReleaseStringUTFChars(env, jname , cname);
  (*env)->ReleaseDoubleArrayElements(env,jx,cx,0);
}

/*----------------------------------
 * execute a scilab instruction 
 *----------------------------------*/ 

JNIEXPORT void JNICALL Java_javasci_Matrix_scilabExec 
  (JNIEnv *env , jclass cl, jstring job)
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



/* 
 * Initialisation de Scilab 
 * avec execution de la startup 
 * pour ne pas avoir a ecrire un script 
 * de lancement je fixe SCI en dur qui est passe par le 
 * Makefile 
 */

#ifndef SCI 
#define SCI "../.."
#endif 

static void Initialize() 
{
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;
  /* je fixe des variables d'environement
   * ici pour pas avoir de callsci a ecrire 
   */ 
  setenv("SCI",SCI,1);
  /* Scilab Initialization */ 
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }
  /* running the startup */ 
  C2F(settmpdir)();

  C2F(scirun)(initstr,strlen(initstr));
  fprintf(stderr,"Fin de Init\n");
}



static int send_scilab_job(char *job) 
{
  static char buf[1024],
    format[]="Err=execstr('%s','errcatch','n');quit;";
  int m,n,lp;
  sprintf(buf,format,job);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);
  return (int) *stk(lp);
}

/* I do not want to see the Scilab banier */ 

void C2F(banier)(int *x) 
{
  fprintf(stdout,"Et Hop ....\n");
  C2F(storeversion)("scilab-2.5.1",12L);
}

/* sert a rien sinon a satisfaire le linker */

int MAIN__() {return 0;};









