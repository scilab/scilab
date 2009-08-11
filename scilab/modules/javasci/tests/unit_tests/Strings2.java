/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
    // to allocate more memory for strings (1000 strings of 3100 characters)
    Scilab.Exec("stacksize('max');");
    
    SciStringArray sSA=new SciStringArray("A",1,s.length,s);
    Scilab.Exec("Sa=size(A);");
    Scilab.Exec("disp(Sa);");

    SciStringArray ssB = new SciStringArray("B",1,s.length);
    Scilab.Exec("B=A+string([1:"+s.length+"]);");

  	ssB.Get();

  	Scilab.Exec("Sb=size(B);");
    Scilab.Exec("disp(Sb);");
    // 1 1000 taille matrice de chaines
    
    Scilab.Exec("len=length(B(Sb(2)));");
    Scilab.Exec("disp(len);");
    // 3104 caracteres
    
    Scilab.Exec("TEST=part(B(Sb(2)),len-7:len);");
    // PSAP1000
    Scilab.Exec("disp(TEST);");
    Scilab.Finish();
    
  }
}
