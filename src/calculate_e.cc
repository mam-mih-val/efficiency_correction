//
// Created by mikhail on 12/1/20.
//

#include <DataContainer.hpp>
#include <TFile.h>
#include <TH2F.h>
int main(){
  auto file_eff = TFile::Open("/home/mikhail/efficiency_correction/cmake-build-newqntools/a1_file.root");
  auto file_flow = TFile::Open("/home/mikhail/OwnQnAnalysis/test-directory/auau-all.root");
  TH2F* a1_histo;
  file_eff->GetObject( "a1_histo", a1_histo);
  Qn::DataContainerStatCalculate* v1_all_container;
  file_flow->GetObject("obs.u_RESCALED.W1_RESCALED(W3_RESCALED,Mf_protons_RESCALED).x1x1", v1_all_container);
  auto v1_y_pT = Qn::DataContainerStatCalculate (*v1_all_container);
  v1_y_pT = v1_y_pT*2;
  v1_y_pT = v1_y_pT.Select( {"event_header_selected_tof_rpc_hits_centrality", 1, 20.0, 25.0} );
  double y=-0.01;
  auto n_bins_y = a1_histo->GetNbinsX();
  auto n_bins_pT = a1_histo->GetNbinsY();
  auto e1_histo = (TH2F*)a1_histo->Clone("e1_histo");
  for( int y_bin =1; y_bin <=n_bins_y; y_bin++ ){
    auto v1_pT =
        v1_y_pT.Select({"mdc_vtx_tracks_rapidity_rapidity", 1, y, y+0.1});
    for (int pT_bin =1; pT_bin <=n_bins_pT; pT_bin++){
      auto v1 = v1_pT.At(pT_bin -1).Mean();
      auto v1_err = v1_pT.At(pT_bin -1).StandardErrorOfMean();
      auto a1 = a1_histo->GetBinContent(y_bin, pT_bin);
      auto a1_err = a1_histo->GetBinError(y_bin, pT_bin);
      auto e1 = a1*v1;
      auto e1_err = sqrt( (a1_err*v1)*(a1_err*v1) + (a1*v1_err)*(a1*v1_err)  );
      e1_histo->SetBinContent(y_bin, pT_bin, e1);
      e1_histo->SetBinError(y_bin, pT_bin, e1_err);
    }
  }
  auto file_out = TFile::Open("e1_file.root", "recreate");
  file_out->cd();
  e1_histo->Write();
  file_out->Close();
  return 0;
}