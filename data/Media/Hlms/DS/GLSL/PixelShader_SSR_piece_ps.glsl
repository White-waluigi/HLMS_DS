@piece(SSR)

		// By Morgan McGuire and Michael Mara at Williams College 2014
		// Released as open source under the BSD 2-Clause License
		// http://opensource.org/licenses/BSD-2-Clause
		#define point2 vec2
		#define point3 vec3
		 
		float distanceSquared(vec2 a, vec2 b) { a -= b; return dot(a, a); }
		 
		// Returns true if the ray hit something
		bool traceScreenSpaceRay1(
		 // Camera-space ray origin, which must be within the view volume
		 point3 csOrig, 
		 
		 // Unit length camera-space ray direction
		 vec3 csDir,
		 
		 // A projection matrix that maps to pixel coordinates (not [-1, +1]
		 // normalized device coordinates)
		 mat4x4 proj, 
		 
		 // The camera-space Z buffer (all negative values)
		 sampler2D csZBuffer,
		 
		 // Dimensions of csZBuffer
		 vec2 csZBufferSize,
		 
		 // Camera space thickness to ascribe to each pixel in the depth buffer
		 float zThickness, 
		 
		 // (Negative number)
		 float nearPlaneZ, 
		 
		 // Step in horizontal or vertical pixels between samples. This is a float
		 // because integer math is slow on GPUs, but should be set to an integer >= 1
		 float stride,
		 
		 // Number between 0 and 1 for how far to bump the ray in stride units
		 // to conceal banding artifacts
		 float jitter,
		 
		 // Maximum number of iterations. Higher gives better images but may be slow
		 const float maxSteps, 
		 
		 // Maximum camera-space distance to trace before returning a miss
		 float maxDistance, 
		 
		 // Pixel coordinates of the first intersection with the scene
		 out point2 hitPixel, 
		 
		 // Camera space location of the ray hit
		 out point3 hitPoint) {
		 
		    // Clip to the near plane    
		    float rayLength = ((csOrig.z + csDir.z * maxDistance) > nearPlaneZ) ?
		        (nearPlaneZ - csOrig.z) / csDir.z : maxDistance;
		    point3 csEndPoint = csOrig + csDir * rayLength;
		 
		    // Project into homogeneous clip space
		    vec4 H0 = proj * vec4(csOrig, 1.0);
		    vec4 H1 = proj * vec4(csEndPoint, 1.0);
		    H0.xy=H0.xy+float(f2u(pass.debug.z))*10.0;
		    H1.xy=H1.xy+float(f2u(pass.debug.z))*10.0;
		    float k0 = 1.0 / H0.w, k1 = 1.0 / H1.w;
		 
		    // The interpolated homogeneous version of the camera-space points  
		    point3 Q0 = csOrig * k0, Q1 = csEndPoint * k1;
		 
		    // Screen-space endpoints
		    point2 P0 = H0.xy * k0, P1 = H1.xy * k1;
		 
		    // If the line is degenerate, make it cover at least one pixel
		    // to avoid handling zero-pixel extent as a special case later
		    P1 += vec2((distanceSquared(P0, P1) < 0.0001) ? 0.01 : 0.0);
		    vec2 delta = P1 - P0;
		 
		    // Permute so that the primary iteration is in x to collapse
		    // all quadrant-specific DDA cases later
		    bool permute = false;
		    if (abs(delta.x) < abs(delta.y)) { 
		        // This is a more-vertical line
		        permute = true; delta = delta.yx; P0 = P0.yx; P1 = P1.yx; 
		    }
		 
		    float stepDir = sign(delta.x);
		    float invdx = stepDir / delta.x;
		 
		    // Track the derivatives of Q and k
		    vec3  dQ = (Q1 - Q0) * invdx;
		    float dk = (k1 - k0) * invdx;
		    vec2  dP = vec2(stepDir, delta.y * invdx);
		 
		    // Scale derivatives by the desired pixel stride and then
		    // offset the starting values by the jitter fraction
		    dP *= stride; dQ *= stride; dk *= stride;
		    P0 += dP * jitter; Q0 += dQ * jitter; k0 += dk * jitter;
		 
		    // Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
		    point3 Q = Q0; 
		 
		    // Adjust end condition for iteration direction
		    float  end = P1.x * stepDir;
		 
		    float k = k0, stepCount = 0.0, prevZMaxEstimate = csOrig.z;
		    float rayZMin = prevZMaxEstimate, rayZMax = prevZMaxEstimate;
		    float sceneZMax = rayZMax + 100;
		    for (point2 P = P0; 
		         ((P.x * stepDir) <= end) && (stepCount < maxSteps) &&
		         ((rayZMax < sceneZMax - zThickness) || (rayZMin > sceneZMax)) &&
		          (sceneZMax != 0); 
		         P += dP, Q.z += dQ.z, k += dk, ++stepCount) {
		         
		        rayZMin = prevZMaxEstimate;
		        rayZMax = (dQ.z * 0.5 + Q.z) / (dk * 0.5 + k);
		        prevZMaxEstimate = rayZMax;
		        if (rayZMin > rayZMax) { 
		           float t = rayZMin; rayZMin = rayZMax; rayZMax = t;
		        }
		 
		        hitPixel = permute ? P.yx : P;
		        // You may need hitPixel.y = csZBufferSize.y - hitPixel.y; here if your vertical axis
		        // is different than ours in screen space
		        sceneZMax = texelFetch(csZBuffer, ivec2(hitPixel), 0).a;
		    }
		     
		    // Advance Q based on the number of steps
		    Q.xy += dQ.xy * stepCount;
		    hitPoint = Q * (1.0 / k);
		    return (rayZMax >= sceneZMax - zThickness) && (rayZMin < sceneZMax);
		}
