
#ifndef __MainBase_H__
#define __MainBase_H__

#include <e3/ViewFactory.h>
#include <ImageCanvas.h>


class MainBase : public e3::Element
{
public:
    struct __Id
    {
        
    };

    struct Id
    {
        
    };

    MainBase(e3::Element* pParent = nullptr);

 
protected:
	e3::Element* mIconsCont = nullptr;
e3::Element* mImagesCont = nullptr;
e3::Element* mCanvasCont = nullptr;
ImageCanvas* mCanvas = nullptr;
e3::Element* mFilters = nullptr;


};

#endif // __MainBase_H__

