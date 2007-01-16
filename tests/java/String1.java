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
    
    System.out.println("S1 : "+ S1.getName()+" = "+S1.getData());
    System.out.println("S2 : "+ S2.getName()+" = "+S2.getData());
    
    Scilab.Exec("Sentence1='You are the best !';");
    Scilab.Exec("Sentence3=Sentence1+' '+Sentence2;");
    
    System.out.println("S3 : "+ S3.getName()+" = "+S3.getData());
    

  }
/********************************************************************************************************/      
}
/********************************************************************************************************/  
