#!/bin/sh

# changedi by Oliver Cordes: 2016-06-15

INFILE=$HOME/git/arctic/demo/testdark_drkA.fits
OUTFILE=$HOME/git/arctic/demo/testA.fits
OUTFILE2=$HOME/git/arctic/demo/testA_pre.fits
OUTFILE_sub=$HOME/git/arctic/demo/testA_sub.fits
OUTFILE_div=$HOME/git/arctic/demo/testA_div.fits


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

arctic -d 10 -c test_opt.cte ${INFILE} ${OUTFILE} -m ACS $args

ic "%1 %2 /"  $OUTFILE $OUTFILE2 > $OUTFILE_div
ic "%1 %2 -"  $OUTFILE $OUTFILE2 > $OUTFILE_sub

imstats $OUTFILE_div
imstats $OUTFILE_sub
