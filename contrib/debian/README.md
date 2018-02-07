
Debian
====================
This directory contains files used to package adnetcoind/adnetcoin-qt
for Debian-based Linux systems. If you compile adnetcoind/adnetcoin-qt yourself, there are some useful files here.

## adnetcoin: URI support ##


adnetcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install adnetcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your adnetcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/adnetcoin128.png` to `/usr/share/pixmaps`

adnetcoin-qt.protocol (KDE)

