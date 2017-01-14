#version 330 core
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
@property( hlms_binormal )in vec3 binormal;@end
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



//layout(binding = 0) uniform instanceBuffer
//{
//	vec4 colour;

	//mat4 view;
	//mat4 model;
	//mat4 proj;

//} instance;


//layout(binding = 4) uniform indexBuffer
//{
//	uvec4 colour; //kD.w is alpha_test_threshold
//	uvec4 viewProj0;
//	uvec4 viewProj1;
//	uvec4 viewProj2;
//	uvec4 viewProj3;
	
//} test;



out vec4 vcolor;
void main()
{

	outVs.drawId=drawId; 
	mat4 final;
//	final[0]=uintBitsToFloat(test.viewProj0);
//	final[1]=uintBitsToFloat(test.viewProj1);
	//final[2]=uintBitsToFloat(test.viewProj2);
	//final[3]=uintBitsToFloat(test.viewProj3);
	
	mat4 testm=mat4(	1.358,0,0,0,
						0, 2.41421, 0, 0,
						0, 0, -1.0004, -0.40008,
						0, 0, -1, 0 );
	
	// mat4x3 worldMat = UNPACK_MAT4x3( worldMatBuf, drawId @property( !hlms_shadowcaster )<< 1u@end );
	// mat4x3 worldMat = UNPACK_MAT4x3( worldMatBuf, drawId<< 1u );
    //mat4 worldView = UNPACK_MAT4( worldMatBuf, (drawId << 1u) + 1u );
    //vec4 worldPos = vec4( (worldView*vertex) );



    mat4 worldMat = UNPACK_MAT4( worldMatBuf, drawId<<1);
	
    mat4 worldView = UNPACK_MAT4( worldMatBuf, (drawId<<1) + 1u );
	
	vec4	worldPos = vec4( (worldView*vertex) );
	//worldPos = vec4( (worldMat * vertex).xyz, 1.0f );
	
 @property( hlms_shadowcaster )
 //	worldPos = vec4( (worldMat*pass.viewMat*vertex) )+vec4(float(drawId)/10.0,0,0,0);
	

 @end
 

 
 
 	worldPos.xyz =  vec4( (worldMat * vertex) ).xyz;
 	worldPos.w =  1.0;
 	
  
 	
	outVs.worldPos= worldPos;


	

	@property( hlms_shadowcaster )worldView = pass.View;@end
	
	
	vcolor =vec4(0.5,float(drawId)/100.0,mod(float(drawId)/10.0,1.0),0);
	vcolor=worldPos;
	vcolor=worldPos;
	
	


    	//vcolor.xyz =vec3(0.5,1,0);
    	
    
    //gl_Position =final*vertex;
   // gl_Position = pass.viewProj*worldPos;
    

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
	//Lighting is in view space
	//outVs.pos		= @insertpiece( CalculatePsPos );

  
  outVs.tangent	= mat3(@insertpiece( worldViewMat )) * @insertpiece(local_tangent);
  
@property( !hlms_dual_paraboloid_mapping )
	outVs.pos		=pass.View*worldPos;
    @property( hlms_normal || hlms_qtangent )outVs.normal	= mat3(@insertpiece( worldViewMat )) * @insertpiece(local_normal);@end


    gl_Position = pass.Proj *(outVs.pos);
    
    

	
	
    //outVs.glPosition=gl_Position;
	//outVs.glPosition.x = (length(outVs.pos.xyz))*pass.depthrange.x;
	outVs.glPosition =gl_Position;
	//outVs.depth =gl_Position.z*pass.Invdepthrange;
	
    @end
        	  vcolor=vertex;
    
    
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