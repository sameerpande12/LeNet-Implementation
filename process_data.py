#used to change the format of data.txt [[[]]] to readable format
f = open("data.txt","r");
contents = f.read();
f.close()
contents = contents.replace('[','');
contents = contents.replace(']','');
f = open("processed_data.txt","w");
f.write(contents);
f.close()
