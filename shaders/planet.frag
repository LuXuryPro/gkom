#version 130
uniform sampler2D ourTexture;
uniform sampler2D normal_map;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 frag_coord;
varying vec3 up;
varying vec3 tangent_v;

vec3 light_pos = vec3(0,0,0);

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
    gl_FragColor = vec4(material_color.xyz*diffuse, 1.0);
   // gl_FragColor = vec4(diff, diff, diff, 1.0);
//gl_FragColor = vec4(norm.xyz, 1.0);
}
