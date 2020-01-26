#htshader vertex
#htversion 330 core

layout {
	vec3 POSITION as position;
};

uniform {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	float skyboxSize;
};

out {
	vec3 positions;
};

main_block {
	positions = position;

	vec3 pos = vec3(position.x * skyboxSize, position.y * skyboxSize, position.z * skyboxSize);
	OUT_POSITION = projectionMatrix * viewMatrix * vec4(pos, 1.0);

};