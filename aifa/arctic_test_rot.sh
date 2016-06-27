#!/bin/sh

# changedi by Oliver Cordes: 2016-06-15

INFILE=$HOME/git/arctic/demo/jbke01rkq_drkA.fits
OUTFILE=$HOME/git/arctic/demo/testA_rot.fits

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

TEMPFILE1="tempfile1.fits"
TEMPFILE2="tempfile2.fits"

../test_suite/bin/./fliprotate.py -i ${INFILE} -o ${TEMPFILE1} --rotate

arctic -d 10 --out_float -c test_opt.cte ${TEMPFILE1} ${TEMPFILE2} -m ACS -rotate $args

../test_suite/bin/./fliprotate.py -i ${TEMPFILE2} -o ${OUTFILE} --arotate

rm -f ${TEMPFILE1} ${TEMPFILE2}
