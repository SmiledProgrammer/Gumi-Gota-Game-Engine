#shader vertex
#version 330 core

/*
 *	MAX_RENDERED_LIGHTS = 512
 */

/* In Variables */
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_textureCoordinates;


/* Out Variables */
out vec2 pass_textureCoordinates;

out vec4 pass_position;


/* Uniforms */
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;


/* Main Loop */
void main()
{
	// Position Calculating
	pass_position = u_modelMatrix * vec4(in_position, 0.0, 1.0);

	// Passing
	pass_textureCoordinates = in_textureCoordinates;

	// Position Setting
	gl_Position = u_projectionMatrix * u_viewMatrix * pass_position;
}






#shader fragment
#version 330 core


/* In Variables */
in vec2 pass_textureCoordinates;

in vec4 pass_position;


/* Out Variables */
out vec4 out_color;


/* Uniforms */
uniform sampler2D u_textureSampler;

uniform vec2 u_lightPosition[512];
uniform vec3 u_lightColor[512];
uniform vec3 u_lightAttenuation[512];


/* Main Loop */
void main()
{
	// Light Calculating
	vec3 totalLightColor = vec3(0.0);

	for (int i = 0; i < 512; i++)
	{
		if (u_lightAttenuation[i] == vec3(0.0) && u_lightColor[i] == vec3(0.0))
		{
			break;
		}

		float lightDistance = length(pass_position.xy - u_lightPosition[i]);
		float attenuationFactor = (u_lightAttenuation[i].x) + (u_lightAttenuation[i].y * lightDistance) + (u_lightAttenuation[i].z * lightDistance * lightDistance);

		if (u_lightColor[i] != vec3(0.0, 0.0, 0.0))
		{
			totalLightColor = totalLightColor + u_lightColor[i] / attenuationFactor;
		}
	}

	// Texture Calculating
	vec4 textureColor = texture2D(u_textureSampler, pass_textureCoordinates);

	// Color Setting
	out_color = textureColor * vec4(totalLightColor, 1.0);
}