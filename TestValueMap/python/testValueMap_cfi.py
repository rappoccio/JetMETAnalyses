import FWCore.ParameterSet.Config as cms

testValueMap = cms.EDAnalyzer("TestValueMap",
  src1  = cms.InputTag("ca8PFJetsCHS"),
  src2  = cms.InputTag("ca8PFJetsCHSPruned"),
  srcLinks = cms.InputTag("ca8PFJetsCHSPrunedLinks"),
)                               
