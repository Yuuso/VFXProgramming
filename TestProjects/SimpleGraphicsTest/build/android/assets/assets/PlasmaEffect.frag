#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying vec4 g_vVSColor;
varying vec2 coord;
uniform float timer;

void main()
{
	float t= timer;
	float x = coord.x * 250.0;
	float y = coord.y * 250.0;

	float vv = 0.25 * (
			(0.5 + (0.5 * sin(x/7.0+t))) +
			(0.5 + (0.5 * sin(y/5.0-t))) +
			(0.5 + (0.5 * sin((x+y)/6.0-t))) +
			(0.5 + (0.5 * sin(sqrt(float(x*x + y*y))/3.0+sin(t)*5))) 														 \n"
			);

    gl_FragColor = vec4(vv*0.5, vv*0.6*t/6, vv, 1.0f) * g_vVSColor;
    if(gl_FragColor.x < 0.1)
    {
		discard;
    }
}