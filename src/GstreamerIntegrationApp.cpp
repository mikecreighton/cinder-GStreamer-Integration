#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "GstGLVideoPlayer.h"
#include <gst/gst.h>
#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;

class GstreamerIntegrationApp : public AppBasic {
  public:
	void setup();
	void prepareSettings( Settings *settings );
	void update();
	void draw();
  
	shared_ptr<GstGLVideoPlayer> video;
  
};

void GstreamerIntegrationApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(1920 * .5f, 1080 * .5f);
	settings->setFrameRate(60.f);
}


void GstreamerIntegrationApp::setup()
{

	video = shared_ptr<GstGLVideoPlayer>(new GstGLVideoPlayer());
	// video->init("1920x1080_5s_rgba_animation_75_1kf.mov", GST_PIXELS_RGBA);
	video->init("the_avengers_trailer.mp4", GST_PIXELS_RGB);
	video->setLoopMode(LOOP_NORMAL);
	video->play();
	video->setRate(1.f);
}

void GstreamerIntegrationApp::update()
{
	if(video) {

		video->update();
		// Check if it's done.
		if(video->getIsDone()) {
			console() << "Our video is done." << endl;
		}

	}
}

void GstreamerIntegrationApp::draw()
{
	gl::clear( Color( 0.1, 0.1, 0.1 ) ); 
	gl::enableAlphaBlending(false);
  
  // Draw the video's texture.
	if(video) {
		// video->draw();
		gl::Texture * mFrameTexture = video->getTexture();
		if(mFrameTexture != NULL) {
			Rectf centeredRect = Rectf( (*mFrameTexture).getBounds() ).getCenteredFit( getWindowBounds(), true );
			gl::draw( *mFrameTexture, centeredRect  );
		}
	}
  
  // Draw the FPS.
	gl::disableDepthWrite();

	float fpsF = getAverageFps();
	ostringstream conv;
	conv << fpsF;
	string fps = conv.str();
	gl::drawString(fps, Vec2f(10.f, getWindowHeight() - 20.f));

	gl::enableDepthWrite();
}

CINDER_APP_BASIC( GstreamerIntegrationApp, RendererGl )
