@piece(SampleShadowBC)		

    vec2 texcoord=shadowSampleTexCoord;
        float fx = fract(shadowSampleTexCoord.x);
    float fy = fract(shadowSampleTexCoord.y);
    texcoord.x -= fx;
    texcoord.y -= fy;

    vec4 xcubic = cubic(fx);
    vec4 ycubic = cubic(fy);

    vec4 c = vec4(texcoord.x - 0.5, texcoord.x + 1.5, texcoord.y -
0.5, texcoord.y + 1.5);
    vec4 s = vec4(xcubic.x + xcubic.y, xcubic.z + xcubic.w, ycubic.x +
ycubic.y, ycubic.z + ycubic.w);
    vec4 offset = c + vec4(xcubic.y, xcubic.w, ycubic.y, ycubic.w) /
s;

    vec4 sample0 = texture2D(GBuffer0, vec2(offset.x, offset.z) *
shadowRes.xy);
    vec4 sample1 = texture2D(GBuffer0, vec2(offset.y, offset.z) *
shadowRes.xy);
    vec4 sample2 = texture2D(GBuffer0, vec2(offset.x, offset.w) *
shadowRes.xy);
    vec4 sample3 = texture2D(GBuffer0, vec2(offset.y, offset.w) *
shadowRes.xy);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    final=fract( mix(
        mix(sample3, sample2, sx),
        mix(sample1, sample0, sx), sy) );
        return;
 @end