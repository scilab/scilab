package org.scilab.modules.xcos.utils;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.Map;

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
	protected void setFieldValue(Object obj, String fieldname, Object value)
	{
		System.out.println("my set field value :" + fieldname);
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
					
					System.out.println("method : " + method.toGenericString() );
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
				}
			}
		}
		catch (Exception e)
		{
			// ignore
		}
	}
	
	
}
