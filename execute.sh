make
python3 preprocess.py $1 $2
./exe $2 conv1.txt conv2.txt fc1.txt fc2.txt
rm $2
