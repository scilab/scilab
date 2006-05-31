import javasci.* ; 


class  Exemple1 {

  public static void main(String[] args) {
  	int col=4;
  	int row=2;
  	int r,c;
  	double [] b=new double[row*col];
  	
  	SciDoubleArray a = new SciDoubleArray("A",row,col);
  	Scilab.Exec("A=[1,2,3,4;5 6 7 8];");
  	a.Get();
  	System.out.println("----------------------------------------------------");
  	a.disp();
  	System.out.println("----------------------------------------------------");
  	for(r=1;r<=a.getRow();r++)
    {
     for(c=1;c<=a.getCol();c++)
     {
     	System.out.print("A("+(r)+","+(c)+")="+a.GetElement(r,c)+"  ");
     }
     System.out.println(" ");
    }
  	System.out.println("----------------------------------------------------");
  	
  	b=a.getData();
  	
  	for(r=0;r<row;r++)
    {
     for(c=0;c<col;c++)
     {
     	System.out.print("b("+r+","+(c)+")="+ b[c*row+r] +"  ");
     }
     System.out.println(" ");
    }
  	System.out.println("----------------------------------------------------");
 	  SciDoubleArray d = new SciDoubleArray("D",1,4,new double [] {1,2,3,4});
 	  SciDoubleArray q = new SciDoubleArray("Q",3,1, new double [] {0,0,0} );

    Scilab.Exec("Str=''Scilab'';");
    Scilab.Exec("P=poly(D'',''x'',''coeff'');");
    Scilab.Exec("Q=real(roots(P));");

		System.out.println("----------------------------------------------------");
		
    System.out.println("----------------------------------------------------");   
    Scilab.Exec("disp(''Str : ''+Str);");
		System.out.println("Q ScilabType : "+Scilab.TypeVar("Q"));
		System.out.println("Str ScilabType : "+Scilab.TypeVar("Str"));
		System.out.println("----------------------------------------------------");
    q.Get();
    q.disp();
    System.out.println("----------------------------------------------------");
    Scilab.Finish();
  }
}

