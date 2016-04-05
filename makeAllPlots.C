#include <stdlib.h>
#include "plotFigures.C"

void makeAllPlots()
{
gErrorIgnoreLevel = kFatal;

for(int i=0;i<15;i++){
  plotNREDist(1,true,i+1,"ybe");
  plotNREDist(1,true,i+1,"sbbe");
}

return; 
}
