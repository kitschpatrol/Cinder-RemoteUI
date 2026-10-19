<!-- title({ prefix: "Cinder-" }) -->

# Cinder-RemoteUI

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**CinderBlock for ofxRemoteUI. Tweak variables inside your Cinder app over the network in real time.**

<!-- /short-description -->

## Overview

Cinder-RemoteUI helps bridge the superlative [ofxRemoteUI](https://github.com/armadillu/ofxRemoteUI) ecosystem into [Cinder](https://libcinder.org). RemoteUI makes it easy to define and tweak variables in your app over the network in real time.

This [CinderBlock](https://libcinder.org/docs/guides/cinder-blocks/index.html) runs an ofxRemoteUI server inside your app that other UI-only clients can connect to and control. At the most basic level, it's similar to Params or the [many](https://web.archive.org/web/20221205153920/https://github.com/rezaali/Cinder-UI) [other](https://github.com/simongeilfus/Cinder-ImGui) [UI](https://github.com/cwhitney/PretzelGui) [libraries](https://github.com/nselikoff/Cinder-MinimalUI) available for Cinder. Philosophically, however, RemoteUI takes a different approach.

First, you generally don't clutter the app window with windows and controls. Instead, parameters are managed from an external app.

Second, it doesn't presume that you're tweaking parameters from the same machine you're running the app on. You can control the server app from any machine on the local network.

Finally, since RemoteUI speaks a consistent protocol over OSC, you're not limited to a single graphical client to render knobs and dials. ofxRemoteUI provides a native Mac app client, in addition to an iOS client, a cross-platform client written in OF, and even [support for physical hardware](https://github.com/armadillu/ofxMidiFighterTwister).

## Getting started

### Dependencies

Requires Cinder on macOS or Windows, and Cinder's newer OSC block (`cinder/osc/Osc.h`), which ships in `blocks/OSC` with Cinder 0.9.1 and later. C++11 is required.

ofxRemoteUI is included as a git submodule, along with the few openFrameworks pieces it needs, so openFrameworks itself isn't needed. A prebuilt ofxRemoteUIClientOSX app is also included.

Originally tested against the [Cinder master branch](https://github.com/cinder/Cinder/commit/dd16254f0f4ab2276df845f45b604355e64299f2) (0.9.1dev). That commit still has the older oscpack-based OSC block, which the current code can't use. The block was updated in October 2016 for the newer OSC block on Cinder master at that time (for example [81fdbd7](https://github.com/cinder/Cinder/commit/81fdbd72a531bd2828947b8d4d6ef780ef058594), also 0.9.1dev).

In January 2016, TinderBox-generated project files were known to work on:

- Mac OS X 10.11.2 with Xcode 7.2

### Installation

From your Cinder `blocks` directory:

```sh
git clone --recursive https://github.com/kitschpatrol/Cinder-RemoteUI.git
```

Note that this block includes a specific fork of the [ofxRemoteUI](https://github.com/kitschpatrol/ofxRemoteUI) project as a git submodule. This fork includes a number of Cinder-related compatibility fixes. Be sure to clone the block repo recursively. If you already cloned it without `--recursive`, run `git submodule update --init` from the block folder.

Then add **RemoteUI** to a project with [TinderBox](https://libcinder.org/docs/guides/tinderbox/index.html), or take a look at the [RemoteUIBasic sample](#remoteuibasic).

Make sure you've defined `CINDER_AVAILABLE` in your preprocessor macros at the project level. Due to issues with include order, the Cinder macros are not always defined before the RemoteUI code is compiled.

## Usage

Share your variables with the server in `setup()`. The server updates itself and saves values to `ofxRemoteUISettings.xml` on exit through Cinder's app signals, so there's nothing to call in `update()` or `draw()`:

```cpp
#include "ofxRemoteUIServer.h"

void MyApp::setup() {
	RUI_SETUP(); // Start the server

	RUI_SET_SIGNAL(&MyApp::serverCallback); // Optional callback for client events

	RUI_NEW_GROUP("Shape"); // Optional group for the params that follow
	RUI_SHARE_PARAM(mShapeScale, 0.0, 1000.0); // float or int with a range
	RUI_SHARE_PARAM(mIsRotationEnabled); // bool
	RUI_SHARE_PARAM(mLabel); // std::string
	RUI_SHARE_COLOR_PARAM(mBackgroundColor); // ci::ColorA8u

	RUI_LOAD_FROM_XML(); // Restore the values saved by the last run
}

void MyApp::serverCallback(RemoteUIServerCallBackArg arg) {
	if (arg.action == CLIENT_UPDATED_PARAM) {
		CI_LOG_I(arg.paramName << " changed");
	}
}
```

Once you have a RemoteUI-enabled Cinder app up and running, open the ofxRemoteUIClientOSX application in the root of the block folder to connect to and control your app.

The `RUI_` macros are defined in `ofxRemoteUIServerMacros.h`. The ones you'll use most often:

| Macro                                        | Description                                                                                                                                              |
| -------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `RUI_SETUP(port)`                            | Start the server. The port is optional; without one, a random port is chosen and reused on later launches.                                               |
| `RUI_SHARE_PARAM(var, min, max)`             | Share a `float`, `int`, `bool`, or `std::string` member. Numbers take a range.                                                                           |
| `RUI_SHARE_COLOR_PARAM(color)`               | Share a `ci::ColorA8u`.                                                                                                                                  |
| `RUI_SHARE_ENUM_PARAM(var, min, max, names)` | Share an enum as a menu, with a `std::vector<std::string>` or `std::string[]` of item names.                                                             |
| `RUI_NEW_GROUP(name)`                        | Put the params shared after this call in a new group.                                                                                                    |
| `RUI_SET_SIGNAL(&MyApp::callback)`           | Connect a member function to client events such as `CLIENT_CONNECTED` and `CLIENT_UPDATED_PARAM`. Call it from a member function, since it binds `this`. |
| `RUI_LOAD_FROM_XML()` / `RUI_SAVE_TO_XML()`  | Load or save values in `ofxRemoteUISettings.xml`.                                                                                                        |
| `RUI_SET_CONFIGS_DIR(dir)`                   | Set the folder for saved settings.                                                                                                                       |
| `RUI_PUSH_TO_CLIENT()`                       | Send current values to the client, for example after changing a shared variable in code.                                                                 |

See the [ofxRemoteUI readme](https://github.com/kitschpatrol/ofxRemoteUI/blob/master/README.md) for more info on what's possible.

## Examples

### RemoteUIBasic

The [RemoteUIBasic sample](samples/RemoteUIBasic/src/RemoteUIBasicApp.cpp) shares a shape's scale, rotation toggle and speed, background color, and shape type (square, circle, or triangle, as an enum menu), plus a test string. It draws the shape and prints the current values in the window. Launch ofxRemoteUIClientOSX.app to change them while the sample runs.

Open [RemoteUIBasic.xcodeproj](samples/RemoteUIBasic/xcode/RemoteUIBasic.xcodeproj) in Xcode or [RemoteUIBasic.sln](samples/RemoteUIBasic/vc2015/RemoteUIBasic.sln) in Visual Studio 2015. The projects expect the block to be in Cinder's `blocks` directory. Both projects have OSC block path problems listed under [Known issues](#known-issues).

## Updating bundled libraries

ofxRemoteUI lives in the `src/ofxRemoteUI` submodule, which tracks the [kitschpatrol/ofxRemoteUI](https://github.com/kitschpatrol/ofxRemoteUI) fork of [armadillu/ofxRemoteUI](https://github.com/armadillu/ofxRemoteUI). To update it, check out a new commit in the submodule and commit the changed submodule pointer in this repository.

The bundled ofxRemoteUIClientOSX.app was built from the fork's `OSX Client` Xcode project. Its `Info.plist` records commit `3a8ac9c` and Xcode 7.2.

## Background

### Implementation notes

Currently ofxRemoteUI is only very minimally integrated with Cinder. Notable changes are as follows:

- Use Cinder's built-in OSC block instead of ofxOsc.
- Use native Cinder types where possible, e.g. color parameters.
- Use signals instead of OF events to hook into the app lifecycle and deliver callbacks.

Still, there are a few lingering dependencies from OF included in this block:

- TinyXML + ofxXmlSettings.
- A few convenience functions. (See `ofStolenUtils.h`.)

`ofxRemoteUI.h` defines `CINDER_AVAILABLE` itself only if `CINDER_VERSION` is already defined, and the RemoteUI sources don't include `cinder/Cinder.h` before that check. The Visual Studio sample project defines `CINDER_AVAILABLE` in its preprocessor settings. The Xcode sample doesn't, and instead relies on its prefix header including `cinder/Cinder.h` before anything else.

### Other projects

- [ofxRemoteUI](https://github.com/armadillu/ofxRemoteUI) is the original openFrameworks addon.

### Known issues

- Some of the more advanced features from the OF version (in-app UI, in-app notifications) are not yet implemented.
- Presets and saved settings are serialized and persisted with TinyXML instead of Cinder's native XmlTree classes.
- The API is a set of global macros rather than a more Cinder-esque API (no namespaces).
- Color types beyond `ci::ColorA8u` aren't supported.
- The ofxRemoteUI Cinder compatibility changes haven't been merged upstream.
- The server can't run on a background thread in Cinder. `RUI_START_THREADED()` only works in openFrameworks.
- The block doesn't provide anything like ofParameter for Cinder.
- RemoteUI hasn't been rewritten with a leaner, template-based implementation.
- The server's update hook assumes 60 fps rather than measuring the time between frames.
- The bundled ofxRemoteUIClientOSX.app is an unsigned x86_64 build from January 2016, so it needs Rosetta 2 on Apple silicon Macs.
- The Xcode sample's header search path for the OSC block (`../../../../../Cinder/blocks/OSC/src`) looks wrong. It should probably be `$(CINDER_PATH)/blocks/OSC/src`. This hasn't been verified with a build.
- The Visual Studio 2015 sample project wasn't updated for Cinder's newer OSC block and still references `OSC/src/Osc.cpp`. Regenerate it with TinderBox or point it at `OSC/src/cinder/osc`.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

## Acknowledgments

ofxRemoteUI was created by [Oriol Ferrer Mesià](https://uri.cat). Oriol also created an unreleased CinderBlock, parts of which have been integrated into this one.

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/Cinder-RemoteUI/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © Eric Mika

<!-- /license -->

The MIT license covers the sample and the block's `cinderblock.xml`. ofxRemoteUI, in the `src/ofxRemoteUI` submodule, including the Cinder compatibility changes in the [kitschpatrol/ofxRemoteUI](https://github.com/kitschpatrol/ofxRemoteUI) fork, is MIT licensed and copyright Oriol Ferrer Mesià. The bundled ofxRemoteUIClientOSX.app is built from ofxRemoteUI and includes the Growl, VVBasics, and VVMIDI frameworks, which have their own licenses.
