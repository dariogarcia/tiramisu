#!/bin/bash

#This script loads all images found in the directory specified in the first parameter. Runs them through a pre-trained CNN and extracts the features of layers pool2, conv3 and conv4. Features are stored as 3 files, named as the image without extension, ending with _pool2, _conv3 and _conv4.
#First parameter needs to be full path


if [ "$#" -ne 1 ]; then
    echo "ERROR: Illegal number of parameters. First parameter should be location of images to process. A directory with full path, that will also be used to store the results."
    exit
fi

ALL="*"
FILES="$1$ALL"
OUTPUT_TARGET=$1
#FILES=/home/dariog/caffe/examples/images/other/*
#OUTPUT_TARGET=/home/dariog/caffe/features_transport_others/others


for f in $FILES
do
  filename="${f##*/}"
  extension="${filename##*.}"
  if [ "$extension" -ne ".jpg" ]; then
    continue
  fi
  filename="${filename%%.*}"
  echo "Processing $f file..."
  echo "Storing output in $OUTPUT_TARGET$filename"
  mkdir "$OUTPUT_TARGET$filename"
  echo "$f  0" > /home/dariog/repos/tiramisu/file_list.txt 
  ( cd /home/dariog/repos/caffe ; /home/dariog/repos/caffe/build/tools/extract_features.bin models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel /home/dariog/repos/caffe/examples/feature_extraction/imagenet_val.prototxt pool2,conv3,conv4 $OUTPUT_TARGET/$filename/pool2,$OUTPUT_TARGET/$filename/conv3,$OUTPUT_TARGET/$filename/conv4 1 lmdb lmdb lmdb )
  mv $OUTPUT_TARGET$filename/pool2/output_0_0 $OUTPUT_TARGET$filename/${filename}_pool2
  mv $OUTPUT_TARGET$filename/conv3/output_0_1 $OUTPUT_TARGET$filename/${filename}_conv3
  mv $OUTPUT_TARGET$filename/conv4/output_0_2 $OUTPUT_TARGET$filename/${filename}_conv4
  mv ${f}_out $OUTPUT_TARGET$filename/
  rm -fr $OUTPUT_TARGET$filename/pool2
  rm -fr $OUTPUT_TARGET$filename/conv3
  rm -fr $OUTPUT_TARGET/$filename/conv4
  rm -fr /home/dariog/repos/tiramisu/file_list.txt
done
