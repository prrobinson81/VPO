A repository of files used to design and document the creation of a Virtual Pipe Organ.

The project is built around the <a href="https://github.com/GrandOrgue/grandorgue">Grand Orgue</a> project.

* The console is built out of 2 x M-audio Keystation 61es MIDI keyboards
* The pistons are made from black thumbtacks, wired up to a <a href="https://www.pjrc.com/store/teensy41.html">Teensy 4.1</a> microcontroller.
* The stop jambs are 2 UPerfect <a href="https://uperfect.com/products/1920x1080-monitor">UTouch E6 Pro</a> monitors.
  * Connected USB-C, providing both the data channel and power to the screens.
  * The VPO host is a Mac Mini (M4, 16GB RAM) so touch support is provided by the <a href="https://v1license.touch-base.com/download">Touch-Base UPDD Driver</a>.
* The pedalboard is a full 30-note concave radiating standards-compliant pedalboard, which has been MIDI-fied by me using another Teensy 4.1 with reed switches providing the inputs. The magnets to actuate the reed switches are mounted on screws under the end of each pedal, meaning the pedalboard is 'all inclusive' (i.e. there's no separate back plate which contains the reed switches).
* The VPO used is 'St Matthew, Cheltenham' from <a href="https://barrittaudio.co.uk/products/st-matthew-cheltenham-sample-set">Barritt Audio</a>. 
  * NB - This is a Hauptwerk organ, but there's a GrandOrgue ODF available courtesy of 'ahall41' on <a href="https://mps-orgelseite.de/home/filebase/index.php?file/214-go-odf-for-father-willis-organ-st-matthew-cheltenham/#versions">MPS Orgel Seite</a>.
  * I've used the 1.4 release with some <a href="./Custom ODFs/Cheltenham full.1.4.organ">custom changes</a>:
    * extended the keyboard compass from 57 to 61 notes
    * replicated the Simple Jamb into Simple-Left and Simple-Right jambs for use with portrait-orientated (touch) screens.
    * added a Choir-to-Swell coupler to make it easier to play the Choir on my 2-manual console.

It's a work in progress, and things will likely change as the project develops over time.  For instance I'd love to add a 3<sup>rd</sup> manual to the console.
