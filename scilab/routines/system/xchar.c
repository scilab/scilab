
#include "../stack-c.h" 

/*-----------------------------------------------------
 * routine systeme dependente  pour caracteres speciaux 
 * copyright Inria/Enpc 
 *------------------------------------------------------*/

int C2F(xchar)(unsigned char *line,int * k,unsigned int line_len)
{
  static integer blank = 40, eof = 99;
  switch ( *line ) 
    {
    case 0 : 
      /*     prise en compte de la marque de fin de chaine C */
      /*     dans le cas d'un appel de scilab par un programme C */
      *k = eof;
      break; 
    case 9 : /*     tab remplace par un blanc */
      *k = blank + 1;
      break; 
    case 10 : /*     \n remplace par un eol */
      *k = eof;
      break; 
    default :
      *k = eof + 1 + *line + 1;
    }
  return 0;
} 



