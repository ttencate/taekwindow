<?xml version="1.0" encoding="utf-8"?>

<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

	<variable name="version" select="document('version.xml')/version"/>

	<variable name="fullversion">
		<value-of select="$version/major"/>
		<text xml:space="preserve">.</text>
		<value-of select="$version/minor"/>
		<text xml:space="preserve">.</text>
		<value-of select="$version/revision"/>
		<text xml:space="preserve">.</text>
		<value-of select="$version/build"/>
	</variable>

	<variable name="fullversioncommas">
		<value-of select="$version/major"/>
		<text xml:space="preserve">,</text>
		<value-of select="$version/minor"/>
		<text xml:space="preserve">,</text>
		<value-of select="$version/revision"/>
		<text xml:space="preserve">,</text>
		<value-of select="$version/build"/>
	</variable>

	<variable name="shortversion">
		<value-of select="$version/major"/>
		<text xml:space="preserve">.</text>
		<value-of select="$version/minor"/>
		<if test="$version/revision != '0' or $version/build != '0'">
			<text xml:space="preserve">.</text>
			<value-of select="$version/revision"/>
			<if test="$version/build != '0'">
				<text xml:space="preserve">.</text>
				<value-of select="$version/build"/>
			</if>
		</if>
	</variable>

</stylesheet>