rm -r temp
mkdir temp
cp $1 temp
cd temp
mkdir /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
rpm2cpio $1 > temp.cpio
mv temp.cpio /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
cd /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
cpio -d -i <temp.cpio
rm temp.cpio
cd -
cd ../
#rm -r temp
