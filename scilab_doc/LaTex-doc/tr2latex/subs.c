/* COPYRIGHT (C) 1987 Kamal Al-Yahya */
/* 
   These subroutines do (in general) small things for the translator.
   They appear in alphabetical order and their names are unique in the
   first six characters.
*/

#include        "setups.h"
#include        "simil.h"
#include        "greek.h"
#include        "flip.h"
#include        "forbid.h"
#include        "maths.h"
#include        "macros.h"

extern def_count;
extern mydef_count;

#define NULL_C '\0'

/* compile-time counting of elements */
int GRK_count = (sizeof(GRK_list)/sizeof(GRK_list[0]));
int sim_count = (sizeof(sim_list)/sizeof(sim_list[0]));
int flip_count = (sizeof(flip_list)/sizeof(flip_list[0]));
int forbd_count = (sizeof(forbid)/sizeof(forbid[0]));
int mathcom_count = (sizeof(math)/sizeof(struct math_equiv));
int macro_count = (sizeof(macro)/sizeof(struct macro_table));

char *
alternate(inbuf,outbuf,w)		/* alternate fonts (manual macro) */
     char *inbuf, *outbuf, *w;
{
  int f1,f2;
  int which=1;
  char font[MAXWORD], font1[MAXWORD], font2[MAXWORD],
    ww[MAXWORD], tmp[MAXWORD];

  tmp[0] = NULL_C;
  f1 = w[1];	f2 = w[2];
  if (f1 == 'R')	strcpy(font1,"\\rm");
  if (f1 == 'I')	strcpy(font1,"\\it");
  if (f1 == 'B')	strcpy(font1,"\\bf");
  if (f2 == 'R')	strcpy(font2,"\\rm");
  if (f2 == 'I')	strcpy(font2,"\\it");
  if (f2 == 'B')	strcpy(font2,"\\bf");

  strcpy(font,font1);
  while (*inbuf != '\n' && *inbuf != NULL_C)
    {
      inbuf += get_arg(inbuf,ww,1);
      sprintf(tmp,"{%s %s}",font,ww);
      outbuf = strapp(outbuf,tmp);
      if (which == 1)
	{
	  which = 2;
	  strcpy(font,font2);
	}
      else
	{
	  which = 1;
	  strcpy(font,font1);
	}
      while (*inbuf == ' ' || *inbuf == '\t')
	inbuf++;
    }

  return(outbuf);
}

int
CAP_GREEK(w)			/* check if w is in the GREEK list */
     char *w;
{
  int i;

  for (i=0; i < GRK_count ; i++)
    {
      if (strcmp(GRK_list[i],w) == 0)
	return(1);
    }
  return(-1);
}

