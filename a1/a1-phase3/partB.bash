#Xingeng Wang, xiw031, 11144515
#Yuchen Lin  , yul761, 11138672
#!/bin/bash


ARCH=$(uname -m)
UNAME_S=$(uname -s)

if [ $1 = "A1"  ]; then
  if [ $UNAME_S = "Linux" ]; then
    echo "can not run A1 on Linux";
    exit 1;
  fi
  while read -r line; do
		ARRAY_INPUT={$line}
    if echo "$line" | egrep -q '^\-?[0-9]*\.?[0-9]+$'; then 
      if [ $line -lt 0 ]; then
	echo "$line is a negetive number";
	exit 1;
      fi
    else 
      echo "$line is not a number"
      exit 1;
    fi
  done
  
	
	THREAD={$ARRAY_INPUT[0]}
	DEADLINE={$ARRAY_INPUT[1]}
	SIZE={$ARRAY_INPUT[2]}
  
  ./partA1.exe $ARRAY_INPUT
  
  exit 0;
elif [ $1 = "A2"  ]; then
  while read -r line; do
    if echo "$line" | egrep -q '^\-?[0-9]*\.?[0-9]+$'; then 
      if [ $line -lt 0 ]; then
	echo "$line is a negetive number";
	exit 1;
      fi
    else 
      echo "$line is not a number"
      exit 1;
    fi
  done

	ARRAY_INPUT={$line}
	THREAD={$ARRAY_INPUT[0]}
	DEADLINE={$ARRAY_INPUT[1]}
	SIZE={$ARRAY_INPUT[2]}

  ./partA2 $THREAD $DEADLINE $SIZE
  exit 0;
elif [ $1 = "A3"  ]; then
  while read -r line; do
    if echo "$line" | egrep -q '^\-?[0-9]*\.?[0-9]+$'; then 
      if [ $line -lt 0 ]; then
	echo "$line is a negetive number";
	exit 1;
      fi
    else 
      echo "$line is not a number"
      exit 1;
    fi
  done

	ARRAY_INPUT={$line}
	THREAD={$ARRAY_INPUT[0]}
	DEADLINE={$ARRAY_INPUT[1]}
	SIZE={$ARRAY_INPUT[2]}
	
 
  ./partA3 $THREAD $DEADLINE $SIZE
  exit 0;
elif [ $1 = "A4"  ]; then
  while read -r line; do
    if echo "$line" | egrep -q '^\-?[0-9]*\.?[0-9]+$'; then 
      if [ $line -lt 0 ]; then
	echo "$line is a negetive number";
	exit 1;
      fi
    else 
      echo "$line is not a number"
      exit 1;
    fi
  done

	ARRAY_INPUT={$line}
	THREAD={$ARRAY_INPUT[0]}
	DEADLINE={$ARRAY_INPUT[1]}
	SIZE={$ARRAY_INPUT[2]}

   
  ./partA4 $THREAD $DEADLINE $SIZE
  exit 0;
   
else
  echo "Invalid input. ";
  exit 1;
fi

