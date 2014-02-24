#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/transform.h"

/**
   \class   MakeTooledJets MakeTooledJets.h "MakeTooledJets.h"

   \brief   Put all of the jet toolbox goodies into the pat::Jet. 

   \description This is an EDProducer to deswizzle all of the 
   ValueMaps produced in the RECO for jet substructure and boosted
   object tagging, adding them all to an output collection of pat::Jets. 

*/

class MakeTooledJets : public edm::EDProducer {

public:
  /// default contructor
  explicit MakeTooledJets(const edm::ParameterSet& cfg);
  /// default destructor
  ~MakeTooledJets(){};

private:
  virtual void produce( edm::Event& event, const edm::EventSetup& setup) override;

private:
  edm::EDGetTokenT<edm::View<pat::Jet> > jetsToken_;
  std::vector<edm::InputTag> doubleValueMapTags_;
  std::vector<std::string> doubleValueMapIDStrings_;
  std::vector< edm::EDGetTokenT<edm::ValueMap<double> > > doubleValueMapTokens_;
};


MakeTooledJets::MakeTooledJets(const edm::ParameterSet& cfg):
  jetsToken_(consumes<edm::View<pat::Jet> >(cfg.getParameter<edm::InputTag>("src")))
{
  doubleValueMapTags_  = cfg.getParameter<std::vector<edm::InputTag> >( "doubleValueMaps" );
  doubleValueMapIDStrings_ = cfg.getParameter< std::vector<std::string> > ("doubleValueMapIDStrings");
  doubleValueMapTokens_ = edm::vector_transform(doubleValueMapTags_, [this](edm::InputTag const & tag){return mayConsume< edm::ValueMap<double> >(tag);});
  produces< std::vector<pat::Jet> > ();
}

void
MakeTooledJets::produce( edm::Event& event, const edm::EventSetup& setup)
{
  std::auto_ptr< std::vector<pat::Jet> > outjets ( new std::vector<pat::Jet> );

  // recieve jet collection label
  edm::Handle<edm::View<pat::Jet> > jets;
  event.getByToken(jetsToken_,jets);

  // Get the vector of jet tags with b-tagging info
  std::vector<edm::Handle< edm::ValueMap<double> > > doubleValueMaps (doubleValueMapTokens_.size());

  for (size_t i = 0; i < doubleValueMapTokens_.size(); ++i) {
    event.getByToken(doubleValueMapTokens_[i], doubleValueMaps[i]);
  }
  


  for ( edm::View<pat::Jet>::const_iterator inputJet = jets->begin(),
	  inputJetBegin = inputJet, inputJetEnd = jets->end();
	inputJet != inputJetEnd; ++inputJet ) {
    edm::Ptr<reco::Candidate> const & original = inputJet->originalObjectRef();

    pat::Jet copyJet( *inputJet );

    reco::Jet const * pjet = dynamic_cast< reco::Jet const *>( &*(original.get()) );
    if ( pjet != 0 ) {

      for ( std::vector<edm::Handle< edm::ValueMap<double> > >::const_iterator imap = doubleValueMaps.begin(),
	      imapBegin = imap, imapEnd = doubleValueMaps.end(); imap != imapEnd; ++imap ) {
	double value = (**imap)[ original ];
	copyJet.addUserFloat( doubleValueMapIDStrings_[imap - imapBegin], value );

      }
      
    }
    outjets->push_back( copyJet );
  }

  event.put ( outjets );

}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MakeTooledJets);
