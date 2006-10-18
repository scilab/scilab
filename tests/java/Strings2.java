import javasci.*;

public class Strings2
{
  public static void main(String[] args)
  {
    String[] s=new String[1000];
    char[] ch=new char[3100];
    int r,c,k;
    k=0;
    for(int i=0;i<s.length;i++)
    {
        for(int j=0;j<ch.length;j++)
        {
          	if (k == 0)
          	{
          		 ch[j]='P';
          		 k++;
          	}
          	else if (k == 1)
          	{
          		ch[j]='S';
          		k++;
          	}
          	else
          	{
          		ch[j]='A';
          		k=0;
          	}
             
        }
        s[i]=new String(ch);
    }
    
    SciStringArray sSA=new SciStringArray("A",s.length,1,s);
    //sSA.disp();
    Scilab.Exec("Sa=size(A);");
    Scilab.Exec("disp(Sa);");
    
    SciStringArray ssB = new SciStringArray("B",s.length,1);
    Scilab.Exec("B=A+string([1:"+s.length+"]);");
        
  	ssB.Get();
  	//ssB.disp();
	
  	
  	Scilab.Exec("Sb=size(B);");
    Scilab.Exec("disp(Sb);");
    // 1 1000 taille matrice de chaines
    
    Scilab.Exec("len=length(B(Sb(2)));");
    Scilab.Exec("disp(len);");
    // 3104 caracteres
    
    Scilab.Exec("TEST=part(B(Sb(2)),len-7:len);");
    // PSAP1000
    Scilab.Exec("disp(TEST);");
    
  }
}
