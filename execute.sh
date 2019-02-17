make
python3 preprocess.py $1 $2
./exe $2 Convolution_Layers/conv1.txt Convolution_Layers/conv2.txt Convolution_Layers/fc1.txt Convolution_Layers/fc2.txt
rm $2
