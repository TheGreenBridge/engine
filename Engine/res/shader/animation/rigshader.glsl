#VERTEX_SHADER
#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in ivec4 in_joints;
layout (location = 4) in vec4 in_weights;

uniform mat4 trans_matrix = mat4(1.0);
uniform mat4 pr_matrix = mat4(1.0);
uniform mat4 view_matrix = mat4(1.0);

uniform mat4 joint_transforms[30];


out vec2 pass_uv;
out vec3 SurfaceNormal;
out vec3 ToLightVector;

void main(){
	mat4 boneTransform = in_weights.x * joint_transforms[in_joints.x];
	boneTransform += in_weights.y * joint_transforms[in_joints.y];
	boneTransform += in_weights.z * joint_transforms[in_joints.z];
	boneTransform += in_weights.w * joint_transforms[in_joints.w];

	vec4 position = boneTransform * vec4(in_position, 1.0);
	vec4 worldPosition = trans_matrix * position;
	vec4 positionRelativeToCam = view_matrix * worldPosition;
	gl_Position = pr_matrix * positionRelativeToCam;
	pass_uv = in_uv;

	vec4 tmp_normal = trans_matrix * boneTransform * vec4(in_normal, 0.0);
	SurfaceNormal = tmp_normal.xyz;
	ToLightVector = vec3(0.0, 1.0, 1.0);
}

#FRAGMENT_SHADER
#version 330 core

in vec2 pass_uv;
in vec3 SurfaceNormal;
in vec3 ToLightVector;

uniform vec3 color;

out vec4 fragColor;

vec3 unitNormal = normalize(SurfaceNormal);
vec3 unitLightVector = normalize(ToLightVector);

void main(){
	vec4 baseColor = vec4(color, 1.0);
	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	

	// Diffuse
	float nDotl = dot(unitNormal, unitLightVector);
    float brightness = max(nDotl, 0.0);
    vec3 totalDiffuse = max(brightness * lightColour, 0.2);
    
	vec4 diffuse = vec4(totalDiffuse, 1.0) * baseColor ;

	//fragColor = diffuse;
	fragColor = vec4(unitNormal, 1.0);

}