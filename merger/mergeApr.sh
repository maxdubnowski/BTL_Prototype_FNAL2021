!/bin/sh
run=${1}
echo $run
fileDRS="/Volumes/uva/testbeam_2021_apr/physics_data/Run_${run}.dat"
filePix="/Volumes/uva/testbeam_2021_apr/pixel/Run${run}_CMSTiming_FastTriggerStream_converted.root"
fileOut="Run_${run}.root"
echo $fileDRS
echo $filePix
echo $fileOut
cp $fileDRS ./drs.dat
cp $filePix ./pix.root
./maketree --inputFileName=drs.dat --pixelInputFileName=pix.root --outputFileName=out.root --nEvents=1000000
rm drs.dat pix.root
mv ./out.root /Volumes/uva/testbeam_2021_apr/merged/$fileOut
