@piece(DynTex)
vec4 finalDynTex=vec4((mod(DynTexUV.y,1.0)<0.5)^^(mod(DynTexUV.x,1.0)<0.5)?0.3:0.6);
finalDynTex-=vec4((mod(DynTexUV.y,0.1)<0.05)^^(mod(DynTexUV.x,0.1)<0.05)?0.1:-0.1);

@end
@piece(DynTexN)
//vec4 finalDynTex=vec4((mod(DynTexUV.y,1.0)<0.5)^^(mod(DynTexUV.x,1.0)<0.5)?0.3:0.6);
vec4 finalDynTexN=vec4(DynTexUVN.x);

@end