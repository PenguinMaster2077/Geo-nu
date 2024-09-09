#!/bin/bash

START_INDEX=0
END_INDEX=3

#REACTOR_START=10
#REACTOR_END=60

rm -f ./Codes/*

for ((index=START_INDEX;index<=END_INDEX;index++))
do
	FORMATTED_INDEX=$(printf "%04d" $index)
        FILENAME="Grid_Scan_${FORMATTED_INDEX}.C"
        FILE="./Codes/${FILENAME}"
        touch $FILE
        cat "./Part1.txt" >> $FILE
        echo "int Grid_Scan_${FORMATTED_INDEX}()" >> $FILE
        cat "./Part2.txt" >> $FILE
        #echo "	Geo_Range.push_back(${index});" >> $FILE
        #echo "	Geo_Range.push_back(${index}+1);" >> $FILE
        #echo "	Reactor_Range.push_back(${REACTOR_START});" >> $FILE
        #echo "	Reactor_Range.push_back(${REACTOR_END});" >> $FILE
        #echo "	std::vector<Int_t> Steps;" >> $FILE
        #echo "	Steps.push_back(2);" >> $FILE
        #echo "	Steps.push_back(100);" >> $FILE
        if [[ $index -eq 0 ]]; then
        	echo "	Scan_U_Th_Ratio_Detail(1, 100, 100, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, true);" >> $FILE
    	else
		echo "	Scan_U_Th_Ratio_Detail($index * 100, ($index + 1) * 100 , 100, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, false);" >> $FILE
	    fi
	echo " 	return 0;" >> $FILE
	echo "}" >> $FILE
done

