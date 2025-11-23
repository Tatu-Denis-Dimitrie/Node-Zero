#version 330

// CRT Shader pentru NodeZero - inspirat din Nodebuster
// Efecte: scanlines, barrel distortion, chromatic aberration, bloom, noise

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 resolution;
uniform float time;

out vec4 finalColor;

// Barrel distortion (ecran curbat) - clasic cu colțuri negre
vec2 barrelDistortion(vec2 coord, float amount) {
    vec2 cc = coord - 0.5;
    float dist = dot(cc, cc);
    return coord + cc * dist * amount;
}

// Chromatic aberration (separare RGB)
vec3 chromaticAberration(sampler2D tex, vec2 uv, float amount) {
    vec2 direction = uv - 0.5;

    vec2 uvR = uv + direction * amount * 0.003;
    vec2 uvG = uv;
    vec2 uvB = uv - direction * amount * 0.003;

    float r = texture(tex, uvR).r;
    float g = texture(tex, uvG).g;
    float b = texture(tex, uvB).b;

    return vec3(r, g, b);
}

// Scanlines orizontale
float scanlineHorizontal(vec2 uv, float intensity) {
    float line = sin(uv.y * resolution.y * 2.0) * 0.5 + 0.5;
    return 1.0 - (line * intensity);
}

// Scanlines verticale
float scanlineVertical(vec2 uv, float intensity) {
    float line = sin(uv.x * resolution.x * 2.0) * 0.5 + 0.5;
    return 1.0 - (line * intensity);
}

// Noise/flicker
float noise(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233)) + time) * 43758.5453);
}

// Vignette (întunecă colțurile)
float vignette(vec2 uv, float intensity) {
    vec2 pos = uv - 0.5;
    float dist = length(pos);
    return 1.0 - smoothstep(0.3, 0.8, dist) * intensity;
}

void main() {
    // Fără barrel distortion - folosim UV-urile normale
    vec2 uv = fragTexCoord;

    // Chromatic aberration (subtil, ca în Nodebuster)
    vec3 color = chromaticAberration(texture0, uv, 1.0);

    // Aplicare scanlines orizontale (subtile)
    float scanH = scanlineHorizontal(uv, 0.15);
    color *= scanH;

    // Aplicare scanlines verticale (foarte subtile)
    // float scanV = scanlineVertical(uv, 0.08);
    // color *= scanV;

    // Noise flicker foarte fin
    float noiseVal = noise(uv * time * 0.01) * 0.02;
    color += vec3(noiseVal);

    // Vignette ușor
    float vig = vignette(uv, 0.3);
    color *= vig;

    // Flicker random foarte subtil (TV vechi)
    float flicker = 1.0 + sin(time * 10.0) * 0.01;
    color *= flicker;

    // Gamma correction pentru aspect mai "analog"
    color = pow(color, vec3(1.1));

    finalColor = vec4(color, 1.0) * colDiffuse * fragColor;
}
