/* setup file */

#ifndef NO_SGTTY
#define HAVE_SGTTY 1			/* host has sgtty.h and ioctl.h */
#endif

#include        <stdio.h>
#include        <ctype.h>
#ifdef SYSV
#include        <string.h>
#include	<stdlib.h>	/* for type declarations */
#else
#ifdef MSC
#include        <string.h>
#include	<stdlib.h>	/* for type declarations */
#include	<io.h>		/* for type declarations */
#else
#include        <strings.h>
#endif
#endif

#if HAVE_SGTTY
#include        <sys/ioctl.h>
#include        <sgtty.h>
#endif

#define	MAXLEN	65535		/* maximum length of document */
#define	MAXWORD	2500		/* maximum word length */
#define	MAXLINE	500		/* maximum line length */
#define	MAXDEF	200		/* maximum number of defines */

#include <malloc.h>
#ifdef IN_TR		/* can only declare globals once */
#else
extern
#endif
int math_mode,		/* math mode status */
    de_arg,		/* .de argument */
    IP_stat,		/* IP status */
    QP_stat,		/* QP status */
    TP_stat;		/* TP status */

#ifdef IN_TR		/* can only declare globals once */
#else
extern
#endif
struct defs {
	char *def_macro;
	char *replace;
	int illegal;
} def[MAXDEF];

#ifdef IN_TR		/* can only declare globals once */
#else
extern
#endif
struct mydefs {
	char *def_macro;
	char *replace;
	int illegal;
	int arg_no;
	int par;		/* if it impiles (or contains) a par break */
} mydef[MAXDEF];

#ifdef IN_TR		/* can only declare globals once */
#else
extern
#endif
struct measure {
	char old_units[MAXWORD];	float old_value;
	char units[MAXWORD];		float value;
	char def_units[MAXWORD];	/* default units */
	int def_value;			/* default value: 0 means take last one */
} linespacing, indent, tmpind, space, vspace;

#ifdef ANSI
char*	alternate(char*, char*, char*);
int	CAP_GREEK(char*);
char*	do_table(char*,char*,int);
char*	end_env(char*);
void	envoke_stat(int);
char*	flip(char*,char*);
char*	flip_twice(char*,char*,char*);
int	get_arg(char*,char*,int ,int );
void	get_brace_arg(char*,char*);
int	get_defword(char*,char*,int*);
int	get_line(char*,char*,int );
int	get_multi_line(char*,char*);
int	get_mydef(char*,char*);
int	get_N_lines(char *,char *,int);
int	get_no_math(char*,char*);
char*	get_over_arg(char*,char*);
int	get_ref(char*,char*);
int	get_string(char*,char*,int );
void	get_size(char*,char*,char*);
int	get_sub_arg(char*,char*);
int	get_till_space(char*,char*);
int	getdef(char*,char*);
int	getword(char*,char*);
void	GR_to_Greek(char*,char*);
int	is_def(char*);
int	is_flip(char*);
int	is_forbid(char*);
int	is_mathcom(char*,char*);
int	is_mydef(char *);
int	is_troff_mac(char*,char*,int*);
int	main(int ,char **);
void	parse_units(char*,int,int,int);
void	scrbuf(FILE*,FILE*);
int	similar(char*);
char*	skip_line(char*);
int	skip_white(char*);
char*	strapp(char*,char*);
void	tmpbuf(FILE*,char*);
void	troff_tex(char *,char *,int );
#else
char*	alternate();
int	CAP_GREEK();
char*	do_table();
char*	end_env();
void	envoke_stat();
char*	flip();
char*	flip_twice();
int	get_arg();
void	get_brace_arg();
int	get_defword();
int	get_line();
int	get_multi_line();
int	get_mydef();
int	get_N_lines();
int	get_no_math();
char*	get_over_arg();
int	get_ref();
int	get_string();
void	get_size();
int	get_till_space();
int	get_sub_arg();
int	getdef();
int	getword();
void	GR_to_Greek();
int	is_def();
int	is_flip();
int	is_forbid();
int	is_mathcom();
int	is_mydef();
int	is_troff_mac();
int	main();
void	parse_units();
void	scrbuf();
int	similar();
char*	skip_line();
int	skip_white();
char*	strapp();
void	tmpbuf();
void	troff_tex();
#endif
