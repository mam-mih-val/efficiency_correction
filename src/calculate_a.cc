//
// Created by mikhail on 12/1/20.
//

#include <DataContainer.hpp>
#include <TFile.h>
#include <TH2F.h>
int main(){
//  auto file_mc_flow_all = TFile::Open("/home/mikhail/Correlations/mc_auau_all_2020_12_15.root");
//  auto file_mc_flow_all = TFile::Open("/home/mikhail/Correlations/botvina_all_2021_01_13.root");
  auto file_mc_flow_all = TFile::Open("/home/mikhail/Correlations/au123_all_botvina_2021_02_24.root");
  auto file_real_all = TFile::Open("/home/mikhail/OwnQnAnalysis/test-directory/auau_all_2020_12_15.root");
//  auto file_real_all = TFile::Open("/home/mikhail/OwnQnAnalysis/test-directory/auau_all_2020_12_21.root");
//  auto file_mc_flow = TFile::Open("/home/mikhail/Correlations/mc_auau_2020_12_15.root");
//  auto file_mc_flow = TFile::Open("/home/mikhail/Correlations/botvina_2021_01_13.root");
  auto file_mc_flow = TFile::Open("/home/mikhail/Correlations/au123_botvina_2021_02_24.root");
  std::map<std::string, std::string> old_name_new_name{
      {"mdc_vtx_tracks_rapidity_rapidity", "y"},
      {"mdc_vtx_tracks_rapidity_protons_rapidity", "y"},
      {"sim_tracks_rapidity_protons_rapidity", "y"},
      {"sim_tracks_rapidity", "y"},
      {"mdc_vtx_tracks_rapidity_pT", "pT"},
      {"sim_tracks_rapidity_pT", "pT"},
      {"sim_tracks_pT", "pT"},
      {"event_header_selected_tof_rpc_hits_centrality", "centrality"},
  };
  Qn::DataContainerStatCollect* v1_mc_all_container;
  Qn::DataContainerStatCalculate* v1_real_all_container;
  Qn::DataContainerStatCollect* v1_pid_container;
  Qn::DataContainerStatCollect* v1_gen_container;
  // *** Flow MC-All ***

  file_mc_flow_all->GetObject("uQ/reco/protons_PLAIN.psi_rp_PLAIN.x1cos1",
                              v1_mc_all_container);
  auto v1_y_pT_mc_all = Qn::DataContainerStatCalculate (*v1_mc_all_container);
  file_mc_flow_all->GetObject("uQ/reco/protons_PLAIN.psi_rp_PLAIN.y1sin1",
                                                                               v1_mc_all_container);
  v1_y_pT_mc_all = v1_y_pT_mc_all + Qn::DataContainerStatCalculate (*v1_mc_all_container);
  v1_y_pT_mc_all.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  // *** Flow real all ***
  file_real_all->GetObject("obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1", v1_real_all_container);
  auto v1_y_pT_real_all = Qn::DataContainerStatCalculate (*v1_real_all_container);
  v1_y_pT_real_all.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  // *** MC-GEN Flow
  file_mc_flow->GetObject("uQ/gen/gen_prim_protons_PLAIN.psi_rp_PLAIN.x1cos1", v1_gen_container);
  if( !v1_gen_container )
    file_mc_flow->GetObject("uQ/gen/gen_prim_protons_PLAIN.psi_rp_PLAIN.x1x1", v1_gen_container);
  auto v1_y_pT_gen = Qn::DataContainerStatCalculate (*v1_gen_container);
  v1_gen_container= nullptr;
  file_mc_flow->GetObject("uQ/gen/gen_prim_protons_PLAIN.psi_rp_PLAIN.y1sin1", v1_gen_container);
  if( !v1_gen_container )
    file_mc_flow->GetObject("uQ/gen/gen_prim_protons_PLAIN.psi_rp_PLAIN.y1y1", v1_gen_container);
  v1_y_pT_gen = v1_y_pT_gen + Qn::DataContainerStatCalculate (*v1_gen_container);
  v1_y_pT_gen.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  // *** MC-PID flow
  file_mc_flow->GetObject("uQ/reco/pid_protons_RESCALED.psi_rp_PLAIN.x1cos1", v1_pid_container);
  if( !v1_pid_container )
    file_mc_flow->GetObject("uQ/reco/pid_protons_RESCALED.psi_rp_PLAIN.x1x1", v1_pid_container);
  auto v1_y_pT_pid = Qn::DataContainerStatCalculate (*v1_pid_container);
  v1_pid_container= nullptr;
  file_mc_flow->GetObject("uQ/reco/pid_protons_RESCALED.psi_rp_PLAIN.y1sin1", v1_pid_container);
  if( !v1_pid_container )
    file_mc_flow->GetObject("uQ/reco/pid_protons_RESCALED.psi_rp_PLAIN.y1y1", v1_pid_container);
  v1_y_pT_pid = v1_y_pT_pid + Qn::DataContainerStatCalculate (*v1_pid_container);
  v1_y_pT_pid.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  auto& mc_all_axes = v1_y_pT_mc_all.GetAxes();
  for( auto &axis : mc_all_axes)
    axis.SetName( old_name_new_name.at(axis.Name()) );
  v1_y_pT_mc_all = v1_y_pT_mc_all.Rebin({"centrality", 6, 0.0, 60.0});
//  v1_y_pT_mc_all = v1_y_pT_mc_all.Rebin({"y", 5, -0.01, 1.49});
  auto& real_all_axes = v1_y_pT_real_all.GetAxes();
  for( auto &axis : real_all_axes)
    axis.SetName( old_name_new_name.at(axis.Name()) );
  v1_y_pT_real_all = v1_y_pT_real_all.Rebin({"centrality", 6, 0.0, 60.0});
//  v1_y_pT_real_all = v1_y_pT_real_all.Rebin({"y", 5, -0.01, 1.49});
  auto& pid_axes = v1_y_pT_pid.GetAxes();
  for( auto &axis : pid_axes )
    axis.SetName( old_name_new_name.at(axis.Name()) );
  v1_y_pT_pid = v1_y_pT_pid.Rebin({"centrality", 6, 0.0, 60.0});
//  v1_y_pT_pid = v1_y_pT_pid.Rebin({"y", 5, -0.01, 1.49});
  auto& gen_axes = v1_y_pT_gen.GetAxes();
  for( auto &axis : gen_axes )
    axis.SetName( old_name_new_name.at(axis.Name()) );
  v1_y_pT_gen = v1_y_pT_gen.Rebin({"centrality", 6, 0.0, 60.0});
//  v1_y_pT_gen = v1_y_pT_gen.Rebin({"y", 5, -0.01, 1.49});

  auto a1_container = (v1_y_pT_pid - v1_y_pT_gen) / v1_y_pT_mc_all;
  auto e1_container = a1_container * v1_y_pT_real_all;
  auto test = a1_container*v1_y_pT_mc_all;
  auto file_out = TFile::Open("a1_file.root", "recreate");
  file_out->cd("/");
  a1_container.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  a1_container.Write("a1_container");
  e1_container.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  e1_container.Write("e1_container");
  test.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  test.Write( "e1_monte_carlo" );
  file_out->Close();
  return 0;
}