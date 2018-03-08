#this file gives some functions for computation of the PN resolution in keVee of varous detectors
import numpy as np


def getRes(det,mode=None):
  
    if det==None:
      raise ArgumentTypeError('getRes: detector not specified')

    #get all the resolution parameters from Danika's note:
    #http://titus.stanford.edu/cdms_restricted/Soudan/R133/ebook/180219_DM/ResModel_180307.html
    respar = {}
    respar['z14'] = {}
    respar['z14']['cdmslite-70V'] = {} 
    respar['z14']['cdmslite-70V']['epF'] = 0.58 
    respar['z14']['cdmslite-70V']['A'] = 0.0095 
    respar['z14']['cdmslite-70V']['sig0'] = 8.312 
    respar['z4'] = {}
    respar['z4']['cdmslite-25V'] = {} 
    respar['z4']['cdmslite-25V']['epF'] = 0.805 
    respar['z4']['cdmslite-25V']['A'] = 0.01072 
    respar['z4']['cdmslite-25V']['sig0'] = 18.71 

    if det not in respar:
      raise ArgumentTypeError('getRes: detector not found in parameter list')

    defaultmode = {}
    defaultmode['z14'] = 'cdmslite-70V'
    defaultmode['z4'] = 'cdmslite-25V'

    if mode==None:
      mode = defaultmode[det]

    if mode not in respar[det]:
      raise ArgumentTypeError('getRes: mode not found in parameter list use: things like cdmslite-70V')

    #epF = 0.08 #fano term
    #A = 0.01 #position dependent
    #sig0 = 18.71 #baseline


    epF = respar[det][mode]['epF']
    A = respar[det][mode]['A']
    sig0 = respar[det][mode]['sig0'] 

    return lambda x: np.sqrt(sig0**2 + epF*x + (A*x)**2) 
