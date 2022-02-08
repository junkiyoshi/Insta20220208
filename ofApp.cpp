#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	
	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofColor color;
	for (int i = 0; i < 16; i++) {

		this->noise_seed_list.push_back(ofRandom(1000));
		this->noise_param_list.push_back(0);

		color.setHsb(ofMap(i, 0, 16, 0, 255), 255, 255);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int radius = 320;
	int span = 28;

	for (int i = 0; i < this->noise_seed_list.size(); i++) {

		for (int deg = 0; deg < 360; deg += 2) {

			auto noise_location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
			auto noise_deg = ofMap(ofNoise(this->noise_seed_list[i], noise_location.x * 0.0025, noise_location.y * 0.0025, this->noise_param_list[i]), 0, 1, -180, 180);

			auto loc = this->make_point(radius, radius * 0.3, noise_deg, deg);
			loc.z = 0;

			this->mesh.addVertex(loc);
			this->mesh.addColor(ofColor(this->color_list[i], 0));
		}

		this->noise_param_list[i] += 0.008;
	}

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->mesh.getNumVertices(); k++) {

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < span) {

				auto alpha = distance < span * 0.25 ? 255 : ofMap(distance, span * 0.25, span, 255, 0);

				if (this->mesh.getColor(i).a < alpha) {

					this->mesh.setColor(i, ofColor(this->mesh.getColor(i), alpha));
				}

				if (this->mesh.getColor(k).a < alpha) {

					this->mesh.setColor(k, ofColor(this->mesh.getColor(k), alpha));
				}

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->mesh.drawWireframe();

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		ofDrawSphere(this->mesh.getVertex(i), 1.85);
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}