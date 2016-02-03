attribute vec4 g_vVertex;
attribute vec4 g_vVColor;
varying vec4 g_vVSColor;
varying vec2 coord;

void main()
{
	gl_Position = vec4(g_vVertex.x, g_vVertex.y, g_vVertex.z, g_vVertex.w);
	g_vVSColor = g_vColor;
	coord = gl_Position.xy;
}