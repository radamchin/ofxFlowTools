
#pragma once
#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {
	
	class ftVelocityBridgeShader : public ftShader {
	public:
		ftVelocityBridgeShader(){
			bInitialized = 1;
			
			if (ofIsGLProgrammableRenderer())
				glThree();
			else
				glTwo();
			
			if (bInitialized)
				ofLogNotice("ftVelocityBridgeShader initialized");
			else
				ofLogWarning("ftVelocityBridgeShader failed to initialize");
		}
		
	protected:
		void glTwo() {
			fragmentShader = GLSL120(
									 uniform sampler2DRect tex0;
									 uniform sampler2DRect tex1;
									 uniform float weight;
									 
									 void main(){
										 vec2 baseVel = texture2DRect(tex0, gl_TexCoord[0].st).xy;
										 vec2 blendVel = texture2DRect(tex1, gl_TexCoord[0].st).xy;
										 vec2 vel = (baseVel * weight) + blendVel;
										 float magnitude = min(length(vel), 1);
										 vel = normalize(vel) * magnitude;
										 gl_FragColor = vec4(vel, 0.0, 1.0);
									 }
									 );
			
			bInitialized *= shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= shader.linkProgram();
			
		}
		
		void glThree() {
			
			fragmentShader = GLSL150(
									 uniform sampler2DRect tex0;
									 uniform sampler2DRect tex1;
									 uniform float weight;
									 
									 in vec2 texCoordVarying;
									 out vec4 fragColor;
									 
									 void main(){
										 vec2 baseVel = texture(tex0, texCoordVarying).xy;
										 vec2 blendVel = texture(tex1, texCoordVarying).xy;
										 vec2 vel = (baseVel * weight) + blendVel;
										 float magnitude = min(length(vel), 1);
										 vel = normalize(vel) * magnitude;
										 fragColor = vec4(vel, 0.0, 1.0);
									 }
									 );
			
			bInitialized *= shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			bInitialized *= shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= shader.bindDefaults();
			bInitialized *= shader.linkProgram();
			
		}
		
	public:
		void update(ofFbo& _fbo, ofTexture& _baseTex,  ofTexture& _blendTex, float _weight){
			_fbo.begin();
			shader.begin();
			shader.setUniformTexture("tex0", _baseTex, 0);
			shader.setUniformTexture("tex1", _blendTex, 1);
			shader.setUniform1f("weight", _weight);
			renderFrame(_fbo.getWidth(), _fbo.getHeight());
			shader.end();
			_fbo.end();
		}
	};
}