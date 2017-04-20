#version 400 core
#extension GL_ARB_shading_language_420pack: require
@property(hlms_shadowcaster)
//Shadow Material
@end

out gl_PerVertex
{
	vec4 gl_Position;
};

layout(std140) uniform;

@insertpiece( Common_Matrix_DeclUnpackMatrix4x4 )
@insertpiece( Common_Matrix_DeclUnpackMatrix4x3 )
@piece( CalculatePsPos )(@insertpiece( worldViewMat ) * @insertpiece(local_vertex)).xyz@end

in vec4 vertex;

@property( hlms_normal )in vec3 normal;@end
@property( hlms_qtangent )in vec4 qtangent;@end


@property( !hlms_qtangent )
in vec3 tangent;
@property( hlms_binormal )
in vec3 binormal;
@end
@end


@property( !hlms_skeleton )
@piece( local_vertex )vertex@end
@piece( local_normal )normal@end
@piece( local_tangent )tangent@end
@end
@property( hlms_skeleton )
@piece( local_vertex )worldPos@end
@piece( local_normal )worldNorm@end
@piece( local_tangent )worldTang@end
@end
@property( hlms_skeleton )
    @piece( worldViewMat )pass.view@end
@end @property( !hlms_skeleton )
    @piece( worldViewMat )worldView@end
@end
@property( hlms_skeleton )
in uvec4 blendIndices;
in vec4 blendWeights;@end

@foreach( hlms_uv_count, n )
in vec@value( hlms_uv_count@n ) uv@n; @end

in uint drawId;

@insertpiece( custom_vs_attributes )

out block
{
@insertpiece( VStoPS_block )
} outVs;

// START UNIFORM DECLARATION
@insertpiece( PassDecl )
@property( hlms_skeleton )@insertpiece( InstanceDecl )@end
layout(binding = 0) uniform samplerBuffer worldMatBuf;
@insertpiece( custom_vs_uniformDeclaration )


@property( hlms_qtangent )
@insertpiece( DeclQuat_xAxis )
@insertpiece( DeclQuat_yAxis )
 @end









void main()
{

	outVs.drawId=drawId; 
	mat4 final;

	mat4 testm=mat4(	1.358,0,0,0,
						0, 2.41421, 0, 0,
						0, 0, -1.0004, -0.40008,
						0, 0, -1, 0 );
	



    mat4 worldMat = UNPACK_MAT4( worldMatBuf, drawId<<1);
	
    mat4 worldView = UNPACK_MAT4( worldMatBuf, (drawId<<1) + 1u );
    
    
    
    outVs.worldMat=worldMat;
	//vec4	worldPos = vec4( (worldView*vertex) );
	
	
	

 
	vec4	worldPos;
 
 
 	worldPos.xyz =  vec4( (worldMat * vertex) ).xyz;
 	worldPos.w =  1.0;
 	
  
 	
	outVs.worldPos= worldPos;


	

	@property( hlms_shadowcaster )worldView = pass.View;@end
	
	




	@foreach( hlms_uv_count, n )
		outVs.uv@n = uv@n;@end
    
    
    outVs.vertex=vertex.xyz;
@property( hlms_qtangent )
	//Decode qTangent to TBN with reflection
	vec3 normal		= xAxis( normalize( qtangent ) );
	vec3 tangent	= yAxis( qtangent );
	outVs.biNormalReflection = sign( qtangent.w ); //We ensure in C++ qtangent.w is never 0
@end
		
		@insertpiece(VertexTransform)
		 
	@property(noTransf)
        gl_Position = vertex*vec4(1,1,1,1);
    @end

		
    }
    
@piece( VertexTransform )

  
  outVs.tangent	= mat3(@insertpiece( worldViewMat )) * @insertpiece(local_tangent);
  
@property( !hlms_dual_paraboloid_mapping )


	@property(noCamTransf)
		worldPos.w=0;
	@end
	outVs.pos		=pass.View*worldPos;
	@property(noCamTransf)
		worldPos.w=1;
		outVs.pos.w=1;
	@end
    @property( hlms_normal || hlms_qtangent )outVs.normal	= mat3(@insertpiece( worldViewMat )) * @insertpiece(local_normal);@end


    outVs.glPosition = pass.Proj *(outVs.pos);
	gl_Position=outVs.glPosition;
	@insertpiece(custom_vertex_post)

    @end


@property( hlms_dual_paraboloid_mapping)
	//Dual Paraboloid Mapping
	gl_Position.w	= 1.0f;
	@property( hlms_normal || hlms_qtangent )gl_Position.xyz	= outVs.pos.xyz;@end
	@property( !hlms_normal && !hlms_qtangent )gl_Position.xyz	= @insertpiece( CalculatePsPos );@end
	float L = length( gl_Position.xyz );
	gl_Position.z	+= 1.0f;
	gl_Position.xy	/= gl_Position.z;
	gl_Position.z	= (L - pass.nearClip) / (pass.farClip - pass.nearClip);@end
@end
