# How To Extend The Keyboard Compass

## GO ODF
The following steps are performed using GO ODF from https://github.com/GrandOrgue/GoOdf/.
You should be able to download pre-compiled versions of the tool from the [Releases](https://github.com/GrandOrgue/GoOdf/releases) folder.

## Manuals
* For each manual change the ‘logical keys’ and ‘accessible keys’ values to 61

## Stops
* Change stop ‘number of logical pipes’ to 61
* 'Flexible pipe loading', starting at pipe 57, matching (MIDI) number 80, total number of pipes to load 5, ‘Load pipes’
* Expand the pipe tree and confirm all the new pipes have been successfully assigned pipe samples (080-g#.wav through to 084-c.wav) - if not, subtract 12 from the current pipe number and expand that pipe number, checking which samples are used, and what tuning offset is applied.
* Select pipe 56, ‘Edit pipe properties’, copy parameters to the next 5 pipes (will set Gain to match existing pipes)
* ‘Next pipe’ to select pipe 57, set pitch correction to 1200 cents (one octave), copy parameters to next 4 pipes
* If you had to manually fix any pipe samples after the flexible pipe loading was performed, edit that pipe property, and add the same tuning offset to the 1200 (octave) value.
	* e.g. if Pipe058 had to be reassigned samples for 082-a#.wav, and the Pitch Tuning for Pipe046 was -100, then the new Pitch Tuning for Pipe058 is now 1200-100=1100
* Close the pipe properties
* Find the stop reference on the relevant manual, remove it, and re-add it
* Save & test 
* Rinse & repeat for all stops

## UI Elements
Clean-up the UI once all the testing is complete, by removing the extra (newly added) keys on the manuals:
* Expand the Panels / Main Panel / GUI Elements and find the element for each displayed manual
* Set the ‘Display Keys’ value back to 56 (or whatever it was originally) to hide the extended keys in the UI and avoid making things look funky.
