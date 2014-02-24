import FWCore.ParameterSet.Config as cms

tooledJets = cms.EDProducer("MakeTooledJets",
  src  = cms.InputTag("ca8PFJetsCHS"),
  doubleValueMaps = cms.VInputTag( ["ca8PFJetsCHSPrunedLinks" ] ),
  doubleValueMapIDStrings = cms.vstring( ['prunedMass'] )
)                               
