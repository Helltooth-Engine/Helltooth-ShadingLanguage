#htshader vertex
#htversion #MAX_VERSION

layout {
	vec3 POSITION as position;
	vec2 UV as uv;
	vec3 NORMAL as normals;
};

uniforms {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
};

struct Test {
	mat4 shit;
	vec4 otherShit;
	vec3 moreShit;
};

out {
	vec2 uv;
} as out;

main_block {

	OUT_POSITION = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

	out.uv = uv;
};