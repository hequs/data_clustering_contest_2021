#!/bin/bash

urls=(
    "https://data-static.usercontent.dev/dc0130-input.tar.gz"
    "https://data-static.usercontent.dev/dc0202-input.tar.gz"
    "https://data-static.usercontent.dev/dc0206-input.tar.gz"
    "https://data-static.usercontent.dev/dc0212-input.tar.gz"
)

mkdir -p data

for url in ${urls[@]}; do
    echo $url
    wget -qO - $url | tar -xz -C data
done
