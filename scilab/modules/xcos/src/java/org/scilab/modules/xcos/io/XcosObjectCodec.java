package org.scilab.modules.xcos.io;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxObjectCodec;

public class XcosObjectCodec extends mxObjectCodec {
	
	public XcosObjectCodec(Object template) {
		super(template);
	}
	
	
	public XcosObjectCodec(Object template, String[] exclude, String[] idrefs,
			Map mapping)
	{
		super(template, exclude, idrefs, mapping);
		
	}

	/**
	 * Sets the value of the field with the specified name
	 * in the specified object instance.
	 */
	public void setFieldValue(Object obj, String fieldname, Object value)
	{
		Field field = null;

		try
		{
			field = getField(obj, fieldname);

			if (field.getType() == Boolean.class)
			{
				value = new Boolean(value.equals("1")
						|| String.valueOf(value).equalsIgnoreCase("true"));
			}

			field.set(obj, value);
		}
		catch (IllegalAccessException e1)
		{
			if (field != null)
			{
				try
				{
					Method method = getAccessor(obj, field, false);
					Class type = method.getParameterTypes()[0];
					value = convertValueFromXml(type, value);
					
					// Converts collection to a typed array before setting
					if (type.isArray() &&
						value instanceof Collection)
					{
						Collection coll = (Collection) value;
						value = coll.toArray((Object[]) Array.newInstance(
							type.getComponentType(), coll.size()));
					}

					method.invoke(obj, new Object[] { value });
				}
				catch (Exception e2)
				{
					System.err.println("(" +
							"setFieldValue: " + e2 + " on "
							+ obj.getClass().getSimpleName() + "." + fieldname
							+ " (" + field.getType().getSimpleName() + ") = "
							+ value + " (" + value.getClass().getSimpleName()
							+ ")");
					e2.printStackTrace();
				}
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		    // ignore
		}
	}
	
	/**
	 * Returns the value of the field with the specified name in the specified
	 * object instance.
	 */
	public Object getFieldValue(Object obj, String fieldname)
	{
		Object value = null;

		if (obj != null && fieldname != null)
		{
			Field field = getField(obj, fieldname);

			try
			{
				if (field != null)
				{
					value = field.get(obj);
				}
			}
			catch (IllegalAccessException e1)
			{
				if (field != null)
				{
					try
					{
						Method method = getAccessor(obj, field, true);
						value = method.invoke(obj, (Object[]) null);
					}
					catch (Exception e2)
					{
						// ignore
					}
				}
			}
			catch (Exception e)
			{
				// ignore
			}
		}

		return value;
	}
	
	
	/**
	 * Reads the specified child into the given object.
	 */
	public void decodeChild(mxCodec dec, Node child, Object obj)
	{
		String fieldname = getFieldName(((Element) child).getAttribute("as"));

		if (fieldname == null || !isExcluded(obj, fieldname, child, false))
		{
			Object value = null;
			

			
			Object template = getFieldValue(obj, fieldname);

			if (child.getNodeName().equals("add"))
			{
				value = ((Element) child).getAttribute("value");

				if (value == null)
				{
					value = child.getTextContent();
				}
			}
			else
			{
				// Arrays are replaced completely
				if (template != null &&
					template.getClass().isArray())
				{
					template = null;
				}
				// Collections are cleared
				if (template instanceof Collection)
				{
					((Collection) template).clear();
				}
				
				value = dec.decode(child, template);
			}
			
			if (value != null && !value.equals(template))
			{
				if (fieldname != null && obj instanceof Map)
				{
					((Map) obj).put(fieldname, value);
				}
				// Arrays are treated as collections and
				// converted in setFieldValue
				else if (obj instanceof Collection)
				{
					((Collection) obj).add(value);
				}
				else if (fieldname != null && fieldname.length() > 0)
				{
					setFieldValue(obj, fieldname, value);
				}			
			}
		}
	}

	/**
	 * Decodec all children of the given node using decodeChild.
	 */
	public void decodeChildren(mxCodec dec, Node node, Object obj)
	{
		Node child = node.getFirstChild();
		while (child != null)
		{
		    
		    if (child.getNodeType() == Node.ELEMENT_NODE
					&& !this.processInclude(dec, child, obj))
			{
				decodeChild(dec, child, obj);
			}

			child = child.getNextSibling();
		}
	}	
	
	
}
