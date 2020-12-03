//
// Created by mikhail on 12/1/20.
//

#include <DataContainer.hpp>
#include <TFile.h>
#include <TH2F.h>
int main(){
  auto file_eff = TFile::Open("/home/mikhail/hades_efficiency/cmake-build-debug/flow.root");
  auto file_mc_flow = TFile::Open("/home/mikhail/Correlations/mc_auau_2020_12_3.root");
  auto file_mc_flow_all = TFile::Open("/home/mikhail/Correlations/mc_all_2020_11_30.root");
  TH2F* e1_histo;
  file_eff->GetObject( "v1_y_pT_22", e1_histo );
  Qn::DataContainerStatCollect* v1_all_container;
  Qn::DataContainerStatCollect* v1_pid_container;
  Qn::DataContainerStatCollect* v1_gen_container;
  file_mc_flow_all->GetObject("uQ/reco/gen_all_PLAIN.psi_rp_PLAIN.x1cos1", v1_all_container);
  file_mc_flow->GetObject("uQ/reco/pid_RESCALED.psi_rp_PLAIN.x1cos1", v1_pid_container);
  file_mc_flow->GetObject("uQ/gen/gen_prim_PLAIN.psi_rp_PLAIN.x1cos1", v1_gen_container);

  auto v1_y_pT_all = Qn::DataContainerStatCalculate (*v1_all_container)*2;
  v1_y_pT_all = v1_y_pT_all.Select( {"event_header_selected_tof_rpc_hits_centrality", 1, 20.0, 25.0} );
  auto v1_y_pT_gen = Qn::DataContainerStatCalculate (*v1_gen_container)*2;
  v1_y_pT_gen = v1_y_pT_gen.Select( {"event_header_selected_tof_rpc_hits_centrality", 1, 20.0, 25.0} );
  auto v1_y_pT_pid = Qn::DataContainerStatCalculate (*v1_pid_container)*2;
  v1_y_pT_pid = v1_y_pT_pid.Select( {"event_header_selected_tof_rpc_hits_centrality", 1, 20.0, 25.0} );

  float y_axis[16];
  for(int j=0; j<16; ++j){ y_axis[j]=-0.75f+0.1f* (float) j; }
  float pt_axis[]={0, 0.29375, 0.35625, 0.41875, 0.48125, 0.54375, 0.61875, 0.70625, 0.81875, 1.01875, 2.0};
  auto a1_histo = new TH2F( "a1_histo", ";y;p_{T} [GeV/c]", 15, y_axis, 10, pt_axis );
  double y = -0.01;
  auto file_out = TFile::Open("a1_file.root", "recreate");
  file_out->mkdir("projections");
  file_out->cd("projections");
  for( int y_bin=1; y_bin <=15; ++y_bin){
    auto v1_pT_all = v1_y_pT_all.Select({"sim_tracks_rapidity_rapidity", 1, y, y+0.1});
    auto v1_pT_all_graph = Qn::ToTGraph( v1_pT_all );
    std::string name_all =  "v1_pT_all_"+std::to_string(y)+"_"+std::to_string(y+0.1);
    v1_pT_all_graph->SetName(name_all.c_str());
    v1_pT_all_graph->Write();
    auto v1_pT_pid = v1_y_pT_pid.Select({"mdc_vtx_tracks_rapidity", 1, y, y+0.1});
    auto v1_pT_pid_graph = Qn::ToTGraph( v1_pT_pid );
    std::string name_pid =  "v1_pT_pid_"+std::to_string(y)+"_"+std::to_string(y+0.1);
    v1_pT_pid_graph->SetName(name_pid.c_str());
    v1_pT_pid_graph->Write();
    auto v1_pT_gen = v1_y_pT_gen.Select({"sim_tracks_rapidity", 1, y, y+0.1});
    auto v1_pT_gen_graph = Qn::ToTGraph( v1_pT_gen );
    std::string name_gen =  "v1_pT_gen_"+std::to_string(y)+"_"+std::to_string(y+0.1);
    v1_pT_gen_graph->SetName(name_gen.c_str());
    v1_pT_gen_graph->Write();
    y+=0.1;
    for( int pT_bin=1; pT_bin<=10; pT_bin++ ){
      auto v1_all = v1_pT_all.At(pT_bin-1).Mean();
      auto v1_all_err = v1_pT_all.At(pT_bin-1).StandardErrorOfMean();
      auto v1_pid = v1_pT_pid.At(pT_bin-1).Mean();
      auto v1_pid_err = v1_pT_pid.At(pT_bin-1).StandardErrorOfMean();
      auto v1_gen = v1_pT_gen.At(pT_bin-1).Mean();
      auto v1_gen_err = v1_pT_gen.At(pT_bin-1).StandardErrorOfMean();

      auto a1 = (v1_pid - v1_gen)/v1_all;
      auto a1_err = sqrt( (v1_pid_err/v1_all)*(v1_pid_err/v1_all) + (v1_pid*v1_all_err/v1_all/v1_all)*(v1_pid*v1_all_err/v1_all/v1_all) +
          (v1_gen_err/v1_all)*(v1_gen_err/v1_all) + (v1_gen*v1_all_err/v1_all/v1_all)*(v1_gen*v1_all_err/v1_all/v1_all));
      a1_histo->SetBinContent( y_bin, pT_bin, a1 );
      a1_histo->SetBinError( y_bin, pT_bin, a1_err );
    }
  }

  file_out->cd("/");
  a1_histo->Write();
  file_out->Close();
  return 0;
}