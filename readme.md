#Cinder-RemoteUI

**Cinder-RemoteUI helps bridge the superlative [ofxRemoteUI](https://github.com/armadillu/ofxRemoteUI) ecosystem into [Cinder](http://libcinder.org). RemoteUI makes it easy to define and tweak variables in your app over the network in real time.**

This Cinderblock runs an ofxRemoteUI server inside your app that other UI-only clients can connect to and control. At the most basic level, it's similar to Params or the [many](https://github.com/rezaali/Cinder-UI) [other](https://github.com/simongeilfus/Cinder-ImGui) [UI](https://github.com/cwhitney/PretzelGui) [libraries](https://github.com/nselikoff/Cinder-MinimalUI) available for Cinder. Philosophically, however, RemoteUI takes a different approach.

First, you generally don't clutter the app window with windows and controls. Instead, parameters are managed from an external app.

Second, it doesn't presume that you're tweaking parameters from the same machine you're running the app on. You can control the server app from any machine on the local network.

Finally, Since RemoteUI speaks a consistent protocol over OSC, you're not limited to a single graphical client to render knobs and dials. ofxRemoteUI provides a native Mac app client, in addition to an iOS client, a cross-platform client written in OF, and even [support for physical hardware](https://github.com/armadillu/ofxMidiFighterTwister).


## Getting Started

Navigate to your `Cinder/blocks` folder, then:

	git clone https://github.com/kitschpatrol/Cinder-OpenCFU --recursive 

Note that this block includes a specific fork of the [ofxRemoteUI](https://github.com/kitschpatrol/ofxRemoteUI) project as a git submodule. This fork includes a number of Cinder-related compatibility fixes. Be sure to clone the block repo recursively.

From there, take a look at the RemoteUIBasic sample or create a new project through Tinderbox.

Make sure you've defined `CINDER_AVAILABLE` in your preprocessor macros at the project level. Looking into a better way to do this, but due to issues with include order the Cinder macros are not always defined before the RemoteUI code is compiled.

Once you have a RemoteUI-enabled Cinder app up and running, open the ofxRemoteUIClientOSX application in the root of the block folder to connect to and control your app.

See the [ofxRemoteUI readme](https://github.com/kitschpatrol/ofxRemoteUI/blob/master/README.md) for more info on what's possible.

## Cinder Integration

Currently ofxRemoteUI is only very minimally integrated with Cinder. Notable changes are as follows:

- Use Cinder's built-in OSC block instead of ofxOSC.
- Use native Cinder types where possible, e.g. color parameters.
- Use signals instead of OF events to hook into app lifecycle and provide deliver callbacks.

Still, there are few lingering dependencies from OF included in this block:

- TinyXML + ofxXmlSettings.
- A few convenience functions. (See ofStolenUtils.h)

Some of the more advanced features from the OF version (in-app UI, in-app notifications) are not yet implemented.

## Origin

ofxRemoteUI was created by [Oriol Ferrer Mesià](http://uri.cat). Oriol also created an unreleased Cinderblock, parts of which have been integrated into this one.

## Compatibility

C++1x is required.

Tested against the [Cinder master branch](https://github.com/cinder/Cinder/commit/dd16254f0f4ab2276df845f45b604355e64299f2) (~v9.1).

Tinderbox-generated project files are known to work on:

- Mac OS X 10.11.2 with Xcode 7.2

##TODO

Sooner:

- Use Cinder's native XMLTree classes instead of TinyXML for preset serialization and persistence.
- Nicer, more Cinder-esque API. (Namespaces? Alternatives to the macros?) 
- Seamlessly support color types beyond ci::ColorA8u.
- Merge ofxRemoteUI Cinder compatibility changes upstream.

Later:

- Port over in-app UI.
- Create something like ofParameter for Cinder?
- Threaded option.
- Completely rewrite RemoteUI with a leaner, template-based implementation.
