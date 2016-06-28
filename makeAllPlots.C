#include <stdlib.h>
#include "plotFigures.C"

void makeAllPlots()
{
gErrorIgnoreLevel = kFatal;

for(int i=0;i<15;i++){
  //plotNREDist(1,true,i+1,"ybe");
  //plotNREDist(1,true,i+1,"sbbe");
  //plotAllEDist(1,true,i+1,"lewide","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160419_0x0003.root","0x0003");
  //plotAllEDist(1,true,i+1,"ybe","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160419_0x0003.root","0x0003");
  //plotAllEDist(1,true,i+1,"lewide","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160419_0x0003.root","0x0003");
  //plotAllEDist(1,true,i+1,"sbbe","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160419_0x0003.root","0x0003");
  plotAllEDist(1,true,i+1,"lewide","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160415_0x0002.root","0x0002");
  plotAllEDist(1,true,i+1,"ybe","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160415_0x0002.root","0x0002");
  plotAllEDist(1,true,i+1,"lewide","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160415_0x0002.root","0x0002");
  plotAllEDist(1,true,i+1,"sbbe","/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160415_0x0002.root","0x0002");
}

return; 
}
