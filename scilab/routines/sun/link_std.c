/* Copyright INRIA/ENPC */
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/file.h>

#if defined mips || defined __alpha || defined hppa
#define COFF
#endif


#if defined(sun) ||  defined mips || defined __alpha || defined hppa
#include <a.out.h>

caddr_t endv[ENTRYMAX+1],wh;

#ifndef _SYS_UNISTD_INCLUDED
caddr_t sbrk();   /* memory allocation */
#endif

#define round(x,s) (((x) + ((s)-1)) & ~((s)-1))
#ifndef hppa
#define RNDVAL1 4
#define RNDVAL2 1024
#else
#define RNDVAL1 0x1000
#define RNDVAL2 0x1000
#endif /* hppa */
extern char *index();

#endif	/* sun || mips || hppa */

/*************************************
 * New version : link entry names 
 *   from new shared lib created with 
 *   files.
 * check if we can improve this for
 * multiple entry names and link /unlink 
 *************************************/

static int lastlink={0};

void SciLink(iflag,rhs,ilib,files,en_names,strf)
     int iflag,*ilib,*rhs;
     char *files[],*en_names[],*strf;
{
  char enamebuf[MAXNAME];
  int i,ii,err=0;
  if ( iflag != 0 ) 
    {
      /* First argument  can't be a number  */
      *ilib=-3;
      return;
    }
  if ( en_names[1] != (char *) 0 )
    {
      /*Only one entry point allowed on this operating system */
      *ilib=-4;
      return;
    }
  if ( NEpoints != 0 && strcmp(en_names[0],EP[NEpoints-1].name) == 0) 
    {
      ii= NEpoints-1;
    }
  else 
    {
      ii= NEpoints;
      NEpoints++;
      if ( NEpoints == ENTRYMAX ) 
	{
	  /* You cannot link more functions, maxentry reached */
	  *ilib=-2;
	  return;
	}
    }
  if ( strf[0] == 'f' )
    Underscores(1,en_names[0],enamebuf);
  else 
    Underscores(0,en_names[0],enamebuf);
  F2C(dynload)(&ii,enamebuf,files,&err);
  if ( err == 0 ) 
    {
      strncpy(EP[ii].name,en_names[0],MAXNAME);
      EP[ii].Nshared = ii;
      hd[ii].ok = OK;
      Nshared = NEpoints;
      *ilib= ii;
      return;
    }
  else
    {
      if ( NEpoints != 0) NEpoints--;
      *ilib=-2;
      return;
      
    }
}

/**************************************
 * return 1 if link accepts multiple file iin one call
 * or 0 elsewhere 
 *************************************/

int LinkStatus()
{
  return(0);
}

/**************************************
 * Unlink a shared lib 
 *************************************/

void C2F(isciulink)(i) 
     integer *i;
{
  sciprint("Std link : No unlink up to now \r\n");
}



