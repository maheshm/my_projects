rm -r temp
mkdir temp
cp $1 temp/
cd temp
ar -x $1
mkdir /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
mv data.tar.gz /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
cd /home/atwork/project_UPM/final_code/UPM_root/usr/local/pkg/$2
tar -xvvzf data.tar.gz
rm data.tar.gz
cd -
#tar -xvvzf control.tar.gz

#cat control|grep Package|cut -d ':' -f 2 > information
#cat control|grep Depends|cut -d ':' -f 2 >> information
#echo $filename
