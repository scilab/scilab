import javasci.* ; 

class  Exemple3 {

  public static void main(String[] args) {
        
  SciReal a = new SciReal("A",4,1, new double [] {1,2,3,4} );
  SciReal b = new SciReal("B",4,1, new double [] {3,1,2,4} );
  SciReal c = new SciReal("C",4,1, new double [] {0,0,0,0} );
  
  a.Send();
  b.Send();
  c.Send();
  
  Scilab.Exec("disp(A);");
  Scilab.Exec("disp(B);");
  Scilab.Exec("disp(C);");
  
  a.disp();
  b.disp();
  c.disp();
    
  Scilab.Exec("C=A+B;");
  a.Get();
  b.Get();
  c.Get();
  
  a.disp();
  b.disp();
  c.disp();
  
	}
}
  
