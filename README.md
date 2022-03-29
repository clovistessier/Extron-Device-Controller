# Extron-Device-Controller
This openFrameworks application provides a bridge from MIDI and OSC to Extron's Simple Instruction Set (SIS) for performance control of Extron Video Scalers and Signal Processors

## Description

### Video Scalers and Signal Processors

Within the video art community, old video gear such video mixers, switchers, and video processors are used because the analog nature of the composite video signal can be manipulated to create interesting visual effects and glitches, especially when the signal is put into a feedback loop, that can't be achieved by other means. This has given a new life to these tools, originally created for live video production and VHS tape editing workflows, as performance instruments used in experimental video art, music videos, and live music accompaniment.

Video scalers are commonly used in video art at either end of the signal chain to convert to and from HD video signals. At the beginning of the signal chain, video scalers are used to downscale HD video from a computer to standard definition over composite video or S-video that can be accepted by the rest of the signal chain. At the other end, video scalers can also be used to convert the final composite or S-video signal back to HD for projection or capture. Extron, a company that makes professional AV equipment and is such unknown to consumers, makes some video scalers that are fairly well known in the video art community for being robust, high fidelity, and low latency. They also tend to be fairly affordable in the secondhand market since the original buyers have all converted their workflows to HD and no longer need SD devices. 

Video processors are commonly used in video art to manipulate video feedback, as preamps/equalizers for video signals before feeding them into sensitive glitch devices, or even as glitch devices themselves after being circuit bent. As such, consumer oriented devices that feature a lot of front panel controls and analog internal signals that can be easily circuit bent tend to be more popular, and Extron's offerings in this area, which tend to be rack mount, hide all their controls behind menus, and use primarily digital internal signals, don't really get a lot of play. Somewhat uniquely among video processors, they do allow for repositioning of the video on the screen, which is very useful when used in video feedback loops.

### Extron SIS

However, Extron video scalers and signal processors do have a trick up their sleeves: **RS-232 control.** Nearly every Extron device I own has an RS-232 Serial port on the back, and what's more, Extron has implemented its own [Simple Instruction Set](https://www.extron.com/article/tech92) (SIS) that is common between all of their devices. The original idea behind this is that an AV operator who might have to manage a dozen or more devices in an equipment rack (that might not even be in the same room) could configure everything from a single computer using a terminal or Extron's own Product Configuration Software. It also allows for the use of remote controllers like touch panels in classrooms or boardrooms. 

Since the SIS implementation is open and explained in the user manuals of Extron devices, I developed this application to convert MIDI and OSC messages to SIS messages in order to expose all of the positioning, size, and picture controls. Knobs and sliders from a USB MIDI controller provide the direct, tangible control needed in a performance context, and OSC also allows for control signals from multimedia and creative coding applications to modulate and automate the picture control parameters.

The following parameters can be controlled:
- H/V Positioning
- H/V Size
- Scale (This is just a composite of positioning and size)
- Brightness
- Contrast
- Saturation
- Tint
- Detail

## Dependencies

