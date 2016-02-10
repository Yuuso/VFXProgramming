precision mediump float;

#extension GL_OES_standard_derivatives : enable

#define saturate(x) clamp(x, 0.0, 1.0)
#define iterations 25
#define deep_steps 50.0

struct MATERIAL
{
    vec4 vAmbient;
    vec4 vDiffuse;
    vec4 vSpecular;
};

uniform MATERIAL g_Material;
uniform float totalTime;
uniform sampler2D s_diffuseMap;

varying vec3 g_vViewVecES;
varying vec3 g_vNormalES;
varying vec3 g_vLightVecES;
varying vec3 coord;

float t = totalTime;

float sphere(vec3 p, vec3 rd, float r)
{
  float b = dot( -p, rd );
  float inner = b * b - dot( p, p ) + r * r;
  if( inner < 0.0 ) return -1.0;
  float s=sqrt(inner);
  return b - s;
}


mat2 rot(float a) 
{
  float c=cos(a), s=sin(a);
  return mat2(c,s,-s,c);
}

vec3 kset(in vec3 p)
{
  p+=sin(p*100.+totalTime*8.)*.0005;
  p*=.74;
  p=abs(1.-mod(p,2.));
  vec3 cp=vec3(0.);
  float c=1000.;
  for (int i=0; i<iterations; i++)
  {
    float dd=dot(p,p);
    vec3 p2=p;
    p=abs(p);
    p=p/dd-1.;
    cp+=exp(-50.*abs(p-p2*.5));
    c=min(c,dd);
  }
  c=pow(max(0.,.2-c)/.2,5.);
  return cp*.03+c*.3;
}

void main()
{
//    float x = coord.x * 250.0;
//    float y = coord.y * 250.0;
//
//    float vv = 0.25 * (
//            (0.5 + (0.5 * sin(x/7.0+t))) +
//            (0.5 + (0.5 * sin(y/5.0-t))) +
//            (0.5 + (0.5 * sin((x+y)/6.0-t))) +
//            (0.5 + (0.5 * sin(sqrt(float(x*x + y*y))/3.0+sin(t)*5)))
//            );

    //gl_FragColor = vec4(vv*0.5, vv*0.6*t/6, vv, 1.0f) * g_vVSColor;
    
    vec2 resolution = vec2(1280.0, 720.0);
  vec2 uv = gl_FragCoord.xy / resolution.xy-.5;
  uv.x *= resolution.x / resolution.y;
//  vec2 mo=mouse;
  vec3 ro=vec3(coord.x-0.5, coord.y-0.5,-2.2-sin(t*3.7562)*.3);
  vec3 rd=normalize(vec3(uv,1.));
  vec3 v=vec3(0.);
  float x=coord.x*2.+t; float y=coord.y*3.+t*2.;
  mat2 rot1=rot(x);
  mat2 rot2=rot(y);
  float f=1.;
  rd.xy*=rot(.3);
  ro.xy*=rot(.3);
  ro.xz*=rot1;
  rd.xz*=rot1;
  ro.yz*=rot2;
  rd.yz*=rot2;
  float c=0.;
  for (float i=0.; i<deep_steps; i++) 
  {
//    float tt=sphere(ro, rd, 1.0-i*.002);
    float tt = 0.25 * (
            (0.5 + (0.5 * sin(x/7.0+t))) +
            (0.5 + (0.5 * sin(y/5.0-t))) +
            (0.5 + (0.5 * sin((x+y)/6.0-t))) +
            (0.5 + (0.5 * sin(sqrt(float(x*x + y*y))/3.0+sin(t)*5)))
            );
    vec3 p=ro+rd*tt;
    vec3 n=normalize(rd-ro);
    vec3 k=kset(p)*step(0.,tt)*f;
    v+=k*pow(max(0.,dot(rd,n)),8.);
    f*=max(0.5,1.-length(k)*3.5);
  }
  vec4 texelColor = texture2D(s_diffuseMap, coord);

    vec3 vNormal = normalize(g_vNormalES);
    vec3 vLight = normalize(g_vLightVecES);
    vec3 vView = normalize(g_vViewVecES);
    vec3 vHalf = normalize(vLight + vView);

    float NdotL = saturate(dot(vNormal, vLight));
    float NdotH = saturate(dot(vNormal, vHalf));

    float fDiffuse = NdotL;

    float fSpecular = pow(NdotH, g_Material.vSpecular.w);
    float SelfShadow = 4.0 * fDiffuse;

    //gl_FragColor.rgba = vec4(0.15, 0.15, 0.15, 0.15) * g_Material.vAmbient;
    gl_FragColor.rgba += g_Material.vDiffuse * fDiffuse;
    gl_FragColor.rgb += SelfShadow * vec3(0.15, 0.15, 0.15) * g_Material.vSpecular.xyz * fSpecular;
    gl_FragColor += texelColor;
}