char *
do_table(inbuf,outbuf,offset)
     char *inbuf, *outbuf;
     int *offset;				/* amount to offset inbuf */
{
  char w[MAXWORD], ww[MAXWORD], format[MAXWORD], tmp[MAXWORD];
  char *ptr;
  int i,j,len,columns=0;
  int tab = '\t';				/* default tab */

  tmp[0] = NULL_C;
  ptr = inbuf;				/* remember where we started */
  len = get_line(inbuf,w,0);
  if (w[strlen(w)-1] == ';')		/* options */
    {
      inbuf += len;
      if (strncmp(w,"tab",3) == 0)	/* get the tab charecter */
	tab = w[4];		/* expect something like tab(&); */
      inbuf = skip_line(inbuf);
    }
  while (*inbuf != NULL_C)			/* get the LAST format line */
    {
      len = get_line(inbuf,w,0);
      if (w[strlen(w)-1] != '.')	break;	/* not a fromat line */
      inbuf += len;
      for (i=0, j=0; i<len-1; i++)
	{
	  if (isspace(w[i]))	continue;
	  columns++;
	  if (w[i] == 'l')	format[j] = 'l';
	  else if (w[i] == 'r')	format[j] = 'r';
	  else			format[j] = 'c';
	  j++;
	}
    }
  if (columns == 0)
    {
      fprintf(stderr,"Sorry, I cannot do tables without a format line\n\
Doing plain translation of table, lines will be commented\n\
You need to fix it yourself\n");
      while (*inbuf != NULL_C)
	{
	  (void) getword(inbuf,w);
	  if (strcmp(w,".TE") ==  0)	{inbuf += 4;	break;}
	  inbuf += get_line(inbuf,w,1);
	  *outbuf++ = '%';
	  outbuf = strapp(outbuf,w);
	  outbuf = strapp(outbuf,"\n");
	  inbuf++;		/* skip the \n */
	}
      *offset = inbuf - ptr;
      return(outbuf);
    }
  format[j] = NULL_C;
  sprintf(tmp,"\\par\n\\begin{tabular}{%s}\n",format);
  outbuf = strapp(outbuf,tmp);

  while (*inbuf != NULL_C)
    {
      for (i=0; i<columns-1; i++)
	{
	  (void) getword(inbuf,w);
	  if (i == 0 && (strcmp(w,"\n") == 0 || strcmp(w,"_") == 0))
	    {inbuf++;	i--;	continue;}
	  if (strcmp(w,".TE") == 0)
	    {
	      inbuf += 4;
	      if (i == 0)
		{
		  outbuf -= 3;	/* take back the \\ and the \n */
		  *outbuf = NULL_C;
		}
	      outbuf = strapp(outbuf,"\n\\end{tabular}\n\\par\n");
	      *offset = inbuf - ptr;
	      return(outbuf);
	    }
	  inbuf += get_table_entry(inbuf,w,tab);
	  inbuf ++;		/* skip tab */
	  troff_tex(w,ww,0,1);
	  sprintf(tmp,"%s & ",ww);
	  outbuf = strapp(outbuf,tmp);
	}
      (void) getword(inbuf,w);
      if (strcmp(w,".TE") == 0)
	{
	  fprintf(stderr,"Oops! I goofed. I told I you I am not very good at tables\nI've encountered an unexpected end for the table\n\
You need to fix it yourself\n");
	  inbuf += 4;
	  outbuf = strapp(outbuf,"\\end{tabular}\n\\par\n");
	  *offset = inbuf - ptr;
	  return(outbuf);
	}
      inbuf += get_table_entry(inbuf,w,'\n');
      inbuf++;		/* skip tab */
      troff_tex(w,ww,0,1);
      outbuf = strapp(outbuf,ww);
      outbuf = strapp(outbuf,"\\\\\n");
    }
  fprintf(stderr,"Oops! I goofed. I told I you I am not very good at tables\n\
File ended and I haven't finished the table!\n\
You need to fix it yourself\n");
  *offset = inbuf - ptr;
  outbuf = strapp(outbuf,"\\end{tabular}\n\\par\n");
  return(outbuf);
}

char *
end_env(outbuf)
     char *outbuf;
{
  if (IP_stat)
    {
      IP_stat=0;
      outbuf = strapp(outbuf,"\\end{scitem}");
    }
  if (QP_stat)
    {
      QP_stat=0;
      outbuf = strapp(outbuf,"\\end{quotation}");
    }
  if (TP_stat)
    {
      TP_stat=0;
      /** JPC 
	  outbuf = strapp(outbuf,"\\end{TPlist}"); **/
      outbuf = strapp(outbuf,"\\end{scitem}% end Env\n"); 
    }
  return(outbuf);
}

void
envoke_stat(par)
     int par;
{

  switch(par)
    {
    case 2:
      IP_stat = 1;
      break;
    case 3:
      TP_stat = 1;
      break;
    case 4:
      QP_stat = 1;
      break;
    default:
      break;
    }
}

