package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;
import java.util.Enumeration;
import java.util.Hashtable;
import javax.swing.filechooser.FileFilter;

/** Implementation pratique de FileFilter filtrant tout les
 * fichiers a l'exception de ceux dont les extentions sont connues.
 * Les extentions sont du type ".foo", typiquement utilises sous
 * Windows et Unix, mais pas sur Macinthosh. Dans ce cas, les extentions sont
 * ignorees.
 * Example - cree un nouveau filtre filtrant tout les fichiers sauf
 * les gif et jpg, fichiers image:
 *
 * JFileChooser chooser = new JFileChooser();
 *
 * FileMasque filter = new FileMasque(new String{"gif", "jpg"}, "Images JPEG & GIF") chooser.addChoosableFileFilter(filter);
 * chooser.showOpenDialog(this);
 *
 * @version 1.16 07/26/04
 * @author Jeff Dinkins
 */
public class FileMasque extends FileFilter
{
	private static String TYPE_UNKNOWN = "Type Unknown";
	private static String HIDDEN_FILE = "Hidden File";
	private Hashtable filters = null;
	private String description = null;
	private String fullDescription = null;
	private boolean useExtensionsInDescription = true;	
	
	/**
	 * Cree un filtre fichier. Si aucun filtre n'est ajouter,
	 * affiche tous les fichiers sans restriction.
	 *
	 * @see addExtension
	 */
	public FileMasque()
	{
		this.filters = new Hashtable();
	}
	/**
	 * Cree un filtre fichier filtrant tout les fichier dont l'extention
	 * n'est pas specifiee.
	 * Example: new ExampleFileFilter("jpg");
	 *
	 * @see addExtension
	 */
	public FileMasque(String extension)
	{
		this(extension,null);
	}
	/**
	 * Creates a file filter that accepts the given file type.
	 * Example: new ExampleFileFilter("jpg", "JPEG Image Images");
	 *
	 * Note that the "." before the extension is not needed. If
	 * provided, it will be ignored.
	 *
	 * @see addExtension
	 */
	public FileMasque(String extension, String description)
	{
		this();
		if(extension!=null) addExtension(extension);
		if(description!=null) setDescription(description);
	}
	/**
	 * Creates a file filter from the given string array.
	 * Example: new ExampleFileFilter(String {"gif", "jpg"});
	 *
	 * Note that the "." before the extension is not needed adn
	 * will be ignored.
	 *
	 * @see addExtension
	 */
	public FileMasque(String[] filters)
	{
		this(filters, null);
	}
	/**
	 * Cree un filtre fichier a partir du tableau de String donne et de la description.
	 * Example: new ExampleFileFilter(String {"gif", "jpg"}, "Gif and JPG Images");
	 *
	 * Noter que le "." avant l'extention n'est pas requis et sera ignore.
	 *
	 * @see addExtension
	 */
	public FileMasque(String[] filters, String description)
	{
		this();
		for (int i = 0; i < filters.length; i++)
		{
			addExtension(filters[i]);// Ajoute les filtres un a un
		}
		if(description!=null) setDescription(description);
	}
	/**
	 * Retourne true si le fichier doit etre montre dans le repertoire,
	 * false s'il ne doit pas l'etre.
	 *
	 * Les fichier commencant par "." sont ignores.
	 *
	 * @see getExtension
	 * @see FileFilteraccepts
	 */
	public boolean accept(File f)
	{
		if(f != null)
		{
			if(f.isDirectory() || filters.size()==0)
			{
				return true;
			}
			String extension = getExtension(f);
			if(extension != null && filters.get(getExtension(f)) != null)
			{
				return true;
			};
		}
		return false;
	}
	/**
	 * Retourne l'extention du nom du fichier.
	 *
	 * @see getExtension
	 * @see FileFilteraccept
	 */
	public String getExtension(File f)
	{
		if(f != null)
		{
			String filename = f.getName();
			int i = filename.lastIndexOf('.');
			if(i>0 && i<filename.length()-1)
			{
				return filename.substring(i+1).toLowerCase();
			};
		}
		return null;
	}
	/**
	 * Adds a filetype "dot" extension to filter against.
	 *
	 * Par example: le code suivant crera un filtre qui filtrera
	 * tout les fichier a l'exception des ".jpg" et ".tif":
	 *
	 * FileMasque filter = new FileMasque();
	 * filter.addExtension("jpg");
	 * filter.addExtension("tif");
	 *
	 * Noter que le "." avant les extention n'est pas requis et sera ignore.
	 */
	public void addExtension(String extension)
	{
		if(filters == null)
		{
			filters = new Hashtable(5);
		}
		filters.put(extension.toLowerCase(), this);
		fullDescription = null;
	}
	/**
	 * Retourne une representation humainement lisible de la description de ce filtre. Par
	 * example: "JPEG and GIF Image Files (*.jpg, *.gif)"
	 *
	 * @see setDescription
	 * @see setExtensionListInDescription
	 * @see isExtensionListInDescription
	 * @see FileFiltergetDescription
	 */
	public String getDescription()
	{
		if(fullDescription == null)
		{
			if(description == null || isExtensionListInDescription())
			{
				fullDescription = description==null ? "(" : description + " ("; // Construit la description a partir des extentions.
				Enumeration extensions = filters.keys();
				if(extensions != null)
				{
					fullDescription += "." + (String) extensions.nextElement();
					while (extensions.hasMoreElements())
					{
						fullDescription += ", ." + (String) extensions.nextElement();
					}
				}
				fullDescription += ")";
			}
			else
			{
				fullDescription = description;
			}
		}
		return fullDescription;
	}
	/**
	 * Definit la description du filtre. Par
	 * example: filter.setDescription("Gif and JPG Images");
	 *
	 * @see setDescription
	 * @see setExtensionListInDescription
	 * @see isExtensionListInDescription
	 */
	public void setDescription(String description)
	{
		this.description = description;
		fullDescription = null;
	}
	/**
	 * Determine quelque soit la liste d'extension (.jpg, .gif, etc) s'il
	 * doit afficher la description des filtres.
	 *
	 * N'est approprie que si la description a ete donee dans le constructeur
	 * ou par l'utilisation de setDescription();
	 *
	 * @see getDescription
	 * @see setDescription
	 * @see isExtensionListInDescription
	 */
	public void setExtensionListInDescription(boolean b)
	{
		useExtensionsInDescription = b;
		fullDescription = null;
	}
	/**
	 * Retourne quelque soit la liste d'extension (.jpg, .gif, etc) s'il
	 * doit afficher la description des filtres.
	 *
	 * N'est approprie que si la description a ete donee dans le constructeur
	 * ou par l'utilisation de setDescription();
	 *
	 * @see getDescription
	 * @see setDescription
	 * @see setExtensionListInDescription
	 */
	public boolean isExtensionListInDescription()
	{
		return useExtensionsInDescription;
	}
	
	public void clearExtensions(){
		filters.clear();
	}
	
	
} 