A new project has been undertaken to replace the current tablets used as stop jambs, with physical controls.

The tablets were running the app version of Elgato StreamDeck, and as such required a steady Wi-Fi connection to the host laptop in order to work.  However it turned out the Wi-Fi just wasn’t stable enough on these (rather old) models, and so they would randomly drop connections, reset, or otherwise just lose sync with the GrandOrgue VPO, making it impossible to know what stops were pulled.

The new project replaces these 4 tablets, with 2 custom-made jambs.

Each jamb consists of:
* Proto PCB (90mm x 150mm) x 4 (<a href="https://thepihut.com/products/proto-pcb-10-pack-9x15cm?variant=43674817200323">The Pi Hut link</a>)
* Momentary switches (5mm) x 30 (<a href="https://thepihut.com/products/tactile-switch-buttons-6mm-tall-x-10-pack?variant=27739414097">The Pi Hut link</a)
* Soft caps x 30 (<a href="https://thepihut.com/products/black-soft-caps-for-tactile-buttons-1-pack?variant=42219896832195">The Pi Hut link</a>)
* Signal Diodes x 30 (<a href="https://thepihut.com/products/1n4148-signal-diode-10-pack?variant=27739521937">The Pi Hut link</a>)
* Adafruit NeoPixel 5mm Diffused Though-Hole LEDs x 30 (<a href="https://thepihut.com/products/neopixel-diffused-5mm-through-hole-led-5-pack">The Pi Hut link</a>)
* Wooden drawer knobs (26mm x 25mm) x 30 (<a href="https://www.amazon.co.uk/dp/B0932TW166?ref=ppx_yo2ov_dt_b_fed_asin_title">Amazon UK link</a>)
* Clear Acrylic tubes (ID 14mm, OD 20mm) cut into 30 'stubs' of 15mm (<a href="https://www.amazon.co.uk/dp/B0DJ7J21SR?ref=ppx_yo2ov_dt_b_fed_asin_title">Amazon UK</a>)

The two jambs are then wired into a single processing unit, currently consisting of:
* Full-sized breadboard x 1 (<a href="https://thepihut.com/products/full-sized-breadboard?variant=27739878097">The Pi Hut link</a>)
* Level Shifter (3V to 5V) x 1 (<a href="https://thepihut.com/products/74ahct125-quad-level-shifter-3v-to-5v?variant=27739617873">The Pi Hut link</a>)
* DC barrel jack (2.1mm) x 1 (<a href="https://thepihut.com/products/breadboard-friendly-2-1mm-dc-barrel-jack?variant=27740417489">The Pi Hut link</a>)
* Teensy 4.1 x 1 (<a href="https://thepihut.com/products/teensy-4-1-with-assembled-pins?variant=37627618066627">The Pi Hut link</a>)
* 5V 5A power supply x 1 (<a href="https://www.amazon.co.uk/dp/B09JS8N331?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1">Amazon UK link</a>)

The momentary switches are wired using a 'keyboard matrix' with the diodes to produce 3 columns of 10 switches.
The LEDs are wired in serial, 0-9 in the first column, 10-19 in the second column, 20-29 in the third column, with the data out from each LED connected to data in of the next.  +5V and 0V are connected on each column of LEDs, and are fed from the single power supply connected to the processing unit.
Each acrylic collar has a 5mm hole drilled into the side, 7mm from the bottom.  The LED is then fed through this hole to encourage as much light as possible into the acrylic, so that it acts as a light tube.
The wooden drawer knobs have had their screw threads removed, and the soft caps glued into them, so they can be easily affixed to the momentary switches.  Each knob will have the relevent stop name etched onto the surface using a pyrography tool.
(Circuit diagram to follow).

