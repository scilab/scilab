package org.scilab.modules.xpad;

import javax.swing.undo.AbstractUndoableEdit;
import javax.swing.undo.UndoableEdit;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.text.JTextComponent;
public class CaretEdit extends javax.swing.undo.AbstractUndoableEdit
{
  private int mark;
  private int dot;
  private JTextComponent textComponent;
  
  // save current caret position to be restored with undo.
  public CaretEdit(JTextComponent t)
  {
	  javax.swing.text.Caret c = t.getCaret();  
	  mark = c.getMark();
	  dot = c.getDot();
	  textComponent = t;
  }
  public boolean isSignificant()
  {
    return true;
  }
  public String getPresentationName()
  {
    return "caret move";
  }
  public void undo() throws CannotUndoException
  {
	    super.undo();
	    try
	    {
	    	System.err.println("undoing caret move going to mark:"+mark+" dot:"+dot);
	    	updateCaret();
	    }
	    catch (Exception ex)
	    {
	      throw new CannotUndoException();
	    }
	  }
  public void redo() throws CannotRedoException
  {
	    super.redo();
	    try
	    {
	    	System.err.println("redoing caret move going to mark:"+mark+" dot:"+dot);
	    	updateCaret();
	    }
	    catch (Exception ex)
	    {
	      throw new CannotUndoException();
	    }
	  }

  void updateCaret() throws Exception
  {
	  
  	int nextMark= textComponent.getCaret().getMark();
  	int nextDot= textComponent.getCaret().getDot();
  	textComponent.setCaretPosition(mark);
  	textComponent.moveCaretPosition(dot);
  	textComponent.setSelectionStart(mark);
  	textComponent.setSelectionEnd(dot);
  	mark = nextMark;
  	dot = nextDot;
  }
  
  public boolean addEdit(UndoableEdit edit)
	  {
	    if (edit instanceof CaretEdit)// we can pretent to store CaretEdit events and just let them die
	    {
	      edit.die();
	      return true;
	    }
	    else
	    {
	      return false;
	    }
	  }
	}