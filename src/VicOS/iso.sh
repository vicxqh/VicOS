#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/osv.kernel isodir/boot/osv.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "osv" {
	multiboot /boot/osv.kernel
}
EOF
grub-mkrescue -o osv.iso isodir
