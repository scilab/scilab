/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2009 - Allan CORNET 
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

import java.awt.*;
import javasci.*; 


/**
 * @deprecated
 */
public class MonPanneau extends Panel
{
    private Label lab;
    private Button bout;
    private int nbrDeClic;
    private TextField EditBox;
    
    class LoopEvent extends Thread {
      boolean continueLoop = true;
      public void run() {
       if (Scilab.HaveAGraph() == false){
         Scilab.Exec("plot3d();");
         do {
           Scilab.Events();
         }
         while ( (Scilab.HaveAGraph() != false) && (continueLoop == true) );
         this.stop();
       }
    }

    public void close() {
      continueLoop = false;
    }
  }
  
  public MonPanneau() {
    nbrDeClic=0;
    lab = new Label("number of click " + nbrDeClic);
    bout = new Button("Click here");
    EditBox = new TextField("Hello",10);

    this.setLayout(new GridLayout(3,1));
    this.add(lab);
    this.add(bout);
    this.add(EditBox);

    // Ecoute d'évènement
    bout.addActionListener(new MonEcouteurPourBouton(this));

    // NEW 
    EditBox.addTextListener(new MonEcouteurPourText(this));
  }
  
  public void YA_EU_UN_Click()
  {
    int i=0;
    nbrDeClic++;
    lab.setText("number of click " + nbrDeClic);
    LoopEvent thread = new LoopEvent();
    thread.start();
  }
  
  public void recupTexte()
  {
    String textelu;
    textelu = EditBox.getText();
    System.out.println("my text is = " + textelu);	
  }
}

