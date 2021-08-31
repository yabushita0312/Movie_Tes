#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	//ポインタ共通
	class PTR_COMMON {
	protected:
		std::shared_ptr<MAPclass::Map> MAPPTs{ nullptr };	//引き継ぐ
		std::shared_ptr<DXDraw> DrawPts{ nullptr };			//引き継ぐ
	public:
		virtual void Set_Ptr_Common(std::shared_ptr<MAPclass::Map>& MAPPTs_t, std::shared_ptr<DXDraw>& DrawPts_t) noexcept {
			DrawPts = DrawPts_t;
			MAPPTs = MAPPTs_t;
		}
		virtual void Dispose_Ptr_Common(void) noexcept {
			DrawPts.reset();
			MAPPTs.reset();
		}
	};
};