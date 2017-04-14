
tar xvzf example.tar.gz -C $2

cd $2

# must be in super-user mode


chmod +x calculate



./calculate $1 &

PIDD=$(pgrep calculate)
#sleep 6
iotop -n5 -b  | grep ./calculate | grep -v grep | awk '{print $6 }' 


