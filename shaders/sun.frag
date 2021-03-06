#version 130
uniform sampler2D ourTexture;
uniform float power;

varying vec3 texCoords;
varying vec3 normal;
varying vec3 frag_coord;

vec3 light_pos = vec3(0,0,0);

void main(void) {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light_pos - frag_coord);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1,1,1);

    vec2 longitudeLatitude = vec2((atan(texCoords.y, texCoords.x) / 3.1415926 +
                1.0) * 0.5,
            (asin(texCoords.z) / 3.1415926 +
             0.5));
    vec4 material_color = texture(ourTexture, longitudeLatitude);
    gl_FragColor = vec4(material_color.xyz * power * 1.2, 1.0);
}
