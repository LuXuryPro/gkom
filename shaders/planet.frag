#version 130
uniform sampler2D ourTexture;
uniform sampler2D normal_map;
uniform sampler2D shadowMap;
uniform float power;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 frag_coord;
varying vec3 up;
varying vec3 tangent_v;

varying vec4 light_space_frag_coord;

vec3 light_pos = vec3(0,0,0);

float ShadowCalculation(vec4 fragPosLightSpace, float cosTheta)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
    bias = clamp(bias, 0,0.1);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) *
                    texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
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
    vec3 diffuse = vec3(diff);
    vec4 material_color = texture(ourTexture, longitudeLatitude);
    float shadow = ShadowCalculation(light_space_frag_coord,
            dot(norm, lightDir));
    gl_FragColor = vec4(material_color.xyz*diffuse*(1-shadow)*power, 1.0);
    // gl_FragColor = vec4(diff, diff, diff, 1.0);
    //gl_FragColor = vec4(norm.xyz, 1.0);
}
