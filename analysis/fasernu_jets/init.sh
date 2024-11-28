export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
lsetup "root 6.32.06-x86_64-el9-gcc13-opt"
export VDT_INCLUDE_DIR=/cvmfs/sft.cern.ch/lcg/releases/vdt/0.4.4-48c41/x86_64-el9-gcc13-opt/include
export FASTJET_DIR=/cvmfs/sft.cern.ch/lcg/releases/fastjet/3.4.1-5af57/x86_64-el9-gcc13-opt
export VDT_LIBRARY=/cvmfs/sft.cern.ch/lcg/releases/vdt/0.4.4-48c41/x86_64-el9-gcc13-opt/lib
export PATH=$FASTJET_DIR/bin:$PATH
