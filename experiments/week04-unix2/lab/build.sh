echo "==================================="
echo "$ make clean"
echo "==================================="
make clean

echo "==================================="
echo "$ make"
echo "==================================="
make

echo "==================================="
echo "$ ./main text.txt"
echo "==================================="
./main test.txt

echo "==================================="
echo "$ ./main text.txt > result.txt"
echo "==================================="
./main test.txt > result.txt

echo "==================================="
echo "$ cat result.txt"
echo "==================================="
cat result.txt

echo "==================================="
echo "          End of script"
echo "==================================="
