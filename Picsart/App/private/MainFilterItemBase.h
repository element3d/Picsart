
#ifndef __MainFilterItemBase_H__
#define __MainFilterItemBase_H__

#include <e3/ViewFactory.h>
#include <ImageCanvas.h>


class MainFilterItemBase : public e3::Element
{
public:
    struct __Id
    {
        
    };

    struct Id
    {
        
    };

    MainFilterItemBase(e3::Element* pParent = nullptr);

 
protected:
	ImageCanvas* mCanvas = nullptr;
e3::FontIcon* mCheck = nullptr;
e3::Element* mTitleCont = nullptr;
e3::Text* mTitle = nullptr;


};

#endif // __MainFilterItemBase_H__

