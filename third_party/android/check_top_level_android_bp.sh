#!/bin/sh

SCRIPT_PATH=$(dirname $(readlink -e $0))

$SCRIPT_PATH/generate_android_bp.py --output Android.bp.autogen

if diff -u Android.bp.autogen $SCRIPT_PATH/../../Android.bp ; then
  exit 0
else
  echo \
    "ERROR: The top level Android.bp does not appear to have been" \
    "automatically generated. Changes to Android.bp should be done" \
    "through the scripts in $SCRIPT_PATH."
  exit 1
fi
