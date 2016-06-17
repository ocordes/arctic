#!/bin/sh

# changedi by Oliver Cordes: 2016-06-15

INFILE=$HOME/git/arctic/test_suite/data/cross_1024x1024.fits
OUTFILE=$HOME/git/arctic/test_suite/data/cross_clock.fits

export PATH=$HOME/git/arctic/build:$HOME/git/arctic:$PATH

args=""

while [ -n "$1" ]; do
  echo $1
  case $1 in
   -i) shift; INFILE=$1;shift;;
   -o) shift; OUTFILE=$1;shift;;
   *)  args="$args $1";shift;;
  esac
done

arctic -d 10 -c clock_test.cte --clock ${INFILE} ${OUTFILE}  $args
