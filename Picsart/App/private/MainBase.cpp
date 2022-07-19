#include "MainBase.h"

#include <e3/ViewFactory.h>
#include "PicsartValues.h"

MainBase::MainBase(e3::Element* pParent)
	: e3::Element(pParent)
{
        this->SetOrientation((e3::EOrientation)1);
        this->SetBackgroundColor(glm::vec4(0, 0, 0, 255));
    mIconsCont = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mIconsCont);
        mIconsCont->SetWidth("100%");
        mIconsCont->SetAlignItemsHor((e3::EAlignment)3);
        mIconsCont->SetPadding("10dp");
        mIconsCont->SetBackgroundColor(glm::vec4(50));
        e3::FontIcon* pFontIcon1 = e3::ViewFactory::CreateFontIcon();
        mIconsCont->AddElement(pFontIcon1);
        pFontIcon1->SetFont(_s(Picsart/icons.ttf));
        pFontIcon1->SetWidth("14dp");
        pFontIcon1->SetAspectRatio(1.000000);
        pFontIcon1->SetColor(glm::vec4(255.000000, 255.000000, 255.000000, 255.000000));
        pFontIcon1->SetCharcode(59401);
        pFontIcon1->SetMarginLeft("10dp");
        e3::FontIcon* pFontIcon2 = e3::ViewFactory::CreateFontIcon();
        mIconsCont->AddElement(pFontIcon2);
        pFontIcon2->SetFont(_s(Picsart/icons.ttf));
        pFontIcon2->SetWidth("18dp");
        pFontIcon2->SetAspectRatio(1.000000);
        pFontIcon2->SetColor(glm::vec4(255.000000, 255.000000, 255.000000, 255.000000));
        pFontIcon2->SetCharcode(59392);
        pFontIcon2->SetMarginRight("10dp");
    mImagesCont = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mImagesCont);
        mImagesCont->SetWidth("100%");
        mImagesCont->SetHeight("100dp");
        mImagesCont->SetBackgroundColor(glm::vec4(50));
        mImagesCont->SetOverflow((e3::EOverflow)2);
    mCanvasCont = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mCanvasCont);
        mCanvasCont->SetWidth("100%");
        mCanvasCont->SetHeight("100%");
        mCanvasCont->SetMargin("20dp");
        mCanvasCont->SetOverflow((e3::EOverflow)0);
        mCanvasCont->SetScaling((e3::EScaling)1);
        mCanvas = new ImageCanvas();
        mCanvasCont->AddElement(mCanvas);
    mFilters = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mFilters);
        mFilters->SetWidth("100%");
        mFilters->SetHeight("100dp");
        mFilters->SetMarginBottom("20dp");
        mFilters->SetBackgroundColor(glm::vec4(50));
        mFilters->SetOverflow((e3::EOverflow)2);

}
