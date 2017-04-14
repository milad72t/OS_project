#! bin/bash
#question 2 extract file


tar xvzf example.tar.gz -C $2

cd $2

# must be in super-user mode


chmod +x calculate


/usr/bin/time -f "Time %e sec" ./calculate milad & 
PIDD=$(pgrep calculate)





sleep 8
MAIN=$(top -n5 -p $PIDD| head -8 | grep calculate )

#TIME=$(echo $MAIN| grep  -o ".....calculate" | grep -o  "^.....") 

CPU=$(echo $MAIN | awk '{print $10}' )

MEM=$(echo $MAIN| awk '{print $11}' )


echo CPU $CPU %

echo -ne "memory (MB)  "

#echo $TIME

echo  "$MEM * 5865 /100" | bc -l | grep -o "[0-9]\+\...."

sleep 10


./calculate milad & 

PIDD=$(pgrep calculate)

TEMP=$(iotop -n10 -b | grep ./calculate | grep -v grep | awk '{print $6 }' )


cd /home/milad/Public/project/project_OS
TEMP2=$(python q2.py $TEMP)

echo I/O $TEMP2 MB





					


