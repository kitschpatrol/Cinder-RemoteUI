#include "cinder/Log.h"
#include "cinder/Utilities.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ofxRemoteUIServer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum ShapeOptions {
	SQUARE,
	CIRCLE,
	TRIANGLE,
};

class RemoteUIBasicApp : public App {
public:
	static void prepareSettings(Settings *settings);
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
	void serverCallback(RemoteUIServerCallBackArg arg);
	
	// Fonts
	Font mFont;
	gl::TextureFontRef mFontTexture;
	
	// Remote Params
	float mShapeScale;
	float mRotationSpeed;
	bool mIsRotationEnabled;
	ColorA8u mBackgroundColor;
	ShapeOptions mCurrentShape;
	string mTestString;
	vector<string> shapeNames;
};

void RemoteUIBasicApp::prepareSettings(Settings *settings) {
	settings->setTitle("Cinder Remote UI Server Example");
	settings->setPowerManagementEnabled(true); // Prevent App Nap
	settings->setFrameRate(60.0);
}

void RemoteUIBasicApp::setup() {
	// Load a font
	mFont = Font("Helvetica", 13);
	mFontTexture = gl::TextureFont::create(mFont);
	
	// Set parameter defaults
	mShapeScale = 300.0;
	mIsRotationEnabled = true;
	mBackgroundColor = ColorA8u(255, 0, 128, 255);
	mCurrentShape = ShapeOptions::TRIANGLE;
	mRotationSpeed = 0.01;
	mTestString = "Superstring";
	
	shapeNames = {"Square", "Circle", "Triangle"};
	
	// Start the server
	RUI_SETUP();
	
	// Get interesting messages
	// This is the cinder equivalent of "RUI_SET_CALLBACK();", but it uses signals instead of events.
	RUI_SET_SIGNAL(&RemoteUIBasicApp::serverCallback);
	
	// RUI_GET_INSTANCE()->setVerbose(true);
	
	// chose a new random color for all the upcoming variables
	RUI_NEW_GROUP("STUFF"); // make a new group (optional)
	RUI_SHARE_PARAM(mShapeScale, 0.0, 1000.0);
	RUI_SHARE_PARAM(mIsRotationEnabled);
	RUI_SHARE_PARAM(mRotationSpeed, -0.25, 0.25);
	RUI_SHARE_COLOR_PARAM(mBackgroundColor);
	
	// privide the enum param, loweset enum, highest enum, and the Enum string list
	RUI_SHARE_ENUM_PARAM(mCurrentShape, SQUARE, TRIANGLE, shapeNames);
	
	RUI_SHARE_PARAM(mTestString);
	
	RUI_LOAD_FROM_XML();
	
	CI_LOG_V("\n\nLaunch ofxRemoteUIClientOSX.app to control variables remotely!");
}

void RemoteUIBasicApp::mouseDown(MouseEvent event) {
}

void RemoteUIBasicApp::update() {
}

void RemoteUIBasicApp::draw() {
	gl::clear(mBackgroundColor);
	gl::color(Color::white());
	gl::pushMatrices();
	gl::translate(getWindowWidth() * 0.5, getWindowHeight() * 0.5);
	
	if (mIsRotationEnabled) {
		static float rotation = 0;
		rotation += mRotationSpeed;
		gl::rotate(rotation);
	}
	
	gl::scale(mShapeScale, mShapeScale);
	
	switch (mCurrentShape) {
		case SQUARE:
			gl::drawSolidRect(Rectf(-0.5, -0.5, 0.5, 0.5));
			break;
		case CIRCLE:
			gl::drawSolidCircle(vec2(0), 0.5, 18);
			break;
		case TRIANGLE:
			gl::drawSolidTriangle(vec2(0, -0.5), vec2(.433, .25), vec2(-.433, .25));
			break;
	}
	
	gl::popMatrices();
	
	// Print the values
	vec2 position = vec2(5, 15);
	vec2 step = vec2(0, 15);
	mFontTexture->drawString("mShapeScale:\t\t" + toString(mShapeScale), position);
	position += step;
	mFontTexture->drawString("mRotationSpeed:\t\t" + toString(mRotationSpeed), position);
	position += step;
	mFontTexture->drawString("mIsRotationEnabled:\t" + toString(mIsRotationEnabled), position);
	position += step;
	mFontTexture->drawString("mBackgroundColor:\t" + toString(mBackgroundColor), position);
	position += step;
	mFontTexture->drawString("mCurrentShape:\t\t" + shapeNames[mCurrentShape], position);
	position += step;
	mFontTexture->drawString("mTestString:\t\t\t" + mTestString, position);
	position += step;
}

void RemoteUIBasicApp::serverCallback(RemoteUIServerCallBackArg arg) {
	switch (arg.action) {
		case CLIENT_CONNECTED:
			CI_LOG_V("CLIENT_CONNECTED");
			break;
		case CLIENT_DISCONNECTED:
			CI_LOG_V("CLIENT_DISCONNECTED");
			break;
		case CLIENT_UPDATED_PARAM:
			CI_LOG_V("CLIENT_UPDATED_PARAM " << arg.paramName << ": ");
			arg.param.print();
			break;
			/*see RemoteUICallServerAction enum for more event callbacks */
		default:
			break;
	}
}

CINDER_APP(RemoteUIBasicApp, RendererGl(RendererGl::Options().msaa(4)), RemoteUIBasicApp::prepareSettings)
