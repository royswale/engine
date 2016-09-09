$in uvec3 a_pos;
#ifdef INSTANCED
// instanced rendering
$in vec3 a_offset;
#endif

uniform mat4 u_light;
uniform mat4 u_model;
$out vec2 o_projZW;

void main()
{
	vec4 pos = u_light * u_model * vec4(a_pos, 1.0f);
#ifdef INSTANCED
	pos = vec4(a_offset, 0.0) + pos;
#endif
	gl_Position = pos;
	o_projZW = pos.zw;
}
