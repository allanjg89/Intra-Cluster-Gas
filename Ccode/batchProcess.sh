#!/bin/bash


runSequence(){
	
	~/jaco-0.97/jaco -E cs-combo_Sim.cfg 
	~/jaco-0.97/jaco -Xtf10 -write 0 -simmode 0 cs-combo_Sim.cfg
	~/jaco-0.97/jaco -Xtm50000 -write 3  cs-combo_Sim.cfg
	
}


count=1;
max=20;
first="runc24a_00000.box"
last="runc24a_02000.box"
prev="runc24a_00000.box"
next="runc24a_00100.box"

#run first sim file
runSequence

#iterate throuh the folowing sim files
for((i=0;i<max;i+=1))
	do
		prevCount=$count
		count=$(($count+1))
		prev=$next
		if [ $count -lt 10 ]
		then next=${next/"00$prevCount"/"00$count"}
		elif [ $count == 10 ]
		then next="runc24a_01000.box"
		else next=${next/"0$prevCount"/"0$count"}
		fi
		#replace simfile
		sed -i.bak s/"$prev"/"$next"/g cs-combo_Sim.cfg
		runSequence
	done
	
	
	
	
#reset file to 0 
sed -i.bak s/"$last"/"$first"/g cs-combo_Sim.cfg


