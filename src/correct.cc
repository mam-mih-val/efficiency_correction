//
// Created by mikhail on 11/22/20.
//

#include <DataContainer.hpp>
#include <TFile.h>

int main(){
  auto file_correlations = TFile::Open( "/home/mikhail/Correlations/mc_2020_11_24.root" );
  auto file_true = TFile::Open( "/home/mikhail/Correlations/true_mc_2020_11_25.root" );
  auto file_corrections = TFile::Open("/home/mikhail/hades_efficiency/cmake-build-debug/new_fit_parameters.root");
  Qn::DataContainerStats* v1_container{nullptr};
  Qn::DataContainerStats* v2_container{nullptr};
  Qn::DataContainerStats* v1_true_container{nullptr};
  TH2F* e0_histo{nullptr};
  TH2F* e1_histo{nullptr};
  TH2F* e2_histo{nullptr};

  file_correlations->GetObject( "PID_No_Eff_Corr_RECENTERED_psi_rp_PLAIN_u1x_Q1x_EP", v1_container );
  file_correlations->GetObject( "PID_No_Eff_Corr_RECENTERED_psi_rp_PLAIN_Q2x_Q2x", v2_container );

  file_true->GetObject( "GEN_Prim_PLAIN_psi_rp_PLAIN_u1x_Q1x_EP", v1_true_container );

  file_corrections->GetObject( "v0_y_pT_22", e0_histo );
  file_corrections->GetObject( "v1_y_pT_22", e1_histo );
  file_corrections->GetObject( "v2_y_pT_22", e2_histo );

  auto v1_pT_container = v1_container->Select({ "Centrality", 1, 20, 25 });
  v1_pT_container = v1_pT_container.Select({ "y_cm_gen", 1, -0.25, -0.15 });
  auto v1_pT_neg = Qn::DataContainerHelper::ToTGraph(v1_pT_container);
  v1_pT_neg->SetName("v1_-025_-015_not_corrected");
  v1_pT_neg->SetTitle("y#in(-0.25,-0.15), not corrected");

  auto v2_pT_container = v2_container->Select({ "Centrality", 1, 20, 25 });
  v2_pT_container = v2_pT_container.Select({ "y_cm_gen", 1, -0.25, -0.15 });
  auto v2_pT_neg = Qn::DataContainerHelper::ToTGraph(v2_pT_container);
  v2_pT_neg->SetName("v2_-025_-015_not_corrected");

  v1_pT_container = v1_container->Select({ "Centrality", 1, 20, 25 });
  v1_pT_container = v1_pT_container.Select({ "y_cm_gen", 1, 0.15, 0.25 });
  v1_pT_container = v1_pT_container*(-1.0);
  auto v1_pT_pos = Qn::DataContainerHelper::ToTGraph(v1_pT_container);
  v1_pT_pos->SetName("v1_015_025_not_corrected");
  v1_pT_pos->SetTitle("y#in(0.15,0.25), not corrected");

  v2_pT_container = v2_container->Select({ "Centrality", 1, 20, 25 });
  v2_pT_container = v2_pT_container.Select({ "y_cm_gen", 1, 0.15, 0.25 });
  v2_pT_container = v2_pT_container*(-1.0);
  auto v2_pT_pos = Qn::DataContainerHelper::ToTGraph(v2_pT_container);
  v2_pT_pos->SetName("v2_015_025_not_corrected");

  auto e1_pT_neg = e1_histo->ProjectionY("e1_pT_y_-025_-015", 6, 6);
  auto e1_pT_pos = e1_histo->ProjectionY("e1_pT_y_015_025", 10, 10);
  e1_pT_pos->Scale(-1.0);
  auto e0_pT_neg = e0_histo->ProjectionY("e0_pT_y_-025_-015", 6, 6);
  auto e0_pT_pos = e0_histo->ProjectionY("e0_pT_y_015_025", 10, 10);
//  e0_pT_pos->Scale(-1.0);

  v1_pT_container = v1_true_container->Select({"Centrality", 1, 20, 25 });
  v1_pT_container = v1_pT_container.Select({ "y_cm_gen", 1, -0.25, -0.15 });
  auto v1_pT_true_neg = Qn::DataContainerHelper::ToTGraph(v1_pT_container);
  v1_pT_true_neg->SetName("v1_true_-025_-015");
  v1_pT_true_neg->SetTitle("y#in(-0.25,-0.15), true");
  v1_pT_container = v1_true_container->Select({"Centrality", 1, 20, 25 });
  v1_pT_container = v1_pT_container.Select({ "y_cm_gen", 1, 0.15, 0.25 });
  v1_pT_container = v1_pT_container*(-1);
  auto v1_pT_true_pos = Qn::DataContainerHelper::ToTGraph(v1_pT_container);
  v1_pT_true_pos->SetName("v1_true_015_025");
  v1_pT_true_pos->SetTitle("y#in(0.15,0.25), true");

  auto*v1_pT_neg_corrected = new TGraphErrors(10);
  v1_pT_neg_corrected->SetName("v1_-025_-015_corrected");
  v1_pT_neg_corrected->SetTitle("y#in(-0.25,-0.15), corrected");
  auto*v1_pT_pos_corrected = new TGraphErrors(10);
  v1_pT_pos_corrected->SetName("v1_015_025_corrected");
  v1_pT_pos_corrected->SetTitle("y#in(0.15,0.25), corrected");
  for( int i = 0; i < 10; ++i ){
    auto pT = e1_pT_neg->GetBinCenter(i+1);
    auto v1_neg = v1_pT_neg->GetPointY(i);
    auto v1_neg_err = v1_pT_neg->GetErrorY(i);
    auto v2_neg = v2_pT_neg->GetPointY(i);
    auto v2_neg_err = v2_pT_neg->GetErrorY(i);
    auto v1_pos = v1_pT_pos->GetPointY(i);
    auto v1_pos_err = v1_pT_pos->GetErrorY(i);
    auto v2_pos = v2_pT_pos->GetPointY(i);
    auto v2_pos_err = v2_pT_pos->GetErrorY(i);

    auto e0_neg = e0_pT_neg->GetBinContent(i+1);
    auto e0_neg_err = e0_pT_neg->GetBinError(i+1);
    auto e0_pos = e0_pT_pos->GetBinContent(i+1);
    auto e0_pos_err = e0_pT_pos->GetBinError(i+1);

    auto e1_neg = e1_pT_neg->GetBinContent(i+1);
    auto e1_neg_err = e1_pT_neg->GetBinError(i+1);
    auto e1_pos = e1_pT_pos->GetBinContent(i+1);
    auto e1_pos_err = e1_pT_pos->GetBinError(i+1);

    auto v1_neg_corr = ( v1_neg - e1_neg ); // v1 = ( v1_exp - e1( 1 + 2v2_exp - e2) ) / ( 1 + e2 - 2e1^2)
    auto v1_pos_corr = ( v1_pos - e1_pos ); // v1 = ( v1_exp - e1( 1 + 2v2_exp - e2) ) / ( 1 + e2 - 2e1^2)
//    auto v1_pos_corr = v1_pos - e1_pos;

    auto v1_neg_corr_err = sqrt(v1_neg_err * v1_neg_err + e1_neg_err * e1_neg_err + v2_neg_err*v2_neg_err );
    v1_pT_neg_corrected->SetPoint( i, pT, v1_neg_corr);
    v1_pT_neg_corrected->SetPointError(i, 0.0, v1_neg_corr_err);

    auto v1_pos_corr_err = sqrt(v1_pos_err * v1_pos_err + e1_pos_err * e1_pos_err + v2_pos_err*v2_pos_err );
    v1_pT_pos_corrected->SetPoint( i, pT, v1_pos_corr);
    v1_pT_pos_corrected->SetPointError(i, 0.0, v1_pos_corr_err);
  }

  auto file_out = TFile::Open("corrected.root", "recreate");
  v1_pT_true_neg->Write();
  v1_pT_true_pos->Write();
  v1_pT_neg->Write();
  v1_pT_pos->Write();
  v2_pT_neg->Write();
  v2_pT_pos->Write();
  e0_pT_neg->Write();
  e0_pT_pos->Write();
  e1_pT_neg->Write();
  e1_pT_pos->Write();
  v1_pT_neg_corrected->Write();
  v1_pT_pos_corrected->Write();
  file_out->Close();
  return 0;
}