#include "Scroller.h"

#include "Window.h"

namespace UI {
	void Scroller::AddElement(Element* elem) {
		bool snap = false;
		if (TotalHeight() <= curscrollpos + _h) snap = true;
		_subelements.push_back(elem);
		elem->SetParent(this);
		if (snap) curscrollpos = TotalHeight() - _h > 0 ? TotalHeight() - _h : 0;
		RecalculatePositions();
	}
	void Scroller::HandleEvent(SDL_Event e) {
		switch (e.type) {
		case SDL_MOUSEWHEEL:
			curscrollpos += (-50 * e.wheel.y);
			if (curscrollpos < 0) curscrollpos = 0;
			if (_h > TotalHeight()) {
				if (curscrollpos > 0) curscrollpos = 0;
			}
			else {
				if (curscrollpos > TotalHeight() - _h) curscrollpos = TotalHeight() - _h;
			}
			RecalculatePositions();
			break;
		default:

			break;
		}
	}
	bool Scroller::OnClick() {
		root->SetFocus(this);
		return true;
	}
	int Scroller::TotalHeight() const {
		int TotalY = 0;
		for (Element* elem : _subelements) {
			TotalY += elem->GetH();
		}
		return TotalY;
	}
	void Scroller::RecalculatePositions() {
		int totalY = 0;
		for (Element* var : _subelements)
		{
			if (totalY > curscrollpos + _h) {
				var->active = false;
				totalY += var->GetH();
				continue;
			}
			var->SetY(totalY - curscrollpos);
			var->SetClipY(totalY < curscrollpos ? curscrollpos - totalY : 0);
			totalY += var->GetH();
			var->SetClipH(totalY > curscrollpos + _h ? (totalY - (curscrollpos + _h)) : 0);
			if (totalY < curscrollpos) {
				var->active = false;
				continue;
			}
			var->active = true;
		}
	}
}
