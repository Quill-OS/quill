# inkbox

## What is it?
InkBox is an open-source, Qt-based eBook reader for Kobo devices. It also comes as an entire [native open-source OS](https://github.com/Kobo-InkBox/rootfs) support for a number of devices.
<br>
It features:
- ePUB and plain text support
- Versatile configuration options for reading
- muPDF rendering engine for ePUB (and soon PDFs)
- Wi-Fi support and web browser
- X11 apps seamless integration (KoBox)
- Fast dictionary search
- Dark mode
- Strict security policy ensuring that only signed software can be run on the device (this, however, can be adapted to your needs by recompiling the [kernel](https://github.com/Kobo-InkBox/kernel)
- Full factory reset option if needed
- Seamless update process
- VNC viewer app
- Search function
- 9 built-in fonts
- Auto-suspend
- Lock screen/passcode
- User-friendly experience
## How do I install it?
You can download precompiled OS/binaries [here](http://pkgs.kobox.fermino.me/bundles/inkbox/native/) and standard Nickel add-ons are available [there](http://pkgs.kobox.fermino.me/bundles/inkbox/nickel/), although support for them has been stopped until further notice. Huge thanks to [@fermino](https://github.com/fermino) for providing free hosting.
<br>
Flashing InkBox OS to your Kobo's SD card is as simple as doing, on most *nix machines, assuming that `inkbox.xz` is the file you downloaded and that `/dev/mmcblk0` is your SD card's device node:
```
xzcat inkbox.xz | dd of=/dev/mmcblk0
```
If you install the Nickel add-on, unpack the 'base' archive in the root of the onboard storage, not in any subfolder inside it.
## How can I contribute?
If you like this project and want to improve it in some way, feel free to fork this repository or [one of the subprojects this organization hosts](https://github.com/Kobo-InkBox), then make a [pull request](https://github.com/Kobo-InkBox/inkbox/pulls). I'll be happy to review it.
<br><br>
On the other hand, if you don't have the appropriate coding skills or just want to help in some way, feel free to [make a donation here](https://liberapay.com/tux-linux/). I'm a student and motivation has been the only thing that has helped me maintain this project for the last months. Developing an entire operating system on my own was not an easy task. I have time, but I like to spend it wisely.
