#!/bin/bash

./crypto createblockchain -address Ivan
./crypto getbalance -address Ivan

./crypto send -from Ivan -to Pedro -amount 6
./crypto getbalance -address Ivan
./crypto getbalance -address Pedro
./crypto send -from Pedro -to Helen -amount 2
./crypto send -from Ivan -to Helen -amount 2

./crypto send -from Helen -to Rachel -amount 3
./crypto getbalance -address Ivan
./crypto getbalance -address Pedro
./crypto getbalance -address Helen
./crypto getbalance -address Rachel

./crypto send -from Pedro -to Ivan -amount 5
./crypto getbalance -address Pedro
./crypto getbalance -address Ivan