#!/bin/bash
BASEDIR="$(cd -P "$(dirname "$0")" && pwd)"
runobjdump=true

## Arrays
drivers=(libkernel libio libLDGame libLDVideo libLDInput)
etc=(ld_intro.txt)

## Misc
cmdline="--debug"
grubpath="$BASEDIR/image/boot/grub/grub.cfg"

## Setup the image folder
rm -f -r "$BASEDIR/Firedrake.iso"
rm -f -r "$BASEDIR/image"

mkdir -p "$BASEDIR/image/boot/grub/i386-pc"
mkdir -p "$BASEDIR/image/modules"

## Create the grub.cfg
echo -e -n "GRUB_DEFAULT=0\nGRUB_TIMEOUT=1\n\nmenuentry \"Firedrake\" {\n\tmultiboot /boot/firedrake $cmdline\n\tmodule /boot/firedrake firedrake\n" > $grubpath

## Copy drivers
for i in ${drivers[@]}; do
	echo -e -n "\tmodule /modules/${i}.so ${i}.so\n" >> $grubpath
	cp "$BASEDIR/../libkernel/${i}/${i}.so" "$BASEDIR/image/modules/${i}.so"

	if [ $runobjdump = true ]; then
		objdump -d "$BASEDIR/../libkernel/${i}/${i}.so" > "$BASEDIR/../libkernel/${i}/dump.txt"
	fi
done

## Copy the /etc/ files
for i in ${etc[@]}; do
	echo -e -n "\tmodule /modules/${i} ${i}\n" >> $grubpath
	cp "$BASEDIR/../etc/${i}" "$BASEDIR/image/modules/${i}"
done

## Finish the grub.cfg and copy the kernel
echo -e -n "}" >> $grubpath
cp "$BASEDIR/../sys/firedrake" "$BASEDIR/image/boot/firedrake"

if [ $runobjdump = true ]; then
	objdump -d "$BASEDIR/../sys/firedrake" > "$BASEDIR/../sys/dump.txt"
fi

## Create the image
grub-mkrescue --modules="ext2 fshelp boot pc" --output="$BASEDIR/Firedrake.iso" "$BASEDIR/image"

## Remove the temporary image folder
rm -f -r "$BASEDIR/image"
