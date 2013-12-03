#!/bin/bash
# This script concats all info for PA1 to an output file
# and email subject

DIR=results/
CP_SCRIPT_RES=P3_CP_Grades
FN_SCRIPT_RES=P3_FN_Grades

#INPUT=max.csv
INPUT=PA3_Grade.csv
OUTPUT=content

OLDIFS=$IFS
IFS=,

SUBJECT="[CSE100]PA3_Grade Feedback"

rm -f $OUTPUT

while read pid lname fname github email slipday grade checkpoint tutor1 finalsub tutor2 tutorname feedback 
do
    echo "<html><body><pre style=\"font-family: monospace; font-size: 12px;\">" > $OUTPUT 

    echo "Hi $fname:" >> $OUTPUT
    echo "This is an automated email given your {\"PID\": \"$pid\", \"githubID\": \"$github\"}." >> $OUTPUT
    echo >> $OUTPUT

    echo "Your PA3 grade: >>> $grade / 25 <<<" >> $OUTPUT
    echo "You've used >> $slipday << slip days so far. Please use the remaining wisely!" >> $OUTPUT
    echo >> $OUTPUT
    echo "Note1: PA3 grading has a timeout of 60 secs for each file compression." >> $OUTPUT
    echo "       Therefore, if you put too many cout<<'s for debugging, it's very likely that your program ran out of time." >> $OUTPUT
    echo "Note2: Checkpoint grading uses dummy BitStream files to compile." >> $OUTPUT
    echo "       Therefore, if your code needs your header files to compile, please contact Max." >> $OUTPUT
    echo "If you have any questions regarding your grades, KEEP CALM and contact Max. I'll try to regrade for you." >> $OUTPUT

    echo >> $OUTPUT
    echo "Max: qiw018@cs.ucsd.edu" >> $OUTPUT
    echo >> $OUTPUT

    echo "PA3 is graded based on the following rubrics:     " >> $OUTPUT
    echo "##################################################" >> $OUTPUT
    echo "Total: 25 pts                                     " >> $OUTPUT
    echo ">>> Checkpoint: 5pts                              " >> $OUTPUT
    echo " >> Get compressed file: +1                       " >> $OUTPUT
    echo " >> Compressed file looks encoded: +2             " >> $OUTPUT
    echo " >> Uncompressed = original: +2                   " >> $OUTPUT
    echo ">>> Final Submission: 20pts                       " >> $OUTPUT
    echo " >> Get compressed file: +0.5 x 10 = +5           " >> $OUTPUT
    echo " >> Uncompressed = original: +1 x 10 = +10        " >> $OUTPUT
    echo " >> Smaller compressed file than refcompress: +3  " >> $OUTPUT
    echo " >> Codying style: +2                             " >> $OUTPUT
    echo "  > header comments: +0.5                         " >> $OUTPUT
    echo "  > variable names: +0.5                          " >> $OUTPUT 
    echo "  > modularity: +0.5                              " >> $OUTPUT
    echo "  > spacing/layout: +0.5                          " >> $OUTPUT
    echo "##################################################" >> $OUTPUT

    echo >> $OUTPUT
    echo >> $OUTPUT

    echo "                    PA3 REPORT" >> $OUTPUT
    echo ">>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<" >> $OUTPUT
    echo >> $OUTPUT

    studentDir=$(find $DIR -type d -iname *$github*)
    #if [ -d $studentDir/latetwo ]
    #    then scriptDir=$studentDir/latetwo
    #    else if [ -d $studentDir/lateone ] 
    #    then scriptDir=$studentDir/lateone
    #    else scriptDir=$studentDir/ontime
    #    fi
    #fi

    if [ -f $studentDir/$CP_SCRIPT_RES ]; then 
        cat $studentDir/$CP_SCRIPT_RES >> $OUTPUT
        echo ">>> This part is manually graded by $tutorname" >> $OUTPUT
        echo "                               [Subtotal:  $tutor1 / 2 ]" >> $OUTPUT
    else 
        echo "Sorry, we couldn't find your CHECKPOINT." >> $OUTPUT
        echo "--------------------------------------------------" >> $OUTPUT
        echo "                               [Subtotal:  0 / 2 ]" >> $OUTPUT
    fi

    echo >> $OUTPUT

    if [ -f $studentDir/$FN_SCRIPT_RES ]; then 
        head -n -3 $studentDir/$FN_SCRIPT_RES >> $OUTPUT
        echo "Testing overall code styling...(2 pts)" >> $OUTPUT
        echo "--------------------------------------------------" >> $OUTPUT
        echo ">>> This part is manually graded by $tutorname" >> $OUTPUT
        echo "                               [Subtotal:  $tutor1 / 2 ]" >> $OUTPUT
    else echo "Sorry, we couldn't find your FINAL SUBMISSION." >> $OUTPUT
        echo "--------------------------------------------------" >> $OUTPUT
        echo "                              [Subtotal:  0 / 20 ]" >> $OUTPUT
    fi

    echo >> $OUTPUT

    echo "Summary:" >> $OUTPUT
    echo "==================================================" >> $OUTPUT
    echo ">>> Checkpoint:              [Subtotal:   $checkpoint /  5 ]" >> $OUTPUT
    echo ">>> Final Submission:        [Subtotal:  $finalsub / 20 ]" >> $OUTPUT
    echo ">>> Tutor Feedback: (by $tutorname)" >> $OUTPUT
    echo "    $feedback" >> $OUTPUT

    echo >> $OUTPUT

    echo ">>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<" >> $OUTPUT
    echo "                  End of Report" >> $OUTPUT

    echo >> $OUTPUT
    echo "Good luck with PA4! And your finals!" >> $OUTPUT

    echo "</pre></body></html>" >> $OUTPUT

    /usr/bin/mail -a "Content-type: text/html;" -s "$SUBJECT" "$email" < $OUTPUT
    echo "Email sent to {\"Student Name\": \"$lname, $fname\" $\"PID\": $pid, \"githubID\": $github}"

done <$INPUT

IFS=$OLDIFS
