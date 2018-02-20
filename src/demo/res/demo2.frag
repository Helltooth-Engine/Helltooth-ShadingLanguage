#htshader fragment
#version #MAX_VERSION

layout {
	vec4 color;
};

in {
	vec2 uv;
}

uniform {
	Texture2D textures[2];
};

main_block {
	color = sample2D(textures[0], uv);
}