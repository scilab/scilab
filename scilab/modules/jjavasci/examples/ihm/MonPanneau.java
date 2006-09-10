import java.awt.*;
import javasci.*; 
					 
public class MonPanneau extends Panel
{
    private Label lab;
    private Button bout;
    private int nbrDeClic;
    private TextField EditBox;
	
    public MonPanneau()
    {
	nbrDeClic=0;
	lab = new Label("le nbr de clic egale "+nbrDeClic);
	bout = new Button("Clic ici");
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
	lab.setText("le nbr de clic egale "+nbrDeClic);
	Scilab.Exec("plot2d();");
	System.out.println("Avant Scilab.HaveAGraph = " + Scilab.HaveAGraph());
   
	do
	    {
		Scilab.Events();
		try
		    {
			Thread.sleep( 1 );
		    }
		catch ( InterruptedException f )
		    {
		    }

		System.out.println("Boucle Java sans fin "+i);
		i++;
	    }
	while (Scilab.HaveAGraph()!=false);

	Scilab.Events();

	System.out.println("Apres Scilab.HaveAGraph");
		
    }
    public void recupTexte()
    {
	String textelu;
	textelu = EditBox.getText();
	System.out.println("mon texte est = " + textelu);	
    }
}

