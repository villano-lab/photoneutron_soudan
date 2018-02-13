# script by A.Anderson
# commented
# 
# 2017 May 26: modified D.Barker
# 2017 Jun 12: save out event number and energy weighted
#	       position for post processing analysis

from ROOT import TFile, TTree
#import ROOT.std
from array import array
import numpy as np
import cPickle as pickle
import sys

infilename = sys.argv[1] #'HousingImplanted_10160520_0000.root'
#infilename = '/data/chocula/barker/surfaceStudies/geant_files/withIonImp/HousingContam_withIonImp_10170526_0000.root'
infile = TFile(infilename, 'READ')

EventNum = array('d', [0.])
Edep = array('d', [0.])
x = array('d', [0.])
y = array('d', [0.])
z = array('d', [0.])
time = array('d', [0.])
ptype = array('d', [0.])
volname = array('c',list('Zip'))

# extract times and particle types from the decays tree in order
# to group hits by type
decaytree = infile.Get('G4SimDir/mcDecays')
decaytree.SetBranchAddress('EventNum', EventNum)
decaytree.SetBranchAddress('Edep', Edep)
decaytree.SetBranchAddress('X1', x)
decaytree.SetBranchAddress('Y1', y)
decaytree.SetBranchAddress('Z1', z)
decaytree.SetBranchAddress('Time1', time)
decaytree.SetBranchAddress('PType', ptype)
decaytree.SetBranchAddress('VolName', volname)
nevt = decaytree.GetEntries()
# (DB) dictionaries need keys and values: making the keys here...
decays = dict()
decays['EventNum'] = np.zeros(nevt)
decays['Edep'] = np.zeros(nevt)
decays['x'] = np.zeros(nevt)
decays['y'] = np.zeros(nevt)
decays['z'] = np.zeros(nevt)
decays['time'] = np.zeros(nevt)
decays['ptype'] = np.zeros(nevt)
decays['volname'] = np.array([], dtype=str)
# (DB) ...now filling in the values 
for jevt in range(nevt):
    decaytree.GetEntry(jevt)
    decays['EventNum'][jevt] = EventNum[0]
    decays['Edep'][jevt] = Edep[0]
    decays['x'][jevt] = x[0]
    decays['y'][jevt] = y[0]
    decays['z'][jevt] = z[0]
    decays['time'][jevt] = time[0]
    decays['ptype'][jevt] = ptype[0]
    # (DB) cannot add in strings in the same manner
    decays['volname'] = np.append(decays['volname'], volname.tostring())
    ## debugging...
    #if (jevt % 100000) == 0:
    #    print 'processing event ',jevt,' in mcDecays'
    #    print '... its volume is ',volname.tostring()
# end loop over decay tree

