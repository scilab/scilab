import org.eclipse.swt.SWT;
import org.eclipse.swt.events.*;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.*;


class SciGUIConsole
{  
	private Display display;
  private Shell shell;
  private Text textouput;
  private Text textinput;
  private boolean IsEnabled;
/*---------------------------------------------------------------------------------------------*/    
	SciGUIConsole()
	{
		IsEnabled=false;
	}
/*---------------------------------------------------------------------------------------------*/    
public void Initialize()
{
	display = new Display( );
  shell = new Shell(display);
  shell.pack ();
  shell.setSize(620,520);
  shell.setText("Scilab 5.0");
  
  textouput = new Text(shell, SWT.MULTI | SWT.V_SCROLL |   SWT.H_SCROLL | SWT.BORDER | SWT.WRAP |SWT.READ_ONLY);
  textouput.setBounds(10,10,600,230);
  
  textinput = new Text(shell, SWT.MULTI | SWT.V_SCROLL |   SWT.H_SCROLL | SWT.BORDER);
  textinput.setBounds(10,275,600,200);
  
  textinput.addTraverseListener(new TraverseListener() {
		public void keyTraversed(TraverseEvent e) {
			if (e.detail == SWT.TRAVERSE_RETURN ) 
			{
				System.out.println (textinput.getText());
				textinput.setText("");
				System.out.println ("number lines: "+textinput.getLineCount());
				
			}
		}
	});
  
	shell.open( );
	IsEnabled=true;
		

	
}
/*---------------------------------------------------------------------------------------------*/  
public void EventsLoop()
{
	while(!shell.isDisposed( ))
  {
  	if(!display.readAndDispatch( )) display.sleep( );
  }
  dispose();
}
/*---------------------------------------------------------------------------------------------*/
public void dispose()
{
	display.dispose( );
}
/*---------------------------------------------------------------------------------------------*/
public boolean IsEnabled()
{
	return IsEnabled;
}
/*---------------------------------------------------------------------------------------------*/
public void PutString(final String Str)
{
	display.syncExec (new Runnable () 
	{
		public void run ()
		{
			textouput.append(Str);
		}
	}); 
}
/*---------------------------------------------------------------------------------------------*/

}