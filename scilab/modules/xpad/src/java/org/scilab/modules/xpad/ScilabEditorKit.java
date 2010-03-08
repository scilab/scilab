/*
 */
package org.scilab.modules.xpad;
import javax.swing.text.StyledEditorKit;



 public class ScilabEditorKit extends StyledEditorKit {

    public ScilabEditorKit() {
    	super();
    }


    // --- EditorKit methods -------------------------

    /**
     * Get the MIME type of the data that this
     * kit represents support for.  This kit supports
     * the type <code>text/java</code>.
     */
    public String getContentType() {
	return "text/scilab";
    }

    /**
     * Create a copy of the editor kit.  This
     * allows an implementation to serve as a prototype
     * for others, so that they can be quickly created.
     */
    public Object clone() {
	ScilabEditorKit kit = new ScilabEditorKit();
	return kit;
    }

    /**
     * Creates an uninitialized text storage model
     * that is appropriate for this type of editor.
     *
     * @return the model
     */
    public javax.swing.text.Document createDefaultDocument() {
    	javax.swing.text.Document doc = new org.scilab.modules.xpad.style.ScilabStyleDocument();
    	return doc;
    }
    /**
     * Fetches a factory that is suitable for producing 
     * views of any models that are produced by this
     * kit.  The default is to have the UI produce the
     * factory, so this method has no implementation.
     *
     * @return the view factory
     */
    /*
     *
    public ScilabContext getStylePreferences() {
	if (preferences == null) {
	    preferences = new ScilabContext();
	}
	return preferences;
    }

    public void setStylePreferences(ScilabContext prefs) {
	preferences = prefs;
    }
    public final ViewFactory getViewFactory() {
	return getStylePreferences();
    }

    ScilabContext preferences;
    */
}







