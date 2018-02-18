#htshader vertex
#htversion 300 core

layout {
	vec3 POSITION as position;
};


main_block {
	SetPosition(vec4(position, 1.0));
}