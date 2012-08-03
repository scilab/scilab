<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="preference" mode="tooltip">Preferences</xsl:template>
  <xsl:template match="backups">
    <xsl:variable name="name" select="@name"/>
    <Title text="_(Preferences backup)">
    <Grid>
      <Label gridx="1" gridy="1" weightx="0" text="_(Select a backup file)"/>
      <FileSelector gridx="2" gridy="1" weightx="1" anchor="above_baseline"
                    listener="EntryListener"
                    href="{@restore-path}"
                    mask="*.xml"
                    desc="_(Scilab Configuration files)"
                    dir-selection="false"
		    check-entry="false">
        <entryChanged choose="restore-path">
          <xsl:call-template name="context"/>
        </entryChanged>
      </FileSelector>
      <Panel gridx="3" gridy="1" weightx="1" fill="both"/>
      <Button gridx="4" gridy="1" text="_(Restore)" listener="ActionListener">
        <actionPerformed callback="Restore Backup">
          <xsl:attribute name="path">
            <xsl:value-of select="@restore-path"/>
          </xsl:attribute>
	  <xsl:call-template name="context"/>
        </actionPerformed>
      </Button>
      <Label gridx="1" gridy="2" weightx="0" text="_(Create a backup file)"/>
      <FileSelector gridx="2" gridy="2" weightx="1" anchor="above_baseline"
                    listener="EntryListener"
                    href="{@save-path}"
                    mask="*.xml"
                    desc="_(Scilab Configuration files)"
                    dir-selection="false"
		    check-entry="false">
        <entryChanged choose="save-path">
          <xsl:call-template name="context"/>
        </entryChanged>
      </FileSelector>
      <Panel gridx="3" gridy="2" weightx="1" fill="both"/>
      <Button gridx="4" gridy="2" text="_(Save)" listener="ActionListener">
        <actionPerformed callback="Save Backup">
	  <xsl:attribute name="path">
            <xsl:value-of select="@save-path"/>
          </xsl:attribute>
	  <xsl:call-template name="context"/>
        </actionPerformed>
      </Button>
    </Grid>
    </Title>
  </xsl:template>
</xsl:stylesheet>