This application was built using openFrameworks. Follow the setup guide for the oF core [here](https://openframeworks.cc/download/).

This application requires [ofxMidi](https://github.com/danomatika/ofxMidi). Follow the installation instructions in the repository to add it your openFrameworks addons folder.

The other ofx addons required by this application are included in the core install of openFrameworks.
- ofxGui
- ofxOsc

## Installation

1. Download [openFrameworks](https://openframeworks.cc/) (tested on 0.11.0 and 0.11.2)
2. Install the [ofxMidi](https://github.com/danomatika/ofxMidi) addon
3. clone this repository into apps/myApps of your oF install
4. import with the projectGenerator to update it for your system
5. build and run using your IDE 

## Usage

I developed this application to interface with two specific Extron devices that I use in my video art practice: [Extron DSC 301 HD Video Scaler](https://www.extron.com/product/dsc301hd) and [Extron USP 405 Universal Signal Processor](https://www.extron.com/product/usp405). I typically use the DSC 301 HD in a camera feedback loop: I connect the composite video output of my video mixer to the composite video input of the scaler, I connect the HDMI output of the scaler to an HDMI monitor, and finally I connect the composite video output of a camera pointed at the HDMI monitor back into a composite video input of the video mixer. Whereas most camera feedback setups involve moving the camera or the monitor to manipulate the feedback, this allows me to leave the camera and monitor stationary and use the positioning and size controls of the scaler to control where the image is drawn on the screen and thus steer the feedback that way. I use the USP 405 in an internal mixer feedback loop: I connect the S-video output of my video mixer to the S-video input of the USP 405, and I connect the S-video output of the USP 405 back into an S-video input of the video mixer.

I use a USB to RS-232 Serial Adapter cable to connect to connect my computer to the DB-9 port of the USP 405. The DSC 301 HD exposes its RS-232 port as a 3 pole captive screw connector, but it also accepts Serial input from the Mini-USB port on the front, which is what I use. The data format is exactly the same: 8N1 at 9600 baud.

Because I wanted to control both of these devices from the same application, I structured the application such that the two interfaces are two separate member variables of ofApp. They are initialized using setup(), which is called during ofApp::setup(), and needs to be updated in the main program loop by calling update() during ofApp::update(). All incoming MIDI and OSC messages are interpreted by ofApp, and relevant message information is passed to the interface objects using updateFromMidi() and updateFromOSC()

Each parameter has three components, the control value, a modulation input, and a modulation depth. The intended use is to use the control values to set each parameter where desired, included performing with them live, and then to send external signals to the modulation input using OSC to add animation. The amount of modulation is controlled by the modulation depth, which can go from -1.0 to 1.0.

### Demo Video

*Coming Soon*

### DSC 301 HD vs USP 405

Based on the copyright dates on their product brochures the DSC 301 HD was released in 2013, while the USP 405 was released in 2002. As such, the DSC 301 HD has some additional features and uses slightly different SIS commands, along with different ranges for its parameters. The USP 405 uses single characters in the SIS command to differentiate between the different controls, while the DSC 301 HD uses 4. For example, to set the tint value to 64 on the USP 405, you need to send `64T` while setting the tint value to 64 on the DSC 301 HD requires starting with the escape character, followed by `64TINT` followed by a carriage return character.

Unlike the DSC 301 HD, which allows you to set the horizontal and vertical centering to specific values, the USP 405 only allows for incrementing and decrementing of the horizontal and vertical centering. This requires that the application keep track of the horizontal and vertical centering and send the correct number of increment and decrement values to get to the desired value. I'm still fine tuning how quickly these SIS commands can be sent before the USP 405 gets overwhelmed and misses a step. The arrow keys are also mapped to send out horizontal and vertical centering commands directly in order to center the image when the position x and position y control are set to 0, and also for adjusting for any missed steps. Horizontal and vertical size on the USP 405 work the same way, but I haven't tackled them yet. In practice, this means that the USP 405 is much slower to react to changes in position as it needs time to catch up to where it's been told to be.

The Scale control in the application uses the XIMG SIS command on the DSC 301 HD, which allows for the direct setting of the horizontal centering, vertical centering, horizontal size, and vertical size all in one command. This allows for a zoom that is centered on the middle of the image, even though the horizontal and vertical size controls stretch the image from the right side and bottom, respectively. The USP 405 doesn't have such a command, and trying to increment/decrement the four relevant controls would result in a very jerky animation. The USP 405's Zoom control could be used to similar effect, but its step sizes are quite large and it also anchors the top left corner of the image when zooming.

It's important to remember in all of this that these devices were never intended for animation and performance in this way. These control ports were intended for configuration and then expected to be left alone. While the DSC 301 HD's processor is fast enough to afford these performance use cases in a smooth manner, the USP 405 has a harder time keeping up, and the resulting motion and changes to the image aren't always as smooth when trying to make large, fast adjustments.

### MIDI

The MIDI port is opened in ofApp::setup(). You can pass either a port number or name. The names and port numbers of all MIDI devices seen by your computer are listed in the console when the program starts.

When I only needed to control one device, I used the [Korg nanoKONTROL2 USB MIDI controller](https://www.korg.com/us/products/computergear/nanokontrol2/). It's a great place to start, even if the faders are a little short. Nowadays I use the [Novation Launch Control XL](https://novationmusic.com/en/launch/launch-control-xl), and all of the default MIDI CC mappings reflect the layout of the first two factory templates on that device. The parameters are controlled by the faders, and the first knob directly above each fader controls the modulation depth (-1.0 to 1.0). MIDI CC messages received on channel 1 are passed to the DSC 301 object, and MIDI CC messages received on channel 2 are passed to the USP 405 object. Since I haven't yet implemented a Scale feature for the USP 405, that parameter doesn't do anything at the moment.

The default MIDI CC mappings are as follows:

| Parameter  | MIDI CC |
| ---------- | ------- |
| Position X | 77      |
| Position Y | 78      |
| Scale      | 79      |
| Brightness | 80      |
| Contrast   | 81      |
| Color      | 82      |
| Tint       | 83      |
| Detail     | 84      |

| Mod Depth  | MIDI CC |
| ---------- | ------- |
| Position X | 49      |
| Position Y | 50      |
| Scale      | 51      |
| Brightness | 52      |
| Contrast   | 53      |
| Color      | 54      |
| Tint       | 55      |
| Detail     | 56      |

These values are stored in midi_cc_lut in Extron.h

### OSC

[Open Sound Control](https://opensoundcontrol.stanford.edu/) is a protocol for sending real time communication messages between synthesizers, computers, and other multimedia devices. 

By default, this application listens for the OSC messages on port 7001. If you want to change this, the relevant #define is at the top of ofApp.h

This application expects OSC messages to contain a single float argument within the range 0.0 to 1.0. These values are sent to the modulation inputs of the appropriate parameter, where their influence on the final parameter value is scaled by the modulation depth. Since I haven't yet implemented a Scale feature for the USP 405, that modulation parameter doesn't do anything at the moment.

The default OSC address map is as follows:

| Parameter  | DSC 301 HD   | USP 405      |
| ---------- | ------------ | ------------ |
| Position X | 1/pos_x      | 2/pos_x      |
| Position Y | 1/pos_y      | 2/pos_y      |
| Scale      | 1/scale      | 2/scale      |
| Brightness | 1/brightness | 2/brightness |
| Contrast   | 1/contrast   | 2/contrast   |
| Color      | 1/color      | 2/color      |
| Tint       | 1/tint       | 2/tint       |
| Detail     | 1/detail     | 2/detail     |

These values are stored in osc_addr_map which gets initialized in the constructor found in Extron.cpp for DSC 301 HD and USP_405.cpp for USP_405.

I use this with the OSC routing application [OSCulator](https://osculator.net/), which is highly recommended if you're using MacOS. [OSCRouter](https://github.com/ETCLabs/OSCRouter) from ETCLabs is another option.

## Improvements

The next improvement I intend to add to the code is exposing more of the configuration options, such as midi channel, midi CCs to listen to, OSC addresses to listen to, and the output serial port. This should make the code more portable and easier to adapt to other Extron devices, as well as easier to drop into your own projects.

The next area for improvement is the position and size controls of the USP 405, and finding the speed limit at which these controls can be changed for increased responsiveness and smoother motion. Both of these devices send response messages once they've enacted the change requested in an SIS message. I've ignored these responses with the DSC 301 HD, but waiting for the response before sending another commmand seems to be more important with the USP 405.

I also want to develop interfaces for other devices used in video art, such as the Edirol V4 or Panasonic MX50, and have this same application pass along MIDI or OSC messages to those devices.

I think this program is probably lightweight enough to run on a Raspberry Pi, which is something that I will be exploring in the future once I've added all the features I want, since a Raspberry Pi is the perfect size to mount into the 19" equipment rack that all of my Extron devices are in.