char *
flip(outbuf,w)			/* do the flipping */
     char *outbuf, *w;
{
  int lb=0, rb=0;
  char ww[MAXWORD], tmp[MAXWORD];

  ww[0] = NULL_C;	tmp[0] = NULL_C;
  outbuf--;
  while (*outbuf == ' ' || *outbuf == '\t' || *outbuf == '\n')
    outbuf--;
  while (1)
    {
      if (*outbuf == '{')
	{
	  lb++;
	  if (lb > rb)	break;
	}
      if (*outbuf == '}')	rb++;
      if (rb == 0)
	{
	  if (*outbuf != ' ' && *outbuf != '\t' && *outbuf != '\n'
	      && *outbuf != '$')
	    {
	      outbuf--;
	      continue;
	    }
	  else	break;
	}
      outbuf--;
      if (lb == rb && lb != 0)	break;
    }
  outbuf++;
  if (*outbuf == '\\')
    {
      outbuf++;
      (void) getword(outbuf,tmp);
      sprintf(ww,"\\%s",tmp);
      outbuf--;
    }
  else if (*outbuf == '{')
    (void) get_brace_arg(outbuf,ww);
  else
    (void) getword(outbuf,ww);
  *outbuf = NULL_C;
  sprintf(tmp,"\\%s %s",w,ww);
  outbuf = strapp(outbuf,tmp);
  return(outbuf);
}

char *
flip_twice(outbuf,w,ww)		/* take care of things like x hat under */
     char *outbuf, *w, *ww;
{
  int lb=0, rb=0;
  char tmp1[MAXWORD], tmp2[MAXWORD];

  tmp1[0] = NULL_C;		tmp2[0] = NULL_C;
  outbuf--;
  while (*outbuf == ' ' || *outbuf == '\t' || *outbuf == '\n')
    outbuf--;
  while (1)
    {
      if (*outbuf == '{')
	{
	  lb++;
	  if (lb > rb)	break;
	}
      if (*outbuf == '}')	rb++;
      if (rb == 0)
	{
	  if (*outbuf != ' ' && *outbuf != '\t' && *outbuf != '\n'
	      && *outbuf != '$')
	    {
	      outbuf--;
	      continue;
	    }
	  else	break;
	}
      outbuf--;
      if (lb == rb && lb != 0)	break;
    }
  outbuf++;
  if (*outbuf == '\\')
    {
      outbuf++;
      (void) getword(outbuf,tmp2);
      sprintf(tmp1,"\\%s",tmp2);
      outbuf--;
    }
  else if (*outbuf == '{')
    (void) get_brace_arg(outbuf,tmp1);
  else
    (void) getword(outbuf,tmp1);
  *outbuf = NULL_C;
  sprintf(tmp2,"\\%s{\\%s %s}",w,ww,tmp1);
  outbuf = strapp(outbuf,tmp2);
  return(outbuf);
}

int
get_arg(inbuf,w,rec)		/* get argumnet */
     char *inbuf, *w;
     int rec;		/* rec=1 means recursive */
{
  int c,len,i;
  char ww[MAXWORD];
  int delim;

  len=0;
  while ((c = *inbuf) == ' ' || c == '\t')	/* skip spaces and tabs */
    {inbuf++;	len++;}
  i=0;
  if (*inbuf == '{' || *inbuf == '\"')
    {
      if (*inbuf == '{')	delim = '}';
      else			delim = '\"';
      inbuf++;	len++;
      while ((c = *inbuf++) != NULL_C && c != delim && i < MAXWORD)
	{
	  if (c == ' ' && delim == '\"')	ww[i++] = '\\';
	  ww[i++] = (char)c;	len++;
	}
      len++;
    }
  else
    {
      while ((c = *inbuf++) != NULL_C && c != ' ' && c != '\t' && c != '\n'
	     && c != '$' && c != '}' && i < MAXWORD)
	{
	  if (math_mode && c == '~')	break;
	  ww[i++] = (char)c;	len++;
	}
    }
  ww[i] = NULL_C;
  if (rec == 1)				/* check if recursion is rquired */
    troff_tex(ww,w,1,1);
  else
    strcpy(w,ww);
  return(len);
}

void
get_brace_arg(buf,w)		/* get argumnet surrounded by braces */
     char *buf, *w;
{
  int c,i, lb=0, rb=0;

  i=0;
  while ((c = *buf++) != NULL_C)
    {
      w[i++] = (char)c;
      if (c == '{')	lb++;
      if (c == '}')	rb++;
      if (lb == rb)	break;
    }
  w[i] = NULL_C;
}

