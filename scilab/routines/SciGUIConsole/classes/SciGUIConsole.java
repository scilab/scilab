import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.*;


class SciGUIConsole
{  
	Display d;
  Shell s;
  Text text1;
/*---------------------------------------------------------------------------------------------*/    
	SciGUIConsole()
	{
	}
/*---------------------------------------------------------------------------------------------*/    
  public void Initialize()
  {
   d = new Display( );
   s = new Shell(d);
   s.setSize(640,400);
   s.setText("Scilab 5.0");
   text1 = new Text(s, SWT.MULTI | SWT.V_SCROLL |   SWT.H_SCROLL | SWT.WRAP | SWT.BORDER);
   text1.setBounds(10,10,600,350);
	 text1.append("Input 1");
	 DispString("RunGUI");
  }
/*---------------------------------------------------------------------------------------------*/  
  public void EventsLoop()
  {
  	 s.open( );
  	DispString(" Start LoopGUI");
    while(!s.isDisposed( ))
    {
     if(!d.readAndDispatch( )) d.sleep( );
    }
    d.dispose( );
    DispString(" Exit LoopGUI");
  }
/*---------------------------------------------------------------------------------------------*/
  public void PutString(String[] args)
  {
  	final String line=args[0];
  	
  	d.asyncExec (new Runnable () 
  	{
      public void run ()
      {
      	text1.append(line);
      }
    }); 
  	 System.out.println(args[0]);
  }
/*---------------------------------------------------------------------------------------------*/
     
  public static void DispString(String InputStr)
  {
  	System.out.println(InputStr);
  }
 /*---------------------------------------------------------------------------------------------*/      
}