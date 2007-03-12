package javasci ;
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
/**
 * Thanks to this class, exceptions can be managed in the constructor of SciDoubleArray , SciStringArray 
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
