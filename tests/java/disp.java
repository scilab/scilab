import javasci.* ; 


class  disp 
{

  public static void main(String[] args) 
	{
    SciDoubleArray A = new SciDoubleArray("A",4,1, new double [] {100.1,102.2,103.3,104.4} );
		A.Send();
		Scilab.Exec("disp('A=');disp(A)");
	}
}

