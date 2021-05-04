#pragma once
#include <string>
#include <vector>
#include <SDL.h>

namespace UI {
	class Window;

	class Element {
	public:
		Element(SDL_Renderer* r, Window* _root, int x, int y, int w, int h);
		Element(std::string name, SDL_Renderer* r, Window* _root, int x, int y, int w, int h);
		~Element() {
			ClearElements();
		}
		virtual void AddElement(Element* element);
		virtual void RemoveElement(Element* element);
		virtual void ClearElements();
		const std::vector<Element*>& GetElements() const;
		Element* GetParent() const;
		void SetParent(Element* elem);
		virtual void Draw() = 0;
		virtual bool OnClick() {
			return false;
		}
		virtual void HandleEvent(SDL_Event e) {
		}
		int GetClipW() const {
			return clip_w;
		}
		int GetClipH() const {
			return clip_h;
		}
		int GetClipX() const {
			return clip_x;
		}
		int GetClipY() const {
			return clip_y;
		}
		int GetW() const {
			return _w;
		}
		int GetH() const {
			return _h;
		}
		int GetX() const {
			return _x;
		}
		int GetY() const {
			return _y;
		}
		void SetClipW(int w) {
			clip_w = w;
			for (Element* var : _subelements) {
				var->SetClipW((w - (_w - (var->GetX() + var->GetW()))) < 0 ? 0 : (w - (_w - (var->GetX() + var->GetW()))));
			}
		}
		void SetClipH(int h) {
			clip_h = h;
			for (Element* var : _subelements) {
				var->SetClipH((h - (_h - (var->GetY() + var->GetH()))) < 0 ? 0 : (h - (_h - (var->GetY() + var->GetH()))));
			}
		}
		void SetClipX(int x) {
			clip_x = x;
			for (Element* var : _subelements) {
				var->SetClipX(x - var->GetX() < 0 ? 0 : x - var->GetX());
			}
		}
		void SetClipY(int y) {
			clip_y = y;
			for (Element* var : _subelements) {
				var->SetClipY(y - var->GetY() < 0 ? 0 : y - var->GetY());
			}
		}
		void SetW(int w) {
			//if ((_x + w > _parent->_w)) return;
			_w = w;
		}
		void SetH(int h) {
			//if ((_y + h > _parent->_h)) return;
			_h = h;
		}
		void SetX(int x) {
			//if ((_h + x > _parent->_w)) return;
			_x = x;
		}
		void SetY(int y) {
			//turns out this breaks things more than It helps.
			//if ((_h + y > _parent->_h)) return;
			_y = y;
		}
		int GetTotalX() const;
		int GetTotalY() const;
		const std::string& GetName() const {
			return name;
		}
		bool active = true;
		Window* root;
	protected:
		
		std::string name;
		SDL_Renderer* _renderer;
		Element* _parent;
		std::vector<Element*> _subelements;
		int _x, _y, _w, _h, clip_x, clip_y, clip_w, clip_h;
	};
}