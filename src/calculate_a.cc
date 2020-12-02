//
// Created by mikhail on 12/1/20.
//

#include <DataContainer.hpp>
#include <TFile.h>
#include <TH2F.h>
int main(){
  auto file_eff = TFile::Open("/home/mikhail/hades_efficiency/cmake-build-debug/new_fit_parameters.root");
  auto file_mc_flow_all = TFile::Open("/home/mikhail/Correlations/mc_all_2020_11_30.root");
  TH2F* e1_histo;
  file_eff->GetObject( "v1_y_pT_22", e1_histo );
  Qn::DataContainerStatCollect* v1_all_container;
  file_mc_flow_all->GetObject("uQ/reco/gen_all_PLAIN.psi_rp_PLAIN.x1cos1", v1_all_container);
  auto v1_y_pT = Qn::DataContainerStatCalculate (*v1_all_container);
  v1_y_pT = v1_y_pT*2;
  v1_y_pT = v1_y_pT.Select( {"event_header_selected_tof_rpc_hits_centrality", 1, 20.0, 25.0} );
  double y=-0.01;
  auto n_bins_y = e1_histo->GetNbinsX();
  auto n_bins_pT = e1_histo->GetNbinsY();
  auto a1_histo = (TH2F*) e1_histo->Clone("a1_histo");
  for( int y_bin =1; y_bin <=n_bins_y; y_bin++ ){
    auto v1_pT =
        v1_y_pT.Select({"sim_tracks_rapidity_rapidity", 1, y, y+0.1});
    for (int pT_bin =1; pT_bin <=n_bins_pT; pT_bin++){
      auto v1 = v1_pT.At(pT_bin -1).Mean();
      auto v1_err = v1_pT.At(pT_bin -1).StandardErrorOfMean();
      auto e1 = e1_histo->GetBinContent(y_bin, pT_bin);
      auto e1_err = e1_histo->GetBinError(y_bin, pT_bin);
      auto a1 = e1/v1;
      auto a1_err = sqrt( (e1_err/v1)*(e1_err/v1) + (e1*v1_err/v1/v1)*(e1*v1_err/v1/v1)  );
      a1_histo->SetBinContent(y_bin, pT_bin, a1);
      a1_histo->SetBinError(y_bin, pT_bin, a1_err);
    }
  }
  auto file_out = TFile::Open("a1_file.root", "recreate");
  file_out->cd();
  a1_histo->Write();
  file_out->Close();
  return 0;
}