import javasci.* ; 
/********************************************************************************************************/      
class Real2 
{
/********************************************************************************************************/  	
public static void main(String[] args) 
{
  SciDoubleArray F = new SciDoubleArray("F",2,2, new double [] {1,2,3,4} );
  SciDoubleArray G = new SciDoubleArray("G",2,2, new double [] {5,6,7,8} );
  SciDoubleArray DimH = new SciDoubleArray("DimH",1,2);
      
  F.Send();
  G.Send();

	Scilab.Exec("disp(''F='');disp(F);");
	Scilab.Exec("disp(''G='');disp(G);");
  
  Scilab.Exec("H=F*G;");
  DimH.Job("DimH=size(H)");
      
  double DxDy[]=DimH.getData();
  
  SciDoubleArray H = new SciDoubleArray("H",(int)DxDy[0],(int)DxDy[1]);
  Scilab.Exec("H=F*G;");
  
  H.Get();
  
  Scilab.Exec("disp(''H='');disp(H);");

}
/********************************************************************************************************/      
}
/********************************************************************************************************/  
