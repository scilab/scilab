import javasci.* ; 
/********************************************************************************************************/      
class String1 
{
/********************************************************************************************************/  	
  public static void main(String[] args) 
  {
		SciString S1 = new SciString("Sentence1","Who is the best ?");
		SciString S2 = new SciString("Sentence2","I have a dream ...");
		SciString S3 = new SciString("Sentence3");
    S1.Send();
    S2.Send();
    
    S1.disp();
    S2.disp();
    
    Scilab.Exec("Sentence1=''You are the best !'';");
    
    Scilab.Exec("disp(Sentence1);");
    Scilab.Exec("disp(Sentence2);");
    Scilab.Exec("Sentence3=Sentence1+'' ''+Sentence2;");
    
    S3.Get();
    S3.disp();
    

  }
/********************************************************************************************************/      
}
/********************************************************************************************************/  
