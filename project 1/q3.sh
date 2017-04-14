#! bin/bash
#question 3 value disk

cd /

DUROOT=$(df / | tail -1 | awk '{print $3}')

SUM=0
for I in $(ls /)
do 

	TEMP=$(du -s $I 2> >(grep -v '^du: cannot \(access\|read\)' >&2) | tail -1 | grep -o "^[0-9]\+" )
	SUM=`expr $SUM + $TEMP`
	#echo $TEMP
	#echo $I

done


echo "Disk Space of root : " $DUROOT
echo "Disk Space of sum sub root :" $SUM
echo "Difrence : " `expr $DUROOT - $SUM`
