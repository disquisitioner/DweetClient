Dweet Client

This is a simple Arduino library aiming to make it easy for devices to post short status messages to [Dweet.io](https://dweet.io/).  If you're not familar with Dweet.io, it's a ridiculously simple messaging platform for the Internet of Things, enabling fast and free publishing of short messages, or "dweets". I've found it useful for all sorts of purposes across a wide-variety of connected devices:
- Posting status messages to summarize activity, e.g. the lastest reading from attached sensors, making it easy to check that remote devices are functioning (or perhaps when they last stopped functioning)
- Publishing error messages or alerts of unexpected events, allowing remote toubleshooting as you might do via the Arduino Serial Monitor but for devices with no local serial connection.
- Logging state changes in an application, e.g., sleep/wake, commands received, important user operations, etc.
- Making it easy to do incremental development by enabling low-effort publishing of intermediate states in ongoing feature implementation, especially for devices that need to operate in a live environment for a while so you can see how well they work.
- Offering a very simple alternative service connection endpoint alongside others so you can more easily tell when a problem is due to the device itself or to a service. (E.g., MQTT messages aren't being sent by the device, but it is still posting dweets).

Dweet.io the marvelous brainchild and free offering from the delightful folks at [BugLabs, Inc.](https://www.buglabs.net/) who have my undying admiration and thanks for all their great work.

# Operations
Dweet.io uses HTTP POST operations to publish a dweet, which can include a data payload in JSON or as query parameters.  Traffic can flow over HTTP or HTTPS. By default, successful posting results in a reply that confirms the operation and echos the payload sent.

By default dweets are public, only persist on dweet.io for twenty four hours, and only the most recent five dweets from a device are retained.  I've never found those to be serious limitations as I have other services I can use for greater volumes of data, privacy, or or longer storage.  You can also subscribe to a device's dweets but I've not yet needed that capability for any of my projects.

# Usage

An example application is included in the `examples` folder.  The DweetClient library contains the following functions:

**`DweetClient(String server, String device, bool verbose)`**
Constructor for the DweetClient object.  `Server` is almost always "dweet.io" but may be otherwise in future implementations. Each dweeting device needs a unique name on dweet.io so its messages can be viewed, which is specified in the `device` argument.  By default, successful dweets return a copy of any data payload included in the dweet along with status information from the server, but any posted message can request processing in "quiet" mode which only returns HTTP status code 204.  This behavior is controlled by the `verbose` argument, which if `true` specifies the default mode and if `false` enables "quiet" mode.

**`postPayloadJSON(JsonDocument payload)`**
Initiates a dweet message and provides a data payload to be included. In this case the payload is formatted as a JSON Document, as supported by the [ArduinoJson library](https://github.com/bblanchon/ArduinoJson).  The JsonDocument will be serialized so it can be passed properly via HTTP POST, which is handled internally within this function.  For large or complex dweet payloads using JSON directly ensures the payload is properly constructed and will be allowed by the Dweet server.  The ArduinoJson library makes this easy, as illustrated in the Dweet Client example included here.

**`postPayloadString(String payload)`**
Initiates a dweet message and provides a data payload as a String.  The contents of this String should be a syntactically correct JSON object, which in simple cases it is easy to construct as an Arduino String that can be passed directly via HTTP POST.  Applications may choose to use this way of sending dweets rather than having to create JsonDocument objects and the ArduinoJson library.

**`setVerbose(bool verbose)`**
Allows switching between verbose (when `verbose is `true`) and "quiet" (when `verbose` is `false`) dweet processing, overriding the value specified in the DweetClient object constructor.

## Viewing Device Dweets
The latest dweet for a thing named `my-thing-name` can be viewed by pointing any web brower at `https://dweet.io/get/latest/dweet/for/my-thing-name`.

For more information on dweeting checkout [Dweet.io](https://dweet.io), including a Play page that allows creating or reading dweets, and the Discover page to explore dweets from actively dweeting devices.

