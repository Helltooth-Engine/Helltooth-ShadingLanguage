#htshader fragment
#htversion 330 core

layout {
	vec4 color;
} as Output;

in {
	vec3 positions;
};

uniform {
	Texture3D textureCube;
};

main_block {
	Output.color = sample3D(textureCube, positions);
};