int F2C(dynload)(ii,ename1,loaded_files,err)
     char ename1[], loaded_files[];
     int *ii;
     int *err;
{
   unsigned long epoint;
   char str[1000] , tmp_file[128], prog[200],*libs,*getenv();

   int readsize, totalsize, diff, n, p, i, nalloc,last;
   float x;
   int kk;
#ifndef _IBMR2
#if defined mips || defined __alpha
   struct  filehdr filehdr;
   struct  aouthdr aouthdr;
#endif
#ifdef hppa
   struct header filehdr;
   struct  som_exec_auxhdr aouthdr;
#endif /* hppa */
#ifndef COFF
   struct exec header;
#endif /* COFF */
   caddr_t end;
#endif /* _IBMR2 */
   extern char *sys_errlist[];
   extern errno;

#ifdef DEBUG
  sciprint("ename1 [%s]\r\n",ename1);
  sprintf(str,"lastlink %d, entry=%d\n",lastlink,*ii);Scistring(str);
#endif

   libs=getenv("SYSLIBS");

   strcpy(prog,"");
   getpath(prog);/* prog est le pathname du fichier executable*/
   
   sprintf (str,"\nlinking %s defined in %s with %s \n",ename1,loaded_files,prog);
   Scistring(str);

   sprintf(tmp_file, "/tmp/SL_%d_XXXXXX",(int) getpid());
   mktemp(tmp_file);

   if ((*ii != lastlink) && (*ii != lastlink-1)) {
     sprintf(str,"cannot (re)define this link \n");
     Scistring(str);
     return;
   }

#ifndef _IBMR2
   if (lastlink==*ii) {
      n = (int)(end = sbrk(0));
      diff = round (n,RNDVAL2) - n;

      if (diff != 0) {
         end = sbrk(diff);
         if ((int)end <= 0) {
              sprintf (str,"sbrk failed\n");
	      Scistring(str);
              *err=4;
            return;
            }
         end = sbrk(0);
         }
      nalloc=0;
      if (lastlink==0) {
        endv[0]=end;
        endv[1]=end;
      }
    }
    else {
      end = endv[*ii];
     /* recuperation d'espace   eventuellement libere */
      nalloc=((int) endv[lastlink])-((int) endv[*ii]);
      lastlink=lastlink-1;
    }
#else /* _IBMR2 */
#define vfork fork
    lastlink = *ii;
#endif /* _IBMR2 */
    {
        int pid, status, wpid, i;
        char *s;
	int argc;
	/**  FILESKP mus be NUMFILES LONG */
#define NUMFILES 20
#define FILESKP 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        static char *argv[] = {
#ifndef _IBMR2

#ifdef hppa
#define TOPT "-R"
#endif /* hppa */
#ifdef mips
#define LD 0
#define LDARG1 "-r"
#define TOPT "-G", "0", "-T"
#define ARGTOPT 8
#define TAILARG 13
#endif /* mips */

#ifndef TOPT
#define TOPT "-T"
#endif /* TOPT */

#ifndef AARG
#define AARG "-A", 0, 
#endif

#ifndef LD
#define LD "/bin/ld"
#endif

#ifndef LDARG1
#define LDARG1  "-N"
#endif

#ifndef ARGTOPT
#define ARGTOPT 6
#endif
	    LD, LDARG1, "-x", AARG TOPT, 0, "-o", 0, "-e", 0, FILESKP
#ifndef TAILARG
#define TAILARG 11
#endif /* TAILARG */
#else /* _IBMR2 */
	    "/bin/ld", 0 /* -bI:prog.exp */,"-K","-o", 0, "-e", 0, FILESKP
#define TAILARG 7
#endif /* _IBMR2 */
        };

#ifndef _IBMR2
        char hexentry[10];
        argv[4] = prog;
#ifdef mips
	strcpy(str, prog);
	if (s = rindex(str,'/'))
	    strcpy(s+1, "dold");
	else
	    strcpy(str, "dold");
	argv[0] = str;
#endif /* mips */
        sprintf(hexentry, "%lx", end);
        argv[ARGTOPT] = hexentry;
#else /* _IBMR2 */
	sprintf(str, "-bI:%s.exp", prog);
	argv[1] = str;
#endif /* _IBMR2 */
        argv[TAILARG - 3] = tmp_file;
        argv[TAILARG - 1] = ename1;
	argc=TAILARG;
#define MAXARGV (TAILARG+NUMFILES-1)

	argc = SetArgv(argv,loaded_files,argc,MAXARGV,err);
	if ( *err == 1 ) return(-1);
#ifdef hppa
	argv[argc] = "/lib/dyncall.o";
	argc++;
#endif /* hppa */
	if (libs) 
	  {
	    argc = SetArgv1(argv,libs,argc,MAXARGV,err);
	    if ( *err == 1 ) return(-1);
        }
	argv[argc] = (char *) 0;
        if ((pid = vfork()) == 0) {
                execv(argv[0], argv);
                _exit(1);
        }
        if (pid < 0) {
                sprintf (str,"can't create new process: %s\n", sys_errlist[errno]);
		Scistring(str);
                goto bad;
        }
        while ((wpid = wait(&status)) != pid)
                if (wpid < 0) {
                        sprintf (str,"no child !\n");
			Scistring(str);
                        goto bad;
                }
        if (status != 0) {

                sprintf (str,"ld returned bad status: %x\n", status);
		Scistring(str);
bad:
/*		printf ("loading error\n");*/
		*err=1;
		goto badunlink;
	}

   }

