#include "MainFilterItemBase.h"

#include <e3/ViewFactory.h>
#include "PicsartValues.h"

MainFilterItemBase::MainFilterItemBase(e3::Element* pParent)
	: e3::Element(pParent)
{
        this->SetAspectRatio(1.000000);
        this->SetHeight("100%");
        this->SetBorderRadius(e3::Dim("10dp"));
        this->SetBackgroundImageFit((e3::EBackgroundSize)1);
        this->SetMarginRight("4dp");
        this->SetMarginLeft("4dp");
        this->SetBackgroundColor(glm::vec4(255.000000, 255.000000, 255.000000, 255.000000));
        this->SetOverflow((e3::EOverflow)0);
        mCanvas = new ImageCanvas();
        AddElement(mCanvas);
        mCanvas->SetWidth("100%");
        mCanvas->SetHeight("100%");
        mCheck = e3::ViewFactory::CreateFontIcon();
        AddElement(mCheck);
        mCheck->SetOpacity(0.000000);
        mCheck->SetWidth("40dp");
        mCheck->SetAspectRatio(1.000000);
        mCheck->SetFont(_s(Picsart/icons.ttf));
        mCheck->SetCharcode(59392);
        mCheck->SetColor(glm::vec4(255.000000, 255.000000, 255.000000, 255.000000));
        mCheck->SetPositionType((e3::EPositionType)1);
    mTitleCont = e3::ViewFactory::CreateShape( e3::EOrientation::Horizontal);
    AddElement(mTitleCont);
        mTitleCont->SetWidth("100%");
        mTitleCont->SetPadding("2dp");
        mTitleCont->SetPositionType((e3::EPositionType)1);
        mTitleCont->SetBottom(0);
        mTitleCont->SetBackgroundColor(glm::vec4(255.000000, 255.000000, 255.000000, 255.000000));
        mTitle = e3::ViewFactory::CreateText();
        mTitleCont->AddElement(mTitle);
        mTitle->SetText("Hudson");
        mTitle->SetFontSize("14dp");
        mTitle->SetTextColor(glm::vec4(255));

}
