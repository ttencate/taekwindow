<?xml version="1.0" encoding="utf-8"?>

<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

	<variable name="version" select="document('version.xml')/version"/>

	<variable name="fullversion">
		<value-of select="$version/major"/>
		.
		<value-of select="$version/minor"/>
		.
		<value-of select="$version/revision"/>
		.
		<value-of select="$version/build"/>
	</variable>

	<variable name="shortversion">
		<value-of select="$version/major"/>
		.
		<value-of select="$version/minor"/>
		<if test="revision != '0' or build != '0'">
			.
			<value-of select="$version/revision"/>
			<if test="build != '0'">
				.
				<value-of select="$version/build"/>
			</if>
		</if>
	</variable>

</stylesheet>