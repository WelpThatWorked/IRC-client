#include "Element.h"

namespace UI {

	Element::Element(SDL_Renderer* r, Window* _root, int x, int y, int w, int h) {
		_parent = NULL;
		_renderer = r;
		_x = x;
		_y = y;
		clip_x = 0;
		clip_y = 0;
		_w = w;
		_h = h;
		clip_w = 0;
		clip_h = 0;
		name = "";
		root = _root;
	}

	Element::Element(std::string name, SDL_Renderer* r, Window* _root, int x, int y, int w, int h) {
		_parent = NULL;
		_renderer = r;
		_x = x;
		_y = y;
		clip_x = 0;
		clip_y = 0;
		_w = w;
		_h = h;
		clip_w = 0;
		clip_h = 0;
		this->name = name;
		root = _root;
	}

	void Element::AddElement(Element* element) {
		if ((element->_x + element->_w > this->_w) || (element->_y + element->_h > this->_h)) return;
		_subelements.push_back(element);
		element->_parent = this;
	}

	void Element::RemoveElement(Element* element) {
		auto index = std::find(_subelements.begin(), _subelements.end(), element);
		if (index == _subelements.end()) return;
		_subelements.erase(index);
		delete element;
	}

	void Element::ClearElements() {
		for (Element* i : _subelements) {
			delete i;
		}
		_subelements.clear();
	}

	const std::vector<Element*>& Element::GetElements() const {
		return _subelements;
	}

	Element* Element::GetParent() const {
		return _parent;
	}

	void Element::SetParent(Element* elem) {
		_parent = elem;
	}

	int Element::GetTotalX() const {
		int res = _x;
		for (Element* i = _parent; i != NULL; i = i->_parent) {
			res += i->_x;
		}
		return res;
	}

	int Element::GetTotalY() const {
		int res = _y;
		for (Element* i = _parent; i != NULL; i = i->_parent) {
			res += i->_y;
		}
		return res;
	}
}