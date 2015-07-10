#!/bin/bash

size_4e3x3=62720
size_3b5x5=75264
size_5b1x1=18816
size_4a3x3=40768
size_5b3x3=18816
size_3b3x3=150528
size_4d3x3=56448
size_4e1x1=50176
size_5b5x5=6272 
size_4c1x1=25088
size_4e5x5=25088
size_4a1x1=37632
size_5a1x1=12544
size_3a1x1=50176
size_3b1x1=100352
size_3a5x5=25088
size_5a3x3=15680
size_4a5x5=9408 
size_5a5x5=6272 
size_4d5x5=12544
size_4b5x5=12544
size_3a3x3=100352
size_4c3x3=50176
size_4b1x1=31360
size_4d1x1=21952
size_4b3x3=43904
size_4c5x5=12544

for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4e-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4e3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3b-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3b5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5b-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5b1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4a-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4a3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5b-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5b3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3b-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3b3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4d-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4d3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4e-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4e1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5b-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5b5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4c-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4c1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4e-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4e5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4a-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4a1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5a-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5a1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3a-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3a1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3b-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3b1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3a-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3a5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5a-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5a3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4a-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4a5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-5a-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_5a5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4d-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4d5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4b-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4b5x5 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-3a-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_3a3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4c-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4c3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4b-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4b1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4d-1x1*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4d1x1 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4b-3x3*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4b3x3 ]
  then
    echo $f;
  fi
done
for f in $(find /home/dariog/ILSVRC2012_val_2 -name '*inception-4c-5x5*' ); 
do 
  var1=$(wc -l $f | cut -d ' ' -f 1)
  if [ $var1 != $size_4c5x5 ]
  then
    echo $f;
  fi
done
