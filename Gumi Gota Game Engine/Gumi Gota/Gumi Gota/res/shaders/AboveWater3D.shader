#shader vertex
#version 330 core

/*
*	MAX_RENDERED_LIGHTS = 16
*/


/* In Variables */
layout(location = 0) in vec3 in_vertex;


/* Out Variables */
out vec4 pass_clipSpacePosition;
out vec2 pass_textureCoordinates;
out vec3 pass_toCameraVector;

out vec3 pass_fromLightVector[16];


/* Uniforms */
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

uniform vec3 u_cameraPosition;

uniform vec3 u_lightPosition[16];

uniform float u_tilingFactor;

uniform bool u_reflectLightVertex;
uniform bool u_reflectSceneVertex;


/* Main Loop */
void main()
{
	/* CALCULATIONS */
	// World Position Calculations
	vec4 worldPosition = u_modelMatrix * vec4(in_vertex, 1.0);

	// Clip Space Calculations
	pass_clipSpacePosition = u_projectionMatrix * u_viewMatrix * worldPosition;

	// Texture Coordinates Calculations
	pass_textureCoordinates = vec2(in_vertex.x / 2.0 + 0.5, in_vertex.z / 2.0 + 0.5) * u_tilingFactor;

	// Camera Calculations
	if (u_reflectSceneVertex == true)
	{
		pass_toCameraVector = u_cameraPosition - worldPosition.xyz;
	}

	// Light Calculations
	if (u_reflectLightVertex == true)
	{
		for (int i = 0; i < 16; i++)
		{
			pass_fromLightVector[i] = worldPosition.xyz - u_lightPosition[i];
		}
	}

	/* FINAL POSITION SETTING */
	gl_Position = pass_clipSpacePosition;
}






#shader fragment
#version 330 core


/* In Variables */
in vec4 pass_clipSpacePosition;
in vec2 pass_textureCoordinates;
in vec3 pass_toCameraVector;

in vec3 pass_fromLightVector[16];


/* Out Variables */
out vec4 out_color;


/* Uniforms */
uniform sampler2D u_reflectionTexture;
uniform sampler2D u_refractionTexture;

uniform sampler2D u_dudvMap;
uniform sampler2D u_normalMap;

uniform sampler2D u_depthMap;

uniform vec2 u_zLimitsData;

uniform vec3 u_lightColor[16];
uniform vec3 u_lightAttenuation[16];

uniform float u_movementFactor;
uniform float u_waveStrength;
uniform float u_lightShineDamper; /// change to u_lightData[0] l8r
uniform float u_lightReflectivity; /// change to u_lightData[1] l8r

uniform bool u_reflectLightFragment;
uniform bool u_reflectSceneFragment;


/* Main Loop */
void main()
{
	/* CALCULATIONS */
	// Normalized Device Space Calculations
	vec2 normalizedDeviceSpacePosition = (pass_clipSpacePosition.xy / pass_clipSpacePosition.w) / 2.0 + 0.5;

	// Texture Coordinates Calculations
	vec2 refractionTextureCoordinates = vec2(normalizedDeviceSpacePosition.x, normalizedDeviceSpacePosition.y);

	// Depth Calculations
	float depth = texture(u_depthMap, refractionTextureCoordinates).r;
	float floorDistance = 2.0 * u_zLimitsData.x * u_zLimitsData.y / (u_zLimitsData.y + u_zLimitsData.x - (2.0 * depth - 1.0) * (u_zLimitsData.y - u_zLimitsData.x));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * u_zLimitsData.x * u_zLimitsData.y / (u_zLimitsData.y + u_zLimitsData.x - (2.0 * depth - 1.0) * (u_zLimitsData.y - u_zLimitsData.x));

	float waterDepth = floorDistance - waterDistance;

	// Water Effects Calculations
	vec2 distortedTextureCoordinates = texture(u_dudvMap, vec2(pass_textureCoordinates.x + u_movementFactor, pass_textureCoordinates.y)).rg * 0.1;
	distortedTextureCoordinates = pass_textureCoordinates + vec2(distortedTextureCoordinates.x, distortedTextureCoordinates.y + u_movementFactor);
	vec2 distortion = (texture(u_dudvMap, distortedTextureCoordinates).rg * 2.0 - 1.0) * u_waveStrength * clamp(waterDepth / 20.0, 0.0, 1.0);


	vec4 reflectionColor;

	if (u_reflectSceneFragment == true)
	{
		vec2 reflectionTextureCoordinates = vec2(normalizedDeviceSpacePosition.x, -normalizedDeviceSpacePosition.y);

		reflectionTextureCoordinates += distortion;
		reflectionTextureCoordinates.x = clamp(reflectionTextureCoordinates.x, 0.001, 0.999);
		reflectionTextureCoordinates.y = clamp(reflectionTextureCoordinates.y, -0.999, -0.001);
		reflectionColor = texture(u_reflectionTexture, reflectionTextureCoordinates);
	}


	refractionTextureCoordinates += distortion;
	refractionTextureCoordinates = clamp(refractionTextureCoordinates, 0.001, 0.999);
	vec4 refractionColor = texture(u_refractionTexture, refractionTextureCoordinates);

	// Normal Calculations
	vec4 normalMapColor = texture(u_normalMap, distortedTextureCoordinates);
	vec3 waterNormal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
	waterNormal = normalize(waterNormal);

	// Fresnel Effect Calculations
	vec3 normalizedToCameraVector;
	float fresnelFactor; // Fresnel Effect

	if (u_reflectSceneFragment == true)
	{
		normalizedToCameraVector = normalize(pass_toCameraVector);
		fresnelFactor = dot(normalizedToCameraVector, waterNormal);
		fresnelFactor = pow(fresnelFactor, 2.0); // Specifying Reflectivity of Water
		fresnelFactor = clamp(fresnelFactor, 0.0, 1.0);
	}

	// Light Calculations
	vec3 totalSpecular = vec3(0.0);

	if (u_reflectLightFragment == true)
	{
		for (int i = 0; i < 16; i++)
		{
			if (u_lightAttenuation[i] == vec3(0.0) && u_lightColor[i] == vec3(0.0))
			{
				break;
			}

			float lightDistance = length(pass_fromLightVector[i]);
			float attenuationFactor = u_lightAttenuation[i].x + (u_lightAttenuation[i].y * lightDistance) + (u_lightAttenuation[i].z * lightDistance * lightDistance);

			vec3 unitLightVector = normalize(pass_fromLightVector[i]);

			vec3 reflectedLightDirection = reflect(unitLightVector, waterNormal);

			float specularFactor = max(dot(reflectedLightDirection, normalizedToCameraVector), 0.0);
			specularFactor = pow(specularFactor, u_lightShineDamper);
			totalSpecular = totalSpecular + (specularFactor * u_lightReflectivity * u_lightColor[i]) / attenuationFactor;
		}

		totalSpecular *= clamp(waterDepth / 5.0, 0.0, 1.0);
	}

	/* FINAL COLOR SETTING */
	if (u_reflectSceneFragment == true)
	{
		out_color = mix(reflectionColor, refractionColor, fresnelFactor) + vec4(totalSpecular, 1.0);
	}
	else
	{
		out_color = refractionColor + vec4(totalSpecular, 1.0);
	}
	
	out_color.a = clamp(waterDepth / 5.0, 0.0, 1.0);
}