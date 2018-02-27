#htshader fragment
#htversion #MAX_VERSION

layout {
    vec4 color1;
    vec4 relection;
    vec4 refraction;
} as Output;

in {
    vec3 position;
    vec2 uv;
    vec3 normals;
};

uniform {
    Texture2D textures[32];
    Texture2D diffuseTexture;
    Texture2D specularTexture;
    Texture2D normalMap;
	Texture3D test2;
	mat4 test;
} as textures;

main_block {
    Output.color1 = sample2D(diffuseTexture, uv);
    Output.reflection = position * normals;
};