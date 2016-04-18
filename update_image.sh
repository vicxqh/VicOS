#!/bin/bash

hdiutil attach -mountpoint boot_folder boot.img
cp $1 boot_folder/
hdiutil detach boot_folder
