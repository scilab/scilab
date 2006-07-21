package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
/**
* Cette classe permet de gérer les exceptions dans les constructeurs SciDoubleArray , SciStringArray 
*/
/********************************************************************************************************/
public class BadDataArgumentException  extends RuntimeException 
{
  BadDataArgumentException(String s) 
  {
    super(s);
  }
}
/********************************************************************************************************/
