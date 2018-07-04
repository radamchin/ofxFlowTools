//
//  ftFlow.cpp
//  example_core
//
//  Created by Ties East on 28/06/2018.
//

#include "ftFlow.h"

namespace flowTools {
	
	void ftFlow::allocate(int _inputWidth, int _inputHeight, GLint _inputInternalFormat, int _outputWidth, int _outputHeight, GLint _outputInternalFormat) {
		inputWidth = _inputWidth;
		inputHeight = _inputHeight;
		inputInternalFormat = _inputInternalFormat;
		outputWidth = _outputWidth;
		outputHeight = _outputHeight;
		outputInternalFormat = _outputInternalFormat;
		
		inputFbo.allocate(inputWidth, inputHeight, inputInternalFormat);
		ftUtil::zero(inputFbo);
		bInputSet = false;
		
		outputFbo.allocate(outputWidth, outputHeight, outputInternalFormat);
		ftUtil::zero(outputFbo);
		
		visualizeScalar.setup(outputWidth, outputHeight);
		visualizeField.setup(outputWidth / 2, outputHeight / 2);
		toggleVisualisationField = false;
	}
	
	void ftFlow::setInput(ofTexture &_inputTex) {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		ftUtil::stretch(inputFbo, _inputTex);
		bInputSet = true;
		ofPopStyle();
	}
	
	void ftFlow::addInput(ofTexture &_inputTex, float _strength) {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		inputFbo.swap();
		AddMultipliedShader.update(inputFbo, inputFbo.getBackTexture(), _inputTex, 1.0, _strength);
		bInputSet = true;
		ofPopStyle();
	}
	
	void ftFlow::setOutput(ofTexture &_inputTex) {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		ftUtil::zero(outputFbo);
		ftUtil::stretch(outputFbo, _inputTex);
		bInputSet = true;
		ofPopStyle();
	}
	
	void ftFlow::addOutput(ofTexture &_inputTex, float _strength) {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		outputFbo.swap();
		AddMultipliedShader.update(outputFbo, outputFbo.getBackTexture(), _inputTex, 1.0, _strength);
		bInputSet = true;
		ofPopStyle();
	}
	
	void ftFlow::drawOutput(int _x, int _y, int _w, int _h) {
		int internalFormat = ftUtil::getInternalFormat(outputFbo);
		if (internalFormat == GL_R32F || internalFormat == GL_RG32F) {
			if (toggleVisualisationField) { visualizeField.draw(outputFbo.getTexture(), _x, _y, _w, _h); }
			else { visualizeScalar.draw(outputFbo.getTexture(), _x, _y, _w, _h); }
		} else { outputFbo.getTexture().draw(_x, _y, _w, _h); }
	}
	
	void ftFlow::drawInput(int _x, int _y, int _w, int _h) {
		int internalFormat = ftUtil::getInternalFormat(inputFbo);
		if (internalFormat == GL_R32F || internalFormat == GL_RG32F) {
			if (toggleVisualisationField) { visualizeField.draw(inputFbo.getTexture(), _x, _y, _w, _h); }
			else { visualizeScalar.draw(inputFbo.getTexture(), _x, _y, _w, _h); }
		} else { inputFbo.getTexture().draw(_x, _y, _w, _h); }
	}
	
	void ftFlow::setDrawScale(float _scale) {
		int internalFormat = ftUtil::getInternalFormat(inputFbo);
		if (internalFormat == GL_R32F || internalFormat == GL_RG32F) {
			visualizeScalar.setScale(_scale);
			visualizeField.setScale(_scale);
		}
	}
};
