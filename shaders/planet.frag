#version 130
uniform sampler2D ourTexture;
uniform sampler2D normal_map;
uniform sampler2D shadowMap;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 frag_coord;
varying vec3 up;
varying vec3 tangent_v;

varying vec4 light_space_frag_coord;

vec3 light_pos = vec3(0,0,0);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.0001;
    float shadow = currentDepth - bias> closestDepth  ? 1.0 : 0.0;
    return shadow;
}

void main(void) {
    vec2 longitudeLatitude = vec2((atan(texCoords.y, texCoords.x) / 3.1415926 +
                1.0) * 0.5,
            (asin(texCoords.z) / 3.415926 +
             0.5));

    vec3 norm = texture(normal_map, longitudeLatitude).rgb;
    norm = normalize(norm*2.0 - 1.0);

    mat3 TBN = mat3(tangent_v, up, normal);
    norm = TBN * norm;

    vec3 lightDir = normalize(light_pos - frag_coord);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1,1,1);
    vec4 material_color = texture(ourTexture, longitudeLatitude);
    float shadow = ShadowCalculation(light_space_frag_coord);
    gl_FragColor = vec4(material_color.xyz*diffuse*(1-shadow), 1.0);
   // gl_FragColor = vec4(diff, diff, diff, 1.0);
//gl_FragColor = vec4(norm.xyz, 1.0);
}