int
get_defword(inbuf,w,illegal)		/* get "define" or .de word */
     char *inbuf, *w;			/* delimited by space only */
     int *illegal;
{
  int c,i;

  *illegal = 0;
  for (i=0; (c = *inbuf++) != NULL_C && c != ' ' && c != '\n'
	 && c != '\t' && i < MAXWORD; i++)
    {
      w[i] = (char)c;
      if (isalpha(c) == 0)	*illegal = 1;	/* illegal TeX macro */ 
    }
  w[i] = NULL_C;
  if (*illegal == 0)
    if (is_forbid(w) >= 0)		*illegal=1;
  return(i);
}

int
get_line(inbuf,w,rec)		/* get the rest of the line */
     char *inbuf, *w;
     int rec;			/* rec=1 means recursion is required */
{
  int c,i,len;
  char ww[MAXLINE];

  i=0;	len=0;
  while ((c = *inbuf++) != NULL_C && c != '\n' && len < MAXLINE)
    {ww[i++] = (char)c;	len++;}
  ww[i] = NULL_C;
  if (rec == 1)
    troff_tex(ww,w,0,1);
  else
    strcpy(w,ww);
  return(len);
}

int
get_multi_line(inbuf,w)		/* get multi-line argument */
     char *inbuf, *w;
{
  int len=0,l=0,lines=0;
  char tmp[MAXWORD];
  int c1,c2;

  w[0] = NULL_C;	tmp[0] = NULL_C;
  while (*inbuf != NULL_C)
    {
      c1 = *inbuf;	c2 = *++inbuf;		--inbuf;
      if (c1 == '.' && isupper(c2))		break; 
      lines++;
      if (lines > 1)
	strcat(w," \\\\\n");
      l = get_line(inbuf,tmp,1);
      strcat(w,tmp);
      len += l+1;	inbuf += l+1;
    }
  len--;		inbuf--;
  return(len);
}

int
get_verbatim_lines(inbuf,w)		/* JPC get .nf .fi argument */
     char *inbuf, *w;
{
  int len=0,l=0,lines=0;
  char tmp[MAXWORD];
  int c1,c2;

  w[0] = NULL_C;	tmp[0] = NULL_C;
  while (*inbuf != NULL_C)
    {
      c1 = *inbuf;	c2 = *++inbuf;		--inbuf;
      if (c1 == '.' && c2=='f')		break; 
      lines++;
      if (lines > 1)
	strcat(w,"\n");
      l = get_line(inbuf,tmp,0);   /* no recursion on verbatim */
      strcat(w,tmp);
      len += l+1;	inbuf += l+1;
    }
  len--;		inbuf--;
  return(len);
}

int
get_mydef(inbuf,w)		/* get the macro substitution */
     char *inbuf, *w;
{
  int c1,c2,l,len;
  char tmp[MAXWORD];

  tmp[0] = NULL_C;
  len=1;
  while (*inbuf != NULL_C)
    {
      c1 = *inbuf;	c2 = *++inbuf;		--inbuf;
      if (c1 == '.' && c2 == '.')		break; 
      l = get_line(inbuf,tmp,1);
      strcat(w,tmp);
      len += l+1;	inbuf += l+1;
    }
  return(len);
}
int
get_N_lines(inbuf,w,N)		/* get N lines */
     char *inbuf, *w;
     int N;
{
  int len=0,l=0,lines=0;
  char tmp[MAXWORD];

  w[0] = NULL_C;	tmp[0] = NULL_C;
  while (*inbuf != NULL_C && lines < N)
    {
      lines++;
      if (lines > 1)
	strcat(w," \\\\\n");
      l = get_line(inbuf,tmp,1);
      strcat(w,tmp);
      len += l+1;	inbuf += l+1;
    }
  len--;		inbuf--;
  return(len);
}

int
get_no_math(inbuf,w)		/* get text surrounded by quotes in math mode */
     char *inbuf, *w;
{
  int c,i,len;

  len = 0;
  for (i=0; (c = *inbuf++) != NULL_C && c != '\"' && i < MAXWORD; i++)
    {
      if (c == '{' || c == '}')
	{w[i] = '\\';	w[++i] = (char)c;}
      else
	w[i] = (char)c;
      len++;
    }
  w[i] = NULL_C;
  return(len);
}

