## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *
## switch to uncheduled mode
process.options.allowUnscheduled = cms.untracked.bool(True)
#process.Tracer = cms.Service("Tracer")

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
#process.load("RecoJets.Configuration.RecoPFJets_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("RecoJets.Configuration.GenJetParticles_cff")

process.ca8GenJetsNoNu = process.ca6GenJetsNoNu.clone( rParam = 0.8 )

## uncomment the following line to add different jet collections
## to the event content
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
from PhysicsTools.PatAlgos.tools.jetTools import switchJetCollection


addJetCollection(
   process,
   labelName = 'CA8PFCHS',
   jetSource = cms.InputTag('ca8PFJetsCHS'),
   algo='ca8',
   jetCorrections = ('AK7PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None')
   )

switchJetCollection(
   process,
   jetSource = cms.InputTag('ak5PFJets'),
   jetCorrections = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-1'),
   btagDiscriminators = [
       'jetBProbabilityBJetTags'
     , 'jetProbabilityBJetTags'
     , 'trackCountingHighPurBJetTags'
     , 'trackCountingHighEffBJetTags'
     , 'simpleSecondaryVertexHighEffBJetTags'
     , 'simpleSecondaryVertexHighPurBJetTags'
     , 'combinedSecondaryVertexBJetTags'
     ],
   )


from JetMETAnalyses.TestValueMap.makeTooledJets_cfi import tooledJets
process.ca8PFJetsCHSTooled   = tooledJets.clone(src='selectedPatJetsCA8PFCHS')


#print process.out.outputCommands

## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#                                         ##
process.source.fileNames = ['file:5.1_TTbar+TTbarFS+HARVESTFS/step1.root']
#                                         ##
process.maxEvents.input = 10
#                                         ##
process.out.outputCommands += [
   'keep *_ak5GenJetsNoNu_*_*',
   'keep *_ca8GenJetsNoNu_*_*',
   'keep *_ca8PFJetsCHSTooled_*_*'
   ]
#                                         ##
process.out.fileName = 'patTuple_tlbsm_train.root'
#                                         ##
#   process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)


