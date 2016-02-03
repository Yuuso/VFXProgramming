precision mediump float;

#ifdef GL_FRAGMENT_PRECISION_HIGH
	#define NEED_HIGHP highp
#else
	#define NEED_HIGHP mediump
#endif

varying vec3 coord;
uniform float totalTime;

void main()
{
	float t = cos(totalTime);
	float x = coord.x * 200.0;
	float y = coord.y * 200.0;
	float z = coord.z * 200.0;

	float vv = 0.20 * (
			(0.5 + (0.5 * sin(x/7.0+t))) +
			(0.5 + (0.5 * sin(y/5.0-t))) +
			(0.5 + (0.5 * sin(z/5.0-t))) +
			(0.5 + (0.5 * sin((x+y+z)/6.0-t))) +
			(0.5 + (0.5 * sin(pow((float(x*x + y*y + z*z)), (1.0/3.0))/3.0+sin(t)*2)))
			);

    gl_FragColor = vec4(vv*0.5, vv*0.6*t/6, vv, 1.0f) * vec4(0.8, 0.6, 0.5, 1.0);
}