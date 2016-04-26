#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/osv.kernel isodir/boot/osv.kernel
cat > isodir/boot/grub/grub.cfg << EOF
timeout=1

menuentry "osv(Vic's toy OS)" {
	multiboot /boot/osv.kernel
}
EOF
grub-mkrescue -o osv.iso isodir
