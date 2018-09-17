#!/usr/bin/env bash
#Keith Skinner
#Lab 02

sh build.sh

./sub > ./output/no_params.txt
./sub -h > ./output/help_param.txt

./sub --abcdefghijklmnopqrstuvwxyz < ./input.txt >./output/only_from_param.txt
./sub --abcdefghijklmnopqrstuvwxyz -+zyxwvutsrqponmlkjihgfedcba < ./input.txt >./output/from_and_to_param.txt
./sub -+zyxwvutsrqponmlkjihgfedcba --abcdefghijklmnopqrstuvwxyz < ./input.txt >./output/backwards_from_and_to_param.txt
./sub -i ./input.txt > ./output/only_input_param.txt
./sub -o ./output/only_output_param.txt < ./input.txt
./sub -i ./input.txt -o ./output/input_and_output_param.txt
./sub -o ./output/backwards_input_and_output_param.txt -i ./input.txt

./sub --abcdefghijklmnopqrstuvwxyz -+zyxwvutsrqponmlkjihgfedcba -i ./input.txt -o ./output/encrypt.txt
./sub --abcdefghijklmnopqrstuvwxyz -+zyxwvutsrqponmlkjihgfedcba -i ./output/encrypt.txt -o ./output/decrypt.txt

diff ./input.txt ./output/decrypt.txt > /dev/null
exit