#ifndef _IBMR2
   if ((p = open(tmp_file, O_RDONLY)) < 0) {
        sprintf (str,"Cannot open %s\n", tmp_file);
	Scistring(str);
        *err=2;
        goto badclose;
      }

   /* read the a.out header and find out how much room to allocate */

#ifdef COFF
   if (read(p, &filehdr, sizeof filehdr) != sizeof filehdr) {
           sprintf (str,"Cannot read filehdr from %s\n", tmp_file);
	   Scistring(str);
           *err=3;
           goto badclose;
   }
#ifdef hppa
#define tsize exec_tsize
#define dsize exec_dsize
#define bsize exec_bsize
#define entry exec_entry
#define text_start exec_tmem
#define TEXTBEGIN aouthdr.exec_tfile
   lseek(p, filehdr.aux_header_location, 0);
#else
#ifdef N_TXTOFF
#define TEXTBEGIN N_TXTOFF(filehdr, aouthdr)
#else
#define TEXTBEGIN sizeof filehdr + filehdr.f_opthdr + \
		filehdr.f_nscns*sizeof (struct scnhdr)
#endif
#endif /* hppa */
   if (read(p, &aouthdr, sizeof aouthdr) != sizeof aouthdr) {
           sprintf (str,"Cannot read auxhdr from %s\n", tmp_file);
	   Scistring(str);
           *err=3;
           goto badclose;
   }

   readsize = round(aouthdr.tsize, RNDVAL1) + round(aouthdr.dsize, RNDVAL1);
   totalsize = readsize + aouthdr.bsize;
   i = lseek(p, TEXTBEGIN, 0);
#else /* ! COFF */
   if (sizeof(header) != read(p, (char *)&header,sizeof(header))) {
        sprintf (str,"Cannot read header from %s\n", tmp_file);
	Scistring(str);
        *err=3;
      goto badclose;
      }

   /* calculate  sizes */

   readsize = round(header.a_text, RNDVAL1) + round(header.a_data, RNDVAL1);
   totalsize = readsize + header.a_bss;
#endif /* COFF */
   totalsize = round(totalsize, RNDVAL2);   /* round up a bit */

   /* allocate more memory, using sbrk */
   wh = sbrk(totalsize-nalloc);
   if ( (int)wh <= 0 ) {
     sprintf (str,"sbrk failed to allocate\n");
     Scistring(str);
     *err=4;
     goto badclose;
   }
   endv[*ii+1]=sbrk(0);

   /* now read in the function */
   i=read(p, (char *)end, readsize);
   if (readsize != i) {
      sprintf (str,"Cannot read %s\n", tmp_file);
      Scistring(str);
      *err=5;
      goto badclose;
      }
#ifdef __alpha
  {
#include <sys/mman.h>
   i = mprotect(end, readsize, PROT_READ|PROT_WRITE|PROT_EXEC);
   if (i < 0) {
     perror("mprotect");
     *err = errno;
     return;
   }
  }
#endif

   /* set the first entry up to the header value */

#ifdef COFF
   EP[*ii].epoint = (function)(aouthdr.entry?aouthdr.entry:aouthdr.text_start);
#if defined mips
#include <mips/cachectl.h>
   cacheflush(end, totalsize, BCACHE);
#endif /* mips */
#else
   EP[*ii].epoint = (function) header.a_entry;

#endif /* COFF */
   lastlink=lastlink+1;
badclose:
   close(p);
   unlink(tmp_file);
