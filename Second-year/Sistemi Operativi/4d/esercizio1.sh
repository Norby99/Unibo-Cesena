for (( CONT=0; $CONT<10; CONT=$CONT+1 )) ; do
	mkdir "1.${CONT}"
done

for (( CONT=0; $CONT<10; CONT=$CONT+1 )) ; do
	mv "1.${CONT}" "2.${CONT}"
done
