#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "TH1.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

/**
   \class   PatJetAnalyzer PatJetAnalyzer.h "PhysicsTools/PatAlgos/plugins/PatJetAnalyzer.h"

   \brief   Module to analyze pat::Jets in the context of a more complex exercise.

   Basic quantities of jets like the transverse momentum, eta and phi as well as the
   invariant dijet mass are plotted. Basic histograms for a jet energy response plot
   as a function of the pt of the reference object are filled. As reference matched
   partons are chosen. Input parameters are:

    - src       --> input for the pat jet collection (edm::InputTag).

    - corrLevel --> string for the pat jet correction level.
*/

class TestValueMap : public edm::EDAnalyzer {

public:
  /// default contructor
  explicit TestValueMap(const edm::ParameterSet& cfg);
  /// default destructor
  ~TestValueMap(){};

private:
  /// everything that needs to be done during the even loop
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup) override;

private:
  /// pat jets
  edm::EDGetTokenT<edm::View<reco::Jet> > jets1Token_;
  edm::EDGetTokenT<edm::View<reco::Jet> > jets2Token_;
  edm::EDGetTokenT<edm::ValueMap<double> > valueMapToken_;
};


TestValueMap::TestValueMap(const edm::ParameterSet& cfg):
  jets1Token_(consumes<edm::View<reco::Jet> >(cfg.getParameter<edm::InputTag>("src1"))),
  jets2Token_(consumes<edm::View<reco::Jet> >(cfg.getParameter<edm::InputTag>("src2"))),
  valueMapToken_(consumes<edm::ValueMap<double> >(cfg.getParameter<edm::InputTag>("srcLinks")))
{
}

void
TestValueMap::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  // recieve jet collection label
  edm::Handle<edm::View<reco::Jet> > jets1;
  event.getByToken(jets1Token_,jets1);
  edm::Handle<edm::View<reco::Jet> > jets2;
  event.getByToken(jets2Token_,jets2);
  edm::Handle<edm::ValueMap<double> > links;
  event.getByToken(valueMapToken_,links);

  std::cout << "------------------" << std::endl;
  // loop jets
  char buff[900];
  for(edm::View<reco::Jet>::const_iterator jet=jets1->begin(); jet!=jets1->end(); ++jet){
    edm::Ptr<reco::Jet> ptr = jets1->ptrAt( jet - jets1->begin() );
    double groomedMass = (*links)[ptr];
    sprintf( buff, "%6d : %6.2f %6.2f %6.2f %6.2f ---> %6.2f", int(jet - jets1->begin()), jet->pt(), jet->eta(), jet->phi(), jet->mass(), groomedMass );
    std::cout << buff << std::endl;
  }
  for(edm::View<reco::Jet>::const_iterator jet=jets2->begin(); jet!=jets2->end(); ++jet){
    sprintf( buff, "%6d : %6.2f %6.2f %6.2f %6.2f", int(jet - jets2->begin()), jet->pt(), jet->eta(), jet->phi(), jet->mass() );
    std::cout << buff << std::endl;
  }


}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TestValueMap);
