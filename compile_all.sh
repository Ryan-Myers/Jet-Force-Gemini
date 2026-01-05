#!/bin/bash

# do us last as that will make it easier to keep decompiling with the default version
versions=('kiosk' 'us')

for i in "${!versions[@]}"; do
    version=${versions[$i]}
    echo "Compiling $version"
    make VERSION="$version" cleanextract && make VERSION="$version" -j12
    if [[ $(sha1sum -c --quiet ver/verification/jfg."$version".sha1) = "" ]]; then echo "$version OK"; else echo "Failed $version"; exit 1; fi
done
