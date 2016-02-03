attribute vec4 g_vPositionOS;
attribute vec3 g_vNormalOS;
attribute vec3 g_vUVCoordOS;

uniform mat4 g_matModelViewProj;
varying vec3 coord;


void main()
{
	vec4 vPositionCS = g_matModelViewProj * g_vPositionOS;

	gl_Position = vPositionCS;
	coord = g_vUVCoordOS;
}