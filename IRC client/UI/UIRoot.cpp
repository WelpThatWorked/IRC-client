/*
*
*  UNUSED
*
*/
#include "UIRoot.h"

void Root::SetFocus(Element* elem) {
	SDL_StopTextInput();
	focused_element = elem;
}

Element* Root::GetElementByName(std::string name) {
	std::queue<Element*> q;
	q.push(this);
	while (!q.empty()) {
		Element* i = q.front();
		if (i->GetName() == name) return i;
		q.pop();
		std::vector<Element*> subelements = i->GetElements();
		for (size_t i = 0; i < subelements.size(); i++) {
			q.push(subelements[i]);
		}
	}
	return NULL;
}
