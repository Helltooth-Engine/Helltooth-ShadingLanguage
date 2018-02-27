#htshader fragment
#htversion #MAX_VERSION

layout {
	vec4 color;
} as Output;

in {
	vec2 uv;
};

uniform {
	Texture2D textures[2];
};

main_block {
	Output.color = sample2D(textures[0], uv);
};