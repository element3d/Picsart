#include "ImageCanvasBase.h"

#include <e3/ViewFactory.h>
#include "PicsartValues.h"

ImageCanvasBase::ImageCanvasBase(e3::Element* pParent)
	: e3::Element(pParent)
{
        this->SetWidth("100%");
        this->SetHeight("100%");
        this->SetScaling((e3::EScaling)1);

}
