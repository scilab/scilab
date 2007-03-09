import javasci.* ; 

/**
 * Test the functions disp and clf
 * @author INRIA
 */
class Clf 
{

  public static void main(String[] args) 
	{
		Scilab.Exec("disp('Test clf 1');");
		Scilab.Exec("clf();");
		Scilab.Exec("disp('Test clf 2');");
	}
}

