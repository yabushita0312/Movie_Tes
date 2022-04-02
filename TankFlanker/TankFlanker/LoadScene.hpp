#pragma once

namespace FPS_n2 {
	namespace Sceneclass {
		class LOADDATA : public TEMPSCENE {
		private:
			std::string LOGO1 = "data/picture/logo.png";
			std::array<std::string, 3> ModelType{ "SKY_TRUE","NEAR_FALSE","FAR_TRUE" };
		private:
			std::vector<std::string> NAMES;
			ModelControl models;
			GraphControl graphs;
			LoadScriptClass LSClass;		//スクリプト読み込み
			TelopClass TLClass;				//テロップ
			std::vector<Cut_Info_First> m_CutInfo;
			std::vector<Cut_Info_Update> m_CutInfoUpdate;
			CutInfoClass attached;
			std::vector<CutAttachDetail> attachedDetail;
			Grass grassmodel;
		public:
			using TEMPSCENE::TEMPSCENE;
			void Set(void) noexcept override {
				auto* DrawParts = DXDraw::Instance();
				/*
				{
					SetUseASyncLoadFlag(FALSE);
					{
						graphs.Load(
							(float)(y_r(1920 * 1 / 2)), (float)(y_r(1080 * 1 / 2)),
							0, 1.f, 0.5f, LOGO1);
					}
					TLClass.Init();
					int mdata = FileRead_open("data/Cut.txt", FALSE);
					int cnt = 0;
					SetUseASyncLoadFlag(TRUE);
					clsDx();
					auto NowTime = GetNowHiPerformanceCount();
					auto TotalTime = GetNowHiPerformanceCount();
					while (FileRead_eof(mdata) == 0) {
						LSClass.LoadScript(getparams::get_str(mdata));
						const auto& args = LSClass.Getargs();
						const auto& func = LSClass.Getfunc();
						if (func == "") { continue; }
						//変数登録
						LSClass.SetArgs(&NAMES);
						//モデル読み込み
						if (func.find("LoadModel") != std::string::npos) {
							for (int i = 0; i < std::stoi(args[1]); i++) {
								models.Load(args[0]);
							}
						}
						//カット
						//新規カット
						if (func.find("SetCut") != std::string::npos) {
							m_CutInfo.resize(m_CutInfo.size() + 1);
							m_CutInfo.back().SetTimeLimit((LONGLONG)(1000000.f * std::stof(args[0])));
							m_CutInfoUpdate.resize(m_CutInfoUpdate.size() + 1);
						}
						//Camposの指定
						else if (func.find("SetCampos_Attach") != std::string::npos) {
							auto startFrame = (int)(m_CutInfo.size()) - 1;
							attached.Switch.resize(attached.Switch.size() + 1);
							attached.Switch.back().SetSwitch(startFrame, startFrame + (std::stoi(args[0]) - 1));
							attachedDetail.resize(attachedDetail.size() + 1);
							attachedDetail.back().poscam.Set(std::stof(args[1]), std::stof(args[2]), std::stof(args[3]));
						}
						else {
							//カメラ座標周り
							if (m_CutInfo.size() > 0) {
								if (m_CutInfo.back().LoadScript(func, args)) {
									m_CutInfoUpdate.back().CameraNotFirst = m_CutInfo.back().Aim_camera;
								}
							}
							if (m_CutInfoUpdate.size() > 0) {
								m_CutInfoUpdate.back().LoadScript(func, args);
							}
							//画像描画
							if (func.find("SetDrawGraph") != std::string::npos) {
								auto startFrame = (int)(m_CutInfo.size()) - 1;
								size_t in_str = args[1].find("~");
								if (in_str != std::string::npos) {
									int start_t = std::stoi(args[1].substr(0, in_str));
									int end_t = std::stoi(args[1].substr(in_str + 1));
									for (int i = start_t; i <= end_t; i++) {
										graphs.Get(args[0], i)->Init(startFrame, std::stoi(args[2]) - 1);
									}
								}
								else {
									graphs.Get(args[0], std::stoi(args[1]))->Init(startFrame, std::stoi(args[2]) - 1);
								}
							}
							else if (func.find("SetGraphBlur") != std::string::npos) {
								auto startFrame = (int)(m_CutInfo.size()) - 1;
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().Blur.Init(startFrame, std::stoi(args[2]) - 1);
							}
							else if (func.find("SetGraphOpacityRate") != std::string::npos) {
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().OpacityRate_Dist = std::stof(args[2]);
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().OpacityRate_Per = 0.f;
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().OpacityRate = std::stof(args[2]);
							}
							else if (func.find("SetGraphOpacityEasing") != std::string::npos) {
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().OpacityRate_Dist = std::stof(args[2]);
								graphs.Get(args[0], std::stoi(args[1]))->CutDetail.back().OpacityRate_Per = std::stof(args[3]);
							}
							//モデル描画
							else if (func.find("SetDrawModel") != std::string::npos) {
								auto startFrame = (int)(m_CutInfo.size()) - 1;
								size_t in_str = args[1].find("~");
								if (in_str != std::string::npos) {
									int start_t = std::stoi(args[1].substr(0, in_str));
									int end_t = std::stoi(args[1].substr(in_str + 1));
									for (int i = start_t; i <= end_t; i++) {
										models.Get(args[0], i)->Init(startFrame, std::stoi(args[2]) - 1);
									}
								}
								else {
									models.Get(args[0], std::stoi(args[1]))->Init(startFrame, std::stoi(args[2]) - 1);
								}
							}
							else if (func.find("SetModelAnime") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								t->CutDetail.back().animsel = std::stoi(args[2]);
								t->CutDetail.back().isloop = (args[3].find("TRUE") != std::string::npos);
								t->CutDetail.back().animspeed = std::stof(args[4]);
								t->CutDetail.back().startframe = std::stof(args[5]);
							}
							else if (func.find("SetModelMat") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								t->CutDetail.back().usemat = true;

								t->CutDetail.back().Yrad1_p = std::stof(args[2]);
								t->CutDetail.back().pos_p = VECTOR_ref::vget(std::stof(args[3]), std::stof(args[4]), std::stof(args[5]));
								t->CutDetail.back().Yrad2_p = std::stof(args[6]);

								t->CutDetail.back().mat_p = MATRIX_ref::RotY(deg2rad(t->CutDetail.back().Yrad1_p)) * MATRIX_ref::Mtrans(t->CutDetail.back().pos_p) * MATRIX_ref::RotY(deg2rad(t->CutDetail.back().Yrad2_p));
							}
							else if (func.find("SetModelPhysicsSpeed") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								t->CutDetail.back().PhysicsSpeed_ = std::stof(args[2]);
							}
							else if (func.find("SetModelOpacityRate") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								t->CutDetail.back().OpacityRate = std::stof(args[2]);
							}
							else if (func.find("SetModelOpacityEasing") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								t->CutDetail.back().OpacityRate_Dist = std::stof(args[2]);
								t->CutDetail.back().OpacityRate_Per = std::stof(args[3]);
							}
							//どの距離で描画するかをセット
							else if (func.find("SetModelMode") != std::string::npos) {
								auto* t = models.Get(args[0], std::stoi(args[1]));
								if (args[2] == ModelType[0]) {
									t->isBGModel = true;
								}
								if (args[2] == ModelType[1]) {
									t->IsNearShadow = false;
								}
								if (args[2] == ModelType[2]) {
									t->IsFarShadow = true;
								}
							}
						}
						//テロップ
						TLClass.LoadTelop(func, args);
						//END
						{
							float tim = float((GetNowHiPerformanceCount() - NowTime) / 1000) / 1000.f;
							if (tim >= 0.001f) {
								GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
							}
							printfDx("ロード%3d完了 [%7.3f s] : %s\n", cnt, tim, func.c_str());
							if (tim >= 0.001f) {
								DrawParts->Screen_Flip();
							}
							NowTime = GetNowHiPerformanceCount();
							cnt++;
						}
						if (ProcessMessage() != 0) { return; }
					}
					FileRead_close(mdata);
					SetUseASyncLoadFlag(FALSE);
					printfDx("非同期読み込みオブジェクトの読み込み待ち…\n");
					int prenum = GetASyncLoadNum(), prenumAll = prenum;
					while (ProcessMessage() == 0 && GetASyncLoadNum() != 0) {
						if (prenum != GetASyncLoadNum()) {
							prenum = GetASyncLoadNum();
							//END
							{
								float tim = float((GetNowHiPerformanceCount() - NowTime) / 1000) / 1000.f;
								if (tim >= 0.001f) {
									GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
								}
								printfDx("ロード%3d完了 [%7.3f s] : %d / %d\n", cnt, tim, prenum, prenumAll);
								if (tim >= 0.001f) {
									DrawParts->Screen_Flip();
								}
								NowTime = GetNowHiPerformanceCount();
								cnt++;
							}
							continue;
						}
					}
					if (ProcessMessage() != 0) {
						return;
					}
					//モデルの事前処理(非同期)
					{
						models.Set();
						GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
						printfDx("モデルのセット完了 : total = [%7.3f s]\n", float((GetNowHiPerformanceCount() - TotalTime) / 1000) / 1000.f);
						DrawParts->Screen_Flip();
						for (auto& n : NAMES) {
							if (n.find(".pmx") != std::string::npos) {
								MV1SaveModelToMV1File(models.Get(n, 0)->obj.get(), (n.substr(0, n.find(".pmx")) + ".mv1").c_str(), MV1_SAVETYPE_NORMAL, -1, 1, 1, 1, 0, 0);
							}
						}
						if (ProcessMessage() != 0) {
							return;
						}
					}
					//モデルのMV!保存
					{
						GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
						printfDx("モデルのMV1変換完了 : total = [%7.3f s]\n", float((GetNowHiPerformanceCount() - TotalTime) / 1000) / 1000.f);
						DrawParts->Screen_Flip();
						if (ProcessMessage() != 0) {
							return;
						}
					}
					GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
					printfDx("ロード全部完了　キーを押してください\n");
					DrawParts->Screen_Flip();
					//
				}
				grassmodel.Init();
				//*/
			}
			bool Update(void) noexcept override {
				return false;
			}
			void Dispose(void) noexcept override {

				WaitKey();
			}
			//
			void UI_Draw(void) noexcept  override {
			}
			void BG_Draw(void) noexcept override {
			}
			void Shadow_Draw_NearFar(void) noexcept override {
			}
			void Shadow_Draw(void) noexcept override {
			}
			void Main_Draw(void) noexcept override {
			}
			void LAST_Draw(void) noexcept override {
			}
		};
	};
};