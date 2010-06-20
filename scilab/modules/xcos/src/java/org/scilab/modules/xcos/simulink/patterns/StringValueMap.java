
package org.scilab.modules.xcos.simulink.patterns;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for StringValueMap complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="StringValueMap">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="simVal" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="xcosVal" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "StringValueMap")
public class StringValueMap {

    @XmlAttribute(required = true)
    protected String simVal;
    @XmlAttribute(required = true)
    protected String xcosVal;

    /**
     * Gets the value of the simVal property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSimVal() {
        return simVal;
    }

    /**
     * Sets the value of the simVal property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSimVal(String value) {
        this.simVal = value;
    }

    /**
     * Gets the value of the xcosVal property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getXcosVal() {
        return xcosVal;
    }

    /**
     * Sets the value of the xcosVal property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setXcosVal(String value) {
        this.xcosVal = value;
    }

}
