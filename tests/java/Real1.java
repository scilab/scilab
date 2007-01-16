import javasci.* ; 
/********************************************************************************************************/      
class Real1 
{
/********************************************************************************************************/  	
  public static void main(String[] args) 
    {
			SciDoubleArray A = new SciDoubleArray("A",4,1, new double [] {1.1,2.2,3.3,4.4} );
			A.Send();
      SciDoubleArray B = new SciDoubleArray("B",4,1, new double [] {4.4,3.3,2.2,1.1} );
      B.Send();
  		SciDoubleArray C = new SciDoubleArray("C",4,1, new double [] {1. ,1. ,1. ,1.} );
  		C.Send();
  		SciDoubleArray D = new SciDoubleArray("Dim",1,2);
  		D.Send();
  		SciDoubleArray E = new SciDoubleArray("E",C);
  		E.Send();
  		
  		
  		Scilab.Exec("disp('A=');disp(A);");
  		Scilab.Exec("disp('B=');disp(B);");
			Scilab.Exec("disp('C=');disp(C);");
			Scilab.Exec("disp('E=');disp(E);");
			
  		Scilab.Exec("C=A+B;");
  		D.Job("Dim=size(C);");
			
			Scilab.Exec("disp('A=');disp(A);");
  		Scilab.Exec("disp('B=');disp(B);");
			Scilab.Exec("disp('C=');disp(C);");
			Scilab.Exec("disp('Dim=');disp(Dim);");
			Scilab.Exec("disp('E=');disp(E);");

    }
/********************************************************************************************************/      
}
/********************************************************************************************************/  
