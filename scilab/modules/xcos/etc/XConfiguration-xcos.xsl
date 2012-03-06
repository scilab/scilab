<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:template match="xcos">
		<HBox>
			Click the button below to launch the Xcos Preferences window.
			<Glue/>
		</HBox>
		<HBox>
			<Glue/>
			<Button text="Launch Xcos Preferences" listener="ActionListener">
				<actionPerformed   callback="XcosPreferences"/>
			</Button>
		</HBox>
		<VSpace height="300"/>
	</xsl:template>

</xsl:stylesheet>

