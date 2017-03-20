#!/bin/sh

# changedi by Oliver Cordes: 2016-10-24

INFILE=$HOME/git/arctic/demo/06u15042j_drkA.fits
OUTFILE=$HOME/git/arctic/demo/testA.fits

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

arctic -d 10 --out_float -c default_express5_niter6.cte ${INFILE} ${OUTFILE} -m ACS $args
