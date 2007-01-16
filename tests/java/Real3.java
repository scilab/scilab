import javasci.* ; 
/********************************************************************************************************/      
class Real3 
{
/********************************************************************************************************/  	
	private static void DisplayResult(String name,double []mat,int r,int c)
	{
		int i=0;
		
		System.out.println("row= "+r);
		System.out.println("col= "+c);
		System.out.println(name+"= ");
		for(i=0;i<r;i++)
		{
			System.out.print(mat[i]+" ");
		}
		System.out.println(" ");
	}
	
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
  		
  		Scilab.Exec("C=A+B;");
  		D.Job("Dim=size(C);");
			
			DisplayResult(A.getName(),A.getData(),A.getRow(),A.getCol());
			DisplayResult(B.getName(),B.getData(),B.getRow(),B.getCol());
			DisplayResult(C.getName(),C.getData(),C.getRow(),C.getCol());
			DisplayResult(E.getName(),E.getData(),E.getRow(),E.getCol());

    }
/********************************************************************************************************/      
}
/********************************************************************************************************/  
