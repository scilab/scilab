import javasci.* ; 
/********************************************************************************************************/      
class Real2 
{
/********************************************************************************************************/  	
public static void main(String[] args) 
{
  SciReal F = new SciReal("F",2,2, new double [] {1,2,3,4} );
  SciReal G = new SciReal("G",2,2, new double [] {5,6,7,8} );
  SciReal DimH = new SciReal("DimH",1,2);
      
  F.Send();
  G.Send();

	Scilab.Exec("disp(''F='');disp(F);");
	Scilab.Exec("disp(''G='');disp(G);");
      
  Scilab.Exec("H=F*G;");
  DimH.Job("DimH=size(H)");
     
  double DxDy[]=DimH.getData();
  SciReal H = new SciReal("H",(int)DxDy[0],(int)DxDy[1]);

  H.Get();
  Scilab.Exec("disp(''H='');disp(H);");

}
/********************************************************************************************************/      
}
/********************************************************************************************************/  
