#!/bin/bash

FILES=/home/dariog/caffe/examples/images/other/*
OUTPUT_TARGET=/home/dariog/caffe/features_transport_others/others

#This script loads all images found in the directory. Runs them through a pre-trained CNN and extracts the features of layers pool2, conv3 and conv4. Features are stored as 3 files, named as the image without extension, ending with _pool2, _conv3 and _conv4.

for f in $FILES
do
  filename="${f##*/}"
  filename="${filename%%.*}"
  echo "Processing $f file..."
  echo "Storing output in $OUTPUT_TARGET/$filename"
  mkdir "$OUTPUT_TARGET/$filename"
  echo "$f  0" > /home/dariog/caffe/examples/_temp/file_list.txt 
  ./build/tools/extract_features.bin models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel examples/_temp/imagenet_val.prototxt pool2,conv3,conv4 $OUTPUT_TARGET/$filename/pool2,$OUTPUT_TARGET/$filename/conv3,$OUTPUT_TARGET/$filename/conv4 1 lmdb lmdb lmdb
  mv $OUTPUT_TARGET/$filename/pool2/output_0_0 $OUTPUT_TARGET/$filename/${filename}_pool2
  mv $OUTPUT_TARGET/$filename/conv3/output_0_1 $OUTPUT_TARGET/$filename/${filename}_conv3
  mv $OUTPUT_TARGET/$filename/conv4/output_0_2 $OUTPUT_TARGET/$filename/${filename}_conv4
  rm -fr $OUTPUT_TARGET/$filename/pool2
  rm -fr $OUTPUT_TARGET/$filename/conv3
  rm -fr $OUTPUT_TARGET/$filename/conv4
done