char *
get_over_arg(inbuf,ww)		/* get the denominator of over */
     char *inbuf, *ww;
{
  char w[MAXWORD], tmp1[MAXWORD], tmp2[MAXWORD];
  int len;

  w[0] = NULL_C;	tmp1[0] = NULL_C;		tmp2[0] = NULL_C;
  inbuf += getword(inbuf,tmp1);		/* read first word */
  inbuf += skip_white(inbuf);
  len = getword(inbuf,tmp2);		/* read second word */
  strcat(w,tmp1);	strcat(w," ");

  /* as long as there is a sup or sub read the next two words */
  while (strcmp(tmp2,"sub") == 0 || strcmp(tmp2,"sup") == 0)
    {
      inbuf += len;
      strcat(w,tmp2); strcat(w," ");
      inbuf += skip_white(inbuf);
      inbuf += getword(inbuf,tmp1);
      strcat(w,tmp1); strcat(w," ");
      inbuf += skip_white(inbuf);
      len = getword(inbuf,tmp2);
    }
  troff_tex(w,ww,0,1);
  return(inbuf);
}

int
get_ref(inbuf,w)		/* get reference */
     char *inbuf, *w;
{
  int len=0, l=0, lines=0;
  char tmp[MAXWORD];

  w[0] = NULL_C;	tmp[0] = NULL_C;
  while (*inbuf != NULL_C)
    {
      if (*inbuf == '\n')		break;
      (void) getword(inbuf,tmp);
      if (tmp[0] == '.' && isupper(tmp[1]))
	{
	  /* these commands don't cause a break in reference */
	  if (strcmp(tmp,".R") != 0 && strcmp(tmp,".I") != 0
	      && strcmp(tmp,".B") != 0)
	    break; 
	}
      else if (tmp[0] == '.' && !(isupper(tmp[1])))
	{
	  /* these commands don't cause a break in reference */
	  if (strcmp(tmp,".br") != 0 && strcmp(tmp,".bp") != 0)
	    break; 
	}
      l = get_line(inbuf,tmp,1);
      lines++;
      if (lines > 1)		strcat(w," ");
      strcat(w,tmp);
      len += l+1;	inbuf += l+1;
    }
  len--;		inbuf--;
  return(len);
}

void
get_size(ww,PARAMETER)
     char *ww;
     struct measure *PARAMETER;
{
  int sign=0, units=0;
  float value;

  if (ww[0] == NULL_C)
    {
      if (PARAMETER->def_value == 0)
	{
	  PARAMETER->value = PARAMETER->old_value;
	  strcpy(PARAMETER->units,PARAMETER->old_units);
	}
      else
	{
	  PARAMETER->value = PARAMETER->def_value;
	  strcpy(PARAMETER->units,PARAMETER->def_units);
	}
    }
  else
    {
      PARAMETER->old_value = PARAMETER->value;
      strcpy(PARAMETER->old_units,PARAMETER->units);
      parse_units(ww,&sign,&value,&units);
      if (units == 'p')
	strcpy(PARAMETER->units,"pt");
      else if (units == 'i')
	strcpy(PARAMETER->units,"in");
      else if (units == 'c')
	strcpy(PARAMETER->units,"cm");
      else if (units == 'm')
	strcpy(PARAMETER->units,"em");
      else if (units == 'n')
	{
	  value = .5*value;	/* n is about half the width of m */
	  strcpy(PARAMETER->units,"em");
	}
      else if (units == 'v')
	strcpy(PARAMETER->units,"ex");
      else if (units == 0)
	{
	  if (sign == 0 || PARAMETER->old_units[0] == NULL_C)
	    strcpy(PARAMETER->units,PARAMETER->def_units);
	  else
	    strcpy(PARAMETER->units,PARAMETER->old_units);
	}
      else
	{
	  fprintf(stderr,"unknown units %c, using default units\n");
	  strcpy(PARAMETER->units,PARAMETER->def_units);
	}
      if (sign == 0)	PARAMETER->value = value;
      else		PARAMETER->value = PARAMETER->old_value + sign*value;
    }
}