# now do the same for each of the zip trees
eventdata = dict()
# (DB) numpy.arange is similar to x = start:stepSize:end; in Matlab 
# now only need three zips 1 2 3
ziplist = np.arange(1,4)
#ziplist = [3]
for zipnum in ziplist:
    # debugging
    print 'processing data for zip ',zipnum
    zipname = 'mczip%d' % zipnum
    ziptree = infile.Get('G4SimDir/'+zipname)
    ziptree.SetBranchAddress('EventNum', EventNum)
    ziptree.SetBranchAddress('Edep', Edep)
    ziptree.SetBranchAddress('X1', x)
    ziptree.SetBranchAddress('Y1', y)
    ziptree.SetBranchAddress('Z1', z)
    ziptree.SetBranchAddress('Time1', time)
    ziptree.SetBranchAddress('PType', ptype)
    nevt = ziptree.GetEntries()
    # (DB) make another dictionary for the zip hits and preallocate
    # resources 
    hits = dict()
    hits['EventNum'] = np.zeros(nevt)
    hits['Edep'] = np.zeros(nevt)
    hits['x'] = np.zeros(nevt)
    hits['y'] = np.zeros(nevt)
    hits['z'] = np.zeros(nevt)
    hits['time'] = np.zeros(nevt)
    hits['ptype'] = np.zeros(nevt)
    # (DB) fill in the hit information 
    for jevt in range(nevt):
        # debugging
        if (jevt % 1000000) == 0:
            print 'Filling data for zip ',zipnum,' event ',jevt
        ziptree.GetEntry(jevt)
        hits['EventNum'][jevt] = EventNum[0]
        hits['Edep'][jevt] = Edep[0]
        hits['x'][jevt] = x[0]
        hits['y'][jevt] = y[0]
        hits['z'][jevt] = z[0]
        hits['time'][jevt] = time[0]
        hits['ptype'][jevt] = ptype[0]

    fields = ['Edep', 'EventNum', 'Xweight', 'Yweight', 'Zweight'] # (DB) I can add more fields here if I want...
    decaytypes = ['Po214', 'Pb210', 'Bi210', 'Po210']
    #decaytypes = ['Be8']
    # (DB) new dictionary, this one will eventualy be for the 
    # individual decay secondaries 
    zipdata = dict()
    nevents = np.max(ziptree.GetEntries())
    # (DB) for the case where there are not many hits in a detector
    #print 'nevents: ', nevents
    #print 'number of uniqe events: ', len(np.unique(hits['EventNum']))
    #print 'max unique event: ', np.max(np.unique(hits['EventNum']))
    if nevents < np.max(np.unique(hits['EventNum'])):
        nevents = int(np.max(np.unique(hits['EventNum']))+1)
    #print 'did my if statment work? nevents: ', nevents
    for type in decaytypes:
        zipdata[type] = dict()
        for field in fields:
            zipdata[type][field] = np.zeros(nevents)

    # (DB) numpy also has a unique function like Matlab unique
    for evnum in np.unique(hits['EventNum']):
        # debugging
        if (evnum % 10000) == 0:
            print 'uniqe event: ',evnum
        # (DB) finding the decay time and particle code for the decay
	# corresponding to the current event number 
        decaytimes = decays['time'][decays['EventNum'] == evnum]
        ptypes = decays['ptype'][decays['EventNum'] == evnum]

        # find hits in this event
        cEvent = hits['EventNum'] == evnum
        hitedep = hits['Edep'][cEvent]
        hittimes = hits['time'][cEvent]
        hitptypes = hits['ptype'][cEvent]
        # (DB) need the position info if going to do energy weighted position
        hitX = hits['x'][cEvent] # (DB) this is X1
        hitY = hits['y'][cEvent]
        hitZ = hits['z'][cEvent]
        #if (evnum % 10000) == 0:
        #    print 'hitedep: ', hitedep
        #    print 'hitX: ', hitX
        #    print 'hitY: ', hitY
        #    print 'hitZ: ', hitZ

        # all hits associated with a single decay occur within
        # about 1 second of each other, so we group hits here
        # by their times up to 1 second
	# (DB) except that for Mike's output, the times are in ns,
	# so multiplying by 1e-4 really converts to 1e-5 sec 
	# or 10 us...
        unique_decaytimes = np.unique(np.round(decaytimes * 1.e-4)) * 1.e4
        for this_time in unique_decaytimes:
            # debugging
            if (evnum % 10000) == 0:
                print 'decay time: ',this_time
	    # (DB) find the particle codes for for a particular
	    # decay time 
            this_ptypes = ptypes[(np.round(decaytimes * 1.e-4) * 1.e4) == this_time]

            # identify type of decay by the decay products
            # 210Pb decay
            cTime = np.abs(hittimes - this_time) < 1.e5
            decayStr = ''
            # only count events if the Pb210 decay occurs in the housing
	    # (DB) Po214 decays to Pb210...we want to start looking at zip
	    # hits for the Pb210 that was implanted into the housing
	        # (DB) the key is 'volname' the value seems to have some cut
		# in it...this is confusing to me; maybe like giving an
		# boolian array to a vector as the argument?
            if 'Zip' in decays['volname'][(decays['ptype'] == 210083) & (decays['EventNum'] == evnum)]: # for Po214
	    #if 'Zip' in decays['volname'][(decays['ptype'] == 4002) & (decays['EventNum'] == evnum)]: # for Be8
	        # (DB) matching the particle code of the decay
		# what are we saying? this event definitely had a 210Pb
		# decay that occured in the housing in the event, but
		# other things likely also happened in the event, like
		# the Pb210 decaying to Bi210, and it had to have 
		# originated as a Po214 decay (since that was the input
		# isotope), so we are looking at all the products of 
		# the decay chain for an event where the Pb210 (in 
		# the decay chain) was implanted into the housing
		# we do not want Pb210 that was implanted into a zip, 
		# or DIBs or lids or ...
                # debugging
                #print 'event has a implanted in zip'
                if 210082 in this_ptypes:
                    decayStr = 'Po214'
                if 210083 in this_ptypes:
                    decayStr = 'Pb210'
                if 210084 in this_ptypes:
                    decayStr = 'Bi210'
                if 206082 in this_ptypes:
                    decayStr = 'Po210'
		#if 4002 in this_ptypes:
		#    decayStr = 'Be8'
                #if (evnum % 1000) == 0:
                #    print 'evnum: ', evnum
                #    print 'time: ', np.abs(hittimes - this_time) < 1.e5
                #    print 'decay: ', decayStr
                #    print 'hitedep: ', hitedep[cTime]
                #    print 'hitX: ', hitX[cTime]
                #    print 'hitY: ', hitY[cTime]
                #    print 'hitZ: ', hitZ[cTime]
                #    print 'np.all(np.abs(hittimes - this_time) < 1.e5 == False): ', np.all((np.abs(hittimes - this_time) < 1.e5) == False)
                #    print 'np.any(np.abs(hittimes - this_time) < 1.e5 == False): ', np.any((np.abs(hittimes - this_time) < 1.e5) == False)
		# (DB) sum up the energy deposited in the zip
                #print 'EventNum: ', evnum
                #print 'decay: ', decayStr
                #print 'cTime: ', cTime
                #print 'hitedep: ', hitedep
                #print 'hitedep[cTime]: ', hitedep[cTime]
                #print 'total edep: ', np.sum(hitedep[cTime])
                zipdata[decayStr]['Edep'][int(evnum)] = np.sum(hitedep[cTime])
                # (DB) record the event number
                zipdata[decayStr]['EventNum'][int(evnum)] = evnum
                # (DB) find the x,y,z weighted position
                if np.all((np.abs(hittimes - this_time) < 1.e5) == False): 
                    zipdata[decayStr]['Xweight'][int(evnum)] = -999999
                    zipdata[decayStr]['Yweight'][int(evnum)] = -999999
                    zipdata[decayStr]['Zweight'][int(evnum)] = -999999
                    #print 'sum(edep) == [] ... X: ', hitX[cTime], ' Y: ', hitY[cTime], ' Z: ', hitZ[cTime]
                elif np.sum(hitedep[cTime]) == 0:
                    zipdata[decayStr]['Xweight'][int(evnum)] = -999999
                    zipdata[decayStr]['Yweight'][int(evnum)] = -999999
                    zipdata[decayStr]['Zweight'][int(evnum)] = -999999
                    #print 'sum(edep) == 0 ... X: ', np.mean(hitX[cTime]), ' Y: ', np.mean(hitY[cTime]), ' Z: ', np.mean(hitZ[cTime])
                else:
                    zipdata[decayStr]['Xweight'][int(evnum)] = np.sum(hitedep[cTime]*hitX[cTime])/np.sum(hitedep[cTime])
                    zipdata[decayStr]['Yweight'][int(evnum)] = np.sum(hitedep[cTime]*hitY[cTime])/np.sum(hitedep[cTime])
                    zipdata[decayStr]['Zweight'][int(evnum)] = np.sum(hitedep[cTime]*hitZ[cTime])/np.sum(hitedep[cTime])
                    #print 'sum(edep) = ', np.sum(hitedep[cTime]), ' ... X: ',  np.sum(hitedep[cTime]*hitX[cTime])/np.sum(hitedep[cTime])
                    #print '             Y: ', np.sum(hitedep[cTime]*hitY[cTime])/np.sum(hitedep[cTime]), ' Z: ', np.sum(hitedep[cTime]*hitZ[cTime])/np.sum(hitedep[cTime])
                #if (evnum % 1000) == 0:
                #    print 'G4Step X: ', hitX[cTime]
                #    print 'G4Step edep: ', hitedep[cTime]
                #    print 'weighted X: ', zipdata[decayStr]['Xweight'][int(evnum)]
                #    print 'summed edep: ', zipdata[decayStr]['Edep'][int(evnum)]

        # end loop over unique times

    # end loop over event number

    # remove events with no hits from dictionary
    # (DB) modified to remove all entries with edep == 0
    #for decayStr in zipdata.keys():
    #    for var in zipdata[decayStr].keys():
    #        zipdata[decayStr][var] = zipdata[decayStr][var][zipdata[decayStr]['Edep'] > 0]
    # (DB) save the edeps from the decays into an zipnum X events matrix 
    # for saving to a file
    eventdata[zipnum] = zipdata

# end loop over zips

outfile = file('collated_data.pkl', 'w')
pickle.dump(eventdata, outfile)
outfile.close()

#outfile = file('decays_data.pkl', 'w')
#pickle.dump(decays, outfile)
#outfile.close()

