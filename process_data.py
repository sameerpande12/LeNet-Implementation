#used to change the format of data.txt [[[]]] to readable format
f = open("data.txt","r");
contents = f.read();
f.close()
contents = contents.replace('[','');
contents = contents.replace(']','');
f = open("pdata.txt","w");
f.write(contents);
f.close();

f = open("conv1.txt","r");
contents = f.read();
f.close();
contents = contents.replace('[','');
contents = contents.replace(']','');
f = open("pconv1.txt","w");
f.write(contents);
f.close();

f = open("conv2.txt","r");
contents = f.read();
f.close()
contents = contents.replace('[','');
contents = contents.replace(']','');
f = open("pconv2.txt","w");
f.write(contents);
f.close()