int
get_string(inbuf,w,rec)		/* get the rest of the line -- Nelson Beebe */
     char *inbuf, *w;
     int rec;			/* rec=1 means recursion is required */
{
  register int c,i,len;
  char ww[MAXLINE];
  register char *start;

  if (*inbuf != '\"')
    return(get_line(inbuf,w,rec));
  start = inbuf;				/* remember start so we can find len */
  i=0;
  inbuf++;				/* point past initial quote */
  while ((c = *inbuf++) != NULL_C && c != '\"' && c != '\n' && i < MAXLINE)
    ww[i++] = (char)c;
  ww[i] = NULL_C;
  if (c != '\n')				/* flush remainder of line */
    while ((c = *inbuf++) != '\n')
      /* NO-OP */;
  len = inbuf - start - 1;		/* count only up to NL, not past */
  if (rec == 1)
    troff_tex(ww,w,0,1);
  else
    strcpy(w,ww);
  return(len);
}

int
get_sub_arg(inbuf,w)		/* get the argument for sub and sup */
     char *inbuf, *w;
{
  int c,len,i;
  char ww[MAXWORD], tmp[MAXWORD];

  len=0;	tmp[0] = NULL_C;
  while ((c = *inbuf) == ' ' || c == '\t')
    {inbuf++;	len++;}
  i=0;
  while ((c = *inbuf++) != NULL_C && c != ' ' && c != '\t' && c != '\n'
	 && c != '$' && c != '}' && c != '~' && i < MAXWORD)
    {ww[i++] = (char)c;	len++;}
  ww[i] = NULL_C;
  if (strcmp(ww,"roman") == 0  || strcmp(ww,"bold") == 0 || strcmp(w,"italic") == 0)
    {
      (void) get_arg(inbuf,tmp,0);
      sprintf(ww,"%s%c%s",ww,c,tmp);
      len += strlen(tmp)+1;
    }
  troff_tex(ww,w,0,1);		/* recursive */
  return(len);
}

int
get_table_entry(inbuf,w,tab)
     char *inbuf, *w;
     int tab;
{
  int c, i=0;

  for (i=0; (c = *inbuf++) != NULL_C && c != tab && i < MAXWORD; i++)
    w[i] = (char)c;
  w[i] = NULL_C;

  return(i);
}

int
get_till_space(inbuf,w)			/* get characters till the next space */
     char *inbuf, *w;
{
  int c,i;

  for (i=0; (c = *inbuf++) != NULL_C && c != ' ' && c != '\n'
	 && c != '\t' && i < MAXWORD; i++)
    w[i] = (char)c;
  w[i] = NULL_C;
  return(i);
}

int
getdef(inbuf,ww)		/* get the define substitution */
     char *inbuf, *ww;
{
  int c,i,len;
  int def_delim;
  char w[MAXWORD];

  def_delim = *inbuf++;		/* take first character as delimiter */
  len=1;		i=0;
  while ((c = *inbuf++) != NULL_C && c != def_delim && i < MAXWORD)
    {len++;		w[i++] = (char)c;}
  w[i] = NULL_C;
  len++;
  if (c != def_delim)
    {
      fprintf(stderr,"WARNING: missing right delimiter in define, define=%s\n",w);
      len--;
    }
  troff_tex(w,ww,0,1);		/* now translate the substitution */
  return(len);
}

int
getword(inbuf,w)		/* get an alphanumeric word (dot also) */
     char *inbuf, *w;
{
  int c,i;

  for (i=0; (c = *inbuf++) != NULL_C
	 && (isalpha(c) || isdigit(c) || c == '.') && i < MAXWORD; i++)
    w[i] = (char)c;
  if (i == 0 && c != NULL_C)
    w[i++] = (char)c;
  w[i] = NULL_C;
  return(i);
}

void
GR_to_Greek(w,ww)			/* change GREEK to Greek */
     char *w, *ww;
{
  *ww++ = '\\';		*ww++ = *w;
  while(*++w != NULL_C)
    *ww++ = tolower(*w);
  *ww = NULL_C;
}

int
is_def(w)		/* check if w was defined by the user */
     char *w;
{
  int i;

  for (i=0; i < def_count; i++)
    {
      if (strcmp(def[i].def_macro,w) == 0)
	return(i);
    }
  return(-1);
}

