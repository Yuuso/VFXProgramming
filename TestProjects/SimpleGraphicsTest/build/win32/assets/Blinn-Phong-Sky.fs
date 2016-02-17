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
uniform samplerCube s_cubeMap;

varying vec3 g_vViewVecES;
varying vec3 g_vNormalES;
varying vec3 g_vLightVecES;
varying vec3 coord;

float t = totalTime;

void main()
{
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

    gl_FragColor = vEnvColor; //Glossyness value
    float mag = 0.03;
    gl_FragColor.rgba += vec4(mag, mag, mag, mag) * g_Material.vDiffuse * fDiffuse;
    gl_FragColor.rgb += SelfShadow * vec3(mag, mag, mag) * g_Material.vSpecular.xyz * fSpecular;
}