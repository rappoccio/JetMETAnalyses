#! /usr/bin/env python

import ROOT
import sys
from DataFormats.FWLite import Events, Handle

files = ["patTuple_tlbsm_train.root"]
printGen = True
events = Events (files)
handle1  = Handle ("std::vector<pat::Jet>")


# for now, label is just a tuple of strings that is initialized just
# like and edm::InputTag
label1 = ("ca8PFJetsCHSTooled")


f = ROOT.TFile("plots.root", "RECREATE")
f.cd()


# loop over events
i = 0
for event in events:
    i = i + 1
    print  '--------- Processing Event ' + str(i)


    print '---- ' + label1
    # use getByLabel, just like in cmsRun
    event.getByLabel (label1, handle1)
    # get the product
    jets1 = handle1.product()

    ijet = 0
    for jet in jets1 :
        print 'Jet {0:4.0f}, pt = {1:6.2f}, eta = {2:6.2f}, phi = {3:6.2f}, m = {4:6.2f}, m_pruned = {5:6.2f}'.format(
            ijet, jet.pt(), jet.eta(), jet.phi(), jet.mass(), jet.userFloat('prunedMass')
            )
        ijet += 1



f.cd()

f.Close()