int
is_flip(w)		/* check if w is in the flip list */
     char *w;
{
  int i;

  for (i=0; i < flip_count; i++)
    {
      if (strcmp(flip_list[i],w) == 0)
	return(i);
    }
  return(-1);
}

int
is_forbid(w)		/* check if w is one of those sacred macros */
     char *w;
{
  int i;

  for (i=0; i < forbd_count; i++)
    {
      if (strcmp(forbid[i],w) == 0)
	return(i);
    }
  return(-1);
}

int
is_mathcom(w,ww)	/* check if w has a simple correspondence in TeX */
     char *w,*ww;
{
  int i;

  for (i=0; i < mathcom_count; i++)
    {
      if (strcmp(math[i].troff_symb,w) == 0)
	{
	  strcpy(ww,math[i].tex_symb);
	  return(i);
	}
    }
  return(-1);
}

int
is_mydef(w)		/* check if w is user-defined macro */
     char *w;
{
  int i;

  for (i=0; i < mydef_count; i++)
    {
      if (strcmp(mydef[i].def_macro,w) == 0)
	return(i);
    }
  return(-1);
}

int
is_troff_mac(w,ww,arg,par)/* check if w is a macro or plain troff command */
     char *w,*ww;
     int *arg,*par;
{
  int i;

  for (i=0; i < macro_count; i++)
    {
      if (strcmp(macro[i].troff_mac,w) == 0)
	{
	  strcpy(ww,macro[i].tex_mac);
	  *arg = macro[i].arg;
	  *par = macro[i].macpar;
	  return(i);
	}
    }
  return(-1);
}

void
parse_units(ww,sign,value,units)
     char *ww;
     int *sign, *units;
     float *value;
{
  int len, k=0, i;
  char tmp[MAXWORD];

  len = strlen(ww);
  if (ww[0] == '-')	*sign = -1;
  else if (ww[0] == '+')	*sign = 1;
  if (*sign != 0)		k++;

  i=0;
  while (k < len)
    {
      if (isdigit(ww[k]) || ww[k] == '.')
	tmp[i++] = ww[k++];
      else	break;
    }
  tmp[i] = NULL_C;
  sscanf(tmp,"%f",value);
  i=0;
  if (k < len)
    {
      *units = ww[k++];
      if (k < len)
	fprintf(stderr,
		"Suspect problem in parsing %s, unit used is %c\n",ww,*units);
    }
}

void
scrbuf(in,out)			/* copy input to output */
     FILE *in,*out;
{
  int c;
  while ((c =getc(in)) != EOF)	putc(c,out);
}

int
similar(w)			/* check if w is in the similar list */
     char *w;
{
  int i;

  for (i=0; i < sim_count ; i++)
    {
      if (strcmp(sim_list[i],w) == 0)
	return(1);
    }
  return(-1);
}

char *
skip_line(inbuf)		/* ignore the rest of the line */
     char *inbuf;
{
  while (*inbuf != '\n' && *inbuf != NULL_C)
    inbuf++;
  if (*inbuf == NULL_C)	return(inbuf);
  else			return(++inbuf);
}

int
skip_white(inbuf)		/* skip white space */
     char *inbuf;
{
  int c,len=0;

  while ((c = *inbuf++) == ' ' || c == '\t' || c == '\n')
    len++;	
  return(len);
}

char *
strapp(s,tail)  /* copy tail[] to s[], return ptr to terminal NULL_C in s[] */
     register char *s;	/* Nelson Beebe */
     register char *tail;
{
  while (*s++ = *tail++)
    /*NO-OP*/;
  return (s-1);			/* pointer to NULL_C at end of s[] */
}

void
tmpbuf(in,buffer)
     /* copy input to buffer, buffer holds only MAXLEN characters */
     FILE *in;
     char *buffer;
{
  int c;
  unsigned int l=0;

  while (l++ < MAXLEN && (c = getc(in)) != EOF)
    *buffer++ = (char)c;
  if (l >= MAXLEN)
    {
      fprintf(stderr,"Sorry: document is too large\n");
      exit(-1);
    }
  *buffer = NULL_C;
}
