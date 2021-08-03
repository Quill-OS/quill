# inkbox

## What is it?
InkBox is an open-source, Qt-based eBook reader for Kobo devices. It currently features, namely, ePUB/plain text support, X11 apps (KoBox) integration and [native open-source OS](https://github.com/Kobo-InkBox/rootfs) support for a number of devices.
<br>
You can download precompiled OS/binaries [here](http://pkgs.kobox.fermino.me/bundles/inkbox/native/) and standard Nickel add-ons are available [there](http://pkgs.kobox.fermino.me/bundles/inkbox/nickel/), although support for them has been stopped until further notice.
<br>
Flashing InkBox OS to your Kobo's SD card is as simple as doing, on most *nix machines, assuming that `inkbox.xz` is the file you downloaded and that `/dev/mmcblk0` is your SD card's device node:
```
xzcat inkbox.xz | dd of=/dev/mmcblk0
```
## How can I contribute?
If you like this project and want to improve it in some way, feel free to fork this repository or [one of the subproject this organization hosts](https://github.com/Kobo-InkBox), then make a [pull request](https://github.com/Kobo-InkBox/inkbox/pulls). I'll be happy to review it.
<br>
On the other hand, if you don't have the appropriate coding skills or just want to help in some way, feel free to [make a donation here](https://liberapay.com/tux-linux/). I'm a student and motivation has been the only thing that has helped me maintain this project for the last months. Developing an entire operating system on my own was not an easy task. I have time, but I like to spend it wisely.
