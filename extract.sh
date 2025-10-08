#!/bin/sh

curl -o ./MM-Lab-main.zip -SsfL https://github.com/mastermach50/MM-Lab/archive/refs/heads/main.zip
unzip -q ./MM-Lab-main.zip -d .
mv ./MM-Lab-main ./MM-Lab
rm ./MM-Lab-main.zip ./MM-Lab/.gitignore ./MM-Lab/.envrc ./MM-Lab/shell.nix ./MM-Lab/extract.sh