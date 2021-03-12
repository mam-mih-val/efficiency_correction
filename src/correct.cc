//
// Created by mikhail on 11/22/20.
//

#include <DataContainer.hpp>
#include <TFile.h>
#include <TH2F.h>
#include "TCanvas.h"
#include "TMultiGraph.h"

int main(){
  auto file_correlations = TFile::Open( "/home/mikhail/OwnQnAnalysis/test-directory/auau_2020_12_15.root" );
  auto file_corrections = TFile::Open("/home/mikhail/efficiency_correction/cmake-build-newqntools/a1_file.root");
  Qn::DataContainerStatCalculate* v1_container{nullptr};
  Qn::DataContainerStatCalculate* e1_container{nullptr};
  std::map<std::string, std::string> old_name_new_name{
      {"mdc_vtx_tracks_rapidity_rapidity", "y"},
      {"sim_tracks_rapidity_rapidity", "y"},
      {"sim_tracks_rapidity", "y"},
      {"mdc_vtx_tracks_rapidity_pT", "pT"},
      {"sim_tracks_rapidity_pT", "pT"},
      {"sim_tracks_pT", "pT"},
      {"event_header_selected_tof_rpc_hits_centrality", "centrality"},
  };
  file_correlations->GetObject( "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1", v1_container );
  file_corrections->GetObject( "e1_container", e1_container );
  auto v1_pT_y_cent = *v1_container;
  auto e1_pT_y_cent = *e1_container;
  auto& axes = v1_pT_y_cent.GetAxes();
  for( auto &axis : axes)
    axis.SetName( old_name_new_name.at(axis.Name()) );
  v1_pT_y_cent = v1_pT_y_cent.Rebin({"centrality", 6, 0.0, 60.0});
//  v1_pT_y_cent = v1_pT_y_cent.Rebin({"y", 5, -0.01, 1.49});
  v1_pT_y_cent.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  auto v1_corrected = v1_pT_y_cent - e1_pT_y_cent;
  v1_corrected.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);

  auto v1_pT_container_neg = v1_pT_y_cent.Select({ "centrality", 1, 20.0, 30.0 });
  v1_pT_container_neg = v1_pT_container_neg.Select({ "y", 3, 0.74-0.25, 0.74-0.15 });
  v1_pT_container_neg = v1_pT_container_neg.Projection({ "pT"});
  auto v1_pT_neg = Qn::DataContainerHelper::ToTGraph(v1_pT_container_neg);
  v1_pT_neg->SetName("v1_-025_-015_not_corrected");
  v1_pT_neg->SetTitle("y#in(-0.45,-0.15), not corrected");

  auto v1_pT_corr_neg = v1_corrected.Select({ "centrality", 1, 20.0, 30.0 });
  v1_pT_corr_neg = v1_pT_corr_neg.Select({ "y", 3, 0.74-0.25, 0.74-0.15 });
  v1_pT_corr_neg = v1_pT_corr_neg.Projection({ "pT"});
  auto v1_pT_neg_corrected = Qn::DataContainerHelper::ToTGraph(v1_pT_corr_neg);
  v1_pT_neg_corrected->SetName("v1_-025_-015_corrected");
  v1_pT_neg_corrected->SetTitle("y#in(-0.45,-0.15), corrected");

  auto e1_pT_container_neg = e1_pT_y_cent.Select({ "centrality", 1, 20.0, 30.0 });
  e1_pT_container_neg = e1_pT_container_neg.Select({ "y", 3, 0.74-0.25, 0.74-0.15 });
  e1_pT_container_neg = e1_pT_container_neg.Projection({ "pT"});
  auto e1_pT_neg = Qn::DataContainerHelper::ToTGraph(e1_pT_container_neg);
  e1_pT_neg->SetName("e1_-025_-015");
  e1_pT_neg->SetTitle("y#in(-0.45,-0.15)");

  auto v1_pT_container_pos = v1_pT_y_cent.Select({ "centrality", 1, 20.0, 30.0 });
  v1_pT_container_pos = v1_pT_container_pos.Select({ "y", 3, 0.74+0.15, 0.74+0.25 });
  v1_pT_container_pos = v1_pT_container_pos.Projection({ "pT"});
  v1_pT_container_pos = v1_pT_container_pos*(-1.0);
  auto v1_pT_pos = Qn::DataContainerHelper::ToTGraph(v1_pT_container_pos);
  v1_pT_pos->SetName("v1_015_025_not_corrected");
  v1_pT_pos->SetTitle("y#in(0.15,0.45), not corrected");

  auto v1_pT_corr_pos = v1_corrected.Select({ "centrality", 1, 20.0, 30.0 });
  v1_pT_corr_pos = v1_pT_corr_pos.Select({ "y", 3, 0.74+0.15, 0.74+0.25 });
  v1_pT_corr_pos = v1_pT_corr_pos.Projection({ "pT"});
  v1_pT_corr_pos = v1_pT_corr_pos*(-1.0);
  auto v1_pT_pos_corrected = Qn::DataContainerHelper::ToTGraph(v1_pT_corr_pos);
  v1_pT_pos_corrected->SetName("v1_015_025_corrected");
  v1_pT_pos_corrected->SetTitle("y#in(0.15,0.45), corrected");

  auto e1_pT_container_pos = e1_pT_y_cent.Select({ "centrality", 1, 20.0, 30.0 });
  e1_pT_container_pos = e1_pT_container_pos.Select({ "y", 3, 0.74+0.15, 0.74+0.25 });
  e1_pT_container_pos = e1_pT_container_pos.Projection({ "pT"});
  e1_pT_container_pos = e1_pT_container_pos*(-1.0);
  auto e1_pT_pos = Qn::DataContainerHelper::ToTGraph(e1_pT_container_pos);
  e1_pT_pos->SetName("e1_015_025");
  e1_pT_pos->SetTitle("y#in(0.15,0.45)");



  auto canvas = new TCanvas( "corrected", "", 1000, 1100 );
  auto stack = new TMultiGraph("graphs", ";p_{T} [GeV/c];v_{1}");
  v1_pT_neg->SetLineColor(kRed);
  v1_pT_neg->SetLineWidth(3);
  v1_pT_neg_corrected->SetLineColor(kRed);
  v1_pT_neg_corrected->SetLineStyle(kDashed);
  v1_pT_neg_corrected->SetLineWidth(3);
  v1_pT_pos->SetLineColor(kBlue);
  v1_pT_pos->SetLineWidth(3);
  v1_pT_pos_corrected->SetLineColor(kBlue);
  v1_pT_pos_corrected->SetLineStyle(kDashed);
  v1_pT_pos_corrected->SetLineWidth(3);
  stack->Add(v1_pT_neg, "L");
  stack->Add(v1_pT_pos, "L");
  stack->Add(v1_pT_neg_corrected);
  stack->Add(v1_pT_pos_corrected);
  stack->Draw("AL");
  gPad->BuildLegend();
  auto file_out = TFile::Open("corrected.root", "recreate");
  v1_pT_neg->Write();
  v1_pT_pos->Write();
  e1_pT_neg->Write();
  e1_pT_pos->Write();
  v1_pT_neg_corrected->Write();
  v1_pT_pos_corrected->Write();
  canvas->Write();
  v1_corrected.Write("corrected_container");
  file_out->Close();
  return 0;
}