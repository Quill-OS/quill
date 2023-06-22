# Legal information
This is the official document concerning legal information about InkBox OS. It has precedence over any other information available on the Internet.

## Definitions
- 'InkBox OS' represents the system image downloadable from the Internet that an end-user can flash and use on their electronic device. It includes the GUI, internal scripts and programs, bootloader and kernel. Such images can be accessed here: http://23.163.0.39/bundles/inkbox/native/
- 'Update packages' represent packages downloadable from the Internet that can be used to update InkBox OS to a newer version. Such packages can be accessed here: http://23.163.0.39/bundles/inkbox/native/update/
- 'The Software' represents both of the things above.

## Statement
The Software is provided to you WITHOUT ANY WARRANTY, and no one but you is responsible if you ever brick your device by installing it.

## Warnings
### Waveform
eInk devices need special data to drive their screen properly. This data is often referenced to as "waveforms", and is usually specific to each device. When distributing images, we cannot guarantee you that the wveform used for our testing device, even if said device *is* of the same model as the one you downloaded it for, will work for yours.<br>Since every eInk display is unique in its conception, every waveform is different, even for the same display model. If inappropriate data is fed to the eInk display's controller, such as wrong voltages for changing the state of a pixel, it can cause permanent damage to the screen. This has been seen happening on Kindle devices with their native firmware, but has yet to be demonstrated on Kobo devices.<br>In summary, you shouldn't be worried of flashing a release image of InkBox OS to your device, since the eInk controller should be tolerant enough to work fine with the input data it receives. If you are using a Kindle Touch, we use an in-kernel "default" waveform provided by Amazon until you provide your own.<br>But exceptions *do* happen, and for them we can't be held responsible for damaging your device.
