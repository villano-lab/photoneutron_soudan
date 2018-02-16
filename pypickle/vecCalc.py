import numpy as np


def dotProd(a,b):

  return np.dot(a,b)

def cosCalc(a,b):

  #calculate some standard angles assuming a=pvec b=rvec
  pnorm = a/np.linalg.norm(a)
  rnorm = b/np.linalg.norm(b)

  #assume top hori surface
  horiSurfNorm = [0,0,-b[2]]
  horiSurfNorm = horiSurfNorm/np.linalg.norm(horiSurfNorm)

  #assume surrounding vertical surface like cylinder wall of mumetal
  vertSurfNorm = [-b[0],-b[1],0]
  vertSurfNorm = vertSurfNorm/np.linalg.norm(vertSurfNorm)

  #get vector toward center of coord system
  vecCentNorm = [-b[0],-b[1],-b[2]]
  vecCentNorm = vecCentNorm/np.linalg.norm(vecCentNorm)

  #return costhet_3d -- cos angle between pdirection and direction to 0,0,0
  #return costhet_2dvert -- cos angle between pdirection and vertical surface norm
  #return costhet_2dhori -- cos angle between pdirection and vertical surface norm

  return np.dot(pnorm,vecCentNorm),np.dot(pnorm,vertSurfNorm),np.dot(pnorm,horiSurfNorm)
