
#ifndef MainFilterItem_H_
#define MainFilterItem_H_

#include "private/MainFilterItemBase.h"

class E3_EXPORT MainFilterItem : public MainFilterItemBase
{
public:
	MainFilterItem(e3::Element* pParent = nullptr);

    ImageCanvas* GetCanvas() { return mCanvas; }
    void SetTitle(const std::string& t);
    void Check() { mCheck->SetOpacity(1); }
    void Uncheck() { mCheck->SetOpacity(0); }
};

#endif // MainFilterItem_H_