#else /* _IBMR2 */
   EP[*ii].epoint =(function) load(tmp_file,1, "");
   if ( EP[*ii].epoint == (function) 0) {
	   sprintf (str,"ibm load routine failed: %s\n", sys_errlist[errno]);
	   Scistring(str);
	   *err = 6;
   }
#endif /* _IBMR2 */
badunlink:
   unlink(tmp_file);
   return;
 }

/*
Etant donne le "nom" du fichier executable getpath retourne son pathname
en utilisant les regles de recherches definies par la variable
d'environnement PATH.
Si en entree name contient au moins un caractere "/" il est considere
que name est un pathname (et non un nom) et retourne tel que
origine S Steer INRIA 1988

*/
getpath(name)

char name[];
{
struct stat stbuf;
short unsigned mode;
char *searchpath, buf[200],prog[200];
char *getenv();
int kd, kf, j, i ,ok, km;

F2C(getpro)(prog,sizeof(prog)-1);
     strcpy(name,prog);

if ( (index(name,'/')) != 0)
     return;

/* on recupere la regle de recherche */
if ( (searchpath=getenv("PATH")) == NULL)
  {
    printf("variable PATH not defined\n");
    return;
  }

ok=kd=kf=0;

while (ok == 0) {
  /* recherche de la fin d'une regle (: ou fin de chaine) */
  while ((searchpath[kf] != ':')&&(searchpath[kf++] !='\0'))
   ;
  if (searchpath[kf-1]=='\0') {
   ok=1;
   kf--;
 }

  /* recopie de cette regle en debut de buf */
  j=0;
  for (i=kd; i<kf; ) {
       buf[j++]=searchpath[i++];
     }

  /* concatenation de la regle avec le nom du fichier */
  buf[j++]='/';
  i=0;
  while ((name[i] != ' ')&& (name[i] != '\0')) {
      buf[j++]=name[i++] ;
      buf[j]='\0';
    }

  /* test d'existence du fichier deisgne par le path fourni par la
     regle de recherche */
  if (stat(buf,&stbuf) != -1) {
    mode=stbuf.st_mode;

  /* les tests suivants permettent de savoir si le fichier designe
      par buf est executable ou non */

    mode=mode-(128*(mode/128));
    km=mode/16;
    if (km != 2*(km/2)) {
       strcpy(name,buf);
       return;
     }
    mode=mode-16*km;
    km=mode/8;
    if (km != 2*(km/2)) {
       strcpy(name,buf);
       return;
     }
    km=mode-8*km;
    if (km != 2*(km/2)) {
       strcpy(name,buf);
       return;
     }
  }
  kf++;
  kd=kf;
  }

return;
}

#ifdef hppa
int
F2C(getpro)(s, l)
char *s;
int l;
{
  extern char *__data_start[];
  strncpy (s,__data_start[0], l);
}
#endif


  
/******************************************************
 * Utility function 
 * files is a null terminated array of char pointers 
 * files[i] is broken into pieces ( separated by ' ') 
 * and positions are stored in argv starting at position 
 * first 
 *******************************************************/

int SetArgv(argv,files,first,max,err)
     char *argv[];
     char *files[];
     int first,max,*err;
{
  int i=0,j=first;
  *err=0;
  while ( files[i] != (char *) NULL) 
    {
      j= SetArgv1(argv,files[i],j,max,err);
      if (*err == 1) return(j);
      i++;
    }
  return(j);
}

int SetArgv1(argv,files,first,max,err)
     char *argv[];
     char *files;
     int first,max,*err;
{
  int j=first;
  char *loc = files;
  while ( *loc == ' ' && *loc != '\0'  ) loc++;
  while ( *loc != '\0' )
    {
      argv[j] = loc; j++;
      if ( j == max ) 
	{
	  sciprint("Link too many files \r\n");
	  *err=1;
	  break;
	}
      if ( ( loc  = strchr(loc, ' ')) != (char *) 0) 
	{ 
	  *loc = 0;	loc++;
	  while ( *loc == ' ' && *loc != '\0'  ) loc++;
	}
      else
	{
	  break;
	}
    }
  return(j);
}
  
