
#version 330 core

#define RENDER_MODE_SELF_ILLUMINATION 1
#define RENDER_MODE_USE_TEXTURE 2

out vec4 FragColor;

in vec3 fragPos;
in vec4 fragPosLightSpace;
in vec4 fragPosLightSpace2;
in vec3 normalFrag;
in vec3 camPosFrag;
in vec2 texCoordFrag;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler;
uniform sampler2D shadowSampler2;
uniform int renderMode;
uniform vec3 color;

vec4 tex2DBiLinear( sampler2D textureSampler_i, vec2 texCoord_i )
{
   float scale = 1024.0;
   vec4 p0q0 = texture(textureSampler_i, texCoord_i);
   vec4 p1q0 = texture(textureSampler_i, texCoord_i + vec2(1, 0)/scale);

   vec4 p0q1 = texture(textureSampler_i, texCoord_i + vec2(0, 1)/scale);
   vec4 p1q1 = texture(textureSampler_i, texCoord_i + vec2(1 , 1)/scale);

   float a = fract( texCoord_i.x * scale ); // Get Interpolation factor for X direction.
	 // Fraction near to valid data.

   vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
   vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

   float b = fract( texCoord_i.y * scale );// Get Interpolation factor for Y direction.
   return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
}

void main()
{

   vec3 light;
   


   if((renderMode & RENDER_MODE_SELF_ILLUMINATION) == RENDER_MODE_SELF_ILLUMINATION){
      light = vec3(1,1,1);
   }else{
      // ambient
      float ambientStrength = 0.3;
      vec3 lightColor = vec3(1,1,1);
      vec3 ambient = ambientStrength * lightColor;
      
      // diffuse
      vec3 normal = normalize(normalFrag);
      //vec3 lightPos = vec3(4,4,4);
      //vec3 lightDir = normalize(lightPos - fragPos);
      vec3 lightDir = normalize(vec3(-1,-1,-1));
      float diff = max(dot(normal, -lightDir), 0.0);
      vec3 diffuse = diff * lightColor;
      
      // specular
      vec3 viewDir = normalize(fragPos - camPosFrag);
      vec3 reflectDir = reflect(lightDir, normal);
      float spec = pow(max(dot(-viewDir, reflectDir), 0.0), 8.0);
      vec3 specular = lightColor * (spec * 1.0);
      
      // Shadow.
      vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
      vec3 projCoords2 = fragPosLightSpace2.xyz / fragPosLightSpace2.w;
      projCoords = projCoords * 0.5 + 0.5;
      projCoords2 = projCoords2 * 0.5 + 0.5;
      // Shadow.
      float shadow;
      bool shadow1Inside = true;
      if(
         projCoords.x < 0.0 || projCoords.x > 1.0 ||
         projCoords.y < 0.0 || projCoords.y > 1.0 ||
         projCoords.z < 0.0 || projCoords.z > 1.0
      ){
         shadow1Inside = false;
         shadow = 0.;
      }else{
        shadow = 0.0;
        float shift = 1./pow(2,12);
        vec2 samples[4] = vec2[](
           vec2(projCoords.x + shift, projCoords.y + shift),
           vec2(projCoords.x + shift, projCoords.y - shift),
           vec2(projCoords.x - shift, projCoords.y + shift),
           vec2(projCoords.x - shift, projCoords.y - shift)
        );
        for(int i=0;i<4;i++){
           float closestDepth = texture(shadowSampler, samples[i]).r;
           float currentDepth = projCoords.z;
           float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.005) * 0.01;  
           //bias = 0.0;
           if(currentDepth - bias <= closestDepth){
              shadow += 0.25;
           }
        }
      }
      // Shadow 2.
      float shadow2;
      bool shadow2Inside = true;
      if(
         projCoords2.x < 0.0 || projCoords2.x > 1.0 ||
         projCoords2.y < 0.0 || projCoords2.y > 1.0 ||
         projCoords2.z < 0.0 || projCoords2.z > 1.0
      ){
         shadow2Inside = false;
         shadow2 = 1.;
      }else{
        shadow2 = 0.0;
        float shift = 1./pow(2,13);
        vec2 samples[4] = vec2[](
           vec2(projCoords2.x + shift, projCoords2.y + shift),
           vec2(projCoords2.x + shift, projCoords2.y - shift),
           vec2(projCoords2.x - shift, projCoords2.y + shift),
           vec2(projCoords2.x - shift, projCoords2.y - shift)
        );
        for(int i=0;i<4;i++){
           float closestDepth = texture(shadowSampler2, samples[i]).r;
           float currentDepth = projCoords2.z;
           float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.005) * 0.02;
           //bias = 0.0;
           if(currentDepth - bias <= closestDepth){
              shadow2 += 0.25;
           }
        }
      }
      shadow = shadow1Inside? shadow: shadow2;
      //float f = min(dot(projCoords - vec3(0.5), projCoords - vec3(0.5)) / 0.25, 1.0);
      //shadow = mix(shadow, shadow2, f);
      
      // Combine.
      light = (ambient + (diffuse + specular) * shadow);
      //light = ambient + (diffuse + specular);
      //light.r = shadow;
      //light.g = shadow2;
   }
   
   //
   vec3 objectColor;
   if((renderMode & RENDER_MODE_USE_TEXTURE) == RENDER_MODE_USE_TEXTURE){
      objectColor = vec3(texture(textureSampler, texCoordFrag));
   }else{
      objectColor = color;
   }
   vec3 result = light * objectColor;

   //TEST
   //result.x = texture(shadowSampler, gl_FragCoord.xy/vec2(1200., 900.)).x;
   //result.xy = gl_FragCoord.xy/1000.;
   //result.x = fragPosLightSpace.w;
   //TEST END

   FragColor = vec4(result, 1.0);
}





