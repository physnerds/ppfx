

#include "NucleonAbsorptionOutOfTargetReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "HistoContainer.h"
#include "ExtractInfo.h"
#include <cstdlib>

#include <iostream>

namespace NeutrinoFluxReweight{
  
  NucleonAbsorptionOutOfTargetReweighter::NucleonAbsorptionOutOfTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
  }
  NucleonAbsorptionOutOfTargetReweighter::~NucleonAbsorptionOutOfTargetReweighter(){
    
  }
  std::vector<bool> NucleonAbsorptionOutOfTargetReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    int index_vol = 0;
    double low_val = 1.E-20;    
  
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;

    bool passVOL = false;
    //Cheking at least one ancestor with amount of materail value:
    for(int index_vol =0;index_vol<3;index_vol++){
      for(int ii=0;ii<3;ii++){
	passVOL = passVOL || (vec_ptv[index_vol].AmountMat[ii] >low_val && (vec_ptv[index_vol].Pdgs[ii]==2212 || vec_ptv[index_vol].Pdgs[ii]==2112));
      }
    }
    if(passVOL)this_nodes.push_back(passVOL);
    return this_nodes;
    
  }
  double NucleonAbsorptionOutOfTargetReweighter::calculateWeight(const InteractionChainData& aa){
    
    std::map<std::string, double> dsig_table = univPars.table;
    std::map<std::string, double>::iterator it = dsig_table.begin();
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    std::string namepar = "inel_piAl_xsec";
    it = dsig_table.find(std::string(namepar));
    double shift = it->second;
    
    double NA_mb    = 6.02E-4;
    double wgt      = 1.0;
    double tot_dist = 0.0;
    double low_val  = 1.E-20;   
    
    for(int index_vol =0;index_vol<3;index_vol++){
      for(int ii=0;ii<3;ii++){	
	tot_dist = vec_ptv[index_vol].AmountMat[ii];
	if(tot_dist<low_val)continue;
	if(vec_ptv[index_vol].Pdgs[ii]!=2212 && vec_ptv[index_vol].Pdgs[ii]!=2112) continue;	
	tot_dist *= NA_mb;
	tot_dist *= shift;
	wgt *= exp(-1.0*tot_dist);   
      }
    }
    
    return wgt;
    
  }

}
