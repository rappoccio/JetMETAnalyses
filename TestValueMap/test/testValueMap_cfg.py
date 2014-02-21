import FWCore.ParameterSet.Config as cms

process = cms.Process("Jets")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:5.1_TTbar+TTbarFS+HARVESTFS/step1.root'
  )
)

process.MessageLogger = cms.Service("MessageLogger")
from JetMETAnalyses.TestValueMap.testValueMap_cfi import testValueMap
process.ca8PFJetsCHSAna   = testValueMap.clone()

## process path
process.p = cms.Path(
    process.ca8PFJetsCHSAna
)
