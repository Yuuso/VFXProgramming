precision mediump float;

#extension GL_OES_standard_derivatives : enable

#define saturate(x) clamp(x, 0.0, 1.0)

struct MATERIAL
{
    vec4 vAmbient;
    vec4 vDiffuse;
    vec4 vSpecular;
};

uniform MATERIAL g_Material;
uniform float totalTime;
uniform sampler2D s_diffuseMap;
uniform samplerCube s_cubeMap;

varying vec3 g_vViewVecES;
varying vec3 g_vNormalES;
varying vec3 g_vLightVecES;
varying vec3 coord;

float t = totalTime;

void main()
{
    float x = coord.x * 250.0;
    float y = coord.y * 250.0;

    float vv = 0.25 * (
            (0.5 + (0.5 * sin(x/7.0+t))) +
            (0.5 + (0.5 * sin(y/5.0-t))) +
            (0.5 + (0.5 * sin((x+y)/6.0-t))) +
            (0.5 + (0.5 * sin(sqrt(float(x*x + y*y))/3.0+sin(t)*5)))
            );

    gl_FragColor = vec4(vv*0.5, vv*0.6*t/6, vv, 1.0f) * g_Material.vAmbient;
    
    vec4 texelColor = texture2D(s_diffuseMap, coord.xy);
    vec3 vNormalt = normalize(g_vNormalES);
    vec3 vViewt = normalize(g_vViewVecES);
    vec4 vEnvColor = textureCube(s_cubeMap, reflect(vViewt, vNormalt));

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
    gl_FragColor.rgb += SelfShadow * vec3(0.1, 0.1, 0.1) * g_Material.vSpecular.xyz * fSpecular;
    gl_FragColor += texelColor;
    gl_FragColor += vEnvColor * 0.3; //Glossyness value
}