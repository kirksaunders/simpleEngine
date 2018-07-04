#version 330 core

uniform vec4 modelColor;
uniform vec4 lightColor;
uniform vec4 ambientColor;

in vec2 texCoords;

in vec3 faceNormal;
in vec3 vertexPosition;
in vec3 lightPosition;
in vec3 cameraPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;

out vec4 color;

void main() {
	vec4 ambient = ambientColor * lightColor;

	vec3 norm = normalize(faceNormal);
	vec3 lightDir = normalize(lightPosition - vertexPosition);
	float lambertian = max(dot(norm, lightDir), 0.0f);
	vec4 diffuseVec = vec4(lambertian, lambertian, lambertian, 1.0f);
	vec4 diffuse = lightColor * diffuseVec;

	vec3 viewDir = normalize(cameraPosition - vertexPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specVec = vec4(spec, spec, spec, 1.0f);
	vec4 specular = specVec * lightColor;

	//vec4 total = ambient + diffuse + specular;

	vec4 dif1 = texture(texture_diffuse1, texCoords);
	vec4 dif2 = texture(texture_diffuse2, texCoords);
	vec4 dif3 = texture(texture_diffuse3, texCoords);
	vec4 dif4 = texture(texture_diffuse4, texCoords);

	vec4 spec1 = texture(texture_specular1, texCoords);
	vec4 spec2 = texture(texture_specular2, texCoords);
	vec4 spec3 = texture(texture_specular3, texCoords);
	vec4 spec4 = texture(texture_specular4, texCoords);

	vec4 difColor = mix(dif4, dif3, 1 - dif4.a);
	difColor = mix(difColor, dif2, 1 - difColor.a);
	difColor = mix(difColor, dif1, 1 - difColor.a);

	float opacity = (1 - dif1.a) * (1 - dif2.a) * (1 - dif3.a) * (1 - dif4.a);

	difColor = mix(vec4(difColor.xyz, 1.0f), modelColor, pow(opacity, 4.0f)) * (diffuse + ambient);

	vec4 specColor = mix(spec4, spec3, 1 - spec4.a);
	specColor = mix(specColor, spec2, 1 - specColor.a);
	specColor = mix(specColor, spec1, 1 - specColor.a);

	opacity = (1 - dif1.a) * (1 - dif2.a) * (1 - dif3.a) * (1 - dif4.a);

	specColor = mix(vec4(specColor.xyz, 1.0f), modelColor, pow(opacity, 4.0f)) * specular;

	color = vec4(difColor.xyz, 1.0f) + vec4(specColor.xyz, 1.0f);
}