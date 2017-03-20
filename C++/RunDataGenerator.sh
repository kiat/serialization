# Clean previous data:
make clean;
sleep 5;

# Compile Data Generator:
make datagen;
sleep 5;

# Generate data:
# HandCoded:
echo "HandCoded: ";
./a.out 0;
sleep 5;

#Proto
echo "Proto: ";
./a.out 1;
sleep 5;

#InPlace
echo "InPlace: ";
./a.out 2;
sleep 5;

#BOOST:
echo "BOOST: ";
./a.out 3;
sleep 5;
