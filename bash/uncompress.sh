#!/usr/bin/env sh
# script for uncompressing files

if [ "$#" -eq "0" ]; then 
    echo "usage: ./unzip filename.zip" 
    exit 1 
fi

homedir=~/Documents/PA3
zipname=$1
repodir=${zipname%.*}
checkpointfiles="[Hcu]*"

unzip $zipname -d $homedir/$repodir
echo ">>>> level0: $zipname completed <<<"

cd $homedir/$repodir
for f in * # f = arden_liao.zip
do 
    unzip -qq -d ${f%.*} $f
    echo " >>> level1: [${f%.*}] completed <<<"
    cd ${f%.*}
    ######################
    rm *.csv
    ######################
    for ff in * # ff = P3_Pair_clarencesiu_jacobgarcia13.tar.gz
    do 
        mkdir ${ff%.tar.gz}
        tar zxf $ff -C ${ff%.tar.gz} 
        echo "  >> level2: [${ff%.tar.gz}] completed <<<"
        cd ${ff%.tar.gz}
        for fff in * # fff = P3_Pair_av3di_misterwaffles_checkpoint.tar
        do
            # ffff = checkpoint/ ontime/ lateone/ ...
            ffff=$(echo $fff | awk -F'[_.]' '{print $(NF-1)}' )
            echo $ffff | xargs -I % sh -c 'mkdir %;'
            tar xf $fff -C $ffff
            echo "   > level3: [$fff] completed <<<"
        done
        # ========================================================
        # echo ">>> start grading... "
        # echo ">>> copying checkpoint to Tester... "

        # cp -f checkpoint/$checkpointfiles $homedir/$repodir/../Tester/checkpoint/

        # # only copy the latest turnin
        # if [ -d latetwo ]; then whichone=latetwo
        # elif [ -d lateone ]; then whichone=lateone
        # else whichone=ontime
        # fi

        # echo ">>> copying final submission folder to Tester... "
        # cp -f $whichone/* $homedir/$repodir/../Tester

        # cd $homedir/$repodir/../Tester/checkpoint
        # ./compileCP
        # echo ">>> pasting back checkpoint results... "
        # cp justABC.c P3_CP_Grades $homedir/$repodir/${f%.*}/${ff%.tar.gz}
        # cd ..
        # ./compileFN
        # echo ">>> pasting back final results... "
        # cp P3_FN_Grades $homedir/$repodir/${f%.*}/${ff%.tar.gz}
        # # ========================================================
        # going back to original dir
        cd $homedir/$repodir/${f%.*}/${ff%.tar.gz}
        cd ..
    done
    cd ..
done
