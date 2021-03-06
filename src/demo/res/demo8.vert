#htshader vertex
#htversion 330 core

layout {
	vec3 POSITION as position;
};

uniform {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	float skyboxSize;
	vec3 padding;
};

out {
	vec3 uvs;
};

main_block {
	vec3 skyboxPosition = vec3(position.x * skyboxSize, position.y * skyboxSize, position.z * skyboxSize);
	OUT_POSITION = projectionMatrix * viewMatrix * vec4(skyboxPosition, 1.0);

	uvs = position;
};