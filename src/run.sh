#!/bin/bash
shopt -s nullglob
files=('dataset'/*.csv)

if [ $1 = "mergesort" ]; then
  methods=('mergesort-branched-inplace' 'mergesort-predicated-inplace') 
elif [ $1 = "bubblesort" ]; then
  methods=('bubblesort-branched-inplace' 'bubblesort-predicated-inplace') 
else
  >&2 echo "incorrect arguement. use 'mergesort' or 'bubblesort'"
  exit 1
fi

echo "${methods[@]}"
powers=(10 12 14 16 17 18 19 20 21 22)

make all
# Header for csv file
echo -ne 'Type,File,Size,'
for i in {1..5}
do
  echo -ne "Run$i,"
done 
echo ''


for method in "${methods[@]}"
do 
  for file in  "${files[@]}"
  do
    for size in "${powers[@]}" 
    do
      type=$(sed 's/.*-\(.*\)-.*/\1/' <<< "$method")

      echo -ne "$type,$(basename $file),$((2**$size)),"
      for i in {1..5}
      do
        ./"$method" $((2**$size)) "$file"
      done
      echo ''
    done
  done
done

make clean
