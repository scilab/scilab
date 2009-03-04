/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import java.awt.event.*;
import javasci.*;
import java.awt.*;

public class MonEcouteurPourBouton implements ActionListener
{
  private MonPanneau appli;
  public MonEcouteurPourBouton(MonPanneau qui)
  {
    appli=qui;	
  }
  public void actionPerformed(ActionEvent e)
  {
    int i=0;
    appli.YA_EU_UN_Click();
    System.out.println(e);
  }
}



	
