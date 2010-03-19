mkdir temp
mv $1 temp
cd temp
tar -xvvzf $1
cd $2
mkdir ../../UPM_root/usr/local/pkg/$2
./configure --prefix="$PWD"/../../UPM_root/usr/local/pkg/$2
make
cd ../..
#rm -r temp
