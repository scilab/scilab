/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import java.awt.event.*;


/**
 * @deprecated
 */
public class MonEcouteurPourText implements TextListener
{
  private MonPanneau appli;
  public MonEcouteurPourText(MonPanneau qui)
  {
    appli=qui;	
  }
  public void textValueChanged(TextEvent e)
  {
    appli.recupTexte();	
    System.out.println(e);
  }
}