vec2 SSR(vec3 n,vec3 p,sampler2D h){
//
//	vec2 projPos=(vec2(p.x,p.y) - 0.5) / 0.5;
//	
//	
//	vec3 dira= reflect(v,n);
//	vec2 dir=dira.xy*(pass.screenx/pass.screeny);
//	vec2 start=p;
//	vec2 bestc=vec2(0.5,1);
//	float bestm=99999;
//	
//	
//	float interx;
//	if(dir.x<0){interx=start.x;}else{interx=1-start.x;}
//	
//	float intery;
//	if(dir.y<0){intery=start.y;}else{intery=1-start.y;}
//	float interv=min(interx,intery)/200.0;
//	
//	vec3 ray =normalize( vec3(projPos.x,projPos.y, 1)*pass.farCorners.xyz );
//	
//	for(uint i=0;i<200u;i++){
//		vec2 coord=mod(	start+(float(i)*interv*dir),1);
//		float m1= texture(h,coord).a;
//		vec3 viewPos = (ray)*m1;	
//		float m2=length(viewPos-(v+(float(i)*v*dira)  ));
//		if (bestm>m2){
//			bestm=m2;
//			bestc=coord;
//		}
//	}
//	return vec2(bestc);
	
	vec2 hitPixel;
	vec3 hitPoint;
	bool succ=traceScreenSpaceRay1(
			 // Camera-space ray origin, which must be within the view volume
			 p, 
			 
			 // Unit length camera-space ray direction
			 normalize(reflect(normalize(p),n)),
			 
			 // A projection matrix that maps to pixel coordinates (not [-1, +1]
			 // normalized device coordinates)
			 pass.Proj, 
			 
			 // The camera-space Z buffer (all negative values)
			 h,
			 
			 // Dimensions of csZBuffer
			 vec2(pass.screenx,pass.screeny),
			 
			 // Camera space thickness to ascribe to each pixel in the depth buffer
			 1, 
			 
			 // (Negative number)
			 -pass.nearClip, 
			 
			 // Step in horizontal or vertical pixels between samples. This is a float
			 // because integer math is slow on GPUs, but should be set to an integer >= 1
			 1,
			 
			 // Number between 0 and 1 for how far to bump the ray in stride units
			 // to conceal banding artifacts
			 0,
			 
			 // Maximum number of iterations. Higher gives better images but may be slow
			 100, 
			 
			 // Maximum camera-space distance to trace before returning a miss
			 20, 
			 
			 // Pixel coordinates of the first intersection with the scene
			 hitPixel, 
			 
			 // Camera space location of the ray hit
			 hitPoint);
	
	return (hitPixel);
}

@end