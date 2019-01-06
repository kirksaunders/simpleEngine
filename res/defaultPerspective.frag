#version 330 core

layout (std140) uniform TestBlock {
    vec4 lightPos;
    vec4 cameraPos;
};

/*uniform vec4 lightPos;
uniform vec4 cameraPos;*/

uniform vec4 modelColor;
uniform vec4 lightColor;

uniform float ambientAmount;

in vec2 texCoords;

in vec3 faceNormal;
in vec3 vertexPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;

uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;

out vec4 color;

vec4 blend(vec4 a, vec4 b) {
    vec4 c;
    c.a = a.a + b.a * (1.0f - a.a);
	c.rgb = mix(b.rgb * b.a, a.rgb, a.a) / max(c.a, 0.001f) * sign(c.a);
    //c.rgb = a.rgb * a.a + b.rgb * b.a * (1 - a.a) / max(c.a, 0.001f) * sign(c.a);
    return c;
}

void main() {
	vec3 norm = normalize(faceNormal);
	vec3 lightDir = normalize(vertexPosition - lightPos.xyz);

    float lambertian = max(dot(norm, -lightDir), 0.0f);
    float spec = pow(max(dot(normalize(cameraPos.xyz - vertexPosition), reflect(lightDir, norm)), 0.0), 32) * sign(lambertian) * 0.25f;

	vec4 dif1 = texture(texture_diffuse1, texCoords);
	vec4 dif2 = texture(texture_diffuse2, texCoords);
	vec4 dif3 = texture(texture_diffuse3, texCoords);
	vec4 dif4 = texture(texture_diffuse4, texCoords);
	vec4 dif5 = texture(texture_diffuse5, texCoords);

	vec4 spec1 = texture(texture_specular1, texCoords);
	vec4 spec2 = texture(texture_specular2, texCoords);
	vec4 spec3 = texture(texture_specular3, texCoords);
	vec4 spec4 = texture(texture_specular4, texCoords);
	vec4 spec5 = texture(texture_specular5, texCoords);

    vec4 difColor = blend(dif2, dif1);
    difColor = blend(dif3, difColor);
    difColor = blend(dif4, difColor);
    difColor = blend(dif5, difColor);
    difColor = blend(difColor, modelColor);

    vec4 ambientColor = vec4(difColor.rgb * ambientAmount, difColor.a);
    difColor.rgb = difColor.rgb * lambertian;

    vec4 specColor = blend(spec2, spec1);
    specColor = blend(spec3, specColor);
    specColor = blend(spec4, specColor);
    specColor = blend(spec5, specColor);
    // final "blend" doesn't require a call to blend because we are blending it onto 1.0, 1.0, 1.0, 1.0
	specColor = mix(vec4(1.0f), specColor, specColor.a);

    specColor.rgb = specColor.rgb * spec;

	color.rgb = ambientColor.rgb + difColor.rgb + specColor.rgb;
	color.a = ambientColor.a;
	color = lightColor * color;
}