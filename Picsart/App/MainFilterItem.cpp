#include "MainFilterItem.h"

MainFilterItem::MainFilterItem(e3::Element* pParent)
	: MainFilterItemBase(pParent)
{
    mTitleCont->SetBackgroundLinearGradient(0, glm::vec4(0), glm::vec4(0, 0, 0, 255));
}

void MainFilterItem::SetTitle(const std::string& t)
{
    mTitle->SetText(t);
}
