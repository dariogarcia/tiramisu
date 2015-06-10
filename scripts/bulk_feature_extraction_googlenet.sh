#!/bin/bash

#This script loads all images found in the directory specified in the first parameter. Runs them through a pre-trained CNN and extracts the features of certain pre-defined layers. Features are stored as 3 files, named as the image without extension, ending with the names of the layers, in the same directory.
#WARNING: First parameter needs to be full path. Ending with slash!
#WARNING: underscores in image names and layers name are problematic. beware


if [ "$#" -ne 1 ]; then
    echo "ERROR: Illegal number of parameters. First parameter should be location of images to process. A directory with full path, that will also be used to store the results."
    exit
fi

ALL="*"
FILES="$1$ALL"
OUTPUT_TARGET=$1

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
  echo "$f  0" > /home/dariog/repos/tiramisu/tmp/file_list.txt 
  ( cd /home/dariog/repos/caffe ; 
    /home/dariog/repos/caffe/build/tools/extract_features.bin /home/dariog/repos/caffe/models/bvlc_googlenet/bvlc_googlenet.caffemodel /home/dariog/repos/caffe/models/bvlc_googlenet/feat_extract.prototxt inception_3a/1x1,inception_3a/3x3,inception_3a/5x5,inception_3b/1x1,inception_3b/3x3,inception_3b/5x5,inception_4a/1x1,inception_4a/3x3,inception_4a/5x5,inception_4b/1x1,inception_4b/3x3,inception_4b/5x5,inception_4c/1x1,inception_4c/3x3,inception_4c/5x5,inception_4d/1x1,inception_4d/3x3,inception_4d/5x5,inception_4e/1x1,inception_4e/3x3,inception_4e/5x5,inception_5a/1x1,inception_5a/3x3,inception_5a/5x5,inception_5b/1x1,inception_5b/3x3,inception_5b/5x5 $OUTPUT_TARGET/$filename/inception-3a-1x1,$OUTPUT_TARGET/$filename/inception-3a-3x3,$OUTPUT_TARGET/$filename/inception-3a-5x5,$OUTPUT_TARGET/$filename/inception-3b-1x1,$OUTPUT_TARGET/$filename/inception-3b-3x3,$OUTPUT_TARGET/$filename/inception-3b-5x5,$OUTPUT_TARGET/$filename/inception-4a-1x1,$OUTPUT_TARGET/$filename/inception-4a-3x3,$OUTPUT_TARGET/$filename/inception-4a-5x5,$OUTPUT_TARGET/$filename/inception-4b-1x1,$OUTPUT_TARGET/$filename/inception-4b-3x3,$OUTPUT_TARGET/$filename/inception-4b-5x5,$OUTPUT_TARGET/$filename/inception-4c-1x1,$OUTPUT_TARGET/$filename/inception-4c-3x3,$OUTPUT_TARGET/$filename/inception-4c-5x5,$OUTPUT_TARGET/$filename/inception-4d-1x1,$OUTPUT_TARGET/$filename/inception-4d-3x3,$OUTPUT_TARGET/$filename/inception-4d-5x5,$OUTPUT_TARGET/$filename/inception-4e-1x1,$OUTPUT_TARGET/$filename/inception-4e-3x3,$OUTPUT_TARGET/$filename/inception-4e-5x5,$OUTPUT_TARGET/$filename/inception-5a-1x1,$OUTPUT_TARGET/$filename/inception-5a-3x3,$OUTPUT_TARGET/$filename/inception-5a-5x5,$OUTPUT_TARGET/$filename/inception-5b-1x1,$OUTPUT_TARGET/$filename/inception-5b-3x3,$OUTPUT_TARGET/$filename/inception-5b-5x5 1 lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb lmdb 
  )
  mv $OUTPUT_TARGET$filename/inception-3a-1x1/output_0_0 $OUTPUT_TARGET$filename/${filename}_inception-3a-1x1
  mv $OUTPUT_TARGET$filename/inception-3a-3x3/output_0_1 $OUTPUT_TARGET$filename/${filename}_inception-3a-3x3
  mv $OUTPUT_TARGET$filename/inception-3a-5x5/output_0_2 $OUTPUT_TARGET$filename/${filename}_inception-3a-5x5
  mv $OUTPUT_TARGET$filename/inception-3b-1x1/output_0_3 $OUTPUT_TARGET$filename/${filename}_inception-3b-1x1
  mv $OUTPUT_TARGET$filename/inception-3b-3x3/output_0_4 $OUTPUT_TARGET$filename/${filename}_inception-3b-3x3
  mv $OUTPUT_TARGET$filename/inception-3b-5x5/output_0_5 $OUTPUT_TARGET$filename/${filename}_inception-3b-5x5
  mv $OUTPUT_TARGET$filename/inception-4a-1x1/output_0_6 $OUTPUT_TARGET$filename/${filename}_inception-4a-1x1
  mv $OUTPUT_TARGET$filename/inception-4a-3x3/output_0_7 $OUTPUT_TARGET$filename/${filename}_inception-4a-3x3
  mv $OUTPUT_TARGET$filename/inception-4a-5x5/output_0_8 $OUTPUT_TARGET$filename/${filename}_inception-4a-5x5
  mv $OUTPUT_TARGET$filename/inception-4b-1x1/output_0_9 $OUTPUT_TARGET$filename/${filename}_inception-4b-1x1
  mv $OUTPUT_TARGET$filename/inception-4b-3x3/output_0_10 $OUTPUT_TARGET$filename/${filename}_inception-4b-3x3
  mv $OUTPUT_TARGET$filename/inception-4b-5x5/output_0_11 $OUTPUT_TARGET$filename/${filename}_inception-4b-5x5
  mv $OUTPUT_TARGET$filename/inception-4c-1x1/output_0_12 $OUTPUT_TARGET$filename/${filename}_inception-4c-1x1
  mv $OUTPUT_TARGET$filename/inception-4c-3x3/output_0_13 $OUTPUT_TARGET$filename/${filename}_inception-4c-3x3
  mv $OUTPUT_TARGET$filename/inception-4c-5x5/output_0_14 $OUTPUT_TARGET$filename/${filename}_inception-4c-5x5
  mv $OUTPUT_TARGET$filename/inception-4d-1x1/output_0_15 $OUTPUT_TARGET$filename/${filename}_inception-4d-1x1
  mv $OUTPUT_TARGET$filename/inception-4d-3x3/output_0_16 $OUTPUT_TARGET$filename/${filename}_inception-4d-3x3
  mv $OUTPUT_TARGET$filename/inception-4d-5x5/output_0_17 $OUTPUT_TARGET$filename/${filename}_inception-4d-5x5
  mv $OUTPUT_TARGET$filename/inception-4e-1x1/output_0_18 $OUTPUT_TARGET$filename/${filename}_inception-4e-1x1
  mv $OUTPUT_TARGET$filename/inception-4e-3x3/output_0_19 $OUTPUT_TARGET$filename/${filename}_inception-4e-3x3
  mv $OUTPUT_TARGET$filename/inception-4e-5x5/output_0_20 $OUTPUT_TARGET$filename/${filename}_inception-4e-5x5
  mv $OUTPUT_TARGET$filename/inception-5a-1x1/output_0_21 $OUTPUT_TARGET$filename/${filename}_inception-5a-1x1
  mv $OUTPUT_TARGET$filename/inception-5a-3x3/output_0_22 $OUTPUT_TARGET$filename/${filename}_inception-5a-3x3
  mv $OUTPUT_TARGET$filename/inception-5a-5x5/output_0_23 $OUTPUT_TARGET$filename/${filename}_inception-5a-5x5
  mv $OUTPUT_TARGET$filename/inception-5b-1x1/output_0_24 $OUTPUT_TARGET$filename/${filename}_inception-5b-1x1
  mv $OUTPUT_TARGET$filename/inception-5b-3x3/output_0_25 $OUTPUT_TARGET$filename/${filename}_inception-5b-3x3
  mv $OUTPUT_TARGET$filename/inception-5b-5x5/output_0_26 $OUTPUT_TARGET$filename/${filename}_inception-5b-5x5
  mv ${f}_out $OUTPUT_TARGET$filename/
  
  rm -fr $OUTPUT_TARGET$filename/inception-3a-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-3a-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-3a-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-3b-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-3b-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-3b-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-4a-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-4a-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-4a-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-4b-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-4b-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-4b-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-4c-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-4c-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-4c-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-4d-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-4d-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-4d-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-4e-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-4e-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-4e-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-5a-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-5a-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-5a-5x5/
  rm -fr $OUTPUT_TARGET$filename/inception-5b-1x1/
  rm -fr $OUTPUT_TARGET$filename/inception-5b-3x3/
  rm -fr $OUTPUT_TARGET$filename/inception-5b-5x5/
  rm -fr /home/dariog/repos/tiramisu/tmp/file_list.txt
done


