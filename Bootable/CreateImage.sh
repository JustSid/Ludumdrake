#!/bin/bash
BASEDIR="$(cd -P "$(dirname "$0")" && pwd)"

## Arrays
programs=(hellostatic)
libraries=()

## Misc
grubpath="$BASEDIR/image/boot/grub/grub.cfg"

## Setup the image folder
rm -f -r "$BASEDIR/Firedrake.iso"
rm -f -r "$BASEDIR/image"

mkdir -p "$BASEDIR/image/boot/grub/i386-pc"
mkdir -p "$BASEDIR/image/modules"

## Create the grub.cfg
echo -e -n "GRUB_DEFAULT=0\nGRUB_TIMEOUT=1\n\nmenuentry \"Firedrake\" {\n\tmultiboot /boot/firedrake\n\tmodule /boot/firedrake firedrake\n" > $grubpath

## Copy programs
for i in ${programs[@]}; do
	echo -e -n "\tmodule /modules/${i}.bin ${i}.bin\n" >> $grubpath
	cp "$BASEDIR/../Programs/${i}/${i}.bin" "$BASEDIR/image/modules/${i}.bin"
done

## Copy libraries
for i in ${libraries[@]}; do
	echo -e -n "\tmodule /modules/${i}.so ${i}.so\n" >> $grubpath
	cp "$BASEDIR/../Libraries/${i}/${i}.so" "$BASEDIR/image/modules/${i}.so"
done

## Finish the grub.cfg and copy the kernel
echo -e -n "}" >> $grubpath
cp "$BASEDIR/../Kernel/firedrake" "$BASEDIR/image/boot/firedrake"

## Create the image
grub-mkrescue --modules="ext2 fshelp boot pc" --output="$BASEDIR/Firedrake.iso" "$BASEDIR/image"

## Remove the temporary image folder
rm -f -r "$BASEDIR/image"