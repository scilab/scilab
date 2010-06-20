
package org.scilab.modules.xcos.simulink.patterns;

import java.math.BigInteger;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for RealValueMap complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="RealValueMap">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="simName" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="xcosIndex" use="required" type="{http://www.w3.org/2001/XMLSchema}integer" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "RealValueMap")
public class RealValueMap {

    @XmlAttribute(required = true)
    protected String simName;
    @XmlAttribute(required = true)
    protected BigInteger xcosIndex;

    /**
     * Gets the value of the simName property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSimName() {
        return simName;
    }

    /**
     * Sets the value of the simName property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSimName(String value) {
        this.simName = value;
    }

    /**
     * Gets the value of the xcosIndex property.
     * 
     * @return
     *     possible object is
     *     {@link BigInteger }
     *     
     */
    public BigInteger getXcosIndex() {
        return xcosIndex;
    }

    /**
     * Sets the value of the xcosIndex property.
     * 
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *     
     */
    public void setXcosIndex(BigInteger value) {
        this.xcosIndex = value;
    }

}
